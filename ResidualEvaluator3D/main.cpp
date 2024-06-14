#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 3D ��ü ���� // Declare 3D object
	CFL3DObject fl3DOReferenceObject;
	CFL3DObject fl3DOTargetObject;

	// 3D �� ���� // Declare 3D view	
	CGUIView3DWrap view3DReference;
	CGUIView3DWrap view3DTarget;


	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult eResult = EResult_UnknownError;


		// Reference Object �ε� // Load the Reference object
		if((eResult = fl3DOReferenceObject.Load(L"../../ExampleImages/ResidualEvaluator3D/ReferencePoints.ply")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the object file.\n");
			break;
		}

		// Target Object �ε� // Load the Target object
		if((eResult = fl3DOTargetObject.Load(L"../../ExampleImages/ResidualEvaluator3D/MeasuredPoints.ply")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the object file.\n");
			break;
		}

		// Reference 3D �� ����
		if(IsFail(eResult = view3DReference.Create(612, 0, 1124, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Destination 3D view.\n");
			break;
		}

		// Target 3D �� ����
		if(IsFail(eResult = view3DTarget.Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Reference 3D view.\n");
			break;
		}

		// Reference Object 3D �� ���� // Create the reference object 3D view
		if((eResult = view3DReference.PushObject(fl3DOReferenceObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to display the 3D object.\n");
			break;
		}

		// Target Object 3D �� ���� // Create the Target object 3D view
		if((eResult = view3DTarget.PushObject(fl3DOTargetObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to display the 3D object.\n");
			break;
		}

		// ResidualEvaluator3D ��ü ���� // Create ResidualEvaluator3D object
		CResidualEvaluator3D ResidualEvaluator3D;

		// Destination object ���� // Set the destination object
		ResidualEvaluator3D.SetLearnObject(fl3DOReferenceObject);
		// Target object ���� // Set the target object
		ResidualEvaluator3D.SetSourceObject(fl3DOTargetObject);
		// �ִ� ��� ���� ���� // Set the max count of match result
		ResidualEvaluator3D.SetResidualType(CResidualEvaluator3D::EResidualType::EResidualType_RMSE);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = ResidualEvaluator3D.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute Surface Match 3D.");
			break;
		}

		view3DTarget.ZoomFit();
		view3DReference.ZoomFit();

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately		
		CGUIView3DLayerWrap layer3DReference = view3DReference.GetLayer(0);
		CGUIView3DLayerWrap layer3DTarget = view3DTarget.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer3DReference.Clear();
		layer3DTarget.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic

		if((eResult = layer3DReference.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Reference Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		if((eResult = layer3DTarget.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Target Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		CFLString<wchar_t> strText;

		strText.Format(L"Residual : %lf", ResidualEvaluator3D.GetResidual());
		Base::TPoint3<float> tp3Min, tp3Max;
		Base::TPoint3<double> tp3Center;

		fl3DOTargetObject.GetBoundingBox(tp3Min, tp3Max);

		tp3Center.x = (tp3Min.x + tp3Max.x) / 2;
		tp3Center.y = (tp3Min.y + tp3Max.y) / 2;
		tp3Center.z = (tp3Min.z + tp3Max.z) / 2;

		// ������ ���� ��� �� score ���
		if((eResult = layer3DTarget.DrawText3D(tp3Center, strText, YELLOW, 0, 9)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		view3DTarget.ZoomFit();
		view3DReference.ZoomFit();

		// �̹��� �並 ���� �մϴ�. // Update image view
		view3DTarget.Invalidate(true);
		view3DReference.Invalidate(true);

		// �̹��� ��, 3D �䰡 ����� �� ���� ��ٸ�
		while(view3DTarget.IsAvailable() && view3DReference.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}