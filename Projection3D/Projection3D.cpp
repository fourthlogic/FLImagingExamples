#include <cstdio>
#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h"

int main()
{
	// You must call the following function once
	// before using any features of the FLImaging(R) library
	CLibraryUtilities::Initialize();

	// 3D 객체 선언 // Declare 3D object
	CFL3DObject floSrc;
	CFLImage fliDst;
	// 3D 뷰 선언 // Declare 3D view	

	CGUIView3DWrap view3DSrc;
	CGUIViewImageWrap viewImgDst;

	do
	{
		// 알고리즘 동작 결과 // Algorithm execution result
		CResult res = EResult_UnknownError;


		// Source Object 로드 // Load the Source object
		if((res = floSrc.Load(L"../../ExampleImages/Projection3D/icosahedron.ply")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the object file.\n");
			break;
		}

		// Source 3D 뷰 생성
		if((res = view3DSrc.Create(612, 0, 1124, 512)).IsFail())
		{
			ErrorPrint(res, "Failed to create the Source 3D view.\n");
			break;
		}

		// Dst 3D 뷰 생성
		if((res = viewImgDst.Create(1124, 0, 1636, 512)).IsFail())
		{
			ErrorPrint(res, "Failed to create the Destination Image view.\n");
			break;
		}

		// Source Object 3D 뷰 생성 // Create the source object 3D view
		if((res = view3DSrc.PushObject(floSrc)).IsFail())
		{
			ErrorPrint(res, L"Failed to display the 3D object.\n");
			break;
		}

		viewImgDst.SynchronizeWindow(&view3DSrc);

		

		// Projection3D 객체 생성 // Create Projection3D object
		CProjection3D projection3D;

		// Destination object 설정 // Set the destination object
		projection3D.SetDestinationImage(fliDst);
		// Source object 설정 // Set the source object
		projection3D.SetSourceObject(floSrc);
		projection3D.SetImageSize(512, 512);

		//3D View의 카메라 파라미터 값을 초기화하기 위하여 먼저 호출
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

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((res = projection3D.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute Projection 3D.");
			break;
		}

		// 화면에 출력하기 위해 View에서 레이어 0번을 얻어옴 // Obtain layer number 0 from view for display
		// 이 객체는 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to the view and does not need to be released separately		

		CGUIView3DLayerWrap layer3DSrc = view3DSrc.GetLayer(0);
		CGUIViewImageLayerWrap layer3DDst = viewImgDst.GetLayer(0);

		// View 정보를 디스플레이한다. // Display view information
		// 함수 DrawTextCanvas는 Screen 좌표를 기준으로 하는 문자열을 드로잉한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		CFLPoint<double> flpTopLeft(0, 0);
		if((res = layer3DSrc.DrawTextCanvas(flpTopLeft, L"Source Object", YELLOW, BLACK, 20)).IsFail() ||
		   (res = layer3DDst.DrawTextCanvas(flpTopLeft, L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		// 결과 이미지를 뷰에 연결 // Map the result image to the destination view
		if((res = viewImgDst.SetImagePtr(&fliDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to set object on the 3d view.\n");
			break;
		}

		//Source View와 동일한 시점을 유지하기 위해, 이미지의 정중앙을 뷰의 중앙에 맞추고 배율을 1로 설정
		viewImgDst.SetViewCenterAndScale(CFLPointD(256, 256), 1.0);

		// 이미지 뷰를 갱신 합니다. // Update image view
		view3DSrc.Invalidate(true);
		viewImgDst.Invalidate(true);

		// 이미지 뷰, 3D 뷰가 종료될 때 까지 기다림
		while(view3DSrc.IsAvailable() && viewImgDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}

