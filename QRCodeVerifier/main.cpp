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
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/QRCode/Plate.flif")))
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

		// QR Code Verifier ��ü ���� // Create QR Code Verifier object
		CQRCodeVerifier qrCodeVerifier;

		// ROI ���� ����
		CFLRect<int32_t> flrROI(210, 60, 400, 250);

		// ó���� �̹��� ����
		qrCodeVerifier.SetSourceImage(fliImage);
		// ó���� ROI ����
		qrCodeVerifier.SetSourceROI(flrROI);
		// Decode ������ ���� ���� ����
		// EQRCodeColors_Auto �� ���� �� �ڵ����� Decode �ȴ�.
		qrCodeVerifier.SetColorMode(EDataCodeColor_WhiteOnBlack);

		// ISO/IEC 15415 ��� �μ� ǰ�� �򰡸� Ȱ��ȭ�մϴ�. �⺻���� true�̸� ó������ �ʾƵ� �˴ϴ�.
		// Enables ISO/IEC 15415 Form Print Quality Assessment. The default is true and does not require processing.
		qrCodeVerifier.EnablePrintQuality_ISOIEC_15415(true);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = qrCodeVerifier.Execute()))
		{
			ErrorPrint(res, "Failed to execute QR code decoder.");

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

		// QR Code Verifier ��� ������ ��´�.
		int64_t i64Results = qrCodeVerifier.GetResultCount();

		for(int64_t i = 0; i < i64Results; ++i)
		{

			// QR Code Verifier ����� ������ ���� FLQuadD ����
			CFLQuad<double> flqdRegion;

			// QR Code Verifier ����� �� Data Region �� ����
			if(IsFail(res = qrCodeVerifier.GetResultDataRegion(i, flqdRegion)))
			{
				ErrorPrint(res, "Failed to get data region from the QR code decoder object.");
				continue;
			}

			// QR Code �� ������ ���÷��� �Ѵ�.
			if(IsFail(res = layer.DrawFigureImage(&flqdRegion, LIME, 2)))
			{
				ErrorPrint(res, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// QR Code Verifier ����� ������ ���� FigureArray ����
			CFLFigureArray flfaGridRegion;

			// QR Code Verifier ����� �� Grid Region �� ����
			if(IsFail(res = qrCodeVerifier.GetResultGridRegion(i, flfaGridRegion)))
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

			// QR Code Verifier ����� ������ ���� FLFigureArray ����
			CFLFigureArray flfaFinderPattern;

			// QR Code Verifier ����� �� Finder Pattern �� ����
			qrCodeVerifier.GetResultFinderPattern(i, flfaFinderPattern);

			// QR Code �� Finder Pattern �� ���÷��� �Ѵ�.
			if(IsFail(res = layer.DrawFigureImage(&flfaFinderPattern, CYAN, 5)))
			{
				ErrorPrint(res, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// QR Code Verifier ����� ������ ���� FLStringW ����
			CFLString<wchar_t> flstrDecoded;

			// QR Code Verifier ����� �� Decoded String �� ����
			if(IsFail(res = qrCodeVerifier.GetResultDecodedString(i, flstrDecoded)))
			{
				ErrorPrint(res, "Failed to get decoded string from the QR code decoder object.");
				continue;
			}

			CQRCodeSpec codeSpec;
			qrCodeVerifier.GetResultQRCodeSpec(i, codeSpec);

			EQRCodeErrorCorrectionLevel eECLevel = codeSpec.GetQRCodeErrorCorrectionLevel();
			EQRCodeSymbolType1 eSymbol1 = EQRCodeSymbolType1_None;
			EQRCodeSymbolType2 eSymbol2 = EQRCodeSymbolType2_None;
			codeSpec.GetSymbolType(eSymbol1, eSymbol2);

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

			if(eSymbol1 != EQRCodeSymbolType1_None)
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

			if(eSymbol2 != EQRCodeSymbolType2_None)
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
			// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
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

			// QR Code Verifier ����� �� �μ� ǰ���� ���� // Get print quality among QR Code Verifier results
			CQRCodePrintQuality_ISOIEC_15415 printQuality;

			if(IsFail(res = qrCodeVerifier.GetResultPrintQuality_ISOIEC_15415(i, printQuality)))
			{
				ErrorPrint(res, "Failed to get print quality from the QR Code decoder object.");
				continue;
			}

			// ��� ����� ó���Ǿ��� �� Ȯ��
			if(printQuality.IsGraded())
			{
				CFLString<wchar_t> flsGrade;

				flsGrade.Format(L"[ISO/IEC 15415]\r\nDecoding Grade : %.1lf\r\nAxialNonuniformity Grade : %.1lf\r\nGridNonuniformity Grade : %.1lf\r\nSymbolContrast Grade : %.1lf\r\nUnusedErrorCorrection Grade : %.1lf\r\nModulation Grade : %.1lf\r\nFormat Information Grade : %.1lf\r\nVersion Information Grade : %.1lf\r\nFixedPatternDamage Grade : %.1lf\r\nHorizontalPrintGrowth Grade : %.1lf\r\nVerticalPrintGrowth Grade : %.1lf\r\nOverallSymbol Grade : %.1lf", printQuality.f64DecodingGrade, printQuality.f64AxialNonuniformityGrade, printQuality.f64GridNonuniformityGrade, printQuality.f64SymbolContrastGrade, printQuality.f64UnusedErrorCorrectionGrade, printQuality.f64ModulationGrade, printQuality.f64FormatInformationGrade, printQuality.f64VersionInformationGrade, printQuality.f64FixedPatternDamageGrade, printQuality.f64HorizontalPrintGrowthGrade, printQuality.f64VerticalPrintGrowthGrade, printQuality.f64OverallSymbolGrade);

				CFLRect<double> flrBoundary = flqdRegion.GetBoundaryRect();

				if(IsFail(res = layer.DrawTextImage(CFLPoint<double>(flrBoundary.left, flrBoundary.top), flsGrade, YELLOW, BLACK, 15, false, 0., EGUIViewImageTextAlignment_RIGHT_TOP)))
				{
					ErrorPrint(res, "Failed to draw string object on the image view.\n");
					continue;
				}
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