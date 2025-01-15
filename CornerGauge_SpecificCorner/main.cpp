#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	const int32_t i32ExampleCount = 4;
	const wchar_t* arrWcsViewText[] =
	{
		L"Left Top",
		L"Left Bottom",
		L"Left Top, Right Top",
		L"Left Bottom, Right Top, Right Bottom",
	};

	CCornerGauge::ECorner arrECorner[] = 
	{
		CCornerGauge::ECorner_LeftTop,
		CCornerGauge::ECorner_LeftBottom,
		CCornerGauge::ECorner_LeftTop_RightTop,
		CCornerGauge::ECorner_RightTop_RightBottom_LeftBottom,
	};

	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage[i32ExampleCount];
	CResult res;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Gauge/Rect.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates imageview
		for(int32_t i = 0; i < i32ExampleCount; ++i)
		{
			int32_t i32X = 450 * (i % 2);
			int32_t i32Y = 450 * (i / 2);

			if(IsFail(res = viewImage[i].Create(i32X, i32Y, i32X + 450, i32Y + 450)))
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

		// Corner Gauge ��ü ���� // Create Corner Gauge object
		CCornerGauge CornerGauge;

		// ó���� �̹��� ���� // Set the image to process
		CornerGauge.SetSourceImage(fliImage);

		// ������ ������ �����մϴ�. // Set the area to measure.
		CFLRect<double> measureRegion(213.577428, 262.324155, 295.020437, 348.179290);
		double tolerance = 50.;
		CornerGauge.SetMeasurementRegion(measureRegion, tolerance);

		// �����ϱ����� �Ķ���͸� �����մϴ�. // Set parameters for extraction.
		// �ڳʸ� �����ϱ����� ������ ����� ��ȭ ���⿡ ���� �����մϴ�. // Set the boundary point change direction to extract to estimate the corner.
		CornerGauge.SetTransitionType(CCornerGauge::ETransitionType_DarkToBrightOrBrightToDark);
		// �ڳʸ� �����ϱ����� ������ ����� �� ����� ����� ������ �����մϴ�. // Select the boundary point type to use among the boundary points extracted to estimate the corner.
		CornerGauge.SetTransitionChoice(CCornerGauge::ETransitionChoice_Closest);
		// �ڳʸ� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� ���� �����մϴ�. // Set the threshold change of the boundary point to be extracted to estimate the corner.
		CornerGauge.SetThreshold(20);
		// �ڳʸ� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� �������� �����մϴ�. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the corner.
		CornerGauge.SetMinimumAmplitude(10);
		// �ڳʸ� �����ϱ����� ������ ��������� ��ǥ�� ǥ�� ������ �����մϴ�. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the corner.
		CornerGauge.SetThickness(1);
		// �ڳʸ� �����ϱ����� ������ ��������� ���� ������ �����մϴ�. // Set the extraction interval of boundary points to be extracted to estimate the corner.
		CornerGauge.SetSamplingStep(1.);
		// �ڳʸ� �����ϱ����� ������ ��������� �̻�ġ ������ ���� �Ӱ谪�� �����մϴ�. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the corner.
		CornerGauge.SetOutliersThreshold(1.);
		// �ڳʸ� �����ϱ����� ������ ��������� �̻�ġ ���� Ƚ���� �����մϴ�. // Set the number of outlier adjustments for boundary points to be extracted to estimate the corner.
		CornerGauge.SetOutliersThresholdCount(3);
		// �ڳʸ� �����ϱ����� �� Ŭ�����͸� ó�� ������ ���� ������ �մϴ�. // Set whether or not to process point clustering to estimate the corner.
		CornerGauge.EnableClusterMode(true);
		// �ڳʸ� �����ϱ����� ������ �����մϴ�. �ʿ信 ���� �� �������� ���������մϴ�. // Set the margin to estimate the corner. It can be set for each zone as needed.
		CornerGauge.SetMeasurementMarginRatio(0., CCornerGauge::EMargin_All);
		// �ڳʸ� �����ϱ����� Tolerance�� �����մϴ�. �ʿ信 ���� �� �������� ���������մϴ�. // Set the Tolerance for estimating the corner. It can be set for each zone as needed.
		CornerGauge.SetTolerance(tolerance, CCornerGauge::ETolerance_All);
		

		for(int32_t i = 0; i < i32ExampleCount; ++i)
		{
			// �ڳʸ� �����ϱ����� ������ �����մϴ�. // Set the area for measuring corners.
			CornerGauge.SetCorner(arrECorner[i]);

			// �˰��� ���� // Execute the algorithm
			if(IsFail(res = CornerGauge.Execute()))
			{
				ErrorPrint(res, "Failed to execute Corner gauge.\n");
				break;
			}

			CGUIViewImageLayerWrap layer = viewImage[i].GetLayer(0);

			layer.Clear();

			if(IsFail(res = layer.DrawTextImage(CFLPoint<double>(0., 0.), arrWcsViewText[i], YELLOW, BLUE, 20, true)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsOK(res))
			{
				// ���� ����� �����ɴϴ�. // Get the execution result.
				CFLFigureArray flfaResultLine;
				// ������ ���� �����ɴϴ�. // Get the estimated line.
				CornerGauge.GetMeasuredLines(flfaResultLine);

				layer.DrawFigureImage(flfaResultLine, BLACK, 5);
				layer.DrawFigureImage(flfaResultLine, CYAN, 3);

				CFLLine<double> arrLines[2];
				arrLines[0] = dynamic_cast<CFLLine<double>*>(flfaResultLine.GetAt(0));
				arrLines[1] = dynamic_cast<CFLLine<double>*>(flfaResultLine.GetAt(1));

				CFLFigureArray flfaResultCorner;
				CornerGauge.GetMeasuredObject(flfaResultCorner);

				layer.DrawFigureImage(flfaResultCorner, BLACK, 3);
				layer.DrawFigureImage(flfaResultCorner, CYAN, 1);

				for(int32_t j = 0; j < flfaResultCorner.GetCount(); ++j)
				{
					CFLPoint<double>* pFlpCorner = (CFLPoint<double>*)flfaResultCorner[j];
					printf("Corner X : %.2lf, Corner Y : %.2lf\n", pFlpCorner->x, pFlpCorner->y);
				}

				for(int32_t i = 0; i < 2; ++i)
				{
					double f64ResultAngle;
					f64ResultAngle = arrLines[i].GetAngle();
					printf("Line Angle : %.2lf��\n", f64ResultAngle);
				}
			}

			CFLFigureArray flfaResultsValid, flfaResultsInvalid;
			// ������ �ڳʸ� ���⿡ ���� ��ȿ ������� �����ɴϴ�. // Get the effective boundary point used to extract the estimated corner.
			CornerGauge.GetMeasuredValidPoints(flfaResultsValid);
			// ������ �ڳʸ� ���⿡ ������ ���� ��ȿ���� ���� ������� �����ɴϴ�. // Get an invalid boundary point that is not used to extract the estimated corner.
			CornerGauge.GetMeasuredInvalidPoints(flfaResultsInvalid);


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