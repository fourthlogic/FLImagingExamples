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
	COpticalCharacterRecognitionDL* pOCR = (COpticalCharacterRecognitionDL*)pParam;

	CResult er = pOCR->Learn();
	g_bTerminated = true;

	return unsigned int(er);
};

int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliLearnImage;
	CFLImage fliValidationImage;
	CFLImage fliResultLabelImage;
	CFLImage fliResultLabelFigureImage;

	/// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageValidation;
	CGUIViewImageWrap viewImagesLabel;
	CGUIViewImageWrap viewImagesLabelFigure;

	// 그래프 뷰 선언 // Declare the graph view
	CGUIViewGraphWrap viewGraph;

	CResult res = EResult_UnknownError;

	do 
	{
		// 이미지 로드 // Loads image
		if(IsFail(res = fliLearnImage.Load(L"../../ExampleImages/OpticalCharacterRecognitionDL/OCR_Learn.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliValidationImage.Load(L"../../ExampleImages/OpticalCharacterRecognitionDL/OCR_Inference.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Creates image view
		if(IsFail(res = viewImageLearn.Create(100, 0, 600, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageValidation.Create(600, 0, 1100, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImagesLabel.Create(100, 500, 600, 1000)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImagesLabelFigure.Create(600, 500, 1100, 1000)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Graph 뷰 생성 // Create graph view
		if(IsFail(res = viewGraph.Create(1100, 0, 1600, 500)))
		{
			ErrorPrint(res, " Failed to create the graph view. \n");
			break;
		}

		viewGraph.SetDarkMode();

		// 다섯 개의 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the four image view windows
		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImageValidation)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImagesLabel)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImagesLabelFigure)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the imageview
		if(IsFail(res = viewImageLearn.SetImagePtr(&fliLearnImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageValidation.SetImagePtr(&fliValidationImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		viewImagesLabel.EnablePixelSegmentationMode(true);

		if(IsFail(res = viewImagesLabel.SetImagePtr(&fliResultLabelImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		fliResultLabelFigureImage.Assign(fliValidationImage);
		fliResultLabelFigureImage.ClearFigures();

		if(IsFail(res = viewImagesLabelFigure.SetImagePtr(&fliResultLabelFigureImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerValidation = viewImageValidation.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabel = viewImagesLabel.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabelFigure = viewImagesLabelFigure.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerLearn.Clear();
		layerValidation.Clear();
		layerResultLabel.Clear();
		layerResultLabelFigure.Clear();

		// View 정보를 디스플레이 합니다. // Display View information.
		// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = layerLearn.DrawTextCanvas(&CFLPoint<double>(0, 0), L"LEARN", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerValidation.DrawTextCanvas(&CFLPoint<double>(0, 0), L"VALIDATION", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerResultLabel.DrawTextCanvas(&CFLPoint<double>(0, 0), L"RESULT LABEL", GREEN, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerResultLabelFigure.DrawTextCanvas(&CFLPoint<double>(0, 0), L"RESULT LABEL FIGURE", GREEN, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesLabel.RedrawWindow();
		viewImagesLabelFigure.RedrawWindow();

		// OCR 객체 생성 // Create OCR object
		COpticalCharacterRecognitionDL ocr;

		// OptimizerSpec 객체 생성 // Create OptimizerSpec object
		COptimizerSpecAdamGradientDescent optSpec;

		// 학습할 이미지 설정 // Set the image to learn
		ocr.SetLearningImage(fliLearnImage);
		// 검증할 이미지 설정 // Set the image to validate
		ocr.SetLearningValidationImage(fliValidationImage);
		// 학습할 OCR 모델 설정 // Set up OCR model to learn
		ocr.SetModel(COpticalCharacterRecognitionDL::EModel_FLSegNet);
		// 학습할 OCR 모델의 버전 설정 // Set up OCR model version to learn
		ocr.SetModelVersion(COpticalCharacterRecognitionDL::EModelVersion_FLSegNet_V1_1024_B3);
		// 학습 epoch 값을 설정 // Set the learn epoch value 
		ocr.SetLearningEpoch(10000);
		// 학습 이미지 Interpolation 방식 설정 // Set Interpolation method of learn image
		ocr.SetInterpolationMethod(EInterpolationMethod_Bilinear);

		// Optimizer의 학습률 설정 // Set learning rate of Optimizer
		optSpec.SetLearningRate(1e-3f);

		// 설정한 Optimizer를 OCR에 적용 // Apply the Optimizer that we set up to OCR
		ocr.SetLearningOptimizerSpec(optSpec);

		// AugmentationSpec 설정 // Set the AugmentationSpec
		CAugmentationSpec augSpec;

		augSpec.SetCommonActivationRatio(0.5);
		augSpec.SetCommonInterpolationMethod(FLImaging::ImageProcessing::EInterpolationMethod_Bilinear);
		augSpec.EnableRotation(true);
		augSpec.SetRotationParam(45., false);

		augSpec.EnableGaussianNoise(true);
		augSpec.SetGaussianNoiseParam(0, 0.02);

		augSpec.EnableScaleCrop(true);
		augSpec.SetScaleCropParam(1., 1., true, .5, 2., .5, 2., true);

		ocr.SetLearningAugmentationSpec(&augSpec);
		
		// Learn 동작을 하는 핸들 객체 선언 // Declare HANDLE object execute learn function
		HANDLE hThread;

		// OCR learn function을 진행하는 스레드 생성 // Create the OCR Learn function thread
		hThread = (HANDLE)_beginthreadex(NULL, 0, LearnThread, (void*)&ocr, 0, nullptr);

		while(!ocr.IsRunning() && !g_bTerminated)
			CThreadUtilities::Sleep(1);

		int32_t i32MaxEpoch = ocr.GetLearningEpoch();
		int32_t i32PrevEpoch = 0;
		int32_t i32PrevCostCount = 0;
		int32_t i32PrevValidationCount = 0;

		while(true)
		{
			CThreadUtilities::Sleep(1);

			// 마지막 미니 배치 최대 반복 횟수 받기 // Get the last maximum number of iterations of the last mini batch 
			int32_t i32MaxIteration = ocr.GetActualMiniBatchCount();
			// 마지막 미니 배치 반복 횟수 받기 // Get the last number of mini batch iterations
			int32_t i32Iteration = ocr.GetLearningResultCurrentIteration();
			// 마지막 학습 횟수 받기 // Get the last epoch learning
			int32_t i32Epoch = ocr.GetLastEpoch();

			// 미니 배치 반복이 완료되면 cost와 validation 값을 디스플레이 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(i32Epoch != i32PrevEpoch && i32Iteration == i32MaxIteration && i32Epoch > 0)
			{
				// 마지막 학습 결과 비용 받기 // Get the last cost of the learning result
				float f32CurrCost = ocr.GetLearningResultLastCost();
				// 마지막 검증 결과 받기 // Get the last validation result
				float f32ValidationPa = ocr.GetLearningResultLastAccuracy();
				float f32ValidationMeanIoU = ocr.GetLearningResultLastMeanIoU();

				// 해당 epoch의 비용과 검증 결과 값 출력 // Print cost and validation value for the relevant epoch
				printf("Cost : %.6f Validation : %.6f mIoU : %.6f Epoch %d / %d\n", f32CurrCost, f32ValidationPa, f32ValidationMeanIoU, i32Epoch, i32MaxEpoch);

				// 학습 결과 비용과 검증 결과 기록을 받아 그래프 뷰에 출력  
				// Get the history of cost and validation and print it at graph view
				CFLArray<float> vctCosts;
				CFLArray<float> vctVadliationPixelAccuracy;
				CFLArray<float> vctMeanIoU;
				CFLArray<float> vctVadliationPixelAccuracyZE;
				CFLArray<float> vctMeanIoUZE;
				CFLArray<int32_t> vctValidationEpoch;

				ocr.GetLearningResultAllHistory(&vctCosts, &vctVadliationPixelAccuracy, &vctMeanIoU, &vctVadliationPixelAccuracyZE, &vctMeanIoUZE, &vctValidationEpoch);

				// 비용 기록이나 검증 결과 기록이 있다면 출력 // Print results if cost or validation history exists
				if((vctCosts.GetCount() && i32PrevCostCount != (int32_t)vctCosts.GetCount()) || (vctVadliationPixelAccuracy.GetCount() && i32PrevValidationCount != (int32_t)vctVadliationPixelAccuracy.GetCount()))
				{
					int32_t i32Step = ocr.GetLearningValidationStep();
					CFLArray<float> flaX;

					for(int64_t i = 0; i < vctVadliationPixelAccuracy.GetCount() - 1; ++i)
						flaX.PushBack((float)(i * i32Step));

					flaX.PushBack((float)(vctCosts.GetCount() - 1));

					// 이전 그래프의 데이터를 삭제 // Clear previous grpah data
					viewGraph.LockUpdate();
					viewGraph.Clear();
					// Graph View 데이터 입력 // Input Graph View Data
					viewGraph.Plot(vctCosts, EChartType_Line, RED, L"Cost");

					// Graph View 데이터 입력 // Input Graph View Data
					viewGraph.Plot(flaX, vctVadliationPixelAccuracy, EChartType_Line, BLUE, L"PixelAccuracy(Zero Exception)");
					viewGraph.Plot(flaX, vctMeanIoU, EChartType_Line, PINK, L"mIoU");
					viewGraph.UnlockUpdate();

					viewGraph.UpdateWindow();
					viewGraph.Invalidate();
					viewGraph.RedrawWindow();
				}

				// 검증 결과가 0.9 이상 일 경우 학습을 중단하고 분류 진행 
				// If the validation result is greater than 0.9, stop learning and classify images 
				if(f32ValidationMeanIoU >= .9f || GetAsyncKeyState(VK_ESCAPE))
					ocr.Stop();

				i32PrevEpoch = i32Epoch;
				i32PrevCostCount = (int32_t)vctCosts.GetCount();
				i32PrevValidationCount = (int32_t)vctVadliationPixelAccuracy.GetCount();
			}
			// epoch만큼 학습이 완료되면 종료 // End when learning progresses as much as epoch
			if(!ocr.IsRunning() && g_bTerminated)
			{
				// learn 동작 스레드가 완전히 종료될 까지 대기 // Wait until learning is completely terminated
				WaitForSingleObject(hThread, INFINITE);
				// 스레드 핸들 종료 // Close thread handle
				CloseHandle(hThread);
				break;
			}
		}

		// Result Label Image에 피겨를 포함하지 않는 Execute
		// 분류할 이미지 설정 // Set the image to classify
		ocr.SetInferenceImage(fliValidationImage);
		// 추론 결과 이미지 설정 // Set the inference result Image
		ocr.SetInferenceResultImage(fliResultLabelImage);
		// 추론 결과 옵션 설정 // Set the inference result options;
		// Result 결과를 Label Image로 받을지 여부 설정 // Set whether to receive the result as a Label Image
		ocr.EnableInferenceResultLabelImage(true);

		// 알고리즘 수행 // Execute the algorithm
		if(IsFail(res = ocr.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}

		// Result Label Image에 피겨를 포함한 Execute
		// 추론 결과 이미지 설정 // Set the inference result Image
		ocr.SetInferenceResultImage(fliResultLabelFigureImage);
		// 추론 결과 옵션 설정 // Set the inference result options;
		// Result 결과를 Label Image로 받을지 여부 설정 // Set whether to receive the result as a Label Image
		ocr.EnableInferenceResultLabelImage(false);
		// Result item settings enum 설정 // Set the result item settings
		ocr.SetInferenceResultItemSettings(COpticalCharacterRecognitionDL::EInferenceResultItemSettings_ClassName_ConfidenceScore_RegionType_Contour);

		// 알고리즘 수행 // Execute the algorithm
		if(IsFail(res = ocr.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}

		int64_t i64LearningClassCount = ocr.GetLearningResultClassCount();
		// ResultContours 인덱스와 매칭 되는 라벨 번호배열을 가져오기 // ResultContours Get an array of label numbers matching the index.
		for(int64_t classNum = 1; classNum < i64LearningClassCount; ++classNum)
		{
			Base::CFLArray<Base::CFLStringW> flaNames;

			ocr.GetLearningResultClassNames(classNum, &flaNames);
			viewImagesLabel.SetSegmentationLabelText(0, (double)classNum, flaNames[0].GetString());
		}

		// ResultLabel 뷰에 Floating Value Range를 설정 // Set Floating Value Range in ResultLabel view
		viewImagesLabel.SetFloatingImageValueRange(0.f, (float)ocr.GetLearningResultClassCount());

		// 이미지 뷰를 갱신 // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesLabel.RedrawWindow();
		viewImagesLabelFigure.RedrawWindow();

		// 그래프 뷰를 갱신 // Update the Graph view.
		viewGraph.RedrawWindow();

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageValidation.IsAvailable() && viewImagesLabel.IsAvailable() && viewImagesLabelFigure.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}