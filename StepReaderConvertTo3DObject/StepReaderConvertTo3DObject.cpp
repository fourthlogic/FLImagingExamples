#include <cstdio>

#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h"


int main()
{
	CResult res = EResult_UnknownError;

	do
	{
		// 3D 뷰 선언
		// Declare 3D views.
		CGUIView3DWrap view3D[2];

		// 3D 뷰 생성
		// Create 3D views.
		if(IsFail(res = view3D[0].Create(100, 0, 612, 512)))
		{
			ErrorPrint(res, "Failed to create the 3D view.\n");
			break;
		}

		if(IsFail(res = view3D[1].Create(612, 0, 1124, 512)))
		{
			ErrorPrint(res, "Failed to create the 3D view.\n");
			break;
		}

		view3D[0].SynchronizeWindow(&view3D[1]);
		view3D[0].SetTopologyType(ETopologyType3D_Wireframe);
		view3D[1].SetTopologyType(ETopologyType3D_Wireframe);

		// 곡선의 접선에서 코드(Chord, 곡선의 두 점을 직선으로 연결한 선분)가 벗어날 수 있는 최대 거리를 나타냅니다. 
		// 이 값이 클수록 분할된 삼각형의 수가 적어지며, 
		// 반대로 값이 작을수록 더 많은 삼각형이 형성되어 곡선을 더 정밀하게 근사합니다. 
		// 기본값 0을 입력하면 step 모델에서 적절한 chordal deviation 값을 자동으로 계산합니다.
		// It represents the maximum distance that a chord (a straight line segment connecting two points on the curve) can deviate from the tangent of the curve. 
		// A larger value results in fewer triangles being formed, 
		// while a smaller value results in more triangles, providing a more precise approximation of the curve. 
		// The default value of 0 automatically calculates the appropriate chordal deviation value based on the imported step model.
		double f64ChordalDeviation = 0.;

		// 방법 1. CFL3DObject 에서 Step 파일 로드
		// Method 1. Load the STEP file directly into a CFL3DObject
		CFL3DObject fl3DObj0;

		if((res = fl3DObj0.LoadSTEP("../../ExampleImages/StepReaderConvertTo3DObject/Cylinder.step", f64ChordalDeviation)).IsFail())
		{
			ErrorPrint(res, "Failed to load step file.\n");
			break;
		}

		if((res = view3D[0].PushObject(fl3DObj0)).IsFail())
		{
			ErrorPrint(res, "Failed to display 3D object.\n");
			break;
		}


		// 방법 2. CStepReader 에서 Step 파일 로드 후 GetResult3DObject() 로 CFL3DObject 에 할당
		// Method 2. Load the STEP file using CStepReader and then assign it to CFL3DObject using GetResult3DObject().
		CFL3DObject fl3DObj1;
		CStepReader sr;
		f64ChordalDeviation = 0.00001;

		if((res = sr.Load("../../ExampleImages/StepReaderConvertTo3DObject\\Cylinder.step")).IsFail())
		{
			ErrorPrint(res, "Failed to load step file.\n");
			break;
		}

		if((res = sr.GetResult3DObject(fl3DObj1, f64ChordalDeviation)).IsFail())
		{
			ErrorPrint(res, "Failed to get 3D object from the StepReader.\n");
			break;
		}

		if((res = view3D[1].PushObject(fl3DObj1)).IsFail())
		{
			ErrorPrint(res, "Failed to display 3D object.\n");
			break;
		}

		view3D[0].ZoomFit();
		view3D[1].ZoomFit();

		// 아래 함수 DrawTextCanvas()는 screen좌표를 기준으로 하는 string을 drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		CFLPoint<double> flp(0, 0);
		if(IsFail(res = view3D[0].GetLayer(2).DrawTextCanvas(&flp, L"Chordal Deviation = 0.0", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3D[1].GetLayer(2).DrawTextCanvas(&flp, L"Chordal Deviation = 0.00001", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		view3D[0].SetCanvasColor(WHITE);
		view3D[1].SetCanvasColor(WHITE);

		// 3D 뷰를 갱신 // Update 3D view
		view3D[0].UpdateScreen();
		view3D[1].UpdateScreen();

		// 3D 뷰가 종료될 때 까지 기다림 // Wait for the 3D view 
		while(view3D[0].IsAvailable() && view3D[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}