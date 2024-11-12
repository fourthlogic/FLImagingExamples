#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliFindImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageFind;

	CResult res = EResult_UnknownError;
	CFLFigureArray flfLearntDrawing;

	do
	{
		if(IsFail(res = fliFindImage.Load(L"../../ExampleImages/Matching/DrawingImage.flif")))
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

		if(IsFail(res = viewImageFind.SetImagePtr(&fliFindImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
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

		// Geometric Match ��ü ���� // Create Geometric Match object
		CDrawingMatch FLDrawingMatch;

		// �н��� �̹��� ���� // Set the image to learn
		// �н��� ������ �����մϴ�. // Set the drawing to learn.
		FLDrawingMatch.SetDrawing(L"../../ExampleImages/Matching/Drawing.gbr");
		// ���鿡 ���� ���ش� ������ �����մϴ�. // Set unit of pixel accuracy.
		FLDrawingMatch.SetDistanceUnit(CDrawingMatch::EDistanceUnit_Millimeter);
		// ���鿡 ���� ���ش��� �����մϴ�. // Set  pixel accuracy.
		FLDrawingMatch.SetPixelAccuracy(0.05, 0.05);

		// �н� �Ķ���͸� �����մϴ�. // Set the learning parameters.
		// ������ Ư¡�� ������ �����մϴ�. // Set the number of feature points to be extracted.
		FLDrawingMatch.SetFeatureCount();
		// ������ Ư¡�� ó������������ ������ ���͸� ������ �����մϴ�. // Set the noise filtering degree in the process of processing the feature points to be extracted.
		FLDrawingMatch.SetFeatureFiltering();
		// ������ Ư¡�� ó������������ ��� �Ӱ谪�� �����մϴ�. // Set the allowable threshold in the feature point processing process to be extracted.
		FLDrawingMatch.SetLearnThresholdCoeff();

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = FLDrawingMatch.Learn()))
		{
			ErrorPrint(res, "Failed to execute Learn.\n");
			break;
		}

		// �н��� Ư¡���� ���÷��� �Ѵ� // Display the learned feature point
		// �н��� Ư¡���� ã�����ϴ� ��ü�� ��Ÿ���⿡ ����ϰ� �� �������� Ȯ���ϰ�, �׷��� �ʴٸ� �н� �Ķ���͸� �����������ν� ��Ȯ���ϸ� ���� �� �� ȿ�����Դϴ�. // Check whether the learned feature points are selected well enough to represent the object to be found.
		flfLearntDrawing = FLDrawingMatch.GetLearntDrawing();

		if(IsFail(res = layerLearn.DrawFigureImage(flfLearntDrawing, BLUE)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}


		// ������ �̹��� ���� // Set image to detect
		FLDrawingMatch.SetSourceImage(fliFindImage);

		// ���� �� ���� �Ķ���͸� �����մϴ�. // Set the parameters to be used for detection.
		// ���� �� ���� �⺻ ������ �����մϴ�. // Set the default angle to be used for detection.
		FLDrawingMatch.SetAngleBias(0.);
		// ���� �� ���� ������ Ž�������� �����մϴ�. // Set the search range of the angle to be used for detection.
		// ������ �⺻ ������ �������� (�⺻ ���� - AngleTolerance, �⺻ ���� + AngleTolerance)�� ���� Ž������ // The angle is based on the basic angle (default angle - AngleTolerance, basic angle + AngleTolerance) is the final search range
		FLDrawingMatch.SetAngleTolerance(5.);
		// ���� �� ���� ������ Ž�������� �����մϴ�. // Set the scale search range to be used for detection.
		FLDrawingMatch.SetScaleRange(0.9, 1.1);
		// ���� �� ���� �ּ� Ž�������� �����մϴ�. // Set the minimum search score to be used for detection.
		FLDrawingMatch.SetMinimumDetectionScore(0.5);
		// ���� �� ���� �ִ� Ž����ü ���� �����մϴ�. // Set the maximum number of search objects to be used for detection.
		FLDrawingMatch.SetMaxObject(1);

		// ���� �� ������ ��� ������ ���� �����մϴ�. // Set whether to use interpolation when detecting.
		FLDrawingMatch.EnableInterpolation();
		// ���� �� ����ȭ ������ ���� �����մϴ�. // Set the degree of optimization for detection.
		FLDrawingMatch.SetOptimizationOption();
		// ���� �� ��������� ���� �����մϴ�. // Set the contrast level for detection.
		FLDrawingMatch.SetContrastOption();
		// ���� �� �̹��� �������� Ž�� ������ �����մϴ�. // Set the degree of search outside the image area when detecting.
		FLDrawingMatch.SetContrastOption(EMatchContrastOption_Any);
		// ���� �� ó������������ ��� �Ӱ谪�� �����մϴ�. // Set the allowable threshold in the process of detection.
		FLDrawingMatch.SetFindThresholdCoeff();
		// ���� �� ������ ��� ������ �����մϴ�. // Set the allowable degree of overlap during detection.
		FLDrawingMatch.SetObjectOverlap();
		// ���� �� �̹��� ��ó�� ������ �����մϴ�. // Set whether or not to pre-process the image during detection.

		// �˰��� ���� // Execute the algorithm
		res = FLDrawingMatch.Execute();

		if(IsFail(res))
		{
			ErrorPrint(res, "Failed to Execute\n");
			break;
		}


		// �������� ���� ���� ����� �����ɴϴ�. // Get the geometric pattern detection result.
		std::vector<CDrawingMatch::SResult> vctResults;
		int64_t i64ResultCount = FLDrawingMatch.GetResultCount();

		printf(" �� Find Information\n");

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			CDrawingMatch::SResult results;
			FLDrawingMatch.GetResult(i, results);

			bool bInverse = results.bInverse;
			float f32Score = results.f32Score;
			float f32Angle = results.f32Angle;
			float f32Scale = results.f32Scale;
			Foundation::CFLFigure* pFlfRegion = results.pFlfRegion;
			Foundation::CFLPoint<double> flpLocation = results.flpLocation;
			Foundation::CFLPoint<double> flpPivot = results.flpPivot;

			CFLString<wchar_t> strResult;
			CFLString<wchar_t> strInverse = bInverse ? L"Inverse Type" : L"Normal Type";

			CFLRect<double> flrResultRegion = pFlfRegion->GetBoundaryRect();

			// �������� ���� ���� ����� Consoleâ�� ����մϴ�. // Output the geometric pattern detection result to the console window.
			printf(" < Instance : %lld >\n", i);
			printf("  1. ROI Shape Type : Rectangle\n");
			printf("    left   : %.3lf\n", flrResultRegion.left);
			printf("    right  : %.3lf\n", flrResultRegion.right);
			printf("    top    : %.3lf\n", flrResultRegion.top);
			printf("    bottom : %.3lf\n", flrResultRegion.bottom);
			printf("    angle  : %.3lf\n", f32Angle);
			printf("  2. Interest Pivot : (%.3lf, %.3lf)\n", flpPivot.x, flpPivot.y);
			printf("  3. Score : %0.3lf\n  4. Angle : %0.3lf\n  5. Scale : %0.3lf\n", f32Score, f32Angle, f32Scale);

			if(bInverse)
				printf("  6. Contrast : Inverse Type\n");
			else
				printf("  6. Contrast : Normal Type\n");

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

			if(IsFail(res = layerFind.DrawFigureImage(pFlfRegion, BLACK, 3)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsFail(res = layerFind.DrawFigureImage(pFlfRegion, CYAN)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			TPoint<double> tpPosition;
			tpPosition.x = flpPivot.x;
			tpPosition.y = flpPivot.y;

			CFLString<wchar_t> strText;
			strText.Format(L"Score : %.3lf\nAngle : %.3lf\nScale : x%.3lf\n", f32Score, f32Angle, f32Scale);

			if((res = layerFind.DrawTextImage(tpPosition, strText, YELLOW, BLACK, 15, false, 0, EGUIViewImageTextAlignment_LEFT_CENTER)).IsFail())
			{
				ErrorPrint(res, "Failed to draw text");
				break;
			}
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImageLearn.ZoomFitToLayer(0);
		viewImageLearn.Invalidate(true);
		viewImageFind.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageFind.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}