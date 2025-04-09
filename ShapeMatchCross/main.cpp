#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"



int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage;

	bool bError = false;

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	do
	{
		// Source 이미지 로드 // Load the source image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/ShapeMatch/Cross_Dark.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage.Create(400, 0, 912, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			bError = true;
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		if(IsFail(res = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			bError = true;
			break;
		}

		if(bError)
			break;

		// Shape Match 객체 생성 // Create Shape Match object
		CShapeMatchCross shapeMatch;

		//// 학습할 십자의 크기 설정 // Set the radius of the cross to learn
		//res = shapeMatch.SetHalfLength(130);
		//res = shapeMatch.SetHalfThickness(50);

		//// 도형 학습 // Learn shape
		//if(IsFail(res = shapeMatch.Learn()))
		//{
		//	ErrorPrint(res, "Failed to Learn.\n");
		//	break;
		//}

		//// 검출할 이미지 설정 // Set image to detect
		res = shapeMatch.SetSourceImage(fliImage);
		//// 검출 시 사용될 유효 변경 크기범위를 설정합니다. // Set the effective change size range to be used for detection.
		//res = shapeMatch.SetScaleRange(0.9, 1.1);
		//// 검출할 객체의 색상을 설정합니다. // Sets the color of the object to be detected.
		//res = shapeMatch.SetObjectColor(EShapeMatchObjectColor_Bright);

		//// 알고리즘 수행 // Execute the algorithm
		//if(IsFail(res = shapeMatch.Execute()))
		//{
		//	ErrorPrint(res, "Failed to execute.\n");
		//	break;
		//}




		// Learn
		res = shapeMatch.SetObjectColor(EShapeMatchObjectColor_Any);
		res = shapeMatch.SetHalfLength(130);
		res = shapeMatch.SetHalfThickness(50);

		//// Learn Advanced
		//res = shapeMatch.SetLearnReductionLevel(1);
		//res = shapeMatch.SetLearnReductionMode(EMatchReductionMode_Area);
		//res = shapeMatch.SetFeatureCount(2047);
		//res = shapeMatch.SetFeatureFiltering(0.51);
		//res = shapeMatch.SetLearnThresholdCoefficient(1.01);

		//// Find
		res = shapeMatch.SetScaleRange(0.9, 1.1);
		//res = shapeMatch.SetAngleBias(0.1);
		//res = shapeMatch.SetAngleTolerance(5.10);
		//res = shapeMatch.SetMinimumDetectionScore(0.71);
		//res = shapeMatch.SetMaxObject(2);
		//res = shapeMatch.SetScoreMode(EMatchScoreMode_Target);

		//// Find Advanced
		//res = shapeMatch.EnableInterpolation(false);
		//res = shapeMatch.SetInvisibleRegionEstimation(1.26);
		//res = shapeMatch.SetFindThresholdCoefficient(1.10);
		//res = shapeMatch.SetObjectOverlap(0.51);
		//res = shapeMatch.SetOptimizationOption(EMatchOptimizationOption_Normal);
		//res = shapeMatch.SetCandidateExtraction(EMatchCandidateExtraction_Accurate);
		//res = shapeMatch.SetJudgementLevel(EMatchJudgementLevel_Rough);


		CShapeMatchCross cross2(shapeMatch);
		CShapeMatchCross cross3;
		cross3 = shapeMatch;
		CShapeMatchCross cross4;
		cross4.Assign(shapeMatch);



		//res = shapeMatch.Clear();
		//
		//// Learn
		//auto l0 = shapeMatch.GetObjectColor();
		//auto l1 = shapeMatch.GetHalfLength();
		//auto l2 = shapeMatch.GetHalfThickness();

		//// Learn Advanced
		//auto la0 = shapeMatch.GetLearnReductionLevel();
		//auto la1 = shapeMatch.GetLearnReductionMode();
		//auto la2 = shapeMatch.GetFeatureCount();
		//auto la3 = shapeMatch.GetFeatureFiltering();
		//auto la4 = shapeMatch.GetLearnThresholdCoefficient();

		//// Find
		//double scale_min;
		//double scale_max;
		//res = shapeMatch.GetScaleRange(scale_min, scale_max);
		//auto f0 = shapeMatch.GetAngleBias();
		//auto f1 = shapeMatch.GetAngleTolerance();
		//auto f2 = shapeMatch.GetMinimumDetectionScore();
		//auto f3 = shapeMatch.GetMaxObject();
		//auto f4 = shapeMatch.GetScoreMode();

		//// Find Advanced
		//auto fa0 = shapeMatch.IsInterpolationEnabled();
		//auto fa1 = shapeMatch.GetInvisibleRegionEstimation();
		//auto fa2 = shapeMatch.GetFindThresholdCoefficient();
		//auto fa3 = shapeMatch.GetObjectOverlap();
		//auto fa4 = shapeMatch.GetOptimizationOption();
		//auto fa5 = shapeMatch.GetCandidateExtraction();
		//auto fa6 = shapeMatch.GetJudgementLevel();


		// 도형 학습 // Learn shape
		if(IsFail(res = cross4.Learn()))
		{
			ErrorPrint(res, "Failed to Learn.\n");
			break;
		}

		// 알고리즘 수행 // Execute the algorithm
		if(IsFail(res = cross4.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}


		CShapeMatchCrossResult smResult2;
		res = cross4.GetResult(0, smResult2);




		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		int64_t i64ResultCount = cross4.GetResultCount();

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			CShapeMatchCrossResult matchResult;

			res = cross4.GetResult(-1, matchResult);
			CFLFigure* pFigure = CFigureUtilities::ConvertFigureStringToObject(L"P[D(376.91358024691363,246.85185185185188),D(326.48148148148152,493.70370370370375),D(233.58024691358025,506.97530864197535),D(222.96296296296296,650.30864197530866),D(366.29629629629630,652.96296296296305),D(424.69135802469140,599.87654320987656),D(499.01234567901236,724.62962962962968),D(597.22222222222229,721.97530864197529),D(602.53086419753095,530.86419753086420),D(504.32098765432102,400.80246913580248)]");
			res = cross4.GetResultShape(100, pFigure);

			// 도형 검출 결과를 Console창에 출력합니다. // Output the shape detection result to the console window.
			printf(" < Instance : %lld >\n", i);
			printf("  1. Shape Type : Cross\n");
			printf("    Pivot X: %.3lf\n", matchResult.flpPivot.x);
			printf("    Pivot Y: %.3lf\n", matchResult.flpPivot.y);
			printf("    Half Length    : %.3lf\n", matchResult.f64HalfLength);
			printf("    Half Thickness    : %.3lf\n", matchResult.f64HalfThickness);
			printf("  2. Score : %.3lf\n  3. Scale : %.3lf\n\n", matchResult.f32Score, matchResult.f32Scale);

			if(IsFail(res = layer.DrawFigureImage(pFigure, CYAN, 3)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsFail(res = layer.DrawTextImage(matchResult.flpPivot, CFLString<wchar_t>().Format(L"Score : %.3lf\nScale : %.3lf\nPivot : (%.2lf, %.2lf)", matchResult.f32Score, matchResult.f32Scale, matchResult.flpPivot.x, matchResult.flpPivot.y), YELLOW, BLACK, 15)))
			{
				ErrorPrint(res, "Failed to draw text\n");
				break;
			}
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImage.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}