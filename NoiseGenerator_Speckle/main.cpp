#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliImage;
	CFLImage fliDestinationImage;

	// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImage[2];
	CResult eResult;

	do
	{
		// 이미지 로드 // Loads image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/NoiseGenerator/Plate.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(eResult = fliDestinationImage.Load(L"../../ExampleImages/NoiseGenerator/Plate.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Creates imageview
		if(IsFail(eResult = viewImage[0].Create(400, 0, 912, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].Create(912, 0, 1424, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다. // Synchronize the viewpoints of the two image views.
		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the imageview
		if(IsFail(eResult = viewImage[0].SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].SetImagePtr(&fliDestinationImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		CResult eResult = EResult_UnknownError;

		// Noise Generator 객체 생성 // Create Noise Generator object
		CNoiseGenerator noiseGenerator;

		// ROI 범위 설정 // Set ROI range
		CFLRectL flrROI(61, 63, 583, 376);

		// 처리할 이미지 설정 // Set the image to process
		noiseGenerator.SetSourceImage(fliImage);
		noiseGenerator.SetDestinationImage(fliDestinationImage);
		// 처리할 ROI 설정 // Set the ROI to be processed
		noiseGenerator.SetSourceROI(flrROI);
		noiseGenerator.SetDestinationROI(flrROI);

		// 생성할 노이즈 설정 // Set the noise to generate
		noiseGenerator.SetNoiseType(CNoiseGenerator::ENoiseType_SpeckleNoise);
		// 간섭 잡음의 비율 설정 // Set the ratio of speckle noise
		noiseGenerator.SetSpeckleNoise(0.05);

		// 알고리즘 수행 // Execute the algorithm
		if(IsFail(eResult = noiseGenerator.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute noise generator.\n");
			break;
		}

		CGUIViewImageLayerWrap layer = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImage[1].GetLayer(0);

		layer.Clear();
		layerDst.Clear();

		// ROI영역이 어디인지 알기 위해 디스플레이 한다 // Display to know where the ROI area is
		if(IsFail(eResult = layer.DrawFigureImage(&flrROI, LIME)))
		{
			ErrorPrint(eResult, "Failed to draw figure\n");
			break;
		}

		if(IsFail(eResult = layerDst.DrawFigureImage(&flrROI, LIME)))
		{
			ErrorPrint(eResult, "Failed to draw figure\n");
			break;
		}

		// 이미지 뷰 정보 표시 // Display image view information
		if(IsFail(eResult = layer.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerDst.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update the image view.
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}