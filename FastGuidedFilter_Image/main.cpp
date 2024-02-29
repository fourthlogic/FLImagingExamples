#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliOperandImage;
	CFLImage fliDestinationImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageOperand;
	CGUIViewImageWrap viewImageDestination;

	CResult eResult = EResult_UnknownError;

	do
	{
		// Source 이미지 로드 // Load the source image
		if((eResult = fliSourceImage.Load(L"../../ExampleImages/GuidedFilter/cat_450x480.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Operand 이미지 로드 // Loads the operand image
		if((eResult = fliOperandImage.Load(L"../../ExampleImages/GuidedFilter/cat_450x480.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination 이미지 로드 // Load the destination image
		if((eResult = fliDestinationImage.Load(L"../../ExampleImages/GuidedFilter/cat_450x480.flif")).IsFail())
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

		// Operand 이미지 뷰 생성 // Creates operand image view
		if((eResult = viewImageOperand.Create(550, 0, 1000, 480)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination 이미지 뷰 생성 // Create destination image view
		if((eResult = viewImageDestination.Create(1000, 0, 1450, 480)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰와 Operand 이미지 뷰의 시점을 동기화 한다
		if((eResult = viewImageSource.SynchronizePointOfView(&viewImageOperand)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// Source 이미지 뷰와 Destination 이미지 뷰의 시점을 동기화 한다
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

		// Operand 이미지 뷰에 이미지를 디스플레이
		if((eResult = viewImageOperand.SetImagePtr(&fliOperandImage)).IsFail())
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
		if((eResult = viewImageSource.SynchronizeWindow(&viewImageOperand)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if((eResult = viewImageSource.SynchronizeWindow(&viewImageDestination)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		//알고리즘 동작 결과
		CResult eResult = EResult_UnknownError;

		// FastGuidedFilter 객체 생성 // Create FastGuidedFilter object
		CFastGuidedFilter fastGuidedFilter;

		// Source 이미지 설정 // Set the source image
		fastGuidedFilter.SetSourceImage(fliSourceImage);

		// Operand 이미지 설정 // Set the operand image
		fastGuidedFilter.SetOperandImage(fliOperandImage);

		// Destination 이미지 설정 // Set the destination image
		fastGuidedFilter.SetDestinationImage(fliDestinationImage);

		// Image Operation 소스로 설정 // Set Operation Source to image
		fastGuidedFilter.SetOperationSource(EOperationSource_Image);

		// filter mask radius = 3 설정
		fastGuidedFilter.SetRadius(3);

		// amplitude = 20 설정
		fastGuidedFilter.SetAmplitude(20);

		// Sampling Factor = 5 설정
		fastGuidedFilter.SetSamplingFactor(5);

		// Interpolation = true 설정
		fastGuidedFilter.EnableInterpolation(true);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((eResult = fastGuidedFilter.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute fast guided filter.\n");
			wprintf(eResult.GetString());
			ErrorPrint(eResult, "\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerOperand = viewImageOperand.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImageDestination.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerOperand.Clear();
		layerDestination.Clear();

		// 이미지 뷰 정보 표시 // Display image view information
		if((eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if((eResult = layerOperand.DrawTextCanvas(&CFLPointD(0, 0), L"Operand Image", YELLOW, BLACK, 30)).IsFail())
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
		viewImageOperand.Invalidate(true);
		viewImageDestination.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageSource.IsAvailable() && viewImageOperand.IsAvailable() && viewImageDestination.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}