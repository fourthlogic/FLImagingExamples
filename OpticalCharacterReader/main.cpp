#include <cstdio>

#include <FLImaging.h>


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;
	CFLImage fliRecognizeImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;
	CGUIViewImageWrap viewImageRecognize;

	do
	{
		// �̹��� �ε� // Load image
		if(IsFail(fliImage.Load(L"../../ExampleImages/OpticalCharacterReader/Fourthlogic_Font.flif")))
		{
			printf("Failed to load the image file.\n");
			break;
		}

		if(IsFail(fliRecognizeImage.Load(L"../../ExampleImages/OpticalCharacterReader/Fourthlogic_Recognition.jpg")))
		{
			printf("Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(viewImage.Create(200, 0, 712, 384)))
		{
			printf("Failed to create the image view.\n");
			break;
		}

		// Converted �̹��� �� ����
		if(IsFail(viewImageRecognize.Create(712, 0, 1716, 508)))
		{
			printf("Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if(IsFail(viewImage.SetImagePtr(&fliImage)))
		{
			printf("Failed to set image object on the image view.\n");
			break;
		}

		// Converted �̹��� �信 �̹����� ���÷���
		if(IsFail(viewImageRecognize.SetImagePtr(&fliRecognizeImage)))
		{
			printf("Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(viewImage.SynchronizeWindow(&viewImageRecognize)))
		{
			printf("Failed to synchronize window.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);
		CGUIViewImageLayerWrap layerRecognize = viewImageRecognize.GetLayer(1);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();
		layerRecognize.Clear();

		if(IsFail(layer.DrawTextCanvas(&CFLPointD(0, 0), L"Train", YELLOW, BLACK, 30)))
		{
			printf("Failed to draw text");
			break;
		}

		if(IsFail(layerRecognize.DrawTextCanvas(&CFLPointD(0, 0), L"Recognition", YELLOW, BLACK, 30)))
		{
			printf("Failed to draw text");
			break;
		}

		// OpticalCharacterReader ��ü ���� // Create OpticalCharacterReader object
		COpticalCharacterReader lattice;
		CResult eResult = EResult_UnknownError;

		// ���ڸ� �н��� �̹��� ����
		if(IsFail(lattice.SetLearnImage(fliImage)))
		{
			printf("Failed to set Source Image.");
			break;
		}

		// �̹������� �н��� ������ ���� �� ��� ������ ����
		if(IsFail(lattice.SetCharacterColor(COpticalCharacterReader::ECharacterColorType_BlackOnWhite)))
		{
			printf("Failed to set character color type.");
			break;
		}

		// �н��� �̹������� ������ �������� ũ�⸦ ����, �������� ����
		if(IsFail(lattice.SetNoiseFilterSize(5, 5000, 5, 5000)))
		{
			printf("Failed to set noise filter size.");
			break;
		}

		// �н��� �̹����� ����Ǿ��ִ� Figure �н�
		if(IsFail(lattice.Train()))
		{
			printf("Failed to train.");
			break;
		}

		// ROI ���� ����
		 CFLRectL flrROI = CFLRectL(0, 0, 1003, 508);

		// ���ڸ� �ν��� �̹��� ����
		if(IsFail(lattice.SetSourceImage(fliRecognizeImage)))
		{
			printf("Failed to set Source Image.");
			break;
		}

		// �ν��� �̹����� ROI ���� ����
		if(IsFail(lattice.SetSourceROI(flrROI)))
		{
			printf("Failed to set Source ROI.");
			break;
		}

		// �ν��� ������ ������ ����
		if(IsFail(lattice.SetAngleRange(0., 0.)))
		{
			printf("Failed to set noise filter size.");
			break;
		}

		// �ν��� �̹������� ������ Threshold ���� �Է� ���� �ʰ� �ڵ����� ã�� ��带 ����
		if(IsFail(lattice.SetThresholdAuto(false)))
		{
			printf("Failed to set threshold auto.");
			break;
		}

		// �ν��� �̹������� ������ Threshold ���� ����
		if(IsFail(lattice.SetThreshold(135)))
		{
			printf("Failed to set threshold.");
			break;
		}

		// �ν��� �̹������� ã�� ������ ���� ���ھ�� ����(0.0~1.0)
		if(IsFail(lattice.SetScoreFilter(.9)))
		{
			printf("Failed to set score filter.");
			break;
		}

		// �ν��� �̹������� ���ڸ� ã�� ����� ����
		if(IsFail(eResult = lattice.Execute()))
		{
			wprintf(eResult.GetString());
			break;
		}

		// ã�� ������ ������ �޾ƿ��� �Լ�
		int64_t i64ResultCount = lattice.GetResultCharacterCount();

		// ã�� ������ ������ �޾ƿ� �����̳�
		COpticalCharacterReader::CResultCharacter resultChar;

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			lattice.GetResultCharacter(i, resultChar);

			if(resultChar.wcCharacter == 0)
				continue;

			CFLStringW fls = resultChar.wcCharacter;

			if(IsFail(layerRecognize.DrawTextImage(&CFLPointD(resultChar.flrRegion.left, resultChar.flrRegion.top), fls, RED, WHITE, 16)))
			{
				printf("Failed to draw recognized character : %d", i);
				break;
			}

			if(IsFail(layerRecognize.DrawFigureImage(&resultChar.flrRegion, TOMATO, 2, 0, EGUIViewImagePenStyle_Solid, 1, 0)))
			{
				printf("Failed to draw recognized character : %d", i);
				break;
			}
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);
		viewImageRecognize.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable() && viewImageRecognize.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}