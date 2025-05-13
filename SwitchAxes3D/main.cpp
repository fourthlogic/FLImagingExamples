﻿#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIView3DWrap view3DSrc;
	CGUIView3DWrap view3DDst;
	CFL3DObject fl3DObjectSrc;
	CFL3DObject fl3DObjectDst;

	do
	{
		// 알고리즘 동작 결과 // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Source 3D 뷰 생성 // Create the Source 3D view
		if((res = view3DSrc.Create(100, 0, 600, 500)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination 3D 뷰 생성 // Create the destination 3D view
		if((res = view3DDst.Create(600, 0, 1100, 500)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		fl3DObjectSrc.Load(L"../../ExampleImages/DistanceTransform3D/binary-vertex.ply");

		// Perspective Transform 3D 객체 생성 // Create Perspective Transform 3D object
		CSwitchAxes3D SwitchAxes3D;

		// 알고리즘 파라미터 설정 // set algorithm parameters
		SwitchAxes3D.SetSourceObject(fl3DObjectSrc);
		SwitchAxes3D.SetDestinationObject(fl3DObjectDst);
		SwitchAxes3D.SetAxisOrder(SwitchAxes3D.EAxisOrder_YZX);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((res = SwitchAxes3D.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute multiFocus.");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIView3DLayerWrap layer3DSrc = view3DSrc.GetLayer(0);
		CGUIView3DLayerWrap layer3DDst = view3DDst.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layer3DSrc.Clear();
		layer3DDst.Clear();

		// Destination 이미지가 새로 생성됨으로 Zoom fit 을 통해 디스플레이 되는 이미지 배율을 화면에 맞춰준다. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		view3DSrc.PushObject(fl3DObjectSrc);
		view3DSrc.ZoomFit();

		view3DDst.PushObject(fl3DObjectDst);
		view3DDst.ZoomFit();

		CFLPoint<double> flpTopLeft(0, 0);

		if((res = layer3DSrc.DrawTextCanvas(flpTopLeft, L"Source Object", YELLOW, BLACK, 20)).IsFail() ||
		   (res = layer3DDst.DrawTextCanvas(flpTopLeft, L"Destination Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		view3DSrc.Invalidate(true);
		view3DDst.Invalidate(true);

		// 이미지 뷰, 3D 뷰가 종료될 때 까지 기다림
		while(view3DSrc.IsAvailable() || view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}