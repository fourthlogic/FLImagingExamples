﻿#include <cstdio>

#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h" 


int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliSourceImage;
	CFLImage fliConvertedImage;

	// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageConverted;

	do
	{
		// 동작 결과 // operation result
		CResult res = EResult_UnknownError;

		// 이미지 로드 // Loads image
		if(IsFail(res = fliSourceImage.Load(L"../../ExampleImages/Decolorization/Pattern.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImageSource.Create(112, 0, 912, 534)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageConverted.Create(913, 0, 1713, 534)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the image view
		if(IsFail(res = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageConverted.SetImagePtr(&fliConvertedImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views. 
		if(IsFail(res = viewImageSource.SynchronizePointOfView(&viewImageConverted)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageSource.SynchronizeWindow(&viewImageConverted)))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		// Decolorization 객체 생성 // Create Decolorization object
		CDecolorization Decolorization;

		// Source 이미지 설정 // Set source image 
		if(IsFail(res = Decolorization.SetSourceImage(fliSourceImage)))
		{
			ErrorPrint(res, "Failed to set Source Image.");
			break;
		}

		// Destination 이미지 설정 // Set destination image
		if(IsFail(res = Decolorization.SetDestinationImage(fliConvertedImage)))
		{
			ErrorPrint(res, "Failed to set Destination Image.");
			break;
		}

		// 연산 속도 설정 // Set the operation speed.
		if(IsFail(res = Decolorization.SetOperationSpeed(CDecolorization::EOperationSpeed_Normal)))
		{
			ErrorPrint(res, "Failed to set Operation Speed.");
			break;
		}

		// 알고리즘 수행 // Execute the algorithm
		if((res = Decolorization.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// 이미지 뷰의 zoom fit // image view zoom fit
		if((res = viewImageConverted.ZoomFit()).IsFail())
		{
			ErrorPrint(res, "Failed to zoom fit\n");
			break;
		}

		// 출력을 위한 이미지 레이어를 얻어옵니다. //  Gets the image layer for output.
		// 따로 해제할 필요 없음 // No need to release separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerConverted = viewImageConverted.GetLayer(1);

		// 기존에 Layer에 그려진 도형들을 삭제 // Delete the shapes drawn on the existing layer
		layerSource.Clear();
		layerConverted.Clear();

		// View 정보를 디스플레이 합니다. // Display View information.
		if(IsFail(res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerConverted.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update the image view.
		viewImageSource.Invalidate(true);
		viewImageConverted.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageSource.IsAvailable() && viewImageConverted.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}