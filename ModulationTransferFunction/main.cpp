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
		if((res = fliImage.Load(L"../../ExampleImages/ModulationTransferFunction/ISO12233Crop.flif")).IsFail())
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

		// CModulationTransferFunction ��ü ���� // Create CModulationTransferFunction object
		CModulationTransferFunction flaGLCM;

		// ROI ���� ���� // Set the ROI value		
		CFLRect<double> flfSourceROI(349.0, 43.0, 396.0, 85.0);
		// Source �̹��� ���� // Set the Source Image
		flaGLCM.SetSourceImage(fliImage);
		// Source ROI ���� // Set the Source ROI
		flaGLCM.SetSourceROI(flfSourceROI);

		// �˰��� ���� // Execute the algorithm
		if((res = flaGLCM.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Modulation Transfer Function.");
			break;
		}

		// ������� �޾ƿ� CFLArray �����̳� ���� // Create the CFLArray object to push the result
		Base::CFLArray<double> flaMTF;

		// �̹��� ��ü(Ȥ�� ROI ����) �ȼ����� MTF�� ���ϴ� �Լ� // Function that calculate MTF of the image(or the region of ROI)
		if((res = flaGLCM.GetResult(flaMTF)).IsFail())
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

		for(int32_t i32PageIdx = 0; i32PageIdx < flaMTF.GetCount(); ++i32PageIdx)
			strText.AppendFormat(L"MTF %.9lf ", flaMTF[i32PageIdx]);

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