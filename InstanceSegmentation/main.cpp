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
	CInstanceSegmentationDL* pInstanceSegmentation = (CInstanceSegmentationDL*)pParam;

	CResult er = pInstanceSegmentation->Learn();
	g_bTerminated = true;

	return unsigned int(er);
};

int main()
{
				// �̹��� ��ü ���� // Declare the image object
	CFLImage fliLearnImage;
	CFLImage fliValidationImage;
	CFLImage fliResultBoxContourImage;
	CFLImage fliResultContourImage;

	/// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageValidation;
	CGUIViewImageWrap viewImagesBoxContour;
	CGUIViewImageWrap viewImagesContour;

	// �׷��� �� ���� // Declare the graph view
	CGUIViewGraphWrap viewGraph;

	CResult res = EResult_UnknownError;

	do
	{
		// ���̺귯���� ������ �ε� �� ������ ��ٸ� // Wait for the library to fully load
		CThreadUtilities::Sleep(1000);

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliLearnImage.Load(L"../../ExampleImages/InstanceSegmentation/Bolt.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliValidationImage.Load(L"../../ExampleImages/InstanceSegmentation/Bolt.flif")))
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

		if(IsFail(res = viewImagesBoxContour.Create(100, 500, 600, 1000)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImagesContour.Create(600, 500, 1100, 1000)))
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

		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImagesBoxContour)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImagesContour)))
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

		if(IsFail(res = viewImagesBoxContour.SetImagePtr(&fliResultBoxContourImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImagesContour.SetImagePtr(&fliResultContourImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerValidation = viewImageValidation.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabel = viewImagesBoxContour.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabelFigure = viewImagesContour.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerLearn.Clear();
		layerValidation.Clear();
		layerResultLabel.Clear();
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

		if(IsFail(res = layerResultLabel.DrawTextCanvas(&CFLPoint<double>(0, 0), L"RESULT BOX CONTOUR", GREEN, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerResultLabelFigure.DrawTextCanvas(&CFLPoint<double>(0, 0), L"RESULT CONTOUR", GREEN, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesBoxContour.RedrawWindow();
		viewImagesContour.RedrawWindow();

		// InstanceSegmentation ��ü ���� // Create InstanceSegmentation object
		CInstanceSegmentationDL InstanceSegmentation;

		// OptimizerSpec ��ü ���� // Create OptimizerSpec object
		COptimizerSpecAdamGradientDescent optSpec;

		// �н��� �̹��� ���� // Set the image to learn
		InstanceSegmentation.SetLearningImage(fliLearnImage);
		// ������ �̹��� ���� // Set the image to validate
		InstanceSegmentation.SetLearningValidationImage(fliValidationImage);
		// �н��� InstanceSegmentation �� ���� // Set up InstanceSegmentation model to learn
		InstanceSegmentation.SetModel(CInstanceSegmentationDL::EModel_MaskRCNN);
		// �н��� InstanceSegmentation ���� ���� ���� // Set up InstanceSegmentation model version to learn
		InstanceSegmentation.SetModelVersion(CInstanceSegmentationDL::EModelVersion_MaskRCNN_V1_256);
		// �н� epoch ���� ���� // Set the learn epoch value 
		InstanceSegmentation.SetLearningEpoch(500);
		// �н� �̹��� Interpolation ��� ���� // Set Interpolation method of learn image
		InstanceSegmentation.SetInterpolationMethod(EInterpolationMethod_Bilinear);
		// �н� �ߴ� ���� ���� // Set the condtiion of stopping learning
		InstanceSegmentation.SetLearningStopCondition(L"cost <= 0 | validation >= 0.8");

		// Optimizer�� �н��� ���� // Set learning rate of Optimizer
		optSpec.SetLearningRate(1e-4f);

		// ������ Optimizer�� InstanceSegmentation�� ���� // Apply the Optimizer that we set up to InstanceSegmentation
		InstanceSegmentation.SetLearningOptimizerSpec(optSpec);

		// AugmentationSpec ���� // Set the AugmentationSpec
		CAugmentationSpec augSpec;

		augSpec.EnableAugmentation(true);
		augSpec.SetCommonActivationRatio(0.700000);
		augSpec.SetCommonIoUThreshold(0.800000);
		augSpec.SetCommonInterpolationMethod(EInterpolationMethod_Bilinear);
		augSpec.EnableHorizontalFlip(true);
		augSpec.EnableVerticalFlip(true);
		augSpec.EnableScaleCrop(true);
		augSpec.SetScaleCropParam(0.900000, 1.000000, true, 1.000000, 1.000000, 1.000000, 1.000000, true);

		InstanceSegmentation.SetLearningAugmentationSpec(&augSpec);
		
		// Learn ������ �ϴ� �ڵ� ��ü ���� // Declare HANDLE object execute learn function
		HANDLE hThread;
		// InstanceSegmentation learn function�� �����ϴ� ������ ���� // Create the InstanceSegmentation Learn function thread
		hThread = (HANDLE)_beginthreadex(NULL, 0, LearnThread, (void*)&InstanceSegmentation, 0, nullptr);

		while(!InstanceSegmentation.IsRunning() && !g_bTerminated)
			CThreadUtilities::Sleep(1);

		int32_t i32MaxEpoch = InstanceSegmentation.GetLearningEpoch();
		int32_t i32PrevEpoch = 0;
		int32_t i32PrevCostCount = 0;
		int32_t i32PrevValidationCount = 0;

		while(true)
		{
			CThreadUtilities::Sleep(1);

			// ������ �̴� ��ġ �ִ� �ݺ� Ƚ�� �ޱ� // Get the last maximum number of iterations of the last mini batch 
			int32_t i32MaxIteration = InstanceSegmentation.GetActualMiniBatchCount();
			// ������ �̴� ��ġ �ݺ� Ƚ�� �ޱ� // Get the last number of mini batch iterations
			int32_t i32Iteration = InstanceSegmentation.GetLearningResultCurrentIteration();
			// ������ �н� Ƚ�� �ޱ� // Get the last epoch learning
			int32_t i32Epoch = InstanceSegmentation.GetLastEpoch();

			// �̴� ��ġ �ݺ��� �Ϸ�Ǹ� cost�� validation ���� ���÷��� 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(i32Epoch != i32PrevEpoch && i32Iteration == i32MaxIteration && i32Epoch > 0)
			{
				// ������ �н� ��� ��� �ޱ� // Get the last cost of the learning result
				float f32CurrCost = InstanceSegmentation.GetLearningResultLastCost();
				// ������ ���� ��� �ޱ� // Get the last validation result
				float f32MeanAP = InstanceSegmentation.GetLearningResultLastMeanAP();

				// �ش� epoch�� ���� ���� ��� �� ��� // Print cost and validation value for the relevant epoch
				printf("Cost : %.6f mAP : %.6f Epoch %d / %d\n", f32CurrCost, f32MeanAP, i32Epoch, i32MaxEpoch);

				// �н� ��� ���� ���� ��� ����� �޾� �׷��� �信 ���  
				// Get the history of cost and validation and print it at graph view
				CFLArray<float> vctCosts;
				CFLArray<float> vctMeanAP;
				CFLArray<int32_t> vctValidationEpoch;

				InstanceSegmentation.GetLearningResultAllHistory(&vctCosts, &vctMeanAP, &vctValidationEpoch);

				// ��� ����̳� ���� ��� ����� �ִٸ� ��� // Print results if cost or validation history exists
				if((vctCosts.GetCount() && i32PrevCostCount != (int32_t)vctCosts.GetCount()) || (vctMeanAP.GetCount() && i32PrevValidationCount != (int32_t)vctMeanAP.GetCount()))
				{
					int32_t i32Step = InstanceSegmentation.GetLearningValidationStep();
					CFLArray<float> flaX;

					for(int64_t i = 0; i < vctMeanAP.GetCount() - 1; ++i)
						flaX.PushBack((float)(i * i32Step));

					flaX.PushBack((float)(vctCosts.GetCount() - 1));

					// ���� �׷����� �����͸� ���� // Clear previous grpah data
					viewGraph.LockUpdate();
					viewGraph.Clear();
					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(vctCosts, EChartType_Line, RED, L"Cost");

					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(flaX, vctMeanAP, EChartType_Line, BLUE, L"mAP");
					viewGraph.UnlockUpdate();

					viewGraph.UpdateWindow();
					viewGraph.Invalidate();
					viewGraph.RedrawWindow();
				}

				// ���� ����� 1.0�� ��� �н��� �ߴ��ϰ� �з� ���� 
				// If the validation result is 1.0, stop learning and classify images 
				if(f32MeanAP == 1.f || GetAsyncKeyState(VK_ESCAPE))
					InstanceSegmentation.Stop();

				i32PrevEpoch = i32Epoch;
				i32PrevCostCount = (int32_t)vctCosts.GetCount();
				i32PrevValidationCount = (int32_t)vctMeanAP.GetCount();
			}
			// epoch��ŭ �н��� �Ϸ�Ǹ� ���� // End when learning progresses as much as epoch
			if(!InstanceSegmentation.IsRunning() && g_bTerminated)
			{
				// learn ���� �����尡 ������ ����� ���� ��� // Wait until learning is completely terminated
				WaitForSingleObject(hThread, INFINITE);
				// ������ �ڵ� ���� // Close thread handle
				CloseHandle(hThread);
				break;
			}
		}

		// Result Image�� Box & Contour ��� ����ϴ� Execute // Execute to print both Box& Contour in Result Image
		// �з��� �̹��� ���� // Set the image to classify
		InstanceSegmentation.SetInferenceImage(fliValidationImage);
		// �߷� ��� �̹��� ���� // Set the inference result Image
		InstanceSegmentation.SetInferenceResultImage(fliResultBoxContourImage);
		// �߷� ��� �ɼ� ���� // Set the inference result options
		// Figure �ɼ� ���� // Set the option of figures
		CInstanceSegmentationDL::EInferenceResultItemSettings eFigureOption = (CInstanceSegmentationDL::EInferenceResultItemSettings)(CInstanceSegmentationDL::EInferenceResultItemSettings_ClassNum | CInstanceSegmentationDL::EInferenceResultItemSettings_ClassName | CInstanceSegmentationDL::EInferenceResultItemSettings_Objectness | CInstanceSegmentationDL::EInferenceResultItemSettings_BoundaryRect | CInstanceSegmentationDL::EInferenceResultItemSettings_Contour);
		InstanceSegmentation.SetInferenceResultItemSettings(eFigureOption);
		// Objectness Threshold ���� // Set the obectness threshold
		InstanceSegmentation.SetInferenceResultObjectnessThreshold(0.5f);
		// Mask Threshold ���� // Set The mask threshold
		InstanceSegmentation.SetInferenceResultMaskThreshold(0.5f);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = InstanceSegmentation.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}

		// Result Image�� Contour �� ����ϴ� Execute // Execute to print both Box& Contour in Result Image
		// �з��� �̹��� ���� // Set the image to classify
		InstanceSegmentation.SetInferenceImage(fliValidationImage);
		// �߷� ��� �̹��� ���� // Set the inference result Image
		InstanceSegmentation.SetInferenceResultImage(fliResultContourImage);
		// �߷� ��� �ɼ� ���� // Set the inference result options
		// Figure �ɼ� ���� // Set the option of figures
		eFigureOption = CInstanceSegmentationDL::EInferenceResultItemSettings_Contour;
		InstanceSegmentation.SetInferenceResultItemSettings(eFigureOption);
		// Objectness Threshold ���� // Set the obectness threshold
		InstanceSegmentation.SetInferenceResultObjectnessThreshold(0.5f);
		// Mask Threshold ���� // Set The mask threshold
		InstanceSegmentation.SetInferenceResultMaskThreshold(0.5f);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = InstanceSegmentation.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesBoxContour.RedrawWindow();
		viewImagesContour.RedrawWindow();

		// �׷��� �並 ���� // Update the Graph view.
		viewGraph.RedrawWindow();

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageValidation.IsAvailable() && viewImagesBoxContour.IsAvailable() && viewImagesContour.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}