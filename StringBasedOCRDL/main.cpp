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
	CStringBasedOCRDL* pStringBasedOCR = (CStringBasedOCRDL*)pParam;

	g_resLearnResult = pStringBasedOCR->Learn();
	g_bTerminated = true;

	return unsigned int(g_resLearnResult);
};

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliLearnImage;
	CFLImage fliSourceImage;
	CFLImage fliResultImage;

	/// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageResult;

	// �׷��� �� ���� // Declare the graph view
	CGUIViewGraphWrap viewGraph;

	CResult res = EResult_UnknownError;

	do
	{
		// ���̺귯���� ������ �ε� �� ������ ��ٸ� // Wait for the library to fully load
		CThreadUtilities::Sleep(1000);

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliLearnImage.Load(L"../../ExampleImages/StringBasedOCR/Learn.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliSourceImage.Load(L"../../ExampleImages/StringBasedOCR/Source.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates image view
		if(IsFail(res = viewImageLearn.Create(100, 0, 600, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageSource.Create(600, 0, 1100, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageResult.Create(100, 500, 600, 1000)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Graph �� ���� // Create graph view
		if(IsFail(res = viewGraph.Create(1100, 0, 1600, 500)))
		{
			ErrorPrint(res, " Failed to create the graph view. \n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(res = viewImageLearn.SetImagePtr(&fliLearnImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageResult.SetImagePtr(&fliResultImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImageSource)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImageResult)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerResult = viewImageResult.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerLearn.Clear();
		layerSource.Clear();
		layerResult.Clear();

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = layerLearn.DrawTextCanvas(&CFLPoint<double>(0, 0), L"LEARN", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Validation", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerResult.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Result", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// StringBasedOCR ��ü ���� // Create StringBasedOCR object
		CStringBasedOCRDL ocrdl;
		
		// �н��� �̹��� ���� // Set the image to learn
		ocrdl.SetLearningImage(fliLearnImage);
		// ������ �̹��� ���� // Set the image to validate
		ocrdl.SetLearningValidationImage(fliSourceImage);

		// �н��� StringBasedOCR �� ���� // Set up StringBasedOCR model to learn
		ocrdl.SetModel(CStringBasedOCRDL::EModel_FLNet);
		// �н��� StringBasedOCR �� ���� // Set up StringBasedOCR model to learn
		ocrdl.SetModelVersion(CStringBasedOCRDL::EModelVersion_FLNet_V1_32_256);
		// �н� epoch ���� ���� // Set the learn epoch value 
		ocrdl.SetLearningEpoch(500);
		// �н� �̹��� Interpolation ��� ���� // Set Interpolation method of learn image
		ocrdl.SetInterpolationMethod(EInterpolationMethod_Bilinear);

		// OptimizerSpec ��ü ���� // Create OptimizerSpec object
		COptimizerSpecAdamGradientDescent optSpec;

		// Optimizer�� �н��� ���� // Set learning rate of Optimizer
		optSpec.SetLearningRate(1e-3f);

		// ������ Optimizer�� StringBasedOCR�� ���� // Apply the Optimizer that we set up to StringBasedOCR
		ocrdl.SetLearningOptimizerSpec(optSpec);
		ocrdl.EnableOptimalLearningStatePreservation(true);

		// AugmentationSpec ���� // Set the AugmentationSpec
		CAugmentationSpec augSpec;

		augSpec.EnableAugmentation(true);
		augSpec.SetCommonActivationRatio(0.8);
		augSpec.SetCommonIoUThreshold(0.8);
		augSpec.SetCommonInterpolationMethod(FLImaging::ImageProcessing::EInterpolationMethod_Bilinear);
		augSpec.EnableRotation(true);
		augSpec.SetRotationParam(-10., 10., false, false);

		augSpec.EnableScale(true);
		augSpec.SetScaleParam(.8, 1.2, .8, 1.2, false);

		augSpec.EnableTranslation(true);
		augSpec.SetTranslationParam(0., .1, 0., .1);

		ocrdl.SetLearningAugmentationSpec(&augSpec);

		// �н��� ������ ���ǽ� ����. Metric ���� 1.0 �̻��� ��� �н� �����Ѵ�. Metric = (1-NED + mAP) / 2
		// Set Conditional Expression to End Learning. If the metric value is 1.0 or higher, end the learning. Metric = (1-NED + mAP) / 2
		ocrdl.SetLearningStopCondition(L"metric >= 1");

		// �ڵ� ���� �ɼ� ���� // Set Auto-Save Options
		CAutoSaveSpec autoSaveSpec;

		// �ڵ� ���� Ȱ��ȭ // Enable Auto-Save
		autoSaveSpec.EnableAutoSave(true);
		// ������ �� ��� ���� // Set Model path to save
		autoSaveSpec.SetAutoSavePath(L"model.flsbocrdl");
		// �ڵ� ���� ���ǽ� ����. ���� Metric ���� �ִ� ���� ��� ���� Ȱ��ȭ
		// Set auto-save conditional expressions. Enable save if the current metric value is the maximum value
		autoSaveSpec.SetAutoSaveCondition(L"epoch >= 10 & metric > max('metric')");

		// �ڵ� ���� �ɼ� ���� // Set Auto-Save Options
		ocrdl.SetLearningAutoSaveSpec(autoSaveSpec);

		// Learn ������ �ϴ� �ڵ� ��ü ���� // Declare HANDLE object execute learn function
		HANDLE hThread;

		// StringBasedOCR learn function�� �����ϴ� ������ ���� // Create the StringBasedOCR Learn function thread
		hThread = (HANDLE)_beginthreadex(NULL, 0, LearnThread, (void*)&ocrdl, 0, nullptr);

		while(!ocrdl.IsRunning() && !g_bTerminated)
			CThreadUtilities::Sleep(1);

		int32_t i32MaxEpoch = ocrdl.GetLearningEpoch();
		int32_t i32PrevEpoch = 0;
		int32_t i32PrevCostCount = 0;
		int32_t i32PrevValidationCount = 0;

		while(true)
		{
			CThreadUtilities::Sleep(1);

			// ������ �̴� ��ġ �ִ� �ݺ� Ƚ�� �ޱ� // Get the last maximum number of iterations of the last mini batch 
			int i32MiniBatchCount = ocrdl.GetActualMiniBatchCount();
			// ������ �̴� ��ġ �ִ� �ݺ� Ƚ�� �ޱ� // Get the last maximum number of iterations of the last mini batch 
			int32_t i32MaxIteration = ocrdl.GetActualMiniBatchCount();
			// ������ �̴� ��ġ �ݺ� Ƚ�� �ޱ� // Get the last number of mini batch iterations
			int32_t i32Iteration = ocrdl.GetLearningResultCurrentIteration();
			// ������ �н� Ƚ�� �ޱ� // Get the last epoch learning
			int32_t i32Epoch = ocrdl.GetLastEpoch();

			// �н� ��� ���� ���� ��� ����� �޾� �׷��� �信 ���  
			// Get the history of cost and validation and print it at graph view
			CFLArray<float> vctCosts;
			CFLArray<float> vct1MNED;
			CFLArray<float> vctMeanAP;
			CFLArray<int32_t> vctValidationEpoch;

			ocrdl.GetLearningResultAllHistory(vctCosts, vct1MNED, vctMeanAP, vctValidationEpoch);
			
			// �̴� ��ġ �ݺ��� �Ϸ�Ǹ� cost�� validation ���� ���÷��� 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(vctCosts.GetCount() && i32Epoch != i32PrevEpoch && i32Iteration == i32MaxIteration && i32Epoch > 0)
			{
				// ������ �н� ��� ��� �ޱ� // Get the last cost of the learning result
				float f32CurrCost = vctCosts.Back();
				// ������ 1-NED ��� �ޱ� // Get the last 1-NED result
				float f321MNED = vct1MNED.GetCount() ? vct1MNED.Back() : 0;
				// ������ MeanAP ��� �ޱ� // Get the last MeanAP result
				float f32MeanAP = vctMeanAP.GetCount() ? vctMeanAP.Back() : 0;
				// �ش� epoch�� ���� ���� ��� �� ��� // Print cost and validation value for the relevant epoch
				printf("Cost : %.6f 1-NED : %.6f mAP : %.6f Epoch %d / %d\n", f32CurrCost, f321MNED, f32MeanAP, i32Epoch, i32MaxEpoch);

				// ��� ����̳� ���� ��� ����� �ִٸ� ��� // Print results if cost or validation history exists
				if((vctCosts.GetCount() && i32PrevCostCount != (int32_t)vctCosts.GetCount()) || (vct1MNED.GetCount() && i32PrevValidationCount != (int32_t)vct1MNED.GetCount()) || (vctMeanAP.GetCount() && i32PrevValidationCount != (int32_t)vctMeanAP.GetCount()))
				{
					int32_t i32Step = ocrdl.GetLearningValidationStep();
					CFLArray<float> flaX;

					for(int64_t i = 0; i < vct1MNED.GetCount() - 1; ++i)
						flaX.PushBack((float)(i * i32Step));

					flaX.PushBack((float)(vctCosts.GetCount() - 1));

					viewGraph.LockUpdate();

					// ���� �׷����� �����͸� ���� // Clear previous grpah data
					viewGraph.Clear();
					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(vctCosts, EChartType_Line, RED, L"Cost");
					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(flaX, vct1MNED, EChartType_Line, BLUE, L"1-NED");
					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(flaX, vctMeanAP, EChartType_Line, GREEN, L"mAP");

					viewGraph.UnlockUpdate();
					viewGraph.Invalidate();
				}

				// ���� ����� 1.0�� ��� �н��� �ߴ��ϰ� �ν� ���� 
				// If the validation result is 1.0, stop learning and recognize
				if(f321MNED == 1.f && f32MeanAP == 1.f || GetAsyncKeyState(VK_ESCAPE))
					ocrdl.Stop();

				i32PrevEpoch = i32Epoch;
				i32PrevCostCount = (int32_t)vctCosts.GetCount();
				i32PrevValidationCount = (int32_t)vct1MNED.GetCount();
			}

			// epoch��ŭ �н��� �Ϸ�Ǹ� ���� // End when learning progresses as much as epoch
			if(!ocrdl.IsRunning())
			{
				// learn ���� �����尡 ������ ����� ���� ��� // Wait until learning is completely terminated
				WaitForSingleObject(hThread, INFINITE);
				// ������ �ڵ� ���� // Close thread handle
				CloseHandle(hThread);
				break;
			}
		}

		if(g_resLearnResult.IsFail())
		{
			ErrorPrint(g_resLearnResult, "Failed to Learn.\n");
			break;
		}

		// �ν��� �̹��� ���� // Set the image to Recognize
		ocrdl.SetInferenceImage(fliSourceImage);
		ocrdl.SetInferenceResultImage(fliResultImage);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = ocrdl.Execute()))
		{
			ErrorPrint(res, "Failed to execute Learn.\n");
			break;
		}

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageSource.RedrawWindow();
		viewImageResult.RedrawWindow();
		// �׷��� �並 ���� // Update the Graph view.
		viewGraph.RedrawWindow();

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageSource.IsAvailable() && viewImageResult.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}