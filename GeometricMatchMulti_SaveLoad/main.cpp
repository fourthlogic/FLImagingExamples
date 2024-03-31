#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliLearnImage[3];
	CFLImage fliFindImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn[3];
	CGUIViewImageWrap viewImageFind;

	CResult res = EResult_UnknownError;

	// Geometric Match Multi ��ü ���� // Create Geometric Match Multi object
	CGeometricMatchMulti FLGeometricMatchMultiSave;
	CGeometricMatchMulti FLGeometricMatchMultiLoad;

	do
	{
		CFLString<wchar_t> arrPath[3] = {L"../../ExampleImages/Matching/Geometric Mult Learn_01.flif",
			L"../../ExampleImages/Matching/Geometric Mult Learn_02.flif",
			L"../../ExampleImages/Matching/Geometric Mult Learn_03.flif" };

		CFLString<wchar_t> arrClassName[3] = {L"A", L"B", L"C"};
		uint32_t arrColor[3] = {LIME, RED, CYAN};

		printf(" �� Learn Information\n");

		for(int64_t i64DataIdx = 0; i64DataIdx < 3; ++i64DataIdx)
		{
			// �̹��� �ε� // Loads image
			if(IsFail(res = fliLearnImage[i64DataIdx].Load(arrPath[i64DataIdx].GetBuffer())))
			{
				ErrorPrint(res, "Failed to load the image file.\n");
				break;
			}

			// �̹��� �� ���� // Creates imageview
			if(IsFail(res = viewImageLearn[i64DataIdx].Create((int32_t)(400 + 512 * i64DataIdx), 0, (int32_t)(400 + 512 * (i64DataIdx + 1)), 384)))
			{
				ErrorPrint(res, "Failed to create the image view.\n");
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
			if(IsFail(res = viewImageLearn[i64DataIdx].SetImagePtr(&fliLearnImage[i64DataIdx])))
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				break;
			}

			CGUIViewImageLayerWrap layerLearn = viewImageLearn[i64DataIdx].GetLayer(0);

			layerLearn.Clear();

			// �н��� �̹��� ���� // Set the image to learn
			FLGeometricMatchMultiSave.SetLearnImage(fliLearnImage[i64DataIdx]);

			// �н��� ������ �����մϴ�. // Set the area to learn.
			CFLRectD learnRegion;

			if(i64DataIdx == 0)
				learnRegion.Set(33.700864, 230.805616, 213.474082, 407.099352);
			else if(i64DataIdx == 1)
				learnRegion.Set(370.366091, 482.671707, 470.402807, 575.431965);
			else if(i64DataIdx == 2)
				learnRegion.Set(363.564795, 344.259179, 486.333693, 430.323974);

			CFLPointD flpLearnPivot = learnRegion.GetCenter();
			FLGeometricMatchMultiSave.SetLearnROI(learnRegion);
			FLGeometricMatchMultiSave.SetLearnPivot(flpLearnPivot);

			// �н� �Ķ���͸� �����մϴ�. // Set the learning parameters.
			// ������ Ư¡�� ������ �����մϴ�. // Set the number of feature points to be extracted.
			FLGeometricMatchMultiSave.SetFeatureCount();
			// ������ Ư¡�� ó������������ ������ ���͸� ������ �����մϴ�. // Set the noise filtering degree in the process of processing the feature points to be extracted.
			FLGeometricMatchMultiSave.SetFeatureFiltering();
			// ������ Ư¡�� ó������������ ��� �Ӱ谪�� �����մϴ�. // Set the allowable threshold in the feature point processing process to be extracted.
			FLGeometricMatchMultiSave.SetLearnThresholdCoeff();
			// ������ Ư¡�� ó������������ �̹��� ��ó�� ������ �����մϴ�. // Set whether or not to pre-process the image in the process of processing the feature points to be extracted.
			FLGeometricMatchMultiSave.EnablePreprocessing();


			// �˰��� ���� // Execute the algorithm
			if(IsFail(res = FLGeometricMatchMultiSave.Learn(arrClassName[i64DataIdx])))
			{
				ErrorPrint(res, "Failed to execute Learn.\n");
				break;
			}

			// ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the measurement area is
			// FLImaging�� Figure��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ���� // Figure objects of FLImaging can be displayed with a single function regardless of any shape
			if(IsFail(res = layerLearn.DrawFigureImage(&learnRegion, BLACK, 3)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsFail(res = layerLearn.DrawFigureImage(&learnRegion, arrColor[i64DataIdx])))
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


			// �н��� Ư¡���� ���÷��� �Ѵ� // Display the learned feature point
			// �н��� Ư¡���� ã�����ϴ� ��ü�� ��Ÿ���⿡ ����ϰ� �� �������� Ȯ���ϰ�, �׷��� �ʴٸ� �н� �Ķ���͸� �����������ν� ��Ȯ���ϸ� ���� �� �� ȿ�����Դϴ�. // Check whether the learned feature points are selected well enough to represent the object to be found.
			CFLFigureArray flfaFeaturePoints;
			FLGeometricMatchMultiSave.GetLearntFeature(flfaFeaturePoints);

			for(int32_t i32Index = 0; i32Index < (int32_t)flfaFeaturePoints.GetCount(); ++i32Index)
			{
				if(flfaFeaturePoints.GetAt(i32Index)->GetDeclType() != EFigureDeclType_Point)
					break;

				CFLPointD* pFlp = static_cast<CFLPointD*>(flfaFeaturePoints.GetAt(i32Index));

				if(pFlp)
				{
					if(IsFail(res = layerLearn.DrawFigureImage(pFlp, arrColor[i64DataIdx])))
					{
						ErrorPrint(res, "Failed to draw figure\n");
						break;
					}
				}
			}
		
			CFLString<wchar_t> strStatus;
			strStatus.Format(L"LEARN CLASS '%s'", arrClassName[i64DataIdx].GetBuffer());

			if(IsFail(res = layerLearn.DrawTextCanvas(&CFLPointD(0, 0), strStatus.GetBuffer(), YELLOW, BLACK, 30)))
			{
				ErrorPrint(res, "Failed to draw text\n");
				break;
			}

			// �н��� ������ ���� Consoleâ�� ����Ѵ� // Print the learned information to the console window
			printf("  < LEARN CLASS '%ws' > \n", arrClassName[i64DataIdx].GetBuffer());
			printf("  1. ROI Shape Type : Rectangle\n");
			printf("    left   : %.3lf\n", learnRegion.left);
			printf("    right  : %.3lf\n", learnRegion.right);
			printf("    top    : %.3lf\n", learnRegion.top);
			printf("    bottom : %.3lf\n", learnRegion.bottom);
			printf("    angle  : %.3lf\n", learnRegion.angle);
			printf("  2. Interest Pivot : (%.3lf, %.3lf)\n", flpLearnPivot.x, flpLearnPivot.y);
			printf("\n");
			
			// �̹��� �並 ���� �մϴ�. // Update the image view.
			viewImageLearn[i64DataIdx].Invalidate(true);
		}

		// ������ �߰��� �Ϸ� �� Save�� �����մϴ�. // After completing data addition, proceed with Save.
		if(IsFail(res = FLGeometricMatchMultiSave.Save(L"../../ExampleImages/Matching/Geometric Multi Learn File")))
		{
			ErrorPrint(res, "Failed to save\n");
			break;
		}

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliFindImage.Load(L"../../ExampleImages/Matching/Geometric Mult Find.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}


		// �̹��� �� ���� // Creates imageview
		if(IsFail(res = viewImageFind.Create(400, 384, 1168, 960)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(res = viewImageFind.SetImagePtr(&fliFindImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		for(int64_t i64DataIdx = 0 ; i64DataIdx < 3; ++i64DataIdx)
		{
			// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
			if(IsFail(res = viewImageFind.SynchronizeWindow(&viewImageLearn[i64DataIdx])))
			{
				ErrorPrint(res, "Failed to synchronize window.\n");
				break;
			}
		}

		CGUIViewImageLayerWrap layerFind = viewImageFind.GetLayer(1);
		layerFind.Clear();

		if(IsFail(res = layerFind.DrawTextCanvas(&CFLPointD(0, 0), L"FIND", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �����͸� Load�մϴ�. // Load data.
		if(IsFail(res = FLGeometricMatchMultiLoad.Load(L"../../ExampleImages/Matching/Geometric Multi Learn File")))
		{
			ErrorPrint(res, "Failed to load\n");
			break;
		}

		// ������ �̹��� ���� // Set image to detect
		FLGeometricMatchMultiLoad.SetSourceImage(fliFindImage);

		// ���� �� ���� �Ķ���͸� �����մϴ�. // Set the parameters to be used for detection.
		// ���� �� ���� �⺻ ������ �����մϴ�. // Set the default angle to be used for detection.
		FLGeometricMatchMultiLoad.SetAngleBias(0.);
		// ���� �� ���� ������ Ž�������� �����մϴ�. // Set the search range of the angle to be used for detection.
		// ������ �⺻ ������ �������� (�⺻ ���� - AngleTolerance, �⺻ ���� + AngleTolerance)�� ���� Ž������ // The angle is based on the basic angle (default angle - AngleTolerance, basic angle + AngleTolerance) is the final search range
		FLGeometricMatchMultiLoad.SetAngleTolerance(180.);
		// ���� �� ���� ������ Ž�������� �����մϴ�. // Set the scale search range to be used for detection.
		FLGeometricMatchMultiLoad.SetScaleRange(0.98, 1.02);
		// ���� �� ���� �ּ� Ž�������� �����մϴ�. // Set the minimum search score to be used for detection.
		FLGeometricMatchMultiLoad.SetMinimumDetectionScore(0.5);
		// ���� �� ���� Ž�� ����� �����մϴ�. // Set the search method to be used for detection.
		FLGeometricMatchMultiLoad.SetMaxObjectMode(CGeometricMatchMulti::EMaxObjectMode_Total);
		// ���� �� ���� �ִ� Ž����ü ���� �����մϴ�. // Set the maximum number of search objects to be used for detection.
		FLGeometricMatchMultiLoad.SetMaxObjectTotal(16);

		// ���� �� ������ ��� ������ ���� �����մϴ�. // Set whether to use interpolation when detecting.
		FLGeometricMatchMultiLoad.EnableInterpolation();
		// ���� �� ����ȭ ������ ���� �����մϴ�. // Set the degree of optimization for detection.
		FLGeometricMatchMultiLoad.SetOptimizationOption();
		// ���� �� ��������� ���� �����մϴ�. // Set the contrast level for detection.
		FLGeometricMatchMultiLoad.SetConstrastOption();
		// ���� �� �̹��� �������� Ž�� ������ �����մϴ�. // Set the degree of search outside the image area when detecting.
		FLGeometricMatchMultiLoad.SetInvisibleRegionEstimation();
		// ���� �� ó������������ ��� �Ӱ谪�� �����մϴ�. // Set the allowable threshold in the process of detection.
		FLGeometricMatchMultiLoad.SetFindThresholdCoeff();
		// ���� �� ������ ��� ������ �����մϴ�. // Set the allowable degree of overlap during detection.
		FLGeometricMatchMultiLoad.SetObjectOverlap();
		// ���� �� �̹��� ��ó�� ������ �����մϴ�. // Set whether or not to pre-process the image during detection.
		// �н��������� ��ó�� ������ ������ �����ϰ� �����ϴ� ���� ��õ�մϴ�. // In the learning process, it is recommended to set the pre-processing status the same as the setting.
		FLGeometricMatchMultiLoad.EnablePreprocessing();

		// �˰��� ���� // Execute the algorithm
		res = FLGeometricMatchMultiLoad.Execute();

		if(IsFail(res))
		{
			ErrorPrint(res, "Failed to execute\n");
			break;
		}

		// �������� ���� ���� ����� �����ɴϴ�. // Get the geometric pattern detection result.
		int64_t i64ResultCount = FLGeometricMatchMultiLoad.GetResultCount();

		printf(" �� Find Information\n");

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			CGeometricMatchMulti::SResult results;
			CFLFigureArray flfaResultPoints;

			FLGeometricMatchMultiLoad.GetResult(i, results);
			FLGeometricMatchMultiLoad.GetResultForDetectedFeature(i, flfaResultPoints);

			bool bInverse = results.bInverse;
			float f32Score = results.f32Score;
			float f32Angle = results.f32Angle;
			float f32Scale = results.f32Scale;
			Foundation::CFLFigure* pFlfRegion = results.pFlfRegion;
			Foundation::CFLPointD flpLocation = results.flpLocation;
			Foundation::CFLPointD flpPivot = results.flpPivot;

			CFLString<wchar_t> strResult;
			CFLString<wchar_t> strInverse = bInverse ? L"Inverse Type" : L"Normal Type";
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
			
			CFLRectD* pFlrResultRegion = dynamic_cast<CFLRectD*>(pFlfRegion);

			if(!pFlrResultRegion)
				break;

			// �������� ���� ���� ����� Consoleâ�� ����մϴ�. // Output the geometric pattern detection result to the console window.
			printf(" < Instance : %lld >\n", i);
			printf(" Class Name : %ws\n", wstrClassName.GetBuffer());
			printf("  1. ROI Shape Type : Rectangle\n");
			printf("    left   : %.3lf\n", pFlrResultRegion->left);
			printf("    right  : %.3lf\n", pFlrResultRegion->right);
			printf("    top    : %.3lf\n", pFlrResultRegion->top);
			printf("    bottom : %.3lf\n", pFlrResultRegion->bottom);
			printf("    angle  : %.3lf\n", pFlrResultRegion->angle);
			printf("  2. Interest Pivot : (%.3lf, %.3lf)\n", flpPivot.x, flpPivot.y);
			printf("  3. Score : %0.3lf\n  4. Angle : %0.3lf\n  5. Scale : %0.3lf\n", f32Score, f32Angle, f32Scale);

			if(bInverse)
				printf(" Inverse Type\n");
			else
				printf(" Normal Type\n");

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

			// ��� Ư¡���� ���÷��� �Ѵ� // Display the resulting feature point
			for(int32_t i32Index = 0; i32Index < (int32_t)flfaResultPoints.GetCount(); ++i32Index)
			{
				if(flfaResultPoints.GetAt(i32Index)->GetDeclType() != EFigureDeclType_Point)
					break;

				CFLPointD* pFlp = static_cast<CFLPointD*>(flfaResultPoints.GetAt(i32Index));

				if(pFlp)
				{
					if(IsFail(res = layerFind.DrawFigureImage(pFlp, arrColor[i64Idx])))
					{
						ErrorPrint(res, "Failed to draw figure\n");
						break;
					}
				}
			}


			TPoint<double> tpPosition;
			tpPosition.x = flpPivot.x;
			tpPosition.y = flpPivot.y;

			if(IsFail(res = layerFind.DrawTextImage(&flpPivot, wstrClassName.GetBuffer(), YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_CENTER, nullptr, 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
			{
				ErrorPrint(res, "Failed to draw text\n");
				break;
			}

			tpPosition.x += 10;
			CFLString<wchar_t> strText;
			strText.Format(L"Score : %.3lf\nAngle : %.3lf\nScale : x%.3lf\n", f32Score, f32Angle, f32Scale);

			if((res = layerFind.DrawTextImage(tpPosition, strText, YELLOW, BLACK, 15, false, 0, EGUIViewImageTextAlignment_LEFT_CENTER)).IsFail())
			{
				ErrorPrint(res, "Failed to draw text");
				break;
			}
		}

		viewImageFind.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageFind.IsAvailable() && viewImageLearn[0].IsAvailable() && viewImageLearn[1].IsAvailable() && viewImageLearn[2].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}