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
		CResult eResult = EResult_UnknownError;
		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
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

		// MicroQR Code Encoder ��ü ����
		CMicroQRCodeEncoder qrCodeEncoder;
		CMicroQRCodeSpec codeSpec;

		// ó���� �̹��� ����
		qrCodeEncoder.SetSourceImage(fliImage);
		// Encoding Message ����
		qrCodeEncoder.SetEncodingMessage(CFLString<wchar_t>(arrWcEncoding));
		// Encoder ������ ���� ���� ����
		// ������ ���� ���� �� �⺻���� EDataCodeColor_BlackOnWhite �� �����Ѵ�.
		codeSpec.SetColorMode(EDataCodeColor_BlackOnWhite);

		// Encoding ���� ����
		// �������� ���� �ÿ��� �⺻ ������ �����Ѵ�
		qrCodeEncoder.SetMicroQRCodeEncodingSpec(codeSpec);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = qrCodeEncoder.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute MicroQR code encoder.");

			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(eResult = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// MicroQR Code Decoder ��ü ���� // Create MicroQR Code Decoder object
		CMicroQRCodeDecoder qrCodeDecoder;

		// ó���� �̹��� ����
		qrCodeDecoder.SetSourceImage(fliImage);

		// Decode ������ ���� ���� ����
		// EMicroQRCodeColors_Auto �� ���� �� �ڵ����� Decode �ȴ�.
		qrCodeDecoder.SetColorMode(EDataCodeColor_BlackOnWhite);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = qrCodeDecoder.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute MicroQR code decoder.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// MicroQR Code Decoder ��� ������ ��´�.
		int64_t i64Results = qrCodeDecoder.GetResultCount();

		for(int64_t i = 0; i < i64Results; ++i)
		{
			// MicroQR Code Decoder ����� ������ ���� FLQuadD ����
			CFLQuadD flqdRegion;

			// MicroQR Code Decoder ����� �� Data Region �� ����
			if(IsFail(eResult = qrCodeDecoder.GetResultDataRegion(i, flqdRegion)))
			{
				ErrorPrint(eResult, "Failed to get data region from the MicroQR code decoder object.");
				continue;
			}
		
			// MicroQR Code �� ������ ���÷��� �Ѵ�.
			// FLImaging�� Figure ��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ���� // FLimaging's Figure objects can be displayed as a function regardless of the shape
			// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
			// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
			// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
			if(IsFail(eResult = layer.DrawFigureImage(&flqdRegion, LIME, 2)))
			{
				ErrorPrint(eResult, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// MicroQR Code Decoder ����� ������ ���� FigureArray ����
			CFLFigureArray flfaGridRegion;

			// MicroQR Code Decoder ����� �� Grid Region �� ����
			if(IsFail(eResult = qrCodeDecoder.GetResultGridRegion(i, flfaGridRegion)))
			{
				ErrorPrint(eResult, "Failed to get grid region from the MicroQR code decoder object.");
				continue;
			}

			// MicroQR Code �� Grid Region �� ���÷��� �Ѵ�.
			if(IsFail(eResult = layer.DrawFigureImage(&flfaGridRegion, LIME, 2)))
			{
				ErrorPrint(eResult, "Failed to draw figure objects on the image view.\n");
				continue;
			}

			// MicroQR Code Decoder ����� ������ ���� FLFigureArray ����
			CFLFigureArray flfaFinderPattern;

			// MicroQR Code Decoder ����� �� Finder Pattern �� ����
			qrCodeDecoder.GetResultFinderPattern(i, flfaFinderPattern);

			// MicroQR Code �� Finder Pattern �� ���÷��� �Ѵ�.
			if(IsFail(eResult = layer.DrawFigureImage(&flfaFinderPattern, CYAN, 5)))
			{
				ErrorPrint(eResult, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// MicroQR Code Decoder ����� ������ ���� FLStringW ����
			CFLString<wchar_t> flstrDecoded;

			// MicroQR Code Decoder ����� �� Decoded String �� ����
			if(IsFail(eResult = qrCodeDecoder.GetResultDecodedString(i, flstrDecoded)))
			{
				ErrorPrint(eResult, "Failed to get decoded string from the MicroQR code decoder object.");
				continue;
			}

			// Decoded String �� ���÷��� �Ѵ�.
			// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
			// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
			// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
			//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
			// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
			//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic

			if(IsFail(eResult = layer.DrawTextImage(&flqdRegion.flpPoints[3], flstrDecoded, CYAN, BLACK, 20, false, 0., EGUIViewImageTextAlignment_LEFT_TOP)))
			{
				ErrorPrint(eResult, "Failed to draw string object on the image view.\n");
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