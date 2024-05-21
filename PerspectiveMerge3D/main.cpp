#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIView3DWrap view3DSrc;
	CGUIView3DWrap view3DSrc2;
	CGUIView3DWrap view3DDst;
	CGUIViewImageWrap viewTestDescription;
	CFL3DObject fl3DObjectSrc;
	CFL3DObject fl3DObjectSrc2;
	CFL3DObject fl3DObjectDst;
	CFLImage fliTestDescription;

	CResult res = EResult_UnknownError;

	do
	{
		// Source 3D 이미지 뷰 생성 // Create the Source 3D image view
		if((res = view3DSrc.Create(100, 0, 600, 500)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Source 3D 이미지 뷰 생성 // Create the Source 3D image view
		if((res = view3DSrc2.Create(600, 0, 1100, 500)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination 3D 이미지 뷰 생성 // Create the destination 3D image view
		if((res = view3DDst.Create(1100, 0, 1600, 500)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if((res = viewTestDescription.Create(100, 500, 600, 1000)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		res = fl3DObjectSrc.Load("../../ExampleImages/PerspectiveMerge3D/Left Cam.ply");
		res = fl3DObjectSrc2.Load("../../ExampleImages/PerspectiveMerge3D/Right Cam.ply");

		CPerspectiveMerge3D algemObject;

		TPoint3<float> tpPosition = TPoint3<float>(-0.152f, 0.0f, 0.0f);
		TPoint3<float> tpRotation = TPoint3<float>(-8.0f, 29.0f, 90.0f);
		TPoint3<float> tpPosition2 = TPoint3<float>(0.152f, 0.0f, 0.0f);
		TPoint3<float> tpRotation2 = TPoint3<float>(-8.0f, -29.0f, 90.0f);

		// 카메라 위치 설정 // Set the camera position
		algemObject.SetPosition(tpPosition, tpPosition2);
		// 카메라 회전정도 설정 // Set the camera degree
		algemObject.SetRotation(tpRotation, tpRotation2);

		// 카메라 1, 2의 Source 객체 설정 // Set the source object of camera 1, 2
		algemObject.SetSourceObject(&fl3DObjectSrc);
		algemObject.SetSourceObject2(&fl3DObjectSrc2);
		// Destination 객체 설정 // Set the destination object
		algemObject.SetDestinationObject(fl3DObjectDst);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((res = algemObject.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute MultiFocus.\n");
			break;
		}

		view3DSrc.PushObject(*algemObject.GetSourceObject());
		view3DSrc2.PushObject(*algemObject.GetSourceObject2());
		view3DDst.PushObject(*algemObject.GetDestinationObject());

		// Destination 이미지가 새로 생성됨으로 Zoom fit 을 통해 디스플레이 되는 이미지 배율을 화면에 맞춰준다. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		view3DSrc.ZoomFit();
		view3DSrc2.ZoomFit();
		view3DDst.ZoomFit();

		CGUIView3DLayerWrap layer3DSrc = view3DSrc.GetLayer(0);
		CGUIView3DLayerWrap layer3DSrc2 = view3DSrc2.GetLayer(0);
		CGUIView3DLayerWrap layer3DDst = view3DDst.GetLayer(0);

		CFLPoint<double> flp(0, 0);

		if((res = layer3DSrc.DrawTextCanvas(flp, L"Left Camera", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text.\n");
			break;
		}

		if((res = layer3DSrc2.DrawTextCanvas(flp, L"Right Camera", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text.\n");
			break;
		}
		if((res = layer3DDst.DrawTextCanvas(flp, L"Result", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text.\n");
			break;
		}

		fliTestDescription.Load(L"../../ExampleImages/PerspectiveMerge3D/Test Environment.flif");
		viewTestDescription.SetImagePtr(&fliTestDescription);
		CGUIViewImageLayerWrap layerTestDescription = viewTestDescription.GetLayer(0);

		if((res = layerTestDescription.DrawTextCanvas(flp, L"Test Environment", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text.\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		view3DSrc.Invalidate(true);
		view3DSrc2.Invalidate(true);
		view3DDst.Invalidate(true);
		viewTestDescription.Invalidate(true);

		// 이미지 뷰, 3D 뷰가 종료될 때 까지 기다림 // Wait for the image and 3D view to close
		while(view3DSrc.IsAvailable() || view3DSrc2.IsAvailable() || view3DDst.IsAvailable() || viewTestDescription.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}