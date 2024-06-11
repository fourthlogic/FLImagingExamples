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
		if((res = fliImage.Load(L"../../ExampleImages/PeripheralLuminance/Bolt.flif")).IsFail())
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

		// CPeripheralLuminance ��ü ���� // Create CPeripheralLuminance object
		CPeripheralLuminance PeripheralLuminance;

		CFLFigureArray flfaMeasurementRegion;

		// �ǰ� ���� �ε� // Figure file Load
		if((res = flfaMeasurementRegion.Load(L"../../ExampleImages/PeripheralLuminance/Measurement Region.fig")).IsFail())
		{
			ErrorPrint(res, "Failed to execute Peripheral Luminance.");
			break;
		}

		// Source �̹��� ���� // Set the Source Image
		PeripheralLuminance.SetSourceImage(fliImage);
		// Measurement Region ���� // Set the Measurement Region
		PeripheralLuminance.SetMeasurementRegion(flfaMeasurementRegion);
		// Thickness ���� // Set Thickness
		PeripheralLuminance.SetThickness(2.0);

		// �˰��� ���� // Execute the algorithm
		if((res = PeripheralLuminance.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Peripheral Luminance.");
			break;
		}

		CFLArray<double> flaResult;

		// ���� �������� �ֵ� ���̿� ��� // Results of luminance differences in measurement areas
		if((res = PeripheralLuminance.GetResult(flaResult)).IsFail())
		{
			ErrorPrint(res, "No Result");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		CFLString<wchar_t> strText;

		for(int32_t i32Idx = 0; i32Idx < flaResult.GetCount(); ++i32Idx)
		{
			CFLFigure* pflfSrc = flfaMeasurementRegion.GetAt(i32Idx);

			strText.Format(L"%lf ", flaResult[i32Idx]);

			// ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where Measurement is
			if((res = layer.DrawFigureImage(pflfSrc, LIME, 1, LIME, EGUIViewImagePenStyle_Solid, 0.3f, 0.3f)).IsFail())
			{
				ErrorPrint(res, "Failed to draw Figure\n");
				break;
			}

			// �̹��� �� ���� ǥ�� // Display image view information
			if((res = layer.DrawTextImage(pflfSrc, strText.GetString(), YELLOW, BLACK, 13, false, 0., EGUIViewImageTextAlignment_LEFT_TOP)).IsFail())
			{
				ErrorPrint(res, "Failed to draw text\n");
				break;
			}
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