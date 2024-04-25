#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliRecognizeImage;
	CFLImage fliRecognizeImageUnicode;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImageRecognize;
	CGUIViewImageWrap viewImageRecognizeUnicode;
	CResult res;

	do
	{
		if((res = fliRecognizeImage.Load(L"../../ExampleImages/OpticalCharacterRecognition/OCR_Recognition_15Degree.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		if((res = fliRecognizeImageUnicode.Load(L"../../ExampleImages/OpticalCharacterRecognition/OCR_Recognition_Unicode2.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Converted 이미지 뷰 생성
		if((res = viewImageRecognize.Create(200, 0, 712, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Converted 이미지 뷰 생성
		if((res = viewImageRecognizeUnicode.Create(712, 0, 1224, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Converted 이미지 뷰에 이미지를 디스플레이
		if((res = viewImageRecognize.SetImagePtr(&fliRecognizeImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Converted 이미지 뷰에 이미지를 디스플레이
		if((res = viewImageRecognizeUnicode.SetImagePtr(&fliRecognizeImageUnicode)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerRecognize = viewImageRecognize.GetLayer(1);
		CGUIViewImageLayerWrap layerRecognizeUnicode = viewImageRecognizeUnicode.GetLayer(1);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerRecognize.Clear();
		layerRecognizeUnicode.Clear();

		if((res = layerRecognize.DrawTextCanvas(&CFLPointD(0, 0), L"Recognition1", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text");
			break;
		}

		if((res = layerRecognizeUnicode.DrawTextCanvas(&CFLPointD(0, 0), L"Recognition2", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text");
			break;
		}

		// OpticalCharacterReader 객체 생성 // Create OpticalCharacterReader object
		COCR ocr;

		// 학습 정보 파일을 로드
		if(IsFail(res = ocr.Load(L"../../ExampleImages/OpticalCharacterRecognition/OCR_FourthLogic.flocr")))
		{
			ErrorPrint(res, L"Failed to load learnt file.");
			break;
		}

	    // 문자를 인식할 이미지 설정
		if(IsFail(res = ocr.SetSourceImage(fliRecognizeImage)))
		{
			ErrorPrint(res, L"Failed to set Source Image.");
			break;
		}

		// 인식할 문자의 각도 범위를 설정
		if(IsFail(res = ocr.SetRecognizingAngleTolerance(20.)))
		{
			ErrorPrint(res, L"Failed to set recognizing angle tolerance.");
			break;
		}

		// 인식할 이미지의 전처리 여부를 설정
		if(IsFail(res = ocr.EnableRecognizingNoiseReduction(false)))
		{
			ErrorPrint(res, L"Failed to set recognizing Noise Reduction.");
			break;
		}

		// 인식할 문자의 색상을 설정
		if(IsFail(res = ocr.SetRecognizingCharacterColorType(COCR::ECharacterColorType_All)))
		{
			ErrorPrint(res, L"Failed to set recognizing character color.");
			break;
		}

		// 인식할 최소 점수를 설정
		if(IsFail(res = ocr.SetRecognizingMinimumScore(0.7)))
		{
			ErrorPrint(res, L"Failed to set minimum score.");
			break;
		}

		// 인식할 이미지에서 문자를 찾는 기능을 수행
		if(IsFail(res = ocr.Execute()))
		{
			ErrorPrint(res, res.GetString());
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
			CFLRect<double> flrBoundary;

			flsResultString.Format(L"[%s] Score : %d%%, Angle : %.1lf", resultChar.flfaCharacter.GetName(), (int32_t)(resultChar.f64Score * 100), resultChar.f64Rotation);
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

		// 문자를 인식할 이미지 설정
		if(IsFail(res = ocr.SetSourceImage(fliRecognizeImageUnicode)))
		{
			ErrorPrint(res, L"Failed to set Source Image.");
			break;
		}

		// 인식할 문자의 각도 범위를 설정
		if(IsFail(res = ocr.SetRecognizingAngleTolerance(10.)))
		{
			ErrorPrint(res, L"Failed to set recognizing angle tolerance.");
			break;
		}

		// 인식할 이미지의 전처리 여부를 설정
		if(IsFail(res = ocr.EnableRecognizingNoiseReduction(true)))
		{
			ErrorPrint(res, L"Failed to set recognizing Noise Reduction.");
			break;
		}

		// 인식할 문자의 색상을 설정
		if(IsFail(res = ocr.SetRecognizingCharacterColorType(COCR::ECharacterColorType_BlackOnWhite)))
		{
			ErrorPrint(res, L"Failed to set recognizing character color.");
			break;
		}

		// 인식할 최소 점수를 설정
		if(IsFail(res = ocr.SetRecognizingMinimumScore(0.5)))
		{
			ErrorPrint(res, L"Failed to set minimum score.");
			break;
		}

		// 인식할 문자의 유니코드 여부를 설정
		if(IsFail(res = ocr.EnableRecognizingUnicodeByteCharacter(true)))
		{
			ErrorPrint(res, L"Failed to Enable unicode byte character.");
			break;
		}

		// 인식할 이미지에서 문자를 찾는 기능을 수행
		if(IsFail(res = ocr.Execute()))
		{
			ErrorPrint(res, res.GetString());
			break;
		}

		// 찾은 문자의 개수를 받아오는 함수
		i64ResultCount = ocr.GetResultCount();

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			ocr.GetResultRecognizedCharactersInfo(i, resultChar);

			CFLString<wchar_t> flsResultString;
			CFLRect<double> flrBoundary;

			flsResultString.Format(L"[%s] Score : %d%%, Angle : %.1lf", resultChar.flfaCharacter.GetName(), (int32_t)(resultChar.f64Score * 100), resultChar.f64Rotation);
			resultChar.flfaCharacter.GetBoundaryRect(flrBoundary);

			if(IsFail(layerRecognizeUnicode.DrawTextImage(CFLPoint<double>(flrBoundary.left, flrBoundary.top), flsResultString, YELLOW, BLACK, 15, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layerRecognizeUnicode.DrawFigureImage(resultChar.flfaCharacter, LIME, 1, LIME, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImageRecognize.Invalidate(true);
		viewImageRecognizeUnicode.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageRecognize.IsAvailable() && viewImageRecognizeUnicode.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}