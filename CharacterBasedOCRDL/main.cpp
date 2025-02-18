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
	CCharacterBasedOCRDL* pOCR = (CCharacterBasedOCRDL*)pParam;

	CResult er = pOCR->Learn();
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
		if(IsFail(res = fliLearnImage.Load(L"../../ExampleImages/CharacterBasedOCRDL/OCR_Learn.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliValidationImage.Load(L"../../ExampleImages/CharacterBasedOCRDL/OCR_Inference.flif")))
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
		if(IsFail(res = viewGraph.Create(600, 500, 1100, 1000)))
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

		viewImagesLabel.EnablePixelSegmentationMode(true);

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

		if(IsFail(res = layerResultLabel.DrawTextCanvas(&CFLPoint<double>(0, 0), L"RESULT LABEL", GREEN, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImagesLabel.RedrawWindow();

		// OCR ��ü ���� // Create OCR object
		CCharacterBasedOCRDL ocr;

		// OptimizerSpec ��ü ���� // Create OptimizerSpec object
		COptimizerSpecAdamGradientDescent optSpec;

		// �н��� �̹��� ���� // Set the image to learn
		ocr.SetLearningImage(fliLearnImage);
		// ������ �̹��� ���� // Set the image to validate
		ocr.SetLearningValidationImage(fliValidationImage);
		// �н��� OCR �� ���� // Set up OCR model to learn
		ocr.SetModel(CCharacterBasedOCRDL::EModel_R_FLSegNet);
		// �н��� OCR ���� ���� ���� // Set up OCR model version to learn
		ocr.SetModelVersion(CCharacterBasedOCRDL::EModelVersion_R_FLSegNet_V1_512);
		// �н� epoch ���� ���� // Set the learn epoch value 
		ocr.SetLearningEpoch(10000);
		// �н� �̹��� Interpolation ��� ���� // Set Interpolation method of learn image
		ocr.SetInterpolationMethod(EInterpolationMethod_Bilinear);

		// Optimizer�� �н��� ���� // Set learning rate of Optimizer
		optSpec.SetLearningRate(1e-4f);
		
		// ������ Optimizer�� OCR�� ���� // Apply the Optimizer that we set up to OCR
		ocr.SetLearningOptimizerSpec(optSpec);

		// AugmentationSpec ���� // Set the AugmentationSpec
		CAugmentationSpec augSpec;

		augSpec.EnableAugmentation(true);
		augSpec.SetCommonActivationRatio(0.5);
		augSpec.SetCommonInterpolationMethod(FLImaging::ImageProcessing::EInterpolationMethod_Bilinear);
		augSpec.EnableRotation(true);
		augSpec.SetRotationParam(-45, 45., false);

		augSpec.EnableGaussianNoise(true);
		augSpec.SetGaussianNoiseParam(0., 0., 0., .02);

		augSpec.EnableScale(true);
		augSpec.SetScaleParam(.95, 1.05, .95, 1.05, true);

		ocr.SetLearningAugmentationSpec(&augSpec);

		// �н��� ������ ���ǽ� ����. map���� 0.9 �̻��� ��� �н� �����Ѵ�. metric�� ������ ���Դϴ�.
		// Set Conditional Expression to End Learning. If the map value is 0.9 or higher, end the learning. Same value as metric.
		ocr.SetLearningStopCondition(L"map >= 0.9");

		// �ڵ� ���� �ɼ� ���� // Set Auto-Save Options
		CAutoSaveSpec autoSaveSpec;

		// �ڵ� ���� Ȱ��ȭ // Enable Auto-Save
		autoSaveSpec.EnableAutoSave(true);
		// ������ �� ��� ���� // Set Model path to save
		autoSaveSpec.SetAutoSavePath(L"model.flocrdl");
		// �ڵ� ���� ���ǽ� ����. ���� map���� �ִ� ���� ��� ���� Ȱ��ȭ
		// Set auto-save conditional expressions. Enable save if the current map value is the maximum value
		autoSaveSpec.SetAutoSaveCondition(L"map > max('map')");

		// �ڵ� ���� �ɼ� ���� // Set Auto-Save Options
		ocr.SetLearningAutoSaveSpec(autoSaveSpec);

		// Learn ������ �ϴ� �ڵ� ��ü ���� // Declare HANDLE object execute learn function
		HANDLE hThread;

		// OCR learn function�� �����ϴ� ������ ���� // Create the OCR Learn function thread
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

			// ������ �̴� ��ġ �ִ� �ݺ� Ƚ�� �ޱ� // Get the last maximum number of iterations of the last mini batch 
			int32_t i32MaxIteration = ocr.GetActualMiniBatchCount();
			// ������ �̴� ��ġ �ݺ� Ƚ�� �ޱ� // Get the last number of mini batch iterations
			int32_t i32Iteration = ocr.GetLearningResultCurrentIteration();
			// ������ �н� Ƚ�� �ޱ� // Get the last epoch learning
			int32_t i32Epoch = ocr.GetLastEpoch();

			// �̴� ��ġ �ݺ��� �Ϸ�Ǹ� cost�� validation ���� ���÷��� 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(i32Epoch != i32PrevEpoch && i32Iteration == i32MaxIteration && i32Epoch > 0)
			{
				// ������ �н� ��� ��� �ޱ� // Get the last cost of the learning result
				float f32CurrCost = ocr.GetLearningResultLastCost();
				// ������ ���� ��� �ޱ� // Get the last validation result
				float f32ValidationMeanAP = ocr.GetLearningResultLastMeanAP();

				// �ش� epoch�� ���� ���� ��� �� ��� // Print cost and validation value for the relevant epoch
				printf("Cost : %.6f mAP : %.6f Epoch %d / %d\n", f32CurrCost, f32ValidationMeanAP, i32Epoch, i32MaxEpoch);

				// �н� ��� ���� ���� ��� ����� �޾� �׷��� �信 ���  
				// Get the history of cost and validation and print it at graph view
				CFLArray<float> vctCosts;
				CFLArray<float> vctMeanAP;
				CFLArray<int32_t> vctValidationEpoch;

				ocr.GetLearningResultAllHistory(&vctCosts, &vctMeanAP, &vctValidationEpoch);

				// ��� ����̳� ���� ��� ����� �ִٸ� ��� // Print results if cost or validation history exists
				if((vctCosts.GetCount() && i32PrevCostCount != (int32_t)vctCosts.GetCount()) || (vctMeanAP.GetCount() && i32PrevValidationCount != (int32_t)vctMeanAP.GetCount()))
				{
					int32_t i32Step = ocr.GetLearningValidationStep();
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

				// ���� ����� 0.9 �̻� �� ��� �н��� �ߴ��ϰ� �з� ���� 
				// If the validation result is greater than 0.9, stop learning and classify images 
				if(f32ValidationMeanAP >= .9f || GetAsyncKeyState(VK_ESCAPE))
					ocr.Stop();

				i32PrevEpoch = i32Epoch;
				i32PrevCostCount = (int32_t)vctCosts.GetCount();
				i32PrevValidationCount = (int32_t)vctMeanAP.GetCount();
			}
			// epoch��ŭ �н��� �Ϸ�Ǹ� ���� // End when learning progresses as much as epoch
			if(!ocr.IsRunning() && g_bTerminated)
			{
				// learn ���� �����尡 ������ ����� ���� ��� // Wait until learning is completely terminated
				WaitForSingleObject(hThread, INFINITE);
				// ������ �ڵ� ���� // Close thread handle
				CloseHandle(hThread);
				break;
			}
		}

		// �з��� �̹��� ���� // Set the image to classify
		ocr.SetInferenceImage(fliValidationImage);
		// Result Label Image�� �ǰܸ� ������ Execute
		// �߷� ��� �̹��� ���� // Set the inference result Image
		ocr.SetInferenceResultImage(fliResultLabelImage);
		// �߷� ��� �ɼ� ���� // Set the inference result options;
		// Result item settings enum ���� // Set the result item settings
		ocr.SetInferenceResultItemSettings(CCharacterBasedOCRDL::EInferenceResultItemSettings_ClassName_Contour);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = ocr.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}

		int64_t i64LearningClassCount = ocr.GetLearningResultClassCount();
		// ResultContours �ε����� ��Ī �Ǵ� �� ��ȣ�迭�� �������� // ResultContours Get an array of label numbers matching the index.
		for(int64_t classNum = 1; classNum < i64LearningClassCount; ++classNum)
		{
			Base::CFLArray<Base::CFLStringW> flaNames;

			ocr.GetLearningResultClassNames(classNum, &flaNames);
			viewImagesLabel.SetSegmentationLabelText(0, (double)classNum, flaNames[0].GetString());
		}

		// ResultLabel �信 Floating Value Range�� ���� // Set Floating Value Range in ResultLabel view
		viewImagesLabel.SetFloatingImageValueRange(0.f, (float)ocr.GetLearningResultClassCount());
		viewImagesLabel.ZoomFit();

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