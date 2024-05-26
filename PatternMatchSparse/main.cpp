#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliLearnImage;
	CFLImage fliFindImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageFind;

	CResult res = EResult_UnknownError;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(res = fliLearnImage.Load(L"../../ExampleImages/Matching/Pattern2 Single Learn.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}
		 
		if(IsFail(res = fliFindImage.Load(L"../../ExampleImages/Matching/Pattern2 Single Find.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates imageview
		if(IsFail(res = viewImageLearn.Create(400, 0, 912, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageFind.Create(912, 0, 1680, 576)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(res = viewImageLearn.SetImagePtr(&fliLearnImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageFind.SetImagePtr(&fliFindImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImageFind)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerFind = viewImageFind.GetLayer(1);

		layerLearn.Clear();
		layerFind.Clear();

		if(IsFail(res = layerLearn.DrawTextCanvas(&CFLPoint<double>(0, 0), L"LEARN", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerFind.DrawTextCanvas(&CFLPoint<double>(0, 0), L"FIND", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// Pattern Match ��ü ���� // Create Pattern Match object
		CPatternMatchSparse FLPatternMatchSparse;

		// �н��� �̹��� ���� // Set the image to learn
		FLPatternMatchSparse.SetLearnImage(fliLearnImage);

		// �н��� ������ �����մϴ�. // Set the area to learn.
		CFLRect<double> learnRegion(150, 150, 760, 840);
		CFLPoint<double> flpLearnPivot = learnRegion.GetCenter();
		FLPatternMatchSparse.SetLearnROI(learnRegion);
		FLPatternMatchSparse.SetLearnPivot(flpLearnPivot);

		// ���ø� ������ �����մϴ�. // Set the sample count.
		FLPatternMatchSparse.SetSampleCount(64);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = FLPatternMatchSparse.Learn()))
		{
			ErrorPrint(res, "Failed to execute Learn.\n");
			break;
		}

		// ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the measurement area is
		if(IsFail(res = layerLearn.DrawFigureImage(&learnRegion, BLACK, 3)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		if(IsFail(res = layerLearn.DrawFigureImage(&learnRegion, CYAN)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		// ������ �߽����� ��ġ�� ���÷��� �Ѵ� // Display the position of the set center point
		CFLFigureArray flfaPointPivot = flpLearnPivot.MakeCrossHair(3, false);

		if(IsFail(res = layerLearn.DrawFigureImage(&flfaPointPivot, BLACK, 3)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		if(IsFail(res = layerLearn.DrawFigureImage(&flfaPointPivot, LIME)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		// �н��� ������ ���� Consoleâ�� ����Ѵ� // Print the learned information to the console window
		printf(" �� Learn Information\n");
		printf("  1. ROI Shape Type : Rectangle\n");
		printf("    left   : %.3lf\n", learnRegion.left);
		printf("    right  : %.3lf\n", learnRegion.right);
		printf("    top    : %.3lf\n", learnRegion.top);
		printf("    bottom : %.3lf\n", learnRegion.bottom);
		printf("    angle  : %.3lf\n", learnRegion.angle);
		printf("  2. Interest Pivot : (%.3lf, %.3lf)\n", flpLearnPivot.x, flpLearnPivot.y);
		printf("\n");

		// ������ �̹��� ���� // Set image to detect
		FLPatternMatchSparse.SetSourceImage(fliFindImage);

		// ���� �� ���� �⺻ ������ �����մϴ�. // Set the default angle to be used for detection.
		FLPatternMatchSparse.SetAngleBias(0.);
		// ���� �� ���� ������ Ž�������� �����մϴ�. // Set the search range of the angle to be used for detection.
		// ������ �⺻ ������ �������� (�⺻ ���� - AngleTolerance, �⺻ ���� + AngleTolerance)�� ���� Ž������ // The angle is based on the basic angle (default angle - AngleTolerance, basic angle + AngleTolerance) is the final search range
		FLPatternMatchSparse.SetAngleTolerance(10.);
		// ���� �� ���� �ּ� Ž�������� �����մϴ�. // Set the minimum search score to be used for detection.
		FLPatternMatchSparse.SetMinimumDetectionScore(0.7);
		// ���� �� ���� �ִ� Ž����ü ���� �����մϴ�. // Set the maximum number of search objects to be used for detection.
		FLPatternMatchSparse.SetMaxObject(1);
		// ���� �� ������ ��� ������ ���� �����մϴ�. // Set whether to use interpolation when detecting.
		FLPatternMatchSparse.EnableInterpolation(true);

		// �˰��� ���� // Execute the algorithm
		res = FLPatternMatchSparse.Execute();

		if(IsFail(res))
		{
			ErrorPrint(res, "Failed to execute\n");
			break;
		}

		// ���� ���� ����� �����ɴϴ�. // Get the pattern detection result.
		int64_t i64ResultCount = FLPatternMatchSparse.GetResultCount();

		printf(" �� Find Information\n");

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			CPatternMatchSparse::SResult results;

			FLPatternMatchSparse.GetResult(i, results);

			float f32Score = results.f32Score;
			float f32Angle = results.f32Angle;
			float f32Scale = results.f32Scale;
			Foundation::CFLPoint<double> flpPivot = results.flpPivot;
			Foundation::CFLFigure* pFlfRegion = results.pFlfRegion;
			Foundation::CFLRect<double>* pFlrResultRegion = dynamic_cast<Foundation::CFLRect<double>*>(pFlfRegion);

			if(!pFlrResultRegion)
				break;

			Foundation::CFLRect<double> flrResultRegion = *pFlrResultRegion;

			CFLString<wchar_t> strData;
			CFLPoint<double> flpResultRegion = CFLPoint<double>(flrResultRegion.left, flrResultRegion.top);

			// ���� ���� ����� Consoleâ�� ����մϴ�. // Output the pattern detection result to the console window.
			printf(" < Instance : %lld >\n", i);
			printf("  1. ROI Shape Type : Rectangle\n");
			printf("    left   : %.3lf\n", flrResultRegion.left);
			printf("    right  : %.3lf\n", flrResultRegion.right);
			printf("    top    : %.3lf\n", flrResultRegion.top);
			printf("    bottom : %.3lf\n", flrResultRegion.bottom);
			printf("    angle  : %.3lf\n", flrResultRegion.angle);
			printf("  2. Interest Pivot : (%.4lf, %.4lf)\n", flpResultRegion.x, flpResultRegion.y);
			printf("  3. Score : %0.3lf\n  4. Angle : %0.3lf\n  5. Scale : %0.3lf\n", f32Score, flrResultRegion.angle, f32Scale);
			printf("\n");

			// ���� ����� �߽����� ���÷��� �Ѵ� // Display the center point of the detection result
			CFLFigureArray flfaPoint = flpPivot.MakeCrossHair(3, false);
			flfaPoint.Rotate(f32Angle, &flpPivot);

			if(IsFail(res = layerFind.DrawFigureImage(&flfaPoint, BLACK, 3)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsFail(res = layerFind.DrawFigureImage(&flfaPoint, LIME)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}
			
			// ��� ������ ���÷��� �Ѵ� // Display the result area
			if(IsFail(res = layerFind.DrawFigureImage(&flrResultRegion, BLACK, 3)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsFail(res = layerFind.DrawFigureImage(&flrResultRegion, LIME)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			TPoint<double> tpPosition;
			tpPosition.x = flpPivot.x;
			tpPosition.y = flpPivot.y;

			CFLString<wchar_t> strText;
			strText.Format(L"Score : %.3lf\nAngle : %.3lf\n\n", f32Score, f32Angle);

			if((res = layerFind.DrawTextImage(tpPosition, strText, YELLOW, BLACK, 15, false, 0, EGUIViewImageTextAlignment_LEFT_CENTER)).IsFail())
			{
				ErrorPrint(res, "Failed to draw text");
				break;
			}
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImageLearn.Invalidate(true);
		viewImageFind.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageFind.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}