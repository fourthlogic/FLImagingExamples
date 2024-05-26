#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	do
	{
		CResult res = EResult_UnknownError;
		// �̹��� �ε� // Load image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Blob/AlignBall.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(res = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
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
		if(IsFail(res = blob.Execute()))
		{
			ErrorPrint(res, "Failed to execute Blob.");
			break;
		}

		// ��� ��ü�� �� �ش�Ǵ� ������ ���� ��ü�� ����

		// ������ 500���� ���� ��ü���� ����
		if(IsFail(res = blob.Filter(CBlob::EFilterItem_Area, 500, ELogicalCondition_LessEqual)))
		{
			ErrorPrint(res, "Blob filtering algorithm error occurred.");
			break;
		}

		// Blob ����� ������ ���� FigureArray ����
		CFLFigureArray flfaBoundaryRects;

		// Blob ����� �� Boundary Rectangle �� ����
		if(IsFail(res = blob.GetResultBoundaryRects(flfaBoundaryRects)))
		{
			ErrorPrint(res, "Failed to get boundary rects from the Blob object.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// flfaBoundaryRects �� Figure���� �迭�̱� ������ Layer�� �ֱ⸸ �ص� ��� ������� �����ϴ�.
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// ���⼭ 0.25�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(res = layer.DrawFigureImage(&flfaBoundaryRects, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);

		// Rect �������� ���� Ȯ���ϴ� �ڵ�
		for(int64_t i = 0; i < flfaBoundaryRects.GetCount(); ++i)
		{
			CFLRect<int32_t>* pFlrRect = (CFLRect<int32_t>*)flfaBoundaryRects.GetAt(i);

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
				layer.DrawTextImage(&CFLPoint<double>(pFlrRect->GetCenter()), strNumber, CYAN, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
				layer.DrawTextImage(&CFLPoint<double>(pFlrRect->left, pFlrRect->top), strLeftTop, YELLOW, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_RIGHT_BOTTOM);
				layer.DrawTextImage(&CFLPoint<double>(pFlrRect->right, pFlrRect->bottom), strRightBottom, YELLOW, BLACK);
				layer.DrawTextImage(&CFLPoint<double>(pFlrRect->right, pFlrRect->top), strInfo, LIME, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_LEFT_BOTTOM);
			}
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}