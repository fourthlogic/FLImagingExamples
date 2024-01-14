#include <cstdio>

#include <FLImaging.h>


int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliImage;
	CFLImage fliRecognizeImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage;
	CGUIViewImageWrap viewImageRecognize;

	do
	{
		// 이미지 로드 // Load image
		if(IsFail(fliImage.Load(L"../../ExampleImages/OpticalCharacterReader/Fourthlogic_Font.flif")))
		{
			printf("Failed to load the image file.\n");
			break;
		}

		if(IsFail(fliRecognizeImage.Load(L"../../ExampleImages/OpticalCharacterReader/Fourthlogic_Recognition.jpg")))
		{
			printf("Failed to load the image file.\n");
			break;
		}

		// Source 이미지 뷰 생성 // Create Source image view
		if(IsFail(viewImage.Create(200, 0, 712, 384)))
		{
			printf("Failed to create the image view.\n");
			break;
		}

		// Converted 이미지 뷰 생성
		if(IsFail(viewImageRecognize.Create(712, 0, 1716, 508)))
		{
			printf("Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰에 이미지를 디스플레이 // Display the image in the source image view
		if(IsFail(viewImage.SetImagePtr(&fliImage)))
		{
			printf("Failed to set image object on the image view.\n");
			break;
		}

		// Converted 이미지 뷰에 이미지를 디스플레이
		if(IsFail(viewImageRecognize.SetImagePtr(&fliRecognizeImage)))
		{
			printf("Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if(IsFail(viewImage.SynchronizeWindow(&viewImageRecognize)))
		{
			printf("Failed to synchronize window.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);
		CGUIViewImageLayerWrap layerRecognize = viewImageRecognize.GetLayer(1);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layer.Clear();
		layerRecognize.Clear();

		if(IsFail(layer.DrawTextCanvas(&CFLPointD(0, 0), L"Train", YELLOW, BLACK, 30)))
		{
			printf("Failed to draw text");
			break;
		}

		if(IsFail(layerRecognize.DrawTextCanvas(&CFLPointD(0, 0), L"Recognition", YELLOW, BLACK, 30)))
		{
			printf("Failed to draw text");
			break;
		}

		// OpticalCharacterReader 객체 생성 // Create OpticalCharacterReader object
		COpticalCharacterReader lattice;
		CResult eResult = EResult_UnknownError;

		// 문자를 학습할 이미지 설정
		if(IsFail(lattice.SetLearnImage(fliImage)))
		{
			printf("Failed to set Source Image.");
			break;
		}

		// 이미지에서 학습할 문자의 색상 및 배경 색상을 설정
		if(IsFail(lattice.SetCharacterColor(COpticalCharacterReader::ECharacterColorType_BlackOnWhite)))
		{
			printf("Failed to set character color type.");
			break;
		}

		// 학습할 이미지에서 무시할 노이즈의 크기를 하한, 상한으로 설정
		if(IsFail(lattice.SetNoiseFilterSize(5, 5000, 5, 5000)))
		{
			printf("Failed to set noise filter size.");
			break;
		}

		// 학습할 이미지에 저장되어있는 Figure 학습
		if(IsFail(lattice.Train()))
		{
			printf("Failed to train.");
			break;
		}

		// ROI 범위 설정
		 CFLRectL flrROI = CFLRectL(0, 0, 1003, 508);

		// 문자를 인식할 이미지 설정
		if(IsFail(lattice.SetSourceImage(fliRecognizeImage)))
		{
			printf("Failed to set Source Image.");
			break;
		}

		// 인식할 이미지에 ROI 범위 설정
		if(IsFail(lattice.SetSourceROI(flrROI)))
		{
			printf("Failed to set Source ROI.");
			break;
		}

		// 인식할 각도의 범위를 설정
		if(IsFail(lattice.SetAngleRange(0., 0.)))
		{
			printf("Failed to set noise filter size.");
			break;
		}

		// 인식할 이미지에서 문자의 Threshold 값을 입력 받지 않고 자동으로 찾는 모드를 설정
		if(IsFail(lattice.SetThresholdAuto(false)))
		{
			printf("Failed to set threshold auto.");
			break;
		}

		// 인식할 이미지에서 문자의 Threshold 값을 설정
		if(IsFail(lattice.SetThreshold(135)))
		{
			printf("Failed to set threshold.");
			break;
		}

		// 인식할 이미지에서 찾은 문자의 하한 스코어값을 설정(0.0~1.0)
		if(IsFail(lattice.SetScoreFilter(.9)))
		{
			printf("Failed to set score filter.");
			break;
		}

		// 인식할 이미지에서 문자를 찾는 기능을 수행
		if(IsFail(eResult = lattice.Execute()))
		{
			wprintf(eResult.GetString());
			break;
		}

		// 찾은 문자의 개수를 받아오는 함수
		int64_t i64ResultCount = lattice.GetResultCharacterCount();

		// 찾은 문자의 정보를 받아올 컨테이너
		COpticalCharacterReader::CResultCharacter resultChar;

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			lattice.GetResultCharacter(i, resultChar);

			if(resultChar.wcCharacter == 0)
				continue;

			CFLStringW fls = resultChar.wcCharacter;

			if(IsFail(layerRecognize.DrawTextImage(&CFLPointD(resultChar.flrRegion.left, resultChar.flrRegion.top), fls, RED, WHITE, 16)))
			{
				printf("Failed to draw recognized character : %d", i);
				break;
			}

			if(IsFail(layerRecognize.DrawFigureImage(&resultChar.flrRegion, TOMATO, 2, 0, EGUIViewImagePenStyle_Solid, 1, 0)))
			{
				printf("Failed to draw recognized character : %d", i);
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