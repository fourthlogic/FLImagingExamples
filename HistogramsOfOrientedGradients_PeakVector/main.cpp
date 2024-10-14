#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewSrcImage;

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		// �̹��� �ε� // Load image
		if((res = fliImage.Load(L"../../ExampleImages/OperationCompare/candle.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.");
			break;
		}

		// �̹��� �� ���� // Create image view
		if((res = viewSrcImage.Create(400, 0, 912, 484)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if((res = viewSrcImage.SetImagePtr(&fliImage)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewSrcImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// HOG ��ü ���� // Create HOG object
		CHistogramsOfOrientedGradients hog;

		// ROI ���� ���� // Create ROI area
		CFLRect<int32_t> flrROI(200, 10, 300, 200);

		// ������ �̹��� ���� // Set Image to Calculate
		if(IsFail(res = hog.SetSourceImage(fliImage)))
		{
			ErrorPrint(res, "Failed to set Source Image.");
			break;
		}

		// ������ ROI ���� // Set ROI to Calculate
		if(IsFail(res = hog.SetSourceROI(flrROI)))
		{
			ErrorPrint(res, "Failed to set Source ROI.");
			break;
		}

		// �˰��� ���� // Execute the algorithm
		if((res = hog.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Histograms Of Oriented Gradients.");
			break;
		}

		// ���� ����� �޾ƿ��� ���� �����̳� // Container to get Calculated results
		CFLFigureArray flfaPeakVectors;

		// ��ũ ���� ���� // Get Peak Vectors
		if((res = hog.GetPeakVectorsFigure(0, flfaPeakVectors)).IsFail())
		{
			ErrorPrint(res, "Failed to get result.");
			break;
		}

		// ��ũ ���͸� ��� // Print Peak Vectors
		layer.DrawFigureImage(flfaPeakVectors[0], BLUE, 3, BLUE, EGUIViewImagePenStyle_Solid, 0.3f, 0.3f);
		layer.DrawFigureImage(flfaPeakVectors[1], GREEN, 3, GREEN, EGUIViewImagePenStyle_Solid, 0.3f, 0.3f);
		layer.DrawFigureImage(flfaPeakVectors[2], RED, 3, RED, EGUIViewImagePenStyle_Solid, 0.3f, 0.3f);

		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
		if(IsFail(res = layer.DrawFigureImage(&flrROI, LIME)))
		{
			ErrorPrint(res, "Failed to draw figures objects on the image view.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewSrcImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewSrcImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}