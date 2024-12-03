#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	const int32_t i32ExampleCount = 3;
	const wchar_t* arrWcsViewText[] =
	{
		L"Dark To Bright To Dark\nClosest",
		L"Bright To Dark To Bright\nClosest",
		L"Dark To Bright To Dark Or Bright To Dark To Bright\nClosest",
	};

	CCrossGauge::ETransitionType arrTransitionType[] =
	{
		CCrossGauge::ETransitionType_DarkToBrightToDark,
		CCrossGauge::ETransitionType_BrightToDarkToBright,
		CCrossGauge::ETransitionType_DarkToBrightToDarkOrBrightToDarkToBright,
	};

	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliImage[i32ExampleCount];

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage[i32ExampleCount];
	CResult res;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(res = fliImage[0].Load(L"../../ExampleImages/Gauge/Cross_Bright.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliImage[1].Load(L"../../ExampleImages/Gauge/Cross_Dark.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliImage[2].Load(L"../../ExampleImages/Gauge/Cross_Dark_Bright.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates imageview
		for(int32_t i = 0; i < i32ExampleCount; ++i)
		{
			int32_t i32X = 400 * (i % 4);

			if(IsFail(res = viewImage[i].Create(i32X, 0, i32X + 400, 400)))
			{
				ErrorPrint(res, "Failed to create the image view.\n");
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
			if(IsFail(res = viewImage[i].SetImagePtr(&fliImage[i])))
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				break;
			}

			// �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the all image views. 
			if(i)
			{
				if(IsFail(res = viewImage[i].SynchronizePointOfView(&viewImage[0])))
				{
					ErrorPrint(res, "Failed to set image object on the image view.\n");
					break;
				}
			}
		}

		CResult res = EResult_UnknownError;

		// Cross Gauge ��ü ���� // Create Cross Gauge object
		CCrossGauge CrossGauge;

		// ������ ������ �����մϴ�. // Set the area to measure.
		CFLRect<double> measureRegion(126., 126., 400., 400.);
		double tolerance = 70.;
		CrossGauge.SetMeasurementRegion(measureRegion, tolerance);

		// �����ϱ����� �Ķ���͸� �����մϴ�. // Set parameters for extraction.		
		// �������� �����ϱ����� ������ ����� �� ����� ����� ������ �����մϴ�. // Select the boundary point type to use among the boundary points extracted to estimate the crosshair.
		CrossGauge.SetTransitionChoice(CCrossGauge::ETransitionChoice_Closest);
		// �������� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� ���� �����մϴ�. // Set the threshold change of the boundary point to be extracted to estimate the crosshair.
		CrossGauge.SetThreshold(20);
		// �������� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� �������� �����մϴ�. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the crosshair.
		CrossGauge.SetMinimumAmplitude(10);
		// �������� �����ϱ����� ������ ��������� ��ǥ�� ǥ�� ������ �����մϴ�. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the crosshairs.
		CrossGauge.SetThickness(1);
		// �������� �����ϱ����� ������ ��������� ���� ������ �����մϴ�. // Set the extraction interval of boundary points to be extracted to estimate the crosshair.
		CrossGauge.SetSamplingStep(1.);
		// �������� �����ϱ����� ������ ��������� �̻�ġ ������ ���� �Ӱ谪�� �����մϴ�. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the crosshairs.
		CrossGauge.SetOutliersThreshold(3.);
		// �������� �����ϱ����� ������ ��������� �̻�ġ ���� Ƚ���� �����մϴ�. // Set the number of outlier adjustments for boundary points to be extracted to estimate the crosshairs.
		CrossGauge.SetOutliersThresholdCount(3);
		// �������� �����ϱ����� �� Ŭ�����͸� ó�� ������ ���� ������ �մϴ�. // Set whether or not to process point clustering to estimate the crosshairs.
		CrossGauge.EnableClusterMode(true);
		// �������� �����ϱ����� ������ �����մϴ�. �ʿ信 ���� �� �������� ���������մϴ�. // Set the margin to estimate the crosshairs. It can be set for each zone as needed.
		CrossGauge.SetMeasurementMarginRatio(0.5, 0.1);

		for(int32_t i = 0; i < i32ExampleCount; ++i)
		{
			// ó���� �̹��� ���� // Set the image to process
			CrossGauge.SetSourceImage(fliImage[i]);

			// �������� �����ϱ����� ������ ����� ��ȭ ���⿡ ���� �����մϴ�. // Set the boundary point change direction to extract to estimate the crosshair.
			CrossGauge.SetTransitionType(arrTransitionType[i]);

			// �˰��� ���� // Execute the algorithm
			if(IsFail(res = CrossGauge.Execute()))
			{
				ErrorPrint(res, "Failed to execute Cross gauge.\n");
				break;
			}

			// ���� ����� �����ɴϴ�. // Get the execution result.
			CFLPoint<double> resultRegion;
			CFLFigureArray flfaResultLine;
			CFLFigureArray flfaResultsValid, flfaResultsInvalid;
			// ������ ���� ������ �����ɴϴ�. // Get the line for inference.
			CrossGauge.GetMeasuredLines(flfaResultLine);
			// ������ �������� �����ɴϴ�. // Get the estimated crosshairs.
			CrossGauge.GetMeasuredObject(resultRegion);
			// ������ �������� ���⿡ ���� ��ȿ ������� �����ɴϴ�. // Get the valid boundary points used to extract the estimated crosshairs.
			CrossGauge.GetMeasuredValidPoints(flfaResultsValid);
			// ������ �������� ���⿡ ������ ���� ��ȿ���� ���� ������� �����ɴϴ�. // Get invalid boundary points that were not used to extract the estimated crosshairs.
			CrossGauge.GetMeasuredInvalidPoints(flfaResultsInvalid);

			CGUIViewImageLayerWrap layer = viewImage[i].GetLayer(0);

			layer.Clear();

			if(IsFail(res = layer.DrawTextImage(CFLPoint<double>(0., 0.), arrWcsViewText[i], YELLOW, BLUE, 17, true)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			CFLRect<double> flrRegion = CrossGauge.GetMeasurementRegion();

			CFLFigureArray flfaResult = resultRegion.MakeCrossHair(25, true);
			layer.DrawFigureImage(flfaResult, BLACK, 3);
			layer.DrawFigureImage(flfaResult, CYAN, 1);

			layer.DrawFigureImage(flfaResultLine, BLACK, 5);
			layer.DrawFigureImage(flfaResultLine, CYAN, 3);

			// ���� �˻� ������ �����ɴϴ�. // Gets the actual inspection area.
			CFLFigureArray flfaMeasurementRegion = CrossGauge.GetActualMeasurementRegion();

			layer.DrawFigureImage(flfaMeasurementRegion, BLUE, 1, LIGHTBLUE, GUI::EGUIViewImagePenStyle_Solid, 0.5f, 0.25f);

			if(IsOK(res))
			{
				double f64ResultAngle;
				CrossGauge.GetMeasuredAngle(f64ResultAngle);

				printf("Cross Center : (%.2lf, %.2lf)\nAngle : %.2lf��\n", resultRegion.x, resultRegion.y, f64ResultAngle);
			}

			// ����� ��ȿ���� ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the extracted valid point is
			for(int32_t i32Index = 0; i32Index < (int32_t)flfaResultsValid.GetCount(); ++i32Index)
			{
				if(flfaResultsValid.GetAt(i32Index)->GetDeclType() != EFigureDeclType_Point)
					break;

				CFLPoint<double>* pFlp = static_cast<CFLPoint<double>*>(flfaResultsValid.GetAt(i32Index));

				CFLFigureArray flfaPoint = CFLPoint<double>(pFlp->x, pFlp->y).MakeCrossHair(1, true);

				if(IsFail(res = layer.DrawFigureImage(&flfaPoint, LIME)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}
			}

			// ����� ��ȿ���� ���� ���� ������� �˱� ���� ���÷��� �Ѵ� // Display to see where the extracted invalid points are
			for(int32_t i32Index = 0; i32Index < (int32_t)flfaResultsInvalid.GetCount(); ++i32Index)
			{
				if(flfaResultsInvalid.GetAt(i32Index)->GetDeclType() != EFigureDeclType_Point)
					break;

				CFLPoint<double>* pFlp = static_cast<CFLPoint<double>*>(flfaResultsInvalid.GetAt(i32Index));

				CFLFigureArray flfaPoint = CFLPoint<double>(pFlp->x, pFlp->y).MakeCrossHair(1, true);

				if(IsFail(res = layer.DrawFigureImage(&flfaPoint, RED)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}
			}

			// ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the measurement area is
			if(IsFail(res = layer.DrawFigureImage(&measureRegion, BLUE)))
			{
				ErrorPrint(res, "Failed to draw figures objects on the image view.\n");
				break;
			}

			// �̹��� �並 ���� �մϴ�. // Update the image view.
			viewImage[i].Invalidate(true);
		}

		bool bTerminated = false;
		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(!bTerminated)
		{
			for(int32_t i = 0; i < i32ExampleCount; ++i)
				bTerminated |= !viewImage[i].IsAvailable();

			CThreadUtilities::Sleep(1);
		}
	}
	while(false);

	return 0;
}