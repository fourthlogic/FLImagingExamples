#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSorted;
	CGUIViewImageWrap viewImageRecover;

	do
	{
		CResult eResult = EResult_UnknownError;
		// �̹��� �ε� // Load image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/Blob/Ball.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImageSorted.Create(200, 0, 968, 576)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageRecover.Create(968, 0, 1736, 576)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageSorted.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageRecover.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Source �̹��� ��� Destination �̹��� ���� ������ ����ȭ �Ѵ�
		if(IsFail(eResult = viewImageSorted.SynchronizePointOfView(&viewImageRecover)))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// Image ũ�⿡ �°� view�� ũ�⸦ ���� // Zoom the view to fit the image size
		if(IsFail(eResult = viewImageSorted.ZoomFit()))
		{
			ErrorPrint(eResult, "Failed to zoom fit\n");
			break;
		}

		if(IsFail(eResult = viewImageRecover.ZoomFit()))
		{
			ErrorPrint(eResult, "Failed to zoom fit\n");
			break;
		}

		// Blob ��ü ���� // Create Blob object
		CBlob blob;

		// ROI ���� ����
		CFLRectL flrROI(450, 425, 1024, 800);

		// ó���� �̹��� ����
		blob.SetSourceImage(fliImage);
		// ó���� ROI ����
		blob.SetSourceROI(flrROI);
		// �� ���� ����
		blob.SetLogicalCondition(ELogicalCondition_GreaterEqual);
		// �Ӱ谪 ����,  ���� ���ǰ� �Ʒ��� ������ �������� 100���� ���ų� ū ��ü�� ����
		blob.SetThreshold(100);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = blob.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute Blob.");
			break;
		}

		// ��� ��ü�� �� �ش�Ǵ� ������ ���� ��ü�� ����

		// 50���� ���ų� ū �庯 ���̸� ���� ��ü���� ����
		if(IsFail(eResult = blob.Filter(CBlob::EFilterItem_MinimumEnclosingRectangleLongSideLength, 50, ELogicalCondition_GreaterEqual)))
		{
			ErrorPrint(eResult, "Blob filtering algorithm error occurs.");
			break;
		}

		// 50���� ���ų� ū �ܺ� ���̸� ���� ��ü���� ����
		if(IsFail(eResult = blob.Filter(CBlob::EFilterItem_MinimumEnclosingRectangleShortSideLength, 50, ELogicalCondition_GreaterEqual)))
		{
			ErrorPrint(eResult, "Blob filtering algorithm error occurred.");
			break;
		}

		// ������ 50���� ���� ��ü���� ����
		if(IsFail(eResult = blob.Filter(CBlob::EFilterItem_Area, 50, ELogicalCondition_LessEqual)))
		{
			ErrorPrint(eResult, "Blob filtering algorithm error occurred.");
			break;
		}

		// Blob ����� ������ ���� FigureArray ����
		CFLFigureArray flfaSortedBoundaryRects;
		CFLFigureArray flfaRecoverBoundaryRects;

		CFLArray<int32_t> flaItem;
		CFLArray<int32_t> flaOrder;

		// ù ��° ������ Bound rect center y��ǥ, �������� ����
		flaItem.PushBack(CBlob::EFilterItem_BoundaryRectCenterY);
		flaOrder.PushBack(CBlob::EOrder_Descending);

		// �� ��° ������ Bound rect center x��ǥ, �������� ����
		flaItem.PushBack(CBlob::EFilterItem_BoundaryRectCenterX);
		flaOrder.PushBack(CBlob::EOrder_Descending);

		// Blob ����� ����
		if(IsFail(eResult = blob.Sort(flaItem, flaOrder)))
		{
			ErrorPrint(eResult, "Failed to sort from the Blob object.");
			break;
		}

		// ���ĵ� Blob ����� �� Boundary Rectangle �� ����
		if(IsFail(eResult = blob.GetResultBoundaryRects(flfaSortedBoundaryRects)))
		{
			ErrorPrint(eResult, "Failed to get boundary rects from the Blob object.");
			break;
		}

		// Blob ���� ���¸� �ʱ� ���·� ����
		if(IsFail(eResult = blob.Sort(CBlob::EFilterItem_Unselected)))
		{
			ErrorPrint(eResult, "Failed to get boundary rects from the Blob object.");
			break;
		}

		// ������ Blob ����� �� Boundary Rectangle �� ����
		if(IsFail(eResult = blob.GetResultBoundaryRects(flfaRecoverBoundaryRects)))
		{
			ErrorPrint(eResult, "Failed to get boundary rects from the Blob object.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately

		CGUIViewImageLayerWrap layerSorted = viewImageSorted.GetLayer(0);

		CGUIViewImageLayerWrap layerRecover = viewImageRecover.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSorted.Clear();
		layerRecover.Clear();

		if(IsFail(eResult = layerSorted.DrawTextCanvas(&CFLPointD(), L"Sort", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(eResult = layerRecover.DrawTextCanvas(&CFLPointD(), L"No Sort", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text on the image view.\n");
			break;
		}

		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
		// FLImaging�� Figure��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ����
		if(IsFail(eResult = layerSorted.DrawFigureImage(&flrROI, BLUE)))
		{
			ErrorPrint(eResult, "Failed to draw figures objects on the image view.\n");
			break;
		}

		if(IsFail(eResult = layerRecover.DrawFigureImage(&flrROI, BLUE)))
		{
			ErrorPrint(eResult, "Failed to draw figures objects on the image view.\n");
			break;
		}

		// flfaOriginBoundaryRects �� Figure���� �迭�̱� ������ Layer�� �ֱ⸸ �ص� ��� ������� �����ϴ�.
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// ���⼭ 0.25�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(eResult = layerSorted.DrawFigureImage(&flfaSortedBoundaryRects, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(eResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		if(IsFail(eResult = layerRecover.DrawFigureImage(&flfaRecoverBoundaryRects, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(eResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// Rect �������� ���� Ȯ���ϴ� �ڵ�
		for(int64_t i = 0; i < flfaSortedBoundaryRects.GetCount(); ++i)
		{
			CFLRectL* pFlrRect = (CFLRectL*)flfaSortedBoundaryRects.GetAt(i);

			if(pFlrRect)
				printf("Sorted No. %lld : (%d,%d,%d,%d)\n", i, pFlrRect->left, pFlrRect->top, pFlrRect->right, pFlrRect->bottom);

			CFLString<wchar_t> strNumber;
			strNumber.Format(L"%d", i);

			layerSorted.DrawTextImage(&CFLPointD(pFlrRect->GetCenter()), (wchar_t*)strNumber.GetString(), CYAN);

			pFlrRect = (CFLRectL*)flfaRecoverBoundaryRects.GetAt(i);

			if(pFlrRect)
				printf("Recover No. %lld : (%d,%d,%d,%d)\n", i, pFlrRect->left, pFlrRect->top, pFlrRect->right, pFlrRect->bottom);

			layerRecover.DrawTextImage(&CFLPointD(pFlrRect->GetCenter()), (wchar_t*)strNumber.GetString(), CYAN);
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSorted.Invalidate(true);
		viewImageRecover.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSorted.IsAvailable() && viewImageRecover.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}