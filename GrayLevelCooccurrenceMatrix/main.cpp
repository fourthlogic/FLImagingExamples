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
		if((res = fliImage.Load(L"../../ExampleImages/GrayLevelCooccurrenceMatrix/Texture.flif")).IsFail())
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

		// CGrayLevelCooccurrenceMatrix ��ü ���� // Create CGrayLevelCooccurrenceMatrix object
		CGrayLevelCooccurrenceMatrix flaGLCM;

		// ROI ���� ���� // Set the ROI value		
		CFLRect<double> flfSourceROI(143.508137, 70.054249, 295.117540, 213.562386, 0.000000);
		// Source �̹��� ���� // Set the Source Image
		flaGLCM.SetSourceImage(fliImage);
		// Source ROI ���� // Set the Source ROI
		flaGLCM.SetSourceROI(flfSourceROI);

		// grayLevel ����(2^8 = 256) // Set gray level (2^8 = 256)
		flaGLCM.SetGrayLevel(8);

		// Matrix Direction 0�� ���� // Set Matrix Direction 0 Degree
		flaGLCM.SetDirection(CGrayLevelCooccurrenceMatrix::EDirection_Degree0);

		// �˰��� ���� // Execute the algorithm
		if((res = flaGLCM.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Gray Level Cooccurrence Matrix.");
			break;
		}

		// ������� �޾ƿ� CFLArray �����̳� ���� // Create the CFLArray object to push the result
		Base::CFLArray<Base::CFLArray<double>> flaEnergy;
		Base::CFLArray<Base::CFLArray<double>> flaCorrelation;
		Base::CFLArray<Base::CFLArray<double>> flaHomogeneity;
		Base::CFLArray<Base::CFLArray<double>> flaContrast;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� Energy�� ���ϴ� �Լ� // Function that calculate Energy of the image(or the region of ROI)
		if((res = flaGLCM.GetResultEnergy(flaEnergy)).IsFail())
		{
			ErrorPrint(res, "No Result");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� Correlation�� ���ϴ� �Լ� // Function that calculate Correlation of the image(or the region of ROI)
		if((res = flaGLCM.GetResultCorrelation(flaCorrelation)).IsFail())
		{
			ErrorPrint(res, "No Result");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� Homogeneity�� ���ϴ� �Լ� // Function that calculate Homogeneity of the image(or the region of ROI)
		if((res = flaGLCM.GetResultHomogeneity(flaHomogeneity)).IsFail())
		{
			ErrorPrint(res, "No Result");
			break;
		}

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� Contrast�� ���ϴ� �Լ� // Function that calculate Contrast of the image(or the region of ROI)
		if((res = flaGLCM.GetResultContrast(flaContrast)).IsFail())
		{
			ErrorPrint(res, "No Result");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
		if((res = layer.DrawFigureImage(&flfSourceROI, LIME)).IsFail())
			ErrorPrint(res, "Failed to draw figure\n");

		CFLString<wchar_t> strText;

		for(int32_t i32PageIdx = 0; i32PageIdx < flaEnergy.GetCount(); ++i32PageIdx)
		{
			//strText.AppendFormat(L"Page.No %d ", i32PageIdx);

			for(int32_t i32Ch = 0; i32Ch < flaEnergy[i32PageIdx].GetCount(); i32Ch++)
			{
				//strText.AppendFormat(L"Channel %d ", i32Ch);
				strText.AppendFormat(L"Energy %.9lf ", flaEnergy[i32PageIdx][i32Ch]);
				strText.AppendFormat(L"\nCorrelation %.9lf ", flaCorrelation[i32PageIdx][i32Ch]);
				strText.AppendFormat(L"\nHomogeneity %.9lf ", flaHomogeneity[i32PageIdx][i32Ch]);
				strText.AppendFormat(L"\nContrast %.9lf ", flaContrast[i32PageIdx][i32Ch]);
			}

			//strText.AppendFormat(L"\n\n");
		}

		wprintf_s(L"%s\n", strText.GetString());

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layer.DrawTextCanvas(&CFLPoint<double>(0, 0), strText.GetString(), YELLOW, BLACK, 25)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
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