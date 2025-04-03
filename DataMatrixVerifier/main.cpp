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
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/DataMatrix/Module.flif")))
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

		// Data Matrix Verifier ��ü ���� // Create Data Matrix Verifier object
		CDataMatrixVerifier dataMatrixVerifier;

		// ROI ���� ����
		CFLRect<int32_t> flrROI(150, 25, 475, 300);

		// ó���� �̹��� ����
		dataMatrixVerifier.SetSourceImage(fliImage);
		// ó���� ROI ����
		dataMatrixVerifier.SetSourceROI(flrROI);
		// Decode ������ ���� ���� ����
		// EDataCodeColor_Auto �� ���� �� �ڵ����� Decode �ȴ�.
		dataMatrixVerifier.SetColorMode(EDataCodeColor_BlackOnWhite);

		// ISO/IEC 15415 ��� �μ� ǰ�� �򰡸� Ȱ��ȭ�մϴ�. �⺻���� true�̸� ó������ �ʾƵ� �˴ϴ�.
		// Enables ISO/IEC 15415 Form Print Quality Assessment. The default is true and does not require processing.
		dataMatrixVerifier.EnablePrintQuality_ISOIEC_15415(true);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = dataMatrixVerifier.Execute()))
		{
			ErrorPrint(res, "Failed to execute data matrix decoder.");

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
		if(IsFail(res = layer.DrawFigureImage(&flrROI, BLUE, 3, 0, EGUIViewImagePenStyle_Solid, 1, 0)))
		{
			ErrorPrint(res, "Failed to draw figures objects on the image view.\n");
			break;
		}

		// Data Matrix Verifier ��� ������ ��´�.
		int64_t i64Results = dataMatrixVerifier.GetResultCount();

		for(int64_t i = 0; i < i64Results; ++i)
		{
			// Data Matrix Verifier ����� ������ ���� FLQuadD ����
			CFLQuad<double> flqdRegion;

			// Data Matrix Verifier ����� �� Data Region �� ����
			if(IsFail(res = dataMatrixVerifier.GetResultDataRegion(i, flqdRegion)))
			{
				ErrorPrint(res, "Failed to get data region from the data matrix decoder object.");
				continue;
			}

			// Data Matrix �� ������ ���÷��� �Ѵ�.
			if(IsFail(res = layer.DrawFigureImage(&flqdRegion, LIME, 2)))
			{
				ErrorPrint(res, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// Data Matrix Verifier ����� ������ ���� FigureArray ����
			CFLFigureArray flfaGridRegion;

			// Data Matrix Verifier ����� �� Grid Region �� ����
			if(IsFail(res = dataMatrixVerifier.GetResultGridRegion(i, flfaGridRegion)))
			{
				ErrorPrint(res, "Failed to get grid region from the data matrix decoder object.");
				continue;
			}

			// Data Matrix �� Grid Region �� ���÷��� �Ѵ�.
			if(IsFail(res = layer.DrawFigureImage(&flfaGridRegion, LIME, 2)))
			{
				ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
				continue;
			}

			// Data Matrix Verifier ����� ������ ���� FLStringW ����
			CFLString<wchar_t> flstrDecoded;

			// Data Matrix Verifier ����� �� Decoded String �� ����
			if(IsFail(res = dataMatrixVerifier.GetResultDecodedString(i, flstrDecoded)))
			{
				ErrorPrint(res, "Failed to get decoded string from the data matrix decoder object.");
				continue;
			}

			wprintf(L"No. %lld Code : ", i);
			wprintf(L"%s\n", flstrDecoded.GetString());

			// String �� ���÷��� �ϱ� ���� ���� ��ǥ FLPointL ����
			CFLPoint<int32_t> flplPos(flqdRegion.flpPoints[3]);

			// Decoded String �� ���÷��� �Ѵ�.
			// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
			// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
			// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
			//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
			// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
			//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
			if(IsFail(res = layer.DrawTextImage(&flplPos, flstrDecoded, CYAN, BLACK, 20, false, flqdRegion.flpPoints[3].GetAngle(flqdRegion.flpPoints[2]))))
			{
				ErrorPrint(res, "Failed to draw string object on the image view.\n");
				continue;
			}

			// Data Matrix Verifier ����� �� �μ� ǰ���� ���� // Get print quality among Data Matrix Verifier results
			CPrintQuality_ISOIEC_15415 printQuality;

			if(IsFail(res = dataMatrixVerifier.GetResultPrintQuality_ISOIEC_15415(i, printQuality)))
			{
				ErrorPrint(res, "Failed to get print quality from the data matrix decoder object.");
				continue;
			}

			// ��� ����� ó���Ǿ��� �� Ȯ��
			if(printQuality.IsGraded())
			{
				CFLString<wchar_t> flsGrade;

				flsGrade.Format(L"[ISO/IEC 15415]\r\nDecoding Grade : %.1lf\r\nAxialNonuniformity Grade : %.1lf\r\nGridNonuniformity Grade : %.1lf\r\nSymbolContrast Grade : %.1lf\r\nUnusedErrorCorrection Grade : %.1lf\r\nModulation Grade : %.1lf\r\nFixedPatternDamage Grade : %.1lf\r\nHorizontalPrintGrowth Grade : %.1lf\r\nVerticalPrintGrowth Grade : %.1lf\r\nOverallSymbol Grade : %.1lf", printQuality.f64DecodingGrade, printQuality.f64AxialNonuniformityGrade, printQuality.f64GridNonuniformityGrade, printQuality.f64SymbolContrastGrade, printQuality.f64UnusedErrorCorrectionGrade, printQuality.f64ModulationGrade, printQuality.f64FixedPatternDamageGrade, printQuality.f64HorizontalPrintGrowthGrade, printQuality.f64VerticalPrintGrowthGrade, printQuality.f64OverallSymbolGrade);

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