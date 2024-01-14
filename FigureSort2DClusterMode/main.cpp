#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// Figure ��ü ���� // Declare figure object
	CFLFigureArray flfaSource;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageNormalSort2D;
	CGUIViewImageWrap viewImageSort2DClusterMode;

	do
	{
		CResult res = EResult_UnknownError;

		// Figure �ε� // Load figure
		if(IsFail(res = flfaSource.Load(L"../../ExampleImages/Figure/RectangleArray.fig")))
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImageNormalSort2D.Create(200, 0, 968, 576)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageSort2DClusterMode.Create(968, 0, 1736, 576)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Source �̹��� ��� Destination �̹��� ���� ������ ����ȭ �Ѵ�
		if(IsFail(res = viewImageNormalSort2D.SynchronizePointOfView(&viewImageSort2DClusterMode)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageNormalSort2D.SynchronizeWindow(&viewImageSort2DClusterMode)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		CFLFigureArray flfaNormalSort2D(flfaSource);
		CFLFigureArray flfaSort2DClusterMode(flfaSource);

		// FigureArray�� �Ϲ� Sort ����. 1���� Y��������, 2���� X��������
		// Normal Sort sort the FigureArray. 1st rank Y ascending, 2nd rank X ascending
		if(IsFail(res = flfaNormalSort2D.Sort2D(ESortOrder2D_Y_Asc_X_Asc)))
		{
			ErrorPrint(res, "Failed to process normal sort.");
			break;
		}

		// FigureArray�� Sort2DClusterMode ����. 1���� Y��������, 2���� X��������
		// Sort the FigureArray with Sort2DClusterMode. 1st rank Y ascending, 2nd rank X ascending
		if(IsFail(res = flfaSort2DClusterMode.Sort2DClusterMode(ESortOrder2D_Y_Asc_X_Asc)))
		{
			ErrorPrint(res, "Failed to process Sort2DClusterMode.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately

		CGUIViewImageLayerWrap layerNormalSort2D = viewImageNormalSort2D.GetLayer(0);
		CGUIViewImageLayerWrap layerSort2DClusterMode = viewImageSort2DClusterMode.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerNormalSort2D.Clear();
		layerSort2DClusterMode.Clear();

		if(IsFail(res = layerNormalSort2D.DrawTextCanvas(&CFLPoint<double>(), L"Normal Sort2D (BoundaryRectCenter Y, X)", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(res = layerSort2DClusterMode.DrawTextCanvas(&CFLPoint<double>(), L"Sort2DClusterMode (Y Asc, X Asc)", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// flfaNormalSort2D �� Figure���� �迭�̱� ������ Layer�� �ֱ⸸ �ص� ��� ������� �����ϴ�.
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// ���⼭ 0.25�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(res = layerNormalSort2D.DrawFigureImage(&flfaNormalSort2D, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		if(IsFail(res = layerSort2DClusterMode.DrawFigureImage(&flfaSort2DClusterMode, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// �������� ���� Ȯ���ϴ� �ڵ� // Code to check each information value
		for(int64_t i = 0; i < flfaNormalSort2D.GetCount(); ++i)
		{
			CFLRegion* pFlrg = (CFLRegion*)flfaNormalSort2D.GetAt(i);

			if(pFlrg)
			{
				CFLPoint<double> flpCenter = pFlrg->GetCenter();

				wprintf(L"NormalSort2D No. %lld : (%lf, %lf)\n", i, flpCenter.x, flpCenter.y);

				CFLString<wchar_t> strNumber;
				strNumber.Format(L"%d", i);

				layerNormalSort2D.DrawTextImage(&flpCenter, (wchar_t*)strNumber.GetString(), CYAN, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
			}
		}

		wprintf(L"\n");

		// �������� ���� Ȯ���ϴ� �ڵ� // Code to check each information value
		for(int64_t i = 0; i < flfaSort2DClusterMode.GetCount(); ++i)
		{
			CFLFigureArray* pFlfaCluster = (CFLFigureArray*)flfaSort2DClusterMode.GetAt(i);

			if(pFlfaCluster)
			{
				CFLRect<double> flrBoundary;
				pFlfaCluster->GetBoundaryRect(flrBoundary);

				wprintf(L"Sort2DClusterMode Cluster No. %lld\n", i);

				for(int64_t j= 0; j < pFlfaCluster->GetCount(); ++j)
				{
					CFLRegion* pFlrg = (CFLRegion*)pFlfaCluster->GetAt(j);

					if(pFlrg)
					{
						CFLPoint<double> flpCenter = pFlrg->GetCenter();

						wprintf(L"Sort2DClusterMode No. %lld : (%lf, %lf)\n", j, flpCenter.x, flpCenter.y);

						CFLString<wchar_t> strNumber;
						strNumber.Format(L"(%lld,%lld)", i, j);

						layerSort2DClusterMode.DrawTextImage(&flpCenter, (wchar_t*)strNumber.GetString(), CYAN, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
					}
				}
			}
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageNormalSort2D.Invalidate(true);
		viewImageSort2DClusterMode.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageNormalSort2D.IsAvailable() && viewImageSort2DClusterMode.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}