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

		// Barcode Encoder ��ü ����
		CBarcodeEncoder barcodeEncoder;
		CBarcodeSpec codeSpec;

		// ó���� �̹��� ����
		barcodeEncoder.SetSourceImage(fliImage);
		// Encoding Barcode Ÿ�� ����
		codeSpec.SetSymbolType(EBarcodeSymbolType_EAN128);
		// Encoding Message ����
		barcodeEncoder.SetEncodingMessage(CFLString<wchar_t>(arrWcEncoding));

		// ������ Barcode ����
		barcodeEncoder.SetBarcodeEncodingSpec(codeSpec);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = barcodeEncoder.Execute()))
		{
			ErrorPrint(res, "Failed to execute barcode encoder.");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(res = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Barcode Decoder ��ü ���� // Create Barcode Decoder object
		CBarcodeDecoder barcodeDecoder;

		// ó���� �̹��� ����
		barcodeDecoder.SetSourceImage(fliImage);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = barcodeDecoder.Execute()))
		{
			ErrorPrint(res, "Failed to execute barcode decoder.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// Barcode Decoder ��� ������ ��´�.
		int64_t i64Results = barcodeDecoder.GetResultCount();

		for(int64_t i = 0; i < i64Results; ++i)
		{
			// Barcode Decoder ����� ������ ���� FLQuadD ����
			CFLQuad<double> flqdRegion;

			// Barcode Decoder ����� �� Data Region �� ����
			if(IsFail(res = barcodeDecoder.GetResultDataRegion(i, flqdRegion)))
			{
				ErrorPrint(res, "Failed to get data region from the barcode decoder object.");
				continue;
			}

			// Barcode �� ������ ���÷��� �Ѵ�.
			// FLImaging�� Figure ��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ���� // FLimaging's Figure objects can be displayed as a function regardless of the shape
			// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
			// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
			// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
			if(IsFail(res = layer.DrawFigureImage(&flqdRegion, LIME, 2, 0, EGUIViewImagePenStyle_Solid, 1, 0)))
			{
				ErrorPrint(res, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// Barcode Decoder ����� ������ ���� FLStringW ����
			CFLString<wchar_t> flStrWDecoded;

			// Barcode Decoder ����� �� Decoded String �� ����
			if(IsFail(res = barcodeDecoder.GetResultDecodedString(i, flStrWDecoded)))
			{
				ErrorPrint(res, "Failed to get decoded string from the barcode decoder object.");
				continue;
			}

			// String �� ���÷��� �ϱ� ���� ���� ��ǥ FLPointL ����
			CFLPoint<int32_t> flplPos(flqdRegion.flpPoints[3]);

			// Decoded String �� ���÷��� �Ѵ�.
			// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
			// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
			// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
			//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
			// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
			//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
			if(IsFail(res = layer.DrawTextImage(&flplPos, flStrWDecoded, CYAN, BLACK, 20)))
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