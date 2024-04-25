#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliRecognizeImage;
	CFLImage fliRecognizeImageUnicode;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageRecognize;
	CGUIViewImageWrap viewImageRecognizeUnicode;
	CResult res;

	do
	{
		if((res = fliRecognizeImage.Load(L"../../ExampleImages/OpticalCharacterRecognition/OCR_Recognition_15Degree.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		if((res = fliRecognizeImageUnicode.Load(L"../../ExampleImages/OpticalCharacterRecognition/OCR_Recognition_Unicode2.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Converted �̹��� �� ����
		if((res = viewImageRecognize.Create(200, 0, 712, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Converted �̹��� �� ����
		if((res = viewImageRecognizeUnicode.Create(712, 0, 1224, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Converted �̹��� �信 �̹����� ���÷���
		if((res = viewImageRecognize.SetImagePtr(&fliRecognizeImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Converted �̹��� �信 �̹����� ���÷���
		if((res = viewImageRecognizeUnicode.SetImagePtr(&fliRecognizeImageUnicode)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerRecognize = viewImageRecognize.GetLayer(1);
		CGUIViewImageLayerWrap layerRecognizeUnicode = viewImageRecognizeUnicode.GetLayer(1);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerRecognize.Clear();
		layerRecognizeUnicode.Clear();

		if((res = layerRecognize.DrawTextCanvas(&CFLPointD(0, 0), L"Recognition1", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text");
			break;
		}

		if((res = layerRecognizeUnicode.DrawTextCanvas(&CFLPointD(0, 0), L"Recognition2", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text");
			break;
		}

		// OpticalCharacterReader ��ü ���� // Create OpticalCharacterReader object
		COCR ocr;

		// �н� ���� ������ �ε�
		if(IsFail(res = ocr.Load(L"../../ExampleImages/OpticalCharacterRecognition/OCR_FourthLogic.flocr")))
		{
			ErrorPrint(res, L"Failed to load learnt file.");
			break;
		}

	    // ���ڸ� �ν��� �̹��� ����
		if(IsFail(res = ocr.SetSourceImage(fliRecognizeImage)))
		{
			ErrorPrint(res, L"Failed to set Source Image.");
			break;
		}

		// �ν��� ������ ���� ������ ����
		if(IsFail(res = ocr.SetRecognizingAngleTolerance(20.)))
		{
			ErrorPrint(res, L"Failed to set recognizing angle tolerance.");
			break;
		}

		// �ν��� �̹����� ��ó�� ���θ� ����
		if(IsFail(res = ocr.EnableRecognizingNoiseReduction(false)))
		{
			ErrorPrint(res, L"Failed to set recognizing Noise Reduction.");
			break;
		}

		// �ν��� ������ ������ ����
		if(IsFail(res = ocr.SetRecognizingCharacterColorType(COCR::ECharacterColorType_All)))
		{
			ErrorPrint(res, L"Failed to set recognizing character color.");
			break;
		}

		// �ν��� �ּ� ������ ����
		if(IsFail(res = ocr.SetRecognizingMinimumScore(0.7)))
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

		// ���ڸ� �ν��� �̹��� ����
		if(IsFail(res = ocr.SetSourceImage(fliRecognizeImageUnicode)))
		{
			ErrorPrint(res, L"Failed to set Source Image.");
			break;
		}

		// �ν��� ������ ���� ������ ����
		if(IsFail(res = ocr.SetRecognizingAngleTolerance(10.)))
		{
			ErrorPrint(res, L"Failed to set recognizing angle tolerance.");
			break;
		}

		// �ν��� �̹����� ��ó�� ���θ� ����
		if(IsFail(res = ocr.EnableRecognizingNoiseReduction(true)))
		{
			ErrorPrint(res, L"Failed to set recognizing Noise Reduction.");
			break;
		}

		// �ν��� ������ ������ ����
		if(IsFail(res = ocr.SetRecognizingCharacterColorType(COCR::ECharacterColorType_BlackOnWhite)))
		{
			ErrorPrint(res, L"Failed to set recognizing character color.");
			break;
		}

		// �ν��� �ּ� ������ ����
		if(IsFail(res = ocr.SetRecognizingMinimumScore(0.5)))
		{
			ErrorPrint(res, L"Failed to set minimum score.");
			break;
		}

		// �ν��� ������ �����ڵ� ���θ� ����
		if(IsFail(res = ocr.EnableRecognizingUnicodeByteCharacter(true)))
		{
			ErrorPrint(res, L"Failed to Enable unicode byte character.");
			break;
		}

		// �ν��� �̹������� ���ڸ� ã�� ����� ����
		if(IsFail(res = ocr.Execute()))
		{
			ErrorPrint(res, res.GetString());
			break;
		}

		// ã�� ������ ������ �޾ƿ��� �Լ�
		i64ResultCount = ocr.GetResultCount();

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			ocr.GetResultRecognizedCharactersInfo(i, resultChar);

			CFLString<wchar_t> flsResultString;
			CFLRect<double> flrBoundary;

			flsResultString.Format(L"[%s] Score : %d%%, Angle : %.1lf", resultChar.flfaCharacter.GetName(), (int32_t)(resultChar.f64Score * 100), resultChar.f64Rotation);
			resultChar.flfaCharacter.GetBoundaryRect(flrBoundary);

			if(IsFail(layerRecognizeUnicode.DrawTextImage(CFLPoint<double>(flrBoundary.left, flrBoundary.top), flsResultString, YELLOW, BLACK, 15, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layerRecognizeUnicode.DrawFigureImage(resultChar.flfaCharacter, LIME, 1, LIME, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageRecognize.Invalidate(true);
		viewImageRecognizeUnicode.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageRecognize.IsAvailable() && viewImageRecognizeUnicode.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}