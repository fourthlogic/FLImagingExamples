#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage[2];

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult cResult;

	do
	{
		// Source Figures View ���� // Create the Source Figures View
		if(IsFail(cResult = viewImage[0].Create(200, 0, 700, 500)))
		{
			ErrorPrint(cResult, "Failed to create the image view.\n");
			break;
		}

		// Warp Result View ���� // Create the Warp Result View
		if(IsFail(cResult = viewImage[1].Create(700, 0, 1200, 500)))
		{
			ErrorPrint(cResult, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ�. // Synchronize the viewpoint of each image view.
		if(IsFail(cResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(cResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ�. // Synchronize the position of each image view window.
		if(IsFail(cResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(cResult, "Failed to synchronize window.\n");
			break;
		}

		// ȭ��� �� ���̵��� ��ǥ 0.5������ ���� // Apply the coordinates 0.5 magnification so that it can be seen clearly on the screen
		viewImage[0].SetScale(0.5);
		// ȭ��� �� ���̵��� ���� Offset ���� // Adjust the viewpoint offset so that it can be seen clearly on the screen
		viewImage[0].SetOffset(&CFLPoint<double>(-120., -35));

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer[2];

		for(int32_t i = 0; i < 2; ++i)
			layer[i] = viewImage[i].GetLayer(0);

		// ȭ��� ��ǥ(���� ��ǥ)�� Source Figure View ���� ǥ�� // Displays Source Figure View in on-screen coordinates (fixed coordinates)
		layer[0].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Source Figures", YELLOW, BLACK, 30);
		// ȭ��� ��ǥ(���� ��ǥ)�� Warp Result View ���� ǥ�� // Display of Warp Result View in on-screen coordinates (fixed coordinates)
		layer[1].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Warp Result", YELLOW, BLACK, 30);

		// Warp�� �����ϱ� ���� Source Figure���� ��� FigureArray�� �ε��մϴ�. (�ٸ� Figure�鵵 ���� �����մϴ�.)
		// Loads a FigureArray containing the source figures for running the warp. (Other figures are also available.)
		CFLFigureArray flfaSource;

		// Source Figure �ҷ����� // Load Source Figure
		if(IsFail(cResult = flfaSource.Load(L"../../ExampleImages/Figure/DistortedCoordinates.fig")))
		{
			ErrorPrint(cResult, "Failed to load the figure file.\n");
			break;
		}
		

		// Source Figure�� �� �������� SourceRegion Quad�� ����
		// Each vertex of the source figure is created as a SourceRegion Quad
		CFLQuad<double> flqSourceRegion(CFLPoint<double>(397.5, 227.0), CFLPoint<double>(1065.0, 292.0), CFLPoint<double>(1063.5, 739.5), CFLPoint<double>(395.0, 822.5));
		// SourceRegion Quad�� ���簢���� ���·� ��ģ Quad�� TargetRegion Quad�� ����
		// Create a TargetRegion Quad with a Quad that spreads the SourceRegion Quad in the form of a rectangle
		CFLQuad<double> flqTargetRegion(CFLPoint<double>(397.5, 227.0), CFLPoint<double>(1065.0, 227.0), CFLPoint<double>(1065.5, 822.5), CFLPoint<double>(397.5, 822.5));

		wprintf(L"Source Quad Region : %s\n", CROIUtilities::ConvertFigureObjectToString(flqSourceRegion).GetString());
		wprintf(L"Target Quad Region : %s\n\n", CROIUtilities::ConvertFigureObjectToString(flqTargetRegion).GetString());

		// Warp ����� �޾ƿ� FigureArray // FigureArray to receive the warp result
		CFLFigureArray flfaResult;

		// Perspective Type���� Warp �Լ� ���� (Perspective, Bilinear �� Ÿ������ �Լ� ���� ����)
		// Warp function works with perspective type (function can be operated with two types, perspective and bilinear)
		if(IsFail(cResult = flfaSource.Warp(flqSourceRegion, flqTargetRegion, flfaResult, EWarpingType_Perspective)))
		{
			ErrorPrint(cResult, "Failed to process.\n");
			break;
		}

		// Source Figure �׸��� // Draw the Source Figure
		if(IsFail(cResult = layer[0].DrawFigureImage(&flfaSource, YELLOW, 3)))
		{
			ErrorPrint(cResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// Warp Result Figure �׸��� // Draw Warp Result Figure
		if(IsFail(cResult = layer[1].DrawFigureImage(&flfaResult, LIME, 3)))
		{
			ErrorPrint(cResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// SourceRegion Quad �׸��� // Draw SourceRegion Quad
		if(IsFail(cResult = layer[0].DrawFigureImage(&flqSourceRegion, RED, 1)))
		{
			ErrorPrint(cResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// TargetRegion Quad �׸��� // Draw TargetRegion Quad
		if(IsFail(cResult = layer[1].DrawFigureImage(&flqTargetRegion, BLUE, 1)))
		{
			ErrorPrint(cResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// Source�� Warp Result Point�� Console â�� ��� // Output the Source and Warp Result Point to the console window
		for(int i = 0 ; i < flfaSource.GetCount(); ++i)
		{
			CFLFigure* pFlfSource = flfaSource.GetAt(i);
			CFLFigure* pFlfTarget = flfaResult.GetAt(i);

			CFLPoint<double>* pFlpSource = (CFLPoint<double>*)pFlfSource;
			CFLPoint<double>* pFlpTarget = (CFLPoint<double>*)pFlfTarget;

			if(pFlfSource && pFlfTarget)
				printf("Source (%.1lf, %.1lf) -> Warp Result (%.1lf, %.1lf)\n", pFlpSource->x, pFlpSource->y, pFlpTarget->x, pFlpTarget->y);
		}

		// �̹��� ����� ���� �մϴ�. // Update the image views.
		for(int32_t i = 0; i < 2; ++i)
			viewImage[i].Invalidate(true);


		// �̹��� �䰡 ���߿� �ϳ��� ������ ����� ���� // If either one of the imageviews is turned off, it is considered to be closed.
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}