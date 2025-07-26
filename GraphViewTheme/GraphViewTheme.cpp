#include <cstdio>

#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h" 


int main()
{
	// You must call the following function once
	// before using any features of the FLImaging(R) library
	CLibraryUtilities::Initialize();

	// 그래프 뷰 선언 // Declare the graph view
	CGUIViewGraphWrap viewGraphDark;
	CGUIViewGraphWrap viewGraphLight;

	do
	{
		// 동작 결과 // operation result
		CResult res = EResult_UnknownError;

		// Graph 뷰 생성 // Create graph view
		if(IsFail(res = viewGraphDark.Create(100, 0, 100 + 440, 340)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Graph 뷰 생성 // Create graph view
		if(IsFail(res = viewGraphLight.Create(100 + 440 * 1, 0, 100 + 440 * 2, 340)))
		{
			ErrorPrint(res, " Failed to create the graph view. \n");
			break;
		}

		// Graph 뷰의 위치 동기화 // / Synchronize the positions of windows
		if(IsFail(res = viewGraphDark.SynchronizeWindow(&viewGraphLight)))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		// 랜덤으로 100개의 데이터를 생성한다. // Create 100 random (x,y) coordinates.
		const size_t stDataCount = 100;
		double arrF64DataX[stDataCount] = { 0., };
		double arrF64DataY[stDataCount] = { 0., };

		double f64PrevX = 0.;
		double f64PrevY = 0.;

		for(size_t i = 0; i < stDataCount; ++i)
		{
			arrF64DataX[i] = f64PrevX + ((rand() % 100) / 10.);
			if(rand() % 2)
				arrF64DataY[i] = f64PrevY + ((rand() % 100) / 10.);
			else
				arrF64DataY[i] = f64PrevY - ((rand() % 100) / 10.);

			f64PrevX = arrF64DataX[i];
			f64PrevY = arrF64DataY[i];
		}

		uint32_t ui32Color = COLOR(rand() % 256, rand() % 256, rand() % 256);
		CFLString<wchar_t> flstrName(L"Chart 1");

		// 그래프에 생성한 데이터를 추가한다. // Plot chart on the graph view.
		viewGraphDark.Plot(arrF64DataX, arrF64DataY, stDataCount, EChartType_Scatter, ui32Color, flstrName, nullptr);
		viewGraphLight.Plot(arrF64DataX, arrF64DataY, stDataCount, EChartType_Scatter, ui32Color, flstrName, nullptr);

		// Graph 뷰 테마를 다크모드로 설정 // Sets the theme of the graph view to dark mode.
		viewGraphDark.SetDarkMode();

		// Graph 뷰 테마를 라이트모드로 설정 // Sets the theme of the graph view to light mode.
		viewGraphLight.SetLightMode();

		// Graph 뷰의 스케일을 조정 // Sets the scales of the graph view.
		viewGraphDark.ZoomFit();
		viewGraphLight.ZoomFit();

		// 하나 이상의 뷰가 닫힐 때까지 대기
		// Wait until at least one view is closed
		while(viewGraphDark.IsAvailable() && viewGraphLight.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}