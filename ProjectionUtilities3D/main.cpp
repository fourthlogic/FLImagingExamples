#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 3D �� ���� // Declare 3D view
	CGUIView3DWrap view3D;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	// �޼����� ���� ���� CMessageReceiver ��ü ���� // Create �޼����� ���� ���� CMessageReceiver object

	CResult res = EResult_UnknownError;

	do
	{
		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage.Create(400, 0, 800, 440)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// CProjectionUtilities3D ��ü ����
		CProjectionUtilities3D pu;

		// ī�޶� ���� ����
		CFL3DCamera cam;
		cam.SetProjectionType(E3DCameraProjectionType_Perspective);
		cam.SetPosition(CFLPoint3F(0.86f, -0.54f, 4.67f));
		cam.SetDirection(CFLPoint3F(-0.09f, 0.16f, -0.98f));
		cam.SetDirectionUp(CFLPoint3F(-0.99f, -0.09f, 0.08f));
		cam.SetAngleOfViewY(45.f);
		cam.SetTarget(CFLPoint3F(0.37f, 0.34f, -0.63f));
		cam.SetFarZ(9.70f);
		cam.SetNearZ(0.00f);
		pu.SetCamera(cam);

		// ��� �̹��� ũ�� ����
		pu.SetResultImageSize(400,400);

		// ��� �̹��� ��� ���� ����
		//pu.SetBackgroundc(400,400);

		// 3D Object ���� �ε�
		CFL3DObject* pObj3D = new CFL3DObject();
		pObj3D->Load(L"../../ExampleImages/ROIUtilities3D/Right Cam.ply");

		// CProjectionUtilities3D ��ü�� 3D Object �� �߰�
		pu.PushBack3DObject(pObj3D);

		// �������� ����
		res = pu.Execute();

		// ��� �̹��� ���
		CFLImage fliCr;
		res = pu.GetResult(fliCr);

		// ��� �̹��� �ε�
		viewImage.GetIntrinsicImage()->Assign(fliCr);
		viewImage.SetImagePtr(viewImage.GetIntrinsicImage());

		// 3D �� ����
		// Create 3D views.
		if(IsFail(res = view3D.Create(0, 0, 400, 440)))
		{
			ErrorPrint(res, "Failed to create the 3D view.\n");
			break;
		}

		if((res = view3D.PushObject(*pObj3D)).IsFail())
		{
			ErrorPrint(res, "Failed to display 3D object.\n");
			break;
		}

		// 3D �並 ���� // Update 3D view
		view3D.ZoomFit();
		view3D.UpdateScreen();

		if(pObj3D)
			delete pObj3D;

		while(view3D.IsAvailable() && viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}