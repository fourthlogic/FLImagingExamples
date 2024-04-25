#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;
	CFLImage fliRecognizeImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;
	CGUIViewImageWrap viewImageRecognize;
	CResult res;

	do
	{
		// �̹��� �ε� // Load image
		if((res = fliImage.Load(L"../../ExampleImages/OpticalCharacterRecognition/OCR_Learn.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		if((res = fliRecognizeImage.Load(L"../../ExampleImages/OpticalCharacterRecognition/OCR_Recognition.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((res = viewImage.Create(200, 0, 712, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Converted �̹��� �� ����
		if((res = viewImageRecognize.Create(712, 0, 1224, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((res = viewImage.SetImagePtr(&fliImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Converted �̹��� �信 �̹����� ���÷���
		if((res = viewImageRecognize.SetImagePtr(&fliRecognizeImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = viewImage.SynchronizeWindow(&viewImageRecognize)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImage.SynchronizePointOfView(&viewImageRecognize)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);
		CGUIViewImageLayerWrap layerRecognize = viewImageRecognize.GetLayer(1);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();
		layerRecognize.Clear();

		if((res = layer.DrawTextCanvas(&CFLPointD(0, 0), L"Learn", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text");
			break;
		}

		if((res = layerRecognize.DrawTextCanvas(&CFLPointD(0, 0), L"Recognition", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text");
			break;
		}

		// OpticalCharacterReader ��ü ���� // Create OpticalCharacterReader object
		COCR ocr;

		// ���ڸ� �н��� �̹��� ����
		if(IsFail(res = ocr.SetLearnImage(fliImage)))
		{
			ErrorPrint(res, L"Failed to set Source Image.");
			break;
		}

		// �н��� �̹����� ����Ǿ��ִ� Figure �н�
		if(IsFail(res = ocr.Learn()))
		{
			ErrorPrint(res, L"Failed to train.");
			break;
		}

		// �н��� ������ ������ �޾ƿ��� �Լ�
		int64_t i64LearntCount = ocr.GetLearntDataCount();
		CFLFigureArray flfaLearnt;
		CFLFigureArray flfaLearntOffset;

		// �н��� ������ ��縦 �޾ƿ��� �Լ�
		ocr.GetLearntCharacter(flfaLearnt);

		// �н��� ������ ��ġ�� �޾ƿ��� �Լ�
		ocr.GetLearntCharacterOffset(flfaLearntOffset);

		for(int64_t i = 0; i < i64LearntCount; ++i)
		{
			CFLString<wchar_t> flsResultString;
			CFLRect<double> flrBoundary;
			CFLPoint<double> flpOffset(flfaLearntOffset.GetAt(i));

			flsResultString.Format(L"%s", flfaLearnt.GetAt(i)->GetName());
			flfaLearnt.GetAt(i)->Offset(flpOffset);
			flfaLearnt.GetAt(i)->GetBoundaryRect(flrBoundary);

			if(IsFail(layer.DrawTextImage(CFLPoint<double>(flrBoundary.left, flrBoundary.top), flsResultString, YELLOW, BLACK, 15, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw learnt character : %lld", i);
				break;
			}

			if(IsFail(layer.DrawFigureImage(flfaLearnt.GetAt(i), LIME, 1, LIME, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw learnt character : %lld", i);
				break;
			}
		}

	   // ���ڸ� �ν��� �̹��� ����
		if(IsFail(res = ocr.SetSourceImage(fliRecognizeImage)))
		{
			ErrorPrint(res, L"Failed to set Source Image.");
			break;
		}

		// �ν��� ������ ������ ����
		if(IsFail(res = ocr.SetRecognizingCharacterColorType(COCR::ECharacterColorType_All)))
		{
			ErrorPrint(res, L"Failed to set recognizing character color.");
			break;
		}

		// �ν��� �ּ� ������ ����
		if(IsFail(res = ocr.SetRecognizingMinimumScore(0.6)))
		{
			ErrorPrint(res, L"Failed to set minimum score.");
			break;
		}

		// �ν��� �̹������� ���ڸ� ã�� ����� ����
		if(IsFail(res = ocr.Execute()))
		{
			ErrorPrint(res, res.GetString());
			break;
		}

		// ã�� ������ ������ �޾ƿ��� �Լ�
		int64_t i64ResultCount = ocr.GetResultCount();

		// ã�� ������ ������ �޾ƿ� �����̳�
		COCR::COCRRecognitionCharacterInfo resultChar;

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			ocr.GetResultRecognizedCharactersInfo(i, resultChar);

			CFLString<wchar_t> flsResultString;
			CFLRect<double> flrBoundary;

			flsResultString.Format(L"[%s] Score : %d%%, Angle : %.1lf", resultChar.flfaCharacter.GetName(), (int32_t)(resultChar.f64Score * 100), resultChar.f64Rotation);
			resultChar.flfaCharacter.GetBoundaryRect(flrBoundary);

			if(IsFail(layerRecognize.DrawTextImage(CFLPoint<double>(flrBoundary.left, flrBoundary.top), flsResultString, YELLOW, BLACK, 15, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layerRecognize.DrawFigureImage(resultChar.flfaCharacter, LIME, 1, LIME, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw recognized character : %lld", i);
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