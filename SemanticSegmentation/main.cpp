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
	CGUIViewImageWrap viewImageResultLabel;
	CGUIViewImageWrap viewImageResultLabelFigure;

	// �׷��� �� ���� // Declare the graph view
	CGUIViewGraphWrap viewGraph;

	CResult eResult = EResult_UnknownError;

	do
	{
		// �̹��� �ε� // Loads image
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

		// �̹��� �� ���� // Creates image view
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

		// Graph �� ���� // Create graph view
		if(IsFail(eResult = viewGraph.Create(1100, 0, 1600, 500)))
		{
			ErrorPrint(eResult, " Failed to create the graph view. \n");
			break;
		}

		viewGraph.SetDarkMode();

		// �ټ� ���� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the four image view windows
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

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
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

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerValidation = viewImageValidation.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabel = viewImageResultLabel.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabelFigure = viewImageResultLabelFigure.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerLearn.Clear();
		layerValidation.Clear();
		layerResultLabel.Clear();
		layerResultLabelFigure.Clear();

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
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

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImageResultLabel.RedrawWindow();
		viewImageResultLabelFigure.RedrawWindow();

		// SemanticSegmentation ��ü ���� // Create SemanticSegmentation object
		CSemanticSegmentationDL semanticSegmentation;

		// OptimizerSpec ��ü ���� // Create OptimizerSpec object
		COptimizerSpecAdamGradientDescent optSpec;

		// �н��� �̹��� ���� // Set the image to learn
		semanticSegmentation.SetLearningImage(fliLearnImage);
		// ������ �̹��� ���� // Set the image to validate
		semanticSegmentation.SetLearningValidationImage(fliValidationImage);
		// �з��� �̹��� ���� // Set the image to classify
		semanticSegmentation.SetSourceImage(fliValidationImage);

		// �н��� SemanticSegmentation �� ���� // Set up SemanticSegmentation model to learn
		semanticSegmentation.SetModel(CSemanticSegmentationDL::EModel_FL_SS_GP);
		// �н��� SemanticSegmentation ���� ���� ���� // Set up SemanticSegmentation model version to learn
		semanticSegmentation.SetModelVersion(CSemanticSegmentationDL::EModelVersion_FL_SS_GP_V1_512);
		// �н� epoch ���� ���� // Set the learn epoch value 
		semanticSegmentation.SetLearningEpoch(120);
		// �н� �̹��� Interpolation ��� ���� // Set Interpolation method of learn image
		semanticSegmentation.SetInterpoloationMethod(EInterpolationMethod_Bilinear);

		// Optimizer�� �н��� ���� // Set learning rate of Optimizer
		optSpec.SetLearningRate(1e-4f);

		// ������ Optimizer�� SemanticSegmentation�� ���� // Apply the Optimizer that we set up to SemanticSegmentation
		semanticSegmentation.SetLearningOptimizerSpec(optSpec);

		// AugmentationSpec ���� // Set the AugmentationSpec
		CAugmentationSpec augSpec;

		augSpec.SetCommonIOUThreshold(0.5);
		augSpec.SetCommonInterpolationMethod(FLImaging::ImageProcessing::EInterpolationMethod_Bilinear);
		augSpec.EnableRotation(true);
		augSpec.SetRotationParam(180., false);
		augSpec.EnableFlip(true);
		augSpec.EnableGaussianNoise(true);
		semanticSegmentation.SetLearningAugmentationSpec(&augSpec);

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
			int32_t i32Iteration = semanticSegmentation.GetCurrentIteration();
			// ������ �н� Ƚ�� �ޱ� // Get the last epoch learning
			int32_t i32Epoch = semanticSegmentation.GetLastEpoch();

			// �̴� ��ġ �ݺ��� �Ϸ�Ǹ� cost�� validation ���� ���÷��� 
			// Display cost and validation value if iterations of the mini batch is completed 
			if(i32Epoch != i32PrevEpoch && i32Iteration == i32MaxIteration && i32Epoch > 0)
			{
				// ������ �н� ��� ��� �ޱ� // Get the last cost of the learning result
				float f32CurrCost;
				semanticSegmentation.GetLearningResultLastCost(f32CurrCost);
				// ������ ���� ��� �ޱ� // Get the last validation result
				float f32ValidationPa = 0;
				float f32ValidationMeanIoU = 0;
				semanticSegmentation.GetLearningResultLastPixelAccuracy(f32ValidationPa);
				semanticSegmentation.GetLearningResultLastMeanIoU(f32ValidationMeanIoU);
				// �ش� epoch�� ���� ���� ��� �� ��� // Print cost and validation value for the relevant epoch
				printf("Cost : %.6f Validation : %.6f mIoU : %.6f Epoch %d / %d\n", f32CurrCost, f32ValidationPa, f32ValidationMeanIoU, i32Epoch, i32MaxEpoch);

				// �н� ��� ���� ���� ��� ����� �޾� �׷��� �信 ���  
				// Get the history of cost and validation and print it at graph view
				CFLArray<float> vctCosts;
				CFLArray<float> vctVadliationPixelAccuracy;
				CFLArray<float> vctMeanIoU;

				semanticSegmentation.GetLearningResultAllHistory(&vctCosts, &vctVadliationPixelAccuracy, &vctMeanIoU);

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
				if(f32ValidationPa == 1.f)
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

		// �˰��� ���� // Execute the algorithm
		if(IsFail(eResult = semanticSegmentation.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute.\n");
			break;
		}

		// ResultLabelImage �޾ƿ��� // Get The ResultLabelImage
		semanticSegmentation.GetInferenceResultImage(&fliResultLabelImage);

		// SegmentationRegionExtractor�� �̿��Ͽ� �� �̹����� �ǰܷ� ���� // Extract label image into figure using SegmentationRegionExtractor
		// SegmentationRegionExtractor ��ü ���� // Create the SegmentationRegionExtractor object
		CSegmentationRegionExtractor semanticRE;

		// SegmentationRegionExtractor �Ķ���� ���� // Set the SegmentationRegionExtractor's parameters
		int32_t i32Mask = (int32_t)CBlob::EBlobResultType_Contour;

		semanticRE.SetResultType(CBlob::EBlobResultType(i32Mask));
		semanticRE.AddRangesToInclude(std::make_pair((int32_t)1, (int32_t)semanticSegmentation.GetLearningResultClassCount()));
		semanticRE.SetContourResultType(CBlob::EContourResultType_Perforated);
		semanticRE.SetSourceImage(fliResultLabelImage);

		// SegmentationRegionExtractor ���� // Execute SegmentationRegionExtractor
		if(IsFail(eResult = semanticRE.Execute()))
		{
			ErrorPrint(eResult, "Failed to process\n");
			break;
		}

		// ����� �����Ͽ� �̹����� �ٿ��ֱ� // Extract results and paste them into image

		CFLFigureArray flfaResultContours;

		if(IsFail(eResult = semanticRE.GetResultContours(flfaResultContours)))
		{
			ErrorPrint(eResult, "Failed to process\n");
			break;
		}

		Base::CFLArray <int64_t> flaLabelList;
		// ResultContours �ε����� ��Ī �Ǵ� �� ��ȣ�迭�� �������� // ResultContours Get an array of label numbers matching the index.
		semanticRE.GetResultSegmentationList(flaLabelList);

		int64_t i64ResultCount = flaLabelList.GetCount();

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			Base::CFLArray<Base::CFLStringW> flaNames;
			CFLFigureArray* flfaResultContourCur = (CFLFigureArray * )flfaResultContours.GetAt(i);
			CFLStringW flsLabel;
			int64_t i32ClassNumber = flaLabelList[i];

			semanticSegmentation.GetLearningResultClassNames(i32ClassNumber, &flaNames);
			flsLabel.Format(L"%d(%s)", i32ClassNumber, flaNames[0].GetString());
			flfaResultContourCur->SetName(flsLabel);
			fliResultLabelFigureImage.PushBackFigure(CROIUtilities::ConvertFigureObjectToString(flfaResultContourCur));
			//ResultLabel �̹����� ���׸����̼� �� �ؽ�Ʈ ���� // Set segmentation label text for tthe result label image
			viewImageResultLabel.SetSegmentationLabelText(0, (double)i32ClassNumber, flaNames[0].GetString());
		}

		// ResultLabel �信 Floating Value Range�� ���� // Set Floating Value Range in ResultLabel view
		viewImageResultLabel.SetFloatingImageValueRange(0.f, (float)semanticSegmentation.GetLearningResultClassCount());

		// �̹��� �並 ���� // Update the image view.
		viewImageLearn.RedrawWindow();
		viewImageValidation.RedrawWindow();
		viewImageResultLabel.RedrawWindow();
		viewImageResultLabelFigure.RedrawWindow();

		// �׷��� �並 ���� // Update the Graph view.
		viewGraph.RedrawWindow();

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageValidation.IsAvailable() && viewImageResultLabel.IsAvailable() && viewImageResultLabelFigure.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}