#include <cstdio>

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
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Gauge/circle.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Loads image view
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

		// Circle Gauge ��ü ���� // Create Circle Gauge object
		CCircleGauge circleGauge;

		// ó���� �̹��� ���� // Set the image to process
		circleGauge.SetSourceImage(fliImage);

		// ������ ������ �����մϴ�. // Set the area to measure.
		CFLCircle<double> measureRegion(235.398801,346.729907, 39.947270);
		double tolerance = 25.;
		circleGauge.SetMeasurementRegion(measureRegion, tolerance);

		// �����ϱ����� �Ķ���͸� �����մϴ�. // Set parameters for extraction.
		// ���� �����ϱ����� ������ ����� ��ȭ ���⿡ ���� �����մϴ�. // Set the boundary point change direction to extract to estimate the circle.
		circleGauge.SetTransitionType(CCircleGauge::ETransitionType_DarkToBrightOrBrightToDark);
		// ���� �����ϱ����� ������ ����� �� ����� ����� ������ �����մϴ�. // Select the boundary point type to use among the boundary points extracted to estimate the circle.
		circleGauge.SetTransitionChoice(CCircleGauge::ETransitionChoice_Begin);
		// ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� ���� �����մϴ�. // Set the threshold change of the boundary point to be extracted to estimate the circle.
		circleGauge.SetThreshold(20);
		// ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� �������� �����մϴ�. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the circle.
		circleGauge.SetMinimumAmplitude(10);
		// ���� �����ϱ����� ������ ��������� ��ǥ�� ǥ�� ������ �����մϴ�. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the circle.
		circleGauge.SetThickness(1);
		// ���� �����ϱ����� ������ ��������� ���� ������ �����մϴ�. // Set the extraction interval of boundary points to be extracted to estimate the circle.
		circleGauge.SetSamplingStep(1.);
		// ���� �����ϱ����� ������ ��������� �̻�ġ ������ ���� �Ӱ谪�� �����մϴ�. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the circle.
		circleGauge.SetOutliersThreshold(1.);
		// ���� �����ϱ����� ������ ��������� �̻�ġ ���� Ƚ���� �����մϴ�. // Set the number of outlier adjustments for boundary points to be extracted to estimate the circle.
		circleGauge.SetOutliersThresholdCount(3);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = circleGauge.Execute()))
		{
			ErrorPrint(res, "Failed to execute Circle gauge.\n");
			break;
		}

		// ���� ����� �����ɴϴ�. // Get the execution result.
		CFLCircle<double> resultRegion;
		CFLFigureArray flfaResultsValid, flfaResultsInvalid;
		// ������ ���� �����ɴϴ�. // Get the estimated circle.
		circleGauge.GetMeasuredObject(resultRegion);
		// ������ ���� ���⿡ ���� ��ȿ ������� �����ɴϴ�. // Get the effective boundary point used to extract the estimated circle.
		circleGauge.GetMeasuredValidPoints(flfaResultsValid);
		// ������ ���� ���⿡ ������ ���� ��ȿ���� ���� ������� �����ɴϴ�. // Get an invalid boundary point that is not used to extract the estimated circle.
		circleGauge.GetMeasuredInvalidPoints(flfaResultsInvalid);

		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();

		// ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the measurement area is
		CFLCircle<double> flcResult;

		res = circleGauge.GetMeasuredObject(flcResult);

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
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}