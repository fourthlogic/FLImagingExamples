#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


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
		CResult eResult = EResult_UnknownError;

		// 이미지 로드 // Loads image
		if(IsFail(eResult = fliSourceImage.Load(L"../../ExampleImages/ColorConverter/city_3.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(eResult = viewImageSource.Create(112, 0, 912, 534)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageConverted.Create(913, 0, 1713, 534)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the image view
		if(IsFail(eResult = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageConverted.SetImagePtr(&fliConvertedImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views. 
		if(IsFail(eResult = viewImageSource.SynchronizePointOfView(&viewImageConverted)))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImageSource.SynchronizeWindow(&viewImageConverted)))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		// ColorConverter 객체 생성 // Create ColorConverter object
		CColorConverter ColorConverter;

		// Source 이미지 설정 // Set source image 
		if(IsFail(eResult = ColorConverter.SetSourceImage(fliSourceImage)))
		{
			ErrorPrint(eResult, "Failed to set Source Image.");
			break;
		}

		// Destination 이미지 설정 // Set destination image
		if(IsFail(eResult = ColorConverter.SetDestinationImage(fliConvertedImage)))
		{
			ErrorPrint(eResult, "Failed to set Destination Image.");
			break;
		}

		// 변환할 픽셀 포멧 입력(C3_U8 -> C4_U8) // Set the pixel format to convert(C3_U8 -> C4_U8)
		if(IsFail(eResult = ColorConverter.SetPixelFormat(EPixelFormat_C4_U8)))
		{
			ErrorPrint(eResult, "Failed to set PixelFormat.");
			break;
		}

		// 변환할 픽셀 포멧 입력 (RGB -> ARGB) // Set the color sequence to convert (RGB -> ARGB)
		if(IsFail(eResult = ColorConverter.SetColorSequence(EColorSequence_ARGB)))
		{
			ErrorPrint(eResult, "Failed to set PixelFormat.");
			break;
		}

		// 알고리즘 수행 // Execute the algorithm
		if((eResult = ColorConverter.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to process.");
			break;
		}

		// 이미지 뷰의 zoom fit // image view zoom fit
		if((eResult = viewImageConverted.ZoomFit()).IsFail())
		{
			ErrorPrint(eResult, "Failed to zoom fit\n");
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
		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image (BGR)", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerConverted.DrawTextCanvas(&CFLPointD(0, 0), L"Converted Image (ARGB)", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
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