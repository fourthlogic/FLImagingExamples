#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage;
	CResult res;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Gauge/Plate.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates imageview
		if(IsFail(res = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(res = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		CResult res = EResult_UnknownError;

		// Line Gauge ��ü ���� // Create Line Gauge object
		CLineGauge lineGauge; 

		// ó���� �̹��� ���� // Set the image to process
		lineGauge.SetSourceImage(fliImage);

		// ������ ������ �����մϴ�. // Set the area to measure.
		CFLLineD measureRegion(119.284786,267.519218, 158.658237,267.665332);
		double tolerance = 10.;
		lineGauge.SetMeasurementRegion(measureRegion, tolerance);

		// �����ϱ����� �Ķ���͸� �����մϴ�. // Set parameters for extraction.
		// ���� �����ϱ����� ������ ����� ��ȭ ���⿡ ���� �����մϴ�. // Set the boundary point change direction to extract to estimate the line.
		lineGauge.SetTransitionType(CLineGauge::ETransitionType_DarkToBrightOrBrightToDark);
		// ���� �����ϱ����� ������ ����� �� ����� ����� ������ �����մϴ�. // Select the boundary point type to use among the boundary points extracted to estimate the line.
		lineGauge.SetTransitionChoice(CLineGauge::ETransitionChoice_Closest);
		// ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� ���� �����մϴ�. // Set the threshold change of the boundary point to be extracted to estimate the line.
		lineGauge.SetThreshold(20);
		// ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� �������� �����մϴ�. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the line.
		lineGauge.SetMinimumAmplitude(10);
		// ���� �����ϱ����� ������ ��������� ��ǥ�� ǥ�� ������ �����մϴ�. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the line.
		lineGauge.SetThickness(1);
		// ���� �����ϱ����� ������ ��������� ���� ������ �����մϴ�. // Set the extraction interval of boundary points to be extracted to estimate the line.
		lineGauge.SetSamplingStep(1.);
		// ���� �����ϱ����� ������ ��������� �̻�ġ ������ ���� �Ӱ谪�� �����մϴ�. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the line.
		lineGauge.SetOutliersThreshold(1.);
		// ���� �����ϱ����� ������ ��������� �̻�ġ ���� Ƚ���� �����մϴ�. // Set the number of outlier adjustments for boundary points to be extracted to estimate the line.
		lineGauge.SetOutliersThresholdCount(3);
		// ���� �����ϱ����� �� Ŭ�����͸� ó�� ������ ���� ������ �մϴ�. // Set whether or not to process point clustering to estimate the line.
		lineGauge.EnableClusterMode(true);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = lineGauge.Execute()))
		{
			ErrorPrint(res, "Failed to execute Line gauge.\n");
			break;
		}

		// ���� ����� �����ɴϴ�. // Get the execution result.
		CFLLineD resultRegion;
		CFLFigureArray flfaResultsValid, flfaResultsInvalid;
		// ������ ���� �����ɴϴ�. // Get the estimated line.
		lineGauge.GetMeasuredObject(resultRegion);
		// ������ ���� ���⿡ ���� ��ȿ ������� �����ɴϴ�. // Get the effective boundary point used to extract the estimated line.
		lineGauge.GetMeasuredValidPoints(flfaResultsValid);
		// ������ ���� ���⿡ ������ ���� ��ȿ���� ���� ������� �����ɴϴ�. // Get an invalid boundary point that is not used to extract the estimated line.
		lineGauge.GetMeasuredInvalidPoints(flfaResultsInvalid);

		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();

		CFLLineD fllResult;

		if(IsOK(res))
			lineGauge.GetMeasuredObject(fllResult);

		// ����� ��ȿ���� ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the extracted valid point is
		for(int32_t i32Index = 0; i32Index < (int32_t)flfaResultsValid.GetCount(); ++i32Index)
		{
			if(flfaResultsValid.GetAt(i32Index)->GetDeclType() != EFigureDeclType_Point)
				break;

			CFLPointD* pFlp = static_cast<CFLPointD*>(flfaResultsValid.GetAt(i32Index));

			CFLFigureArray flfaPoint = CFLPointD(pFlp->x, pFlp->y).MakeCrossHair(1, true);

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

			CFLPointD* pFlp = static_cast<CFLPointD*>(flfaResultsInvalid.GetAt(i32Index));

			CFLFigureArray flfaPoint = CFLPointD(pFlp->x, pFlp->y).MakeCrossHair(1, true);

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

		// ���� ������ ���÷��� �մϴ�. // Display the direction of the line.
		CFLPointD flpCenter;
		double f64Angle;
		CFLLineD fllCenter;

		flpCenter = measureRegion.GetCenter();
		f64Angle = measureRegion.GetAngle();

		fllCenter.flpPoints[0].x = flpCenter.x;
		fllCenter.flpPoints[0].y = flpCenter.y;
		fllCenter.flpPoints[1].x = flpCenter.x;
		fllCenter.flpPoints[1].y = flpCenter.y;
		fllCenter.Rotate(f64Angle, flpCenter);

		CFLPointD flpCenter1(flpCenter.x - 1.5, flpCenter.y - sqrt(1.5) * .5 * 1.5);
		CFLPointD flpCenter2(flpCenter.x + 1.5, flpCenter.y - sqrt(1.5) * .5 * 1.5);
		CFLPointD flpCenter3(flpCenter.x, flpCenter.y + sqrt(1.5) * .5 * 1.5);

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
		CFLQuadD flqDraw;
		double f64ToleranceLeft, f64ToleranceRight;
		lineGauge.GetTolerance(f64ToleranceLeft, f64ToleranceRight);

		CFLPointD fllNorm = measureRegion.GetNormalVector();
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
			// ������ ���� ���÷��� �մϴ�. // Display the estimated line.
			if(IsFail(res = layer.DrawFigureImage(&fllResult.GetInfiniteLine(), CYAN)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

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
			CFLPointD flpLineCenter = fllResult.GetCenter();
			double f64LineAngle = fllResult.GetAngle();
			printf("Line Center : (%.2lf, %.2lf)\nAngle : %.2lf��\n", flpLineCenter.x, flpLineCenter.y, f64LineAngle);
		}
		
		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}