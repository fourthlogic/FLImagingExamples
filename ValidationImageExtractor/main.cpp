#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"
#include <cfloat>
#include <cmath>
#include <iostream>
#include <limits>
#include <process.h>

bool g_bTerminated = false;

unsigned int __stdcall LearnThread(void* pParam)
{
	CSemanticSegmentationDL* pSemanticSegmentation = (CSemanticSegmentationDL*)pParam;

	CResult er = pSemanticSegmentation->Learn();
	g_bTerminated = true;

	return unsigned int(er);
};

int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliSourceImage;
	CFLImage fliResultLearnImage;
	CFLImage fliResultValidationImage;

	/// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageResultLearn;
	CGUIViewImageWrap viewImageResultValidation;

	CResult eResult = EResult_UnknownError;

	do
	{
		// 이미지 로드 // Loads image
		if(IsFail(eResult = fliSourceImage.Load(L"../../ExampleImages/SemanticSegmentation/Train.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Creates image view
		if(IsFail(eResult = viewImageSource.Create(100, 0, 600, 500)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageResultLearn.Create(600, 0, 1100, 500)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageResultValidation.Create(1100, 0, 1700, 500)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 세 개의 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the three image view windows
		if(IsFail(eResult = viewImageSource.SynchronizeWindow(&viewImageResultLearn)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(eResult = viewImageSource.SynchronizeWindow(&viewImageResultValidation)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the imageview
		if(IsFail(eResult = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageResultLearn.SetImagePtr(&fliResultLearnImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageResultValidation.SetImagePtr(&fliResultValidationImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerValidation = viewImageResultLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabel = viewImageResultValidation.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerLearn.Clear();
		layerValidation.Clear();
		layerResultLabel.Clear();

		// View 정보를 디스플레이 합니다. // Display View information.
		// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = layerLearn.DrawTextCanvas(&CFLPointD(0, 0), L"SOURCE", RED, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerValidation.DrawTextCanvas(&CFLPointD(0, 0), L"TRAIN(EXTRACTED (4/6))", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerResultLabel.DrawTextCanvas(&CFLPointD(0, 0), L"VALIDATION(EXTRACTED (2/6))", GREEN, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 // Update the image view.
		viewImageSource.RedrawWindow();
		viewImageResultLearn.RedrawWindow();
		viewImageResultValidation.RedrawWindow();
		
		// Validation Image 비율 설정 // Set ratio of validation image
		float f32Ratio = 0.4f;
		// Dataset type 설정 // Set the data set type
		CValidationImageExtractorDL::EDatasetType eDatasetType = CValidationImageExtractorDL::EDatasetType_SemanticSegmentation;
		// Validation Set에 최소한 몇 개의 클래스가 1개 이상 씩 포함될 것인지 설정 // Set how many classes each will be included in the Validation Set
		int64_t i64MinimumClassIncluded = 2;

		// Validation Extractor 실행 // Validaiton Exctractor Execute 
		if(IsFail(eResult = CValidationImageExtractorDL::Extract(&fliSourceImage, f32Ratio, eDatasetType, &fliResultLearnImage, &fliResultValidationImage, i64MinimumClassIncluded)))
		{
			ErrorPrint(eResult, "Failed to Process\n");
			break;
		}

		// 세 개의 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the three image view windows
		if(IsFail(eResult = viewImageSource.SynchronizeWindow(&viewImageResultLearn)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(eResult = viewImageSource.SynchronizeWindow(&viewImageResultValidation)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// 이미지 뷰를 갱신 // Update the image view.
		viewImageResultLearn.ZoomFit();
		viewImageResultValidation.ZoomFit();
		viewImageSource.RedrawWindow();
		viewImageResultLearn.RedrawWindow();
		viewImageResultValidation.RedrawWindow();

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageSource.IsAvailable() && viewImageResultLearn.IsAvailable() && viewImageResultValidation.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}