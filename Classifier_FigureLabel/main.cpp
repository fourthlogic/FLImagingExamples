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
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliLearnImage;
	CFLImage fliSourceImage;
	CFLImage fliValidateImage;

	/// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageValidate;

	// �׷��� �� ���� // Declare the graph view
	CGUIViewGraphWrap viewGraph;
	CResult res = EResult_UnknownError;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(res = fliLearnImage.Load(L"../../ExampleImages/Classifier/CircleLabel_Learn.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliSourceImage.Load(L"../../ExampleImages/Classifier/CircleLabel_Validation.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliValidateImage.Load(L"../../ExampleImages/Classifier/CircleLabel_Validation.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates image view
		if(IsFail(res = viewImageLearn.Create(100, 0, 612, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageSource.Create(612, 0, 1124, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageValidate.Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Graph �� ���� // Create graph view
		if(IsFail(res = viewGraph.Create(100, 512, 612, 1024)))
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

		if(IsFail(res = viewImageValidate.SetImagePtr(&fliValidateImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageValidate.SynchronizePointOfView(&viewImageSource)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerValidate = viewImageValidate.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerLearn.Clear();
		layerSource.Clear();
		layerValidate.Clear();

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

		if(IsFail(res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"INFERENCE", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerValidate.DrawTextCanvas(&CFLPoint<double>(0, 0), L"VALIDATE", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// Classifier ��ü ���� // Create Classifier object
		CClassifierDL classifier;

		// OptimizerSpec ��ü ���� // Create OptimizerSpec object
		COptimizerSpecAdamGradientDescent optSpec;

		// �н��� �̹��� ���� // Set the image to learn
		classifier.SetLearningImage(fliLearnImage);
		// ������ �̹��� ���� // Set the image to validate
		classifier.SetLearningValidationImage(fliValidateImage);
		// �з��� �̹��� ���� // Set the image to classify
		classifier.SetInferenceImage(fliSourceImage);
		classifier.SetInferenceResultImage(fliSourceImage);

		// �н��� Classifier �� ���� // Set up Classifier model to learn
		classifier.SetModel(CClassifierDL::EModel_FL_CF_C);
		// �н��� Classifier �� ���� // Set up Classifier model to learn
		classifier.SetModelVersion(CClassifierDL::EModelVersion_FL_CF_C_V1_32);
		// �н� epoch ���� ���� // Set the learn epoch value 
		classifier.SetLearningEpoch(150);
		// �н� �̹��� Interpolation ��� ���� // Set Interpolation method of learn image
		classifier.SetInterpolationMethod(EInterpolationMethod_Bilinear);

		// Optimizer�� �н��� ���� // Set learning rate of Optimizer
		optSpec.SetLearningRate(1e-3f);

		// ������ Optimizer�� Classifier�� ���� // Apply the Optimizer that we set up to Classifier
		classifier.SetLearningOptimizerSpec(optSpec);

		// Learn ������ �ϴ� �ڵ� ��ü ���� // Declare HANDLE object execute learn function
		HANDLE hThread;

		// Classifier learn function�� �����ϴ� ������ ���� // Create the Classifier Learn function thread
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

			// ������ �̴� ��ġ �ִ� �ݺ� Ƚ�� �ޱ� // Get the last maximum number of iterations of the last mini batch 
			int32_t i32MaxIteration = classifier.GetActualMiniBatchCount();
			// ������ �̴� ��ġ �ݺ� Ƚ�� �ޱ� // Get the last number of mini batch iterations
			int32_t i32Iteration = classifier.GetLearningResultCurrentIteration();
			// ������ �н� Ƚ�� �ޱ� // Get the last epoch learning
			int32_t i32Epoch = classifier.GetLastEpoch();

			// �н� ��� ���� ���� ��� ����� �޾� �׷��� �信 ���  
			// Get the history of cost and validation and print it at graph view
			CFLArray<float> vctCosts;
			CFLArray<float> vctValidations;
			CFLArray<float> vctF1Score;
			CFLArray<int32_t> vctValidationEpoch;

			classifier.GetLearningResultAllHistory(vctCosts, vctValidations, vctF1Score, vctValidationEpoch);

			// �̴� ��ġ �ݺ��� �Ϸ�Ǹ� cost�� validation ���� ���÷��� 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(vctCosts.GetCount() && i32Epoch != i32PrevEpoch && i32Iteration == i32MaxIteration && i32Epoch > 0)
			{
				// ������ �н� ��� ��� �ޱ� // Get the last cost of the learning result
				float f32CurrCost = vctCosts.Back();
				// ������ ���� ��� �ޱ� // Get the last validation result
				float f32Validation = vctValidations.GetCount() ? vctValidations.Back() : 0;
				// ������ F1���� ��� �ޱ� // Get the last F1 Score result
				float f32F1Score = vctF1Score.GetCount() ? vctF1Score.Back() : 0;
				// �ش� epoch�� ���� ���� ��� �� ��� // Print cost and validation value for the relevant epoch
				printf("Cost : %.6f Validation : %.6f F1 Score : %.6f Epoch %d / %d\n", f32CurrCost, f32Validation, f32F1Score, i32Epoch, i32MaxEpoch);

				// ��� ����̳� ���� ��� ����� �ִٸ� ��� // Print results if cost or validation history exists
				if((vctCosts.GetCount() && i32PrevCostCount != (int32_t)vctCosts.GetCount()) || (vctValidations.GetCount() && i32PrevValidationCount != (int32_t)vctValidations.GetCount()))
				{
					viewGraph.LockUpdate();

					// ���� �׷����� �����͸� ���� // Clear previous grpah data
					viewGraph.Clear();
					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(vctCosts, EChartType_Line, RED, L"Cost");

					int32_t i32Step = classifier.GetLearningValidationStep();
					CFLArray<float> flaX;

					for(int64_t i = 0; i < vctValidations.GetCount() - 1; ++i)
						flaX.PushBack((float)(i * i32Step));

					flaX.PushBack((float)(vctCosts.GetCount() - 1));
					// Graph View ������ �Է� // Input Graph View Data
					viewGraph.Plot(flaX, vctValidations, EChartType_Line, BLUE, L"Validation");

					viewGraph.UnlockUpdate();
					viewGraph.Invalidate();
				}

				// ���� ����� 1.0�� ��� �Ǵ� esc Ű�� ���� ��� �н��� �ߴ��ϰ� �з� ���� 
				// If the validation result is 1.0 or press ESC key, stop learning and classify images 
				if(f32Validation == 1.f || GetAsyncKeyState(VK_ESCAPE))
					classifier.Stop();

				i32PrevEpoch = i32Epoch;
				i32PrevCostCount = (int32_t)vctCosts.GetCount();
				i32PrevValidationCount = (int32_t)vctValidations.GetCount();
			}

			// epoch��ŭ �н��� �Ϸ�Ǹ� ���� // End when learning progresses as much as epoch
			if(!classifier.IsRunning())
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

		// �߷� ��� ������ ���� ���� // Set for the inference result information
		classifier.SetInferenceResultItemSettings(CClassifierDL::EInferenceResultItemSettings_ClassNum_ClassName_ConfidenceScore);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = classifier.Execute()))
		{
			ErrorPrint(res, "Failed to execute Learn.\n");
			break;
		}

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.Invalidate(true);
		viewImageSource.Invalidate(true);
		viewImageValidate.Invalidate(true);
		// �׷��� �並 ���� // Update the Graph view.
		viewGraph.Invalidate(true);

		wprintf(L"Inference Complete\n");

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageSource.IsAvailable() && viewImageValidate.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}