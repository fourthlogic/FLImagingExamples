#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// 그래프 뷰 선언 // Declare the graph view
	CGUIViewGraphWrap arrViewGraph[3];

	do
	{
		// 동작 결과 // operation result
		CResult eResult = EResult_UnknownError;

		// Graph 뷰 생성 // Create graph view
		if(IsFail(eResult = arrViewGraph[0].Create(100, 0, 100 + 440, 340)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Graph 뷰 생성 // Create graph view
		if(IsFail(eResult = arrViewGraph[1].Create(100 + 440 * 1, 0, 100 + 440 * 2, 340)))
		{
			ErrorPrint(eResult, " Failed to create the graph view. \n");
			break;
		}

		// Graph 뷰 생성 // Create graph view
		if(IsFail(eResult = arrViewGraph[2].Create(100 + 440 * 2, 0, 100 + 440 * 3, 340)))
		{
			ErrorPrint(eResult, " Failed to create the graph view. \n");
			break;
		}

		// Graph 뷰의 위치 동기화 // / Synchronize the positions of windows
		if(IsFail(eResult = arrViewGraph[0].SynchronizeWindow(&arrViewGraph[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		// Graph 뷰의 위치 동기화 // / Synchronize the positions of windows
		if(IsFail(eResult = arrViewGraph[1].SynchronizeWindow(&arrViewGraph[2])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		// 3 개의 차트를 생성한다. // Create 3 charts.
		for(size_t k = 0; k < 3; ++k)
		{
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
			CFLString<wchar_t> flstrName;
			flstrName.Format(L"Chart[%d]", k);

			// 그래프에 생성한 데이터를 추가한다. // Plot chart on the graph view.
			arrViewGraph[0].Plot(arrF64DataX, arrF64DataY, stDataCount, EChartType_Line, ui32Color, flstrName, nullptr);
			arrViewGraph[1].Plot(arrF64DataX, arrF64DataY, stDataCount, EChartType_Line, ui32Color, flstrName, nullptr);
			arrViewGraph[2].Plot(arrF64DataX, arrF64DataY, stDataCount, EChartType_Line, ui32Color, flstrName, nullptr);
			CThreadUtilities::Sleep(1);
		}

		// 전체 차트에 대한 Y 축의 최솟값과 차트 이름 표시 // Display the Y-Axis minimum value for the entire charts and name.
		arrViewGraph[0].IndicateEntireChart(EViewGraphRange_MinX, EViewGraphIndicateType_Value | EViewGraphIndicateType_Name);

		// 전체 차트에 대한 Y 축의 최댓값과 이름 표시 // Display the Y-Axis maximum value for the entire charts and name.
		arrViewGraph[0].IndicateEntireChart(EViewGraphRange_MaxY, EViewGraphIndicateType_Value | EViewGraphIndicateType_Name);


		// 모든 차트에 각각 X 축의 최대, 최솟값과 Y 축의 최대, 최솟값을 표시 // Display the range and label for every individual chart.
		arrViewGraph[1].IndicateEveryIndividualChart(EViewGraphRange_MinX, EViewGraphIndicateType_All);
		arrViewGraph[1].IndicateEveryIndividualChart(EViewGraphRange_MaxX, EViewGraphIndicateType_All);
		arrViewGraph[1].IndicateEveryIndividualChart(EViewGraphRange_MinY, EViewGraphIndicateType_All);
		arrViewGraph[1].IndicateEveryIndividualChart(EViewGraphRange_MaxY, EViewGraphIndicateType_All);


		// 특정 차트(2번째 차트)에 Y 축의 최대/최소값을 표시 // Display the Y-Axis minimum/maximum value and name for a particular chart(chart index 0).
		int i32ChartIndex = 2;
		arrViewGraph[2].Indicate(i32ChartIndex, EViewGraphRange_MinX, EViewGraphIndicateType_None);
		arrViewGraph[2].Indicate(i32ChartIndex, EViewGraphRange_MaxX, EViewGraphIndicateType_None);
		arrViewGraph[2].Indicate(i32ChartIndex, EViewGraphRange_MinY, EViewGraphIndicateType_All);
		arrViewGraph[2].Indicate(i32ChartIndex, EViewGraphRange_MaxY, EViewGraphIndicateType_All);

		// Graph 뷰의 마우스 커서 위치에 십자선 표시 해제 // Set the crosshair on mouse cursor invisible.
		arrViewGraph[0].ShowCrosshair(false);
		arrViewGraph[1].ShowCrosshair(false);
		arrViewGraph[2].ShowCrosshair(false);

		// Graph 뷰의 스케일을 조정 // Sets the scales of the graph view.
		arrViewGraph[0].ZoomFit();
		arrViewGraph[1].ZoomFit();
		arrViewGraph[2].ZoomFit();

		// 그래프 뷰가 종료될 때 까지 기다림
		while(arrViewGraph[0].IsAvailable() && arrViewGraph[1].IsAvailable() && arrViewGraph[2].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}