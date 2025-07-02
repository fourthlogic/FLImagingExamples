#include <cstdio>

#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h"

int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[3];

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	do
	{
		// View 1 생성 // Create View 1
		if(IsFail(res = viewImage[0].Create(200, 0, 700, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// View 2 생성 // Create View 2
		if(IsFail(res = viewImage[1].Create(700, 0, 1200, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// View 3 생성 // Create View 3
		if(IsFail(res = viewImage[2].Create(1200, 0, 1700, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 각 이미지 뷰의 시점을 동기화 한다. // Synchronize the viewpoint of each image view.
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}
		if(IsFail(res = viewImage[1].SynchronizePointOfView(&viewImage[2])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 각 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the position of each image view window
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}
		if(IsFail(res = viewImage[1].SynchronizeWindow(&viewImage[2])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View 에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer[3];

		for(int32_t i = 0; i < 3; ++i)
			layer[i] = viewImage[i].GetLayer(0);

		// 화면상 좌표(고정 좌표)에 View 의 이름을 표시
		// Indicates view name on screen coordinates (fixed coordinates)
		layer[0].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Default", YELLOW, BLACK, 30);
		layer[1].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Parameter 1", YELLOW, BLACK, 30);
		layer[2].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Parameter 2", YELLOW, BLACK, 30);

		// (x1, y1) = (100, 400), (x2, y2) = (400, 100)  선 생성
		// (x1, y1) = (100, 400), (x2, y2) = (400, 100)  Create Line
		CFLLine<double> fll(100, 400, 400,100);

		// 선 위의 점들을 추출 // Sample points on a line
		CFLFigureArray flfaSrc;
		if((res = fll.GetSamplingPointsOnSegment(10., &flfaSrc)).IsFail())
		{
			ErrorPrint(res, "Failed to calculate.\n");
			break;
		}

		// Noise 가 추가된 PointArray 생성 // Create a PointArray with noise added
		CFLPointArray flpaNoise;
		double f64Epsilon = 10.;

		for(int64_t i = 0; i < flfaSrc.GetCount(); ++i)
			flpaNoise.PushBack(flfaSrc.GetAt(i)->GetCenter() + CRandomGenerator::Double(-f64Epsilon, f64Epsilon));


		CFLLine<double> fllResult1;
		int64_t i64OutlierThresholdCount1 = 0;
		double f64OutlierThreshold1 = 3.;

		// Fit 함수 실행 (Default parameter) // Fit function execution (Default parameter)
		if((res = fllResult1.Fit(flpaNoise)).IsFail())
		{
			ErrorPrint(res, "Failed to calculate.\n");
			break;
		}

		// 0번 Layer 에 Figure 와 Text 를 출력 // Draw Figure and Text to Layer 0
		layer[0].DrawFigureImage(fllResult1, BLACK, 5);
		layer[0].DrawFigureImage(fllResult1, CYAN, 3);
		layer[0].DrawFigureImage(flpaNoise, BLACK, 3);
		layer[0].DrawFigureImage(flpaNoise, LIME, 1);
 		layer[0].DrawTextCanvas(&CFLPoint<int32_t>(0, 40), CFLString<wchar_t>().Format(L"Outlier Threshold Count : %lld", i64OutlierThresholdCount1), YELLOW, BLACK, 15.f);
		layer[0].DrawTextImage(&CFLPoint<int32_t>(350, 350), CFLString<wchar_t>().Format(L"(x1, y1) = (%.3lf,%.3lf)\r\n(x2, y2) = (%.3lf,%.3lf)\r\nAngle = %.3lf", fllResult1.flpPoints[0].x, fllResult1.flpPoints[0].y, fllResult1.flpPoints[1].x, fllResult1.flpPoints[1].y, fllResult1.GetAngle()), YELLOW, BLACK, 13, false, 0., EGUIViewImageTextAlignment_CENTER_CENTER);


		CFLLine<double> fllResult2;
		int64_t i64OutlierThresholdCount2 = 1;
		double f64OutlierThreshold2 = 1.7;
		CFLArray<int64_t> flaOutlierIndices2;
		CFLPointArray flpaOutlier2;

		// Fit 함수 실행 (Parameter1) // Fit function execution (Parameter1)
		if((res = fllResult2.Fit(flpaNoise, i64OutlierThresholdCount2, f64OutlierThreshold2, &flaOutlierIndices2)).IsFail())
		{
			ErrorPrint(res, "Failed to calculate.\n");
			break;
		}

		// Outlier 인덱스로 Outlier PointArray 추가 // Add Outlier PointArray as Outlier Index
		for(int64_t i = 0 ; i < flaOutlierIndices2.GetCount(); ++i)
			flpaOutlier2.PushBack(flpaNoise.GetAt(flaOutlierIndices2.GetAt(i)));

		// 1번 Layer 에 Figure 와 Text 를 출력 // Draw Figure and Text to Layer 1
		layer[1].DrawFigureImage(fllResult2, BLACK, 5);
		layer[1].DrawFigureImage(fllResult2, CYAN, 3);
		layer[1].DrawFigureImage(flpaNoise, BLACK, 3);
		layer[1].DrawFigureImage(flpaNoise, LIME, 1);
		layer[1].DrawFigureImage(flpaOutlier2, RED, 1);
		layer[1].DrawTextCanvas(&CFLPoint<int32_t>(0, 40), CFLString<wchar_t>().Format(L"Outlier Threshold Count : %lld\r\nOutlier Threshold : %.3lf", i64OutlierThresholdCount2, f64OutlierThreshold2), YELLOW, BLACK, 15);
		layer[1].DrawTextImage(&CFLPoint<int32_t>(350, 350), CFLString<wchar_t>().Format(L"(x1, y1) = (%.3lf,%.3lf)\r\n(x2, y2) = (%.3lf,%.3lf)\r\nAngle = %.3lf", fllResult2.flpPoints[0].x, fllResult2.flpPoints[0].y, fllResult2.flpPoints[1].x, fllResult2.flpPoints[1].y, fllResult2.GetAngle()), YELLOW, BLACK, 13, false, 0., EGUIViewImageTextAlignment_CENTER_CENTER);

		CFLLine<double> fllResult3;
		int64_t i64OutlierThresholdCount3 = 3;
		double f64OutlierThreshold3 = 1.;
		CFLArray<int64_t> flaOutlierIndices3;
		CFLPointArray flpaOutlier3;

		// Fit 함수 실행 (Parameter2) // Fit function execution (Parameter2)
		if((res = fllResult3.Fit(flpaNoise, i64OutlierThresholdCount3, f64OutlierThreshold3, &flaOutlierIndices3)).IsFail())
		{
			ErrorPrint(res, "Failed to calculate.\n");
			break;
		}

		// Outlier 인덱스로 Outlier PointArray 추가 // Add Outlier PointArray as Outlier Index
		for(int64_t i = 0; i < flaOutlierIndices3.GetCount(); ++i)
			flpaOutlier3.PushBack(flpaNoise.GetAt(flaOutlierIndices3.GetAt(i)));

		// 2번 Layer 에 Figure 와 Text 를 출력 // Draw Figure and Text to Layer 2
		layer[2].DrawFigureImage(fllResult3, BLACK, 5);
		layer[2].DrawFigureImage(fllResult3, CYAN, 3);
		layer[2].DrawFigureImage(flpaNoise, BLACK, 3);
		layer[2].DrawFigureImage(flpaNoise, LIME, 1);
		layer[2].DrawFigureImage(flpaOutlier3, RED, 1);
		layer[2].DrawTextCanvas(&CFLPoint<int32_t>(0, 40), CFLString<wchar_t>().Format(L"Outlier Threshold Count : %lld\r\nOutlier Threshold : %.3lf", i64OutlierThresholdCount3, f64OutlierThreshold3), YELLOW, BLACK, 15);
		layer[2].DrawTextImage(&CFLPoint<int32_t>(350, 350), CFLString<wchar_t>().Format(L"(x1, y1) = (%.3lf,%.3lf)\r\n(x2, y2) = (%.3lf,%.3lf)\r\nAngle = %.3lf", fllResult3.flpPoints[0].x, fllResult3.flpPoints[0].y, fllResult3.flpPoints[1].x, fllResult3.flpPoints[1].y, fllResult3.GetAngle()), YELLOW, BLACK, 13, false, 0., EGUIViewImageTextAlignment_CENTER_CENTER);
		 

		// Console 출력 // Console output
		wprintf(L"Source Points (Without noise)\n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flfaSrc).GetString());

		wprintf(L"Source Points (With noise)\n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flpaNoise).GetString());

		wprintf(L"[Default parameter]\n");
		wprintf(L"Outlier Threshold Count : %lld\n", i64OutlierThresholdCount1);
		wprintf(L"Result Line : \n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&fllResult1).GetString());

		wprintf(L"[Parameter 1]\n");
		wprintf(L"Outlier Threshold Count : %lld\n", i64OutlierThresholdCount2);
		wprintf(L"Outlier Threshold : %.3lf\n", f64OutlierThreshold2);
		wprintf(L"Result Line : \n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&fllResult2).GetString());

		wprintf(L"[Parameter 2]\n");
		wprintf(L"Outlier Threshold Count : %lld\n", i64OutlierThresholdCount3);
		wprintf(L"Outlier Threshold : %.3lf\n", f64OutlierThreshold3);
		wprintf(L"Result Line : \n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&fllResult3).GetString());


		// 이미지 뷰들을 갱신 합니다. // Update the image views.
		for(int32_t i = 0; i < 3; ++i)
			viewImage[i].Invalidate(true);

		// 이미지 뷰가 셋중에 하나라도 꺼지면 종료로 간주 // Consider closed when any of the three image views are turned off
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable() && viewImage[2].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}