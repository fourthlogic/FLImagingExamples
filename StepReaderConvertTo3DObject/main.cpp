#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 3D �� ����
	CGUIView3DWrap view3D[2];

	CResult res = EResult_UnknownError;


	do
	{
		// 3D �� ����
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

		view3D[0].SetTopologyType(ETopologyType3D_Wireframe);
		view3D[1].SetTopologyType(ETopologyType3D_Wireframe);

		//��� �������� �ڵ�(Chord, ��� �� ���� �������� ������ ����)�� ��� �� �ִ� �ִ� �Ÿ��� ��Ÿ���ϴ�. 
		// �� ���� Ŭ���� ���ҵ� �ﰢ���� ���� ��������, 
		// �ݴ�� ���� �������� �� ���� �ﰢ���� �����Ǿ� ��� �� �����ϰ� �ٻ��մϴ�. 
		// �⺻�� 0�� �Է��ϸ� step �𵨿��� ������ chordal deviation ���� �ڵ����� ����մϴ�.
		// It represents the maximum distance that a chord (a straight line segment connecting two points on the curve) can deviate from the tangent of the curve. 
		// A larger value results in fewer triangles being formed, 
		// while a smaller value results in more triangles, providing a more precise approximation of the curve. 
		// The default value of 0 automatically calculates the appropriate chordal deviation value based on the imported step model.
		double f64ChordalDeviation = 0.;

		// ��� 1. CFL3DObject ���� Step ���� �ε�
		// Method 1. Load the STEP file in CFL3DObject
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


		// ��� 2. CStepReader ���� Step ���� �ε� �� GetResult3DObject() �� CFL3DObject �� �Ҵ�
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

		// �Ʒ� �Լ� DrawTextCanvas()�� screen��ǥ�� �������� �ϴ� string�� drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
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

		// 3D �並 ���� // Update 3D view
		view3D[0].UpdateScreen();
		view3D[1].UpdateScreen();

		// 3D �䰡 ����� �� ���� ��ٸ� // Wait for the 3D view 
		while(view3D[0].IsAvailable() && view3D[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}