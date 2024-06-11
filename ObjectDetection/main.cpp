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
	CObjectDetectionDL* pobjectDetection = (CObjectDetectionDL*)pParam;

	CResult er = pobjectDetection->Learn();
	g_bTerminated = true;

	return unsigned int(er);
};

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliLearnImage;
	CFLImage fliValidationImage;
	CFLImage fliResultLabelImage;

	/// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageValidation;
	CGUIViewImageWrap viewImagesLabel;

	// �׷��� �� ���� // Declare the graph view
	CGUIViewGraphWrap viewGraph;

	CResult res = EResult_UnknownError;

	do
	{
		// ���̺귯���� ������ �ε� �� ������ ��ٸ� // Wait for the library to fully load
		CThreadUtilities::Sleep(1000);

		// �̹��� �ε� // Loads image
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

		// �̹��� �� ���� // Creates image view
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

		// Graph �� ���� // Create graph view
		if(IsFail(res = viewGraph.Create(1100, 0, 1600, 500)))
		{
			ErrorPrint(res, " Failed to create the graph view. \n");
			break;
		}

		viewGraph.SetDarkMode();

		// �ټ� ���� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the four image view windows
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

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
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

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerValidation = viewImageValidation.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabel = viewImagesLabel.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerLearn.Clear();
		layerValidation.Clear();
		layerResultLabel.Clear();
	
		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
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

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesLabel.RedrawWindow();

		// objectDetection ��ü ���� // Create objectDetection object
		CObjectDetectionDL objectDetection;

		// OptimizerSpec ��ü ���� // Create OptimizerSpec object
		COptimizerSpecAdamGradientDescent optSpec;

		// �н��� �̹��� ���� // Set the image to learn
		objectDetection.SetLearningImage(fliLearnImage);
		// ������ �̹��� ���� // Set the image to validate
		objectDetection.SetLearningValidationImage(fliValidationImage);
		// �н��� objectDetection �� ���� // Set up objectDetection model to learn
		objectDetection.SetModel(CObjectDetectionDL::EModel_YOLO);
		// �н��� objectDetection ���� ���� ���� // Set up objectDetection model version to learn
		objectDetection.SetModelVersion(CObjectDetectionDL::EModelVersion_YOLO_V4_256);
		// �н� epoch ���� ���� // Set the learn epoch value 
		objectDetection.SetLearningEpoch(1024);
		// �н� �̹��� Interpolation ��� ���� // Set Interpolation method of learn image
		objectDetection.SetInterpolationMethod(EInterpolationMethod_Bilinear);
	    // ������ ���� �� �ּ� ��� Cost�� ���� // Set the minimum average cost value at which verification will be triggered
		objectDetection.SetLearningRequiredCostForValidation(5.f);
		// �н� �ߴ� Metrics �� ���� // Set Learning Stop Metrics Values
		objectDetection.SetLearningStopConditionMetrics(.9f);
		// Optimizer�� �н��� ���� // Set learning rate of Optimizer
		optSpec.SetLearningRate(5e-4f);
		optSpec.SetWeightDecay(0.f);

		// ������ Optimizer�� objectDetection�� ���� // Apply the Optimizer that we set up to objectDetection
		objectDetection.SetLearningOptimizerSpec(optSpec);

		// AugmentationSpec ���� // Set the AugmentationSpec
		CAugmentationSpec augSpec;

		augSpec.SetCommonActivationRatio(0.8);
		augSpec.SetCommonInterpolationMethod(FLImaging::ImageProcessing::EInterpolationMethod_Bilinear);
		augSpec.EnableRotation(true);
		augSpec.SetRotationParam(30., true, false);
		augSpec.EnableScaleCrop(true);
		augSpec.SetScaleCropParam(.8, 1.0, true, 1., 1., 1., 1., true);
		augSpec.EnableHorizontalFlip(true);
		augSpec.EnableVerticalFlip(true);

		objectDetection.SetLearningAugmentationSpec(&augSpec);

		// Learn ������ �ϴ� �ڵ� ��ü ���� // Declare HANDLE object execute learn function
		HANDLE hThread;

		// objectDetection learn function�� �����ϴ� ������ ���� // Create the objectDetection Learn function thread
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

			// ������ �н� Ƚ�� �ޱ� // Get the last epoch learning
			int32_t i32Epoch = objectDetection.GetLastEpoch();

			// �̴� ��ġ �ݺ��� �Ϸ�Ǹ� cost�� validation ���� ���÷��� 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(i32Epoch != i32PrevEpoch && i32Epoch > 0)
			{
				// ������ �н� ��� ��� �ޱ� // Get the last cost of the learning result
				float f32CurrCost = objectDetection.GetLearningResultLastCost();
				// ������ ��� �н� ��� ��� �ޱ� // Get the last cost of the learning result
				float f32AvgCost = objectDetection.GetLearningResultLastAverageCost();
				// ������ ���� ��� �ޱ� // Get the last validation result
				float f32Validation = objectDetection.GetLearningResultLastMeanAP();
				// ������ WrittenBox �� �ޱ� // Get last WrittenBox value
				float f32WrittenBox = objectDetection.GetLearningResultLastRewrittenBox();

				// �ش� epoch�� ���� ���� ��� �� ��� // Print cost and validation value for the relevant epoch
				if(f32AvgCost < objectDetection.GetLearningRequiredCostForValidation())
					printf("Cost : %.6f Avg Cost : %.6f mAP : %.6f Epoch %d / %d\n", f32CurrCost, f32AvgCost, f32Validation, i32Epoch, i32MaxEpoch);
				else
					printf("Cost : %.6f Avg Cost : %.6f Epoch %d / %d\n", f32CurrCost, f32AvgCost, i32Epoch, i32MaxEpoch);

				// �н� ��� ���� ���� ��� ����� �޾� �׷��� �信 ���  
				// Get the history of cost and validation and print it at graph view
				CFLArray<float> vctCosts;
				CFLArray<float> vctAvgCosts;
				CFLArray<float> vctmAP;
				CFLArray<int32_t> vctValidationEpoch;

				objectDetection.GetLearningResultAllHistory(&vctCosts, &vctAvgCosts, &vctmAP, &vctValidationEpoch);

				// ��� ����̳� ���� ��� ����� �ִٸ� ��� // Print results if cost or validation history exists
				if((vctCosts.GetCount() && i32PrevCostCount != (int32_t)vctCosts.GetCount()) || (vctAvgCosts.GetCount() && i32PrevCostCount != (int32_t)vctAvgCosts.GetCount()) || (vctmAP.GetCount() && i32PrevValidationCount != (int32_t)vctValidationEpoch.GetCount()))
				{
					int32_t i32Step = objectDetection.GetLearningValidationStep();
					CFLArray<float> flaX;

					for(int64_t i = 0; i < vctmAP.GetCount() - 1; ++i)
						flaX.PushBack((float)(i * i32Step));

					flaX.PushBack((float)(vctCosts.GetCount() - 1));

					// ���� �׷����� �����͸� ���� // Clear previous graph data
					viewGraph.LockUpdate();
					viewGraph.Clear();
					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(vctCosts, EChartType_Line, RED, L"Cost");

					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(vctAvgCosts, EChartType_Line, BLUE, L"Avg Cost");
					viewGraph.Plot(flaX, vctmAP, EChartType_Line, PINK, L"mAP");
					viewGraph.UnlockUpdate();

					viewGraph.UpdateWindow();
					viewGraph.Invalidate();
					viewGraph.RedrawWindow();
				}

				// ���� ����� 1.0�� ��� �н��� �ߴ��ϰ� �з� ���� 
				// If the validation result is 1.0, stop learning and classify images 
				if(f32Validation == 1.f || GetAsyncKeyState(VK_ESCAPE))
					objectDetection.Stop();

				i32PrevEpoch = i32Epoch;
				i32PrevCostCount = (int32_t)vctCosts.GetCount();
				i32PrevValidationCount = (int32_t)vctmAP.GetCount();
			}
			// epoch��ŭ �н��� �Ϸ�Ǹ� ���� // End when learning progresses as much as epoch
			if(!objectDetection.IsRunning() && g_bTerminated)
			{
				// learn ���� �����尡 ������ ����� ���� ��� // Wait until learning is completely terminated
				WaitForSingleObject(hThread, INFINITE);
				// ������ �ڵ� ���� // Close thread handle
				CloseHandle(hThread);
				break;
			}
		}

		// Result Label Image�� �ǰܸ� �������� �ʴ� Execute
		// �з��� �̹��� ���� // Set the image to classify
		objectDetection.SetInferenceImage(fliValidationImage);
		// �߷� ��� �̹��� ���� // Set the inference result Image
		objectDetection.SetInferenceResultImage(fliResultLabelImage);
		// �߷� ��� �ɼ� ���� // Set the inference result options;
		// Result ����� �ɼ� ���� // Set the option of results
		objectDetection.SetInferenceResultItemSettings(CObjectDetectionDL::EInferenceResultItemSettings_ClassNum_ClassName_Objectness);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = objectDetection.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesLabel.RedrawWindow();

		// �׷��� �並 ���� // Update the Graph view.
		viewGraph.RedrawWindow();

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageValidation.IsAvailable() && viewImagesLabel.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}