#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[2];

	CResult eResult = EResult_UnknownError;

	do
	{
		// 이미지 뷰 생성 // Create image view
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

		// SourceView, DstView 의 0번 레이어 가져오기 // Get Layer 0 of SourceView, DstView
		CGUIViewImageLayerWrap SrcLayer0 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap DstLayer0 = viewImage[1].GetLayer(0);

		SrcLayer0.DrawTextCanvas(TPointD(0, 0), L"Figure To Save", YELLOW, BLACK, 15);
		DstLayer0.DrawTextCanvas(TPointD(0, 0), L"Loaded Figure", YELLOW, BLACK, 15);

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		//////// Save
		// Figure 생성 // Create figure
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

		// SourceView의 0번 레이어에 그리기 // Draw on Layer 0 of SourceView
		SrcLayer0.DrawFigureImage(flr, RED);
		SrcLayer0.DrawFigureImage(flfa, BLUE);

		// 경로 없이 파일명만 넣고 저장하는 것도 가능 // It is also possible to put only the file name without path and save it
		flr.Save(L"FLRect.fig");

		// 확장자명 없이 저장하는 것도 가능 // It is also possible to save without an extension name
		eResult = flfa.Save(L"FigureArray");

		//////// Load
		// 다른 DeclType 인 파일을 Load할 경우 반환값이 EResult_OK 가 아닌 다른 반환값을 반환
		// When loading a file with a different DeclType, return value other than EResult_OK is returned
		CFLRectD flrLoad;

		// Rect 에 FigureArray 로드했으므로 실패 // Failed because we loaded FigureArray into Rect
		eResult = flrLoad.Load(L"FigureArray");

		// Rect 에 Rect 파일을 로드했으므로 파일을 로드했으므로 성공 EResult_OK 반환
		// Loaded the Rect file into Rect, so we loaded the file, so return EResult_OK
		eResult = flrLoad.Load(L"FLRect");

		// 다른 DeclType 인 파일을 Load할 경우 반환값이 EResult_OK 가 아닌 다른 반환값을 반환
		// When loading a file with a different DeclType, return value other than EResult_OK is returned
		CFLFigureArray flfaLoad;

		// FigureArray 에 Rect 파일을 로드했으므로 실패 // Failed because Rect file was loaded into FigureArray
		eResult = flfaLoad.Load(L"FLRect");

		// FigureArray 에 FigureArray 파일을 로드했으므로 성공 EResult_OK 반환
		// Success returned EResult_OK because FigureArray file was loaded into FigureArray
		eResult = flfaLoad.Load(L"FigureArray");

		wprintf(L"Loaded Figure\n");

		strFigure.Format(L"Rect : %s\n", CROIUtilities::ConvertFigureObjectToString(&flrLoad).GetString());
		wprintf(L"%s", strFigure.GetString());

		strFigure.Format(L"Figure Array : %s\n\n", CROIUtilities::ConvertFigureObjectToString(&flfaLoad).GetString());
		wprintf(L"%s", strFigure.GetString());

		// DestinationView의 0번 레이어에 그리기 // Draw on Layer 0 of DestinationView
		DstLayer0.DrawFigureImage(flrLoad, MAGENTA);
		DstLayer0.DrawFigureImage(flfaLoad, LIME);

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}