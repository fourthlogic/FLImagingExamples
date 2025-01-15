#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;
	CResult res;

	do
	{
		// �̹��� �ε� // Load image
		if((res = fliImage.Load(L"../../ExampleImages/OCR/Dot_Print_Number.flif")).IsFail())
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

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((res = viewImage.SetImagePtr(&fliImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Learn & Recognize", YELLOW, BLACK, 30)).IsFail())
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
		int64_t i64LearnedCount = ocr.GetLearnedDataCount();
		CFLFigureArray flfaLearned;

		// �н��� ������ ��縦 �޾ƿ��� �Լ�
		ocr.GetLearnedCharacter(flfaLearned);

		for(int64_t i = 0; i < i64LearnedCount; ++i)
		{
			CFLString<wchar_t> flsResultString;
			CFLRect<double> flrBoundary;

			flsResultString.Format(L"%s", flfaLearned.GetAt(i)->GetName());
			flfaLearned.GetAt(i)->GetBoundaryRect(flrBoundary);

			if(IsFail(layer.DrawTextImage(CFLPoint<double>(flrBoundary.left, flrBoundary.top), flsResultString, YELLOW, BLACK, 15, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw learnt character : %lld", i);
				break;
			}

			if(IsFail(layer.DrawFigureImage(flfaLearned.GetAt(i), LIME, 1, LIME, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw learnt character : %lld", i);
				break;
			}
		}

	   // ���ڸ� �ν��� �̹��� ����
		if(IsFail(res = ocr.SetSourceImage(fliImage)))
		{
			ErrorPrint(res, L"Failed to set Source Image.");
			break;
		}

		// �ν��� ������ ���� ������ ����
		if(IsFail(res = ocr.SetRecognizingAngleTolerance(6.)))
		{
			ErrorPrint(res, L"Failed to set recognizing angle tolerance.");
			break;
		}

		// ���� ������ ���θ� ����
		if(IsFail(res = ocr.EnableRecognizingAutoScale(false)))
		{
			ErrorPrint(res, L"Failed to set recognizing auto scale.");
			break;
		}

		// �ν��� ������ �ּ� ���簢 �ʺ� ����
		if(IsFail(res = ocr.SetRecognizingMinimumRectangleWidth(20.)))
		{
			ErrorPrint(res, L"Failed to set recognizing minimum rectangle width.");
			break;
		}

		// �ν��� ������ �ּ� ���簢 ���̸� ����
		if(IsFail(res = ocr.SetRecognizingMinimumRectangleHeight(40.)))
		{
			ErrorPrint(res, L"Failed to set recognizing minimum rectangle height.");
			break;
		}

		// �ν��� ������ �ִ� ���簢 �ʺ� ����
		if(IsFail(res = ocr.SetRecognizingMaximumRectangleWidth(40.)))
		{
			ErrorPrint(res, L"Failed to set recognizing maximum rectangle width.");
			break;
		}

		// �ν��� ������ �ִ� ���簢 ���̸� ����
		if(IsFail(res = ocr.SetRecognizingMaximumRectangleHeight(60.)))
		{
			ErrorPrint(res, L"Failed to set recognizing maximum rectangle height.");
			break;
		}

		// �ν��� �ּ� ������ ����
		if(IsFail(res = ocr.SetRecognizingMinimumScore(0.5)))
		{
			ErrorPrint(res, L"Failed to set minimum score.");
			break;
		}

		// �ν��� �ִ� ������ ����
		if(IsFail(res = ocr.SetRecognizingMaximumCharacterCount(100)))
		{
			ErrorPrint(res, L"Failed to set maximum character count.");
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

			flsResultString.Format(L"[%s] Score: %d%%\nScale: %.2lf\nAngle : %.1lf", resultChar.flfaCharacter.GetName(), (int32_t)(resultChar.f64Score * 100), resultChar.f64ScaleWidth* resultChar.f64ScaleHeight, resultChar.f64Rotation);
			resultChar.flfaCharacter.GetBoundaryRect(flrBoundary);

			if(IsFail(layer.DrawTextImage(CFLPoint<double>(flrBoundary.left, flrBoundary.top), flsResultString, YELLOW, BLACK, 12, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layer.DrawFigureImage(resultChar.flfaCharacter, LIME, 1, LIME, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}