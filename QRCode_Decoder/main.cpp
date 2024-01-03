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
		CResult eResult = EResult_UnknownError;
		// �̹��� �ε� // Load image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/QRCode/Plate.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(eResult = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// QR Code Decoder ��ü ���� // Create QR Code Decoder object
		CQRCodeDecoder qrCodeDecoder;

		// ROI ���� ����
		CFLRectL flrROI(210, 60, 400, 250);

		// ó���� �̹��� ����
		qrCodeDecoder.SetSourceImage(fliImage);
		// ó���� ROI ����
		qrCodeDecoder.SetSourceROI(flrROI);
		// Decode ������ ���� ���� ����
		// EQRCodeColors_Auto �� ���� �� �ڵ����� Decode �ȴ�.
		qrCodeDecoder.SetColorMode(EDataCodeColor_WhiteOnBlack);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters

		if(IsFail(eResult = qrCodeDecoder.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute QR code decoder.");

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
		if(IsFail(eResult = layer.DrawFigureImage(&flrROI, BLUE, 3)))
		{
			ErrorPrint(eResult, "Failed to draw figures objects on the image view.\n");
			break;
		}

		// QR Code Decoder ��� ������ ��´�.
		int64_t i64Results = qrCodeDecoder.GetResultCount();

		for(int64_t i = 0; i < i64Results; ++i)
		{

			// QR Code Decoder ����� ������ ���� FLQuadD ����
			CFLQuadD flqdRegion;

			// QR Code Decoder ����� �� Data Region �� ����
			if(IsFail(eResult = qrCodeDecoder.GetResultDataRegion(i, flqdRegion)))
			{
				ErrorPrint(eResult, "Failed to get data region from the QR code decoder object.");
				continue;
			}

			// QR Code �� ������ ���÷��� �Ѵ�.
			if(IsFail(eResult = layer.DrawFigureImage(&flqdRegion, LIME, 2)))
			{
				ErrorPrint(eResult, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// QR Code Decoder ����� ������ ���� FigureArray ����
			CFLFigureArray flfaGridRegion;

			// QR Code Decoder ����� �� Grid Region �� ����
			if(IsFail(eResult = qrCodeDecoder.GetResultGridRegion(i, flfaGridRegion)))
			{
				ErrorPrint(eResult, "Failed to get grid region from the QR code decoder object.");
				continue;
			}

			// QR Code �� Grid Region �� ���÷��� �Ѵ�.
			if(IsFail(eResult = layer.DrawFigureImage(&flfaGridRegion, LIME, 2)))
			{
				ErrorPrint(eResult, "Failed to draw figure objects on the image view.\n");
				continue;
			}

			// QR Code Decoder ����� ������ ���� FLFigureArray ����
			CFLFigureArray flfaFinderPattern;

			// QR Code Decoder ����� �� Finder Pattern �� ����
			qrCodeDecoder.GetResultFinderPattern(i, flfaFinderPattern);

			// QR Code �� Finder Pattern �� ���÷��� �Ѵ�.
			if(IsFail(eResult = layer.DrawFigureImage(&flfaFinderPattern, CYAN, 5)))
			{
				ErrorPrint(eResult, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// QR Code Decoder ����� ������ ���� FLStringW ����
			CFLString<wchar_t> flstrDecoded;

			// QR Code Decoder ����� �� Decoded String �� ����
			if(IsFail(eResult = qrCodeDecoder.GetResultDecodedString(i, flstrDecoded)))
			{
				ErrorPrint(eResult, "Failed to get decoded string from the QR code decoder object.");
				continue;
			}

			CQRCodeSpec codeSpec;
			qrCodeDecoder.GetResultQRCodeSpec(i, codeSpec);

			EQRCodeErrorCorrectionLevel eECLevel = codeSpec.GetQRCodeErrorCorrectionLevel();
			EQRCodeSymbolVersion1 eSymbol1 = EQRCodeSymbolVersion1_None;
			EQRCodeSymbolVersion2 eSymbol2 = EQRCodeSymbolVersion2_None;
			codeSpec.GetQRCodeSymbolVersion(eSymbol1, eSymbol2);

			CFLString<wchar_t> flsAdditionalData;

			wprintf(L"No. %lld Code : ", i);

			switch(eECLevel)
			{
			case FLImaging::AdvancedFunctions::EQRCodeErrorCorrectionLevel_Low:
				flsAdditionalData = L"[Low";
				break;
			case FLImaging::AdvancedFunctions::EQRCodeErrorCorrectionLevel_Medium:
				flsAdditionalData = L"[Medium";
				break;
			case FLImaging::AdvancedFunctions::EQRCodeErrorCorrectionLevel_Quartile:
				flsAdditionalData = L"[Quartile";
				break;
			case FLImaging::AdvancedFunctions::EQRCodeErrorCorrectionLevel_High:
				flsAdditionalData = L"[High";
				break;
			default:
				break;
			}

			if(eSymbol1 != EQRCodeSymbolVersion1_None)
			{
				int32_t i32SymbolValue = (int32_t)eSymbol1;
				int32_t i32Symbol = 0;

				for(int32_t i = 0; i < 20; ++i)
				{
					if((i32SymbolValue >> i) & 1)
					{
						i32Symbol = i + 1;
						break;
					}
				}

				flsAdditionalData.AppendFormat(L"-%d]", i32Symbol);
			}

			if(eSymbol2 != EQRCodeSymbolVersion2_None)
			{
				int32_t i32SymbolValue = (int32_t)eSymbol2;
				int32_t i32Symbol = 0;

				for(int32_t i = 0; i < 20; ++i)
				{
					if((i32SymbolValue >> i) & 1)
					{
						i32Symbol = i + 21;
						break;
					}
				}

				flsAdditionalData.AppendFormat(L"-%d]", i32Symbol);
			}

			wprintf(L"%s\n", flstrDecoded.GetString());

			// Decoded String �� ���÷��� �Ѵ�.
			// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
			// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
			// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
			//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
			// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
			//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
			if(IsFail(eResult = layer.DrawTextImage(&flqdRegion.flpPoints[0], flsAdditionalData, YELLOW, BLACK, 20, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				ErrorPrint(eResult, "Failed to draw string object on the image view.\n");
				continue;
			}

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