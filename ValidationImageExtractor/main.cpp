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
	CFLImage fliSourceImage;
	CFLImage fliResultLearnImage;
	CFLImage fliResultValidationImage;

	/// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageResultLearn;
	CGUIViewImageWrap viewImageResultValidation;

	CResult eResult = EResult_UnknownError;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(eResult = fliSourceImage.Load(L"../../ExampleImages/SemanticSegmentation/Train.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates image view
		if(IsFail(eResult = viewImageSource.Create(100, 0, 600, 500)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageResultLearn.Create(600, 0, 1100, 500)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageResultValidation.Create(1100, 0, 1700, 500)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �� ���� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the three image view windows
		if(IsFail(eResult = viewImageSource.SynchronizeWindow(&viewImageResultLearn)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(eResult = viewImageSource.SynchronizeWindow(&viewImageResultValidation)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(eResult = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageResultLearn.SetImagePtr(&fliResultLearnImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageResultValidation.SetImagePtr(&fliResultValidationImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerLearn = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerValidation = viewImageResultLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerResultLabel = viewImageResultValidation.GetLayer(0);

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
		if(IsFail(eResult = layerLearn.DrawTextCanvas(&CFLPointD(0, 0), L"SOURCE", RED, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerValidation.DrawTextCanvas(&CFLPointD(0, 0), L"TRAIN(EXTRACTED (4/6))", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerResultLabel.DrawTextCanvas(&CFLPointD(0, 0), L"VALIDATION(EXTRACTED (2/6))", GREEN, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update the image view.
		viewImageSource.RedrawWindow();
		viewImageResultLearn.RedrawWindow();
		viewImageResultValidation.RedrawWindow();
		
		// Validation Image ���� ���� // Set ratio of validation image
		float f32Ratio = 0.4f;
		// Dataset type ���� // Set the data set type
		CValidationImageExtractorDL::EDatasetType eDatasetType = CValidationImageExtractorDL::EDatasetType_SemanticSegmentation;
		// Validation Set�� �ּ��� �� ���� Ŭ������ 1�� �̻� �� ���Ե� ������ ���� // Set how many classes each will be included in the Validation Set
		int64_t i64MinimumClassIncluded = 2;

		// Validation Extractor ���� // Validaiton Exctractor Execute 
		if(IsFail(eResult = CValidationImageExtractorDL::Extract(&fliSourceImage, f32Ratio, eDatasetType, &fliResultLearnImage, &fliResultValidationImage, i64MinimumClassIncluded)))
		{
			ErrorPrint(eResult, "Failed to Process\n");
			break;
		}

		// �� ���� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the three image view windows
		if(IsFail(eResult = viewImageSource.SynchronizeWindow(&viewImageResultLearn)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(eResult = viewImageSource.SynchronizeWindow(&viewImageResultValidation)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �̹��� �並 ���� // Update the image view.
		viewImageResultLearn.ZoomFit();
		viewImageResultValidation.ZoomFit();
		viewImageSource.RedrawWindow();
		viewImageResultLearn.RedrawWindow();
		viewImageResultValidation.RedrawWindow();

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSource.IsAvailable() && viewImageResultLearn.IsAvailable() && viewImageResultValidation.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}