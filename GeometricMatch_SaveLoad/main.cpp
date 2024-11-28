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
		if(IsFail(res = fliLearnImage.Load(L"../../ExampleImages/Matching/Geometric Single Learn.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliFindImage.Load(L"../../ExampleImages/Matching/Geometric Single Find.flif")))
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

		// Geometric Match ��ü ���� // Create Geometric Match object
		CGeometricMatch FLGeometricMatchSave;

		// �н��� �̹��� ���� // Set the image to learn
		FLGeometricMatchSave.SetLearnImage(fliLearnImage);

		// �н��� ������ �����մϴ�. // Set the area to learn.
		CFLRect<double> learnRegion(40.079914, 227.326134, 212.894168, 408.259179);
		CFLPoint<double> flpLearnPivot = learnRegion.GetCenter();
		FLGeometricMatchSave.SetLearnROI(learnRegion);
		FLGeometricMatchSave.SetLearnPivot(flpLearnPivot);

		// �н� �Ķ���͸� �����մϴ�. // Set the learning parameters.
		// ������ Ư¡�� ������ �����մϴ�. // Set the number of feature points to be extracted.
		FLGeometricMatchSave.SetFeatureCount();
		// ������ Ư¡�� ó������������ ������ ���͸� ������ �����մϴ�. // Set the noise filtering degree in the process of processing the feature points to be extracted.
		FLGeometricMatchSave.SetFeatureFiltering();
		// ������ Ư¡�� ó������������ ��� �Ӱ谪�� �����մϴ�. // Set the allowable threshold in the feature point processing process to be extracted.
		FLGeometricMatchSave.SetLearnThresholdCoeff();

		//�˰��� ���� // Execute the algorithm
		if(IsFail(res = FLGeometricMatchSave.Learn()))
		{
			ErrorPrint(res, "Failed to execute Learn.\n");
			break;
		}

		// ������ Save�� �����մϴ�. // Proceed to save data.
		if(IsFail(res = FLGeometricMatchSave.Save(L"../../ExampleImages/Matching/Geometric Single Learn File")))
		{
			ErrorPrint(res, "Failed to save\n");
			break;
		}

		// �н� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to see where the learning area is
		if(IsFail(res = layerLearn.DrawFigureImage(&learnRegion, BLACK, 3)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerLearn.DrawFigureImage(&learnRegion, CYAN)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// ������ �߽����� ��ġ�� ���÷��� �Ѵ� // Display the position of the set center point
		CFLFigureArray flfaPoint = flpLearnPivot.MakeCrossHair(3, false);

		if(IsFail(res = layerLearn.DrawFigureImage(&flfaPoint, BLACK, 3)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		if(IsFail(res = layerLearn.DrawFigureImage(&flfaPoint, LIME)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		// �н��� Ư¡���� ���÷��� �Ѵ� // Display the learned feature point
		// �н��� Ư¡���� ã�����ϴ� ��ü�� ��Ÿ���⿡ ����ϰ� �� �������� Ȯ���ϰ�, �׷��� �ʴٸ� �н� �Ķ���͸� �����������ν� ��Ȯ���ϸ� ���� �� �� ȿ�����Դϴ�. // Check whether the learned feature points are selected well enough to represent the object to be found.
		CFLFigureArray flfaFeaturePoints;
		
		if(IsFail(res = FLGeometricMatchSave.GetLearntFeature(flfaFeaturePoints)))
		{
			ErrorPrint(res, "Failed to get learnt features.\n");
			break;
		}

		for(int32_t i32Index = 0; i32Index < (int32_t)flfaFeaturePoints.GetCount(); ++i32Index)
		{
			if(flfaFeaturePoints.GetAt(i32Index)->GetDeclType() != EFigureDeclType_Point)
				break;

			CFLPoint<double>* pFlp = static_cast<CFLPoint<double>*>(flfaFeaturePoints.GetAt(i32Index));

			if(pFlp)
			{
				if(IsFail(res = layerLearn.DrawFigureImage(pFlp, BLUE)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}
			}
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

		CGeometricMatch FLGeometricMatchLoad;

		// �����͸� Load�մϴ�. // Load data.
		if(IsFail(res = FLGeometricMatchLoad.Load(L"../../ExampleImages/Matching/Geometric Single Learn File")))
		{
			ErrorPrint(res, "Failed to load\n");
			break;
		}

		// ������ �̹��� ���� // Set image to detect
		FLGeometricMatchLoad.SetSourceImage(fliFindImage);

		// ���� �� ���� �Ķ���͸� �����մϴ�. // Set the parameters to be used for detection.
		// ���� �� ���� �⺻ ������ �����մϴ�. // Set the default angle to be used for detection.
		FLGeometricMatchLoad.SetAngleBias(0.);
		// ���� �� ���� ������ Ž�������� �����մϴ�. // Set the search range of the angle to be used for detection.
		// ������ �⺻ ������ �������� (�⺻ ���� - AngleTolerance, �⺻ ���� + AngleTolerance)�� ���� Ž������ // The angle is based on the basic angle (default angle - AngleTolerance, basic angle + AngleTolerance) is the final search range
		FLGeometricMatchLoad.SetAngleTolerance(180.);
		// ���� �� ���� ������ Ž�������� �����մϴ�. // Set the scale search range to be used for detection.
		FLGeometricMatchLoad.SetScaleRange(0.98, 1.02);
		// ���� �� ���� �ּ� Ž�������� �����մϴ�. // Set the minimum search score to be used for detection.
		FLGeometricMatchLoad.SetMinimumDetectionScore(0.7);
		// ���� �� ���� �ִ� Ž����ü ���� �����մϴ�. // Set the maximum number of search objects to be used for detection.
		FLGeometricMatchLoad.SetMaxObject(5);

		// ���� �� ������ ��� ������ ���� �����մϴ�. // Set whether to use interpolation when detecting.
		FLGeometricMatchLoad.EnableInterpolation();
		// ���� �� ����ȭ ������ ���� �����մϴ�. // Set the degree of optimization for detection.
		FLGeometricMatchLoad.SetOptimizationOption();
		// ���� �� ��������� ���� �����մϴ�. // Set the contrast level for detection.
		FLGeometricMatchLoad.SetContrastOption();
		// ���� �� �̹��� �������� Ž�� ������ �����մϴ�. // Set the degree of search outside the image area when detecting.
		FLGeometricMatchLoad.SetInvisibleRegionEstimation();
		// ���� �� ó������������ ��� �Ӱ谪�� �����մϴ�. // Set the allowable threshold in the process of detection.
		FLGeometricMatchLoad.SetFindThresholdCoeff();
		// ���� �� ������ ��� ������ �����մϴ�. // Set the allowable degree of overlap during detection.
		FLGeometricMatchLoad.SetObjectOverlap();
		// ���� �� �̹��� ��ó�� ������ �����մϴ�. // Set whether or not to pre-process the image during detection.

		// �˰��� ���� // Execute the algorithm
		res = FLGeometricMatchLoad.Execute();

		if(IsFail(res))
		{
			ErrorPrint(res, "Failed to execute\n");
			break;
		}


		// �������� ���� ���� ����� �����ɴϴ�. // Get the geometric pattern detection result.
		std::vector<CGeometricMatch::SResult> vctResults;
		int64_t i64ResultCount = FLGeometricMatchLoad.GetResultCount();

		printf(" �� Find Information\n");

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			CGeometricMatch::SResult results;
			CFLFigureArray flfaResultPoints;

			FLGeometricMatchLoad.GetResult(i, results);
			FLGeometricMatchLoad.GetResultForDetectedFeature(i, flfaResultPoints);

			float f32Score = results.f32Score;
			float f32Angle = results.f32Angle;
			float f32Scale = results.f32Scale;
			Foundation::CFLFigure* pFlfRegion = results.pFlfRegion;
			Foundation::CFLPoint<double> flpLocation = results.flpLocation;
			Foundation::CFLPoint<double> flpPivot = results.flpPivot;

			CFLString<wchar_t> strResult;
			
			CFLRect<double>* pFlrResultRegion = dynamic_cast<CFLRect<double>*>(pFlfRegion);

			if(!pFlrResultRegion)
				break;

			// �������� ���� ���� ����� Consoleâ�� ����մϴ�. // Output the geometric pattern detection result to the console window.
			printf(" < Instance : %lld >\n", i);
			printf("  1. ROI Shape Type : Rectangle\n");
			printf("    left   : %.3lf\n", pFlrResultRegion->left);
			printf("    right  : %.3lf\n", pFlrResultRegion->right);
			printf("    top    : %.3lf\n", pFlrResultRegion->top);
			printf("    bottom : %.3lf\n", pFlrResultRegion->bottom);
			printf("    angle  : %.3lf\n", f32Angle);
			printf("  2. Interest Pivot : (%.3lf, %.3lf)\n", flpPivot.x, flpPivot.y);
			printf("  3. Score : %0.3lf\n  4. Angle : %0.3lf\n  5. Scale : %0.3lf\n", f32Score, f32Angle, f32Scale);

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

				CFLPoint<double>* pFlp = static_cast<CFLPoint<double>*>(flfaResultPoints.GetAt(i32Index));

				if(pFlp)
				{
					if(IsFail(res = layerFind.DrawFigureImage(pFlp, LIME)))
					{
						ErrorPrint(res, "Failed to draw figure\n");
						break;
					}
				}
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
		viewImageLearn.Invalidate(true);
		viewImageFind.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageFind.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}