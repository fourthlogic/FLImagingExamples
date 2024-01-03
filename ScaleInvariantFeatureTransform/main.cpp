#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage;

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult eResult;

	do
	{
		// 이미지 로드 // Load image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/ScaleInvariantFeatureTransform/Chip.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(eResult = viewImage.Create(400, 0, 1168, 540)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		if(IsFail(eResult = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layer.Clear();

		// SIFT 객체 생성 // Create SIFT object
		CScaleInvariantFeatureTransform SIFT;

		// 처리할 이미지 설정
		if(IsFail(eResult = SIFT.SetSourceImage(fliImage)))
		{
			ErrorPrint(eResult, "Failed to set Source Image.");
			break;
		}

		// 특징점을 추출할 Octave Layer 수를 설정
		if(IsFail(eResult = SIFT.SetOctaveLayers(3)))
		{
			ErrorPrint(eResult, "Failed to set octave layers.");
			break;
		}

		// 추출할 특징점의 대비 임계값 설정
		if(IsFail(eResult = SIFT.SetContrastThreshold(0.04f)))
		{
			ErrorPrint(eResult, "Failed to set contrast threshold.");
			break;
		}

		// 추출할 특징점의 에지 임계값 설정
		if(IsFail(eResult = SIFT.SetEdgeThreshold(10.f)))
		{
			ErrorPrint(eResult, "Failed to set edge threshold.");
			break;
		}

		// SIFT의 파라미터 Sigma 설정
		if(IsFail(eResult = SIFT.SetSigma(1.6f)))
		{
			ErrorPrint(eResult, "Failed to set param sigma.");
			break;
		}

		// SIFT 실행 함수
		if(IsFail(eResult = SIFT.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute.");
			break;
		}

		// 실행 결과를 받아오기 위한 컨테이너
		Foundation::CFLPointArray flfaResultPoints;

		// 키 포인트 점을 검출
		if(IsFail(eResult = SIFT.GetResultPoints(&flfaResultPoints)))
		{
			ErrorPrint(eResult, "Failed to get result.");
			break;
		}

		// 검출된 점을 출력
		layer.DrawFigureImage(flfaResultPoints, RED, 1);

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImage.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}