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
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/Gauge/CrossImage.flif")))
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

		// Cross Gauge 객체 생성 // Create Cross Gauge object
		CCrossGauge CrossGauge;

		// 처리할 이미지 설정 // Set the image to process
		CrossGauge.SetSourceImage(fliImage);

		// 측정할 영역을 설정합니다. // Set the area to measure.
		CFLRectD measureRegion(264.293737, 247.352397, 787.094791, 779.478531);
		double tolerance = 150.;
		CrossGauge.SetMeasurementRegion(measureRegion, tolerance);

		// 추출하기위한 파라미터를 설정합니다. // Set parameters for extraction.
		// 십자형을 추정하기위해 추출할 경계점 변화 방향에 대해 설정합니다. // Set the boundary point change direction to extract to estimate the crosshair.
		CrossGauge.SetTransitionType(CCrossGauge::ETransitionType_DarkToBrightToDarkOrBrightToDarkToBright);
		// 십자형을 추정하기위해 추출한 경계점 중 사용할 경계점 유형을 선택합니다. // Select the boundary point type to use among the boundary points extracted to estimate the crosshair.
		CrossGauge.SetTransitionChoice(CCrossGauge::ETransitionChoice_Closest);
		// 십자형을 추정하기위해 추출할 경계점의 변화 임계값에 대해 설정합니다. // Set the threshold change of the boundary point to be extracted to estimate the crosshair.
		CrossGauge.SetThreshold(20);
		// 십자형을 추정하기위해 추출할 경계점의 변화 임계값에 보정값을 설정합니다. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the crosshair.
		CrossGauge.SetMinimumAmplitude(10);
		// 십자형을 추정하기위해 추출할 경계점들의 대표값 표본 개수를 설정합니다. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the crosshairs.
		CrossGauge.SetThickness(1);
		// 십자형을 추정하기위해 추출할 경계점들의 추출 간격을 설정합니다. // Set the extraction interval of boundary points to be extracted to estimate the crosshair.
		CrossGauge.SetSamplingStep(1.);
		// 십자형을 추정하기위해 추출할 경계점들의 이상치 조정을 위한 임계값을 설정합니다. // Set the threshold value for outlier adjustment of the boundary points to be extracted to estimate the crosshairs.
		CrossGauge.SetOutliersThreshold(1.);
		// 십자형을 추정하기위해 추출할 경계점들의 이상치 조정 횟수을 설정합니다. // Set the number of outlier adjustments for boundary points to be extracted to estimate the crosshairs.
		CrossGauge.SetOutliersThresholdCount(3);
		// 십자형을 추정하기위해 점 클러스터링 처리 유무에 대한 설정을 합니다. // Set whether or not to process point clustering to estimate the crosshairs.
		CrossGauge.EnableClusterMode(true);
		// 십자형을 추정하기위해 마진을 설정합니다. 필요에 따라 각 구역별로 설정가능합니다. // Set the margin to estimate the crosshairs. It can be set for each zone as needed.
		CrossGauge.SetMeasurementMarginRatio();

		// 알고리즘 수행 // Execute the algorithm
		if(IsFail(eResult = CrossGauge.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute Cross gauge.\n");
			break;
		}

		// 실행 결과를 가져옵니다. // Get the execution result.
		CFLPoint<double> resultRegion;
		CFLFigureArray flfaResultLine;
		CFLFigureArray flfaResultsValid, flfaResultsInvalid;
		// 추정을 위한 라인을 가져옵니다. // Get the line for inference.
		CrossGauge.GetMeasuredLines(flfaResultLine);
		// 추정된 십자형을 가져옵니다. // Get the estimated crosshairs.
		CrossGauge.GetMeasuredObject(resultRegion);
		// 추정된 십자형을 추출에 사용된 유효 경계점을 가져옵니다. // Get the valid boundary points used to extract the estimated crosshairs.
		CrossGauge.GetMeasuredValidPoints(flfaResultsValid);
		// 추정된 십자형을 추출에 사용되지 못한 유효하지 않은 경계점을 가져옵니다. // Get invalid boundary points that were not used to extract the estimated crosshairs.
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

			printf("Cross Center : (%.2lf, %.2lf)\nAngle : %.2lf˚\n", resultRegion.x, resultRegion.y, f64ResultAngle);
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