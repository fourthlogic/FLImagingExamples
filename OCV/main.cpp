#include <cstdio>
#include <iostream>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage1;
	CFLImage fliImage2;
	CFLImage fliImage3;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage1;
	CGUIViewImageWrap viewImage2;
	CGUIViewImageWrap viewImage3;
	CResult res;

	do
	{
		// �̹��� �ε� // Load image
		if((res = fliImage1.Load(L"../../ExampleImages/OCV/Print_Example1.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		if((res = fliImage2.Load(L"../../ExampleImages/OCV/Print_Example2.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}
		
		if((res = fliImage3.Load(L"../../ExampleImages/OCV/Print_Example3.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((res = viewImage1.Create(200, 0, 712, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		if((res = viewImage2.Create(712, 0, 1224, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		if((res = viewImage3.Create(1224, 0, 1736, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((res = viewImage1.SetImagePtr(&fliImage1)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		if((res = viewImage2.SetImagePtr(&fliImage2)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		if((res = viewImage3.SetImagePtr(&fliImage3)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = viewImage1.SynchronizeWindow(&viewImage2)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		if((res = viewImage1.SynchronizeWindow(&viewImage3)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImage1.SynchronizePointOfView(&viewImage2)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		if((res = viewImage1.SynchronizePointOfView(&viewImage3)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer1 = viewImage1.GetLayer(0);
		CGUIViewImageLayerWrap layer2 = viewImage2.GetLayer(1);
		CGUIViewImageLayerWrap layer3 = viewImage3.GetLayer(2);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer1.Clear();
		layer2.Clear();
		layer3.Clear();

		// OpticalCharacterReader ��ü ���� // Create OpticalCharacterReader object
		COCV ocv;

		// OCR Font ������ �ε�
		if(IsFail(res = ocv.LoadFontData(L"../../ExampleImages/OCV/Font_A-Z_0-9.flocr")))
		{
			ErrorPrint(res, L"Failed to load Font file.");
			break;
		}

		// �ν��� �ּ� ����Ƽ�� ����
		if(IsFail(res = ocv.SetMinimumQuality(0.6)))
		{
			ErrorPrint(res, L"Failed to set minimum quality.");
			break;
		}

		// ���� ���� ���� ����ġ ����
		if(IsFail(res = ocv.SetSigma(5.)))
		{
			ErrorPrint(res, L"Failed to set sigma.");
			break;
		}

		// ���ڸ� ������ �̹��� ����
		if(IsFail(res = ocv.SetSourceImage(fliImage1)))
		{
			ErrorPrint(res, L"Failed to set Source Image.");
			break;
		}

		// �̹������� ���ڸ� �����ϴ� ����� ����
		if(IsFail(res = ocv.Execute()))
		{
			ErrorPrint(res, res.GetString());
			break;
		}

		if((res = layer1.DrawTextCanvas(&CFLPoint<double>(0, 0), ocv.GetResultVerificationState() == COCV::EVerificationState_OK ? L"Verify" : L"Fail", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text");
			break;
		}

		// ã�� ������ ������ �޾ƿ��� �Լ�
		int64_t i64ResultCount = ocv.GetResultCount();

		// ã�� ������ ������ �޾ƿ� �����̳�
		COCV::COCVVerificationCharacterInfo resultChar;

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			ocv.GetResultVerificationCharactersInfo(i, resultChar);

			CFLString<wchar_t> flsResultString;
			CFLString<wchar_t> flsResultString2 = L"";
			CFLRect<double> flrBoundary = resultChar.flrBoundary;
			CFLLine<double> fllBlankSpaceWidth = resultChar.fllBlankSpaceWidthLine;

			flsResultString.Format(L"[%s] Quality: %d%%\nScale: %.2lf\nAngle: %.1lf\nLighting: %.2lf\nContrast: %.2lf", resultChar.flfaCharacter.GetName(), (int32_t)(resultChar.f64Quality * 100), resultChar.f64ScaleWidth * resultChar.f64ScaleHeight, resultChar.f64Rotation, resultChar.f64Lighting, resultChar.f64Contrast);
			flsResultString2.Format(L"Space Width: %.2lf", resultChar.f64BlankSpaceWidth);

			if(IsFail(layer1.DrawTextImage(CFLPoint<double>(flrBoundary.left, flrBoundary.top), flsResultString, YELLOW, BLACK, 10, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layer1.DrawFigureImage(resultChar.flfaCharacter, LIME, 1, LIME, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layer1.DrawFigureImage(flrBoundary, resultChar.bVerified ? GREEN : RED, 3, resultChar.bVerified ? GREEN : RED, EGUIViewImagePenStyle_Solid, 1.f, 0.0f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			layer1.DrawFigureImage(resultChar.flfaIntrusion, YELLOW, 1, YELLOW, EGUIViewImagePenStyle_Solid, 1.f, 0.3f);
			layer1.DrawFigureImage(resultChar.flfaExtrusion, BLUE, 1, BLUE, EGUIViewImagePenStyle_Solid, 1.f, 0.3f);

			if(IsFail(layer1.DrawFigureImage(fllBlankSpaceWidth, BLACK, 3, BLACK, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layer1.DrawTextImage(CFLPointArray(fllBlankSpaceWidth)[0], flsResultString2, YELLOW, BLACK, 10, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}
		}

		// ���ڸ� ������ �̹��� ����
		if(IsFail(res = ocv.SetSourceImage(fliImage2)))
		{
			ErrorPrint(res, L"Failed to set Source Image.");
			break;
		}

		// �̹������� ���ڸ� �����ϴ� ����� ����
		if(IsFail(res = ocv.Execute()))
		{
			ErrorPrint(res, res.GetString());
			break;
		}

		if((res = layer2.DrawTextCanvas(&CFLPoint<double>(0, 0), ocv.GetResultVerificationState() == COCV::EVerificationState_OK ? L"Verify" : L"Fail", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text");
			break;
		}

		// ã�� ������ ������ �޾ƿ��� �Լ�
		i64ResultCount = ocv.GetResultCount();

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			ocv.GetResultVerificationCharactersInfo(i, resultChar);

			CFLString<wchar_t> flsResultString;
			CFLString<wchar_t> flsResultString2 = L"";
			CFLRect<double> flrBoundary = resultChar.flrBoundary;
			CFLLine<double> fllBlankSpaceWidth = resultChar.fllBlankSpaceWidthLine;

			flsResultString.Format(L"[%s] Quality: %d%%\nScale: %.2lf\nAngle: %.1lf\nLighting: %.2lf\nContrast: %.2lf", resultChar.flfaCharacter.GetName(), (int32_t)(resultChar.f64Quality * 100), resultChar.f64ScaleWidth * resultChar.f64ScaleHeight, resultChar.f64Rotation, resultChar.f64Lighting, resultChar.f64Contrast);
			flsResultString2.Format(L"Space Width: %.2lf", resultChar.f64BlankSpaceWidth);

			if(IsFail(layer2.DrawTextImage(CFLPoint<double>(flrBoundary.left, flrBoundary.top), flsResultString, YELLOW, BLACK, 10, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layer2.DrawFigureImage(resultChar.flfaCharacter, LIME, 1, LIME, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layer2.DrawFigureImage(flrBoundary, resultChar.bVerified ? GREEN : RED, 3, resultChar.bVerified ? GREEN : RED, EGUIViewImagePenStyle_Solid, 1.f, 0.0f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			layer2.DrawFigureImage(resultChar.flfaIntrusion, YELLOW, 1, YELLOW, EGUIViewImagePenStyle_Solid, 1.f, 0.3f);
			layer2.DrawFigureImage(resultChar.flfaExtrusion, BLUE, 1, BLUE, EGUIViewImagePenStyle_Solid, 1.f, 0.3f);

			if(IsFail(layer2.DrawFigureImage(fllBlankSpaceWidth, BLACK, 3, BLACK, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layer2.DrawTextImage(CFLPointArray(fllBlankSpaceWidth)[0], flsResultString2, YELLOW, BLACK, 10, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}
		}

		// ���ڸ� ������ �̹��� ����
		if(IsFail(res = ocv.SetSourceImage(fliImage3)))
		{
			ErrorPrint(res, L"Failed to set Source Image.");
			break;
		}

		// �̹������� ���ڸ� �����ϴ� ����� ����
		if(IsFail(res = ocv.Execute()))
		{
			ErrorPrint(res, res.GetString());
			break;
		}

		if((res = layer3.DrawTextCanvas(&CFLPoint<double>(0, 0), ocv.GetResultVerificationState() == COCV::EVerificationState_OK ? L"Verify" : L"Fail", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text");
			break;
		}

		// ã�� ������ ������ �޾ƿ��� �Լ�
		i64ResultCount = ocv.GetResultCount();

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			ocv.GetResultVerificationCharactersInfo(i, resultChar);

			CFLString<wchar_t> flsResultString;
			CFLString<wchar_t> flsResultString2 = L"";
			CFLRect<double> flrBoundary = resultChar.flrBoundary;
			CFLLine<double> fllBlankSpaceWidth = resultChar.fllBlankSpaceWidthLine;

			flsResultString.Format(L"[%s] Quality: %d%%\nScale: %.2lf\nAngle: %.1lf\nLighting: %.2lf\nContrast: %.2lf", resultChar.flfaCharacter.GetName(), (int32_t)(resultChar.f64Quality * 100), resultChar.f64ScaleWidth * resultChar.f64ScaleHeight, resultChar.f64Rotation, resultChar.f64Lighting, resultChar.f64Contrast);
			flsResultString2.Format(L"Space Width: %.2lf", resultChar.f64BlankSpaceWidth);

			if(IsFail(layer3.DrawTextImage(CFLPoint<double>(flrBoundary.left, flrBoundary.top), flsResultString, YELLOW, BLACK, 10, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layer3.DrawFigureImage(resultChar.flfaCharacter, LIME, 1, LIME, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layer3.DrawFigureImage(flrBoundary, resultChar.bVerified ? GREEN : RED, 3, resultChar.bVerified ? GREEN : RED, EGUIViewImagePenStyle_Solid, 1.f, 0.0f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			layer3.DrawFigureImage(resultChar.flfaIntrusion, YELLOW, 1, YELLOW, EGUIViewImagePenStyle_Solid, 1.f, 0.3f);
			layer3.DrawFigureImage(resultChar.flfaExtrusion, BLUE, 1, BLUE, EGUIViewImagePenStyle_Solid, 1.f, 0.3f);
			
			if(IsFail(layer3.DrawFigureImage(fllBlankSpaceWidth, BLACK, 3, BLACK, EGUIViewImagePenStyle_Solid, 1.f, 0.35f)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}

			if(IsFail(layer3.DrawTextImage(CFLPointArray(fllBlankSpaceWidth)[0], flsResultString2, YELLOW, BLACK, 10, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				printf("Failed to draw recognized character : %lld", i);
				break;
			}
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage1.Invalidate(true);
		viewImage2.Invalidate(true);
		viewImage3.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage1.IsAvailable() && viewImage2.IsAvailable() && viewImage3.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}