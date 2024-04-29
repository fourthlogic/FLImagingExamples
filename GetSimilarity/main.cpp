#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[2];

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	do
	{
		// Get Similarity Matrix View 생성 // Create Get Similarity Matrix View
		if(IsFail(res = viewImage[0].Create(200, 0, 700, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Transformed View 생성 // Create Transformed View
		if(IsFail(res = viewImage[1].Create(700, 0, 1200, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 각 이미지 뷰의 시점을 동기화 한다. // Synchronize the viewpoint of each image view.
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 각 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the position of each image view window
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View 에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer[2];

		for(int32_t i = 0; i < 2; ++i)
			layer[i] = viewImage[i].GetLayer(0);

		// 화면상 좌표(고정 좌표)에 Get Similarity Matrix View 임을 표시
		// Indicates Get Similarity Matrix View on screen coordinates (fixed coordinates)
		layer[0].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Get Similarity Matrix", YELLOW, BLACK, 30);
		// 화면상 좌표(고정 좌표)에 Transformed View 임을 표시
		// Indicates Transformed View on screen coordinates (fixed coordinates)
		layer[1].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Transformed", YELLOW, BLACK, 30);

		CFLEllipse<double> fleSourceFig;

		// Source Figure 불러오기 // Load source figure
		if((res = fleSourceFig.Load(L"../../ExampleImages/Figure/Ellipse1.fig")).IsFail())
		{
			ErrorPrint(res, "Failed to load figure.\n");
			break;
		}

		// Source Figure 를 Transformed Figure 에 복사한 후 Affine 변환
		// Affine transformation after copying the Source Figure to the Transformed Figure
		CFLEllipse<double> fleTransformedFig(fleSourceFig);
		fleTransformedFig.Scale(fleSourceFig.GetCenter(), 1.8, 1.8);
		fleTransformedFig.Rotate(30., fleSourceFig.GetCenter());
		fleTransformedFig.Offset(-200, 180);

		// Source Figure 와 Transformed Figure 로부터 점을 샘플링
		// Sample points from the Source Figure and Transformed Figure
		CFLFigureArray flfaSource, flfaTransformed;
		fleSourceFig.GetSamplingPointsOnSegment(5., &flfaSource);
		fleTransformedFig.GetSamplingPointsOnSegment(5., &flfaTransformed);

		CFLPointArray flpaSource;
		for(int32_t i = 0; i < flfaSource.GetCount(); ++i)
			flpaSource.PushBack(*flfaSource.GetAt(i));

		// Sampling 한 Source Points 들을 Transformed Points 로 복사한 후 Figure 와 동일하게 Affine 변환
		// After copying the sampled Source Points to Transformed Points, convert the Affine in the same way as the Figure
		CFLPointArray flpaTransformed(flpaSource);
		flpaTransformed.Scale(fleSourceFig.GetCenter(), 1.8, 1.8);
		flpaTransformed.Rotate(30., fleSourceFig.GetCenter());
		flpaTransformed.Offset(-200, 180);

		// Transformed Points 에 Random Noise 를 추가 // Add Random Noise to Transformed Points
		CFLPointArray flpaTransformedWithNoise;
		for(int64_t i = 0; i < flpaTransformed.GetCount(); ++i)
		{
			CFLPoint<double>* pFlp = flpaTransformed.GetAt(i);

			flpaTransformedWithNoise.PushBack(CFLPoint<double>(pFlp->x + CRandomGenerator::Double(-5, 5), pFlp->y + CRandomGenerator::Double(-5, 5)));
		}

		// 0번 Layer 에 Figure 들과 Text 를 출력 // Draw Figures and Text to Layer 0
		layer[0].DrawTextImage(fleSourceFig.GetCenter(), L"Source", LIME, BLACK, 15, false, 0., EGUIViewImageTextAlignment_CENTER_CENTER);
		layer[0].DrawTextImage(fleTransformedFig.GetCenter(), L"Destination", CYAN, BLACK, 15, false, 0., EGUIViewImageTextAlignment_CENTER_CENTER);

		layer[0].DrawFigureImage(fleSourceFig, RED);
		layer[0].DrawFigureImage(flpaSource, LIME, 1);

		layer[0].DrawFigureImage(fleTransformedFig, BLUE);
		layer[0].DrawFigureImage(flpaTransformedWithNoise, CYAN, 1);

		// Similarity 행렬 계산 // Calculate the similarity matrix
		CMatrix<double> matResult;
		if((res = CMatrix<double>::GetSimilarity(flpaSource, flpaTransformedWithNoise, &matResult)).IsFail())
		{
			ErrorPrint(res, "Failed to calculate.\n");
			break;
		}

		// Console 출력 // Console output
		wprintf(L"\n[index] Source Ellipse Points -> Target Points with noise\n");

		for(int64_t i = 0; i < flpaSource.GetCount(); ++i)
			wprintf(L"[%lld] (%.3lf,%.3lf) -> (%.3lf,%.3lf)\n", i, flpaSource.GetAt(i)->x, flpaSource.GetAt(i)->y, flpaTransformedWithNoise.GetAt(i)->x, flpaTransformedWithNoise.GetAt(i)->y);

		wprintf(L"\n\nSimilarity Matrix\n");
		wprintf(L"[%6.3lf, %6.3lf, %6.3lf]\n", *matResult.GetValue(0, 0), *matResult.GetValue(0, 1), *matResult.GetValue(0, 2));
		wprintf(L"[%6.3lf, %6.3lf, %6.3lf]\n", *matResult.GetValue(1, 0), *matResult.GetValue(1, 1), *matResult.GetValue(1, 2));
		wprintf(L"[%6.3lf, %6.3lf, %6.3lf]\n\n\n", *matResult.GetValue(2, 0), *matResult.GetValue(2, 1), *matResult.GetValue(2, 2));

		// 계산된 Similarity 행렬을 사용하여 Affine 변환할 Source Grid Point 생성
		// Create a Source Grid Point to be Affine Transformed using the calculated Similarity Matrix
		CFLPointArray flpaSourceGrid;
		CFLPoint<int32_t> flpGridSize(5, 5);
		int32_t i32GridPitch = 20;
		int32_t i32GridOffsetX = 325;
		int32_t i32GridOffsetY = 90;

		for(int32_t y = 0; y < flpGridSize.y; ++y)
		{
			int32_t i32PosY = y * i32GridPitch + i32GridOffsetY;

			for(int32_t x = 0; x < flpGridSize.x; ++x)
			{
				int32_t i32PosX = x * i32GridPitch + i32GridOffsetX;

				flpaSourceGrid.PushBack(CFLPoint<double>(i32PosX, i32PosY));
			}
		}

		// View 에 Text 출력 // Output text to View
		CFLPoint<int32_t> flpDrawTextPosition(flpaSourceGrid.GetBoundaryRect().left - 3, flpaSourceGrid.GetBoundaryRect().top - 5);

		layer[1].DrawFigureImage(flpaSourceGrid, LIME, 3);
		layer[1].DrawTextImage(flpDrawTextPosition, L"Source", LIME, BLACK, 15, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM);

		// Affine 변환할 Result PointArray 선언 // Declaration of Result PointArray to be converted to Affine
		CFLPointArray flpaResult;
		// Affine 변환에 사용할 Matrix 선언 // Declaration of Matrix to be used for Affine transformation
		CMatrix<double> matA(3, 1);
		CMatrix<double> matB;

		wprintf(L"Affine Transform using Similarity Matrix\n\n");
		wprintf(L"[index] Source Grid -> Transformed Grid\n");

		// Source Grid Point 를 Affine 변환 // Convert Source Grid Point to Affine
		for(int32_t i = 0; i < flpaSourceGrid.GetCount(); ++i)
		{
			matA.SetValue(0, 0, flpaSourceGrid.GetAt(i)->x);
			matA.SetValue(1, 0, flpaSourceGrid.GetAt(i)->y);
			matA.SetValue(2, 0, 1.);

			if((res = matResult.Multiply(matA, &matB)).IsFail())
			{
				ErrorPrint(res, "Failed to calculate Matrix Operation\n");
				break;
			}

			flpaResult.PushBack(CFLPoint<double>(*matB.GetValue(0, 0), *matB.GetValue(1, 0)));

			// Console 출력 // Console output
			wprintf(L"[%d] (%.3lf,%.3lf) -> (%.3lf,%.3lf)\n", i, flpaSourceGrid.GetAt(i)->x, flpaSourceGrid.GetAt(i)->y, flpaResult.GetAt(i)->x, flpaResult.GetAt(i)->y);
		}

		// View 에 Text 출력 // Output text to View
		flpDrawTextPosition.Scale(flpaSourceGrid.GetCenter(), 1.8, 1.8);
		flpDrawTextPosition.Rotate(30., flpaSourceGrid.GetCenter());
		flpDrawTextPosition.Offset(-200, 180);

		layer[1].DrawFigureImage(flpaResult, CYAN, 3);
		layer[1].DrawTextImage(flpDrawTextPosition, L"Transformed", CYAN, BLACK, 15, false, 30., EGUIViewImageTextAlignment_LEFT_BOTTOM);

		layer[0].DrawTextCanvas(&CFLPoint<int32_t>(5, 40), CFLString<wchar_t>().Format(L"[%6.3lf, %6.3lf, %6.3lf]", *matResult.GetValue(0, 0), *matResult.GetValue(0, 1), *matResult.GetValue(0, 2)), YELLOW, BLACK, 15);
		layer[0].DrawTextCanvas(&CFLPoint<int32_t>(5, 60), CFLString<wchar_t>().Format(L"[%6.3lf, %6.3lf, %6.3lf]", *matResult.GetValue(1, 0), *matResult.GetValue(1, 1), *matResult.GetValue(1, 2)), YELLOW, BLACK, 15);
		layer[0].DrawTextCanvas(&CFLPoint<int32_t>(5, 80), CFLString<wchar_t>().Format(L"[%6.3lf, %6.3lf, %6.3lf]", *matResult.GetValue(2, 0), *matResult.GetValue(2, 1), *matResult.GetValue(2, 2)), YELLOW, BLACK, 15);

		// 이미지 뷰들을 갱신 합니다. // Update the image views.
		for(int32_t i = 0; i < 2; ++i)
			viewImage[i].Invalidate(true);

		// 이미지 뷰가 둘중에 하나라도 꺼지면 종료로 간주 // Consider closed when any of the two image views are turned off
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}