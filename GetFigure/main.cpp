#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[4];

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult eResult;

	do
	{
		// 이미지 뷰 생성 // Create image view
		if(IsFail(eResult = viewImage[0].Create(400, 0, 812, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].Create(812, 0, 1224, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[2].Create(400, 384, 812, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[3].Create(812, 384, 1224, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// SourceView, DstView 의 0번 레이어 가져오기 // Get Layer 0 of SourceView, DstView
		CGUIViewImageLayerWrap Src1Layer0 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap Dst1Layer0 = viewImage[1].GetLayer(0);
		CGUIViewImageLayerWrap Src2Layer0 = viewImage[2].GetLayer(0);
		CGUIViewImageLayerWrap Dst2Layer0 = viewImage[3].GetLayer(0);

		Src1Layer0.DrawTextCanvas(TPoint<double>(0, 0), L"Source Figure 1", YELLOW, BLACK, 15);
		Src2Layer0.DrawTextCanvas(TPoint<double>(0, 0), L"Source Figure 2", YELLOW, BLACK, 15);
		Dst1Layer0.DrawTextCanvas(TPoint<double>(0, 0), L"Result Figure 1", YELLOW, BLACK, 15);
		Dst2Layer0.DrawTextCanvas(TPoint<double>(0, 0), L"Result Figure 2", YELLOW, BLACK, 15);

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = viewImage[2].SynchronizePointOfView(&viewImage[3])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		for(int32_t i = 1; i < 4; ++i)
		{
			if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[i])))
			{
				ErrorPrint(eResult, "Failed to synchronize window.\n");
				break;
			}
		}

		CFLFigureArray flfaSource1;
		CFLFigureArray flfaSource2;

		// Source Figure 불러오기 // Load source figure
		if(IsFail(eResult = flfaSource1.Load(L"../../ExampleImages/Figure/various shapes.fig")))
		{
			ErrorPrint(eResult, "Failed to load the figure file.\n");
			break;
		}

		// Source Figure 불러오기 // Load source figure
		if(IsFail(eResult = flfaSource2.Load(L"../../ExampleImages/Figure/Circles.fig")))
		{
			ErrorPrint(eResult, "Failed to load the figure file.\n");
			break;
		}

		// 지원되는 연산자 : [, ] , { , }, (, ), +, -, *, / , ^, <, >, <= , =<, >= , =>, != , =!, =, == , &, &&, and, | , || , or
		// 
		// 	연산자 설명
		// 	2 - 1. 괄호 연산자 : [, { , (,] , }, )
		// 	2 - 2. 덧셈 연산자 : +
		// 	2 - 3. 뺄셈 연산자 : -
		// 	2 - 4. 곱셈 연산자 : *
		// 	2 - 5. 나눗셈 연산자 : /
		// 	2 - 6. 거듭제곱 연산자 : ^
		// 	2 - 7. 관계 연산자 : <, >, <= , =<, >= , =>, != , =!, =, ==
		// 	2 - 8. 논리 곱 : &, &&, and
		// 	2 - 9. 논리 합 : | , || , or
		// 
		// 	같은 역할을 하는 연산자
		// 	3 - 1.[, { , (
		// 	3 - 2.], }, )
		// 	3 - 3. <= , = <
		// 	3 - 3. >= , =>
		// 	3 - 4. != , =!
		// 	3 - 5. =, ==
		// 	3 - 6. &, &&, and
		// 	3 - 7. | , || , or
		// 
		// 지원되는 Figure 조건식
		//  넓이                                : Area
		//	둘레                                : Perimeter
		//	길이                                : Length
		//	폭                                  : Width
		//	높이                                : Height
		//	각도                                : Angle
		//	중심의 x 좌표                       : Center.x
		//	중심의 y 좌표                       : Center.y
		//	무게중심의 x 좌표                   : CenterOfGravity.x
		//	무게중심의 y 좌표                   : CenterOfGravity.y
		//	BoundaryRect의 left                 : BoundaryRect.left 혹은 left
		//	BoundaryRect의 top                  : BoundaryRect.top 혹은 top
		//	BoundaryRect의 right                : BoundaryRect.right 혹은 right
		//	BoundaryRect의 bottom               : BoundaryRect.bottom 혹은 bottom
		//	최소 둘레 직사각형의 넓이           : MinimumEnclosingRectangleArea 혹은 MerArea
		//	최소 둘레 직사각형의 둘레           : MinimumEnclosingRectanglePerimeter 혹은 MerPerimeter
		//	최소 둘레 직사각형의 폭             : MinimumEnclosingRectangleWidth 혹은 MerWidth
		//	최소 둘레 직사각형의 높이           : MinimumEnclosingRectangleHeight 혹은 MerHeight
		//	최소 둘레 직사각형의 각도           : MinimumEnclosingRectangleAngle 혹은 MerAngle
		//	최소 둘레 직사각형의 중심의 x 좌표  : MinimumEnclosingRectangleCenter.x 혹은 MerCenter.x
		//	최소 둘레 직사각형의 중심의 y 좌표  : MinimumEnclosingRectangleCenter.y 혹은 MerCenter.y
		//	내부의 모든 정점의 개수             : VertexCount
		//
		// 지원되는 함수
		//  최대값                              : Max
		//	최소값                              : Min
		//	평균값                              : Mean 혹은 Average 혹은 Avg
		//	중간값                              : Median
		//	합                                  : Sum
		//	제곱합                              : SqrSum
		//	분산                                : Variance 혹은 Var
		//	표준편차                            : StDev

		// Supported Operators : [, ] , { , }, (, ), +, -, *, / , ^, <, >, <= , =<, >= , =>, != , =!, =, == , &, &&, and, | , || , or
		// 
		// 	Operator description
		// 	2 - 1. parenthesis operator : [, { , (,] , }, )
		// 	2 - 2. addition operator : +
		// 	2 - 3. subtraction operator : -
		// 	2 - 4. multiplication operator : *
		// 	2 - 5. division operator : /
		// 	2 - 6. power operator : ^
		// 	2 - 7. relational operator : <, >, <= , =<, >= , =>, != , =!, =, ==
		// 	2 - 8. logical product : &, &&, and
		// 	2 - 9. logical sum : | , || , or
		// 
		// 	Operators that do the same thing
		// 	3 - 1.[, { , (
		// 	3 - 2.], }, )
		// 	3 - 3. <= , = <
		// 	3 - 3. >= , =>
		// 	3 - 4. != , =!
		// 	3 - 5. =, ==
		// 	3 - 6. &, &&, and
		// 	3 - 7. | , || , or
		// 
		//  Supported figure conditional expressions
		//  area                                                  : Area
		//	perimeter                                             : Perimeter
		//	length                                                : Length
		//	width                                                 : Width
		//	height                                                : Height
		//	angle                                                 : Angle
		//	x-coordinate of center                                : Center.x
		//	y-coordinate of center                                : Center.y
		//	x-coordinate of the center of gravity                 : CenterOfGravity.x
		//	y-coordinate of the center of gravity                 : CenterOfGravity.y
		//	left of BoundaryRect                                  : BoundaryRect.left or left
		//	top of BoundaryRect                                   : BoundaryRect.top or top
		//	right of BoundaryRect                                 : BoundaryRect.right or right
		//	bottom of BoundaryRect                                : BoundaryRect.bottom or bottom
		//	Area of ​​minimum enclosing rectangle                   : MinimumEnclosingRectangleArea or MerArea
		//	perimeter of ​​minimum enclosing rectangle              : MinimumEnclosingRectanglePerimeter or MerPerimeter
		//	width of ​​minimum enclosing rectangle                  : MinimumEnclosingRectangleWidth or MerWidth
		//	height of ​​minimum enclosing rectangle                 : MinimumEnclosingRectangleHeight or MerHeight
		//	angle of ​​minimum enclosing rectangle                  : MinimumEnclosingRectangleAngle or MerAngle
		//	x-coordinate of center of ​​minimum enclosing rectangle : MinimumEnclosingRectangleCenter.x or MerCenter.x
		//	y-coordinate of center of ​​minimum enclosing rectangle : MinimumEnclosingRectangleCenter.y or MerCenter.y
		//	the number of all vertices in the interior            : VertexCount
		//
		//  Supported functions
		//  maximum value                                         : Max
		//	minimum value                                         : Min
		//	mean value                                            : Mean or Average or Avg
		//	median value                                          : Median
		//	sum                                                   : Sum
		//	sum of squares                                        : SqrSum
		//	variance                                              : Variance or Var
		//	standard deviation                                    : StDev


		// 조건식 문자열 // Condition string
		CFLString<wchar_t> strExpression1;
		strExpression1.Format(L"area > 400 and center.y < 160 or vertexcount = 3");

		// 조건식을 View에 표기 // Draw the conditional expression in the View
		Dst1Layer0.DrawTextCanvas(TPoint<double>(0, 20), strExpression1, YELLOW, BLACK, 13);

		// 조건식을 만족하는 Figure를 flfaResult1에 추출 // Extract the figure that satisfies the conditional expression to flfaResult1
		CFLFigureArray flfaResult1;

		if(IsFail(eResult = flfaSource1.GetFigure(strExpression1, &flfaResult1)))
		{
			ErrorPrint(eResult, "Failed to process.\n");
			break;
		}

		// 조건식 문자열 // Condition string
		CFLString<wchar_t> strExpression2;
		strExpression2.Format(L"area >= mean('area')");

		// 조건식을 View에 표기 // Draw the conditional expression in the View
		Dst2Layer0.DrawTextCanvas(TPoint<double>(0, 20), strExpression2, YELLOW, BLACK, 13);

		// 조건식을 만족하는 Figure를 flfaResult2에 추출 // Get the figure that satisfies the conditional expression to flfaResult2
		CFLFigureArray flfaResult2;

		if(IsFail(eResult = flfaSource2.GetFigure(strExpression2, &flfaResult2)))
		{
			ErrorPrint(eResult, "Failed to process.\n");
			break;
		}

		// SourceView의 0번 레이어에 Source Figure 그리기 // Draw the Source Figure on Layer 0 of the SourceView
		Src1Layer0.DrawFigureImage(flfaSource1, CYAN);
		Src2Layer0.DrawFigureImage(flfaSource2, CYAN);

		// DstView1의 0번 레이어에 결과 그리기 // Draw the result on layer 0 of DstView1
		Dst1Layer0.DrawFigureImage(flfaSource1, CYAN);
		Dst1Layer0.DrawFigureImage(flfaResult1, LIME, 3, LIME, EGUIViewImagePenStyle_Solid, 1, 0.2f);

		// DstView2의 0번 레이어에 결과 그리기 // Draw the result on layer 0 of DstView2
		Dst2Layer0.DrawFigureImage(flfaSource2, CYAN);
		Dst2Layer0.DrawFigureImage(flfaResult2, LIME, 3, LIME, EGUIViewImagePenStyle_Solid, 1, 0.2f);

		// Console 출력 // Console output
		wprintf(L"Source1 Figure Array\n");
		wprintf(L"%s\n\n", CROIUtilities::ConvertFigureObjectToString(&flfaSource1).GetString());

		wprintf(L"Result1 Figure that area > 400 and center.y < 160 or vertexcount = 3\n");
		wprintf(L"%s\n\n", CROIUtilities::ConvertFigureObjectToString(&flfaResult1).GetString());

		wprintf(L"Source2 Figure Array\n");
		wprintf(L"%s\n\n", CROIUtilities::ConvertFigureObjectToString(&flfaSource2).GetString());

		wprintf(L"Result2 Figure that area >= mean('area')\n");
		wprintf(L"%s\n\n", CROIUtilities::ConvertFigureObjectToString(&flfaResult2).GetString());

		// 이미지 뷰를 갱신 합니다. // Update image view
		for(int32_t i = 0; i < 4; ++i)
			viewImage[i].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable() && viewImage[2].IsAvailable() && viewImage[3].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}