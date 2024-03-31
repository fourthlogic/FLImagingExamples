#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewSourceImage;

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
		if(IsFail(res = viewSourceImage.Create(200, 0, 812, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewSourceImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Image ũ�⿡ �°� view�� ũ�⸦ ���� // Zoom the view to fit the image size
		if(IsFail(res = viewSourceImage.ZoomFit()))
		{
			ErrorPrint(res, "Failed to zoom fit\n");
			break;
		}

		// Blob ��ü ���� // Create Blob object
		CBlobSubsampled blob;

		// ó���� �̹��� ����
		blob.SetSourceImage(fliImage);

		// �� ���� ����
		CMultiVarLL mvLogical((int64_t)ELogicalCondition_Less);
		blob.SetLogicalCondition(mvLogical);
		// �Ӱ谪 ����,  ���� ���ǰ� �Ʒ��� ������ �������� 127���� ���� ��ü�� ����
		CMultiVar<double> mvThreshold(127.0);
		blob.SetThreshold(mvThreshold);
		blob.SetThresholdMode(EThresholdMode_Single);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = blob.Execute()))
		{
			ErrorPrint(res, "Failed to execute Blob.");
			break;
		}

		// ��� ��ü�� �� �ش�Ǵ� ������ ���� ��ü�� ����

		// 20���� �۰ų� ū �庯 ���̸� ���� ��ü���� ����
		if(IsFail(res = blob.Filter(CBlob::EFilterItem_BoundaryRectWidth, 20, ELogicalCondition_LessEqual)))
		{
			ErrorPrint(res, "Blob filtering algorithm error occurs.");
			break;
		}

		// circularity �� 0.9���� ���� ��ü���� ����
		if(IsFail(res = blob.Filter(CBlob::EFilterItem_Circularity, 0.9, ELogicalCondition_Less)))
		{
			ErrorPrint(res, "Blob filtering algorithm error occurred.");
			break;
		}

		CFLArray<int32_t> flaItem;
		CFLArray<int32_t> flaOrder;

		// Blob ����� Median, LowerQuartile, UpperQuartile �� ������ ���� Statistics ��ü ����
		CImageStatistics imgStatistics;
		int64_t i64ResultCount = blob.GetResultCount();

		// Statistics �ҽ� �̹��� ����
		imgStatistics.SetSourceImage(fliImage);

		// Blob ����� ������ ���� FigureArray ����
		CFLFigureArray flfaContour;

		// Blob ����� �� Contour �� ����
		if(IsFail(res = blob.GetResultContours(flfaContour)))
		{
			ErrorPrint(res, "Failed to get boundary rects from the Blob object.");
			break;
		}

		CFLString<wchar_t> strMedian, strLowerQuartile, strUpperQuartile;

		// �� Coutour�� �´� Median, LowerQuartile, UpperQuartile �� ������ �ڵ�
		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			imgStatistics.SetSourceROI(flfaContour.GetAt(i));

			CMultiVar<double> mvMedian;
			if(IsFail(res = imgStatistics.GetMedian(mvMedian)))
			{
				ErrorPrint(res, "Failed to get Median Value from the Blob object.");
				break;
			}

			strMedian.Format(L"No. %d Median : ", i);
			strMedian.AppendFormat(L"%lf", mvMedian.GetAt(0));
			wprintf_s(L"%s\n", strMedian.GetString());

			CMultiVar<double> mvLowerQuartile;
			if(IsFail(res = imgStatistics.GetLowerQuartile(mvLowerQuartile)))
			{
				ErrorPrint(res, "Failed to get LowerQuartile Value from the Blob object.");
				break;
			}

			strLowerQuartile.Format(L"No. %d LowerQuartile : ", i);
			strLowerQuartile.AppendFormat(L"%lf", mvLowerQuartile.GetAt(0));
			wprintf_s(L"%s\n", strLowerQuartile.GetString());

			CMultiVar<double> mvUpperQuartile;
			if(IsFail(res = imgStatistics.GetUpperQuartile(mvUpperQuartile)))
			{
				ErrorPrint(res, "Failed to get UpperQuartile Value from the Blob object.");
				break;
			}

			strUpperQuartile.Format(L"No. %d UpperQuartile : ", i);
			strUpperQuartile.AppendFormat(L"%lf", mvUpperQuartile.GetAt(0));
			wprintf_s(L"%s\n", strUpperQuartile.GetString());
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately

		CGUIViewImageLayerWrap layerSource = viewSourceImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();

		if(IsFail(res = layerSource.DrawTextCanvas(&CFLPointD(), L"Source", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// flfaOriginBoundaryRects �� Figure���� �迭�̱� ������ Layer�� �ֱ⸸ �ص� ��� ������� �����ϴ�.
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// ���⼭ 0.25�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(res = layerSource.DrawFigureImage(&flfaContour, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// Rect �������� ���� Ȯ���ϴ� �ڵ�
		for(int64_t i = 0; i < flfaContour.GetCount(); ++i)
		{
			CFLRegion* pFlrg = (CFLRegion*)flfaContour.GetAt(i);

			CFLString<wchar_t> strNumber;
			strNumber.Format(L"%d", i);

			CFLPointD flpCenter;
			pFlrg->GetCenter(flpCenter);

			layerSource.DrawTextImage(&flpCenter, (wchar_t*)strNumber.GetString(), CYAN);
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewSourceImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewSourceImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}