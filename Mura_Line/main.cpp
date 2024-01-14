#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliImageSrc;
	CFLImage fliImageDst;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	do
	{
		CResult eResult = EResult_UnknownError;

		// 이미지 로드 // Load image
		if(IsFail(eResult = fliImageSrc.Load(L"../../ExampleImages/Mura/Line.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(eResult = viewImageSrc.Create(100, 0, 548, 448)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		if(IsFail(eResult = viewImageSrc.SetImagePtr(&fliImageSrc)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(eResult = viewImageDst.Create(548, 0, 996, 448)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		if(IsFail(eResult = viewImageDst.SetImagePtr(&fliImageDst)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views.
		if(IsFail(eResult = viewImageSrc.SynchronizePointOfView(&viewImageDst)))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImageSrc.SynchronizeWindow(&viewImageDst)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Mura 객체 생성 // Create Mura object
		CMura Mura;

		// 처리할 이미지 설정
		Mura.SetSourceImage(fliImageSrc);
		// Auto Threshold 모드 설정 // Set auto threshold mode
		Mura.EnableAutoThresholdMode(false);
		// Kernel Size Rate 설정 // Set kernel size rate
		Mura.SetKernelSizeRate(0.25);
		// Mura Color Type 설정 // Set mura color type
		Mura.SetMuraColorType(CMura::EMuraColorType_BlackOnWhite);

		// 논리 조건 설정
		Mura.SetLogicalCondition(ELogicalCondition_GreaterEqual);

		// 임계값 설정,  위의 조건과 아래의 조건이 합쳐지면 100보다 같거나 큰 객체를 검출
		Mura.SetThreshold(0.8);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(eResult = Mura.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute Mura.");
			break;
		}

		// 결과 객체들 중 해당되는 조건을 가진 객체를 제거

		// 50보다 같거나 큰 징뱐 길이를 가진 객체들을 제거
		if(IsFail(eResult = Mura.Filter(CBlob::EFilterItem_MinimumEnclosingRectangleLongSideLength, 50, ELogicalCondition_LessEqual)))
		{
			ErrorPrint(eResult, "Blob filtering algorithm error occurs.");
			break;
		}

		Mura.GetResultMuraImage(fliImageDst);

		// Mura 결과를 얻어오기 위해 FigureArray 선언
		CFLFigureArray flfaContours;

		// Mura 결과들 중 Contour를 얻어옴
		if(IsFail(eResult = Mura.GetResultContours(flfaContours)))
		{
			ErrorPrint(eResult, "Failed to get boundary rects from the Mura object.");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImageDst.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layer.Clear();

		// flfaContours 는 Figure들의 배열이기 때문에 Layer에 넣기만 해도 모두 드로윙이 가능하다.
		// 아래 함수 DrawFigureImage는 Image좌표를 기준으로 하는 Figure를 Drawing 한다는 것을 의미하며 // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// 맨 마지막 두개의 파라미터는 불투명도 값이고 1일경우 불투명, 0일경우 완전 투명을 의미한다. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// 여기서 0.25이므로 옅은 반투명 상태라고 볼 수 있다.
		// 파라미터 순서 : 레이어 -> Figure 객체 -> 선 색 -> 선 두께 -> 면 색 -> 펜 스타일 -> 선 알파값(불투명도) -> 면 알파값 (불투명도) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(eResult = layer.DrawFigureImage(&flfaContours, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(eResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageSrc.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}