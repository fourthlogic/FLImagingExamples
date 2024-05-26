#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


enum EType
{
	EType_Src0 = 0,
	EType_Src1,
	EType_Src2,
	EType_Src3,
	ETypeCount,
};

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];
	bool bError = false;

	do
	{
		CResult res = EResult_UnknownError;

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			CFLString<wchar_t> flsFileName;

			flsFileName.Format(L"../../ExampleImages/Blob/Blob Sort %d.flif", i + 1);

			// �̹��� �ε� // Load image
			if(IsFail(res = arrFliImage[i].Load(flsFileName)))
			{
				ErrorPrint(res, "Failed to load the image file.\n");
				break;
			}

			int32_t i32X = i % 2;
			int32_t i32Y = i / 2;

			// �̹��� �� ���� // Create image view
			if(IsFail(res = arrViewImage[i].Create(i32X * 400 + 400, i32Y * 400, i32X * 400 + 400 + 400, i32Y * 400 + 400)))
			{
				ErrorPrint(res, "Failed to create the image view.\n");
				bError = true;
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
			if(IsFail(res = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}

			if(IsFail(res = arrViewImage[i].ZoomFit()))
			{
				ErrorPrint(res, "Failed to zoom fit\n");
				break;
			}

			// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
			if(i != EType_Src0)
			{
				if(IsFail(res = arrViewImage[EType_Src0].SynchronizePointOfView(&arrViewImage[i])))
				{
					ErrorPrint(res, "Failed to synchronize view\n");
					bError = true;
					break;
				}

				// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
				if(IsFail(res = arrViewImage[EType_Src0].SynchronizeWindow(&arrViewImage[i])))
				{
					ErrorPrint(res, "Failed to synchronize window.\n");
					bError = true;
					break;
				}
			}
		}

		if(bError)
			break;

		for(int64_t k = 0; k < ETypeCount; ++k)
		{
			// Blob ��ü ���� 
			// Create Blob object
			CBlobSubsampled blob;

			// ó���� �̹��� ����
			blob.SetSourceImage(arrFliImage[k]);
			// �� ���� ����
			blob.SetLogicalCondition(ELogicalCondition_Greater);
			// �Ӱ谪 ����,  ���� ���ǰ� �Ʒ��� ������ �������� 100���� ���ų� ���� ��ü�� ����
			blob.SetThreshold(100);

			// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
			if(IsFail(res = blob.Execute()))
			{
				ErrorPrint(res, "Failed to execute Blob.");
				break;
			}

			// Blob ����� ������ ���� FigureArray ����
			CFLFigureArray flfaSortClusterModeBoundaryRects;

			//////////////////////////////////////////////////////////////////////////
			// SortClusterMode ����� �̿��ؼ� Blob�� ����� ���� ������ ����
			// ��ü�� Center��ǥ�� �������� 1���� Y��������, 2���� X������������ ������ ����
			//////////////////////////////////////////////////////////////////////////

			// SortClusterMode �Լ��� ���� ������ ���� (��ü�� Center��ǥ�� �������� 1���� Y��������, 2���� X��������)
			if(IsFail(res = blob.SortClusterMode(CBlob::ESortClusterModeMethod_Center_Y_Asc_X_Asc)))
			{
				ErrorPrint(res, "Failed to get sort from the Blob object.");
				break;
			}

			// SortClusterMode�� Blob ����� �� Boundary Rectangle �� ����
			if(IsFail(res = blob.GetResultBoundaryRects(flfaSortClusterModeBoundaryRects)))
			{
				ErrorPrint(res, "Failed to get boundary rects from the Blob object.");
				break;
			}

			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately

			CGUIViewImageLayerWrap layerSortClusterMode = arrViewImage[k].GetLayer(0);

			if(IsFail(res = layerSortClusterMode.DrawTextCanvas(&CFLPoint<double>(), L"(Y Asc, X Asc)", YELLOW, BLACK, 30)))
			{
				ErrorPrint(res, "Failed to draw text on the image view.\n");
				break;
			}

			// flfaOriginBoundaryRects �� Figure���� �迭�̱� ������ Layer�� �ֱ⸸ �ص� ��� ������� �����ϴ�.
			// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
			// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
			// ���⼭ 0.25�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
			// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)			
			if(IsFail(res = layerSortClusterMode.DrawFigureImage(&flfaSortClusterModeBoundaryRects, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
			{
				ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
				break;
			}

			// Rect �������� ���� Ȯ���ϴ� �ڵ�
			for(int64_t i = 0; i < flfaSortClusterModeBoundaryRects.GetCount(); ++i)
			{
				CFLFigureArray* pFlfa = (CFLFigureArray*)flfaSortClusterModeBoundaryRects[i];
				int64_t i64ObjectCount = pFlfa->GetCount();

				for(int64_t j = 0; j < i64ObjectCount; ++j)
				{
					CFLRect<double>* pFlrRect = (CFLRect<double>*)pFlfa->GetAt(j);

					CFLString<wchar_t> strNumber;
					strNumber.Format(L"(%d,%d)", (int32_t)i, (int32_t)j);

					if(pFlrRect)
						printf("SortClusterMode [%lld] No. (%d,%d) : (%lf,%lf,%lf,%lf)\n", k, (int32_t)i, (int32_t)j, pFlrRect->left, pFlrRect->top, pFlrRect->right, pFlrRect->bottom);

					layerSortClusterMode.DrawTextImage(&CFLPoint<double>(pFlrRect->GetCenter()), (wchar_t*)strNumber.GetString(), CYAN, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_CENTER);
				}
			}

			// �̹��� �並 ���� �մϴ�. // Update image view
			arrViewImage[k].RedrawWindow();
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		bool bRun = true;

		while(bRun)
		{
			for(int32_t i = 0; i < ETypeCount; ++i)			
				bRun &= arrViewImage[i].IsAvailable();
			
			CThreadUtilities::Sleep(1);
		}
	}
	while(false);

	return 0;
}