#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliImage;

	// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImage;
	CResult res;

	do
	{
		// 이미지 로드 // Loads image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Gauge/ellipse.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Creates imageview
		if(IsFail(res = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the imageview
		if(IsFail(res = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		CResult res = EResult_UnknownError;

		// Ellipse Gauge 객체 생성 // Create Ellipse Gauge object
		CEllipseGauge ellipseGauge;

		// 처리할 이미지 설정 // Set the image to process
		ellipseGauge.SetSourceImage(fliImage);

		// 측정할 영역을 설정합니다. // Set the area to measure.
		CFLEllipse<double> measureRegion(180.071947,102.600690, 10.505232,36.645048, -70.682726);
		double tolerance = 25.;
		ellipseGauge.SetMeasurementRegion(measureRegion, tolerance);

		// 추출하기위한 파라미터를 설정합니다. // Set parameters for extraction.
		// 타원을 추정하기위해 추출할 경계점 변화 방향에 대해 설정합니다. // Set the boundary point change direction to extract to estimate the ellipse.
		ellipseGauge.SetTransitionType(CEllipseGauge::ETransitionType_DarkToBrightOrBrightToDark);
		// 타원을 추정하기위해 추출한 경계점 중 사용할 경계점 유형을 선택합니다. // Select the boundary point type to use among the boundary points extracted to estimate the ellipse.
		ellipseGauge.SetTransitionChoice(CEllipseGauge::ETransitionChoice_Closest);
		// 타원을 추정하기위해 추출할 경계점의 변화 임계값에 대해 설정합니다. // Set the threshold change of the boundary point to be extracted to estimate the ellipse.
		ellipseGauge.SetThreshold(20);
		// 타원을 추정하기위해 추출할 경계점의 변화 임계값에 보정값을 설정합니다. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the ellipse.
		ellipseGauge.SetMinimumAmplitude(10); 
		// 타원을 추정하기위해 추출할 경계점들의 대표값 표본 개수를 설정합니다. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the ellipse.
		ellipseGauge.SetThickness(1);
		// 타원을 추정하기위해 추출할 경계점들의 추출 간격을 설정합니다. // Set the extraction interval of boundary points to be extracted to estimate the ellipse.
		ellipseGauge.SetSamplingStep(1.); 
		// 타원을 추정하기위해 추출할 경계점들의 이상치 조정을 위한 임계값을 설정합니다. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the ellipse.
		ellipseGauge.SetOutliersThreshold(1.);
		// 타원을 추정하기위해 추출할 경계점들의 이상치 조정 횟수을 설정합니다. // Set the number of outlier adjustments for boundary points to be extracted to estimate the ellipse.
		ellipseGauge.SetOutliersThresholdCount(3);

		// 알고리즘 수행 // Execute the algorithm
		if(IsFail(res = ellipseGauge.Execute()))
		{
			ErrorPrint(res, "Failed to execute Ellipse gauge.\n");
			break;
		}

		// 실행 결과를 가져옵니다. // Get the execution result.
		CFLEllipse<double> resultRegion;
		CFLFigureArray flfaResultsValid, flfaResultsInvalid; 
		// 추정된 타원을 가져옵니다. // Get the estimated ellipse.
		ellipseGauge.GetMeasuredObject(resultRegion);
		// 추정된 타원을 추출에 사용된 유효 경계점을 가져옵니다. // Get the valid boundary point used to extract the estimated ellipse.
		ellipseGauge.GetMeasuredValidPoints(flfaResultsValid);
		// 추정된 타원을 추출에 사용되지 못한 유효하지 않은 경계점을 가져옵니다. // Get an invalid boundary point that is not used to extract the estimated ellipse.
		ellipseGauge.GetMeasuredInvalidPoints(flfaResultsInvalid);

		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();

		// 측정 영역이 어디인지 알기 위해 디스플레이 한다 // Display to know where the measurement area is
		CFLEllipse<double> flres;
		double f64Tolerance;

		res = ellipseGauge.GetMeasuredObject(flres);

		CFLEllipse<double> fleRegion = ellipseGauge.GetMeasurementRegion();

		CFLEllipse<double>  fleInner = fleRegion, fleOuter = fleRegion;

		double f64Radius1, f64Radius2;

		fleInner.GetRadius1(f64Radius1);
		fleInner.GetRadius2(f64Radius2);

		f64Tolerance = ellipseGauge.GetTolerance();

		double f64Radius1Tolerance = fleRegion.radius1 >= fleRegion.radius2 ? f64Tolerance : f64Tolerance * (fleRegion.radius1 / fleRegion.radius2);
		double f64Radius2Tolerance = fleRegion.radius1 >= fleRegion.radius2 ? f64Tolerance * (fleRegion.radius2 / fleRegion.radius1) : f64Tolerance;

		// 설정된 ROI에 대해 내부 및 외부 측정영역을 디스플레이 합니다. // Display the inner and outer measurement areas for the set ROI.
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
			// 추정된 타원을 디스플레이 합니다. // Display the estimated ellipse.
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

			// 타원의 정보를 Console창에 출력합니다. // Output the information of the ellipse to the console window.
			double f64RadiusResult1, f64RadiusResult2, f64Anglres;
			flres.GetRadius1(&f64RadiusResult1);
			flres.GetRadius2(&f64RadiusResult2);
			f64Anglres = flres.GetAngle();
			CFLPoint<double> flpLineCenter = flres.GetCenter();
			printf("Ellipse Center : (%.2lf, %.2lf)\nRadius X : %.2lf pixels\nRadius Y : %.2lf pixels\nAngle : %.2lf˚\n", 
				flpLineCenter.x, flpLineCenter.y, f64RadiusResult1, f64RadiusResult2, f64Anglres);
		}

		// 추출된 유효점이 어디인지 알기 위해 디스플레이 한다 // Display to know where the extracted valid point is
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
			
		// 추출된 유효하지 않은 점이 어디인지 알기 위해 디스플레이 한다 // Display to see where the extracted invalid points are
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
		 
		// 측정 영역이 어디인지 알기 위해 디스플레이 한다 // Display to know where the measurement area is
		if(IsFail(res = layer.DrawFigureImage(&measureRegion, BLUE)))
		{
			ErrorPrint(res, "Failed to draw figures objects on the image view.\n");
			break;
		}
		
		// 이미지 뷰를 갱신 합니다. // Update the image view.
		viewImage.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}