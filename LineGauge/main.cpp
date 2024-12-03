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

	CLineGauge::ETransitionType arrTransitionType[] =
	{
		CLineGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CLineGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CLineGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CLineGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CLineGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CLineGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CLineGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CLineGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CLineGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CLineGauge::ETransitionType_DarkToBright,
		CLineGauge::ETransitionType_BrightToDark,
		CLineGauge::ETransitionType_DarkToBrightToDark,
	};

	CLineGauge::ETransitionChoice arrTransitionChoice[] =
	{
		CLineGauge::ETransitionChoice_Begin,
		CLineGauge::ETransitionChoice_Begin,
		CLineGauge::ETransitionChoice_Begin,
		CLineGauge::ETransitionChoice_LargestArea,
		CLineGauge::ETransitionChoice_End,
		CLineGauge::ETransitionChoice_End,
		CLineGauge::ETransitionChoice_End,
		CLineGauge::ETransitionChoice_LargestAmplitude,
		CLineGauge::ETransitionChoice_Closest,
		CLineGauge::ETransitionChoice_Closest,
		CLineGauge::ETransitionChoice_Closest,
		CLineGauge::ETransitionChoice_Closest,
	};

	double arrTolerance[] = 
	{
		200.,
		200.,
		200.,
		200.,
		200.,
		200.,
		200.,
		200.,
		100.,
		100.,
		100.,
		100.,
	};

	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage[i32ExampleCount];
	CResult res;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Gauge/Stripe.flif")))
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

		// Line Gauge ��ü ���� // Create Line Gauge object
		CLineGauge lineGauge;

		// ó���� �̹��� ���� // Set the image to process
		lineGauge.SetSourceImage(fliImage);
		// ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� ���� �����մϴ�. // Set the threshold change of the boundary point to be extracted to estimate the line.
		lineGauge.SetThreshold(20);
		// ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� �������� �����մϴ�. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the line.
		lineGauge.SetMinimumAmplitude(10);
		// ���� �����ϱ����� ������ ��������� ��ǥ�� ǥ�� ������ �����մϴ�. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the line.
		lineGauge.SetThickness(1);
		// ���� �����ϱ����� ������ ��������� ���� ������ �����մϴ�. // Set the extraction interval of boundary points to be extracted to estimate the line.
		lineGauge.SetSamplingStep(1.);
		// ���� �����ϱ����� ������ ��������� �̻�ġ ������ ���� �Ӱ谪�� �����մϴ�. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the line.
		lineGauge.SetOutliersThreshold(3.);
		// ���� �����ϱ����� ������ ��������� �̻�ġ ���� Ƚ���� �����մϴ�. // Set the number of outlier adjustments for boundary points to be extracted to estimate the line.
		lineGauge.SetOutliersThresholdCount(3);
		// ���� �����ϱ����� �� Ŭ�����͸� ó�� ������ ���� ������ �մϴ�. // Set whether or not to process point clustering to estimate the line.
		lineGauge.EnableClusterMode(true);

		CFLLine<double> measureRegion(250., 480., 250., 80.);

		for(int32_t i = 0; i < i32ExampleCount; ++i)
		{
			// ������ ������ �����մϴ�. // Set the area to measure.
			lineGauge.SetMeasurementRegion(measureRegion, arrTolerance[i]);
			// �����ϱ����� �Ķ���͸� �����մϴ�. // Set parameters for extraction.
			// ���� �����ϱ����� ������ ����� ��ȭ ���⿡ ���� �����մϴ�. // Set the boundary point change direction to extract to estimate the line.
			lineGauge.SetTransitionType(arrTransitionType[i]);
			// ���� �����ϱ����� ������ ����� �� ����� ����� ������ �����մϴ�. // Select the boundary point type to use among the boundary points extracted to estimate the line.
			lineGauge.SetTransitionChoice(arrTransitionChoice[i]);

			// �˰��� ���� // Execute the algorithm
			if(IsFail(res = lineGauge.Execute()))
			{
				ErrorPrint(res, "Failed to execute Line gauge.\n");
				break;
			}

			// ���� ����� �����ɴϴ�. // Get the execution result.
			CFLLine<double> fllResult;
			CFLFigureArray flfaResultsValid, flfaResultsInvalid;

			// index�� ��� TransitionChoice�� Begin, End������ ��ȿ�մϴ�. //Index works only at Begin and End of TransitionChoice
			// ������ ���� �����ɴϴ�. // Get the estimated line.	
			lineGauge.GetMeasuredObject(fllResult, i % 4);
			// ������ ���� ���⿡ ���� ��ȿ ������� �����ɴϴ�. // Get the effective boundary point used to extract the estimated line.
			lineGauge.GetMeasuredValidPoints(flfaResultsValid, i % 4);
			// ������ ���� ���⿡ ������ ���� ��ȿ���� ���� ������� �����ɴϴ�. // Get an invalid boundary point that is not used to extract the estimated line.
			lineGauge.GetMeasuredInvalidPoints(flfaResultsInvalid, i % 4);

			CGUIViewImageLayerWrap layer = viewImage[i].GetLayer(0);

			layer.Clear();

			if(IsFail(res = layer.DrawTextImage(CFLPoint<double>(0., 0.), arrWcsViewText[i], YELLOW, BLUE, 20, true)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
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
			if(IsFail(res = layer.DrawFigureImage(&measureRegion, YELLOW)))
			{
				ErrorPrint(res, "Failed to draw figures objects on the image view.\n");
				break;
			}

		// ���� ������ ���÷��� �մϴ�. // Display the direction of the line.
			CFLPoint<double> flpCenter;
			double f64Angle;
			CFLLine<double> fllCenter;

			flpCenter = measureRegion.GetCenter();
			f64Angle = measureRegion.GetAngle();

			fllCenter.flpPoints[0].x = flpCenter.x;
			fllCenter.flpPoints[0].y = flpCenter.y;
			fllCenter.flpPoints[1].x = flpCenter.x;
			fllCenter.flpPoints[1].y = flpCenter.y;
			fllCenter.Rotate(f64Angle, flpCenter);

			CFLPoint<double> flpCenter1(flpCenter.x - 1.5, flpCenter.y - sqrt(1.5) * .5 * 1.5);
			CFLPoint<double> flpCenter2(flpCenter.x + 1.5, flpCenter.y - sqrt(1.5) * .5 * 1.5);
			CFLPoint<double> flpCenter3(flpCenter.x, flpCenter.y + sqrt(1.5) * .5 * 1.5);

			CFLComplexRegion flTriangle;
			flTriangle.PushBack(&flpCenter1);
			flTriangle.PushBack(&flpCenter2);
			flTriangle.PushBack(&flpCenter3);
			flTriangle.Rotate(f64Angle, &flpCenter);

			if(IsFail(res = layer.DrawFigureImage(&fllCenter, BLUE)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsFail(res = layer.DrawFigureImage(&flTriangle, BLUE)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			// ������ ROI�� ���� ���������� ���÷��� �մϴ�. // Display the measurement area for the set ROI.
			CFLQuad<double> flqDraw;
			double f64ToleranceLeft, f64ToleranceRight;
			lineGauge.GetTolerance(f64ToleranceLeft, f64ToleranceRight);

			CFLPoint<double> fllNorm = measureRegion.GetNormalVector();
			flqDraw.flpPoints[0] = measureRegion.flpPoints[0] + fllNorm * f64ToleranceLeft;
			flqDraw.flpPoints[1] = measureRegion.flpPoints[1] + fllNorm * f64ToleranceLeft;
			flqDraw.flpPoints[2] = measureRegion.flpPoints[1] - fllNorm * f64ToleranceRight;
			flqDraw.flpPoints[3] = measureRegion.flpPoints[0] - fllNorm * f64ToleranceRight;

			if(IsFail(res = layer.DrawFigureImage(&flqDraw, BLUE)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsOK(res))
			{
				// ������ ������ ���÷��� �մϴ�. // Display the estimated line segment.
				if(IsFail(res = layer.DrawFigureImage(&fllResult, BLACK, 5)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}

				if(IsFail(res = layer.DrawFigureImage(&fllResult, CYAN, 3)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}

				// ���� ������ Consoleâ�� ����մϴ�. // Output the original information to the console window.
				CFLPoint<double> flpLineCenter = fllResult.GetCenter();
				double f64LineAngle = fllResult.GetAngle();
				printf("Line Center : (%.2lf, %.2lf)\nAngle : %.2lf��\n", flpLineCenter.x, flpLineCenter.y, f64LineAngle);
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