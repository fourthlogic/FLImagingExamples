#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliLearnImage[2];
	CFLImage fliFindImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn[2];
	CGUIViewImageWrap viewImageFind;

	CResult eResult = EResult_UnknownError;

	// Pattern Match Multi ��ü ���� // Create Pattern Match Multi object
	CPatternMatchMulti FLPatternMatchMultiSave;

	do
	{
		CFLString<wchar_t> arrPath[2] = {L"../../ExampleImages/Matching/Pattern Multi Learn.flif",
			L"../../ExampleImages/Matching/Pattern Multi Learn.flif"};

		CFLString<wchar_t> arrClassName[2] = {L"A", L"B"};
		uint32_t arrColor[2] = {LIME, RED};
		CFLRectD arrLearnRegion[2] = { CFLRectD(390.76, 174.77, 678.95, 457.39), CFLRectD(23.55,742.79,176.94,888.74) };

		printf(" �� Learn Information\n");

		for(int64_t i64DataIdx = 0; i64DataIdx < 2; ++i64DataIdx)
		{
			// �̹��� �ε� // Loads image
			if(IsFail(eResult = fliLearnImage[i64DataIdx].Load(arrPath[i64DataIdx].GetBuffer())))
			{
				ErrorPrint(eResult, "Failed to load the image file.\n");
				break;
			}

			// �̹��� �� ���� // Creates imageview
			if(IsFail(eResult = viewImageLearn[i64DataIdx].Create((int32_t)(400 + 512 * i64DataIdx), 0, (int32_t)(400 + 512 * (i64DataIdx + 1)), 384)))
			{
				ErrorPrint(eResult, "Failed to create the image view.\n");
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
			if(IsFail(eResult = viewImageLearn[i64DataIdx].SetImagePtr(&fliLearnImage[i64DataIdx])))
			{
				ErrorPrint(eResult, "Failed to set image object on the image view.\n");
				break;
			}

			CGUIViewImageLayerWrap layerLearn = viewImageLearn[i64DataIdx].GetLayer(0);

			layerLearn.Clear();

			// �н��� �̹��� ���� // Set the image to learn
			FLPatternMatchMultiSave.SetLearnImage(fliLearnImage[i64DataIdx]);

			// �н��� ������ �����մϴ�. // Set the area to learn.
			CFLPointD flpLearnPivot = arrLearnRegion[i64DataIdx].GetCenter();
			FLPatternMatchMultiSave.SetLearnROI(arrLearnRegion[i64DataIdx]);
			FLPatternMatchMultiSave.SetLearnPivot(flpLearnPivot);

		    // �˰��� ���� // Execute the algorithm
			if(IsFail(eResult = FLPatternMatchMultiSave.Learn(arrClassName[i64DataIdx])))
			{
				ErrorPrint(eResult, "Failed to execute Learn.\n");
				break;
			}

			// ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the measurement area is
			if(IsFail(eResult = layerLearn.DrawFigureImage(&arrLearnRegion[i64DataIdx], BLACK, 3)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			if(IsFail(eResult = layerLearn.DrawFigureImage(&arrLearnRegion[i64DataIdx], arrColor[i64DataIdx])))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			// ������ �߽����� ��ġ�� ���÷��� �Ѵ� // Display the position of the set center point
			CFLFigureArray flfaPointPivot = flpLearnPivot.MakeCrossHair(3, false);

			if(IsFail(eResult = layerLearn.DrawFigureImage(&flfaPointPivot, BLACK, 3)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			if(IsFail(eResult = layerLearn.DrawFigureImage(&flfaPointPivot, LIME)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

		
			CFLString<wchar_t> strStatus;
			strStatus.Format(L"LEARN CLASS '%s'", arrClassName[i64DataIdx].GetBuffer());

			if(IsFail(eResult = layerLearn.DrawTextCanvas(&CFLPointD(0, 0), strStatus.GetBuffer(), YELLOW, BLACK, 30)))
			{
				ErrorPrint(eResult, "Failed to draw text\n");
				break;
			}

			// �н��� ������ ���� Consoleâ�� ����Ѵ� // Print the learned information to the console window
			printf("  < LEARN CLASS '%ws' > \n", arrClassName[i64DataIdx].GetBuffer());
			printf("  1. ROI Shape Type : Rectangle\n");
			printf("    left   : %.3lf\n", arrLearnRegion[i64DataIdx].left);
			printf("    right  : %.3lf\n", arrLearnRegion[i64DataIdx].right);
			printf("    top    : %.3lf\n", arrLearnRegion[i64DataIdx].top);
			printf("    bottom : %.3lf\n", arrLearnRegion[i64DataIdx].bottom);
			printf("    angle  : %.3lf\n", arrLearnRegion[i64DataIdx].angle);
			printf("  2. Interest Pivot : (%.3lf, %.3lf)\n", flpLearnPivot.x, flpLearnPivot.y);
			printf("\n");


			// �̹��� �並 ���� �մϴ�. // Update the image view.
			viewImageLearn[i64DataIdx].Invalidate(true);
		}
		
		// ������ �߰��� �Ϸ� �� Save�� �����մϴ�. // After completing data addition, proceed with Save.
		if(IsFail(eResult = FLPatternMatchMultiSave.Save(L"../../ExampleImages/Matching/Pattern Multi Learn File")))
		{
			ErrorPrint(eResult, "Failed to save\n");
			break;
		}

		// �̹��� �ε� // Loads image
		if(IsFail(eResult = fliFindImage.Load(L"../../ExampleImages/Matching/Pattern Multi Find.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}


		// �̹��� �� ���� // Creates imageview
		if(IsFail(eResult = viewImageFind.Create(400, 384, 1168, 960)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(eResult = viewImageFind.SetImagePtr(&fliFindImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		for(int64_t i64DataIdx = 0; i64DataIdx < 2; ++i64DataIdx)
		{
			// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
			if(IsFail(eResult = viewImageFind.SynchronizeWindow(&viewImageLearn[i64DataIdx])))
			{
				ErrorPrint(eResult, "Failed to synchronize window.\n");
				break;
			}
		}

		CGUIViewImageLayerWrap layerFind = viewImageFind.GetLayer(1);
		layerFind.Clear();

		if(IsFail(eResult = layerFind.DrawTextCanvas(&CFLPointD(0, 0), L"FIND", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Fai led to draw text\n");
			break;
		}

		CPatternMatchMulti FLPatternMatchMultiLoad;

		// �����͸� Load�մϴ�. // Load data.
		if(IsFail(eResult = FLPatternMatchMultiLoad.Load(L"../../ExampleImages/Matching/Pattern Multi Learn File")))
		{
			ErrorPrint(eResult, "Failed to save\n");
			break;
		}

		// ������ �̹��� ���� // Set image to detect
		FLPatternMatchMultiSave.SetSourceImage(fliFindImage);

		// ���� �� ���� �Ķ���͸� �����մϴ�. // Set the parameters to be used for detection.
		// ���� �� ���� ������ ������ �����մϴ�. // Set the scale section to be used for detection.
		FLPatternMatchMultiSave.SetScaleRange(1.0, 1.0);
		// ���� �� ���� �⺻ ������ �����մϴ�. // Set the default angle to be used for detection.
		FLPatternMatchMultiSave.SetAngleBias(0.);
		// ���� �� ���� ������ Ž�������� �����մϴ�. // Set the search range of the angle to be used for detection.
		// ������ �⺻ ������ �������� (�⺻ ���� - AngleTolerance, �⺻ ���� + AngleTolerance)�� ���� Ž������ // The angle is based on the basic angle (default angle - AngleTolerance, basic angle + AngleTolerance) is the final search range
		FLPatternMatchMultiSave.SetAngleTolerance(10.);
		// ���� �� ����ȭ ������ �����մϴ�. // Set the degree of optimization for detection.
		// 0 ~ 1�������� 0�� �������� ��Ȯ���� ������ �� ������, �ӵ��� ����˴ϴ�. // From 0 to 1, the closer to 0, the lower the accuracy, but the higher the speed.
		FLPatternMatchMultiSave.SetAccuracy(0.5);
		// ���� �� ���� �ּ� Ž�������� �����մϴ�. // Set the minimum search score to be used for detection.
		FLPatternMatchMultiSave.SetMinimumDetectionScore(0.7);
		// ���� �� ���� Ž�� ����� �����մϴ�. // Set the search method to be used for detection.
		FLPatternMatchMultiSave.SetMaxObjectMode(CPatternMatchMulti::EMaxObjectMode_Total);
		// ���� �� ���� �ִ� Ž����ü ���� �����մϴ�. // Set the maximum number of search objects to be used for detection.
		FLPatternMatchMultiSave.SetMaxObjectTotal(2);
		// ���� �� ������ ��� ������ ���� �����մϴ�. // Set whether to use interpolation when detecting.
		FLPatternMatchMultiSave.EnableInterpolation();
		// ���� �� ���� �ٸ� Ŭ������ ���� ���� �ߺ��� ��� ������ ���� �����մϴ�. // Set whether to allow area overlap for different classes during detection.
		FLPatternMatchMultiSave.SetConflictDetectionMethod();

		// �˰��� ���� // Execute the algorithm
		eResult = FLPatternMatchMultiSave.Execute();

		if(IsFail(eResult))
		{
			ErrorPrint(eResult, "Failed to execute\n");
			break;
		}

		// ���� ���� ����� �����ɴϴ�. // Get the pattern detection result.
		int64_t i64ResultCount = FLPatternMatchMultiSave.GetResultCount();

		printf(" �� Find Information\n");

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			CPatternMatchMulti::SResult results;
			CFLFigureArray flfaResultPoints;

			FLPatternMatchMultiSave.GetResult(i, results);

			float f32Score = results.f32Score;
			float f32Angle = results.f32Angle;
			float f32Scale = results.f32Scale;
			Foundation::CFLPointD flpPivot = results.flpPivot;
			Foundation::CFLFigure* pFlfRegion = results.pFlfRegion;
			Foundation::CFLRectD* pFlrResultRegion = dynamic_cast<Foundation::CFLRectD*>(pFlfRegion);

			if(!pFlrResultRegion)
				break;

			Foundation::CFLRectD flrResultRegion = *pFlrResultRegion;

			CFLString<wchar_t> strResult;
			CFLString<wchar_t> wstrClassName = results.strClassName;

			int64_t i64Idx = 0;

			for(int64_t i64ResultIndex = 0; i64ResultIndex < 3; ++i64ResultIndex)
			{
				if(wstrClassName == arrClassName[i64ResultIndex])
				{
					i64Idx = i64ResultIndex;
					break;
				}
			}
			
			// ���� ���� ����� Consoleâ�� ����մϴ�. // Output the pattern detection result to the console window.
			printf(" < Instance : %lld >\n", i);
			printf(" Class Name : %ws\n", wstrClassName.GetBuffer());
			printf("  1. ROI Shape Type : Rectangle\n");
			printf("    left   : %.3lf\n", flrResultRegion.left);
			printf("    right  : %.3lf\n", flrResultRegion.right);
			printf("    top    : %.3lf\n", flrResultRegion.top);
			printf("    bottom : %.3lf\n", flrResultRegion.bottom);
			printf("    angle  : %.3lf\n", flrResultRegion.angle);
			printf("  2. Interest Pivot : (%.3lf, %.3lf)\n", flpPivot.x, flpPivot.y);
			printf("  3. Score : %0.3lf\n  4. Angle : %0.3lf\n  5. Scale : %0.3lf\n", f32Score, f32Angle, f32Scale);

			printf("\n");

			if(IsFail(eResult = layerFind.DrawFigureImage(&flrResultRegion, BLACK, 3)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			if(IsFail(eResult = layerFind.DrawFigureImage(&flrResultRegion, arrColor[i64Idx])))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			// ���� ����� �߽����� ���÷��� �Ѵ� // Display the center point of the detection result
			CFLFigureArray flfaPoint = flpPivot.MakeCrossHair(3, false);
			flfaPoint.Rotate(f32Angle, &flpPivot);
			 
			if(IsFail(eResult = layerFind.DrawFigureImage(&flfaPoint, BLACK, 3)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			if(IsFail(eResult = layerFind.DrawFigureImage(&flfaPoint, LIME)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			TPoint<double> tpPosition;
			tpPosition.x = flpPivot.x;
			tpPosition.y = flpPivot.y;

			// ���� ����� �ش��ϴ� Ŭ�������� ���÷��� �Ѵ� // Display the class name corresponding to the detection result
			if(IsFail(eResult = layerFind.DrawTextImage(tpPosition, wstrClassName.GetBuffer(), YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_CENTER, nullptr, 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
			{
				ErrorPrint(eResult, "Failed to draw text\n");
				break;
			}

			tpPosition.x += 10;
			CFLString<wchar_t> strText;
			strText.Format(L"Score : %.3lf\nAngle : %.3lf\nScale : x%.3lf\n", f32Score, f32Angle, f32Scale);

			if((eResult = layerFind.DrawTextImage(tpPosition, strText, YELLOW, BLACK, 15, false, 0, EGUIViewImageTextAlignment_LEFT_CENTER)).IsFail())
			{
				ErrorPrint(eResult, "Failed to draw text");
				break;
			}
		}

		viewImageFind.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageFind.IsAvailable() && viewImageLearn[0].IsAvailable() && viewImageLearn[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}