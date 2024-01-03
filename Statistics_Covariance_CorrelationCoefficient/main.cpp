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
		if((eResult = fliImage.Load(L"../../ExampleImages/Statistics/MultiChannelSource.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if((eResult = viewImage.Create(400, 0, 1150, 700)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if((eResult = viewImage.SetImagePtr(&fliImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Statistics ��ü ���� // Create Statistics object
		CImageStatistics statistics;

		// Source �̹��� ���� // Set the Source Image
		statistics.SetSourceImage(fliImage);
		// ������踦 ���� ä���� ���� // Set the Correlation Channels
		statistics.SetCorrelatedChannel(0, 1);

		// ������� �޾ƿ� CMultiVar<double> �����̳� ���� // Create the CMultiVar<double> object to push the result
		double f64Covariance, f64CorrelationCoeff;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� ���л��� ���ϴ� �Լ� // Function that calculate the covariance of the pixel value of the image(or the region of ROI)
		if((eResult = statistics.GetCovariance(f64Covariance)).IsFail())
		{
			ErrorPrint(eResult, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �������� ���ϴ� �Լ� // Function that calculate the correlation coefficient of the pixel value of the image(or the region of ROI)
		if((eResult = statistics.GetCorrelationCoefficient(f64CorrelationCoeff)).IsFail())
		{
			ErrorPrint(eResult, "Failed to process.");
			break;
		}

		// ������踦 ���� ä���� ���� // Set the Correlation Channels
		statistics.SetCorrelatedChannel(0, 2);

		// ������� �޾ƿ� CMultiVar<double> �����̳� ���� // Create the CMultiVar<double> object to push the result
		double f64Covariance2, f64CorrelationCoeff2;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� ���л��� ���ϴ� �Լ� // Function that calculate the covariance of the pixel value of the image(or the region of ROI)
		if((eResult = statistics.GetCovariance(f64Covariance2)).IsFail())
		{
			ErrorPrint(eResult, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �������� ���ϴ� �Լ� // Function that calculate the correlation coefficient of the pixel value of the image(or the region of ROI)
		if((eResult = statistics.GetCorrelationCoefficient(f64CorrelationCoeff2)).IsFail())
		{
			ErrorPrint(eResult, "Failed to process.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		CFLString<wchar_t> strCorrChannel, strCovarianceValue, strCorrelationCoeffValue;
		strCorrChannel.Format(L"Correlation Channel : Channel 0 - Channel 1");
		strCovarianceValue.Format(L"Covariance : %lf", f64Covariance);
		strCorrelationCoeffValue.Format(L"Correlation Coefficient : %lf", f64CorrelationCoeff);

		wprintf_s(L"%s\n", strCorrChannel.GetString());
		wprintf_s(L"%s\n", strCovarianceValue.GetString());
		wprintf_s(L"%s\n", strCorrelationCoeffValue.GetString());

		CFLString<wchar_t> strCorrChannel2, strCovarianceValue2, strCorrelationCoeffValue2;
		strCorrChannel2.Format(L"Correlation Channel : Channel 0 - Channel 2");
		strCovarianceValue2.Format(L"Covariance : %lf", f64Covariance2);
		strCorrelationCoeffValue2.Format(L"Correlation Coefficient : %lf", f64CorrelationCoeff2);

		wprintf_s(L"%s\n", strCorrChannel2.GetString());
		wprintf_s(L"%s\n", strCovarianceValue2.GetString());
		wprintf_s(L"%s\n", strCorrelationCoeffValue2.GetString());

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 0), strCorrChannel.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 30), strCovarianceValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 60), strCorrelationCoeffValue.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 90), strCorrChannel2.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 120), strCovarianceValue2.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 150), strCorrelationCoeffValue2.GetString(), YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
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