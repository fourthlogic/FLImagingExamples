#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage;
	CResult eResult;

	do
	{ 
		// �̹��� �ε� // Loads image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/Moment/airEdge.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates imageview
		if(IsFail(eResult = viewImage.Create(600, 0, 1424, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(eResult = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		CResult eResult = EResult_UnknownError;

		// Moment ��ü ���� // Create Moment object
		CMoment moment;

		// ROI ���� ���� // Set ROI range
		CFLRectL flrROI(15, 150, 420, 280);

		// ó���� �̹��� ���� // Set the image to process
		moment.SetSourceImage(fliImage);
		// ó���� ROI ���� // Set the ROI to be processed
		moment.SetSourceROI(flrROI);
		// ó���� �̹����� ����ȭ�̹����� �Ǵ� ���� ���� // Set whether to judge the image to be processed as a binarized image
		moment.EnableBinaryImage(true);

		// ��� ��� ����Ű ���Ʈ N, M �Ķ���͸� �߰��մϴ�. // Add Journey Moment N, M parameters to the calculation target.
 		moment.AddZernike(1, -1);
 		moment.AddZernike(1, 1);
 		moment.AddZernike(3, -3);
 		moment.AddZernike(3, -1);
 		moment.AddZernike(3, 1);
 		moment.AddZernike(3, 3);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(eResult = moment.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute moment.\n");
			break;
		}

		// ���Ʈ ������� �����ɴϴ�. // Get the moment results.
		CMoment::SZernike zernike;
		int64_t i64ZernikeCount = (int64_t)moment.GetZernikeCount();

		for(int i = 0; i < i64ZernikeCount; ++i)
		{
			moment.GetZernike(&zernike, i);
			printf("Zernike N = %d, M = %d, RealValue : %.4lf, Imaginary : %.4lf\n", zernike.i32N, zernike.i32M, zernike.f64ZernikeReal, zernike.f64ZernikeImag);
		}
		
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();
		
		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the ROI area is
		if(IsFail(eResult = layer.DrawFigureImage(&flrROI, BLUE)))
		{
			ErrorPrint(eResult, "Failed to draw figure\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}