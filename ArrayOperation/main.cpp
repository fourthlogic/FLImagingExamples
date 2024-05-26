#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	const int32_t i32ViewCount = 4;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage[i32ViewCount];
	CResult res = EResult_UnknownError;

	do
	{
		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage[0].Create(400, 0, 912, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(912, 0, 1424, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}
		if(IsFail(res = viewImage[2].Create(400, 384, 912, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[3].Create(912, 384, 1424, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		for(int32_t i = 1; i < i32ViewCount; ++i)
		{
			// �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoint of the image view
			if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[i])))
			{
				ErrorPrint(res, "Failed to synchronize view\n");
				break;
			}

			// �̹��� �� �������� ��ġ�� ���� // Align the position of the image view window
			if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[i])))
			{
				ErrorPrint(res, "Failed to synchronize window.\n");
				break;
			}
		}

		// SourceView, DstView �� 0�� ���̾� �������� // Get Layer 0 of SourceView, DstView
		CGUIViewImageLayerWrap ViewLayer[i32ViewCount];

		for(int32_t i = 0; i < i32ViewCount; ++i)
			ViewLayer[i] = viewImage[i].GetLayer(0);

		// Figure ���� // Create Figure
		CFLRect<double> flr(50, 50, 100, 100, 15);
		CFLQuad<double> flq(200, 50, 360, 50, 400, 160, 150, 110);
		CFLCircle<double> flc(100., 150., 30., 0, 30, 90, EArcClosingMethod_Center);
		CFLEllipse<double> fle(300, 250, 100, 50, 0, 30, 200, EArcClosingMethod_Center);

		CFLFigureArray flfa;

		flfa.PushBack(flr);
		flfa.PushBack(flq);
		flfa.PushBack(flc);
		flfa.PushBack(fle);

		CFLString<wchar_t> strFigure;
		wprintf(L"Figure Array\n");

		for(int32_t i = 0; i < flfa.GetCount(); ++i)
		{
			strFigure.Format(L"[%d]\n %s\n", i, CFigureUtilities::ConvertFigureObjectToString(flfa.GetAt(i)).GetString());
			wprintf(L"%s", strFigure.GetString());
		}

		wprintf(L"\n");

		// Figure �׸��� // Draw Figure
		for(int32_t i = 0; i < flfa.GetCount(); ++i)
			ViewLayer[i].DrawFigureImage(flfa, LIME);

		//////////////////////////////// GetCenterElementwise()
		// �߽��� ��ǥ�� ���� FigureArray ���� // Create a FigureArray to hold the coordinates of the center point
		CFLFigureArray flfaCenter;

		// Figure Array �� ����� �߽��� ��� // Calculate the center point of each element of Figure Array
		flfa.GetCenterElementwise(&flfaCenter);

		// �߽ɵ��� View0�� 0�� ���̾ �׸��� // Draw the centers on layer 0 of View0
		ViewLayer[0].DrawFigureImage(flfaCenter, RED);
		ViewLayer[0].DrawTextCanvas(TPointD(0, 0), L"GetCenterElementwise() Result", YELLOW, BLACK, 15);

		// �ֿܼ� �߽� ��ǥ ǥ�� // Print center coordinates in console
		wprintf(L"Center Point\n");

		for(int32_t i = 0; i < flfa.GetCount(); ++i)
		{
			strFigure.Format(L"[%d]\n %s\n", i, CFigureUtilities::ConvertFigureObjectToString(flfaCenter.GetAt(i)).GetString());
			wprintf(L"%s", strFigure.GetString());
		}

		wprintf(L"\n");


		//////////////////////////////// GetPerimeterElementwise()
		// �� �ѷ��� ���̸� ������ CFLFigureArray ���� // Create CFLFigureArray to store the length of each perimeter
		CFLFigureArray flfaPerimeter;

		// Figure Array �� ����� �ѷ� ��� // Calculate the perimeter of each element of the Figure Array
		flfa.GetPerimeterElementwise(&flfaPerimeter);

		// Figure Array �� ����� �ѷ� ǥ�� // Display perimeter of each element of Figure Array
		for(int64_t i = 0; i < flfaPerimeter.GetCount(); ++i)
		{
			CFLString<wchar_t> strPerimeter;
			strPerimeter.Format(L"%lf", ((CFLScalarD*)flfaPerimeter[i])->v);
			ViewLayer[1].DrawTextImage(flfaCenter[i], strPerimeter, BLACK);
		}
		ViewLayer[1].DrawTextCanvas(TPointD(0, 0), L"GetPerimeterElementwise() Result", YELLOW, BLACK, 15);

		// �ֿܼ� ���� ǥ�� // Display the length in the console
		wprintf(L"Perimeter\n");

		for(int64_t i = 0; i < flfaPerimeter.GetCount(); ++i)
		{
			strFigure.Format(L"[%d]\n %lf\n", i, ((CFLScalarD*)flfaPerimeter[i])->v);
			wprintf(L"%s", strFigure.GetString());
		}

		wprintf(L"\n");


		//////////////////////////////// GetCenterOfGravityElementwise()
		// �����߽��� ��ǥ�� ���� FigureArray ���� // Create a FigureArray to contain the coordinates of the center of gravity
		CFLFigureArray flfaCenterOfGravity;

		// Figure Array �� ����� �����߽��� ��� // Calculate the center of gravity of each element of the Figure Array
		flfa.GetCenterOfGravityElementwise(&flfaCenterOfGravity);

		// �����߽ɵ��� View0�� 0�� ���̾ �׸��� // Draw the centers of gravity on Layer 0 of View0
		ViewLayer[2].DrawFigureImage(flfaCenterOfGravity, CYAN);
		ViewLayer[2].DrawTextCanvas(TPointD(0, 0), L"GetCenterOfGravityElementwise() Result", YELLOW, BLACK, 15);

		// �ֿܼ� �����߽� ��ǥ ǥ�� // Display barycentric coordinates in console
		wprintf(L"Center Of Gravity Point\n");

		for(int32_t i = 0; i < flfa.GetCount(); ++i)
		{
			strFigure.Format(L"[%d]\n %s\n", i, CFigureUtilities::ConvertFigureObjectToString(flfaCenterOfGravity.GetAt(i)).GetString());
			wprintf(L"%s", strFigure.GetString());
		}

		wprintf(L"\n");


		//////////////////////////////// GetMinimumEnclosingRectangleElementwise()
		// �ּҵѷ� ���簢���� ���� FigureArray ���� // Create a FigureArray to contain the minimum enclosing rectangle
		CFLFigureArray flfaMER;

		// Figure Array �� ����� �ּҵѷ� ���簢���� ��� // Calculate the minimum enclosing rectangle of each element of the Figure Array
		flfa.GetMinimumEnclosingRectangleElementwise(&flfaMER);

		// �ּҵѷ� ���簢������ View0�� 0�� ���̾ �׸��� // Draw the minimum enclosing rectangle on Layer 0 of View0
		ViewLayer[3].DrawFigureImage(flfaMER, BLUE);
		ViewLayer[3].DrawTextCanvas(TPointD(0, 0), L"GetMinimumEnclosingRectangleElementwise() Result", YELLOW, BLACK, 15);

		// �ֿܼ� �ּҵѷ� ���簢���� ǥ�� // Display the minimum enclosing rectangle in console
		wprintf(L"Minimum Enclosing Rectangle\n");

		for(int32_t i = 0; i < flfa.GetCount(); ++i)
		{
			strFigure.Format(L"[%d]\n %s\n", i, CFigureUtilities::ConvertFigureObjectToString(flfaMER.GetAt(i)).GetString());
			wprintf(L"%s", strFigure.GetString());
		}

		wprintf(L"\n");

		// �̹��� �並 ���� �մϴ�. // Update image view
		for(int32_t i = 0; i < i32ViewCount; ++i)
		{
			viewImage[i].Invalidate(true);
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable() && viewImage[2].IsAvailable() && viewImage[3].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}