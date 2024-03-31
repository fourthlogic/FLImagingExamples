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
		if(IsFail(res =  fliImage.Load(L"../../ExampleImages/Barcode/Barcode.flif")))
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

		// Barcode Decoder ��ü ���� // Create Barcode Decoder object
		CBarcodeDecoder barcodeDecoder;

		// ó���� �̹��� ����
		barcodeDecoder.SetSourceImage(fliImage);
		// Barcode Ÿ�� ����
		// �� ������ EBarcodeDecodingType_Auto �� ��� �ɺ��� Ž���Ѵ� �����Ѵ�.
		barcodeDecoder.SetBarcodeSymbolToDetect(EBarcodeSymbol_EAN13);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		
		if(IsFail(res =  barcodeDecoder.Execute()))
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
			CFLQuadD flqdRegion;

			// Barcode Decoder ����� �� Data Region �� ����
			if(IsFail(res = barcodeDecoder.GetResultDataRegion(i, flqdRegion)))
			{
				ErrorPrint(res, "Failed to get data region from the barcode decoder object.\n");
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
				ErrorPrint(res, "Failed to get decoded string from the barcode decoder object.\n");
				continue;
			}

			wprintf(L"No. %lld Code : ", i);

			// Barcode Decoder ��� �� Symbol�� ����
			CBarcodeSpec bcs;
			barcodeDecoder.GetResultBarcodeSpec(i, bcs);

			EBarcodeSymbol eSymbol = bcs.GetBarcodeSymbol();
			CFLString<wchar_t> flsSymbol;

			switch(eSymbol)
			{
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_CODE11:
				flsSymbol = L"[CODE-11]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_CODE39:
				flsSymbol = L"[CODE-39]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_Codabar:
				flsSymbol = L"[Codabar]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_Datalogic2Of5:
				flsSymbol = L"[Datalogic 2/5]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_Interleaved2Of5:
				flsSymbol = L"[Interleaved 2/5]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_Industrial2Of5:
				flsSymbol = L"[Industrial 2/5]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_MSI:
				flsSymbol = L"[MSI]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_Plessey:
				flsSymbol = L"[Plessy UK]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_UPCA:
				flsSymbol = L"[UPC-A]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_UPCE:
				flsSymbol = L"[UPC-E]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_EAN8:
				flsSymbol = L"[EAN-8]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_EAN13:
				flsSymbol = L"[EAN-13]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_EAN128:
				flsSymbol = L"[EAN-128]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_CODE93:
				flsSymbol = L"[CODE-93]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_GS1DatabarOmniTrunc:
				flsSymbol = L"[GS1 DatabarOmniTrunc]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_GS1DatabarLimited:
				flsSymbol = L"[GS1 DatabarLimited]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_GS1DatabarExpanded:
				flsSymbol = L"[GS1 DatabarExpanded]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_USPSIntelligent:
				flsSymbol = L"[USPS Intelligent]";
				break;
			case FLImaging::AdvancedFunctions::EBarcodeSymbol_JapanesePostalCustomer:
				flsSymbol = L"[Japanese Postal Customer]";
				break;
			default:
				break;
			}

			wprintf(L"%s ", flsSymbol.GetString());
			wprintf(L"%s\n", flStrWDecoded.GetString());

			// String �� ���÷��� �ϱ� ���� ���� ��ǥ FLPointL ����
			CFLPointL flplPos(flqdRegion.flpPoints[3]);

			// Decoded String �� ���÷��� �Ѵ�.
			// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
			// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
			// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
			//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
			// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
			//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
			if(IsFail(res = layer.DrawTextImage(&flqdRegion.flpPoints[0], flsSymbol, YELLOW, BLACK, 15, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				ErrorPrint(res, "Failed to draw string object on the image view.\n");
				continue;
			}
			
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