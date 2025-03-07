#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// Figure 객체 선언 // Declare figure object
	CFLFigureArray flfaSource;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[2];

	do
	{
		CResult res = EResult_UnknownError;

		// Figure 로드 // Load figure
		if(IsFail(res = flfaSource.Load(L"../../ExampleImages/Figure/various shapes2.fig")))
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage[0].Create(200, 0, 968, 576)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(968, 0, 1736, 576)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰와 Destination 이미지 뷰의 시점을 동기화 한다
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// 화면상에 잘 보이도록 좌표 1.5배율을 적용 // Apply 1.5 magnification to the coordinates so that they can be seen clearly on the screen
		double f64Scale = 1.5;
		// 화면상에 잘 보이도록 시점 Offset 조정 // Adjust the viewpoint offset so that it can be seen clearly on the screen
		double f64CenterCoordX = 200.;
		double f64CenterCoordY = 180.;

		viewImage[0].SetViewCenterAndScale(CFLPoint<double>(f64CenterCoordX, f64CenterCoordY), f64Scale);

		CFLFigureArray flfaCenterFirst(flfaSource);
		CFLFigureArray flfaAreaFirst(flfaSource);

		// FigureArray를 1순위 CenterY 오름차순, 2순위 CenterX 오름차순, 3순위 Area 내림차순으로 정렬
		// Sort FigureArray in ascending order as 1st priority CenterY, ascending order as 2nd priority CenterX, and descending order as 3rd priority Area.
		if(IsFail(res = flfaCenterFirst.Sort(ESortOrderFigure_CenterY_Asc, ESortOrderFigure_CenterX_Asc, ESortOrderFigure_Area_Desc)))
		{
			ErrorPrint(res, "Failed to sort.");
			break;
		}

		// FigureArray를 1순위 Area 내림차순, 2순위 CenterY 오름차순, 3순위 CenterX 오름차순으로 정렬
		// Sort FigureArray in descending order as 1st priority Area, ascending order as 2nd priority CenterY, and ascending order as 3rd priority CenterX.
		if(IsFail(res = flfaAreaFirst.Sort(ESortOrderFigure_Area_Desc, ESortOrderFigure_CenterY_Asc, ESortOrderFigure_CenterX_Asc)))
		{
			ErrorPrint(res, "Failed to sort.");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer[2];

		for(int32_t i = 0; i < 2; ++i)
			layer[i] = viewImage[i].GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		for(int32_t i = 0; i < 2; ++i)
			layer[i].Clear();

		if(IsFail(res = layer[0].DrawTextCanvas(&CFLPoint<double>(), L"1st Y, 2nd X, 3rd Area", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(res = layer[1].DrawTextCanvas(&CFLPoint<double>(), L"1st Area, 2nd Y, 3rd X", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// flfaCenterFirst 는 Figure들의 배열이기 때문에 Layer에 넣기만 해도 모두 드로윙이 가능하다.
		// 아래 함수 DrawFigureImage는 Image좌표를 기준으로 하는 Figure를 Drawing 한다는 것을 의미하며 // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// 맨 마지막 두개의 파라미터는 불투명도 값이고 1일경우 불투명, 0일경우 완전 투명을 의미한다. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// 여기서 0.25이므로 옅은 반투명 상태라고 볼 수 있다.
		// 파라미터 순서 : 레이어 -> Figure 객체 -> 선 색 -> 선 두께 -> 면 색 -> 펜 스타일 -> 선 알파값(불투명도) -> 면 알파값 (불투명도) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(res = layer[0].DrawFigureImage(&flfaCenterFirst, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		if(IsFail(res = layer[1].DrawFigureImage(&flfaAreaFirst, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// 정보값을 각각 확인하는 코드 // Code to check each information value
		for(int64_t i = 0; i < flfaCenterFirst.GetCount(); ++i)
		{
			CFLFigure* pFlf = flfaCenterFirst.GetAt(i);

			if(pFlf)
			{
				CFLPoint<double> flpCenter = pFlf->GetCenter();

				wprintf(L"CenterFirst No. %lld : (%lf, %lf), Area : %lf\n", i, flpCenter.x, flpCenter.y, pFlf->GetArea());

				CFLString<wchar_t> strNumber;
				strNumber.Format(L"%d", i);

				layer[0].DrawTextImage(&flpCenter, (wchar_t*)strNumber.GetString(), CYAN, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
			}
		}

		wprintf(L"\n");

		for(int64_t i = 0; i < flfaAreaFirst.GetCount(); ++i)
		{
			CFLFigure* pFlf = flfaAreaFirst.GetAt(i);

			if(pFlf)
			{
				CFLPoint<double> flpCenter = pFlf->GetCenter();

				wprintf(L"AreaFirst No. %lld : (%lf, %lf), Area : %lf\n", i, flpCenter.x, flpCenter.y, pFlf->GetArea());

				CFLString<wchar_t> strNumber;
				strNumber.Format(L"%d", i);

				layer[1].DrawTextImage(&flpCenter, (wchar_t*)strNumber.GetString(), CYAN, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
			}
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}