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

		// �Ӱ谪 ����,  ���� ���ǰ� �Ʒ��� ������ �������� 50���� ���� ��ü�� ����
		blob.SetThreshold(50);

		// Result Type ����
		blob.SetResultType(CBlob::EBlobResultType_Contour);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = blob.Execute()))
		{
			ErrorPrint(res, "Failed to execute Blob.");
			break;
		}

		// ������ 50 ���� ���� ��ü���� ����(���� ������ ����)
		if(IsFail(res = blob.Filter(CBlob::EFilterItem_Area, 50, ELogicalCondition_Less)))
		{
			ErrorPrint(res, "Blob filtering algorithm error occurred.");
			break;
		}

		// Circularity ����� 0.7���� ���� ��ü���� ����(������ ������ ���� ��ü ����, �ִ밪 : 1.0)
		if(IsFail(res = blob.Filter(CBlob::EFilterItem_Circularity, 0.7, ELogicalCondition_Less)))
		{
			ErrorPrint(res, "Blob filtering algorithm error occurred.");
			break;
		}

		// Blob ����� ������ ���� CFLFigureArray ����
		CFLFigureArray flfaContours;

		// Blob ����� �� Contour �� ����
		if(IsFail(res = blob.GetResultContours(flfaContours)))
		{
			ErrorPrint(res, "Failed to get contours from the Blob object.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// flfaContours �� Figure���� �迭�̱� ������ Layer�� �ֱ⸸ �ص� ��� ������� �����ϴ�.
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// ���⼭ 0.25�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(res = layer.DrawFigureImage(&flfaContours, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);

		CFLString<wchar_t> str;
		CFLString<wchar_t> flsTextResult;

		// Image View ��ü�� Index, Contour Length ������� ���
		for(int64_t i = 0; i < flfaContours.GetCount(); ++i)
		{
			str.Format(L"[%lld]\n", i);
			flsTextResult.Format(L"\nContour Length %.2lf", flfaContours[i]->GetPerimeter());

			// Image View ���
			layer.DrawTextImage(CFLPoint<double>(flfaContours[i]), str, LIME, BLACK, 10, false, 0., GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
			layer.DrawTextImage(CFLPoint<double>(flfaContours[i]), flsTextResult, YELLOW, BLACK, 10, false, 0., GUI::EGUIViewImageTextAlignment_CENTER_CENTER);

			// �ֿܼ� ���
			printf("[%lld] Contour Length %.2lf \n", i, flfaContours[i]->GetPerimeter());
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}
