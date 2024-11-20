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
		if((res = view3DSrc.Create(612, 0, 1124, 512)).IsFail())
		{
			ErrorPrint(res, "Failed to create the Source 3D view.\n");
			break;
		}

		// Dst 3D �� ����
		if((res = viewImgDst.Create(1124, 0, 1636, 512)).IsFail())
		{
			ErrorPrint(res, "Failed to create the Destination Image view.\n");
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
		CProjection3D projection3D;

		// Destination object ���� // Set the destination object
		projection3D.SetDestinationImage(fliDst);
		// Source object ���� // Set the source object
		projection3D.SetSourceObject(floSrc);
		projection3D.SetImageSize(512, 512);

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

		projection3D.SetCameraPosition(ptCamPos);
		projection3D.SetCameraDirection(ptCamDir);
		projection3D.SetDirectionUp(ptCamDirUp);

		projection3D.SetAngleOfView(f32AovX, f32AovY);
		projection3D.SetWorkingDistance(f32TargetDistance);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = projection3D.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute Projection 3D.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� View���� ���̾� 0���� ���� // Obtain layer number 0 from view for display
		// �� ��ü�� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to the view and does not need to be released separately		

		CGUIView3DLayerWrap layer3DSrc = view3DSrc.GetLayer(0);
		CGUIViewImageLayerWrap layer3DDst = viewImgDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer3DSrc.Clear();
		layer3DDst.Clear();

		// View ������ ���÷����Ѵ�. // Display view information
		// �Լ� DrawTextCanvas�� Screen ��ǥ�� �������� �ϴ� ���ڿ��� ������Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		CFLPoint<double> flpTopLeft(0, 0);
		if((res = layer3DSrc.DrawTextCanvas(&flpTopLeft, L"Source Object", YELLOW, BLACK, 20)).IsFail() ||
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

		//Source View�� ������ ������ �����ϱ� ����, �̹����� ���߾��� ���� �߾ӿ� ���߰� ������ 1�� ����
		viewImgDst.MoveCenterImageCoord(CFLPointD(256, 256), 1.0);

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

