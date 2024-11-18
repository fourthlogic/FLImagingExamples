#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{	// 3D 객체 선언 // Declare 3D object
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
		if(IsFail(res = view3DSrc.Create(612, 0, 1124, 512)))
		{
			ErrorPrint(res, "Failed to create the Destination 3D view.\n");
			break;
		}

		// Dst 3D 뷰 생성
		if(IsFail(res = viewImgDst.Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(res, "Failed to create the Dst Image view.\n");
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
		CProjection3D Projection3D;

		// Destination object 설정 // Set the destination object
		Projection3D.SetDestinationImage(fliDst);
		// Source object 설정 // Set the source object
		Projection3D.SetSourceObject(floSrc);
		Projection3D.SetImageSize(512, 512);

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

		Projection3D.SetCameraPosition(ptCamPos);
		Projection3D.SetCameraDirection(ptCamDir);
		Projection3D.SetDirectionUp(ptCamDirUp);

		Projection3D.SetAngleOfView(f32AovX, f32AovY);
		Projection3D.SetWorkingDistance(f32TargetDistance);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((res = Projection3D.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute Projection 3D.");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately		

		CGUIView3DLayerWrap layer3DSource = view3DSrc.GetLayer(0);
		CGUIViewImageLayerWrap layer3DDst = viewImgDst.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layer3DSource.Clear();
		layer3DDst.Clear();

		// View 정보를 디스플레이 한다. // Display view information
		// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		CFLPoint<double> flpTopLeft(0, 0);
		if((res = layer3DSource.DrawTextCanvas(&flpTopLeft, L"Source Object", YELLOW, BLACK, 20)).IsFail() ||
		   (res = layer3DDst.DrawTextCanvas(&flpTopLeft, L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		// 3D 오브젝트 뷰에 결과 Object와 비교를 위한 Source 오브젝트 디스플레이
		if((res = viewImgDst.SetImagePtr(&fliDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to set object on the 3d view.\n");
			break;
		}

		//view3DSrc.ZoomFit();
		viewImgDst.ZoomFit();

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

