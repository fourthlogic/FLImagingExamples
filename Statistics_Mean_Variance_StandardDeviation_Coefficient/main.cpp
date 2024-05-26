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
		CFLRect<double> flrROI(264, 189, 432, 364);
		// Source �̹��� ���� // Set the Source Image
		statistics.SetSourceImage(fliImage);
		// Source ROI ���� // Set the Source ROI
		statistics.SetSourceROI(flrROI);

		// ������� �޾ƿ� CMultiVar<double> �����̳� ���� // Create the CMultiVar<double> object to push the result
		CMultiVar<double> mvMean, mvVariance, mvStandardDeviation, mvCoefficientOfVariance;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� ����� ���ϴ� �Լ� // Function that calculate the mean of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetMean(mvMean)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �л��� ���ϴ� �Լ� // Function that calculate the variance of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetVariance(mvVariance)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� ǥ�������� ���ϴ� �Լ� // Function that calculate the standard deviation of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetStandardDeviation(mvStandardDeviation)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� ��������� ���ϴ� �Լ� // Function that calculate the coefficient of variance of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetCoefficientOfVariance(mvCoefficientOfVariance)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// trimming �ɼ� ����(Lower:0.2, Upper:0.4) // Set the trimming value(Lower:0.2, Upper:0.4)
		statistics.SetTrimming(0.2, CImageStatistics::ETrimmingLocation_Lower);
		statistics.SetTrimming(0.4, CImageStatistics::ETrimmingLocation_Upper);

		// trimming �� ������� �޾ƿ� CMultiVar<double> �����̳� ���� // Create the CMultiVar<double> object to push the trimmed result
		CMultiVar<double> mvTrimmingMean, mvTrimmingVariance, mvTrimmingStandardDeviation, mvTrimmingCoefficientOfVariance;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� ����� ���ϴ� �Լ� // Function that calculate the mean of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetMean(mvTrimmingMean)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �л��� ���ϴ� �Լ� // Function that calculate the variance of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetVariance(mvTrimmingVariance)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� ǥ�������� ���ϴ� �Լ� // Function that calculate the standard deviation of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetStandardDeviation(mvTrimmingStandardDeviation)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� ��������� ���ϴ� �Լ� // Function that calculate the coefficient of variance of the pixel value of the image(or the region of ROI)
		if((res = statistics.GetCoefficientOfVariance(mvTrimmingCoefficientOfVariance)).IsFail())
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

		CFLString<wchar_t> strMeanValue, strVarianceValue, strStandardDeviationValue, strCoefficientOfVariance;
		strMeanValue.Format(L"Mean Of Region : %lf", mvMean.GetAt(0));
		strStandardDeviationValue.Format(L"StandardDeviation  Of Region : %lf", mvStandardDeviation.GetAt(0));
		strVarianceValue.Format(L"Variance  Of Region : %lf", mvVariance.GetAt(0));
		strCoefficientOfVariance.Format(L"Coefficient of Variance Of Region : %lf", mvCoefficientOfVariance.GetAt(0));

		CFLString<wchar_t> strTrimmingMeanValue, strTrimmingVarianceValue, strTrimmingStandardDeviationValue, strTrimmingCoefficientOfVariance;
		strTrimmingMeanValue.Format(L"Mean Of Trimmed Region : %lf", mvTrimmingMean.GetAt(0));
		strTrimmingStandardDeviationValue.Format(L"StandardDeviation  Of Trimmed Region : %lf", mvTrimmingStandardDeviation.GetAt(0));
		strTrimmingVarianceValue.Format(L"Variance  Of Trimmed Region : %lf", mvTrimmingVariance.GetAt(0));
		strTrimmingCoefficientOfVariance.Format(L"Coefficient of Variance Of Trimmed Region : %lf", mvTrimmingCoefficientOfVariance.GetAt(0));

		wprintf_s(L"%s\n", strMeanValue.GetString());
		wprintf_s(L"%s\n", strStandardDeviationValue.GetString());
		wprintf_s(L"%s\n", strVarianceValue.GetString());
		wprintf_s(L"%s\n", strCoefficientOfVariance.GetString());

		wprintf_s(L"%s\n", strTrimming.GetString());

		wprintf_s(L"%s\n", strTrimmingMeanValue.GetString());
		wprintf_s(L"%s\n", strTrimmingStandardDeviationValue.GetString());
		wprintf_s(L"%s\n", strTrimmingVarianceValue.GetString());
		wprintf_s(L"%s\n", strTrimmingCoefficientOfVariance.GetString());

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 0), strMeanValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 30), strStandardDeviationValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 60), strVarianceValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 90), strCoefficientOfVariance.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 120), strTrimming.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 150), strTrimmingMeanValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 180), strTrimmingStandardDeviationValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 210), strTrimmingVarianceValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 240), strTrimmingCoefficientOfVariance.GetString(), YELLOW, BLACK, 30)).IsFail())
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