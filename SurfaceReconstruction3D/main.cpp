#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 3D ��ü ���� // Declare 3D object
	CFL3DObject floSourceObject;
	CFL3DObject floDestinationObject;

	// 3D �� ���� // Declare 3D view	
	CGUIView3DWrap view3DDst;
	CGUIView3DWrap view3DSource;


	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult eResult = EResult_UnknownError;

		// Source Object �ε� // Load the Source object
		if((eResult = floSourceObject.Load(L"../../ExampleImages/SurfaceReconstruction3D/Source.ply")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the object file.\n");
			break;
		}

		// Source 3D �� ����
		if(IsFail(eResult = view3DSource.Create(612, 0, 1124, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Source 3D view.\n");
			break;
		}

		// Dst 3D �� ����
		if(IsFail(eResult = view3DDst.Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Dst 3D view.\n");
			break;
		}

		if((eResult = view3DSource.PushObject(floSourceObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to display the 3D object.\n");
			break;
		}

		// SurfaceReconstruction3D ��ü ���� // Create SurfaceReconstruction3D object
		CSurfaceReconstruction3D SurfaceReconstruction3D;

		// Source object ���� // Set the source object
		SurfaceReconstruction3D.SetSourceObject(floSourceObject);

		// Destination object ���� // Set the destination object
		SurfaceReconstruction3D.SetDestinationObject(floDestinationObject);

		// Reconstruction Method ���� // Set the reconstruction method
		SurfaceReconstruction3D.SetReconstructionMethod(CSurfaceReconstruction3D::EReconstructionMethod_Explicit_RegionGrowing);

		// �ִ� �ݰ� �ڵ� ����ϵ��� ���� // Set to auto-calculate maximum radius
		SurfaceReconstruction3D.EnableExplicitAutoRadius(true);

		// �ִ� �ݰ� ��� ���� // Set the maximum radius coefficient
		SurfaceReconstruction3D.SetExplicitMaxRadiusCoefficient(5.000000f);

		// Hole filling �ִ� �ݰ� �ڵ� ����ϵ��� ���� // Set to auto-calculate hole filling maximum radius
		SurfaceReconstruction3D.EnableExplicitHoleFillingAutoRadius(true);

		// �ִ� �ݰ� ��� ���� // Set the maximum radius coefficient
		SurfaceReconstruction3D.SetExplicitHoleFillingRadiusCoefficient(1.000000f);

		// ���� ���� �ִ� ���� ���� // Set the maximum number of starting points
		SurfaceReconstruction3D.SetExplicitMaxSeed(10000);

		// �׸��� �� �Ÿ� �ڵ� ����ϵ��� ���� // Set to auto-calculate grid cell distance
		SurfaceReconstruction3D.EnableExplicitAutoCellRadius(true);

		// �׸��� �� �Ÿ� ��� ���� // Set the grid cell distance coefficient
		SurfaceReconstruction3D.SetExplicitCellRadiusCoefficient(1.000000f);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = SurfaceReconstruction3D.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute Surface Reconstruction 3D.");
			break;
		}

		if((eResult = view3DDst.PushObject(floDestinationObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to display the 3D object.\n");
			break;
		}

		view3DSource.SetShadingType(EShadingType3D_Phong);
		view3DDst.SetShadingType(EShadingType3D_Phong);

		view3DSource.ZoomFit();
		view3DDst.ZoomFit();


		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately		
		CGUIView3DLayerWrap layer3DDst = view3DDst.GetLayer(0);
		CGUIView3DLayerWrap layer3DSource = view3DSource.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer3DDst.Clear();
		layer3DSource.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic

		if((eResult = layer3DSource.DrawTextCanvas(CFLPoint<double>(0, 0), L"Source Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		if((eResult = layer3DDst.DrawTextCanvas(CFLPoint<double>(0, 0), L"Destination Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		view3DSource.ZoomFit();
		view3DDst.ZoomFit();

		view3DSource.SynchronizePointOfView(&view3DDst);

		// �̹��� �並 ���� �մϴ�. // Update image view
		view3DSource.Invalidate(true);
		view3DDst.Invalidate(true);

		// �̹��� ��, 3D �䰡 ����� �� ���� ��ٸ�
		while(view3DSource.IsAvailable() && view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}