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
	CSuperResolutionDL* pSuperResolution = (CSuperResolutionDL*)pParam;

	CResult er = pSuperResolution->Learn();
	g_bTerminated = true;

	return unsigned int(er);
};

int main()
{
				// �̹��� ��ü ���� // Declare the image object
	CFLImage fliLearnImageLowResolution;
	CFLImage fliLearnImageHighResolution;
	CFLImage fliValidationImageLowResolution;
	CFLImage fliValidationImageHighResolution;
	CFLImage fliResultLabelFigureImage;

	/// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearnLowResolution;
	CGUIViewImageWrap viewImageLearnHighResolution;
	CGUIViewImageWrap viewImageValidationLowResolution;
	CGUIViewImageWrap viewImageValidationHighResolution;
	CGUIViewImageWrap viewImagesLabelFigure;

	// �׷��� �� ���� // Declare the graph view
	CGUIViewGraphWrap viewGraph;

	CResult res = EResult_UnknownError;

	do
	{
		// ���̺귯���� ������ �ε� �� ������ ��ٸ� // Wait for the library to fully load
		CThreadUtilities::Sleep(1000);

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliLearnImageLowResolution.Load(L"../../ExampleImages/SuperResolution/SuperResolutionTrainDataLowResolution.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliLearnImageHighResolution.Load(L"../../ExampleImages/SuperResolution/SuperResolutionTrainDataHighResolution.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliValidationImageLowResolution.Load(L"../../ExampleImages/SuperResolution/SuperResolutionValidationDataLowResolution.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliValidationImageHighResolution.Load(L"../../ExampleImages/SuperResolution/SuperResolutionValidationDataHighResolution.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates image view
		if(IsFail(res = viewImageLearnLowResolution.Create(100, 0, 600, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �� ���� // Creates image view
		if(IsFail(res = viewImageLearnHighResolution.Create(600, 0, 1100, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageValidationLowResolution.Create(100, 500, 600, 1000)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageValidationHighResolution.Create(600, 500, 1100, 1000)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImagesLabelFigure.Create(1100, 0, 1600, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewGraph.Create(1100, 500, 1600, 1000)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		viewGraph.SetDarkMode();

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(res = viewImageLearnLowResolution.SetImagePtr(&fliLearnImageLowResolution)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageValidationLowResolution.SetImagePtr(&fliValidationImageLowResolution)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageLearnHighResolution.SetImagePtr(&fliLearnImageHighResolution)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageValidationHighResolution.SetImagePtr(&fliValidationImageHighResolution)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImagesLabelFigure.SetImagePtr(&fliResultLabelFigureImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// ����� ��ȭ�� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the Inference Result image view and High Resolution Image view

		if(IsFail(res = viewImageLearnLowResolution.SynchronizeWindow(&viewImageLearnHighResolution)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageLearnLowResolution.SynchronizeWindow(&viewImageValidationLowResolution)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageLearnLowResolution.SynchronizeWindow(&viewImageValidationHighResolution)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageLearnLowResolution.SynchronizeWindow(&viewImagesLabelFigure)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageLearnLowResolution.SynchronizeWindow(&viewGraph)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageLearnLowResolution.SynchronizePageIndex(&viewImageLearnHighResolution)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageValidationLowResolution.SynchronizePageIndex(&viewImageValidationHighResolution)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageValidationHighResolution.SynchronizePageIndex(&viewImagesLabelFigure)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageValidationHighResolution.SynchronizePointOfView(&viewImagesLabelFigure)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearnLowResolution = viewImageLearnLowResolution.GetLayer(0);
		CGUIViewImageLayerWrap layerLearnHighResolution = viewImageLearnHighResolution.GetLayer(0);
		CGUIViewImageLayerWrap layerInferenceLowResolution = viewImageValidationLowResolution.GetLayer(0);
		CGUIViewImageLayerWrap layerInferenceHighResolution = viewImageValidationHighResolution.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabelFigure = viewImagesLabelFigure.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerLearnLowResolution.Clear();
		layerLearnHighResolution.Clear();
		layerInferenceLowResolution.Clear();
		layerInferenceHighResolution.Clear();
		layerResultLabelFigure.Clear();

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = layerLearnLowResolution.DrawTextCanvas(&CFLPoint<double>(0, 0), L"LEARN LOW RESOLUTION", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerInferenceLowResolution.DrawTextCanvas(&CFLPoint<double>(0, 0), L"INFERENCE LOW RESOLUTION", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerLearnHighResolution.DrawTextCanvas(&CFLPoint<double>(0, 0), L"LEARN HIGH RESOLUTION", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerInferenceHighResolution.DrawTextCanvas(&CFLPoint<double>(0, 0), L"INFERENCE HIGH RESOLUTION", YELLOW, BLACK, 30)))
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
		viewImageLearnLowResolution.RedrawWindow();
		viewImageLearnHighResolution.RedrawWindow();
		viewImageValidationLowResolution.RedrawWindow();
		viewImageValidationHighResolution.RedrawWindow();
		viewImagesLabelFigure.RedrawWindow();

		// SuperResolution ��ü ���� // Create SuperResolution object
		CSuperResolutionDL SuperResolution;

		// OptimizerSpec ��ü ���� // Create OptimizerSpec object
		COptimizerSpecAdamGradientDescent optSpec;

		// �н��� �̹��� ���� // Set the image to learn
		SuperResolution.SetLearningLowResolutionImage(fliLearnImageLowResolution);
		SuperResolution.SetLearningHighResolutionImage(fliLearnImageHighResolution);
		// �н��� �̹��� ���� // Set the image to learn
		SuperResolution.SetLearningLowResolutionValidationImage(fliValidationImageLowResolution);
		SuperResolution.SetLearningHighResolutionValidationImage(fliValidationImageHighResolution);
		// �н��� SuperResolution �� ���� // Set up SuperResolution model to learn
		SuperResolution.SetModel(CSuperResolutionDL::EModel_SRCNN);
		// �н��� SuperResolution ���� ���� ���� // Set up SuperResolution model version to learn
		SuperResolution.SetModelVersion(CSuperResolutionDL::EModelVersion_SRCNN_V1_128);
		// �н� epoch ���� ���� // Set the learn epoch value 
		SuperResolution.SetLearningEpoch(1000);
		// �н� �̹��� Interpolation ��� ���� // Set Interpolation method of learn image
		SuperResolution.SetInterpolationMethod(EInterpolationMethod_Bilinear);
		// �̹��� ���� ���� // Set Scale Ratio
		SuperResolution.SetScaleRatio(2);

		// Optimizer�� �н��� ���� // Set learning rate of Optimizer
		optSpec.SetLearningRate(1e-3f);

		// ������ Optimizer�� SuperResolution�� ���� // Apply the Optimizer that we set up to SuperResolution
		SuperResolution.SetLearningOptimizerSpec(optSpec);

		// AugmentationSpec ���� // Set the AugmentationSpec
		CAugmentationSpec augSpec;

		augSpec.EnableAugmentation(true);
		augSpec.SetCommonActivationRatio(0.700000);
		augSpec.SetCommonIoUThreshold(0.000000);
		augSpec.SetCommonInterpolationMethod(EInterpolationMethod_Bilinear);
		augSpec.EnableTranslation(true);
		augSpec.SetTranslationParam(0.1,0.1);
		augSpec.EnableHorizontalFlip(true);
		augSpec.EnableVerticalFlip(true);

		SuperResolution.SetLearningAugmentationSpec(&augSpec);

		// �н��� ������ ���ǽ� ����. cost�� 0.1 �����̰� accuracy���� 0.9 �̻��� ��� �н� �����Ѵ�.
		// Set Conditional Expression to End Learning. If cost is 0.1 or less and the accumulation value is 0.9 or more, end learning.
		SuperResolution.SetLearningStopCondition(L"cost <= 0.1 & accuracy >= 0.98");

		// �ڵ� ���� �ɼ� ���� // Set Auto-Save Options
		CAutoSaveSpec autoSaveSpec;

		// �ڵ� ���� Ȱ��ȭ // Enable Auto-Save
		autoSaveSpec.EnableAutoSave(true);
		// ������ �� ��� ���� // Set Model path to save
		autoSaveSpec.SetAutoSavePath(L"model.flsr");
		// �ڵ� ���� ���ǽ� ����. ���� cost���� �ּ��̰� accuracy���� �ִ� ���� ��� ���� Ȱ��ȭ
		// Set auto-save conditional expressions. Enable save if the current cost value is minimum and the accumulation value is maximum
		autoSaveSpec.SetAutoSaveCondition(L"cost < min('cost') & accuracy > max('accuracy')");

		// �ڵ� ���� �ɼ� ���� // Set Auto-Save Options
		SuperResolution.SetLearningAutoSaveSpec(autoSaveSpec);

		// Learn ������ �ϴ� �ڵ� ��ü ���� // Declare HANDLE object execute learn function
		HANDLE hThread;

		// SuperResolution learn function�� �����ϴ� ������ ���� // Create the SuperResolution Learn function thread
		hThread = (HANDLE)_beginthreadex(NULL, 0, LearnThread, (void*)&SuperResolution, 0, nullptr);

		while(!SuperResolution.IsRunning() && !g_bTerminated)
			CThreadUtilities::Sleep(1);

		int32_t i32MaxEpoch = SuperResolution.GetLearningEpoch();
		int32_t i32PrevEpoch = 0;
		int32_t i32PrevCostCount = 0;
		int32_t i32PrevPSNRCount = 0;
		int32_t i32PrevSSIMCount = 0;
		int32_t i32PrevValidationCount = 0;

		while(true)
		{
			CThreadUtilities::Sleep(1);

			// ������ �̴� ��ġ �ִ� �ݺ� Ƚ�� �ޱ� // Get the last maximum number of iterations of the last mini batch 
			int32_t i32MaxIteration = SuperResolution.GetActualMiniBatchCount();
			// ������ �̴� ��ġ �ݺ� Ƚ�� �ޱ� // Get the last number of mini batch iterations
			int32_t i32Iteration = SuperResolution.GetLearningResultCurrentIteration();
			// ������ �н� Ƚ�� �ޱ� // Get the last epoch learning
			int32_t i32Epoch = SuperResolution.GetLastEpoch();

			// �̴� ��ġ �ݺ��� �Ϸ�Ǹ� cost�� validation ���� ���÷��� 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(i32Epoch != i32PrevEpoch && i32Iteration == i32MaxIteration && i32Epoch > 0)
			{
				// ������ �н� ��� ��� �ޱ� // Get the last cost of the learning result
				float f32CurrCost = SuperResolution.GetLearningResultLastCost();
				// ������ PSNR ��� �ޱ� // Get the last PSNR result
				float f32PSNRPa = SuperResolution.GetLearningResultLastPSNR();
				// ������ SSIM ��� �ޱ� // Get the last SSIM result
				float f32SSIMPa = SuperResolution.GetLearningResultLastSSIM();
				// ������ ���� ��� �ޱ� // Get the last validation result
				float f32ValidationPa = SuperResolution.GetLearningResultLastAccuracy();

				// �ش� epoch�� ���� ���� ��� �� ��� // Print cost and validation value for the relevant epoch
				printf("Cost : %.6f PSNR : %.6f SSIM : %.6f Accuracy : %.6f Epoch %d / %d\n", f32CurrCost, f32PSNRPa, f32SSIMPa, f32ValidationPa, i32Epoch, i32MaxEpoch);

				// �н� ��� ���� ���� ��� ����� �޾� �׷��� �信 ���  
				// Get the history of cost and validation and print it at graph view
				CFLArray<float> flaCostHistory;
				CFLArray<float> flaPSNRHistory;
				CFLArray<float> flaSSIMHistory;
				CFLArray<float> flaValidationHistory;
				CFLArray<int32_t> vctValidationEpoch;

				SuperResolution.GetLearningResultAllHistory(&flaCostHistory, &flaValidationHistory, &flaPSNRHistory, &flaSSIMHistory, &vctValidationEpoch);

				// ��� ����̳� ���� ��� ����� �ִٸ� ��� // Print results if cost or validation history exists
				if((flaCostHistory.GetCount() && i32PrevCostCount != (int32_t)flaCostHistory.GetCount()) || (flaPSNRHistory.GetCount() && i32PrevPSNRCount != (int32_t)flaPSNRHistory.GetCount()) || (flaSSIMHistory.GetCount() && i32PrevSSIMCount != (int32_t)flaSSIMHistory.GetCount()) || (flaValidationHistory.GetCount() && i32PrevValidationCount != (int32_t)flaValidationHistory.GetCount()))
				{
					int32_t i32Step = SuperResolution.GetLearningValidationStep();
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
					viewGraph.Plot(flaX, flaPSNRHistory, EChartType_Line, GREEN, L"PSNR");
					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(flaX, flaSSIMHistory, EChartType_Line, YELLOW, L"SSIM");
					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(flaX, flaValidationHistory, EChartType_Line, BLUE, L"Accuracy");
					viewGraph.UnlockUpdate();

					viewGraph.UpdateWindow();
					viewGraph.Invalidate();
					viewGraph.RedrawWindow();
				}

				// ���� ����� 1.0�� ��� �н��� �ߴ��ϰ� �з� ���� 
				// If the validation result is 1.0, stop learning and classify images 
				if(f32ValidationPa == 1.f || GetAsyncKeyState(VK_ESCAPE))
					SuperResolution.Stop();

				i32PrevEpoch = i32Epoch;
				i32PrevCostCount = (int32_t)flaCostHistory.GetCount();
				i32PrevPSNRCount = (int32_t)flaPSNRHistory.GetCount();
				i32PrevSSIMCount = (int32_t)flaSSIMHistory.GetCount();
				i32PrevValidationCount = (int32_t)flaValidationHistory.GetCount();
			}
			// epoch��ŭ �н��� �Ϸ�Ǹ� ���� // End when learning progresses as much as epoch
			if(!SuperResolution.IsRunning() && g_bTerminated)
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
		SuperResolution.SetInferenceImage(fliValidationImageLowResolution);
		// �߷� ��� �̹��� ���� // Set the inference result Image
		SuperResolution.SetInferenceResultImage(fliResultLabelFigureImage);
		// �߷� ��� �ɼ� ���� // Set the inference result options;
		if(IsFail(res = SuperResolution.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}

		// �̹��� �並 ���� // Update the image view.
		viewImageLearnLowResolution.RedrawWindow();
		viewImageValidationLowResolution.RedrawWindow();
		viewImagesLabelFigure.RedrawWindow();

		// �׷��� �並 ���� // Update the Graph view.
		viewGraph.RedrawWindow();

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearnLowResolution.IsAvailable() && viewImageValidationLowResolution.IsAvailable() && viewImageLearnHighResolution.IsAvailable() && viewImageValidationHighResolution.IsAvailable() && viewImagesLabelFigure.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}