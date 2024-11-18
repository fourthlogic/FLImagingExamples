#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{	// 3D ��ü ���� // Declare 3D object
	CFL3DObject floSrc;
	CFLImage fliDst;
	// 3D �� ���� // Declare 3D view	

	CGUIView3DWrap view3DSrc;
	CGUIViewImageWrap viewImgDst;

	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult res = EResult_UnknownError;


		// Source Object �ε� // Load the Source object
		if((res = floSrc.Load(L"../../ExampleImages/Projection3D/icosahedron.ply")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the object file.\n");
			break;
		}

		// Source 3D �� ����
		if(IsFail(res = view3DSrc.Create(612, 0, 1124, 512)))
		{
			ErrorPrint(res, "Failed to create the Destination 3D view.\n");
			break;
		}

		// Dst 3D �� ����
		if(IsFail(res = viewImgDst.Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(res, "Failed to create the Dst Image view.\n");
			break;
		}

		// Source Object 3D �� ���� // Create the source object 3D view
		if((res = view3DSrc.PushObject(floSrc)).IsFail())
		{
			ErrorPrint(res, L"Failed to display the 3D object.\n");
			break;
		}

		viewImgDst.SynchronizeWindow(&view3DSrc);

		

		// Projection3D ��ü ���� // Create Projection3D object
		CProjection3D Projection3D;

		// Destination object ���� // Set the destination object
		Projection3D.SetDestinationImage(fliDst);
		// Source object ���� // Set the source object
		Projection3D.SetSourceObject(floSrc);
		Projection3D.SetImageSize(512, 512);

		//3D View�� ī�޶� �Ķ���� ���� �ʱ�ȭ�ϱ� ���Ͽ� ���� ȣ��
		view3DSrc.ZoomFit();

		auto &cam = *view3DSrc.GetCamera();
		auto lToTPoint3 = [](const CFLPoint3F &pt)
		{
			return TPoint3F(pt.x, pt.y, pt.z);
		};

		TPoint3F ptCamPos = lToTPoint3(cam.GetPosition());
		TPoint3F ptCamDir = lToTPoint3(cam.GetDirection());
		TPoint3F ptCamDirUp = lToTPoint3(cam.GetDirectionUp());
		float f32AovX = cam.GetAngleOfViewX();
		float f32AovY = cam.GetAngleOfViewY();
		float f32TargetDistance = cam.GetDistanceFromTarget();

		Projection3D.SetCameraPosition(ptCamPos);
		Projection3D.SetCameraDirection(ptCamDir);
		Projection3D.SetDirectionUp(ptCamDirUp);

		Projection3D.SetAngleOfView(f32AovX, f32AovY);
		Projection3D.SetWorkingDistance(f32TargetDistance);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = Projection3D.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute Projection 3D.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately		

		CGUIView3DLayerWrap layer3DSource = view3DSrc.GetLayer(0);
		CGUIViewImageLayerWrap layer3DDst = viewImgDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer3DSource.Clear();
		layer3DDst.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		CFLPoint<double> flpTopLeft(0, 0);
		if((res = layer3DSource.DrawTextCanvas(&flpTopLeft, L"Source Object", YELLOW, BLACK, 20)).IsFail() ||
		   (res = layer3DDst.DrawTextCanvas(&flpTopLeft, L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		// 3D ������Ʈ �信 ��� Object�� �񱳸� ���� Source ������Ʈ ���÷���
		if((res = viewImgDst.SetImagePtr(&fliDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to set object on the 3d view.\n");
			break;
		}

		//view3DSrc.ZoomFit();
		viewImgDst.ZoomFit();

		// �̹��� �並 ���� �մϴ�. // Update image view
		view3DSrc.Invalidate(true);
		viewImgDst.Invalidate(true);

		// �̹��� ��, 3D �䰡 ����� �� ���� ��ٸ�
		while(view3DSrc.IsAvailable() && viewImgDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}

