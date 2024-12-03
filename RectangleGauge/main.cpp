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

	CRectangleGauge::ETransitionType arrTransitionType[] =
	{
		CRectangleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CRectangleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CRectangleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CRectangleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CRectangleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CRectangleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CRectangleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CRectangleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CRectangleGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CRectangleGauge::ETransitionType_DarkToBright,
		CRectangleGauge::ETransitionType_BrightToDark,
		CRectangleGauge::ETransitionType_DarkToBrightToDark,
	};

	CRectangleGauge::ETransitionChoice arrTransitionChoice[] =
	{
		CRectangleGauge::ETransitionChoice_Begin,
		CRectangleGauge::ETransitionChoice_Begin,
		CRectangleGauge::ETransitionChoice_Begin,
		CRectangleGauge::ETransitionChoice_LargestArea,
		CRectangleGauge::ETransitionChoice_End,
		CRectangleGauge::ETransitionChoice_End,
		CRectangleGauge::ETransitionChoice_End,
		CRectangleGauge::ETransitionChoice_LargestAmplitude,
		CRectangleGauge::ETransitionChoice_Closest,
		CRectangleGauge::ETransitionChoice_Closest,
		CRectangleGauge::ETransitionChoice_Closest,
		CRectangleGauge::ETransitionChoice_Closest,
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

		// Rectangle Gauge ��ü ���� // Create a Rectangle Gauge object
		CRectangleGauge rectangleGauge;

		// ó���� �̹��� ���� // Set the image to process
		rectangleGauge.SetSourceImage(fliImage);

		// ������ ������ �����մϴ�. // Set the area to measure.
		CFLRect<double> measureRegion(213.577428,262.324155,295.020437,348.179290);
		double tolerance = 50.;
		rectangleGauge.SetMeasurementRegion(measureRegion, tolerance);

		// �����ϱ����� �Ķ���͸� �����մϴ�. // Set parameters for extraction.		
		// �簢���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� ���� �����մϴ�. // Set the threshold change of the boundary point to be extracted to estimate the rectangle.
		rectangleGauge.SetThreshold(20);
		// �簢���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� �������� �����մϴ�. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the rectangle.
		rectangleGauge.SetMinimumAmplitude(10);
		// �簢���� �����ϱ����� ������ ��������� ��ǥ�� ǥ�� ������ �����մϴ�. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the rectangle.
		rectangleGauge.SetThickness(1);
		// �簢���� �����ϱ����� ������ ��������� ���� ������ �����մϴ�. // Set the extraction interval of boundary points to be extracted to estimate the rectangle.
		rectangleGauge.SetSamplingStep(1.);
		// �簢���� �����ϱ����� ������ ��������� �̻�ġ ������ ���� �Ӱ谪�� �����մϴ�. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the rectangle.
		rectangleGauge.SetOutliersThreshold(3.);
		// �簢���� �����ϱ����� ������ ��������� �̻�ġ ���� Ƚ���� �����մϴ�. // Set the number of outlier adjustments for boundary points to be extracted to estimate the rectangle.
		rectangleGauge.SetOutliersThresholdCount(3);
		// �簢���� �����ϱ����� �� Ŭ�����͸� ó�� ������ ���� ������ �մϴ�. // Set whether or not to process point clustering to estimate the rectangle.
		rectangleGauge.EnableClusterMode(true);
		// �簢���� �����ϱ����� ������ �����մϴ�. �ʿ信 ���� �� �������� ���������մϴ�. // Set the margin to estimate the rectangle. It can be set for each zone as needed.
		rectangleGauge.SetMeasurementMarginRatio(0., CRectangleGauge::EMargin_All);
		// �簢���� �����ϱ����� Tolerance�� �����մϴ�. �ʿ信 ���� �� �������� ���������մϴ�. // Set the Tolerance for estimating the rectangle. It can be set for each zone as needed.
		rectangleGauge.SetTolerance(tolerance, CRectangleGauge::ETolerance_All);

		for(int32_t i = 0; i < i32ExampleCount; ++i)
		{
			// �簢���� �����ϱ����� ������ ����� ��ȭ ���⿡ ���� �����մϴ�. // Set the boundary point change direction to extract to estimate the rectangle.
			rectangleGauge.SetTransitionType(arrTransitionType[i]);
			// �簢���� �����ϱ����� ������ ����� �� ����� ����� ������ �����մϴ�. // Select the boundary point type to use among the boundary points extracted to estimate the rectangle.
			rectangleGauge.SetTransitionChoice(arrTransitionChoice[i]);
			// �˰��� ���� // Execute the algorithm
			if(IsFail(res = rectangleGauge.Execute()))
			{
				ErrorPrint(res, "Failed to execute Rectangle gauge.\n");
				break;
			}

			// ���� ����� �����ɴϴ�. // Get the execution result.
			CFLRect<double> flrResult;
			CFLFigureArray flfaResultsValid, flfaResultsInvalid;
			// ������ �簢���� �����ɴϴ�. // Get the estimated rectangle.
			res = rectangleGauge.GetMeasuredObject(flrResult, i % 4);
			// ������ �簢���� ���⿡ ���� ��ȿ ������� �����ɴϴ�. // Get the valid bounding point used to extract the estimated rectangle.
			rectangleGauge.GetMeasuredValidPoints(flfaResultsValid, i % 4);
			// ������ �簢���� ���⿡ ������ ���� ��ȿ���� ���� ������� �����ɴϴ�. // Get an invalid bounding point that was not used to extract the estimated rectangle.
			rectangleGauge.GetMeasuredInvalidPoints(flfaResultsInvalid, i % 4);

			CGUIViewImageLayerWrap layer = viewImage[i].GetLayer(0);

			layer.Clear();

			if(IsFail(res = layer.DrawTextImage(CFLPoint<double>(0., 0.), arrWcsViewText[i], YELLOW, BLUE, 20, true)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			// ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the measurement area is
			CFLRect<double> flrRegion = rectangleGauge.GetMeasurementRegion();
			CFLArray<double> arrTolerance = rectangleGauge.GetTolerance();
			double f64Tolerance = arrTolerance[0];

			double arrF64Tolerance[4];
			double f64WidthTolerance, f64HeightTolerance;
			double f64Ratio;

			double f64Height = flrRegion.GetHeight();
			double f64Width = flrRegion.GetWidth();

			// ������ ROI�� ���� ���� �� �ܺ� ���������� ���÷��� �մϴ�. // Display the inner and outer measurement areas for the set ROI.
			if(f64Height >= f64Width)
			{
				f64Ratio = f64Width / f64Height;

				double f64MinTolerance = f64Height / 2.;

				if(f64Tolerance >= f64MinTolerance)
					f64Tolerance = f64MinTolerance;

				f64HeightTolerance = f64Tolerance;
				f64WidthTolerance = f64HeightTolerance * f64Ratio;
			}

			if(f64Height < f64Width)
			{
				f64Ratio = f64Height / f64Width;

				double f64MinTolerance = f64Width / 2.;

				if(f64Tolerance >= f64MinTolerance)
					f64Tolerance = f64MinTolerance;

				f64WidthTolerance = f64Tolerance;
				f64HeightTolerance = f64WidthTolerance * f64Ratio;
			}

			for(int32_t j = 0; j < 2; ++j)
			{
				arrF64Tolerance[2 * j] = f64WidthTolerance;
				arrF64Tolerance[2 * j + 1] = f64HeightTolerance;
			}

			CFLPoint<double> flpCent = flrRegion.GetCenter();
			CFLRect<double>  flrInner = flrRegion, flrOuter = flrRegion;

			if(flrInner.GetWidth() / 2. <= arrF64Tolerance[0] || flrInner.GetHeight() / 2. <= arrF64Tolerance[1])
			{
				if(IsFail(res = layer.DrawFigureImage(&CFLPoint<double>(flrInner.GetCenter()), RED)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}
			}
			else
			{
				flrInner.Offset(-flpCent.x, -flpCent.y);
				flrInner.Multiply(((double)flrRegion.GetWidth() - arrF64Tolerance[0] * 2.) / (double)flrRegion.GetWidth(), ((double)flrRegion.GetHeight() - arrF64Tolerance[1] * 2.) / (double)flrRegion.GetHeight());
				flrInner.Offset(flpCent);

				if(IsFail(res = layer.DrawFigureImage(&flrInner, RED)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}
			}

			flrOuter.Offset(-flpCent.x, -flpCent.y);
			flrOuter.Multiply(((double)flrRegion.GetWidth() + arrF64Tolerance[0] * 2.) / (double)flrRegion.GetWidth(), ((double)flrRegion.GetHeight() + arrF64Tolerance[1] * 2.) / (double)flrRegion.GetHeight());
			flrOuter.Offset(flpCent);

			if(IsFail(res = layer.DrawFigureImage(&flrOuter, RED)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsOK(res))
			{
				// ������ �簢���� ���÷��� �մϴ�. // Display the estimated rectangle.
				if(IsFail(res = layer.DrawFigureImage(&flrResult, BLACK, 5)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}

				if(IsFail(res = layer.DrawFigureImage(&flrResult, CYAN, 3)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}

				// �簢���� ������ Consoleâ�� ����մϴ�. // Output the square information to the console window.
				double f64ResultWidth, f64ResultHeight, f64ResultAngle;
				f64ResultWidth = flrResult.GetWidth();
				f64ResultHeight = flrResult.GetHeight();
				f64ResultAngle = flrResult.GetAngle();
				CFLPoint<double> flpLineCenter = flrResult.GetCenter();
				printf("Rectangle Center : (%.2lf, %.2lf)\nWidth : %.2lf pixels\nHeight : %.2lf pixels\nAngle : %.2lf��\n", flpLineCenter.x, flpLineCenter.y, f64ResultWidth, f64ResultHeight, f64ResultAngle);
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