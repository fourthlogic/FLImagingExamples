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
		CResult eResult = EResult_UnknownError;
		// �̹��� �ε� // Load image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/Blob/AlignBall.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewSourceImage.Create(200, 0, 812, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewSourceImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Image ũ�⿡ �°� view�� ũ�⸦ ���� // Zoom the view to fit the image size
		if(IsFail(eResult = viewSourceImage.ZoomFit()))
		{
			ErrorPrint(eResult, "Failed to zoom fit\n");
			break;
		}

		// Blob ��ü ���� // Create Blob object
		CBlob blob;

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
		if(IsFail(eResult = blob.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute Blob.");
			break;
		}

		// ��� ��ü�� �� �ش�Ǵ� ������ ���� ��ü�� ����

		// 20���� �۰ų� ū �庯 ���̸� ���� ��ü���� ����
		if(IsFail(eResult = blob.Filter(CBlob::EFilterItem_MinimumEnclosingRectangleLongSideLength, 20, ELogicalCondition_LessEqual)))
		{
			ErrorPrint(eResult, "Blob filtering algorithm error occurs.");
			break;
		}

		// circularity �� 0.9���� ���� ��ü���� ����
		if(IsFail(eResult = blob.Filter(CBlob::EFilterItem_Circularity, 0.9, ELogicalCondition_Less)))
		{
			ErrorPrint(eResult, "Blob filtering algorithm error occurred.");
			break;
		}

		CFLArray<int32_t> flaItem;
		CFLArray<int32_t> flaOrder;

		// Blob ����� Mean, Variance, StandardDeviation �� ������ ���� Statistics ��ü ����
		CImageStatistics imgStatistics;
		int64_t i64ResultCount = blob.GetResultCount();

		// Statistics �ҽ� �̹��� ����
		imgStatistics.SetSourceImage(fliImage);

		// Blob ����� ������ ���� FigureArray ����
		CFLFigureArray flfaContour;

		// Blob ����� �� Contour �� ����
		if(IsFail(eResult = blob.GetResultContours(flfaContour)))
		{
			ErrorPrint(eResult, "Failed to get boundary rects from the Blob object.");
			break;
		}

		CFLString<wchar_t> strMean, strVariance, strStandardDeviation;

		// �� Coutour�� �´� Mean, Variance, StandardDeviation �� ������ �ڵ�
		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			imgStatistics.SetSourceROI(flfaContour.GetAt(i));

			CMultiVar<double> mvMean;
			if(IsFail(eResult = imgStatistics.GetMean(mvMean)))
			{
				ErrorPrint(eResult, "Failed to get Mean Value from the Blob object.");
				break;
			}

			strMean.Format(L"No. %d Mean : ", i);
			strMean.AppendFormat(L"%lf", mvMean.GetAt(0));
			wprintf_s(L"%s\n", strMean.GetString());

			CMultiVar<double> mvVariance;
			if(IsFail(eResult = imgStatistics.GetVariance(mvVariance)))
			{
				ErrorPrint(eResult, "Failed to get Variance Value from the Blob object.");
				break;
			}

			strVariance.Format(L"No. %d Variance : ", i);
			strVariance.AppendFormat(L"%lf", mvVariance.GetAt(0));
			wprintf_s(L"%s\n", strVariance.GetString());

			CMultiVar<double> mvStandardDeviation;
			if(IsFail(eResult = imgStatistics.GetStandardDeviation(mvStandardDeviation)))
			{
				ErrorPrint(eResult, "Failed to get StandardDeviation Value from the Blob object.");
				break;
			}

			strStandardDeviation.Format(L"No. %d StandardDeviation : ", i);
			strStandardDeviation.AppendFormat(L"%lf", mvStandardDeviation.GetAt(0));
			wprintf_s(L"%s\n", strStandardDeviation.GetString());
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately

		CGUIViewImageLayerWrap layerSource = viewSourceImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();

		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(), L"Source", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text on the image view.\n");
			break;
		}

		// flfaOriginBoundaryRects �� Figure���� �迭�̱� ������ Layer�� �ֱ⸸ �ص� ��� ������� �����ϴ�.
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// ���⼭ 0.25�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(eResult = layerSource.DrawFigureImage(&flfaContour, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(eResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// Rect �������� ���� Ȯ���ϴ� �ڵ�
		for(int64_t i = 0; i < flfaContour.GetCount(); ++i)
		{
			CFLRectL* pFlrRect = (CFLRectL*)flfaContour.GetAt(i);

			CFLString<wchar_t> strNumber;
			strNumber.Format(L"%d", i);

			layerSource.DrawTextImage(&CFLPointD(pFlrRect->GetCenter()), (wchar_t*)strNumber.GetString(), CYAN);
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