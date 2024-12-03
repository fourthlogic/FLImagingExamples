#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	const int32_t i32ExampleCount = 12;
	const wchar_t* arrWcsViewText[] =
	{
		L"Dark To Bright Or Bright To Dark\nBegin 0",
		L"Dark To Bright Or Bright To Dark\nBegin 1",
		L"Dark To Bright Or Bright To Dark\nBegin 2",
		L"Dark To Bright Or Bright To Dark\nLargest Area",
		L"Dark To Bright Or Bright To Dark\nEnd 0",
		L"Dark To Bright Or Bright To Dark\nEnd 1",
		L"Dark To Bright Or Bright To Dark\nEnd 2",
		L"Dark To Bright Or Bright To Dark\nLargest Amplitude",
		L"Dark To Bright Or Bright To Dark\nClosest",
		L"Dark To Bright\nClosest",
		L"Bright To Dark\nClosest",
		L"Dark To Bright To Dark\nClosest",
	};

	CCircleGauge::ETransitionType arrTransitionType[] =
	{
		CCircleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CCircleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CCircleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CCircleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CCircleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CCircleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CCircleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CCircleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CCircleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CCircleGauge::ETransitionType_DarkToBright,
		CCircleGauge::ETransitionType_BrightToDark,
		CCircleGauge::ETransitionType_DarkToBrightToDark,
	};

	CCircleGauge::ETransitionChoice arrTransitionChoice[] =
	{
		CCircleGauge::ETransitionChoice_Begin,
		CCircleGauge::ETransitionChoice_Begin,
		CCircleGauge::ETransitionChoice_Begin,
		CCircleGauge::ETransitionChoice_LargestArea,
		CCircleGauge::ETransitionChoice_End,
		CCircleGauge::ETransitionChoice_End,
		CCircleGauge::ETransitionChoice_End,
		CCircleGauge::ETransitionChoice_LargestAmplitude,
		CCircleGauge::ETransitionChoice_Closest,
		CCircleGauge::ETransitionChoice_Closest,
		CCircleGauge::ETransitionChoice_Closest,
		CCircleGauge::ETransitionChoice_Closest,
	};

	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage[i32ExampleCount];
	CResult res;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Gauge/Circle.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates imageview
		for(int32_t i = 0; i < i32ExampleCount; ++i)
		{
			int32_t i32X = 300 * (i % 4);
			int32_t i32Y = 300 * (i / 4);

			if(IsFail(res = viewImage[i].Create(i32X, i32Y, i32X + 300, i32Y + 300)))
			{
				ErrorPrint(res, "Failed to create the image view.\n");
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
			if(IsFail(res = viewImage[i].SetImagePtr(&fliImage)))
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

		// Circle Gauge ��ü ���� // Create Circle Gauge object
		CCircleGauge circleGauge;

		// ó���� �̹��� ���� // Set the image to process
		circleGauge.SetSourceImage(fliImage);

		// ������ ������ �����մϴ�. // Set the area to measure.
		CFLCircle<double> measureRegion(260., 265., 120.);
		double tolerance = 100.;
		circleGauge.SetMeasurementRegion(measureRegion, tolerance);

		// �����ϱ����� �Ķ���͸� �����մϴ�. // Set parameters for extraction.
		// ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� ���� �����մϴ�. // Set the threshold change of the boundary point to be extracted to estimate the circle.
		circleGauge.SetThreshold(20);
		// ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� �������� �����մϴ�. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the circle.
		circleGauge.SetMinimumAmplitude(10);
		// ���� �����ϱ����� ������ ��������� ��ǥ�� ǥ�� ������ �����մϴ�. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the circle.
		circleGauge.SetThickness(3);
		// ���� �����ϱ����� ������ ��������� ���� ������ �����մϴ�. // Set the extraction interval of boundary points to be extracted to estimate the circle.
		circleGauge.SetSamplingStep(1.);
		// ���� �����ϱ����� ������ ��������� �̻�ġ ������ ���� �Ӱ谪�� �����մϴ�. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the circle.
		circleGauge.SetOutliersThreshold(3.);
		// ���� �����ϱ����� ������ ��������� �̻�ġ ���� Ƚ���� �����մϴ�. // Set the number of outlier adjustments for boundary points to be extracted to estimate the circle.
		circleGauge.SetOutliersThresholdCount(3);

		for(int32_t i = 0; i < i32ExampleCount; ++i)
		{
			// ���� �����ϱ����� ������ ����� ��ȭ ���⿡ ���� �����մϴ�. // Set the boundary point change direction to extract to estimate the circle.
			circleGauge.SetTransitionType(arrTransitionType[i]);
			// ���� �����ϱ����� ������ ����� �� ����� ����� ������ �����մϴ�. // Select the boundary point type to use among the boundary points extracted to estimate the circle.
			circleGauge.SetTransitionChoice(arrTransitionChoice[i]);
			// �˰��� ���� // Execute the algorithm
			if(IsFail(res = circleGauge.Execute()))
			{
				ErrorPrint(res, "Failed to execute Circle gauge.\n");
				break;
			}

			// ���� ����� �����ɴϴ�. // Get the execution result.
			CFLCircle<double> flcResult;
			CFLFigureArray flfaResultsValid, flfaResultsInvalid;

			// index�� ��� TransitionChoice�� Begin, End������ ��ȿ�մϴ�. //Index works only at Begin and End of TransitionChoice
			// ������ ���� �����ɴϴ�. // Get the estimated circle.	
			res = circleGauge.GetMeasuredObject(flcResult, i % 4);
			// ������ ���� ���⿡ ���� ��ȿ ������� �����ɴϴ�. // Get the effective boundary point used to extract the estimated circle.
			circleGauge.GetMeasuredValidPoints(flfaResultsValid, i % 4);
			// ������ ���� ���⿡ ������ ���� ��ȿ���� ���� ������� �����ɴϴ�. // Get an invalid boundary point that is not used to extract the estimated circle.
			circleGauge.GetMeasuredInvalidPoints(flfaResultsInvalid, i % 4);

			CGUIViewImageLayerWrap layer = viewImage[i].GetLayer(0);

			layer.Clear();

			if(IsFail(res = layer.DrawTextImage(CFLPoint<double>(0., 0.), arrWcsViewText[i], YELLOW, BLUE, 20, true)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			// ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the measurement area is
			CFLCircle<double> flcRegion = circleGauge.GetMeasurementRegion();

			CFLCircle<double> flcInner, flcOuter;
			double f64Tolerance;
			f64Tolerance = circleGauge.GetTolerance();

			// ������ ROI�� ���� ���� �� �ܺ� ���������� ���÷��� �մϴ�. // Display the inner and outer measurement areas for the set ROI.
			if(flcRegion.radius < f64Tolerance)
				flcInner.Set((float)flcRegion.GetCenter().x, (float)flcRegion.GetCenter().y, 0.1f);
			else
				flcInner.Set((float)flcRegion.GetCenter().x, (float)flcRegion.GetCenter().y, flcRegion.radius - f64Tolerance);

			flcOuter.Set((float)flcRegion.GetCenter().x, (float)flcRegion.GetCenter().y, flcRegion.radius + f64Tolerance);

			if(IsFail(res = layer.DrawFigureImage(&flcInner, RED)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsFail(res = layer.DrawFigureImage(&flcOuter, RED)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsOK(res))
			{
				// ������ ���� ���÷��� �մϴ�. // Display the estimated circle.
				if(IsFail(res = layer.DrawFigureImage(&flcResult, BLACK, 5)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}

				if(IsFail(res = layer.DrawFigureImage(&flcResult, CYAN, 3)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}

				// ���� ������ Consoleâ�� ����մϴ�. // Output the original information to the console window.
				double f64Radius;
				flcResult.GetRadius(&f64Radius);
				CFLPoint<double> flpLineCenter = flcResult.GetCenter();
				printf("Circle Center : (%.2lf, %.2lf)\nRadius : %.2lf pixels", flpLineCenter.x, flpLineCenter.y, f64Radius);
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