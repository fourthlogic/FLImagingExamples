#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� ����� ��ü ���� // Declare image drawing object
	CFLImageDrawingWrap fliImageDrawing;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	do
	{
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Load image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/Blob/AlignBall.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Drawing �̹����� Src �̹����� ������ �̹����� ����
		if(IsFail(eResult = fliImageDrawing.Assign(fliImage)))
		{
			ErrorPrint(eResult, "Failed to assign the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImageSrc.Create(400, 0, 800, 400)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImageDst.Create(800, 0, 1200, 400)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ�. // Synchronize the viewpoints of the two image views.
		if(IsFail(eResult = viewImageSrc.SynchronizePointOfView(&viewImageDst)))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImageSrc.SynchronizeWindow(&viewImageDst)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(eResult = viewImageSrc.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Blob ��ü ���� // Create Blob object
		CBlob blob;

		// ó���� �̹��� ����
		blob.SetSourceImage(fliImage);
		// �� ���� ����
		blob.SetLogicalCondition(ELogicalCondition_Less);
		// �Ӱ谪 ����,  ���� ���ǰ� �Ʒ��� ������ �������� 127���� ���� ��ü�� ����
		blob.SetThreshold(127);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = blob.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute Blob.");
			break;
		}

		// ��� ��ü�� �� �ش�Ǵ� ������ ���� ��ü�� ����

		// ������ 500���� ���� ��ü���� ����
		if(IsFail(eResult = blob.Filter(CBlob::EFilterItem_Area, 500, ELogicalCondition_LessEqual)))
		{
			ErrorPrint(eResult, "Blob filtering algorithm error occurred.");
			break;
		}

		// Blob ����� ������ ���� FigureArray ����
		CFLFigureArray flfaBoundaryRects;

		// Blob ����� �� Boundary Rectangle �� ����
		if(IsFail(eResult = blob.GetResultBoundaryRects(flfaBoundaryRects)))
		{
			ErrorPrint(eResult, "Failed to get boundary rects from the Blob object.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾ ���� // Obtain layer from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer1 = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layer2 = viewImageSrc.GetLayer(1);
		CGUIViewImageLayerWrap layer3 = viewImageSrc.GetLayer(2);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer1.Clear();
		layer2.Clear();
		layer3.Clear();

		// �̹��� �� ���� ǥ�� // Display image view information
		if(IsFail(eResult = layer3.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// flfaBoundaryRects �� Figure���� �迭�̱� ������ Layer�� �ֱ⸸ �ص� ��� ������� �����ϴ�.
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// ���⼭ 0.25�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(eResult = layer1.DrawFigureImage(&flfaBoundaryRects, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(eResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// Rect �������� ���� Ȯ���ϴ� �ڵ�
		for(int64_t i = 0; i < flfaBoundaryRects.GetCount(); ++i)
		{
			CFLRectL* pFlrRect = (CFLRectL*)flfaBoundaryRects.GetAt(i);

			if(pFlrRect)
			{
				printf("No. [%lld]\n", i);
				printf("LeftTop     : (%d,%d)\n", pFlrRect->left, pFlrRect->top);
				printf("RightBottom : (%d,%d)\n", pFlrRect->right, pFlrRect->bottom);
				printf("Width  : %.lf\n", pFlrRect->GetWidth());
				printf("Height : %.lf\n", pFlrRect->GetHeight());
				printf("Center : (%.1lf, %.1lf)\n\n", pFlrRect->GetCenter().x, pFlrRect->GetCenter().y);

				CFLString<wchar_t> strNumber;
				strNumber.Format(L"[%d]", i);

				CFLString<wchar_t> strLeftTop, strRightBottom, strInfo;
				strLeftTop.Format(L"(%d, %d)", pFlrRect->left, pFlrRect->top);
				strRightBottom.Format(L"(%d, %d)", pFlrRect->right, pFlrRect->bottom);
				strInfo.Format(L"Width : %.lf\nHeight : %.lf\nCenter : (%.1lf, %.1lf)", pFlrRect->GetWidth(), pFlrRect->GetHeight(), pFlrRect->GetCenter().x, pFlrRect->GetCenter().y);

				// �Ʒ� �Լ� DrawTextImage�� Image��ǥ�� �������� �ϴ� Text�� Drawing �Ѵٴ� ���� �ǹ��Ѵ�.
				// �Ķ���� ���� : ���̾� -> ���ڿ� ��ǥ -> ���ڿ� ���� -> ���ڿ� �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ��� �׸����� ���� -> ���� -> ���ڿ��� ��ġ ����
				layer2.DrawTextImage(&CFLPointD(pFlrRect->GetCenter()), strNumber, CYAN, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
				layer2.DrawTextImage(&CFLPointD(pFlrRect->left, pFlrRect->top), strLeftTop, YELLOW, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_RIGHT_BOTTOM);
				layer2.DrawTextImage(&CFLPointD(pFlrRect->right, pFlrRect->bottom), strRightBottom, YELLOW, BLACK);
				layer2.DrawTextImage(&CFLPointD(pFlrRect->right, pFlrRect->top), strInfo, LIME, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_LEFT_BOTTOM);
			}
		}

		// �̹����� ����ϱ� ���� �̹��� ����� ��ä���� ���̾ ���� // Gets layers from image drawing object for output to image
		CGUIViewImageLayerWrap layer = fliImageDrawing.GetLayer();

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// ���� ���̾ ����׵� ���ҵ��� �ش� ���̾� ���ʿ� �߰��մϴ�. // Add elements drawn from an existing layer to the back of that layer.
		if(IsFail(eResult = layer.PushBack(layer1)))
		{
			ErrorPrint(eResult, "Failed to push back.\n");
			break;
		}

		if(IsFail(eResult = layer.PushBack(layer2)))
		{
			ErrorPrint(eResult, "Failed to push back.\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if(IsFail(eResult = layer.DrawTextImage(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹����� �׸��ϴ�. // Draw in the image.
		if(IsFail(eResult = fliImageDrawing.Draw()))
		{
			ErrorPrint(eResult, "Failed to draw.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(eResult = viewImageDst.SetImagePtr(&fliImageDrawing)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate();
		viewImageDst.Invalidate();

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSrc.IsAvailable() || viewImageDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}