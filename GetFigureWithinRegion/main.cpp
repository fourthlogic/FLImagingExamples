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

		if(IsFail(eResult = viewImage[2].Create(400, 400, 912, 794)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[3].Create(912, 400, 1424, 794)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// SourceView, DstView �� 0�� ���̾� �������� // Get Layer 0 of SourceView, DstView
		CGUIViewImageLayerWrap SrcLayer0 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap DstLayer0 = viewImage[1].GetLayer(0);
		CGUIViewImageLayerWrap SrcLayer1 = viewImage[2].GetLayer(0);
		CGUIViewImageLayerWrap DstLayer1 = viewImage[3].GetLayer(0);

		SrcLayer0.DrawTextCanvas(TPoint<double>(0, 0), L"Source Figure And Region1", YELLOW, BLACK, 15);
		DstLayer0.DrawTextCanvas(TPoint<double>(0, 0), L"Get Figure Within Region1", YELLOW, BLACK, 15);
		SrcLayer1.DrawTextCanvas(TPoint<double>(0, 0), L"Source Figure And Region2", YELLOW, BLACK, 15);
		DstLayer1.DrawTextCanvas(TPoint<double>(0, 0), L"Get Figure Within Region2", YELLOW, BLACK, 15);

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		for(int32_t i = 1; i < 4; ++i)
		{
			if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[i])))
			{
				ErrorPrint(eResult, "Failed to synchronize view\n");
				break;
			}
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

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		for(int32_t i = 1; i < 4; ++i)
		{
			if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[i])))
			{
				ErrorPrint(eResult, "Failed to synchronize window.\n");
				break;
			}
		}

		// Figure ���� // Create figure
		CFLLine<double> fll(76, 300, 130, 210);

		CFLRect<double> flr(50, 50, 100, 100);

		CFLCircle<double> flc(150., 100., 30., 0., 0., 80., EArcClosingMethod_Center);

		CFLEllipse<double> fle(300, 150, 100, 50, 0, 180, 60, EArcClosingMethod_EachOther);

		CFLComplexRegion flcr;
		flcr.PushBack(CFLPoint<double>(270, 100));
		flcr.PushBack(CFLPoint<double>(420, 160));
		flcr.PushBack(CFLPoint<double>(300, 200));

		CFLFigureArray flfaSource;

		flfaSource.PushBack(fll);
		flfaSource.PushBack(flr);
		flfaSource.PushBack(flc);
		flfaSource.PushBack(fle);
		flfaSource.PushBack(flcr);

		// Region ���� // Create region
		CFLComplexRegion flcrRegion1;

		flcrRegion1.PushBack(CFLPoint<double>(0, 0));
		flcrRegion1.PushBack(CFLPoint<double>(220, 50));
		flcrRegion1.PushBack(CFLPoint<double>(240, 100));
		flcrRegion1.PushBack(CFLPoint<double>(200, 150));
		flcrRegion1.PushBack(CFLPoint<double>(110, 170));
		flcrRegion1.PushBack(CFLPoint<double>(70, 200));

		CFLComplexRegion flcrRegion2;

		flcrRegion2.PushBack(CFLPoint<double>(150, 100));
		flcrRegion2.PushBack(CFLPoint<double>(240, 160));
		flcrRegion2.PushBack(CFLPoint<double>(430, 250));
		flcrRegion2.PushBack(CFLPoint<double>(300, 400));
		flcrRegion2.PushBack(CFLPoint<double>(200, 300));
		flcrRegion2.PushBack(CFLPoint<double>(140, 200));
		flcrRegion2.PushBack(CFLPoint<double>(110, 80));

		CFLString<wchar_t> strFigure;
		wprintf(L"Source Figure Array\n");

		strFigure.Format(L"%s\n\n", CFigureUtils::ConvertFigureObjectToString(&flfaSource).GetString());
		wprintf(L"%s", strFigure.GetString());

		wprintf(L"Region1\n");

		strFigure.Format(L"%s\n\n", CFigureUtils::ConvertFigureObjectToString(&flcrRegion1).GetString());
		wprintf(L"%s", strFigure.GetString());

		// SourceView1�� 0�� ���̾ Source Figure, Region1 �׸��� // Draw Source Figure, Region1 on Layer 0 of SourceView1
		SrcLayer0.DrawFigureImage(flfaSource, CYAN);
		SrcLayer0.DrawFigureImage(flcrRegion1, BLUE, 1, BLUE, EGUIViewImagePenStyle_Solid, 1, 0.2f);

		wprintf(L"Region2\n");

		strFigure.Format(L"%s\n\n", CFigureUtils::ConvertFigureObjectToString(&flcrRegion2).GetString());
		wprintf(L"%s", strFigure.GetString());

		// SourceView2�� 0�� ���̾ Source Figure, Region2 �׸��� // Draw Source Figure, Region2 on Layer 0 of SourceView2
		SrcLayer1.DrawFigureImage(flfaSource, CYAN);
		SrcLayer1.DrawFigureImage(flcrRegion2, BLUE, 1, BLUE, EGUIViewImagePenStyle_Solid, 1, 0.2f);

		// Region1�� �������� Figure ���� // Get figure overlapping with Region1
		CFLFigureArray flfaResult1;
		flfaSource.GetFigureWithinRegion(flcrRegion1, &flfaResult1);

		wprintf(L"Result Figure Within Region1\n");

		strFigure.Format(L"%s\n\n", CFigureUtils::ConvertFigureObjectToString(&flfaResult1).GetString());
		wprintf(L"%s", strFigure.GetString());

		// DstView1�� 0�� ���̾ ��� �׸��� // Draw the result on layer 0 of DstView1
		DstLayer0.DrawFigureImage(flfaSource, CYAN);
		DstLayer0.DrawFigureImage(flcrRegion1, BLUE, 1, BLUE, EGUIViewImagePenStyle_Solid, 1, 0.2f);
		DstLayer0.DrawFigureImage(flfaResult1, LIME, 3, LIME, EGUIViewImagePenStyle_Solid, 1, 0.2f);

		// Region2�� �������� Figure ���� // Get figure overlapping with Region2
		CFLFigureArray flfaResult2;
		flfaSource.GetFigureWithinRegion(&flcrRegion2, &flfaResult2);

		wprintf(L"Result Figure Within Region2\n");

		strFigure.Format(L"%s\n\n", CFigureUtils::ConvertFigureObjectToString(&flfaResult2).GetString());
		wprintf(L"%s", strFigure.GetString());

		// DstView1�� 0�� ���̾ ��� �׸��� // Draw the result on layer 0 of DstView1
		DstLayer1.DrawFigureImage(flfaSource, CYAN);
		DstLayer1.DrawFigureImage(flcrRegion2, BLUE, 1, BLUE, EGUIViewImagePenStyle_Solid, 1, 0.2f);
		DstLayer1.DrawFigureImage(flfaResult2, LIME, 3, LIME, EGUIViewImagePenStyle_Solid, 1, 0.2f);

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