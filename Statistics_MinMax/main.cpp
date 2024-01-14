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
	CResult eResult = EResult_UnknownError;

	do
	{
		// �̹��� �ε� // Load image
		if((eResult = fliImage.Load(L"../../ExampleImages/Statistics/StatisticsSource.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.");
			break;
		}

		// �̹��� �� ���� // Create image view
		if((eResult = viewImage.Create(400, 0, 1150, 500)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.");
			break;
		}

        // �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if((eResult = viewImage.SetImagePtr(&fliImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.");
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
		CMultiVar<double> mvMin, mvMax;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �ּҰ��� ���ϴ� �Լ� // Function that calculate the min of the pixel value of the image(or the region of ROI)
		if((eResult = statistics.GetMin(mvMin)).IsFail())
		{
			ErrorPrint(eResult, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �ִ밪�� ���ϴ� �Լ� // Function that calculate the max of the pixel value of the image(or the region of ROI)
		if((eResult = statistics.GetMax(mvMax)).IsFail())
		{
			ErrorPrint(eResult, "Failed to process.");
			break;
		}

		// trimming �ɼ� ����(Lower:0.2, Upper:0.4) // Set the trimming value(Lower:0.2, Upper:0.4)
		statistics.SetTrimming(0.2, CImageStatistics::ETrimmingLocation_Lower);
		statistics.SetTrimming(0.4, CImageStatistics::ETrimmingLocation_Upper);

		// trimming �� ������� �޾ƿ� CMultiVar<double> �����̳� ���� // Create the CMultiVar<double> object to push the trimmed result
		CMultiVar<double> mvTrimmingMin, mvTrimmingMax;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �ּҰ��� ���ϴ� �Լ� // Function that calculate the min of the pixel value of the image(or the region of ROI)
		if((eResult = statistics.GetMin(mvTrimmingMin)).IsFail())
		{
			ErrorPrint(eResult, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �ִ밪�� ���ϴ� �Լ� // Function that calculate the max of the pixel value of the image(or the region of ROI)
		if((eResult = statistics.GetMax(mvTrimmingMax)).IsFail())
		{
			ErrorPrint(eResult, "Failed to process.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
		if((eResult = layer.DrawFigureImage(&flrROI, LIME)).IsFail())
			ErrorPrint(eResult, "Failed to draw figure\n");

		CFLString<wchar_t> strTrimming;
		strTrimming.Format(L"Trimming Lower : %.2lf, Upper : %.2lf", statistics.GetTrimming(CImageStatistics::ETrimmingLocation_Lower), statistics.GetTrimming(CImageStatistics::ETrimmingLocation_Upper));

		CFLString<wchar_t> strMinValue, strMaxValue, strTrimmingMinValue, strTrimmingMaxValue;
		strMinValue.Format(L"Min Of Region : %lf", mvMin.GetAt(0));
		strMaxValue.Format(L"Max Of Region : %lf", mvMax.GetAt(0));
		strTrimmingMinValue.Format(L"Min Of Trimmed Region : %lf", mvTrimmingMin.GetAt(0));
		strTrimmingMaxValue.Format(L"Max Of Trimmed Region : %lf", mvTrimmingMax.GetAt(0));

		wprintf_s(L"%s\n", strMinValue.GetString());
		wprintf_s(L"%s\n", strMaxValue.GetString());
		wprintf_s(L"%s\n", strTrimming.GetString());
		wprintf_s(L"%s\n", strTrimmingMinValue.GetString());
		wprintf_s(L"%s\n", strTrimmingMaxValue.GetString());

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 0), strMinValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 30), strMaxValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 60), strTrimming.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 90), strTrimmingMinValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 120), strTrimmingMaxValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		CFLRectD flrMinRegion(429, 336, 430, 337);
		CFLRectD flrMaxRegion(492, 370, 493, 371);
		flrMinRegion.Inflate(0.5);
		flrMaxRegion.Inflate(0.5);

		if((eResult = layer.DrawFigureImage(&flrROI, LIME)).IsFail())
			ErrorPrint(eResult, "Failed to draw figure\n");

		if((eResult = layer.DrawFigureImage(&flrMinRegion, BLUE, 1, LIGHTBLUE, GUI::EGUIViewImagePenStyle_Solid, 0.5F, 0.5F)).IsFail())
			ErrorPrint(eResult, "Failed to draw figure\n");

		if((eResult = layer.DrawFigureImage(&flrMaxRegion, RED, 1, RED, GUI::EGUIViewImagePenStyle_Solid, 0.5F, 0.5F)).IsFail())
			ErrorPrint(eResult, "Failed to draw figure\n");


		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}