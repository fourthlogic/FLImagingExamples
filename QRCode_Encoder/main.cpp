#include <cstdio>
#include <locale.h>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	setlocale(LC_ALL, "Korean");

	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	do
	{
		CResult res = EResult_UnknownError;
		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// ���ڿ��� �Է� ���� wchar_t �� �迭 ����
		wchar_t arrWcEncoding[4096] = { 0, };

		printf("Please input encoding message.: ");

		// ���ڿ��� �Է� �޴´�
		fgetws(arrWcEncoding, 4096, stdin);

		int32_t i32StrLen = (int32_t)wcslen(arrWcEncoding);

		if(i32StrLen >= 1)
		{
			if(arrWcEncoding[i32StrLen - 1] == '\n')
			{
				if(i32StrLen >= 2 && arrWcEncoding[wcslen(arrWcEncoding) - 2] == '\r' )				
					arrWcEncoding[wcslen(arrWcEncoding) - 2] = '\0';

				arrWcEncoding[wcslen(arrWcEncoding) - 1] = '\0';
			}
			else if(arrWcEncoding[wcslen(arrWcEncoding) - 1] == '\r')
				arrWcEncoding[wcslen(arrWcEncoding) - 1] = '\0';
		}

		// QR Code Encoder ��ü ����
		CQRCodeEncoder qrCodeEncoder;
		CQRCodeSpec codeSpec;

		// ó���� �̹��� ����
		qrCodeEncoder.SetSourceImage(fliImage);
		// Encoding Message ����
		qrCodeEncoder.SetEncodingMessage(CFLString<wchar_t>(arrWcEncoding));
		// Encoder ������ ���� ���� ����
		// ù��° �Ķ���ͷ� EQRCodeColor_Custom �� �Է��ϸ� ������ ������ ��� ������ �����Ӱ� ���� �� �� �ִ�.
		codeSpec.SetColorMode(EDataCodeColor_BlackOnWhite);

		// Encoding ���� ����
		// �������� ���� �ÿ��� �⺻ ������ �����Ѵ�
		qrCodeEncoder.SetQRCodeEncodingSpec(codeSpec);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = qrCodeEncoder.Execute()))
		{
			ErrorPrint(res, "Failed to execute QR code encoder.");

			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(res = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// QR Code Decoder ��ü ���� // Create QR Code Decoder object
		CQRCodeDecoder qrCodeDecoder;

		// ó���� �̹��� ����
		qrCodeDecoder.SetSourceImage(fliImage);

		// Decode ������ ���� ���� ����
		// EQRCodeColors_Auto �� ���� �� �ڵ����� Decode �ȴ�.
		qrCodeDecoder.SetColorMode(EDataCodeColor_BlackOnWhite);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = qrCodeDecoder.Execute()))
		{
			ErrorPrint(res, "Failed to execute QR code decoder.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// QR Code Decoder ��� ������ ��´�.
		int64_t i64Results = qrCodeDecoder.GetResultCount();

		for(int64_t i = 0; i < i64Results; ++i)
		{
			// QR Code Decoder ����� ������ ���� FLQuadD ����
			CFLQuad<double> flqdRegion;

			// QR Code Decoder ����� �� Data Region �� ����
			if(IsFail(res = qrCodeDecoder.GetResultDataRegion(i, flqdRegion)))
			{
				ErrorPrint(res, "Failed to get data region from the QR code decoder object.");
				continue;
			}
		
			// QR Code �� ������ ���÷��� �Ѵ�.
			// FLImaging�� Figure ��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ���� // FLimaging's Figure objects can be displayed as a function regardless of the shape
			// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
			// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
			// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
			if(IsFail(res = layer.DrawFigureImage(&flqdRegion, LIME, 2)))
			{
				ErrorPrint(res, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// QR Code Decoder ����� ������ ���� FigureArray ����
			CFLFigureArray flfaGridRegion;

			// QR Code Decoder ����� �� Grid Region �� ����
			if(IsFail(res = qrCodeDecoder.GetResultGridRegion(i, flfaGridRegion)))
			{
				ErrorPrint(res, "Failed to get grid region from the QR code decoder object.");
				continue;
			}

			// QR Code �� Grid Region �� ���÷��� �Ѵ�.
			if(IsFail(res = layer.DrawFigureImage(&flfaGridRegion, LIME, 2)))
			{
				ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
				continue;
			}

			// QR Code Decoder ����� ������ ���� FLFigureArray ����
			CFLFigureArray flfaFinderPattern;

			// QR Code Decoder ����� �� Finder Pattern �� ����
			qrCodeDecoder.GetResultFinderPattern(i, flfaFinderPattern);

			// QR Code �� Finder Pattern �� ���÷��� �Ѵ�.
			if(IsFail(res = layer.DrawFigureImage(&flfaFinderPattern, CYAN, 5)))
			{
				ErrorPrint(res, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// QR Code Decoder ����� ������ ���� FLStringW ����
			CFLString<wchar_t> flstrDecoded;

			// QR Code Decoder ����� �� Decoded String �� ����
			if(IsFail(res = qrCodeDecoder.GetResultDecodedString(i, flstrDecoded)))
			{
				ErrorPrint(res, "Failed to get decoded string from the QR code decoder object.");
				continue;
			}

			// Decoded String �� ���÷��� �Ѵ�.
			// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
			// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
			// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
			//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
			// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
			//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic

			if(IsFail(res = layer.DrawTextImage(&flqdRegion.flpPoints[3], flstrDecoded, CYAN, BLACK, 20, false, 0., EGUIViewImageTextAlignment_LEFT_TOP)))
			{
				ErrorPrint(res, "Failed to draw string object on the image view.\n");
				continue;
			}
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}