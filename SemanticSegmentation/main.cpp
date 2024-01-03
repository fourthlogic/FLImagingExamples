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
	CFLImage fliLearnImage;
	CFLImage fliValidationImage;
	CFLImage fliResultLabelImage;
	CFLImage fliResultLabelFigureImage;

	/// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageValidation;
	CGUIViewImageWrap viewImageResultLabel;
	CGUIViewImageWrap viewImageResultLabelFigure;

	// 그래프 뷰 선언 // Declare the graph view
	CGUIViewGraphWrap viewGraph;

	CResult eResult = EResult_UnknownError;

	do
	{
		// 이미지 로드 // Loads image
		if(IsFail(eResult = fliLearnImage.Load(L"../../ExampleImages/SemanticSegmentation/Train.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(eResult = fliValidationImage.Load(L"../../ExampleImages/SemanticSegmentation/Validation.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}


		if(IsFail(eResult = fliValidationImage.Load(L"../../ExampleImages/SemanticSegmentation/Validation.flif")))
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

		if(IsFail(eResult = viewImageValidation.Create(600, 0, 1100, 500)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageResultLabel.Create(100, 500, 600, 1000)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageResultLabelFigure.Create(600, 500, 1100, 1000)))
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

		viewGraph.SetDarkMode();

		// 다섯 개의 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the four image view windows
		if(IsFail(eResult = viewImageLearn.SynchronizeWindow(&viewImageValidation)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(eResult = viewImageLearn.SynchronizeWindow(&viewImageResultLabel)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(eResult = viewImageLearn.SynchronizeWindow(&viewImageResultLabelFigure)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the imageview
		if(IsFail(eResult = viewImageLearn.SetImagePtr(&fliLearnImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageValidation.SetImagePtr(&fliValidationImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		viewImageResultLabel.EnablePixelSegmentationMode(true);

		if(IsFail(eResult = viewImageResultLabel.SetImagePtr(&fliResultLabelImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		fliResultLabelFigureImage.Assign(fliValidationImage);
		fliResultLabelFigureImage.ClearFigures();

		if(IsFail(eResult = viewImageResultLabelFigure.SetImagePtr(&fliResultLabelFigureImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerValidation = viewImageValidation.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabel = viewImageResultLabel.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabelFigure = viewImageResultLabelFigure.GetLayer(0);

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
		if(IsFail(eResult = layerLearn.DrawTextCanvas(&CFLPointD(0, 0), L"LEARN", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerValidation.DrawTextCanvas(&CFLPointD(0, 0), L"VALIDATION", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerResultLabel.DrawTextCanvas(&CFLPointD(0, 0), L"RESULT LABEL", GREEN, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerResultLabelFigure.DrawTextCanvas(&CFLPointD(0, 0), L"RESULT LABEL FIGURE", GREEN, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImageResultLabel.RedrawWindow();
		viewImageResultLabelFigure.RedrawWindow();

		// SemanticSegmentation 객체 생성 // Create SemanticSegmentation object
		CSemanticSegmentationDL semanticSegmentation;

		// OptimizerSpec 객체 생성 // Create OptimizerSpec object
		COptimizerSpec optSpec;

		// 학습할 이미지 설정 // Set the image to learn
		semanticSegmentation.SetLearnImage(fliLearnImage);
		// 검증할 이미지 설정 // Set the image to validate
		semanticSegmentation.SetLearnValidationImage(fliValidationImage);
		// 분류할 이미지 설정 // Set the image to classify
		semanticSegmentation.SetSourceImage(fliValidationImage);

		// 학습할 SemanticSegmentation 모델 설정 // Set up SemanticSegmentation model to learn
		semanticSegmentation.SetModel(CSemanticSegmentationDL::EModel_FL_SS_GP);
		// 학습할 SemanticSegmentation 모델의 버전 설정 // Set up SemanticSegmentation model version to learn
		semanticSegmentation.SetModelVersion(CSemanticSegmentationDL::EModelVersion_FL_SS_GP_V1_512);
		// 학습 epoch 값을 설정 // Set the learn epoch value 
		semanticSegmentation.SetLearnEpoch(120);
		// 학습 이미지 Interpolation 방식 설정 // Set Interpolation method of learn image
		semanticSegmentation.SetInterpoloationMethod(EInterpolationMethod_Bilinear);

		// SemanticSegmentation의 Optimizer 타입 설정 // Set Optimizer type of SemanticSegmentation
		optSpec.SetOptimizerType(COptimizerSpec::EOptimizerType_AdamGradientDescent);
		// Optimizer의 학습률 설정 // Set learning rate of Optimizer
		optSpec.SetLearningRate(1e-4f);

		// 설정한 Optimizer를 SemanticSegmentation에 적용 // Apply the Optimizer that we set up to SemanticSegmentation
		semanticSegmentation.SetLearnOptimizerSpec(optSpec);

		// AugmentationSpec 설정 // Set the AugmentationSpec
		CAugmentationSpec augSpec;

		augSpec.SetCommonIOUThreshold(0.5);
		augSpec.SetCommonInterpolationMethod(FLImaging::ImageProcessing::EInterpolationMethod_Bilinear);
		augSpec.EnableRotation(true);
		augSpec.SetRotationParam(180., false);
		augSpec.EnableFlip(true);
		augSpec.EnableGaussianNoise(true);
		semanticSegmentation.SetLearnAugmentationSpec(&augSpec);

		// Learn 동작을 하는 핸들 객체 선언 // Declare HANDLE object execute learn function
		HANDLE hThread;

		// SemanticSegmentation learn function을 진행하는 스레드 생성 // Create the SemanticSegmentation Learn function thread
		hThread = (HANDLE)_beginthreadex(NULL, 0, LearnThread, (void*)&semanticSegmentation, 0, nullptr);

		while(!semanticSegmentation.IsRunning() && !g_bTerminated)
			CThreadUtilities::Sleep(1);

		int32_t i32MaxEpoch = semanticSegmentation.GetLearnEpoch();
		int32_t i32PrevEpoch = 0;
		int32_t i32PrevCostCount = 0;
		int32_t i32PrevValidationCount = 0;

		while(true)
		{
			CThreadUtilities::Sleep(1);

			// 마지막 미니 배치 최대 반복 횟수 받기 // Get the last maximum number of iterations of the last mini batch 
			int32_t i32MaxIteration = 0;
			semanticSegmentation.GetLastMaxIteration(i32MaxIteration);
			// 마지막 미니 배치 반복 횟수 받기 // Get the last number of mini batch iterations
			int32_t i32Iteration = 0;
			semanticSegmentation.GetLastIteration(i32Iteration);
			// 마지막 학습 횟수 받기 // Get the last epoch learning
			int32_t i32Epoch = 0;
			semanticSegmentation.GetLastEpoch(i32Epoch);

			// 미니 배치 반복이 완료되면 cost와 validation 값을 디스플레이 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(i32Epoch != i32PrevEpoch && i32Iteration == i32MaxIteration && i32Epoch > 0)
			{
				// 마지막 학습 결과 비용 받기 // Get the last cost of the learning result
				float f32CurrCost;
				semanticSegmentation.GetLastCost(f32CurrCost);
				// 마지막 검증 결과 받기 // Get the last validation result
				float f32ValidationPa = 0;
				float f32ValidationMIoU = 0;
				semanticSegmentation.GetLastValidationPixelAccuracy(f32ValidationPa);
				semanticSegmentation.GetLastValidationMIoU(f32ValidationMIoU);
				// 해당 epoch의 비용과 검증 결과 값 출력 // Print cost and validation value for the relevant epoch
				printf("Cost : %.6f Validation : %.6f mIoU : %.6f Epoch %d / %d\n", f32CurrCost, f32ValidationPa, f32ValidationMIoU, i32Epoch, i32MaxEpoch);

				// 학습 결과 비용과 검증 결과 기록을 받아 그래프 뷰에 출력  
				// Get the history of cost and validation and print it at graph view
				CFLArray<float> vctCosts;
				CFLArray<float> vctVadliationPixelAccuracy;
				CFLArray<float> vctMIoU;

				semanticSegmentation.GetCostHistory(vctCosts);
				semanticSegmentation.GetValidationHistory(vctVadliationPixelAccuracy);
				semanticSegmentation.GetMIoUHistory(vctMIoU);

				// 비용 기록이나 검증 결과 기록이 있다면 출력 // Print results if cost or validation history exists
				if((vctCosts.GetCount() && i32PrevCostCount != (int32_t)vctCosts.GetCount()) || (vctVadliationPixelAccuracy.GetCount() && i32PrevValidationCount != (int32_t)vctVadliationPixelAccuracy.GetCount()))
				{
					int32_t i32Step = semanticSegmentation.GetLearnValidationStep();
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
					viewGraph.Plot(flaX, vctMIoU, EChartType_Line, PINK, L"mIoU");
					viewGraph.UnlockUpdate();

					viewGraph.UpdateWindow();
					viewGraph.Invalidate();
					viewGraph.RedrawWindow();
				}

				// 검증 결과가 1.0일 경우 학습을 중단하고 분류 진행 
				// If the validation result is 1.0, stop learning and classify images 
				if(f32ValidationPa == 1.f)
					semanticSegmentation.Stop();

				i32PrevEpoch = i32Epoch;
				i32PrevCostCount = (int32_t)vctCosts.GetCount();
				i32PrevValidationCount = (int32_t)vctVadliationPixelAccuracy.GetCount();
			}
			// epoch만큼 학습이 완료되면 종료 // End when learning progresses as much as epoch
			if(!semanticSegmentation.IsRunning() && g_bTerminated)
			{
				// learn 동작 스레드가 완전히 종료될 까지 대기 // Wait until learning is completely terminated
				WaitForSingleObject(hThread, INFINITE);
				// 스레드 핸들 종료 // Close thread handle
				CloseHandle(hThread);
				break;
			}
		}

		// 알고리즘 수행 // Execute the algorithm
		if(IsFail(eResult = semanticSegmentation.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute.\n");
			break;
		}

		// ResultLabelImage 받아오기 // Get The ResultLabelImage
		semanticSegmentation.GetInferenceResultLabelImage(&fliResultLabelImage);

		// Blob 객체 생성 // Create the blob object
		CBlob blob;

		// Blob 파라미터 셋팅 // Set the blob's parameters
		CMultiVar<int64_t> mvLogical((int64_t)ELogicalCondition_Equal);
		CMultiVar<double> mvThreshold(0);
		int32_t i32Mask = (int32_t)CBlob::EBlobResultType_Contour;

		//blob.EnableSynchronization(false);
		blob.SetLogicalCondition(mvLogical);
		blob.SetThresholdMode(EThresholdMode_Single);
		blob.SetResultType(CBlob::EBlobResultType(i32Mask));
		blob.SetContourResultType(CBlob::EContourResultType_Perforated);
		blob.SetSourceImage(fliResultLabelImage);
		blob.SetThreshold(mvThreshold);

		// 라벨 카테고리 수만큼 반복하여 Blob 수행 // Perform Blob repeatedly as many times as the number of label categories
		int64_t i64LoopCount = semanticSegmentation.GetLearntResultLabelCategoryCount();

		for(int64_t i = 1; i < i64LoopCount; ++i)
		{
			mvThreshold[0] = (double)i;

			blob.SetThreshold(mvThreshold);

			if(IsFail(eResult = blob.Execute()))
			{
				ErrorPrint(eResult, "Failed to process\n");
				break;
			}

			CFLFigureArray flfaContours;

			if(IsFail(eResult = blob.GetResultContours(flfaContours)))
			{
				ErrorPrint(eResult, "Failed to process\n");
				break;
			}

			Base::CFLArray<Base::CFLStringW> flaNames;
			int32_t i32ClassNumber = (int32_t)i;
			semanticSegmentation.GetLearntResultLabelCategoryClassNames(i, &flaNames);


			CFLStringW flsLabel;

			flsLabel.Format(L"%d(%s)", i32ClassNumber, flaNames[0].GetString());

			// Blob figure 결과를 이미지에 붙여넣기 // Paste blob's results to the result blob image
			for(int64_t j = 0; j < flfaContours.GetCount(); ++j)
			{
				CFLFigure* pFlrg = flfaContours.GetAt(j);

				pFlrg->SetName(flsLabel);
				fliResultLabelFigureImage.PushBackFigure(CROIUtilities::ConvertFigureObjectToString(pFlrg));
			}

		}

		// ResultLabl 뷰에 Floating Value Range를 설정
		viewImageResultLabel.SetFloatingImageValueRange(0.f, (float)semanticSegmentation.GetLearntResultLabelCategoryCount());

		// 이미지 뷰를 갱신 // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImageResultLabel.RedrawWindow();
		viewImageResultLabelFigure.RedrawWindow();

		// 그래프 뷰를 갱신 // Update the Graph view.
		viewGraph.RedrawWindow();

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageValidation.IsAvailable() && viewImageResultLabel.IsAvailable() && viewImageResultLabelFigure.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}