#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliSourceDestinationImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewIamgeDst;

	 // 수행 결과 객체 선언 // Declare the execution result object
	CResult eResult = EResult_UnknownError;

	do
	{
		// Source 이미지 로드 // Load the source image
		if((eResult = fliSourceImage.Load(L"../../ExampleImages/OperationLinearPolynomial/Space.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination 이미지를 Source 이미지와 동일하게 설정
		if((eResult = fliSourceDestinationImage.Assign(fliSourceImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Source 이미지 뷰 생성 // Create Source image view
		if((eResult = viewImageSrc.Create(100, 0, 612, 512)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination 이미지 뷰 생성 // Create Destination image view
		if((eResult = viewIamgeDst.Create(612, 0, 1124, 512)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if((eResult = viewImageSrc.SynchronizePointOfView(&viewIamgeDst)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// Source 이미지 뷰에 이미지를 디스플레이 // Display the image in the source image view
		if((eResult = viewImageSrc.SetImagePtr(&fliSourceImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination 이미지 뷰에 이미지를 디스플레이 // Display the image in the destination image view
		if((eResult = viewIamgeDst.SetImagePtr(&fliSourceDestinationImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if((eResult = viewImageSrc.SynchronizeWindow(&viewIamgeDst)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Operation Linear 객체 생성 // Create Operation Linear object
		COperationLinear linear;

		// Source 이미지 설정 // Set the source image
		linear.SetSourceImage(fliSourceImage);

		// Source 이미지의 ROI 범위 설정 // Set the Source ROI value
		CFLCircleL flcSourceROI(128, 128, 80, 0, 0, 360, EArcClosingMethod_EachOther);

		// Source 이미지의 ROI 지정 // Set the Source ROI
		linear.SetSourceROI(flcSourceROI);

		// Scalar Operation 소스로 설정 // Set Operation Source to scalar
		linear.SetOperationSource(EOperationSource_Scalar);
		linear.SetOperationOrder(EOperationOrder_CoefficientConstant);
		linear.SetDestinationImage(fliSourceDestinationImage);

		// 스칼라 값 지정 // Set the Scalar value
		linear.SetScalarValue(CMultiVar<double>(2), CMultiVar<double>(1));

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((eResult = linear.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute operation Linear Polynomial.");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewIamgeDst.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerDestination.Clear();

		// FLImaging의 Figure객체들은 어떤 도형모양이든 상관없이 하나의 함수로 디스플레이가 가능
		// Source ROI 영역이 어디인지 알기 위해 디스플레이 한다
		if((eResult = layerSource.DrawFigureImage(&flcSourceROI, LIME)).IsFail())
			ErrorPrint(eResult, "Failed to draw figure\n");

			if((eResult = layerDestination.DrawFigureImage(&flcSourceROI, LIME)).IsFail())
				ErrorPrint(eResult, "Failed to draw figure\n");

			// 이미지 뷰 정보 표시 // Display image view information
				if((eResult = layerSource.DrawTextCanvas(CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
				{
					//ErrorPrint(eResult, "Failed to draw text\n");
					break;
				}

		if((eResult = layerDestination.DrawTextCanvas(CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 // Update image view
		viewImageSrc.Invalidate(true);
		viewIamgeDst.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewIamgeDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}