#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// Figure ��ü ���� // Declare figure object
	CFLFigureArray flfaSource;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage[2];

	do
	{
		CResult res = EResult_UnknownError;

		// Figure �ε� // Load figure
		if(IsFail(res = flfaSource.Load(L"../../ExampleImages/Figure/various shapes2.fig")))
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage[0].Create(200, 0, 968, 576)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(968, 0, 1736, 576)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Source �̹��� ��� Destination �̹��� ���� ������ ����ȭ �Ѵ�
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// ȭ��� �� ���̵��� ��ǥ 1.5������ ���� // Apply 1.5 magnification to the coordinates so that they can be seen clearly on the screen
		double f64Scale = 1.5;
		// ȭ��� �� ���̵��� ���� Offset ���� // Adjust the viewpoint offset so that it can be seen clearly on the screen
		double f64CenterCoordX = 200.;
		double f64CenterCoordY = 180.;

		viewImage[0].SetViewCenterAndScale(CFLPoint<double>(f64CenterCoordX, f64CenterCoordY), f64Scale);

		CFLFigureArray flfaCenterFirst(flfaSource);
		CFLFigureArray flfaAreaFirst(flfaSource);

		// FigureArray�� 1���� CenterY ��������, 2���� CenterX ��������, 3���� Area ������������ ����
		// Sort FigureArray in ascending order as 1st priority CenterY, ascending order as 2nd priority CenterX, and descending order as 3rd priority Area.
		if(IsFail(res = flfaCenterFirst.Sort(ESortOrderFigure_CenterY_Asc, ESortOrderFigure_CenterX_Asc, ESortOrderFigure_Area_Desc)))
		{
			ErrorPrint(res, "Failed to sort.");
			break;
		}

		// FigureArray�� 1���� Area ��������, 2���� CenterY ��������, 3���� CenterX ������������ ����
		// Sort FigureArray in descending order as 1st priority Area, ascending order as 2nd priority CenterY, and ascending order as 3rd priority CenterX.
		if(IsFail(res = flfaAreaFirst.Sort(ESortOrderFigure_Area_Desc, ESortOrderFigure_CenterY_Asc, ESortOrderFigure_CenterX_Asc)))
		{
			ErrorPrint(res, "Failed to sort.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer[2];

		for(int32_t i = 0; i < 2; ++i)
			layer[i] = viewImage[i].GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		for(int32_t i = 0; i < 2; ++i)
			layer[i].Clear();

		if(IsFail(res = layer[0].DrawTextCanvas(&CFLPoint<double>(), L"1st Y, 2nd X, 3rd Area", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(res = layer[1].DrawTextCanvas(&CFLPoint<double>(), L"1st Area, 2nd Y, 3rd X", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// flfaCenterFirst �� Figure���� �迭�̱� ������ Layer�� �ֱ⸸ �ص� ��� ������� �����ϴ�.
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// ���⼭ 0.25�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(res = layer[0].DrawFigureImage(&flfaCenterFirst, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		if(IsFail(res = layer[1].DrawFigureImage(&flfaAreaFirst, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// �������� ���� Ȯ���ϴ� �ڵ� // Code to check each information value
		for(int64_t i = 0; i < flfaCenterFirst.GetCount(); ++i)
		{
			CFLFigure* pFlf = flfaCenterFirst.GetAt(i);

			if(pFlf)
			{
				CFLPoint<double> flpCenter = pFlf->GetCenter();

				wprintf(L"CenterFirst No. %lld : (%lf, %lf), Area : %lf\n", i, flpCenter.x, flpCenter.y, pFlf->GetArea());

				CFLString<wchar_t> strNumber;
				strNumber.Format(L"%d", i);

				layer[0].DrawTextImage(&flpCenter, (wchar_t*)strNumber.GetString(), CYAN, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
			}
		}

		wprintf(L"\n");

		for(int64_t i = 0; i < flfaAreaFirst.GetCount(); ++i)
		{
			CFLFigure* pFlf = flfaAreaFirst.GetAt(i);

			if(pFlf)
			{
				CFLPoint<double> flpCenter = pFlf->GetCenter();

				wprintf(L"AreaFirst No. %lld : (%lf, %lf), Area : %lf\n", i, flpCenter.x, flpCenter.y, pFlf->GetArea());

				CFLString<wchar_t> strNumber;
				strNumber.Format(L"%d", i);

				layer[1].DrawTextImage(&flpCenter, (wchar_t*)strNumber.GetString(), CYAN, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
			}
		}

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