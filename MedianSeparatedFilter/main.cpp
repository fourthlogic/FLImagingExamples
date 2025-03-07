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
        // 알고리즘 동작 결과 // Algorithmic executation result
		CResult res = EResult_UnknownError;

		// Source 이미지 로드 // Load the source image
		if(IsFail(res = fliSrcImage.Load(L"../../ExampleImages/NoiseImage/NoiseImage1.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}
		
		// Destination이미지를 Src 이미지와 동일한 이미지로 생성
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

		// Source이미지 뷰와 Dst 이미지 뷰의 초점을 맞춤
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

		// Source Image 크기에 맞게 view의 크기를 조정
		if(IsFail(res = viewImageSrc.ZoomFit()))
		{
			ErrorPrint(res, "Failed to zoom fit\n");
			break;
		}

		// Destination Image 크기에 맞게 view의 크기를 조정
		if(IsFail(res = viewImageDst.ZoomFit()))
		{
			ErrorPrint(res, "Failed to zoom fit\n");
			break;
		}

		// MedianSeparatedFilter 객체 생성 // Create MedianSeparatedFilter object
		CMedianSeparatedFilter medianSeparateFilter;

		CFLRect<int32_t> flrROI(100, 190, 360, 420);

		// Source 이미지 설정 // Set the source image
		medianSeparateFilter.SetSourceImage(fliSrcImage);

		// Source ROI 설정 // Set the source ROI
		medianSeparateFilter.SetSourceROI(flrROI);

		// Destination 이미지 설정 // Set the destination image
		medianSeparateFilter.SetDestinationImage(fliDstImage);

		// Destination ROI 설정
		medianSeparateFilter.SetDestinationROI(flrROI);

		// 처리할 Filter의 Kernel Size 설정 (KernelSize = 11 일 경우, Kernel Size : 11x11, 홀수만 설정가능)
		medianSeparateFilter.SetKernel(11);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(res = medianSeparateFilter.Execute()))
		{
			ErrorPrint(res, "Failed to execute morphology MedianSeparate.");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerDst.Clear();

		// ROI영역이 어디인지 알기 위해 디스플레이 한다 // Display to find out where ROI is
		// FLImaging의 Figure객체들은 어떤 도형모양이든 상관없이 하나의 함수로 디스플레이가 가능
		if(IsFail(res = layerSrc.DrawFigureImage(&flrROI, LIME)))
			ErrorPrint(res, "Failed to draw figure\n");

		if(IsFail(res = layerDst.DrawFigureImage(&flrROI, LIME)))
			ErrorPrint(res, "Failed to draw figure\n");

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