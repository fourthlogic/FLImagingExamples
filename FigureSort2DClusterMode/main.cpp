#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// Figure 객체 선언 // Declare figure object
	CFLFigureArray flfaSource;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImageNormalSort2D;
	CGUIViewImageWrap viewImageSort2DClusterMode;

	do
	{
		CResult res = EResult_UnknownError;

		// Figure 로드 // Load figure
		if(IsFail(res = flfaSource.Load(L"../../ExampleImages/Figure/RectangleArray.fig")))
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImageNormalSort2D.Create(200, 0, 968, 576)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageSort2DClusterMode.Create(968, 0, 1736, 576)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰와 Destination 이미지 뷰의 시점을 동기화 한다
		if(IsFail(res = viewImageNormalSort2D.SynchronizePointOfView(&viewImageSort2DClusterMode)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageNormalSort2D.SynchronizeWindow(&viewImageSort2DClusterMode)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		CFLFigureArray flfaNormalSort2D(flfaSource);
		CFLFigureArray flfaSort2DClusterMode(flfaSource);

		// FigureArray를 일반 Sort 정렬. 1순위 Y오름차순, 2순위 X오름차순
		// Normal Sort sort the FigureArray. 1st rank Y ascending, 2nd rank X ascending
		if(IsFail(res = flfaNormalSort2D.Sort2D(ESortOrder2D_Y_Asc_X_Asc)))
		{
			ErrorPrint(res, "Failed to process normal sort.");
			break;
		}

		// FigureArray를 Sort2DClusterMode 정렬. 1순위 Y오름차순, 2순위 X오름차순
		// Sort the FigureArray with Sort2DClusterMode. 1st rank Y ascending, 2nd rank X ascending
		if(IsFail(res = flfaSort2DClusterMode.Sort2DClusterMode(ESortOrder2D_Y_Asc_X_Asc)))
		{
			ErrorPrint(res, "Failed to process Sort2DClusterMode.");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately

		CGUIViewImageLayerWrap layerNormalSort2D = viewImageNormalSort2D.GetLayer(0);
		CGUIViewImageLayerWrap layerSort2DClusterMode = viewImageSort2DClusterMode.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerNormalSort2D.Clear();
		layerSort2DClusterMode.Clear();

		if(IsFail(res = layerNormalSort2D.DrawTextCanvas(&CFLPoint<double>(), L"Normal Sort2D (BoundaryRectCenter Y, X)", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(res = layerSort2DClusterMode.DrawTextCanvas(&CFLPoint<double>(), L"Sort2DClusterMode (Y Asc, X Asc)", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// flfaNormalSort2D 는 Figure들의 배열이기 때문에 Layer에 넣기만 해도 모두 드로윙이 가능하다.
		// 아래 함수 DrawFigureImage는 Image좌표를 기준으로 하는 Figure를 Drawing 한다는 것을 의미하며 // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// 맨 마지막 두개의 파라미터는 불투명도 값이고 1일경우 불투명, 0일경우 완전 투명을 의미한다. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// 여기서 0.25이므로 옅은 반투명 상태라고 볼 수 있다.
		// 파라미터 순서 : 레이어 -> Figure 객체 -> 선 색 -> 선 두께 -> 면 색 -> 펜 스타일 -> 선 알파값(불투명도) -> 면 알파값 (불투명도) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(res = layerNormalSort2D.DrawFigureImage(&flfaNormalSort2D, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		if(IsFail(res = layerSort2DClusterMode.DrawFigureImage(&flfaSort2DClusterMode, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// 정보값을 각각 확인하는 코드 // Code to check each information value
		for(int64_t i = 0; i < flfaNormalSort2D.GetCount(); ++i)
		{
			CFLRegion* pFlrg = (CFLRegion*)flfaNormalSort2D.GetAt(i);

			if(pFlrg)
			{
				CFLPoint<double> flpCenter = pFlrg->GetCenter();

				wprintf(L"NormalSort2D No. %lld : (%lf, %lf)\n", i, flpCenter.x, flpCenter.y);

				CFLString<wchar_t> strNumber;
				strNumber.Format(L"%d", i);

				layerNormalSort2D.DrawTextImage(&flpCenter, (wchar_t*)strNumber.GetString(), CYAN, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
			}
		}

		wprintf(L"\n");

		// 정보값을 각각 확인하는 코드 // Code to check each information value
		for(int64_t i = 0; i < flfaSort2DClusterMode.GetCount(); ++i)
		{
			CFLFigureArray* pFlfaCluster = (CFLFigureArray*)flfaSort2DClusterMode.GetAt(i);

			if(pFlfaCluster)
			{
				CFLRect<double> flrBoundary;
				pFlfaCluster->GetBoundaryRect(flrBoundary);

				wprintf(L"Sort2DClusterMode Cluster No. %lld\n", i);

				for(int64_t j= 0; j < pFlfaCluster->GetCount(); ++j)
				{
					CFLRegion* pFlrg = (CFLRegion*)pFlfaCluster->GetAt(j);

					if(pFlrg)
					{
						CFLPoint<double> flpCenter = pFlrg->GetCenter();

						wprintf(L"Sort2DClusterMode No. %lld : (%lf, %lf)\n", j, flpCenter.x, flpCenter.y);

						CFLString<wchar_t> strNumber;
						strNumber.Format(L"(%lld,%lld)", i, j);

						layerSort2DClusterMode.DrawTextImage(&flpCenter, (wchar_t*)strNumber.GetString(), CYAN, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
					}
				}
			}
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImageNormalSort2D.Invalidate(true);
		viewImageSort2DClusterMode.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageNormalSort2D.IsAvailable() && viewImageSort2DClusterMode.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}