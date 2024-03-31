#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

    // ���� ��� ��ü ���� // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		// �̹��� �ε� // Load image
		if((res = fliImage.Load(L"../../ExampleImages/Statistics/StatisticsSource.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if((res = viewImage.Create(400, 0, 1150, 500)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

        // �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if((res = viewImage.SetImagePtr(&fliImage)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Statistics ��ü ���� // Create Statistics object
		CImageStatistics statistics;

		// ROI ���� ���� // Set the ROI value
		CFLRectD flrROI(264, 189, 432, 364);
		// Source �̹��� ���� // Set the Source Image
		statistics.SetSourceImage(fliImage);
		// Source ROI ���� // Set the Source ROI
		statistics.SetSourceROI(flrROI);

		// ������� �޾ƿ� CMultiVar<double> �����̳� ���� // Create the CMultiVar<double> object to push the result
		CMultiVar<double> mvMedian, mvUpperQuartile, mvLowerQuartile;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �߰����� ���ϴ� �Լ� // Function that calculate the median of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetMedian(mvMedian)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� 25��������� ���ϴ� �Լ� // Function that calculate the lower quartile of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetLowerQuartile(mvLowerQuartile)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� 75��������� ���ϴ� �Լ� // Function that calculate the higher quartile of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetUpperQuartile(mvUpperQuartile)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// trimming �ɼ� ����(Lower:0.2, Upper:0.4) // Set the trimming value(Lower:0.2, Upper:0.4)
		statistics.SetTrimming(0.2, CImageStatistics::ETrimmingLocation_Lower);
		statistics.SetTrimming(0.4, CImageStatistics::ETrimmingLocation_Upper);

		// trimming �� ������� �޾ƿ� CMultiVar<double> �����̳� ���� // Create the CMultiVar<double> object to push the trimmed result
		CMultiVar<double> mvTrimmingMedian, mvTrimmingUpperQuartile, mvTrimmingLowerQuartile;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �߰����� ���ϴ� �Լ� // Function that calculate the median of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetMedian(mvTrimmingMedian)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� 25��������� ���ϴ� �Լ� // Function that calculate the lower quartile of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetLowerQuartile(mvTrimmingLowerQuartile)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� 75��������� ���ϴ� �Լ� // Function that calculate the upper quartile of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetUpperQuartile(mvTrimmingUpperQuartile)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
		if((res = layer.DrawFigureImage(&flrROI, LIME)).IsFail())
			ErrorPrint(res, "Failed to draw figure\n");

		CFLString<wchar_t> strTrimming;
		strTrimming.Format(L"Trimming Lower : %.2lf, Upper : %.2lf", statistics.GetTrimming(CImageStatistics::ETrimmingLocation_Lower), statistics.GetTrimming(CImageStatistics::ETrimmingLocation_Upper));

		CFLString<wchar_t> strMedianValue, strLowerQuartileValue, strUpperQuartileValue;
		CFLString<wchar_t> strTrimmingMedianValue, strTrimmingLowerQuartileValue, strTrimmingUpperQuartileValue;
		strMedianValue.Format(L"Median Of Region : %lf", mvMedian.GetAt(0));
		strLowerQuartileValue.Format(L"25 Quartile Of Region : %lf", mvLowerQuartile.GetAt(0));
		strUpperQuartileValue.Format(L"75 Quartile Of Region : %lf", mvUpperQuartile.GetAt(0));
		strTrimmingMedianValue.Format(L"Median Of Trimmed Region : %lf", mvTrimmingMedian.GetAt(0));
		strTrimmingLowerQuartileValue.Format(L"25 Quartile Of Trimmed Region : %lf", mvTrimmingLowerQuartile.GetAt(0));
		strTrimmingUpperQuartileValue.Format(L"75 Quartile Of Trimmed Region : %lf", mvTrimmingUpperQuartile.GetAt(0));

		wprintf_s(L"%s\n", strMedianValue.GetString());
		wprintf_s(L"%s\n", strLowerQuartileValue.GetString());
		wprintf_s(L"%s\n", strUpperQuartileValue.GetString());
		wprintf_s(L"%s\n", strTrimming.GetString());
		wprintf_s(L"%s\n", strTrimmingMedianValue.GetString());
		wprintf_s(L"%s\n", strTrimmingLowerQuartileValue.GetString());
		wprintf_s(L"%s\n", strTrimmingUpperQuartileValue.GetString());

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPointD(0, 0), strMedianValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPointD(0, 30), strLowerQuartileValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPointD(0, 60), strUpperQuartileValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPointD(0, 90), strTrimming.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPointD(0, 120), strTrimmingMedianValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPointD(0, 150), strTrimmingLowerQuartileValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPointD(0, 180), strTrimmingUpperQuartileValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = layer.DrawFigureImage(&flrROI, BLUE, 1, LIGHTBLUE, GUI::EGUIViewImagePenStyle_Solid, 0.5F, 0.5F)).IsFail())
			ErrorPrint(res, "Failed to draw figure\n");

		if((res = layer.DrawFigureImage(&flrROI, LIME)).IsFail())
			ErrorPrint(res, "Failed to draw figure\n");


		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}