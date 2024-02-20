#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage[4];

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult eResult;

	do
	{
		// �̹��� �� ���� // Create image view
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

		// SourceView, DstView �� 0�� ���̾� �������� // Get Layer 0 of SourceView, DstView
		CGUIViewImageLayerWrap Src1Layer0 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap Dst1Layer0 = viewImage[1].GetLayer(0);
		CGUIViewImageLayerWrap Src2Layer0 = viewImage[2].GetLayer(0);
		CGUIViewImageLayerWrap Dst2Layer0 = viewImage[3].GetLayer(0);

		Src1Layer0.DrawTextCanvas(TPoint<double>(0, 0), L"Source Figure 1", YELLOW, BLACK, 15);
		Src2Layer0.DrawTextCanvas(TPoint<double>(0, 0), L"Source Figure 2", YELLOW, BLACK, 15);
		Dst1Layer0.DrawTextCanvas(TPoint<double>(0, 0), L"Result Figure 1", YELLOW, BLACK, 15);
		Dst2Layer0.DrawTextCanvas(TPoint<double>(0, 0), L"Result Figure 2", YELLOW, BLACK, 15);

		Dst1Layer0.DrawTextCanvas(TPoint<double>(0, 20), L"Minimum Distance", CYAN, BLACK);
		Dst2Layer0.DrawTextCanvas(TPoint<double>(0, 20), L"Minimum Distance", CYAN, BLACK);

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = viewImage[2].SynchronizePointOfView(&viewImage[3])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		for(int32_t i = 1; i < 4; ++i)
		{
			if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[i])))
			{
				ErrorPrint(eResult, "Failed to synchronize window.\n");
				break;
			}
		}

		// Figure ���� // Figure declaration
		CFLCircle<double> flcSource1;
		CFLQuad<double> flqOperand1;
		CFLFigureArray flfaSource2;
		CFLFigureArray flfaOperand2;

		// Source Figure �ҷ����� // Load source figure
		if(IsFail(eResult = flcSource1.Load(L"../../ExampleImages/Figure/Circle1.fig")))
		{
			ErrorPrint(eResult, "Failed to load the figure file.\n");
			break;
		}

		if(IsFail(eResult = flfaSource2.Load(L"../../ExampleImages/Figure/various shapes_Top.fig")))
		{
			ErrorPrint(eResult, "Failed to load the figure file.\n");
			break;
		}

		// Operand Figure �ҷ����� // Load Operand Figure
		if(IsFail(eResult = flqOperand1.Load(L"../../ExampleImages/Figure/Quad1.fig")))
		{
			ErrorPrint(eResult, "Failed to load the figure file.\n");
			break;
		}

		if(IsFail(eResult = flfaOperand2.Load(L"../../ExampleImages/Figure/various shapes_Bottom.fig")))
		{
			ErrorPrint(eResult, "Failed to load the figure file.\n");
			break;
		}

		
		// Figure ������ �ּ� �Ÿ��� ��Ÿ���� ���� ���� // Get the point representing the minimum distance between figures
		CFLPointArray flpaResult1;

		if(IsFail(eResult = flcSource1.GetPointsOfMinimumDistance(flqOperand1, &flpaResult1)))
		{
			ErrorPrint(eResult, "Failed to process.\n");
			break;
		}

		CFLPointArray flpaResult2;

		if(IsFail(eResult = flfaSource2.GetPointsOfMinimumDistance(flfaOperand2, &flpaResult2)))
		{
			ErrorPrint(eResult, "Failed to process.\n");
			break;
		}

		// Figure ������ �ּ� �Ÿ��� ��� // Calculate the minimum distance between figures
		double f64MinimumDistance1 = 0.;

		if(IsFail(eResult = flcSource1.GetMinimumDistance(flqOperand1, &f64MinimumDistance1)))
		{
			ErrorPrint(eResult, "Failed to process.\n");
			break;
		}

		double f64MinimumDistance2 = 0.;

		if(IsFail(eResult = flfaSource2.GetMinimumDistance(flfaOperand2, &f64MinimumDistance2)))
		{
			ErrorPrint(eResult, "Failed to process.\n");
			break;
		}

		// �� Point�� �մ� Line�� ���� // Create a line connecting two points
		CFLLine<double> fllMin1(flpaResult1.GetAt(0), flpaResult1.GetAt(1));
		CFLLine<double> fllMin2(flpaResult2.GetAt(0), flpaResult2.GetAt(1));

		// SourceView1�� 0�� ���̾ Source, Operand Figure �׸��� // Draw Source and Operand Figure on Layer 0 of SourceView1
		Src1Layer0.DrawFigureImage(flcSource1, BLACK, 3);
		Src1Layer0.DrawFigureImage(flcSource1, KHAKI);
		Src1Layer0.DrawFigureImage(flqOperand1, BLACK, 3);
		Src1Layer0.DrawFigureImage(flqOperand1, LIME);

		// DstView1�� 0�� ���̾ ��� �׸��� // Draw the result on layer 0 of DstView1
		Dst1Layer0.DrawFigureImage(flcSource1, BLACK, 3);
		Dst1Layer0.DrawFigureImage(flcSource1, KHAKI);
		Dst1Layer0.DrawFigureImage(flqOperand1, BLACK, 3);
		Dst1Layer0.DrawFigureImage(flqOperand1, LIME);
		Dst1Layer0.DrawFigureImage(flpaResult1, CYAN, 3);
		Dst1Layer0.DrawFigureImage(fllMin1, CYAN);
		Dst1Layer0.DrawTextImage(fllMin1.GetCenter(), CFLString<wchar_t>().Format(L"%.3lf", f64MinimumDistance1), CYAN, BLACK, 12, false, 0, EGUIViewImageTextAlignment_RIGHT_TOP);

		// SourceView2�� 0�� ���̾ Source, Operand Figure �׸��� // Draw Source and Operand Figure on Layer 0 of SourceView2
		Src2Layer0.DrawFigureImage(flfaSource2, BLACK, 3);
		Src2Layer0.DrawFigureImage(flfaSource2, KHAKI);
		Src2Layer0.DrawFigureImage(flfaOperand2, BLACK, 3);
		Src2Layer0.DrawFigureImage(flfaOperand2, LIME);

		// DstView2�� 0�� ���̾ ��� �׸��� // Draw the result on layer 0 of DstView2
		Dst2Layer0.DrawFigureImage(flfaSource2, BLACK, 3);
		Dst2Layer0.DrawFigureImage(flfaSource2, KHAKI);
		Dst2Layer0.DrawFigureImage(flfaOperand2, BLACK, 3);
		Dst2Layer0.DrawFigureImage(flfaOperand2, LIME);
		Dst2Layer0.DrawFigureImage(flpaResult2, CYAN, 3);
		Dst2Layer0.DrawFigureImage(fllMin2, CYAN);
		Dst2Layer0.DrawTextImage(fllMin2.GetCenter(), CFLString<wchar_t>().Format(L"%.3lf", f64MinimumDistance2), CYAN, BLACK, 12, false, 0, EGUIViewImageTextAlignment_LEFT_BOTTOM);

		// Console ��� // Console output
		wprintf(L"Source1 CFLCircle<double>\n");
		wprintf(L"%s\n\n", CFigureUtils::ConvertFigureObjectToString(&flcSource1).GetString());

		wprintf(L"Operand1 CFLQuad<double>\n");
		wprintf(L"%s\n\n", CFigureUtils::ConvertFigureObjectToString(&flqOperand1).GetString());

		wprintf(L"Result1 Points of Minimum distance\n");
		wprintf(L"%s\n\n", CFigureUtils::ConvertFigureObjectToString(&flpaResult1).GetString());

		wprintf(L"Result1 Minimum distance\n");
		wprintf(L"%lf\n\n", f64MinimumDistance1);

		wprintf(L"\n\n");

		wprintf(L"Source2 CFLFigureArray\n");
		wprintf(L"%s\n\n", CFigureUtils::ConvertFigureObjectToString(&flfaSource2).GetString());

		wprintf(L"Operand2 CFLFigureArray\n");
		wprintf(L"%s\n\n", CFigureUtils::ConvertFigureObjectToString(&flfaOperand2).GetString());

		wprintf(L"Result2 Points of Minimum distance\n");
		wprintf(L"%s\n\n", CFigureUtils::ConvertFigureObjectToString(&flpaResult2).GetString());

		wprintf(L"Result2 Minimum distance\n");
		wprintf(L"%lf\n\n", f64MinimumDistance2);

		// �̹��� �並 ���� �մϴ�. // Update image view
		for(int32_t i = 0; i < 4; ++i)
			viewImage[i].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable() && viewImage[2].IsAvailable() && viewImage[3].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}