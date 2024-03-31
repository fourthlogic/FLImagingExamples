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
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Gauge/ellipse.flif")))
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

		// Ellipse Gauge ��ü ���� // Create Ellipse Gauge object
		CEllipseGauge ellipseGauge;

		// ó���� �̹��� ���� // Set the image to process
		ellipseGauge.SetSourceImage(fliImage);

		// ������ ������ �����մϴ�. // Set the area to measure.
		CFLEllipseD measureRegion(180.071947,102.600690, 10.505232,36.645048, -70.682726);
		double tolerance = 25.;
		ellipseGauge.SetMeasurementRegion(measureRegion, tolerance);

		// �����ϱ����� �Ķ���͸� �����մϴ�. // Set parameters for extraction.
		// Ÿ���� �����ϱ����� ������ ����� ��ȭ ���⿡ ���� �����մϴ�. // Set the boundary point change direction to extract to estimate the ellipse.
		ellipseGauge.SetTransitionType(CEllipseGauge::ETransitionType_DarkToBrightOrBrightToDark);
		// Ÿ���� �����ϱ����� ������ ����� �� ����� ����� ������ �����մϴ�. // Select the boundary point type to use among the boundary points extracted to estimate the ellipse.
		ellipseGauge.SetTransitionChoice(CEllipseGauge::ETransitionChoice_Closest);
		// Ÿ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� ���� �����մϴ�. // Set the threshold change of the boundary point to be extracted to estimate the ellipse.
		ellipseGauge.SetThreshold(20);
		// Ÿ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� �������� �����մϴ�. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the ellipse.
		ellipseGauge.SetMinimumAmplitude(10); 
		// Ÿ���� �����ϱ����� ������ ��������� ��ǥ�� ǥ�� ������ �����մϴ�. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the ellipse.
		ellipseGauge.SetThickness(1);
		// Ÿ���� �����ϱ����� ������ ��������� ���� ������ �����մϴ�. // Set the extraction interval of boundary points to be extracted to estimate the ellipse.
		ellipseGauge.SetSamplingStep(1.); 
		// Ÿ���� �����ϱ����� ������ ��������� �̻�ġ ������ ���� �Ӱ谪�� �����մϴ�. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the ellipse.
		ellipseGauge.SetOutliersThreshold(1.);
		// Ÿ���� �����ϱ����� ������ ��������� �̻�ġ ���� Ƚ���� �����մϴ�. // Set the number of outlier adjustments for boundary points to be extracted to estimate the ellipse.
		ellipseGauge.SetOutliersThresholdCount(3);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = ellipseGauge.Execute()))
		{
			ErrorPrint(res, "Failed to execute Ellipse gauge.\n");
			break;
		}

		// ���� ����� �����ɴϴ�. // Get the execution result.
		CFLEllipseD resultRegion;
		CFLFigureArray flfaResultsValid, flfaResultsInvalid; 
		// ������ Ÿ���� �����ɴϴ�. // Get the estimated ellipse.
		ellipseGauge.GetMeasuredObject(resultRegion);
		// ������ Ÿ���� ���⿡ ���� ��ȿ ������� �����ɴϴ�. // Get the valid boundary point used to extract the estimated ellipse.
		ellipseGauge.GetMeasuredValidPoints(flfaResultsValid);
		// ������ Ÿ���� ���⿡ ������ ���� ��ȿ���� ���� ������� �����ɴϴ�. // Get an invalid boundary point that is not used to extract the estimated ellipse.
		ellipseGauge.GetMeasuredInvalidPoints(flfaResultsInvalid);

		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();

		// ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the measurement area is
		CFLEllipseD flres;
		double f64Tolerance;

		res = ellipseGauge.GetMeasuredObject(flres);

		CFLEllipseD fleRegion = ellipseGauge.GetMeasurementRegion();

		CFLEllipseD  fleInner = fleRegion, fleOuter = fleRegion;

		double f64Radius1, f64Radius2;

		fleInner.GetRadius1(f64Radius1);
		fleInner.GetRadius2(f64Radius2);

		f64Tolerance = ellipseGauge.GetTolerance();

		double f64Radius1Tolerance = fleRegion.radius1 >= fleRegion.radius2 ? f64Tolerance : f64Tolerance * (fleRegion.radius1 / fleRegion.radius2);
		double f64Radius2Tolerance = fleRegion.radius1 >= fleRegion.radius2 ? f64Tolerance * (fleRegion.radius2 / fleRegion.radius1) : f64Tolerance;

		// ������ ROI�� ���� ���� �� �ܺ� ���������� ���÷��� �մϴ�. // Display the inner and outer measurement areas for the set ROI.
		if(f64Radius1 < f64Radius1Tolerance || f64Radius2 < f64Radius2Tolerance)
			fleInner.Set((float)fleInner.GetCenter().x, (float)fleInner.GetCenter().y, 0.1f, 0.1f);
		else
		{
			fleInner.radius1 -= f64Radius1Tolerance;
			fleInner.radius2 -= f64Radius2Tolerance;
		}

		fleOuter.radius1 += f64Radius1Tolerance;
		fleOuter.radius2 += f64Radius2Tolerance;

		if(IsFail(res = layer.DrawFigureImage(&fleInner, RED)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		if(IsFail(res = layer.DrawFigureImage(&fleOuter, RED)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		if(IsOK(res))
		{
			// ������ Ÿ���� ���÷��� �մϴ�. // Display the estimated ellipse.
			if(IsFail(res = layer.DrawFigureImage(&flres, BLACK, 5)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsFail(res = layer.DrawFigureImage(&flres, CYAN, 3)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			// Ÿ���� ������ Consoleâ�� ����մϴ�. // Output the information of the ellipse to the console window.
			double f64RadiusResult1, f64RadiusResult2, f64Anglres;
			flres.GetRadius1(&f64RadiusResult1);
			flres.GetRadius2(&f64RadiusResult2);
			f64Anglres = flres.GetAngle();
			CFLPointD flpLineCenter = flres.GetCenter();
			printf("Ellipse Center : (%.2lf, %.2lf)\nRadius X : %.2lf pixels\nRadius Y : %.2lf pixels\nAngle : %.2lf��\n", 
				flpLineCenter.x, flpLineCenter.y, f64RadiusResult1, f64RadiusResult2, f64Anglres);
		}

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
		
		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}