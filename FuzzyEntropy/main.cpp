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
		if((res = fliImage.Load(L"../../ExampleImages/FuzzyEntropy/FuzzyEntropySource.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.");
			break;
		}

		// �̹��� �� ���� // Create image view
		if((res = viewImage.Create(400, 0, 912, 612)).IsFail())
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

		// fuzzyentropy ��ü ���� // Create fuzzyentropy object
		CFuzzyEntropy fuzzyentropy;

		// ROI ���� ���� // Set the ROI value		
		CFLCircle<double> flrROI(310.466830, 81.769042, 81.769042, 0.000000, 0.000000, 360.000000, EArcClosingMethod_EachOther);
		// Source �̹��� ���� // Set the Source Image
		fuzzyentropy.SetSourceImage(fliImage);
		// Source ROI ���� // Set the Source ROI
		fuzzyentropy.SetSourceROI(flrROI);

		// Parameter A, C ���� (A: 0, C: 255) // Create the parameter A and C (A: 0, C: 255)
		const Base::CMultiVar<double> flmvParameterA = Base::CMultiVar<double>(0.);
		const Base::CMultiVar<double> flmvParameterC = Base::CMultiVar<double>(255.);

		// Parameter ����(A: 0, C: 255) // Set the parameter(A: 0, C: 255)
		fuzzyentropy.SetParameterA(flmvParameterA);
		fuzzyentropy.SetParameterC(flmvParameterC);

		// �˰��� ���� // Execute the algorithm
		if((res = fuzzyentropy.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute FuzzyEntropy.");
			break;
		}

		// ������� �޾ƿ� CMultiVar<double> �����̳� ���� // Create the CMultiVar<double> object to push the result
		CMultiVar<double> mvFuzzyEntropy;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� Fuzzy Entropy�� ���ϴ� �Լ� // Function that calculate the fuzzy entropy of the image(or the region of ROI)
		if((res = fuzzyentropy.GetResultFuzzyEntropy(mvFuzzyEntropy)).IsFail())
		{
			ErrorPrint(res, "Failed to process.");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� �ּҰ��� ���ϴ� �Լ� // Function that calculate the min of the pixel value of the image(or the region of ROI)
		if((res = fuzzyentropy.GetResultFuzzyEntropy(mvFuzzyEntropy)).IsFail())
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

		CFLString<wchar_t> strFuzzyEntropy;
		strFuzzyEntropy.Format(L"Fuzzy Entropy Of Region : %lf", mvFuzzyEntropy.GetAt(0));

		wprintf_s(L"%s\n", strFuzzyEntropy.GetString());

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 0), strFuzzyEntropy.GetString(), YELLOW, BLACK, 25)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

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