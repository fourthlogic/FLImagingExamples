#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[4];

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	// Source Figure 포인터 선언 // Declare source figure pointer
	CFLFigure* pFlfSource = nullptr;

	do
	{
		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage[0].Create(400, 0, 812, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(812, 0, 1224, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[2].Create(400, 384, 812, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[3].Create(812, 384, 1224, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// SourceView, DstView 의 0번 레이어 가져오기 // Get Layer 0 of SourceView, DstView
		CGUIViewImageLayerWrap layerMin0 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layerMax0 = viewImage[1].GetLayer(0);
		CGUIViewImageLayerWrap layerMin1 = viewImage[2].GetLayer(0);
		CGUIViewImageLayerWrap layerMax1 = viewImage[3].GetLayer(0);

		layerMin0.DrawTextCanvas(TPoint<double>(0, 0), L"Minimum Thickness", YELLOW, BLACK, 15);
		layerMax0.DrawTextCanvas(TPoint<double>(0, 0), L"Maximum Thickness", YELLOW, BLACK, 15);
		layerMin1.DrawTextCanvas(TPoint<double>(0, 0), L"Minimum Thickness", YELLOW, BLACK, 15);
		layerMax1.DrawTextCanvas(TPoint<double>(0, 0), L"Maximum Thickness", YELLOW, BLACK, 15);

		layerMin0.DrawTextCanvas(TPoint<double>(0, 20), L"Trim Ratio : Default", YELLOW, BLACK);
		layerMax0.DrawTextCanvas(TPoint<double>(0, 20), L"Trim Ratio : Default", YELLOW, BLACK);
		layerMin1.DrawTextCanvas(TPoint<double>(0, 20), L"Trim Ratio : 0.01", YELLOW, BLACK);
		layerMax1.DrawTextCanvas(TPoint<double>(0, 20), L"Trim Ratio : 0.05", YELLOW, BLACK);

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		for(int64_t i = 1; i < 4; ++i)
		{
			if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[i])))
			{
				ErrorPrint(res, "Failed to synchronize view\n");
				break;
			}
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		for(int32_t i = 1; i < 4; ++i)
		{
			if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[i])))
			{
				ErrorPrint(res, "Failed to synchronize window.\n");
				break;
			}
		}

		// 화면상에 잘 보이도록 좌표 0.5배율을 적용 // Apply 0.5 magnification to the coordinates so that they can be seen clearly on the screen
		double f64Scale = 0.5;
		// 화면상에 잘 보이도록 시점 Offset 조정 // Adjust the viewpoint offset so that it can be seen clearly on the screen
		double f64CenterCoordX = 500.;
		double f64CenterCoordY = 500.;
		viewImage[0].SetViewCenterAndScale(CFLPoint<double>(f64CenterCoordX, f64CenterCoordY), f64Scale);

		// Source Figure 불러오기 // Load source figure
		if(!(pFlfSource = CFigureUtilities::LoadFigure(L"../../ExampleImages/Figure/Thickness1.fig")))
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}


		// 도형의 최소 두께를 나타내는 점을 얻어옴 // Get a point representing the minimum thickness of the figure
		CFLPointArray flpaResultMinPoints1;

		if(IsFail(res = pFlfSource->GetPointsOfMinimumThickness(&flpaResultMinPoints1)))
		{
			ErrorPrint(res, "Failed to process.\n");
			break;
		}

		// 두께를 측정한 값들에 대해 Trimming 파라미터 적용하여 계산 // Calculated by applying trimming parameters to the measured thickness values
		CFLPointArray flpaResultMinPoints2;

		if(IsFail(res = pFlfSource->GetPointsOfMinimumThickness(&flpaResultMinPoints2, 0.01)))
		{
			ErrorPrint(res, "Failed to process.\n");
			break;
		}

		// 도형의 최대 두께를 나타내는 점을 얻어옴 // Get a point representing the maximum thickness of the figure
		CFLPointArray flpaResultMaxPoints1;

		if(IsFail(res = pFlfSource->GetPointsOfMaximumThickness(&flpaResultMaxPoints1)))
		{
			ErrorPrint(res, "Failed to process.\n");
			break;
		}

		// 두께를 측정한 값들에 대해 Trimming 파라미터 적용하여 계산 // Calculated by applying trimming parameters to the measured thickness values
		CFLPointArray flpaResultMaxPoints2;

		if(IsFail(res = pFlfSource->GetPointsOfMaximumThickness(&flpaResultMaxPoints2, 0.05)))
		{
			ErrorPrint(res, "Failed to process.\n");
			break;
		}

		// 도형의 최소 두께를 얻어옴 // Get the minimum thickness of the figure
		double f64MinimumThickness1 = pFlfSource->GetMinimumThickness();

		// 두께를 측정한 값들에 대해 Trimming 파라미터 적용하여 계산 // Calculated by applying trimming parameters to the measured thickness values
		double f64MinimumThickness2 = pFlfSource->GetMinimumThickness(0.01);

		// 도형의 최대 두께를 얻어옴 // Get the minimum thickness of the figure
		double f64MaximumThickness1 = pFlfSource->GetMaximumThickness();

		// 두께를 측정한 값들에 대해 Trimming 파라미터 적용하여 계산 // Calculated by applying trimming parameters to the measured thickness values
		double f64MaximumThickness2 = pFlfSource->GetMaximumThickness(0.05);

		// 각각의 레이어에 Source Figure 그리기 // Draw source figure on each layer
		layerMin0.DrawFigureImage(pFlfSource, BLACK, 3);
		layerMin0.DrawFigureImage(pFlfSource, LIME);
		layerMin1.DrawFigureImage(pFlfSource, BLACK, 3);
		layerMin1.DrawFigureImage(pFlfSource, LIME);
		layerMax0.DrawFigureImage(pFlfSource, BLACK, 3);
		layerMax0.DrawFigureImage(pFlfSource, LIME);
		layerMax1.DrawFigureImage(pFlfSource, BLACK, 3);
		layerMax1.DrawFigureImage(pFlfSource, LIME);


		layerMin0.DrawFigureImage(flpaResultMinPoints1, BLACK, 3);
		layerMin0.DrawFigureImage(flpaResultMinPoints1, MAGENTA);
		layerMin0.DrawTextImage(flpaResultMinPoints1.GetCenter() + CFLPoint<double>(0, 20), CFLString<wchar_t>().Format(L"%.3lf", f64MinimumThickness1), YELLOW, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_TOP);

		layerMin1.DrawFigureImage(flpaResultMinPoints2, BLACK, 3);
		layerMin1.DrawFigureImage(flpaResultMinPoints2, MAGENTA);
		layerMin1.DrawTextImage(flpaResultMinPoints2.GetCenter() + CFLPoint<double>(0, 20), CFLString<wchar_t>().Format(L"%.3lf", f64MinimumThickness2), YELLOW, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_TOP);

		layerMax0.DrawFigureImage(flpaResultMaxPoints1, BLACK, 3);
		layerMax0.DrawFigureImage(flpaResultMaxPoints1, CYAN);
		layerMax0.DrawTextImage(flpaResultMaxPoints1.GetCenter() + CFLPoint<double>(0, 20), CFLString<wchar_t>().Format(L"%.3lf", f64MaximumThickness1), YELLOW, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_TOP);

		layerMax1.DrawFigureImage(flpaResultMaxPoints2, BLACK, 3);
		layerMax1.DrawFigureImage(flpaResultMaxPoints2, CYAN);
		layerMax1.DrawTextImage(flpaResultMaxPoints2.GetCenter() + CFLPoint<double>(0, 20), CFLString<wchar_t>().Format(L"%.3lf", f64MaximumThickness2), YELLOW, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_TOP);

		// Console 출력 // Console output
		wprintf(L"<Minimum Thickness>\nTrim Ratio : Default\n");
		wprintf(L"Result Thickness : %lf\n", f64MinimumThickness1);
		wprintf(L"Result Points : %s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flpaResultMinPoints1).GetString());

		wprintf(L"<Maximum Thickness>\nTrim Ratio : Default\n");
		wprintf(L"Result Thickness : %lf\n", f64MaximumThickness1);
		wprintf(L"Result Points : %s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flpaResultMaxPoints1).GetString());

		wprintf(L"<Minimum Thickness>\nTrim Ratio : 0.01\n");
		wprintf(L"Result Thickness : %lf\n", f64MinimumThickness2);
		wprintf(L"Result Points : %s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flpaResultMinPoints2).GetString());

		wprintf(L"<Maximum Thickness>\nTrim Ratio : 0.05\n");
		wprintf(L"Result Thickness : %lf\n", f64MaximumThickness2);
		wprintf(L"Result Points : %s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flpaResultMaxPoints2).GetString());

		// 이미지 뷰를 갱신 합니다. // Update image view
		for(int32_t i = 0; i < 4; ++i)
			viewImage[i].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable() && viewImage[2].IsAvailable() && viewImage[3].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	// Source figure 포인터 메모리 할당 해제 // Deallocate memory for the Source figure
	if(pFlfSource)
	{
		delete pFlfSource;
		pFlfSource = nullptr;
	}

	return 0;
}