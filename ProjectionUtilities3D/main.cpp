#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 3D 뷰 선언 // Declare 3D view
	CGUIView3DWrap view3D;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage;

	// 메세지를 전달 받을 CMessageReceiver 객체 생성 // Create 메세지를 전달 받을 CMessageReceiver object

	CResult res = EResult_UnknownError;

	do
	{
		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage.Create(400, 0, 800, 440)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// CProjectionUtilities3D 객체 생성
		CProjectionUtilities3D pu;

		// 카메라 시점 설정
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

		// 결과 이미지 크기 설정
		pu.SetResultImageSize(400,400);

		// 결과 이미지 배경 색상 설정
		//pu.SetBackgroundc(400,400);

		// 3D Object 파일 로드
		CFL3DObject* pObj3D = new CFL3DObject();
		pObj3D->Load(L"../../ExampleImages/ROIUtilities3D/Right Cam.ply");

		// CProjectionUtilities3D 객체에 3D Object 를 추가
		pu.PushBack3DObject(pObj3D);

		// 프로젝션 수행
		res = pu.Execute();

		// 결과 이미지 얻기
		CFLImage fliCr;
		res = pu.GetResult(fliCr);

		// 결과 이미지 로드
		viewImage.GetIntrinsicImage()->Assign(fliCr);
		viewImage.SetImagePtr(viewImage.GetIntrinsicImage());

		// 3D 뷰 생성
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

		// 3D 뷰를 갱신 // Update 3D view
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