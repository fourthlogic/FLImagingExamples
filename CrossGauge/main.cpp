#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage;
	CResult eResult;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/Gauge/CrossImage.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Loads image view
		if(IsFail(eResult = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(eResult = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		CResult eResult = EResult_UnknownError;

		// Cross Gauge ��ü ���� // Create Cross Gauge object
		CCrossGauge CrossGauge;

		// ó���� �̹��� ���� // Set the image to process
		CrossGauge.SetSourceImage(fliImage);

		// ������ ������ �����մϴ�. // Set the area to measure.
		CFLRectD measureRegion(264.293737, 247.352397, 787.094791, 779.478531);
		double tolerance = 150.;
		CrossGauge.SetMeasurementRegion(measureRegion, tolerance);

		// �����ϱ����� �Ķ���͸� �����մϴ�. // Set parameters for extraction.
		// �������� �����ϱ����� ������ ����� ��ȭ ���⿡ ���� �����մϴ�. // Set the boundary point change direction to extract to estimate the crosshair.
		CrossGauge.SetTransitionType(CCrossGauge::ETransitionType_DarkToBrightToDarkOrBrightToDarkToBright);
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
		CrossGauge.SetOutliersThreshold(1.);
		// �������� �����ϱ����� ������ ��������� �̻�ġ ���� Ƚ���� �����մϴ�. // Set the number of outlier adjustments for boundary points to be extracted to estimate the crosshairs.
		CrossGauge.SetOutliersThresholdCount(3);
		// �������� �����ϱ����� �� Ŭ�����͸� ó�� ������ ���� ������ �մϴ�. // Set whether or not to process point clustering to estimate the crosshairs.
		CrossGauge.EnableClusterMode(true);
		// �������� �����ϱ����� ������ �����մϴ�. �ʿ信 ���� �� �������� ���������մϴ�. // Set the margin to estimate the crosshairs. It can be set for each zone as needed.
		CrossGauge.SetMeasurementMarginRatio();

		// �˰��� ���� // Execute the algorithm
		if(IsFail(eResult = CrossGauge.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute Cross gauge.\n");
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

		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();

		CFLRectD flrRegion = CrossGauge.GetMeasurementRegion();
		
		CFLFigureArray flfaResult = resultRegion.MakeCrossHair(25, true);
		layer.DrawFigureImage(flfaResult, BLACK, 3);
		layer.DrawFigureImage(flfaResult, CYAN, 1);

		layer.DrawFigureImage(flfaResultLine, BLACK, 5);
		layer.DrawFigureImage(flfaResultLine, CYAN, 3);
		

		if(IsOK(eResult))
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

			CFLPointD* pFlp = static_cast<CFLPointD*>(flfaResultsValid.GetAt(i32Index));

			CFLFigureArray flfaPoint = CFLPointD(pFlp->x, pFlp->y).MakeCrossHair(1, true);

			if(IsFail(eResult = layer.DrawFigureImage(&flfaPoint, LIME)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
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

			if(IsFail(eResult = layer.DrawFigureImage(&flfaPoint, RED)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}
		}

		// ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the measurement area is
		if(IsFail(eResult = layer.DrawFigureImage(&measureRegion, BLUE)))
		{
			ErrorPrint(eResult, "Failed to draw figures objects on the image view.\n");
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