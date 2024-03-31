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
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Gauge/Plate.flif")))
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

		// Line Gauge 객체 생성 // Create Line Gauge object
		CLineGauge lineGauge; 

		// 처리할 이미지 설정 // Set the image to process
		lineGauge.SetSourceImage(fliImage);

		// 측정할 영역을 설정합니다. // Set the area to measure.
		CFLLineD measureRegion(119.284786,267.519218, 158.658237,267.665332);
		double tolerance = 10.;
		lineGauge.SetMeasurementRegion(measureRegion, tolerance);

		// 추출하기위한 파라미터를 설정합니다. // Set parameters for extraction.
		// 선을 추정하기위해 추출할 경계점 변화 방향에 대해 설정합니다. // Set the boundary point change direction to extract to estimate the line.
		lineGauge.SetTransitionType(CLineGauge::ETransitionType_DarkToBrightOrBrightToDark);
		// 선을 추정하기위해 추출한 경계점 중 사용할 경계점 유형을 선택합니다. // Select the boundary point type to use among the boundary points extracted to estimate the line.
		lineGauge.SetTransitionChoice(CLineGauge::ETransitionChoice_Closest);
		// 선을 추정하기위해 추출할 경계점의 변화 임계값에 대해 설정합니다. // Set the threshold change of the boundary point to be extracted to estimate the line.
		lineGauge.SetThreshold(20);
		// 선을 추정하기위해 추출할 경계점의 변화 임계값에 보정값을 설정합니다. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the line.
		lineGauge.SetMinimumAmplitude(10);
		// 선을 추정하기위해 추출할 경계점들의 대표값 표본 개수를 설정합니다. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the line.
		lineGauge.SetThickness(1);
		// 선을 추정하기위해 추출할 경계점들의 추출 간격을 설정합니다. // Set the extraction interval of boundary points to be extracted to estimate the line.
		lineGauge.SetSamplingStep(1.);
		// 선을 추정하기위해 추출할 경계점들의 이상치 조정을 위한 임계값을 설정합니다. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the line.
		lineGauge.SetOutliersThreshold(1.);
		// 선을 추정하기위해 추출할 경계점들의 이상치 조정 횟수을 설정합니다. // Set the number of outlier adjustments for boundary points to be extracted to estimate the line.
		lineGauge.SetOutliersThresholdCount(3);
		// 선을 추정하기위해 점 클러스터링 처리 유무에 대한 설정을 합니다. // Set whether or not to process point clustering to estimate the line.
		lineGauge.EnableClusterMode(true);

		// 알고리즘 수행 // Execute the algorithm
		if(IsFail(res = lineGauge.Execute()))
		{
			ErrorPrint(res, "Failed to execute Line gauge.\n");
			break;
		}

		// 실행 결과를 가져옵니다. // Get the execution result.
		CFLLineD resultRegion;
		CFLFigureArray flfaResultsValid, flfaResultsInvalid;
		// 추정된 선을 가져옵니다. // Get the estimated line.
		lineGauge.GetMeasuredObject(resultRegion);
		// 추정된 선을 추출에 사용된 유효 경계점을 가져옵니다. // Get the effective boundary point used to extract the estimated line.
		lineGauge.GetMeasuredValidPoints(flfaResultsValid);
		// 추정된 선을 추출에 사용되지 못한 유효하지 않은 경계점을 가져옵니다. // Get an invalid boundary point that is not used to extract the estimated line.
		lineGauge.GetMeasuredInvalidPoints(flfaResultsInvalid);

		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();

		CFLLineD fllResult;

		if(IsOK(res))
			lineGauge.GetMeasuredObject(fllResult);

		// 추출된 유효점이 어디인지 알기 위해 디스플레이 한다 // Display to know where the extracted valid point is
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

		// 추출된 유효하지 않은 점이 어디인지 알기 위해 디스플레이 한다 // Display to see where the extracted invalid points are
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

		// 측정 영역이 어디인지 알기 위해 디스플레이 한다 // Display to know where the measurement area is
		if(IsFail(res = layer.DrawFigureImage(&measureRegion, BLUE)))
		{
			ErrorPrint(res, "Failed to draw figures objects on the image view.\n");
			break;
		}

		// 선의 방향을 디스플레이 합니다. // Display the direction of the line.
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

		// 설정된 ROI에 대해 측정영역을 디스플레이 합니다. // Display the measurement area for the set ROI.
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
			// 추정된 선을 디스플레이 합니다. // Display the estimated line.
			if(IsFail(res = layer.DrawFigureImage(&fllResult.GetInfiniteLine(), CYAN)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			// 추정된 선분을 디스플레이 합니다. // Display the estimated line segment.
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

			// 선의 정보를 Console창에 출력합니다. // Output the original information to the console window.
			CFLPointD flpLineCenter = fllResult.GetCenter();
			double f64LineAngle = fllResult.GetAngle();
			printf("Line Center : (%.2lf, %.2lf)\nAngle : %.2lf˚\n", flpLineCenter.x, flpLineCenter.y, f64LineAngle);
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