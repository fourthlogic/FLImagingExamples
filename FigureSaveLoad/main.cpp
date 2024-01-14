#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage[2];

	CResult eResult = EResult_UnknownError;

	do
	{
		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImage[0].Create(400, 0, 912, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].Create(912, 0, 1424, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// SourceView, DstView �� 0�� ���̾� �������� // Get Layer 0 of SourceView, DstView
		CGUIViewImageLayerWrap SrcLayer0 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap DstLayer0 = viewImage[1].GetLayer(0);

		SrcLayer0.DrawTextCanvas(TPointD(0, 0), L"Figure To Save", YELLOW, BLACK, 15);
		DstLayer0.DrawTextCanvas(TPointD(0, 0), L"Loaded Figure", YELLOW, BLACK, 15);

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		//////// Save
		// Figure ���� // Create figure
		CFLRectD flr(50, 50, 100, 100);

		CFLCircleD flc(150., 100., 30., 0., 0., 80., EArcClosingMethod_Center);

		CFLEllipseD fle(300, 150, 100, 50,0,180,60, EArcClosingMethod_EachOther);

		CFLFigureArray flfa;

		flfa.PushBack(flc);
		flfa.PushBack(fle);

		CFLString<wchar_t> strFigure;
		wprintf(L"Figure To Save\n");

		strFigure.Format(L"Rect : %s\n", CROIUtilities::ConvertFigureObjectToString(&flr).GetString());
		wprintf(L"%s", strFigure.GetString());

		strFigure.Format(L"Figure Array : %s\n\n", CROIUtilities::ConvertFigureObjectToString(&flfa).GetString());
		wprintf(L"%s", strFigure.GetString());

		// SourceView�� 0�� ���̾ �׸��� // Draw on Layer 0 of SourceView
		SrcLayer0.DrawFigureImage(flr, RED);
		SrcLayer0.DrawFigureImage(flfa, BLUE);

		// ��� ���� ���ϸ� �ְ� �����ϴ� �͵� ���� // It is also possible to put only the file name without path and save it
		flr.Save(L"FLRect.fig");

		// Ȯ���ڸ� ���� �����ϴ� �͵� ���� // It is also possible to save without an extension name
		eResult = flfa.Save(L"FigureArray");

		//////// Load
		// �ٸ� DeclType �� ������ Load�� ��� ��ȯ���� EResult_OK �� �ƴ� �ٸ� ��ȯ���� ��ȯ
		// When loading a file with a different DeclType, return value other than EResult_OK is returned
		CFLRectD flrLoad;

		// Rect �� FigureArray �ε������Ƿ� ���� // Failed because we loaded FigureArray into Rect
		eResult = flrLoad.Load(L"FigureArray");

		// Rect �� Rect ������ �ε������Ƿ� ������ �ε������Ƿ� ���� EResult_OK ��ȯ
		// Loaded the Rect file into Rect, so we loaded the file, so return EResult_OK
		eResult = flrLoad.Load(L"FLRect");

		// �ٸ� DeclType �� ������ Load�� ��� ��ȯ���� EResult_OK �� �ƴ� �ٸ� ��ȯ���� ��ȯ
		// When loading a file with a different DeclType, return value other than EResult_OK is returned
		CFLFigureArray flfaLoad;

		// FigureArray �� Rect ������ �ε������Ƿ� ���� // Failed because Rect file was loaded into FigureArray
		eResult = flfaLoad.Load(L"FLRect");

		// FigureArray �� FigureArray ������ �ε������Ƿ� ���� EResult_OK ��ȯ
		// Success returned EResult_OK because FigureArray file was loaded into FigureArray
		eResult = flfaLoad.Load(L"FigureArray");

		wprintf(L"Loaded Figure\n");

		strFigure.Format(L"Rect : %s\n", CROIUtilities::ConvertFigureObjectToString(&flrLoad).GetString());
		wprintf(L"%s", strFigure.GetString());

		strFigure.Format(L"Figure Array : %s\n\n", CROIUtilities::ConvertFigureObjectToString(&flfaLoad).GetString());
		wprintf(L"%s", strFigure.GetString());

		// DestinationView�� 0�� ���̾ �׸��� // Draw on Layer 0 of DestinationView
		DstLayer0.DrawFigureImage(flrLoad, MAGENTA);
		DstLayer0.DrawFigureImage(flfaLoad, LIME);

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}