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
		CResult eResult = EResult_UnknownError;
		// �̹��� �ε� // Load image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/Blob/Ball.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(eResult = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
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

		// 50���� ���ų� ū ¡�� ���̸� ���� ��ü���� ����
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
		CFLFigureArray flfaContours;

		// Blob ����� �� Contour�� ����
		if(IsFail(eResult = blob.GetResultContours(flfaContours)))
		{
			ErrorPrint(eResult, "Failed to get boundary rects from the Blob object.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();


		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
		// FLImaging�� Figure��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ����
		if(IsFail(eResult = layer.DrawFigureImage(&flrROI, BLUE)))
		{
			ErrorPrint(eResult, "Failed to draw figures objects on the image view.\n");
			break;
		}

		// flfaContours �� Figure���� �迭�̱� ������ Layer�� �ֱ⸸ �ص� ��� ������� �����ϴ�.
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// ���⼭ 0.25�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(eResult = layer.DrawFigureImage(&flfaContours, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(eResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// Rect �������� ���� Ȯ���ϴ� �ڵ�
		for(int64_t i = 0; i < flfaContours.GetCount(); ++i)
		{
			CFLRegion* pFlrg = (CFLRegion*)flfaContours.GetAt(i);

			// �������� ���� ������ �ֿܼ� ���
			printf("No. %lld : [", i);

			for(int64_t j = 0; j < pFlrg->GetCount(); ++j)
			{
				if(j)
					printf(",");

				CFLPointD* pFlpVertex = (CFLPointD*)pFlrg->GetAt(j);

				printf("(%lf,%lf)", pFlpVertex->x, pFlpVertex->y);
			}

			printf("]\n\n");

			CFLPointD flpCenter;
			pFlrg->GetCenter(flpCenter);

			CFLString<wchar_t> strNumber;
			strNumber.Format(L"%d", i);

			layer.DrawTextImage(&flpCenter, (wchar_t*)strNumber.GetString(), CYAN);
		}


		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}