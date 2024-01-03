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
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/OrientedFASTandRotatedBRIEF/Chip.flif")))
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

		// ORB 객체 생성 // Create ORB object
		COrientedFASTandRotatedBRIEF ORB;

		// ROI 범위 설정
		CFLRectL flrROI(100, 50, 450, 450);

		// 처리할 이미지 설정
		if(IsFail(eResult = ORB.SetSourceImage(fliImage)))
		{
			ErrorPrint(eResult, "Failed to set Source Image.");
			break;
		}

		// 처리할 ROI 설정
		if(IsFail(eResult = ORB.SetSourceROI(flrROI)))
		{
			ErrorPrint(eResult, "Failed to set Source ROI.");
			break;
		}

		// 특징점을 추출할 NLevels 수를 설정
		if(IsFail(eResult = ORB.SetNLevels(4)))
		{
			ErrorPrint(eResult, "Failed to set NLevels.");
			break;
		}

		// 특징점을 추출할 Nfeature 수를 설정
		if(IsFail(eResult = ORB.SetNfeature(500)))
		{
			ErrorPrint(eResult, "Failed to set Nfeature.");
			break;
		}

		// 특징점을 추출할 ScoreType 설정
		if(IsFail(eResult = ORB.SetScoreType(COrientedFASTandRotatedBRIEF::EScoreType_FastScore)))
		{
			ErrorPrint(eResult, "Failed to set Score Type.");
			break;
		}

		// 추출할 특징점의 FAST Threshold 설정
		if(IsFail(eResult = ORB.SetFASTThreshold(10)))
		{
			ErrorPrint(eResult, "Failed to set FAST Threshold.");
			break;
		}

		// ORB의 파라미터 Scale Factor 설정
		if(IsFail(eResult = ORB.SetScaleFactor(1.2f)))
		{
			ErrorPrint(eResult, "Failed to set Scale Factor.");
			break;
		}

		// ORB 실행 함수
		if(IsFail(eResult = ORB.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute.");
			break;
		}

		// 실행 결과를 받아오기 위한 컨테이너
		Foundation::CFLPointArray flfaResultPoints;

		// 키 포인트 점을 검출
		if(IsFail(eResult = ORB.GetResultPoints(&flfaResultPoints)))
		{
			ErrorPrint(eResult, "Failed to get result.");
			break;
		}

		// 검출된 점을 출력
		layer.DrawFigureImage(flfaResultPoints, RED, 1);

		// ROI영역이 어디인지 알기 위해 디스플레이 한다 // Display to find out where ROI is
		// FLImaging의 Figure객체들은 어떤 도형모양이든 상관없이 하나의 함수로 디스플레이가 가능
		if(IsFail(eResult = layer.DrawFigureImage(&flrROI, BLUE)))
		{
			ErrorPrint(eResult, "Failed to draw figures objects on the image view.\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImage.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}