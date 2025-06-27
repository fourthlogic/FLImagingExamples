#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 3D 뷰 선언
	// Declaration of the 3D view 
	CGUIView3DWrap view3D[2];

	// 3D 뷰 layer 선언
	// Declaration of the 3D view layer
	CGUIView3DLayerWrap layer3D[2];

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	do
	{
		// 3D 뷰 생성 // Create the 3D view
		if(IsFail(res = view3D[0].Create(400, 0, 812, 384)))
		{
			ErrorPrint(res, "Failed to create the 3D view.\n");
			break;
		}

		if(IsFail(res = view3D[1].Create(812, 0, 1224, 384)))
		{
			ErrorPrint(res, "Failed to create the 3D view.\n");
			break;
		}

		// 각 3D 뷰의 시점을 동기화 한다. // Synchronize the viewpoint of each 3D view.
		if(IsFail(res = view3D[0].SynchronizePointOfView(&view3D[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}
		// 각 3D 뷰 윈도우의 위치를 동기화 한다 // Synchronize the position of each 3D view window
		if(IsFail(res = view3D[0].SynchronizeWindow(&view3D[1])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// 각각의 3D View 에서 0번 레이어 가져오기 // Get Layer 0 from each 3D view 
		for(int64_t i = 0; i < 2; ++i)
			layer3D[i] = view3D[i].GetLayer(0);

		// 각 레이어 캔버스에 텍스트 그리기 // Draw text to each Layer Canvas
		layer3D[0].DrawTextCanvas(CFLPoint<double>(3, 0), L"Figure A", YELLOW, BLACK, 20);
		layer3D[1].DrawTextCanvas(CFLPoint<double>(3, 0), L"Figure A", YELLOW, BLACK, 20);

		layer3D[0].DrawTextCanvas(CFLPoint<double>(3, 30), L"Base Plane", YELLOW, BLACK, 15);
		layer3D[1].DrawTextCanvas(CFLPoint<double>(3, 30), L"Length : +20", YELLOW, BLACK, 15);

		// Figure A 의 한쪽 면 생성 // Create one side of Figure A
		CFLPoint3<double> flpFigA0(0, 0, 0);
		CFLPoint3<double> flpFigA1(0, 10, 0);
		CFLPoint3<double> flpFigA2(10, 0, 0);
		CFLTriangle3<double> flqBasePlaneFigA(flpFigA0, flpFigA1, flpFigA2);
		CFLTriangularSolid3<double> flqsBasePlaneFigA(flqBasePlaneFigA, flqBasePlaneFigA);

		// 두 번째 평면은 첫 번째 평면의 법선 방향으로 `Length`만큼 떨어진 위치에 계산됩니다.
		// The second plane is calculated at a distance of `Length` in the normal direction of the first plane.
		double f64LengthA = 20.;
		CFLTriangularSolid3<double> flqsSolidFigA(flqBasePlaneFigA, f64LengthA);


		// 3D 뷰에 3D figure 추가 // Add 3D figures to the 3D view
		view3D[0].PushBackROI(&flqsBasePlaneFigA);
		view3D[1].PushBackROI(&flqsSolidFigA);

		// 추가한 3D 객체가 화면 안에 들어오도록 Zoom Fit // Perform Zoom Fit to ensure added 3D objects are within the view
		view3D[1].ZoomFit();

		// 3D 뷰어의 시점(카메라) 변경 // Change the viewpoint (camera) of the 3D viewer
		view3D[1].GetCamera()->SetPosition(CFLPoint3<float>(18.48, -32.13, 7.31));
		view3D[1].GetCamera()->SetDirection(CFLPoint3<float>(-0.38, 0.92, 0.06));
		view3D[1].GetCamera()->SetDirectionUp(CFLPoint3<float>(0.03, -0.05, 1.00));



		// Console 출력 // Console output
		wprintf(L"<Figure A>\n");
		wprintf(L"Base Plane : \n%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flqBasePlaneFigA).GetString());
		wprintf(L"Solid Figure : \n%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flqsSolidFigA).GetString());

		// 3D 뷰들을 갱신 합니다. // Update the 3D views.
		for(int32_t i = 0; i < 2; ++i)
		{
			view3D[i].UpdateScreen();
			view3D[i].Invalidate(true);
		}

		// 3D 뷰가 둘중에 하나라도 꺼지면 종료로 간주 // Consider closed when any of the two 3D views are turned off
		while(view3D[0].IsAvailable() && view3D[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}