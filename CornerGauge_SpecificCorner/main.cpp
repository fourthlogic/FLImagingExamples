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
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/Gauge/CornerImage.flif")))
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

		// Corner Gauge ��ü ���� // Create Corner Gauge object
		CCornerGauge CornerGauge;

		// ó���� �̹��� ���� // Set the image to process
		CornerGauge.SetSourceImage(fliImage);

		// ������ ������ �����մϴ�. // Set the area to measure.
		CFLRectD measureRegion(383.484046, 403.048099, 648.883614, 648.542699);
		double tolerance = 100.;
		CornerGauge.SetMeasurementRegion(measureRegion, tolerance);

		// �����ϱ����� �Ķ���͸� �����մϴ�. // Set parameters for extraction.
		// �ڳ��� �����ϱ����� ������ ����� ��ȭ ���⿡ ���� �����մϴ�. // Set the boundary point change direction to extract to estimate the corner.
		CornerGauge.SetTransitionType(CCornerGauge::ETransitionType_DarkToBrightOrBrightToDark);
		// �ڳ��� �����ϱ����� ������ ����� �� ����� ����� ������ �����մϴ�. // Select the boundary point type to use among the boundary points extracted to estimate the corner.
		CornerGauge.SetTransitionChoice(CCornerGauge::ETransitionChoice_Closest);
		// �ڳ��� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� ���� �����մϴ�. // Set the threshold change of the boundary point to be extracted to estimate the corner.
		CornerGauge.SetThreshold(20);
		// �ڳ��� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� �������� �����մϴ�. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the corner.
		CornerGauge.SetMinimumAmplitude(10);
		// �ڳ��� �����ϱ����� ������ ��������� ��ǥ�� ǥ�� ������ �����մϴ�. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the corner.
		CornerGauge.SetThickness(1);
		// �ڳ��� �����ϱ����� ������ ��������� ���� ������ �����մϴ�. // Set the extraction interval of boundary points to be extracted to estimate the corner.
		CornerGauge.SetSamplingStep(1.);
		// �ڳ��� �����ϱ����� ������ ��������� �̻�ġ ������ ���� �Ӱ谪�� �����մϴ�. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the corner.
		CornerGauge.SetOutliersThreshold(1.);
		// �ڳ��� �����ϱ����� ������ ��������� �̻�ġ ���� Ƚ���� �����մϴ�. // Set the number of outlier adjustments for boundary points to be extracted to estimate the corner.
		CornerGauge.SetOutliersThresholdCount(3);
		// �ڳ��� �����ϱ����� �� Ŭ�����͸� ó�� ������ ���� ������ �մϴ�. // Set whether or not to process point clustering to estimate the corner.
		CornerGauge.EnableClusterMode(true);
		// �ڳ��� �����ϱ����� ������ �����մϴ�. �ʿ信 ���� �� �������� ���������մϴ�. // Set the margin to estimate the corner. It can be set for each zone as needed.
		CornerGauge.SetMeasurementMarginRatio(0., CCornerGauge::EMargin_All);
		// �ڳ��� �����ϱ����� Tolerance�� �����մϴ�. �ʿ信 ���� �� �������� ���������մϴ�. // Set the Tolerance for estimating the corner. It can be set for each zone as needed.
		CornerGauge.SetTolerance(tolerance, CCornerGauge::ETolerance_All);
		// �ڳʸ� �����ϱ����� ������ �����մϴ�. // Set the area for measuring corners.
		CornerGauge.SetCorner(CCornerGauge::ECorner_LeftTop);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(eResult = CornerGauge.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute Corner gauge.\n");
			break;
		}


		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();

		if(IsOK(eResult))
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

			CFLPoint<double> flpResultCorner;
			CornerGauge.GetMeasuredObject(flpResultCorner);

			layer.DrawFigureImage(flpResultCorner, BLACK, 3);
			layer.DrawFigureImage(flpResultCorner, CYAN, 1);

			printf("Corner X : %.2lf, Corner Y : %.2lf\n", flpResultCorner.x, flpResultCorner.y);

			for(int32_t i = 0; i < 2; ++i)
			{
				double f64ResultAngle;
				f64ResultAngle = arrLines[i].GetAngle();
				printf("Line Angle : %.2lf��\n", f64ResultAngle);
			}
		}

		CFLFigureArray flfaResultsValid, flfaResultsInvalid;
		// ������ �ڳ��� ���⿡ ���� ��ȿ ������� �����ɴϴ�. // Get the effective boundary point used to extract the estimated corner.
		CornerGauge.GetMeasuredValidPoints(flfaResultsValid);
		// ������ �ڳ��� ���⿡ ������ ���� ��ȿ���� ���� ������� �����ɴϴ�. // Get an invalid boundary point that is not used to extract the estimated corner.
		CornerGauge.GetMeasuredInvalidPoints(flfaResultsInvalid);


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