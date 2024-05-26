#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	do
	{
		CResult res = EResult_UnknownError;
		// �̹��� �ε� // Load image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/MicroQRCode/FLImaging.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(res = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// MicroQR Code Decoder ��ü ���� // Create MicroQR Code Decoder object
		CMicroQRCodeDecoder qrCodeDecoder;

		// ROI ���� ����
		CFLRect<int32_t> flrROI(112, 349, 217, 454);

		// ó���� �̹��� ����
		qrCodeDecoder.SetSourceImage(fliImage);
		// ó���� ROI ����
		qrCodeDecoder.SetSourceROI(flrROI);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters

		if(IsFail(res = qrCodeDecoder.Execute()))
		{
			ErrorPrint(res, "Failed to execute MicroQR code decoder.");

			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();


		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
		// FLImaging�� Figure ��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ���� // FLimaging's Figure objects can be displayed as a function regardless of the shape
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(res = layer.DrawFigureImage(&flrROI, BLUE, 3)))
		{
			ErrorPrint(res, "Failed to draw figures objects on the image view.\n");
			break;
		}

		// MicroQR Code Decoder ��� ������ ��´�.
		int64_t i64Results = qrCodeDecoder.GetResultCount();

		for(int64_t i = 0; i < i64Results; ++i)
		{

			// MicroQR Code Decoder ����� ������ ���� FLQuadD ����
			CFLQuad<double> flqdRegion;

			// MicroQR Code Decoder ����� �� Data Region �� ����
			if(IsFail(res = qrCodeDecoder.GetResultDataRegion(i, flqdRegion)))
			{
				ErrorPrint(res, "Failed to get data region from the MicroQR code decoder object.");
				continue;
			}
		
			// MicroQR Code �� ������ ���÷��� �Ѵ�.
			if(IsFail(res = layer.DrawFigureImage(&flqdRegion, LIME, 2)))
			{
				ErrorPrint(res, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// MicroQR Code Decoder ����� ������ ���� FigureArray ����
			CFLFigureArray flfaGridRegion;

			// MicroQR Code Decoder ����� �� Grid Region �� ����
			if(IsFail(res = qrCodeDecoder.GetResultGridRegion(i, flfaGridRegion)))
			{
				ErrorPrint(res, "Failed to get grid region from the MicroQR code decoder object.");
				continue;
			}

			// MicroQR Code �� Grid Region �� ���÷��� �Ѵ�.
			if(IsFail(res = layer.DrawFigureImage(&flfaGridRegion, LIME, 2)))
			{
				ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
				continue;
			}

			// MicroQR Code Decoder ����� ������ ���� FLFigureArray ����
			CFLFigureArray flfaFinderPattern;

			// MicroQR Code Decoder ����� �� Finder Pattern �� ����
			qrCodeDecoder.GetResultFinderPattern(i, flfaFinderPattern);

			// MicroQR Code �� Finder Pattern �� ���÷��� �Ѵ�.
			if(IsFail(res = layer.DrawFigureImage(&flfaFinderPattern, CYAN, 5)))
			{
				ErrorPrint(res, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// MicroQR Code Decoder ����� ������ ���� FLStringW ����
			CFLString<wchar_t> flstrDecoded;

			// MicroQR Code Decoder ����� �� Decoded String �� ����
			if(IsFail(res = qrCodeDecoder.GetResultDecodedString(i, flstrDecoded)))
			{
				ErrorPrint(res, "Failed to get decoded string from the MicroQR code decoder object.");
				continue;
			}

			CMicroQRCodeSpec codeSpec;
			qrCodeDecoder.GetResultMicroQRCodeSpec(i, codeSpec);

			EMicroQRCodeErrorCorrectionLevel eECLevel = codeSpec.GetMicroQRCodeErrorCorrectionLevel();
			EMicroQRCodeSymbolType eSymbol = codeSpec.GetSymbolType();

			CFLString<wchar_t> flsAdditionalData;

			wprintf(L"No. %lld Code : ", i);

			switch(eECLevel)
			{
			case FLImaging::AdvancedFunctions::EMicroQRCodeErrorCorrectionLevel_None:
				flsAdditionalData = L"[None";
				break;
			case FLImaging::AdvancedFunctions::EMicroQRCodeErrorCorrectionLevel_Low:
				flsAdditionalData = L"[Low";
				break;
			case FLImaging::AdvancedFunctions::EMicroQRCodeErrorCorrectionLevel_Medium:
				flsAdditionalData = L"[Medium";
				break;
			case FLImaging::AdvancedFunctions::EMicroQRCodeErrorCorrectionLevel_Quartile:
				flsAdditionalData = L"[Quartile";
				break;
			default:
				break;
			}

			switch(eSymbol)
			{
			case FLImaging::AdvancedFunctions::EMicroQRCodeSymbolType_MicroQRVersion1:
				flsAdditionalData += L"-1]";
				break;
			case FLImaging::AdvancedFunctions::EMicroQRCodeSymbolType_MicroQRVersion2:
				flsAdditionalData += L"-2]";
				break;
			case FLImaging::AdvancedFunctions::EMicroQRCodeSymbolType_MicroQRVersion3:
				flsAdditionalData += L"-3]";
				break;
			case FLImaging::AdvancedFunctions::EMicroQRCodeSymbolType_MicroQRVersion4:
				flsAdditionalData += L"-4]";
				break;
			default:
				break;
			}

			wprintf(L"%s", flsAdditionalData.GetString());
			wprintf(L", %s\n", flstrDecoded.GetString());

			// Decoded String �� ���÷��� �Ѵ�.
			// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
			// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
			// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
			//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
			// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
			//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
			if(IsFail(res = layer.DrawTextImage(&flqdRegion.flpPoints[0], flsAdditionalData, YELLOW, BLACK, 20, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				ErrorPrint(res, "Failed to draw string object on the image view.\n");
				continue;
			}
			
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