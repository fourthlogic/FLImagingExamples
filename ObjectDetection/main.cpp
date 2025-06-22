﻿#include <cstdio>

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
	CObjectDetectionDL* pobjectDetection = (CObjectDetectionDL*)pParam;

	CResult er = pobjectDetection->Learn();
	g_bTerminated = true;

	return unsigned int(er);
};

int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliLearnImage;
	CFLImage fliValidationImage;
	CFLImage fliResultLabelImage;

	/// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageValidation;
	CGUIViewImageWrap viewImagesLabel;

	// 그래프 뷰 선언 // Declare the graph view
	CGUIViewGraphWrap viewGraph;

	CResult res = EResult_UnknownError;

	do
	{
		// 라이브러리가 완전히 로드 될 때까지 기다림 // Wait for the library to fully load
		CThreadUtilities::Sleep(1000);

		// 이미지 로드 // Loads image
		if(IsFail(res = fliLearnImage.Load(L"../../ExampleImages/ObjectDetection/Train.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliValidationImage.Load(L"../../ExampleImages/ObjectDetection/Validation.flif")))
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

		if(IsFail(res = viewImagesLabel.SetImagePtr(&fliResultLabelImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerValidation = viewImageValidation.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabel = viewImagesLabel.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerLearn.Clear();
		layerValidation.Clear();
		layerResultLabel.Clear();
	
		// View 정보를 디스플레이 합니다. // Display View information.
		// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다.// The function DrawTextCanvas below draws a String based on the screen coordinates.
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

		// 이미지 뷰를 갱신 // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesLabel.RedrawWindow();

		// objectDetection 객체 생성 // Create objectDetection object
		CObjectDetectionDL objectDetection;

		// OptimizerSpec 객체 생성 // Create OptimizerSpec object
		COptimizerSpecAdamGradientDescent optSpec;

		// 학습할 이미지 설정 // Set the image to learn
		objectDetection.SetLearningImage(fliLearnImage);
		// 검증할 이미지 설정 // Set the image to validate
		objectDetection.SetLearningValidationImage(fliValidationImage);
		// 학습할 objectDetection 모델 설정 // Set up objectDetection model to learn
		objectDetection.SetModel(CObjectDetectionDL::EModel_R_FLNet);
		// 학습할 objectDetection 모델의 버전 설정 // Set up objectDetection model version to learn
		objectDetection.SetModelVersion(CObjectDetectionDL::EModelVersion_R_FLNet_V1_256);
		// 학습 epoch 값을 설정 // Set the learn epoch value 
		objectDetection.SetLearningEpoch(1024);
		// 학습 이미지 Interpolation 방식 설정 // Set Interpolation method of learn image
		objectDetection.SetInterpolationMethod(EInterpolationMethod_Bilinear);
	    // 검증을 진행 할 최소 평균 Cost값 설정 // Set the minimum average cost value at which verification will be triggered
		objectDetection.SetLearningRequiredAvgCostForValidation(5.f);
		// 모델의 최적의 상태를 추적 후 마지막에 최적의 상태로 적용할 지 여부 설정 // Set whether to track the optimal state of the model and apply it as the optimal state at the end.
		objectDetection.EnableOptimalLearningStatePreservation(false);
		// 학습 중단 Metrics 값 설정 // Set Learning Stop Metrics Values
		objectDetection.SetLearningStopCondition(L"mAP >= 0.9");
		// Optimizer의 학습률 설정 // Set learning rate of Optimizer
		optSpec.SetLearningRate(1e-4f);
		optSpec.SetWeightDecay(0.f);
	
		// 설정한 Optimizer를 objectDetection에 적용 // Apply the Optimizer that we set up to objectDetection
		objectDetection.SetLearningOptimizerSpec(optSpec);

		// AugmentationSpec 설정 // Set the AugmentationSpec
		CAugmentationSpec augSpec;

		augSpec.EnableAugmentation(true);
		augSpec.SetCommonActivationRate(0.8);
		augSpec.SetCommonInterpolationMethod(FLImaging::ImageProcessing::EInterpolationMethod_Bilinear);
		augSpec.EnableRotation(true);
		augSpec.SetRotationParam(-30., 30., true, false);
		augSpec.EnableScale(true);
		augSpec.SetScaleParam(1., 1.2, 1., 1.2, true);
		augSpec.EnableHorizontalFlip(true);
		augSpec.EnableVerticalFlip(true);

		objectDetection.SetLearningAugmentationSpec(&augSpec);

		// 학습을 종료할 조건식 설정. map값이 0.9 이상인 경우 학습 종료한다. metric와 동일한 값입니다.
		// Set Conditional Expression to End Learning. If the map value is 0.9 or higher, end the learning. Same value as metric.
		objectDetection.SetLearningStopCondition(L"map >= 0.9");

		// 자동 저장 옵션 설정 // Set Auto-Save Options
		CAutoSaveSpec autoSaveSpec;

		// 자동 저장 활성화 // Enable Auto-Save
		autoSaveSpec.EnableAutoSave(true);
		// 저장할 모델 경로 설정 // Set Model path to save
		autoSaveSpec.SetAutoSavePath(L"model.flod");
		// 자동 저장 조건식 설정. 현재 map값이 최대 값인 경우 저장 활성화
		// Set auto-save conditional expressions. Enable save if the current map value is the maximum value
		autoSaveSpec.SetAutoSaveCondition(L"map > max('map')");

		// 자동 저장 옵션 설정 // Set Auto-Save Options
		objectDetection.SetLearningAutoSaveSpec(autoSaveSpec);

		// Learn 동작을 하는 핸들 객체 선언 // Declare HANDLE object execute learn function
		HANDLE hThread;

		// objectDetection learn function을 진행하는 스레드 생성 // Create the objectDetection Learn function thread
		hThread = (HANDLE)_beginthreadex(NULL, 0, LearnThread, (void*)&objectDetection, 0, nullptr);

		while(!objectDetection.IsRunning() && !g_bTerminated)
			CThreadUtilities::Sleep(1);

		int32_t i32MaxEpoch = objectDetection.GetLearningEpoch();
		int32_t i32PrevEpoch = 0;
		int32_t i32PrevCostCount = 0;
		int32_t i32PrevValidationCount = 0;

		while(true)
		{
			CThreadUtilities::Sleep(1);

			// 마지막 학습 횟수 받기 // Get the last epoch learning
			int32_t i32Epoch = objectDetection.GetLastEpoch();

			// 미니 배치 반복이 완료되면 cost와 validation 값을 디스플레이 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(i32Epoch != i32PrevEpoch && i32Epoch > 0)
			{
				// 마지막 학습 결과 비용 받기 // Get the last cost of the learning result
				float f32CurrCost = objectDetection.GetLearningResultLastCost();
				// 마지막 평균 학습 결과 비용 받기 // Get the last cost of the learning result
				float f32AvgCost = objectDetection.GetLearningResultLastAverageCost();
				// 마지막 검증 결과 받기 // Get the last validation result
				float f32Validation = objectDetection.GetLearningResultLastMeanAP();

				// 해당 epoch의 비용과 검증 결과 값 출력 // Print cost and validation value for the relevant epoch
				if(f32AvgCost < objectDetection.GetLearningRequiredCostForValidation())
					printf("Cost : %.6f Avg Cost : %.6f mAP : %.6f Epoch %d / %d\n", f32CurrCost, f32AvgCost, f32Validation, i32Epoch, i32MaxEpoch);
				else
					printf("Cost : %.6f Avg Cost : %.6f Epoch %d / %d\n", f32CurrCost, f32AvgCost, i32Epoch, i32MaxEpoch);

				// 학습 결과 비용과 검증 결과 기록을 받아 그래프 뷰에 출력  
				// Get the history of cost and validation and print it at graph view
				CFLArray<float> vctCosts;
				CFLArray<float> vctAvgCosts;
				CFLArray<float> vctmAP;
				CFLArray<int32_t> vctValidationEpoch;

				objectDetection.GetLearningResultAllHistory(&vctCosts, &vctAvgCosts, &vctmAP, &vctValidationEpoch);

				// 비용 기록이나 검증 결과 기록이 있다면 출력 // Print results if cost or validation history exists
				if((vctCosts.GetCount() && i32PrevCostCount != (int32_t)vctCosts.GetCount()) || (vctAvgCosts.GetCount() && i32PrevCostCount != (int32_t)vctAvgCosts.GetCount()) || (vctmAP.GetCount() && i32PrevValidationCount != (int32_t)vctValidationEpoch.GetCount()))
				{
					int32_t i32Step = objectDetection.GetLearningValidationStep();
					CFLArray<float> flaX;

					for(int64_t i = 0; i < vctmAP.GetCount() - 1; ++i)
						flaX.PushBack((float)(i * i32Step));

					flaX.PushBack((float)(vctCosts.GetCount() - 1));

					// 이전 그래프의 데이터를 삭제 // Clear previous graph data
					viewGraph.LockUpdate();
					viewGraph.Clear();
					// Graph View 데이터 입력 // Input Graph View Data
					viewGraph.Plot(vctCosts, EChartType_Line, RED, L"Cost");

					// Graph View 데이터 입력 // Input Graph View Data
					viewGraph.Plot(vctAvgCosts, EChartType_Line, BLUE, L"Avg Cost");
					viewGraph.Plot(flaX, vctmAP, EChartType_Line, PINK, L"mAP");
					viewGraph.UnlockUpdate();

					viewGraph.UpdateWindow();
					viewGraph.Invalidate();
					viewGraph.RedrawWindow();
				}

				// 검증 결과가 1.0일 경우 학습을 중단하고 분류 진행 
				// If the validation result is 1.0, stop learning and classify images 
				if(f32Validation == 1.f || GetAsyncKeyState(VK_ESCAPE))
					objectDetection.Stop();

				i32PrevEpoch = i32Epoch;
				i32PrevCostCount = (int32_t)vctCosts.GetCount();
				i32PrevValidationCount = (int32_t)vctmAP.GetCount();
			}
			// epoch만큼 학습이 완료되면 종료 // End when learning progresses as much as epoch
			if(!objectDetection.IsRunning() && g_bTerminated)
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
		objectDetection.SetInferenceImage(fliValidationImage);
		// 추론 결과 이미지 설정 // Set the inference result Image
		objectDetection.SetInferenceResultImage(fliResultLabelImage);
		// 추론 결과 옵션 설정 // Set the inference result options;
		// Result 결과의 옵션 설정 // Set the option of results
		objectDetection.SetInferenceResultItemSettings(CObjectDetectionDL::EInferenceResultItemSettings_ClassNum_ClassName_Objectness);

		// 알고리즘 수행 // Execute the algorithm
		if(IsFail(res = objectDetection.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}

		// 결과 이미지를 이미지 뷰에 맞게 조정합니다. // Fit the result image to the image view.
		viewImagesLabel.ZoomFit();

		// 이미지 뷰를 갱신 // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesLabel.RedrawWindow();

		// 그래프 뷰를 갱신 // Update the Graph view.
		viewGraph.RedrawWindow();

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageValidation.IsAvailable() && viewImagesLabel.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}