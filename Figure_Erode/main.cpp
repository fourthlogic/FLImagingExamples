#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[4];

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	do
	{
		// Source View 생성 // Create Source View
		if(IsFail(res = viewImage[0].Create(200, 0, 700, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Erode result1 View 생성 // Create Erode result1 view
		if(IsFail(res = viewImage[1].Create(700, 0, 1200, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Erode result2 View 생성 // Create Erode result2 view
		if(IsFail(res = viewImage[2].Create(200, 500, 700, 1000)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Erode result3 View 생성 // Create Erode result3 view
		if(IsFail(res = viewImage[3].Create(700, 500, 1200, 1000)))
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
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[2])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[3])))
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
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[2])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[3])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View 에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer[4];

		for(int32_t i = 0; i < 4; ++i)
			layer[i] = viewImage[i].GetLayer(0);

		// 화면상 좌표(고정 좌표)에 Source Figure View 임을 표시
		// Indicates Source Figure View on screen coordinates (fixed coordinates)
		layer[0].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Source Figure", YELLOW, BLACK, 30);
		// 화면상 좌표(고정 좌표)에 Result View 임을 표시
		// Indicates Result View on screen coordinates (fixed coordinates)
		layer[1].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Erode Result1", YELLOW, BLACK, 30);
		layer[2].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Erode Result2", YELLOW, BLACK, 30);
		layer[3].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Erode Result3", YELLOW, BLACK, 30);

		CFLRegion flrgSourceFig;

		// Source Figure 불러오기 // Load source figure
		if((res = flrgSourceFig.Load(L"../../ExampleImages/Figure/RegionForReduce.fig")).IsFail())
		{
			ErrorPrint(res, "Failed to load figure.\n");
			break;
		}

		// 각 Layer 에 Figure 를 출력 // Draw Figure to each Layers
		for(int32_t i = 0 ; i < 4; ++i)
		{
			layer[i].DrawFigureImage(flrgSourceFig, BLACK, 5);
			layer[i].DrawFigureImage(flrgSourceFig, LIME, 3);
		}
		
		// Erode 함수 실행 (HalfSize : 10, default kernel : Rectangle)
		// Erode function execution (HalfSize : 10, default kernel : Rectangle)
		CFLFigureArray flfaResult1;
		int64_t i64HalfSize = 10;

		if((res = flrgSourceFig.Erode(i64HalfSize, i64HalfSize, &flfaResult1)).IsFail())
		{
			ErrorPrint(res, "Failed to calculate.\n");
			break;
		}

		// Erode 함수 실행 (HalfSize : 10, kernel shape : Circle)
		// Erode function execution (HalfSize : 10, kernel shape : Circle)
		CFLFigureArray flfaResult2;

		if((res = flrgSourceFig.Erode(i64HalfSize, i64HalfSize, &flfaResult2, EKernelShape_Circle)).IsFail())
		{
			ErrorPrint(res, "Failed to calculate.\n");
			break;
		}

		// Erode 함수 실행 (Figure Kernel : 반지름이 10인 원)
		// Erode function execution (Figure Kernel : Circle with radius 10)
		CFLFigureArray flfaResult3;

		CFLEllipse<double> fleKernel(0., 0., 5., 20., 90);
		CFLEllipse<double> fleForDrawing(fleKernel);

		fleForDrawing.Offset(CFLPoint<double>(245, 53) - fleForDrawing.GetCenter());

		if((res = flrgSourceFig.Erode(&fleKernel, &flfaResult3)).IsFail())
		{
			ErrorPrint(res, "Failed to calculate.\n");
			break;
		}

		// View 에 결과 FigureArray 그리기 // Draw the resulting FigureArray in the View
		layer[1].DrawFigureImage(flfaResult1, BLACK, 5);
		layer[1].DrawFigureImage(flfaResult1, CYAN, 3);
		layer[2].DrawFigureImage(flfaResult2, BLACK, 5);
		layer[2].DrawFigureImage(flfaResult2, YELLOW, 3);
		layer[3].DrawFigureImage(flfaResult3, BLACK, 5);
		layer[3].DrawFigureImage(flfaResult3, LIGHTRED, 3);
		layer[1].DrawTextCanvas(&CFLPoint<int32_t>(0, 40), L"Rectangle (HalfSize : 10)", YELLOW, BLACK, 20);
		layer[2].DrawTextCanvas(&CFLPoint<int32_t>(0, 40), L"Circle (HalfSize : 10)", YELLOW, BLACK, 20);
		layer[3].DrawTextCanvas(&CFLPoint<int32_t>(0, 40), L"User Defined Kernel", YELLOW, BLACK, 20);
		layer[3].DrawFigureCanvas(fleForDrawing, LIGHTRED, 1, LIGHTRED, GUI::EGUIViewImagePenStyle_Solid, 1.f, 0.5f);

 		// Console 출력 // Console output
   		wprintf(L"\n<Source Figure>\n\n");
		wprintf(L"%s", CFigureUtilities::ConvertFigureObjectToString(flrgSourceFig).GetString());
   
 		wprintf(L"\n\n<Erode Result1>\nHalfSize = %lld\nKernel Shape = Default(Rectangle)\n\n", i64HalfSize);
		wprintf(L"Result1 Figure : %s", CFigureUtilities::ConvertFigureObjectToString(flfaResult1).GetString());

		wprintf(L"\n\n<Erode Result2>\nHalfSize = %lld\nKernel Shape = Circle\n\n", i64HalfSize);
		wprintf(L"Result2 Figure : %s", CFigureUtilities::ConvertFigureObjectToString(flfaResult2).GetString());

		wprintf(L"\n\n<Erode Result3>\nKernel Shape = User Defined Kernel\n");
		wprintf(L"Kernel Figure : %s\n\n", CFigureUtilities::ConvertFigureObjectToString(fleKernel).GetString());
		wprintf(L"Result3 Figure : %s\n", CFigureUtilities::ConvertFigureObjectToString(flfaResult3).GetString());


		// 이미지 뷰들을 갱신 합니다. // Update the image views.
		for(int32_t i = 0; i < 4; ++i)
			viewImage[i].Invalidate(true);

		// 이미지 뷰가 하나라도 꺼지면 종료로 간주 // Consider closed when any of image views are turned off
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable() && viewImage[2].IsAvailable() && viewImage[3].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}