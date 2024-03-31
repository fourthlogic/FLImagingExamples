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

		// BlobSubsampled ��ü ���� // Create BlobSubsampled object
		CBlobSubsampled blob;

		// ó���� �̹��� ����
		blob.SetSourceImage(fliImage);

		// �� ���� ����
		blob.SetLogicalCondition(ELogicalCondition_Less);

		// �Ӱ谪 ����,  ���� ���ǰ� �Ʒ��� ������ �������� 50���� ���� ��ü�� ����
		blob.SetThreshold(50);

		// BlobSubsampled Result Type mask ���� (Contour, Area)
		int32_t i32ResultTypeMask = (int32_t)CBlobSubsampled::EBlobResultType_Contour | (int32_t)CBlobSubsampled::EBlobResultType_Area;

		// Result Type ����
		blob.SetResultType((CBlobSubsampled::EBlobResultType)i32ResultTypeMask);

		// Subsampling Level ����
		blob.SetSubsamplingLevel(3);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = blob.Execute()))
		{
			ErrorPrint(res, "Failed to execute BlobSubsampled.");
			break;
		}

		// ������ 100���� ���� ��ü���� ����(���� ������ ����)
		if(IsFail(res = blob.Filter(CBlobSubsampled::EFilterItem_Area, 100, ELogicalCondition_Less)))
		{
			ErrorPrint(res, "BlobSubsampled filtering algorithm error occurred.");
			break;
		}

		// BlobSubsampled ����� ������ ���� CFLFigureArray / CFLArray<uint64_t> ����
		CFLFigureArray flfaContours;
		CFLArray<double> flaArea;

		// BlobSubsampled ����� �� Contour �� ����
		if(IsFail(res = blob.GetResultContours(flfaContours)))
		{
			ErrorPrint(res, "Failed to get contours from the BlobSubsampled object.");
			break;
		}

		// BlobSubsampled ����� �� Area �� ����
		if(IsFail(res = blob.GetResultAreas(flaArea)))
		{
			ErrorPrint(res, "Failed to get area from the BlobSubsampled object.");
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

		CFLString<wchar_t> str;
		CFLString<wchar_t> flsTextResult;

		// Image View ��ü�� Index, Area ������� ���
		for(int64_t i = 0; i < flfaContours.GetCount(); ++i)
		{
			str.Format(L"[%lld]\n", i);
			flsTextResult.Format(L"\nArea %.2lf", flaArea[i]);

			// Image View ���
			layer.DrawTextImage(CFLPointD(flfaContours[i]), str, LIME, BLACK, 10, false, 0., GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
			layer.DrawTextImage(CFLPointD(flfaContours[i]), flsTextResult, YELLOW, BLACK, 10, false, 0., GUI::EGUIViewImageTextAlignment_CENTER_CENTER);

			// �ܼ� ���
			printf("[%lld] Area %.2lf \n", i, flaArea[i]);
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
