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
				// �̹��� ��ü ���� // Declare the image object
	CFLImage fliLearnImage;
	CFLImage fliValidationImage;
	CFLImage fliResultLabelImage;
	CFLImage fliResultLabelFigureImage;

	/// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageValidation;
	CGUIViewImageWrap viewImagesLabel;
	CGUIViewImageWrap viewImagesLabelFigure;

	// �׷��� �� ���� // Declare the graph view
	CGUIViewGraphWrap viewGraph;

	CResult res = EResult_UnknownError;

	do
	{
		// ���̺귯���� ������ �ε� �� ������ ��ٸ� // Wait for the library to fully load
		CThreadUtilities::Sleep(1000);

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliLearnImage.Load(L"../../ExampleImages/SemanticSegmentation/Cityscape.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliValidationImage.Load(L"../../ExampleImages/SemanticSegmentation/Cityscape.flif")))
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

		if(IsFail(res = viewImagesLabelFigure.Create(600, 500, 1100, 1000)))
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

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerValidation = viewImageValidation.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabel = viewImagesLabel.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabelFigure = viewImagesLabelFigure.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerLearn.Clear();
		layerValidation.Clear();
		layerResultLabel.Clear();
		layerResultLabelFigure.Clear();

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> 
		//                 ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� -> ����� -> ��Ʈ �̸� -> 
		//                 ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> 
		//                  Area color -> font size -> actual size -> angle -> Align -> Font Name -> 
		//                  Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> 
		//                  Font Thickness -> Font Italic
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

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesLabel.RedrawWindow();
		viewImagesLabelFigure.RedrawWindow();

		// SemanticSegmentation ��ü ���� // Create SemanticSegmentation object
		CSemanticSegmentationDL semanticSegmentation;

		// OptimizerSpec ��ü ���� // Create OptimizerSpec object
		COptimizerSpecAdamGradientDescent optSpec;

		// �н��� �̹��� ���� // Set the image to learn
		semanticSegmentation.SetLearningImage(fliLearnImage);
		// ������ �̹��� ���� // Set the image to validate
		semanticSegmentation.SetLearningValidationImage(fliValidationImage);
		// �н��� SemanticSegmentation �� ���� // Set up SemanticSegmentation model to learn
		semanticSegmentation.SetModel(CSemanticSegmentationDL::EModel_FLSegNet);
		// �н��� SemanticSegmentation ���� ���� ���� // Set up SemanticSegmentation model version to learn
		semanticSegmentation.SetModelVersion(CSemanticSegmentationDL::EModelVersion_FLSegNet_V1_256_B1);
		// �н� epoch ���� ���� // Set the learn epoch value 
		semanticSegmentation.SetLearningEpoch(1000);
		// �н� �̹��� Interpolation ��� ���� // Set Interpolation method of learn image
		semanticSegmentation.SetInterpolationMethod(EInterpolationMethod_Bilinear);

		// Optimizer�� �н��� ���� // Set learning rate of Optimizer
		optSpec.SetLearningRate(1e-3f);

		// ������ Optimizer�� SemanticSegmentation�� ���� // Apply the Optimizer that we set up to SemanticSegmentation
		semanticSegmentation.SetLearningOptimizerSpec(optSpec);

		// �н� ���� ���� ����.
		// Metric ������ mIoU ������, ���� Ŭ���� ������ �� Ŭ������ ������ ���� ����ġ ������ �Ͽ� ��Ȯ���� ����
		// Setting the End-of-Learning Condition
		// Metric condition is the mIoU value, and the accuracy is set by performing a weight operation on the actual number of classes and the score of each class
		semanticSegmentation.SetLearningStopCondition(L"accuracy >= 0.99 | mIoU >= 0.75");
	
		// �н� �̹��� ���� ��� ����.
		// ���簢�� ������ �����Ͽ� ���� �̹����� ������ ó���Ѵ�.
		// Set the training image segmentation mode.
		// The original image is divided and processed by maintaining the square ratio.
		semanticSegmentation.SetImageTilingMode(ETilingMode_SingleAxisTiling_Fit);

		// �н��� ������ ���ǽ� ����. miou.ze���� 0.9 �̻��� ��� �н� �����Ѵ�. metric.ze�� ������ ���Դϴ�.
		// Set Conditional Expression to End Learning. If the miou.ze value is 0.9 or higher, end the learning. Same value as metric.ze.
		semanticSegmentation.SetLearningStopCondition(L"miou.ze >= 0.9");

		// �ڵ� ���� �ɼ� ���� // Set Auto-Save Options
		CAutoSaveSpec autoSaveSpec;

		// �ڵ� ���� Ȱ��ȭ // Enable Auto-Save
		autoSaveSpec.EnableAutoSave(true);
		// ������ �� ��� ���� // Set Model path to save
		autoSaveSpec.SetAutoSavePath(L"model.flss");
		// �ڵ� ���� ���ǽ� ����. ���� miou.ze���� �ִ� ���� ��� ���� Ȱ��ȭ
		// Set auto-save conditional expressions. Enable save if the current miou.ze value is the maximum value
		autoSaveSpec.SetAutoSaveCondition(L"miou.ze > max('miou.ze')");

		// �ڵ� ���� �ɼ� ���� // Set Auto-Save Options
		semanticSegmentation.SetLearningAutoSaveSpec(autoSaveSpec);

		// Learn ������ �ϴ� �ڵ� ��ü ���� // Declare HANDLE object execute learn function
		HANDLE hThread;

		// SemanticSegmentation learn function�� �����ϴ� ������ ���� // Create the SemanticSegmentation Learn function thread
		hThread = (HANDLE)_beginthreadex(NULL, 0, LearnThread, (void*)&semanticSegmentation, 0, nullptr);

		while(!semanticSegmentation.IsRunning() && !g_bTerminated)
			CThreadUtilities::Sleep(1);

		int32_t i32MaxEpoch = semanticSegmentation.GetLearningEpoch();
		int32_t i32PrevEpoch = 0;
		int32_t i32PrevCostCount = 0;
		int32_t i32PrevValidationCount = 0;

		while(true)
		{
			CThreadUtilities::Sleep(1);

			// ������ �̴� ��ġ �ִ� �ݺ� Ƚ�� �ޱ� // Get the last maximum number of iterations of the last mini batch 
			int32_t i32MaxIteration = semanticSegmentation.GetActualMiniBatchCount();
			// ������ �̴� ��ġ �ݺ� Ƚ�� �ޱ� // Get the last number of mini batch iterations
			int32_t i32Iteration = semanticSegmentation.GetLearningResultCurrentIteration();
			// ������ �н� Ƚ�� �ޱ� // Get the last epoch learning
			int32_t i32Epoch = semanticSegmentation.GetLastEpoch();

			// �̴� ��ġ �ݺ��� �Ϸ�Ǹ� cost�� validation ���� ���÷��� 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(i32Epoch != i32PrevEpoch && i32Iteration == i32MaxIteration && i32Epoch > 0)
			{
				// ������ �н� ��� ��� �ޱ� // Get the last cost of the learning result
				float f32CurrCost = semanticSegmentation.GetLearningResultLastCost();
				// ������ ���� ��� �ޱ� // Get the last validation result
				float f32ValidationPa = semanticSegmentation.GetLearningResultLastAccuracy();
				float f32ValidationMeanIoU = semanticSegmentation.GetLearningResultLastMeanIoU();

				// �ش� epoch�� ���� ���� ��� �� ��� // Print cost and validation value for the relevant epoch
				printf("Cost : %.6f Validation : %.6f mIoU : %.6f Epoch %d / %d\n", f32CurrCost, f32ValidationPa, f32ValidationMeanIoU, i32Epoch, i32MaxEpoch);

				// �н� ��� ���� ���� ��� ����� �޾� �׷��� �信 ���  
				// Get the history of cost and validation and print it at graph view
				CFLArray<float> vctCosts;
				CFLArray<float> vctVadliationPixelAccuracy;
				CFLArray<float> vctMeanIoU;
				CFLArray<float> vctVadliationPixelAccuracyZE;
				CFLArray<float> vctMeanIoUZE;
				CFLArray<int32_t> vctValidationEpoch;

				semanticSegmentation.GetLearningResultAllHistory(&vctCosts, &vctVadliationPixelAccuracy, &vctMeanIoU, &vctVadliationPixelAccuracyZE, &vctMeanIoUZE, &vctValidationEpoch);

				// ��� ����̳� ���� ��� ����� �ִٸ� ��� // Print results if cost or validation history exists
				if((vctCosts.GetCount() && i32PrevCostCount != (int32_t)vctCosts.GetCount()) || (vctVadliationPixelAccuracy.GetCount() && i32PrevValidationCount != (int32_t)vctVadliationPixelAccuracy.GetCount()))
				{
					int32_t i32Step = semanticSegmentation.GetLearningValidationStep();
					CFLArray<float> flaX;

					for(int64_t i = 0; i < vctVadliationPixelAccuracy.GetCount() - 1; ++i)
						flaX.PushBack((float)(i * i32Step));

					flaX.PushBack((float)(vctCosts.GetCount() - 1));

					// ���� �׷����� �����͸� ���� // Clear previous grpah data
					viewGraph.LockUpdate();
					viewGraph.Clear();
					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(vctCosts, EChartType_Line, RED, L"Cost");

					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(flaX, vctVadliationPixelAccuracy, EChartType_Line, BLUE, L"PixelAccuracy(Zero Exception)");
					viewGraph.Plot(flaX, vctMeanIoU, EChartType_Line, PINK, L"mIoU");
					viewGraph.UnlockUpdate();

					viewGraph.UpdateWindow();
					viewGraph.Invalidate();
					viewGraph.RedrawWindow();
				}

				// ���� ����� 1.0�� ��� �н��� �ߴ��ϰ� �з� ���� 
				// If the validation result is 1.0, stop learning and classify images 
				if(f32ValidationPa == 1.f || GetAsyncKeyState(VK_ESCAPE))
					semanticSegmentation.Stop();

				i32PrevEpoch = i32Epoch;
				i32PrevCostCount = (int32_t)vctCosts.GetCount();
				i32PrevValidationCount = (int32_t)vctVadliationPixelAccuracy.GetCount();
			}
			// epoch��ŭ �н��� �Ϸ�Ǹ� ���� // End when learning progresses as much as epoch
			if(!semanticSegmentation.IsRunning() && g_bTerminated)
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
		semanticSegmentation.SetInferenceImage(fliValidationImage);
		// �߷� ��� �̹��� ���� // Set the inference result Image
		semanticSegmentation.SetInferenceResultImage(fliResultLabelImage);
		// �߷� ��� �ɼ� ���� // Set the inference result options;
		// Result ����� Label Image�� ������ ���� ���� // Set whether to receive the result as a Label Image
		semanticSegmentation.EnableInferenceResultLabelImage(true);
		// Result ����� Region Figure�� ���� ���� ���� // Set whether to include region figure in result
		semanticSegmentation.EnableInferenceResultIncludingRegionFigures(false);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = semanticSegmentation.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}

		// Result Label Image�� �ǰܸ� ������ Execute
		// �߷� ��� �̹��� ���� // Set the inference result Image
		semanticSegmentation.SetInferenceResultImage(fliResultLabelFigureImage);
		// �߷� ��� �ɼ� ���� // Set the inference result options;
		// Result ����� Label Image�� ������ ���� ���� // Set whether to receive the result as a Label Image
		semanticSegmentation.EnableInferenceResultLabelImage(false);
		// Result ����� Region Figure�� ���� ���� ���� // Set whether to include region figure in result
		semanticSegmentation.EnableInferenceResultIncludingRegionFigures(true);
		// Result item settings enum ���� // Set the result item settings
		semanticSegmentation.SetInferenceResultItemSettings(CSemanticSegmentationDL::EInferenceResultItemSettings_ClassNum_ClassName_ConfidenceScore_RegionType_Contour);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = semanticSegmentation.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}

		// ResultLabel �信 Floating Value Range�� ���� // Set Floating Value Range in ResultLabel view
		viewImagesLabel.SetFloatingImageValueRange(0.f, (float)semanticSegmentation.GetLearningResultClassCount());

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesLabel.RedrawWindow();
		viewImagesLabelFigure.RedrawWindow();

		// �׷��� �並 ���� // Update the Graph view.
		viewGraph.RedrawWindow();

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageValidation.IsAvailable() && viewImagesLabel.IsAvailable() && viewImagesLabelFigure.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}