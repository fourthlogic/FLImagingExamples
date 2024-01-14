#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[2];

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult cResult;

	do
	{
		// Source Figures View 생성 // Create the Source Figures View
		if(IsFail(cResult = viewImage[0].Create(200, 0, 700, 500)))
		{
			ErrorPrint(cResult, "Failed to create the image view.\n");
			break;
		}

		// Warp Result View 생성 // Create the Warp Result View
		if(IsFail(cResult = viewImage[1].Create(700, 0, 1200, 500)))
		{
			ErrorPrint(cResult, "Failed to create the image view.\n");
			break;
		}

		// 각 이미지 뷰의 시점을 동기화 한다. // Synchronize the viewpoint of each image view.
		if(IsFail(cResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(cResult, "Failed to synchronize view\n");
			break;
		}

		// 각 이미지 뷰 윈도우의 위치를 동기화 한다. // Synchronize the position of each image view window.
		if(IsFail(cResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(cResult, "Failed to synchronize window.\n");
			break;
		}

		// 화면상에 잘 보이도록 좌표 0.5배율을 적용 // Apply the coordinates 0.5 magnification so that it can be seen clearly on the screen
		viewImage[0].SetScale(0.5);
		// 화면상에 잘 보이도록 시점 Offset 조정 // Adjust the viewpoint offset so that it can be seen clearly on the screen
		viewImage[0].SetOffset(&CFLPoint<double>(-120., -35));

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer[2];

		for(int32_t i = 0; i < 2; ++i)
			layer[i] = viewImage[i].GetLayer(0);

		// 화면상 좌표(고정 좌표)에 Source Figure View 임을 표시 // Displays Source Figure View in on-screen coordinates (fixed coordinates)
		layer[0].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Source Figures", YELLOW, BLACK, 30);
		// 화면상 좌표(고정 좌표)에 Warp Result View 임을 표시 // Display of Warp Result View in on-screen coordinates (fixed coordinates)
		layer[1].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Warp Result", YELLOW, BLACK, 30);

		// Warp을 동작하기 위한 Source Figure들이 담긴 FigureArray를 로드합니다. (다른 Figure들도 동작 가능합니다.)
		// Loads a FigureArray containing the source figures for running the warp. (Other figures are also available.)
		CFLFigureArray flfaSource;

		// Source Figure 불러오기 // Load Source Figure
		if(IsFail(cResult = flfaSource.Load(L"../../ExampleImages/Figure/DistortedCoordinates.fig")))
		{
			ErrorPrint(cResult, "Failed to load the figure file.\n");
			break;
		}
		

		// Source Figure의 각 꼭지점을 SourceRegion Quad로 생성
		// Each vertex of the source figure is created as a SourceRegion Quad
		CFLQuad<double> flqSourceRegion(CFLPoint<double>(397.5, 227.0), CFLPoint<double>(1065.0, 292.0), CFLPoint<double>(1063.5, 739.5), CFLPoint<double>(395.0, 822.5));
		// SourceRegion Quad를 직사각형의 형태로 펼친 Quad로 TargetRegion Quad를 생성
		// Create a TargetRegion Quad with a Quad that spreads the SourceRegion Quad in the form of a rectangle
		CFLQuad<double> flqTargetRegion(CFLPoint<double>(397.5, 227.0), CFLPoint<double>(1065.0, 227.0), CFLPoint<double>(1065.5, 822.5), CFLPoint<double>(397.5, 822.5));

		wprintf(L"Source Quad Region : %s\n", CROIUtilities::ConvertFigureObjectToString(flqSourceRegion).GetString());
		wprintf(L"Target Quad Region : %s\n\n", CROIUtilities::ConvertFigureObjectToString(flqTargetRegion).GetString());

		// Warp 결과를 받아올 FigureArray // FigureArray to receive the warp result
		CFLFigureArray flfaResult;

		// Perspective Type으로 Warp 함수 동작 (Perspective, Bilinear 두 타입으로 함수 동작 가능)
		// Warp function works with perspective type (function can be operated with two types, perspective and bilinear)
		if(IsFail(cResult = flfaSource.Warp(flqSourceRegion, flqTargetRegion, flfaResult, EWarpingType_Perspective)))
		{
			ErrorPrint(cResult, "Failed to process.\n");
			break;
		}

		// Source Figure 그리기 // Draw the Source Figure
		if(IsFail(cResult = layer[0].DrawFigureImage(&flfaSource, YELLOW, 3)))
		{
			ErrorPrint(cResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// Warp Result Figure 그리기 // Draw Warp Result Figure
		if(IsFail(cResult = layer[1].DrawFigureImage(&flfaResult, LIME, 3)))
		{
			ErrorPrint(cResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// SourceRegion Quad 그리기 // Draw SourceRegion Quad
		if(IsFail(cResult = layer[0].DrawFigureImage(&flqSourceRegion, RED, 1)))
		{
			ErrorPrint(cResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// TargetRegion Quad 그리기 // Draw TargetRegion Quad
		if(IsFail(cResult = layer[1].DrawFigureImage(&flqTargetRegion, BLUE, 1)))
		{
			ErrorPrint(cResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// Source와 Warp Result Point를 Console 창에 출력 // Output the Source and Warp Result Point to the console window
		for(int i = 0 ; i < flfaSource.GetCount(); ++i)
		{
			CFLFigure* pFlfSource = flfaSource.GetAt(i);
			CFLFigure* pFlfTarget = flfaResult.GetAt(i);

			CFLPoint<double>* pFlpSource = (CFLPoint<double>*)pFlfSource;
			CFLPoint<double>* pFlpTarget = (CFLPoint<double>*)pFlfTarget;

			if(pFlfSource && pFlfTarget)
				printf("Source (%.1lf, %.1lf) -> Warp Result (%.1lf, %.1lf)\n", pFlpSource->x, pFlpSource->y, pFlpTarget->x, pFlpTarget->y);
		}

		// 이미지 뷰들을 갱신 합니다. // Update the image views.
		for(int32_t i = 0; i < 2; ++i)
			viewImage[i].Invalidate(true);


		// 이미지 뷰가 둘중에 하나라도 꺼지면 종료로 간주 // If either one of the imageviews is turned off, it is considered to be closed.
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}