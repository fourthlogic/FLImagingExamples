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

	 // 수행 결과 객체 선언 // Declare the execution result object
	CResult eResult = EResult_UnknownError;

	do
	{
		// Source 이미지 로드 // Load the source image
		if((eResult = fliSourceImage.Load(L"../../ExampleImages/OperationComplexDivide/ExampleSource.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Operand 이미지 로드 // Loads the operand image
		if((eResult = fliOperandImage.Load(L"../../ExampleImages/OperationComplexDivide/ExampleOperand.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination 이미지를 Source 이미지와 동일하게 설정 // Assign the Source image to Destination image
		if((eResult = fliDestinationImage.Assign(fliSourceImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Source 이미지 뷰 생성 // Create Source image view
		if((eResult = viewImageSource.Create(100, 0, 600, 500)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Operand 이미지 뷰 생성 // Creates operand image view
		if((eResult = viewImageOperand.Create(600, 0, 1100, 500)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination 이미지 뷰 생성 // Create destination image view
		if((eResult = viewImageDestination.Create(1100, 0,1600, 500)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰와 Operand 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the source view and the operand view
		if((eResult = viewImageSource.SynchronizePointOfView(&viewImageOperand)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
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

		// Operand 이미지 뷰에 이미지를 디스플레이 // Display the image in the operand image view
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

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if((eResult = viewImageSource.SynchronizeWindow(&viewImageOperand)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if((eResult = viewImageSource.SynchronizeWindow(&viewImageDestination)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// OperationComplexDivide 객체 생성 // Create OperationComplexDivide object
		COperationComplexDivide cd;

		// Source 이미지 설정 // Set the source image
		cd.SetSourceImage(fliSourceImage);

		// Operand 이미지 설정 // Set the operand image
		cd.SetOperandImage(fliOperandImage);
		
		// Destination 이미지 설정 // Set the destination image
		cd.SetDestinationImage(fliDestinationImage);

		// 오버플로 처리 방법 설정 // Set the overflow handling method
		cd.SetOverflowMethod(EOverflowMethod_Wrapping);

		// Image Operation 소스로 설정 // Set Operation Source to image
		cd.SetOperationSource(EOperationSource_Image);

		// 공백 색상 칠하기 모드 해제 // Set the Fill blank color mode false
		// 결과 이미지가 이미 존재할 경우 연산되지 않은 영역을 공백 색상으로 칠하지 않고 원본 그대로 둔다. // If the destination image already exists, the uncomputed area is left intact without being painted in a blank color.
		cd.EnableFillBlankColorMode(false);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((eResult = cd.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute operation complex divide.");
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