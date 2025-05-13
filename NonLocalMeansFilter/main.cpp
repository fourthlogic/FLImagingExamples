﻿#include <cstdio>
#include "../CommomHeader/ErrorPrint.h"
#include <FLImaging.h>


int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliSrcImage;
	CFLImage fliDstImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	do
	{
        // 알고리즘 동작 결과 // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Source 이미지 로드 // Load the source image
		if(IsFail(res = fliSrcImage.Load(L"../../ExampleImages/NoiseImage/NoiseImage1.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}
		
		// Destination 이미지를 Source 이미지와 동일한 이미지로 생성 // Create destination image as same as source image
		if(IsFail(res = fliDstImage.Assign(fliSrcImage)))
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Source 이미지 뷰 생성 // Create the source image view
		if(IsFail(res = viewImageSrc.Create(400, 0, 912, 612)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰에 이미지를 디스플레이 // Display the image in the source image view
		if(IsFail(res = viewImageSrc.SetImagePtr(&fliSrcImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination 이미지 뷰 생성 // Create the destination image view
		if(IsFail(res = viewImageDst.Create(912, 0, 1424, 612)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination 이미지 뷰에 이미지를 디스플레이 // Display the image in the destination image view
		if(IsFail(res = viewImageDst.SetImagePtr(&fliDstImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(res = viewImageSrc.SynchronizePointOfView(&viewImageDst)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageSrc.SynchronizeWindow(&viewImageDst)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Source Image 크기에 맞게 view의 크기를 조정 // Zoom the view to fit the image size
		if(IsFail(res = viewImageSrc.ZoomFit()))
		{
			ErrorPrint(res, "Failed to zoom fit\n");
			break;
		}

		// Destination Image 크기에 맞게 view의 크기를 조정 // Zoom the view to fit the image size
		if(IsFail(res = viewImageDst.ZoomFit()))
		{
			ErrorPrint(res, "Failed to zoom fit\n");
			break;
		}

		// NonLocalMeansFilter 객체 생성 // Create NonLocalMeansFilter object
		CNonLocalMeansFilter nonLocalMeansFilter;

		// Source 이미지 설정 // Set the source image
		nonLocalMeansFilter.SetSourceImage(fliSrcImage);

		// Destination 이미지 설정 // Set the destination image
		nonLocalMeansFilter.SetDestinationImage(fliDstImage);

		// 처리할 Filter의 Patch Radius 설정 // Set the patch radius of the filter
		nonLocalMeansFilter.SetPatchRadius(1);

		// 처리할 Filter의 Window Radius 설정 // Set the window radius of the filter
		nonLocalMeansFilter.SetWindowRadius(3);

		// Sigma (노이즈의 표준편차) 설정 // Set the sigma (standard deviation of the noise)
		nonLocalMeansFilter.SetSigma(0.3);

		// H (필터링 파라미터) 설정 // Set the H (filtering parameter)
		nonLocalMeansFilter.SetH(0.3);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(res = nonLocalMeansFilter.Execute()))
		{
			ErrorPrint(res, "Failed to execute Non Local Means Filter.");
			break;
		}

		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);

		if(IsFail(res = layerSrc.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerDst.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}