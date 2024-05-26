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
			ErrorPrint(res, "Failed to load the image file.");
			break;
		}

		// �̹��� �� ���� // Create image view
		if((res = viewImage.Create(400, 0, 1150, 500)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if((res = viewImage.SetImagePtr(&fliImage)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.");
			break;
		}

		// Statistics ��ü ���� // Create Statistics object
		CImageStatistics statistics;

		// ROI ���� ���� // Set the ROI value
		CFLRect<double> flrROI(264, 189, 432, 364);
		// Source �̹��� ���� // Set the Source Image
		statistics.SetSourceImage(fliImage);
		// Source ROI ���� // Set the Source ROI
		statistics.SetSourceROI(flrROI);

		// ������� �޾ƿ� CMultiVar<double> �����̳� ���� // Create the CMultiVar<double> object to push the result
		CMultiVar<double> mvSum, mvSumOfSquares;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� ���� ���ϴ� �Լ� // Function that calculate the sum of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetSum(mvSum)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �������� ���ϴ� �Լ� // Function that calculate the sum of squares of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetSumOfSquares(mvSumOfSquares)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// trimming �ɼ� ����(Lower:0.2, Upper:0.4) // Set the Trimming value(Lower:0.2, Upper:0.4)
		statistics.SetTrimming(0.2, CImageStatistics::ETrimmingLocation_Lower);
		statistics.SetTrimming(0.4, CImageStatistics::ETrimmingLocation_Upper);

		// trimming �� ������� �޾ƿ� CMultiVar<double> �����̳� ���� // Create the CMultiVar<double> object to push the trimmed result
		CMultiVar<double> mvTrimmingSum, mvTrimmingSumOfSquares;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� ���� ���ϴ� �Լ� // Function that calculate the sum of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetSum(mvTrimmingSum)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �������� ���ϴ� �Լ� // Function that calculate the sum of squares of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetSumOfSquares(mvTrimmingSumOfSquares)).IsFail())
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
			ErrorPrint(res, "Failed to draw figure.");

		CFLString<wchar_t> strTrimming;
		strTrimming.Format(L"Trimming Lower : %.2lf, Upper : %.2lf", statistics.GetTrimming(CImageStatistics::ETrimmingLocation_Lower), statistics.GetTrimming(CImageStatistics::ETrimmingLocation_Upper));

		CFLString<wchar_t> strSumValue, strSumOfSquaresValue;
		strSumValue.Format(L"Sum Of Region : %lf", mvSum.GetAt(0));
		strSumOfSquaresValue.Format(L"Sum of Squares Of Region : %lf", mvSumOfSquares.GetAt(0));

		wprintf_s(L"%s\n", strSumValue.GetString());
		wprintf_s(L"%s\n", strSumOfSquaresValue.GetString());
		wprintf_s(L"%s\n", strTrimming.GetString());

		CFLString<wchar_t> strTrimmingSumValue, strTrimmingSumOfSquaresValue;
		strTrimmingSumValue.Format(L"Sum of Trimmed Region : %lf", mvTrimmingSum.GetAt(0));
		strTrimmingSumOfSquaresValue.Format(L"Sum of Squares of Trimmed Region : %lf", mvTrimmingSumOfSquares.GetAt(0));

		wprintf_s(L"%s\n", strTrimmingSumValue.GetString());
		wprintf_s(L"%s\n", strTrimmingSumOfSquaresValue.GetString());

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 0), strSumValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text.");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 30), strSumOfSquaresValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text.");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 60), strTrimming.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text.");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 90), strTrimmingSumValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text.");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 120), strTrimmingSumOfSquaresValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text.");
			break;
		}

		if((res = layer.DrawFigureImage(&flrROI, BLUE, 1, LIGHTBLUE, GUI::EGUIViewImagePenStyle_Solid, 0.5F, 0.5F)).IsFail())
			ErrorPrint(res, "Failed to draw figure.");

		if((res = layer.DrawFigureImage(&flrROI, LIME)).IsFail())
			ErrorPrint(res, "Failed to draw figure.");


		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}