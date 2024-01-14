#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliImage;

	// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImage;
	CResult eResult;

	do
	{
		// 이미지 로드 // Loads image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/Gauge/circle.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Loads image view
		if(IsFail(eResult = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the imageview
		if(IsFail(eResult = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		CResult eResult = EResult_UnknownError;

		// Circle Gauge 객체 생성 // Create Circle Gauge object
		CCircleGauge circleGauge;

		// 처리할 이미지 설정 // Set the image to process
		circleGauge.SetSourceImage(fliImage);

		// 측정할 영역을 설정합니다. // Set the area to measure.
		CFLCircleD measureRegion(235.398801,346.729907, 39.947270);
		double tolerance = 25.;
		circleGauge.SetMeasurementRegion(measureRegion, tolerance);

		// 추출하기위한 파라미터를 설정합니다. // Set parameters for extraction.
		// 원을 추정하기위해 추출할 경계점 변화 방향에 대해 설정합니다. // Set the boundary point change direction to extract to estimate the circle.
		circleGauge.SetTransitionType(CCircleGauge::ETransitionType_DarkToBrightOrBrightToDark);
		// 원을 추정하기위해 추출한 경계점 중 사용할 경계점 유형을 선택합니다. // Select the boundary point type to use among the boundary points extracted to estimate the circle.
		circleGauge.SetTransitionChoice(CCircleGauge::ETransitionChoice_Begin);
		// 원을 추정하기위해 추출할 경계점의 변화 임계값에 대해 설정합니다. // Set the threshold change of the boundary point to be extracted to estimate the circle.
		circleGauge.SetThreshold(20);
		// 원을 추정하기위해 추출할 경계점의 변화 임계값에 보정값을 설정합니다. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the circle.
		circleGauge.SetMinimumAmplitude(10);
		// 원을 추정하기위해 추출할 경계점들의 대표값 표본 개수를 설정합니다. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the circle.
		circleGauge.SetThickness(1);
		// 원을 추정하기위해 추출할 경계점들의 추출 간격을 설정합니다. // Set the extraction interval of boundary points to be extracted to estimate the circle.
		circleGauge.SetSamplingStep(1.);
		// 원을 추정하기위해 추출할 경계점들의 이상치 조정을 위한 임계값을 설정합니다. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the circle.
		circleGauge.SetOutliersThreshold(1.);
		// 원을 추정하기위해 추출할 경계점들의 이상치 조정 횟수을 설정합니다. // Set the number of outlier adjustments for boundary points to be extracted to estimate the circle.
		circleGauge.SetOutliersThresholdCount(3);

		// 알고리즘 수행 // Execute the algorithm
		if(IsFail(eResult = circleGauge.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute Circle gauge.\n");
			break;
		}

		// 실행 결과를 가져옵니다. // Get the execution result.
		CFLCircleD resultRegion;
		CFLFigureArray flfaResultsValid, flfaResultsInvalid;
		// 추정된 원을 가져옵니다. // Get the estimated circle.
		circleGauge.GetMeasuredObject(resultRegion);
		// 추정된 원을 추출에 사용된 유효 경계점을 가져옵니다. // Get the effective boundary point used to extract the estimated circle.
		circleGauge.GetMeasuredValidPoints(flfaResultsValid);
		// 추정된 원을 추출에 사용되지 못한 유효하지 않은 경계점을 가져옵니다. // Get an invalid boundary point that is not used to extract the estimated circle.
		circleGauge.GetMeasuredInvalidPoints(flfaResultsInvalid);

		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();

		// 측정 영역이 어디인지 알기 위해 디스플레이 한다 // Display to know where the measurement area is
		CFLCircleD flcResult;

		eResult = circleGauge.GetMeasuredObject(flcResult);

		CFLCircleD flcRegion = circleGauge.GetMeasurementRegion();

		CFLCircleD flcInner, flcOuter;
		double f64Tolerance;
		f64Tolerance = circleGauge.GetTolerance();

		// 설정된 ROI에 대해 내부 및 외부 측정영역을 디스플레이 합니다. // Display the inner and outer measurement areas for the set ROI.
		if(flcRegion.radius < f64Tolerance)
			flcInner.Set((float)flcRegion.GetCenter().x, (float)flcRegion.GetCenter().y, 0.1f);
		else
			flcInner.Set((float)flcRegion.GetCenter().x, (float)flcRegion.GetCenter().y, flcRegion.radius - f64Tolerance);

		flcOuter.Set((float)flcRegion.GetCenter().x, (float)flcRegion.GetCenter().y, flcRegion.radius + f64Tolerance);

		if(IsFail(eResult = layer.DrawFigureImage(&flcInner, RED)))
		{
			ErrorPrint(eResult, "Failed to draw figure\n");
			break;
		}

		if(IsFail(eResult = layer.DrawFigureImage(&flcOuter, RED)))
		{
			ErrorPrint(eResult, "Failed to draw figure\n");
			break;
		}

		if(IsOK(eResult))
		{
			// 추정된 원을 디스플레이 합니다. // Display the estimated circle.
			if(IsFail(eResult = layer.DrawFigureImage(&flcResult, BLACK, 5)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			if(IsFail(eResult = layer.DrawFigureImage(&flcResult, CYAN, 3)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			// 원의 정보를 Console창에 출력합니다. // Output the original information to the console window.
			double f64Radius;
			flcResult.GetRadius(&f64Radius);
			CFLPointD flpLineCenter = flcResult.GetCenter();
		    printf("Circle Center : (%.2lf, %.2lf)\nRadius : %.2lf pixels", flpLineCenter.x, flpLineCenter.y, f64Radius);
		}

		// 추출된 유효점이 어디인지 알기 위해 디스플레이 한다 // Display to know where the extracted valid point is
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

		// 추출된 유효하지 않은 점이 어디인지 알기 위해 디스플레이 한다 // Display to see where the extracted invalid points are
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

		// 측정 영역이 어디인지 알기 위해 디스플레이 한다 // Display to know where the measurement area is
		if(IsFail(eResult = layer.DrawFigureImage(&measureRegion, BLUE)))
		{
			ErrorPrint(eResult, "Failed to draw figures objects on the image view.\n");
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