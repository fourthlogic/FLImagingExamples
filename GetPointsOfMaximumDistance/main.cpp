﻿#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[4];

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	do
	{
		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage[0].Create(400, 0, 812, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(812, 0, 1224, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[2].Create(400, 384, 812, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[3].Create(812, 384, 1224, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
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

		Dst1Layer0.DrawTextCanvas(TPoint<double>(0, 20), L"Maximum Distance", CYAN, BLACK);
		Dst2Layer0.DrawTextCanvas(TPoint<double>(0, 20), L"Maximum Distance", CYAN, BLACK);

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(res = viewImage[2].SynchronizePointOfView(&viewImage[3])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		for(int32_t i = 1; i < 4; ++i)
		{
			if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[i])))
			{
				ErrorPrint(res, "Failed to synchronize window.\n");
				break;
			}
		}

		// Figure 선언 // Figure declaration
		CFLCircle<double> flcSource1;
		CFLQuad<double> flqOperand1;
		CFLFigureArray flfaSource2;
		CFLFigureArray flfaOperand2;

		// Source Figure 불러오기 // Load source figure
		if(IsFail(res = flcSource1.Load(L"../../ExampleImages/Figure/Circle1.fig")))
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}

		if(IsFail(res = flfaSource2.Load(L"../../ExampleImages/Figure/various shapes_Top.fig")))
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}

		// Operand Figure 불러오기 // Load Operand Figure
		if(IsFail(res = flqOperand1.Load(L"../../ExampleImages/Figure/Quad1.fig")))
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}

		if(IsFail(res = flfaOperand2.Load(L"../../ExampleImages/Figure/various shapes_Bottom.fig")))
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}

		
		// Figure 사이의 최대 거리를 나타내는 점을 추출 // Get the point representing the maximum distance between figures
		CFLPointArray flpaResult1;

		if(IsFail(res = flcSource1.GetPointsOfMaximumDistance(flqOperand1, &flpaResult1)))
		{
			ErrorPrint(res, "Failed to process.\n");
			break;
		}

		CFLPointArray flpaResult2;

		if(IsFail(res = flfaSource2.GetPointsOfMaximumDistance(flfaOperand2, &flpaResult2)))
		{
			ErrorPrint(res, "Failed to process.\n");
			break;
		}

		// Figure 사이의 최대 거리를 계산 // Calculate the maximum distance between figures
		double f64MaximumDistance1 = 0.;

		if(IsFail(res = flcSource1.GetMaximumDistance(flqOperand1, &f64MaximumDistance1)))
		{
			ErrorPrint(res, "Failed to process.\n");
			break;
		}

		double f64MaximumDistance2 = 0.;

		if(IsFail(res = flfaSource2.GetMaximumDistance(flfaOperand2, &f64MaximumDistance2)))
		{
			ErrorPrint(res, "Failed to process.\n");
			break;
		}

		// 두 Point를 잇는 Line을 생성 // Create a line connecting two points
		CFLLine<double> fllMax1(flpaResult1.GetAt(0), flpaResult1.GetAt(1));
		CFLLine<double> fllMax2(flpaResult2.GetAt(0), flpaResult2.GetAt(1));

		// SourceView1의 0번 레이어에 Source, Operand Figure 그리기 // Draw Source and Operand Figure on Layer 0 of SourceView1
		Src1Layer0.DrawFigureImage(flcSource1, BLACK, 3);
		Src1Layer0.DrawFigureImage(flcSource1, KHAKI);
		Src1Layer0.DrawFigureImage(flqOperand1, BLACK, 3);
		Src1Layer0.DrawFigureImage(flqOperand1, LIME);

		// DstView1의 0번 레이어에 결과 그리기 // Draw the result on layer 0 of DstView1
		Dst1Layer0.DrawFigureImage(flcSource1, BLACK, 3);
		Dst1Layer0.DrawFigureImage(flcSource1, KHAKI);
		Dst1Layer0.DrawFigureImage(flqOperand1, BLACK, 3);
		Dst1Layer0.DrawFigureImage(flqOperand1, LIME);
		Dst1Layer0.DrawFigureImage(flpaResult1, CYAN, 3);
		Dst1Layer0.DrawFigureImage(fllMax1, CYAN);
		Dst1Layer0.DrawTextImage(fllMax1.GetCenter(), CFLString<wchar_t>().Format(L"%.3lf", f64MaximumDistance1), CYAN, BLACK, 12, false, 0, EGUIViewImageTextAlignment_LEFT_BOTTOM);

		// SourceView2의 0번 레이어에 Source, Operand Figure 그리기 // Draw Source and Operand Figure on Layer 0 of SourceView2
		Src2Layer0.DrawFigureImage(flfaSource2, BLACK, 3);
		Src2Layer0.DrawFigureImage(flfaSource2, KHAKI);
		Src2Layer0.DrawFigureImage(flfaOperand2, BLACK, 3);
		Src2Layer0.DrawFigureImage(flfaOperand2, LIME);

		// DstView2의 0번 레이어에 결과 그리기 // Draw the result on layer 0 of DstView2
		Dst2Layer0.DrawFigureImage(flfaSource2, BLACK, 3);
		Dst2Layer0.DrawFigureImage(flfaSource2, KHAKI);
		Dst2Layer0.DrawFigureImage(flfaOperand2, BLACK, 3);
		Dst2Layer0.DrawFigureImage(flfaOperand2, LIME);
		Dst2Layer0.DrawFigureImage(flpaResult2, CYAN, 3);
		Dst2Layer0.DrawFigureImage(fllMax2, CYAN);
		Dst2Layer0.DrawTextImage(fllMax2.GetCenter(), CFLString<wchar_t>().Format(L"%.3lf", f64MaximumDistance2), CYAN, BLACK, 12, false, 0, EGUIViewImageTextAlignment_RIGHT_TOP);

		// Console 출력 // Console output
		wprintf(L"Source1 CFLCircle<double>\n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flcSource1).GetString());

		wprintf(L"Operand1 CFLQuad<double>\n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flqOperand1).GetString());

		wprintf(L"Result1 Points of Maximum distance\n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flpaResult1).GetString());

		wprintf(L"Result1 Maximum distance\n");
		wprintf(L"%lf\n\n", f64MaximumDistance1);

		wprintf(L"\n\n");

		wprintf(L"Source2 CFLFigureArray\n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flfaSource2).GetString());

		wprintf(L"Operand2 CFLFigureArray\n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flfaOperand2).GetString());

		wprintf(L"Result2 Points of Maximum distance\n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(&flpaResult2).GetString());

		wprintf(L"Result2 Maximum distance\n");
		wprintf(L"%lf\n\n", f64MaximumDistance2);

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