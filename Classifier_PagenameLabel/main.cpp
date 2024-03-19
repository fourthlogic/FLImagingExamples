#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"
#include <cfloat>
#include <cmath>
#include <iostream>
#include <limits>
#include <process.h>

bool g_bTerminated = false;
CResult g_resLearnResult;
// 
unsigned int __stdcall LearnThread(void* pParam)
{
	CClassifierDL* pClassifier = (CClassifierDL*)pParam;

	g_resLearnResult = pClassifier->Learn();
	g_bTerminated = true;

	return unsigned int(g_resLearnResult);
};

int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliLearnImage;
	CFLImage fliSourceImage;

	/// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageSource;

	// 그래프 뷰 선언 // Declare the graph view
	CGUIViewGraphWrap viewGraph;

	CResult eResult = EResult_UnknownError;

	do
	{
		// 이미지 로드 // Loads image
		if(IsFail(eResult = fliLearnImage.Load(L"../../ExampleImages/Classifier/mnist1000.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(eResult = fliSourceImage.Load(L"../../ExampleImages/Classifier/mnist100.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Creates image view
		if(IsFail(eResult = viewImageLearn.Create(100, 0, 600, 500)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageSource.Create(600, 0, 1100, 500)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Graph 뷰 생성 // Create graph view
		if(IsFail(eResult = viewGraph.Create(1100, 0, 1600, 500)))
		{
			ErrorPrint(eResult, " Failed to create the graph view. \n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the imageview
		if(IsFail(eResult = viewImageLearn.SetImagePtr(&fliLearnImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImageLearn.SynchronizeWindow(&viewImageSource)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerLearn.Clear();
		layerSource.Clear();

		// View 정보를 디스플레이 합니다. // Display View information.
		// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = layerLearn.DrawTextCanvas(&CFLPointD(0, 0), L"LEARN", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"SOURCE", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// Classifier 객체 생성 // Create Classifier object
		CClassifierDL classifier;

		// OptimizerSpec 객체 생성 // Create OptimizerSpec object
		COptimizerSpecAdamGradientDescent optSpec;

		// 학습할 이미지 설정 // Set the image to learn
		classifier.SetLearningImage(fliLearnImage);
		// 검증할 이미지 설정 // Set the image to validate
		classifier.SetLearningValidationImage(fliSourceImage);
		// 분류할 이미지 설정 // Set the image to classify
		classifier.SetInferenceImage(fliSourceImage);
		classifier.SetInferenceResultImage(fliSourceImage);

		// 학습할 Classifier 모델 설정 // Set up Classifier model to learn
		classifier.SetModel(CClassifierDL::EModel_FL_CF_C);
		// 학습할 Classifier 모델 설정 // Set up Classifier model to learn
		classifier.SetModelVersion(CClassifierDL::EModelVersion_FL_CF_C_V1_32);
		// 학습 epoch 값을 설정 // Set the learn epoch value 
		classifier.SetLearningEpoch(150);
		// 학습 이미지 Interpolation 방식 설정 // Set Interpolation method of learn image
		classifier.SetInterpoloationMethod(EInterpolationMethod_Bilinear);

		// Optimizer의 학습률 설정 // Set learning rate of Optimizer
		optSpec.SetLearningRate(1e-3f);

		// 설정한 Optimizer를 Classifier에 적용 // Apply the Optimizer that we set up to Classifier
		classifier.SetLearningOptimizerSpec(optSpec);

		// Learn 동작을 하는 핸들 객체 선언 // Declare HANDLE object execute learn function
		HANDLE hThread;

		// Classifier learn function을 진행하는 스레드 생성 // Create the Classifier Learn function thread
		hThread = (HANDLE)_beginthreadex(NULL, 0, LearnThread, (void*)&classifier, 0, nullptr);

		while(!classifier.IsRunning() && !g_bTerminated)
			CThreadUtilities::Sleep(1);

		int32_t i32MaxEpoch = classifier.GetLearningEpoch();
		int32_t i32PrevEpoch = 0;
		int32_t i32PrevCostCount = 0;
		int32_t i32PrevValidationCount = 0;

		while(true)
		{
			CThreadUtilities::Sleep(1);

			// 마지막 미니 배치 최대 반복 횟수 받기 // Get the last maximum number of iterations of the last mini batch 
			int i32MiniBatchCount = classifier.GetActualMiniBatchCount();
			// 마지막 미니 배치 최대 반복 횟수 받기 // Get the last maximum number of iterations of the last mini batch 
			int32_t i32MaxIteration = classifier.GetActualMiniBatchCount();
			// 마지막 미니 배치 반복 횟수 받기 // Get the last number of mini batch iterations
			int32_t i32Iteration = classifier.GetLearningResultCurrentIteration();
			// 마지막 학습 횟수 받기 // Get the last epoch learning
			int32_t i32Epoch = classifier.GetLastEpoch();

			// 미니 배치 반복이 완료되면 cost와 validation 값을 디스플레이 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(i32Epoch != i32PrevEpoch && i32Iteration == i32MaxIteration && i32Epoch > 0)
			{
				// 학습 결과 비용과 검증 결과 기록을 받아 그래프 뷰에 출력  
				// Get the history of cost and validation and print it at graph view
				CFLArray<float> vctCosts;
				CFLArray<float> vctValidations;
				CFLArray<float> vctF1Score;

				classifier.GetLearningResultAllHistory(vctCosts, vctValidations, vctF1Score);

				// 마지막 학습 결과 비용 받기 // Get the last cost of the learning result
				float f32CurrCost = vctCosts.Back();
				// 마지막 검증 결과 받기 // Get the last validation result
				float f32Validation = vctValidations.Back();
				// 마지막 F1점수 결과 받기 // Get the last F1 Score result
				float f32F1Score = vctF1Score.Back();
				// 해당 epoch의 비용과 검증 결과 값 출력 // Print cost and validation value for the relevant epoch
				printf("Cost : %.6f Validation : %.6f F1 Score : %.6f Epoch %d / %d\n", f32CurrCost, f32Validation, f32F1Score, i32Epoch, i32MaxEpoch);

				// 비용 기록이나 검증 결과 기록이 있다면 출력 // Print results if cost or validation history exists
				if((vctCosts.GetCount() && i32PrevCostCount != (int32_t)vctCosts.GetCount()) || (vctValidations.GetCount() && i32PrevValidationCount != (int32_t)vctValidations.GetCount()))
				{
					viewGraph.LockUpdate();

					// 이전 그래프의 데이터를 삭제 // Clear previous grpah data
					viewGraph.Clear();
					// Graph View 데이터 입력 // Input Graph View Data
					viewGraph.Plot(vctCosts, EChartType_Line, RED, L"Cost");

					int32_t i32Step = classifier.GetLearningValidationStep();
					CFLArray<float> flaX;

					for(int64_t i = 0; i < vctValidations.GetCount() - 1; ++i)
						flaX.PushBack((float)(i * i32Step));

					flaX.PushBack((float)(vctCosts.GetCount() - 1));
					// Graph View 데이터 입력 // Input Graph View Data
					viewGraph.Plot(flaX, vctValidations, EChartType_Line, BLUE, L"Validation");

					viewGraph.UnlockUpdate();
					viewGraph.Invalidate();
				}

				// 검증 결과가 1.0일 경우 학습을 중단하고 분류 진행 
				// If the validation result is 1.0, stop learning and classify images 
				if(f32Validation == 1.f)
					classifier.Stop();

				i32PrevEpoch = i32Epoch;
				i32PrevCostCount = (int32_t)vctCosts.GetCount();
				i32PrevValidationCount = (int32_t)vctValidations.GetCount();
			}

			// epoch만큼 학습이 완료되면 종료 // End when learning progresses as much as epoch
			if(!classifier.IsRunning())
			{
				// learn 동작 스레드가 완전히 종료될 까지 대기 // Wait until learning is completely terminated
				WaitForSingleObject(hThread, INFINITE);
				// 스레드 핸들 종료 // Close thread handle
				CloseHandle(hThread);
				break;
			}
		}

		if(g_resLearnResult.IsFail())
		{
			ErrorPrint(g_resLearnResult, "Failed to Learn.\n");
			break;
		}

		// 추론 결과 정보에 대한 설정 // Set for the inference result information
		classifier.SetInferenceResultItemSettings(CClassifierDL::EInferenceResultItemSettings_ClassNum_ClassName_Percentage);

		// 알고리즘 수행 // Execute the algorithm
		if(IsFail(eResult = classifier.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute Learn.\n");
			break;
		}

		// 이미지 뷰를 갱신 // Update the image view.
		viewImageLearn.Invalidate(true);
		viewImageSource.Invalidate(true);
		// 그래프 뷰를 갱신 // Update the Graph view.
		viewGraph.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageSource.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}