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
	CAnomalyDetectionDL* pAnomalyDetection = (CAnomalyDetectionDL*)pParam;

	CResult er = pAnomalyDetection->Learn();
	g_bTerminated = true;

	return unsigned int(er);
};

int main()
{
				// �̹��� ��ü ���� // Declare the image object
	CFLImage fliLearnImage;
	CFLImage fliValidationImage;
	CFLImage fliResultLabelFigureImage;

	/// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageValidation;
	CGUIViewImageWrap viewImagesLabelFigure;

	// �׷��� �� ���� // Declare the graph view
	CGUIViewGraphWrap viewGraph;

	CResult res = EResult_UnknownError;

	do
	{
		// ���̺귯���� ������ �ε� �� ������ ��ٸ� // Wait for the library to fully load
		CThreadUtilities::Sleep(1000);

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliLearnImage.Load(L"../../ExampleImages/AnomalyDetection/AnomalyDetectionTrainData.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliValidationImage.Load(L"../../ExampleImages/AnomalyDetection/AnomalyDetectionValidationData.flif")))
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

		if(IsFail(res = viewImagesLabelFigure.Create(100, 500, 600, 1000)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewGraph.Create(600, 500, 1100, 1000)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		viewGraph.SetDarkMode();

		// �ټ� ���� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the four image view windows
		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImageValidation)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImagesLabelFigure)))
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

		fliResultLabelFigureImage.Assign(fliValidationImage);
		fliResultLabelFigureImage.ClearFigures();

		if(IsFail(res = viewImagesLabelFigure.SetImagePtr(&fliResultLabelFigureImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerValidation = viewImageValidation.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabelFigure = viewImagesLabelFigure.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerLearn.Clear();
		layerValidation.Clear();
		layerResultLabelFigure.Clear();

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

		if(IsFail(res = layerValidation.DrawTextCanvas(&CFLPoint<double>(0, 0), L"VALIDATION", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerResultLabelFigure.DrawTextCanvas(&CFLPoint<double>(0, 0), L"RESULT LABEL FIGURE", GREEN, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesLabelFigure.RedrawWindow();

		// AnomalyDetection ��ü ���� // Create AnomalyDetection object
		CAnomalyDetectionDL AnomalyDetection;

		// OptimizerSpec ��ü ���� // Create OptimizerSpec object
		COptimizerSpecAdamGradientDescent optSpec;

		// �н��� �̹��� ���� // Set the image to learn
		AnomalyDetection.SetLearningImage(fliLearnImage);
		// �н��� AnomalyDetection �� ���� // Set up AnomalyDetection model to learn
		AnomalyDetection.SetModel(CAnomalyDetectionDL::EModel_FLDefNet);
		// �н��� AnomalyDetection ���� ���� ���� // Set up AnomalyDetection model version to learn
		AnomalyDetection.SetModelVersion(CAnomalyDetectionDL::EModelVersion_FLDefNet_V1_64);
		// �н� epoch ���� ���� // Set the learn epoch value 
		AnomalyDetection.SetLearningEpoch(5000);
		// �н� �̹��� Interpolation ��� ���� // Set Interpolation method of learn image
		AnomalyDetection.SetInterpolationMethod(EInterpolationMethod_Bilinear);

		// Optimizer�� �н��� ���� // Set learning rate of Optimizer
		optSpec.SetLearningRate(1e-3f);

		// ������ Optimizer�� AnomalyDetection�� ���� // Apply the Optimizer that we set up to AnomalyDetection
		AnomalyDetection.SetLearningOptimizerSpec(optSpec);

		// AugmentationSpec ���� // Set the AugmentationSpec
		CAugmentationSpec augSpec;

		augSpec.EnableAugmentation(true);
		augSpec.SetCommonActivationRatio(0.500000);
		augSpec.SetCommonIoUThreshold(0.000000);
		augSpec.SetCommonInterpolationMethod(EInterpolationMethod_Bilinear);

		augSpec.EnableRotation(true);
		augSpec.SetRotationParam(1.000000, false, false);

		augSpec.EnableScaleCrop(true);
		augSpec.SetScaleCropParam(1.000000, 1.000000, false, 0.900000, 1.100000, 0.900000, 1.100000, true);

		AnomalyDetection.SetLearningAugmentationSpec(&augSpec);

		// Learn ������ �ϴ� �ڵ� ��ü ���� // Declare HANDLE object execute learn function
		HANDLE hThread;

		// AnomalyDetection learn function�� �����ϴ� ������ ���� // Create the AnomalyDetection Learn function thread
		hThread = (HANDLE)_beginthreadex(NULL, 0, LearnThread, (void*)&AnomalyDetection, 0, nullptr);

		while(!AnomalyDetection.IsRunning() && !g_bTerminated)
			CThreadUtilities::Sleep(1);

		int32_t i32MaxEpoch = AnomalyDetection.GetLearningEpoch();
		int32_t i32PrevEpoch = 0;
		int32_t i32PrevCostCount = 0;
		int32_t i32PrevValidationCount = 0;

		while(true)
		{
			CThreadUtilities::Sleep(1);

			// ������ �̴� ��ġ �ִ� �ݺ� Ƚ�� �ޱ� // Get the last maximum number of iterations of the last mini batch 
			int32_t i32MaxIteration = AnomalyDetection.GetActualMiniBatchCount();
			// ������ �̴� ��ġ �ݺ� Ƚ�� �ޱ� // Get the last number of mini batch iterations
			int32_t i32Iteration = AnomalyDetection.GetLearningResultCurrentIteration();
			// ������ �н� Ƚ�� �ޱ� // Get the last epoch learning
			int32_t i32Epoch = AnomalyDetection.GetLastEpoch();

			// �̴� ��ġ �ݺ��� �Ϸ�Ǹ� cost�� validation ���� ���÷��� 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(i32Epoch != i32PrevEpoch && i32Iteration == i32MaxIteration && i32Epoch > 0)
			{
				// ������ �н� ��� ��� �ޱ� // Get the last cost of the learning result
				float f32CurrCost = AnomalyDetection.GetLearningResultLastCost();
				// ������ ���� ��� �ޱ� // Get the last validation result
				float f32ValidationPa = AnomalyDetection.GetLearningResultLastAccuracy();

				// �ش� epoch�� ���� ���� ��� �� ��� // Print cost and validation value for the relevant epoch
				printf("Cost : %.6f Validation : %.6f Epoch %d / %d\n", f32CurrCost, f32ValidationPa, i32Epoch, i32MaxEpoch);

				// �н� ��� ���� ���� ��� ����� �޾� �׷��� �信 ���  
				// Get the history of cost and validation and print it at graph view
				CFLArray<float> flaCostHistory;
				CFLArray<float> flaValidationHistory;
				CFLArray<int32_t> vctValidationEpoch;

				AnomalyDetection.GetLearningResultAllHistory(&flaCostHistory, &flaValidationHistory,  &vctValidationEpoch);

				// ��� ����̳� ���� ��� ����� �ִٸ� ��� // Print results if cost or validation history exists
				if((flaCostHistory.GetCount() && i32PrevCostCount != (int32_t)flaCostHistory.GetCount()) || (flaValidationHistory.GetCount() && i32PrevValidationCount != (int32_t)flaValidationHistory.GetCount()))
				{
					int32_t i32Step = AnomalyDetection.GetLearningValidationStep();
					CFLArray<float> flaX;

					for(int64_t i = 0; i < flaValidationHistory.GetCount() - 1; ++i)
						flaX.PushBack((float)(i * i32Step));

					flaX.PushBack((float)(flaCostHistory.GetCount() - 1));

					// ���� �׷����� �����͸� ���� // Clear previous grpah data
					viewGraph.LockUpdate();
					viewGraph.Clear();
					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(flaCostHistory, EChartType_Line, RED, L"Cost");

					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(flaX, flaValidationHistory, EChartType_Line, BLUE, L"PixelAccuracy");
					viewGraph.UnlockUpdate();

					viewGraph.UpdateWindow();
					viewGraph.Invalidate();
					viewGraph.RedrawWindow();
				}

				// ���� ����� 1.0�� ��� �н��� �ߴ��ϰ� �з� ���� 
				// If the validation result is 1.0, stop learning and classify images 
				if(f32ValidationPa == 1.f || GetAsyncKeyState(VK_ESCAPE))
					AnomalyDetection.Stop();

				i32PrevEpoch = i32Epoch;
				i32PrevCostCount = (int32_t)flaCostHistory.GetCount();
				i32PrevValidationCount = (int32_t)flaValidationHistory.GetCount();
			}
			// epoch��ŭ �н��� �Ϸ�Ǹ� ���� // End when learning progresses as much as epoch
			if(!AnomalyDetection.IsRunning() && g_bTerminated)
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
		AnomalyDetection.SetInferenceImage(fliValidationImage);
		// �߷� ��� �̹��� ���� // Set the inference result Image
		AnomalyDetection.SetInferenceResultImage(fliResultLabelFigureImage);
		// ������ ��� �� Threshold ���� // Set Anomaly Threshold
		AnomalyDetection.SetInferenceAnomalyThreshold(0.5);
		// ������ �ּ� ũ�� ���� // Set Minimum Anomaly Area
		AnomalyDetection.SetInferenceMinimumAnomalyArea(4);
		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = AnomalyDetection.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesLabelFigure.RedrawWindow();

		// �׷��� �並 ���� // Update the Graph view.
		viewGraph.RedrawWindow();

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageValidation.IsAvailable() && viewImagesLabelFigure.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}