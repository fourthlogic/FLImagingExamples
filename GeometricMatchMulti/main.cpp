#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliLearnImage[3];
	CFLImage fliFindImage;

	// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImageLearn[3];
	CGUIViewImageWrap viewImageFind;

	CResult eResult = EResult_UnknownError;

	// Geometric Match Multi 객체 생성 // Create Geometric Match Multi object
	CGeometricMatchMulti FLGeometricMatchMulti;

	do
	{
		CFLString<wchar_t> arrPath[3] = {L"../../ExampleImages/Matching/Geometric Mult Learn_01.flif",
			L"../../ExampleImages/Matching/Geometric Mult Learn_02.flif",
			L"../../ExampleImages/Matching/Geometric Mult Learn_03.flif" };

		CFLString<wchar_t> arrClassName[3] = {L"A", L"B", L"C"};
		uint32_t arrColor[3] = {LIME, RED, CYAN};

		printf(" ▷ Learn Information\n");

		for(int64_t i64DataIdx = 0; i64DataIdx < 3; ++i64DataIdx)
		{
			// 이미지 로드 // Loads image
			if(IsFail(eResult = fliLearnImage[i64DataIdx].Load(arrPath[i64DataIdx].GetBuffer())))
			{
				ErrorPrint(eResult, "Failed to load the image file.\n");
				break;
			}

			// 이미지 뷰 생성 // Creates imageview
			if(IsFail(eResult = viewImageLearn[i64DataIdx].Create((int32_t)(400 + 512 * i64DataIdx), 0, (int32_t)(400 + 512 * (i64DataIdx + 1)), 384)))
			{
				ErrorPrint(eResult, "Failed to create the image view.\n");
				break;
			}

			// 이미지 뷰에 이미지를 디스플레이 // Display the image in the imageview
			if(IsFail(eResult = viewImageLearn[i64DataIdx].SetImagePtr(&fliLearnImage[i64DataIdx])))
			{
				ErrorPrint(eResult, "Failed to set image object on the image view.\n");
				break;
			}

			CGUIViewImageLayerWrap layerLearn = viewImageLearn[i64DataIdx].GetLayer(0);

			layerLearn.Clear();

			// 학습할 이미지 설정 // Set the image to learn
			FLGeometricMatchMulti.SetLearnImage(fliLearnImage[i64DataIdx]);

			// 학습할 영역을 설정합니다. // Set the area to learn.
			CFLRectD learnRegion;

			if(i64DataIdx == 0)
				learnRegion.Set(33.700864, 230.805616, 213.474082, 407.099352);
			else if(i64DataIdx == 1)
				learnRegion.Set(370.366091, 482.671707, 470.402807, 575.431965);
			else if(i64DataIdx == 2)
				learnRegion.Set(363.564795, 344.259179, 486.333693, 430.323974);

			CFLPointD flpLearnPivot = learnRegion.GetCenter();
			FLGeometricMatchMulti.SetLearnROI(learnRegion);
			FLGeometricMatchMulti.SetLearnPivot(flpLearnPivot);

			// 학습 파라미터를 설정합니다. // Set the learning parameters.
			// 추출할 특징점 개수를 설정합니다. // Set the number of feature points to be extracted.
			FLGeometricMatchMulti.SetFeatureCount();
			// 추출할 특징점 처리과정에서의 노이즈 필터링 정도를 설정합니다. // Set the noise filtering degree in the process of processing the feature points to be extracted.
			FLGeometricMatchMulti.SetFeatureFiltering();
			// 추출할 특징점 처리과정에서의 허용 임계값을 설정합니다. // Set the allowable threshold in the feature point processing process to be extracted.
			FLGeometricMatchMulti.SetLearnThresholdCoeff();
			// 추출할 특징점 처리과정에서의 이미지 전처리 유무를 설정합니다. // Set whether or not to pre-process the image in the process of processing the feature points to be extracted.
			FLGeometricMatchMulti.EnablePreprocessing();


			// 알고리즘 수행 // Execute the algorithm
			if(IsFail(eResult = FLGeometricMatchMulti.Learn(arrClassName[i64DataIdx])))
			{
				ErrorPrint(eResult, "Failed to execute Learn.\n");
				break;
			}

			// 측정 영역이 어디인지 알기 위해 디스플레이 한다 // Display to know where the measurement area is
			// FLImaging의 Figure객체들은 어떤 도형모양이든 상관없이 하나의 함수로 디스플레이가 가능 // Figure objects of FLImaging can be displayed with a single function regardless of any shape
			if(IsFail(eResult = layerLearn.DrawFigureImage(&learnRegion, BLACK, 3)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			if(IsFail(eResult = layerLearn.DrawFigureImage(&learnRegion, arrColor[i64DataIdx])))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			// 설정된 중심점의 위치를 디스플레이 한다 // Display the position of the set center point
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


			// 학습한 특징점을 디스플레이 한다 // Display the learned feature point
			// 학습한 특징점이 찾고자하는 객체를 나타내기에 충분하게 잘 뽑혔는지 확인하고, 그렇지 않다면 학습 파라미터를 재조정함으로써 재확인하면 검출 시 더 효과적입니다. // Check whether the learned feature points are selected well enough to represent the object to be found.
			CFLFigureArray flfaFeaturePoints;
			FLGeometricMatchMulti.GetLearntFeature(flfaFeaturePoints);

			for(int32_t i32Index = 0; i32Index < (int32_t)flfaFeaturePoints.GetCount(); ++i32Index)
			{
				if(flfaFeaturePoints.GetAt(i32Index)->GetDeclType() != EFigureDeclType_Point)
					break;

				CFLPointD* pFlp = static_cast<CFLPointD*>(flfaFeaturePoints.GetAt(i32Index));

				if(pFlp)
				{
					if(IsFail(eResult = layerLearn.DrawFigureImage(pFlp, arrColor[i64DataIdx])))
					{
						ErrorPrint(eResult, "Failed to draw figure\n");
						break;
					}
				}
			}
		
			CFLString<wchar_t> strStatus;
			strStatus.Format(L"LEARN CLASS '%s'", arrClassName[i64DataIdx].GetBuffer());

			if(IsFail(eResult = layerLearn.DrawTextCanvas(&CFLPointD(0, 0), strStatus.GetBuffer(), YELLOW, BLACK, 30)))
			{
				ErrorPrint(eResult, "Failed to draw text\n");
				break;
			}

			// 학습한 정보에 대해 Console창에 출력한다 // Print the learned information to the console window
			printf("  < LEARN CLASS '%ws' > \n", arrClassName[i64DataIdx].GetBuffer());
			printf("  1. ROI Shape Type : Rectangle\n");
			printf("    left   : %.3lf\n", learnRegion.left);
			printf("    right  : %.3lf\n", learnRegion.right);
			printf("    top    : %.3lf\n", learnRegion.top);
			printf("    bottom : %.3lf\n", learnRegion.bottom);
			printf("    angle  : %.3lf\n", learnRegion.angle);
			printf("  2. Interest Pivot : (%.3lf, %.3lf)\n", flpLearnPivot.x, flpLearnPivot.y);
			printf("\n");
			
			// 이미지 뷰를 갱신 합니다. // Update the image view.
			viewImageLearn[i64DataIdx].Invalidate(true);
		}
		
		// 이미지 로드 // Loads image
		if(IsFail(eResult = fliFindImage.Load(L"../../ExampleImages/Matching/Geometric Mult Find.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}


		// 이미지 뷰 생성 // Creates imageview
		if(IsFail(eResult = viewImageFind.Create(400, 384, 1168, 960)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the imageview
		if(IsFail(eResult = viewImageFind.SetImagePtr(&fliFindImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		for(int64_t i64DataIdx = 0 ; i64DataIdx < 3; ++i64DataIdx)
		{
			// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
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
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// 검출할 이미지 설정 // Set image to detect
		FLGeometricMatchMulti.SetSourceImage(fliFindImage);

		// 검출 시 사용될 파라미터를 설정합니다. // Set the parameters to be used for detection.
		// 검출 시 사용될 기본 각도를 설정합니다. // Set the default angle to be used for detection.
		FLGeometricMatchMulti.SetAngleBias(0.); 
		// 검출 시 사용될 각도의 탐색범위를 설정합니다. // Set the search range of the angle to be used for detection.
		// 각도는 기본 각도를 기준으로 (기본 각도 - AngleTolerance, 기본 각도 + AngleTolerance)가 최종 탐색범위 // The angle is based on the basic angle (default angle - AngleTolerance, basic angle + AngleTolerance) is the final search range
		FLGeometricMatchMulti.SetAngleTolerance(180.);
		// 검출 시 사용될 스케일 탐색범위를 설정합니다. // Set the scale search range to be used for detection.
		FLGeometricMatchMulti.SetScaleRange(0.98, 1.02);
		// 검출 시 사용될 최소 탐색점수를 설정합니다. // Set the minimum search score to be used for detection.
		FLGeometricMatchMulti.SetMinimumDetectionScore(0.5);
		// 검출 시 사용될 탐색 방식을 설정합니다. // Set the search method to be used for detection.
		FLGeometricMatchMulti.SetMaxObjectMode(CGeometricMatchMulti::EMaxObjectMode_Total);
		// 검출 시 사용될 최대 탐색객체 수를 설정합니다. // Set the maximum number of search objects to be used for detection.
		FLGeometricMatchMulti.SetMaxObjectTotal(16);

		// 검출 시 보간법 사용 유무에 대해 설정합니다. // Set whether to use interpolation when detecting.
		FLGeometricMatchMulti.EnableInterpolation();
		// 검출 시 최적화 정도에 대해 설정합니다. // Set the degree of optimization for detection.
		FLGeometricMatchMulti.SetOptimizationOption();
		// 검출 시 대비정도에 대해 설정합니다. // Set the contrast level for detection.
		FLGeometricMatchMulti.SetConstrastOption();
		// 검출 시 이미지 영역밖의 탐색 정도를 설정합니다. // Set the degree of search outside the image area when detecting.
		FLGeometricMatchMulti.SetInvisibleRegionEstimation();
		// 검출 시 처리과정에서의 허용 임계값을 설정합니다. // Set the allowable threshold in the process of detection.
		FLGeometricMatchMulti.SetFindThresholdCoeff();
		// 검출 시 겹쳐짐 허용 정도를 설정합니다. // Set the allowable degree of overlap during detection.
		FLGeometricMatchMulti.SetObjectOverlap();
		// 검출 시 이미지 전처리 유무를 설정합니다. // Set whether or not to pre-process the image during detection.
		// 학습과정에서 전처리 유무를 설정과 동일하게 설정하는 것을 추천합니다. // In the learning process, it is recommended to set the pre-processing status the same as the setting.
		FLGeometricMatchMulti.EnablePreprocessing();

		// 알고리즘 수행 // Execute the algorithm
		eResult = FLGeometricMatchMulti.Execute();

		if(IsFail(eResult))
		{
			ErrorPrint(eResult, "Failed To Execute\n");
			break;
		}

		// 기하학적 패턴 검출 결과를 가져옵니다. // Get the geometric pattern detection result.
		int64_t i64ResultCount = FLGeometricMatchMulti.GetResultCount();

		printf(" ▶ Find Information\n");

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			CGeometricMatchMulti::SResult results;
			CFLFigureArray flfaResultPoints;

			FLGeometricMatchMulti.GetResult(i, results);
			FLGeometricMatchMulti.GetResultForDetectedFeature(i, flfaResultPoints);

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

			// 기하학적 패턴 검출 결과를 Console창에 출력합니다. // Output the geometric pattern detection result to the console window.
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

			// 검출 결과의 중심점을 디스플레이 한다 // Display the center point of the detection result
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

			// 결과 특징점을 디스플레이 한다 // Display the resulting feature point
			for(int32_t i32Index = 0; i32Index < (int32_t)flfaResultPoints.GetCount(); ++i32Index)
			{
				if(flfaResultPoints.GetAt(i32Index)->GetDeclType() != EFigureDeclType_Point)
					break;

				CFLPointD* pFlp = static_cast<CFLPointD*>(flfaResultPoints.GetAt(i32Index));

				if(pFlp)
				{
					if(IsFail(eResult = layerFind.DrawFigureImage(pFlp, arrColor[i64Idx])))
					{
						ErrorPrint(eResult, "Failed to draw figure\n");
						break;
					}
				}
			}


			TPoint<double> tpPosition;
			tpPosition.x = flpPivot.x;
			tpPosition.y = flpPivot.y;

			if(IsFail(eResult = layerFind.DrawTextImage(&flpPivot, wstrClassName.GetBuffer(), YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_CENTER, nullptr, 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
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

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageFind.IsAvailable() && viewImageLearn[0].IsAvailable() && viewImageLearn[1].IsAvailable() && viewImageLearn[2].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}