#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliDestinationImage0;
	CFLImage fliDestinationImage1;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst0;
	CGUIViewImageWrap viewImageDst1;

	 // 수행 결과 객체 선언 // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		// 이미지 로드 // Loads image
		if(IsFail(res = fliSourceImage.Load(L"../../ExampleImages/OperationSoftplus/Coord1D.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image views
		if ((res = viewImageSrc.Create(100, 0, 600, 545)).IsFail() ||
			(res = viewImageDst0.Create(600, 0, 1100, 545)).IsFail() ||
			(res = viewImageDst1.Create(1100, 0, 1600, 545)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화한다 // Synchronize the viewpoints of the two image views
		if ((res = viewImageSrc.SynchronizePointOfView(&viewImageDst0)).IsFail() ||
			(res = viewImageSrc.SynchronizePointOfView(&viewImageDst1)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the images in the image views
		if ((res = viewImageSrc.SetImagePtr(&fliSourceImage)).IsFail() ||
			(res = viewImageDst0.SetImagePtr(&fliDestinationImage0)).IsFail() ||
			(res = viewImageDst1.SetImagePtr(&fliDestinationImage1)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화한다 // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageDst0)).IsFail() ||
		   (res = viewImageSrc.SynchronizeWindow(&viewImageDst1)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Operation Softplus 객체 생성 // Create Operation Softplus object
		COperationSoftplus algObject;

		// 이미지 설정 // Set the images
		algObject.SetSourceImage(fliSourceImage);
		algObject.SetDestinationImage(fliDestinationImage0);
		algObject.SetOperationMode(COperationSoftplus::EOperationMode_Forward);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((res = algObject.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute operation Softplus.");
			break;
		}

		algObject.SetDestinationImage(fliDestinationImage1);
		algObject.SetOperationMode(COperationSoftplus::EOperationMode_Backward);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((res = algObject.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute operation Softplus.");
			break;
		}

		// 화면에 출력하기 위해 Image View 에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination0 = viewImageDst0.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination1 = viewImageDst1.GetLayer(0);

		// 기존에 Layer 에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerDestination0.Clear();
		layerDestination1.Clear();

		// 이미지 뷰 정보 표시 // Display image view information
		CFLPoint<double> flpPoint = CFLPoint<double>(0, 0);

		if ((res = layerSource.DrawTextCanvas(&flpPoint, L"Source Image", YELLOW, BLACK, 20)).IsFail() ||
			(res = layerDestination0.DrawTextCanvas(&flpPoint, L"Destination Forward Image", YELLOW, BLACK, 20)).IsFail() ||
			(res = layerDestination1.DrawTextCanvas(&flpPoint, L"Destination Backward Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰의 값 표현 방식 설정 // Set how values ​​are expressed in image view
		viewImageSrc.SetPixelNumberMode(EPixelNumberMode_Decimal);
		viewImageDst0.SetPixelNumberMode(EPixelNumberMode_Decimal);
		viewImageDst1.SetPixelNumberMode(EPixelNumberMode_Decimal);

		// floating 이미지의 색상 표현 범위 설정 // Set the color expression range of floating images
		viewImageSrc.SetFloatingImageValueRange(-1.0, 1.0);
		viewImageDst0.SetFloatingImageValueRange(-1.0, 1.0);
		viewImageDst1.SetFloatingImageValueRange(-1.0, 1.0);

		// 이미지 뷰를 갱신 // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst0.Invalidate(true);
		viewImageDst1.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewImageDst0.IsAvailable() && viewImageDst1.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}