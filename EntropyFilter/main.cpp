#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliDestinationImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageDestination;

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult eResult = EResult_UnknownError;

	do
	{
		// Source 이미지 로드 // Load the source image
		if((eResult = fliSourceImage.Load(L"../../ExampleImages/EntropyFilter/Cat3Ch.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination 이미지 로드 // Load the destination image
		if((eResult = fliDestinationImage.Load(L"../../ExampleImages/EntropyFilter/Cat3Ch.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Source 이미지 뷰 생성 // Create Source image view
		if((eResult = viewImageSource.Create(100, 0, 550, 480)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination 이미지 뷰 생성 // Create destination image view
		if((eResult = viewImageDestination.Create(550, 0, 1000, 480)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰와 Destination 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the source view and the destination view
		if((eResult = viewImageSource.SynchronizePointOfView(&viewImageDestination)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// Source 이미지 뷰에 이미지를 디스플레이 // Display the image in the source image view
		if((eResult = viewImageSource.SetImagePtr(&fliSourceImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination 이미지 뷰에 이미지를 디스플레이 // Display the image in the destination image view
		if((eResult = viewImageDestination.SetImagePtr(&fliDestinationImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if((eResult = viewImageSource.SynchronizeWindow(&viewImageDestination)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// EntropyFilter 객체 생성 // Create EntropyFilter object
		CEntropyFilter entropyFilter;

		// Source 이미지 설정 // Set the source image
		entropyFilter.SetSourceImage(fliSourceImage);

		// Destination 이미지 설정 // Set the destination image
		entropyFilter.SetDestinationImage(fliDestinationImage);

		// Kernel Width = 5 설정 // Set the Kernel Width = 5
		entropyFilter.SetKernelWidth(5);

		// Kernel Height = 5 설정 // Set the Kernel Height = 5
		entropyFilter.SetKernelHeight(5);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((eResult = entropyFilter.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute entropy filter.\n");
			wprintf(eResult.GetString());
			ErrorPrint(eResult, "\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImageDestination.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerDestination.Clear();

		// 이미지 뷰 정보 표시 // Display image view information
		if((eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if((eResult = layerDestination.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 // Update image view
		viewImageSource.Invalidate(true);
		viewImageDestination.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageSource.IsAvailable() && viewImageDestination.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}