﻿#include <cstdio>
#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h"

int main()
{
	CFLImage fliSrcImage;
	CFLImage fliDstImage;
	CFLImage fliTxtImage;
	CFL3DObjectHeightMap floDstObject;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;
	CGUIView3DWrap view3DDst;

	CResult res = EResult_UnknownError;

	do
	{
		// Source 이미지 로드 // Load the source image
		if((res = fliSrcImage.Load(L"../../ExampleImages/MultiFocusDerivativeBased3D/")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if((res = viewImageSrc.Create(100, 0, 600, 500)).IsFail() ||
		   (res = viewImageDst.Create(600, 0, 1100, 500)).IsFail() ||
		   (res = view3DDst.Create(400, 200, 1300, 800)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if((res = viewImageSrc.SynchronizePointOfView(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view. \n");
			break;
		}

		// 두 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		if((res = viewImageSrc.SetImagePtr(&fliSrcImage)).IsFail() ||
		   (res = viewImageDst.SetImagePtr(&fliDstImage)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		viewImageSrc.SetFixThumbnailView(true);


		// 알고리즘 객체 생성 // Create algorithm object
		CMultiFocusDerivativeBased3D algObject;

		if((res = algObject.SetSourceImage(fliSrcImage)).IsFail()) break;
		if((res = algObject.SetDestinationHeightMapImage(fliDstImage)).IsFail()) break;
		if((res = algObject.SetDestinationTextureImage(&fliTxtImage)).IsFail()) break;
		if((res = algObject.SetDestinationObject(floDstObject)).IsFail()) break;
		if((res = algObject.SetPixelAccuracy(0.1)).IsFail()) break;
		if((res = algObject.SetDepthPitch(0.2)).IsFail()) break;
		if((res = algObject.SetFilter(CMultiFocusDerivativeBased3D::EFilter_FLDenoisingType1)).IsFail()) break;
		if((res = algObject.SetFLDenoisingKernel(7)).IsFail()) break;
		if((res = algObject.SetFLDenoisingSigma(15.00)).IsFail()) break;
		if((res = algObject.SetFLDenoisingAmplitude(15.00)).IsFail()) break;
		if((res = algObject.EnableGaussianInterpolation(true)).IsFail())break;

		// 알고리즘 수행 // Execute the algorithm
		if((res = algObject.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute the algorithm.");
			break;
		}


		// floDstObject.SetTextureImage(fliTxtImage);
		// floDstObject.ActivateVertexColorTexture(true);

		// 3D 이미지 뷰에 Height Map (Destination Image) 이미지를 디스플레이 // Display the Height Map (Destination Image) on the 3D image view
		if((res = view3DDst.PushObject(floDstObject)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);
		CGUIView3DLayerWrap layer3D = view3DDst.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerDst.Clear();
		layer3D.Clear();

		// 이미지 뷰 정보 표시 // Display image view information
		CFLPoint<double> flp = new CFLPoint<double>();
		if((res = layerSrc.DrawTextCanvas(flp, L"Source Image", YELLOW, BLACK, 20)).IsFail() ||
		   (res = layerDst.DrawTextCanvas(flp, L"Destination Height Map Image", YELLOW, BLACK, 20)).IsFail() ||
		   (res = layer3D.DrawTextCanvas(flp, L"Destination Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		viewImageSrc.SetLayerAutoClearMode(ELayerAutoClearMode_PageChanged, false);

		// Zoom Fit
		viewImageSrc.ZoomFit();
		viewImageDst.ZoomFit();
		view3DDst.ZoomFit();

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);
		view3DDst.Invalidate(true);

		// 이미지 뷰, 3D 뷰가 종료될 때 까지 기다림
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable() && view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}