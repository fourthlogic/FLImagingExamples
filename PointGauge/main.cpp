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

		// Point Gauge 객체 생성 // Create Point Gauge object
		CPointGauge pointGauge;

		// 처리할 이미지 설정 // Set the image to process
		pointGauge.SetSourceImage(fliImage);

		// 측정할 영역을 설정합니다. // Set the area to measure.
		CFLPoint<double> measureCenter(267.481728,240.846156);
		double tolerance = 400.;
		double angle = 25.;
		pointGauge.SetMeasurementRegion(measureCenter, tolerance, angle);

		// 추출하기위한 파라미터를 설정합니다. // Set parameters for extraction.
		// 점을 추정하기위해 추출할 경계점 변화 방향에 대해 설정합니다. // Set the boundary point change direction to extract to estimate the point.
		pointGauge.SetTransitionType(CPointGauge::ETransitionType_DarkToBrightOrBrightToDark);
		// 추출한 경계점 중 최종적으로 얻고자하는 경계점 유형을 선택합니다. // Select the boundary point type you want to finally get among the extracted boundary points.
		pointGauge.SetTransitionChoice(CPointGauge::ETransitionChoice_Begin);
		// 점을 추정하기위해 추출할 경계점의 변화 임계값에 대해 설정합니다. // Set the threshold change of the boundary point to be extracted to estimate the point.
		pointGauge.SetThreshold(20);
		// 점을 추정하기위해 추출할 경계점의 변화 임계값에 보정값을 설정합니다. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the point.
		pointGauge.SetMinimumAmplitude(10);
		// 점을 추정하기위해 추출할 경계점들의 대표값 표본 개수를 설정합니다. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the points.
		pointGauge.SetThickness(1);

		// 알고리즘 수행 // Execute the algorithm
		if(IsFail(res = pointGauge.Execute()))
		{
			ErrorPrint(res, "Failed to execute point gauge.\n");
			break;
		}

		// 실행 결과를 가져옵니다. // Get the execution result.
		int64_t i64Count = pointGauge.GetMeasuredObjectCount();
		// 추정과정에 사용된 선을 가져옵니다. // Get the line used in the estimation process.
		CFLLine<double> fllLine = pointGauge.GetMeasurementRegion();

		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();

		// 추출된 점이 어디인지 알기 위해 디스플레이 한다 // Display to know where the extracted point is
		for(int32_t i32Index = 0; i32Index < (int32_t)i64Count; ++i32Index)
		{
			CFLPoint<double> flp;
			pointGauge.GetMeasuredObject(flp, i32Index);

			if(IsFail(res = layer.DrawFigureImage(flp.MakeCrossHair(7, true), BLACK, 3)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsFail(res = layer.DrawFigureImage(flp.MakeCrossHair(7, true), YELLOW)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			printf("Index %d : (%lf, %lf)\n", i32Index, flp.x, flp.y);
		}

		// 측정 영역이 어디인지 알기 위해 디스플레이 한다 // Display to know where the measurement area is
		if(IsFail(res = layer.DrawFigureImage(&fllLine, BLUE)))
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