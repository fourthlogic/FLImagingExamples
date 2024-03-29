#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliImage;
	CFLImage fliRecognizeImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage;
	CGUIViewImageWrap viewImageRecognize;
	CResult eResult;

	do
	{
		// 이미지 로드 // Load image
		if((eResult = fliImage.Load(L"../../ExampleImages/OpticalCharacterRecognition/OCR_Learn.flif")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the image file.\n");
			break;
		}

		if((eResult = fliRecognizeImage.Load(L"../../ExampleImages/OpticalCharacterRecognition/OCR_Recognition.flif")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the image file.\n");
			break;
		}

		// Source 이미지 뷰 생성 // Create Source image view
		if((eResult = viewImage.Create(200, 0, 712, 512)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Converted 이미지 뷰 생성
		if((eResult = viewImageRecognize.Create(712, 0, 1224, 512)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰에 이미지를 디스플레이 // Display the image in the source image view
		if((eResult = viewImage.SetImagePtr(&fliImage)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		// Converted 이미지 뷰에 이미지를 디스플레이
		if((eResult = viewImageRecognize.SetImagePtr(&fliRecognizeImage)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if((eResult = viewImage.SynchronizeWindow(&viewImageRecognize)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to synchronize window.\n");
			break;
		}


		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if((eResult = viewImage.SynchronizePointOfView(&viewImageRecognize)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to synchronize view\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);
		CGUIViewImageLayerWrap layerRecognize = viewImageRecognize.GetLayer(1);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layer.Clear();
		layerRecognize.Clear();

		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 0), L"Learn", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text");
			break;
		}

		if((eResult = layerRecognize.DrawTextCanvas(&CFLPointD(0, 0), L"Recognition", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text");
			break;
		}

		// OpticalCharacterReader 객체 생성 // Create OpticalCharacterReader object
		COCR ocr;

		// 문자를 학습할 이미지 설정
		if(IsFail(eResult = ocr.SetLearnImage(fliImage)))
		{
			ErrorPrint(eResult, L"Failed to set Source Image.");
			break;
		}

		// 이미지에서 학습할 문자의 색상 및 배경 색상을 설정
		/*if(IsFail(ocr.SetCharacterColor(COCR::ECharacterColorType_BlackOnWhite)))
		{
			printf("Failed to set character color type.");
			break;
		}*/

		// 학습할 이미지에 저장되어있는 Figure 학습
		if(IsFail(eResult = ocr.Learn()))
		{
			ErrorPrint(eResult, L"Failed to train.");
			break;
		}

	   // 문자를 인식할 이미지 설정
		if(IsFail(eResult = ocr.SetSourceImage(fliRecognizeImage)))
		{
			ErrorPrint(eResult, L"Failed to set Source Image.");
			break;
		}

		// 인식할 각도의 범위를 설정
		if(IsFail(eResult = ocr.SetAngleTolerance(0.)))
		{
			ErrorPrint(eResult, L"Failed to set noise filter size.");
			break;
		}

		// 인식할 이미지에서 문자의 Threshold 값을 입력 받지 않고 자동으로 찾는 모드를 설정
		if(IsFail(eResult = ocr.EnableAutoScale(true)))
		{
			ErrorPrint(eResult, L"Failed to set threshold auto.");
			break;
		}

		// 인식할 최소 점수를 설정
		if(IsFail(eResult = ocr.SetMinimumScore(0.7)))
		{
			ErrorPrint(eResult, L"Failed to set noise filter size.");
			break;
		}

		// 인식할 이미지에서 문자를 찾는 기능을 수행
		if(IsFail(eResult = ocr.Execute()))
		{
			ErrorPrint(eResult, eResult.GetString());
			break;
		}

		// 찾은 문자의 개수를 받아오는 함수
		int64_t i64ResultCount = ocr.GetResultCount();

		// 찾은 문자의 정보를 받아올 컨테이너
		COCR::COCRRecognitionCharacterInfo resultChar;

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			ocr.GetResultRecognizedCharactersInfo(i, resultChar);

			CFLString<wchar_t> flsResultString;
			flsResultString.Format(L"[%s] Score : %d%%", resultChar.flfaCharacter.GetName(), (int32_t)(resultChar.f64Score * 100));

			CFLRect<double> flrBoundary;

			resultChar.flfaCharacter.GetBoundaryRect(flrBoundary);

			if(IsFail(layerRecognize.DrawTextImage(CFLPoint<double>(flrBoundary.left, flrBoundary.top), flsResultString, YELLOW, BLACK, 15, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layerRecognize.DrawFigureImage(resultChar.flfaCharacter, LIME, 1, LIME, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImage.Invalidate(true);
		viewImageRecognize.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage.IsAvailable() && viewImageRecognize.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}