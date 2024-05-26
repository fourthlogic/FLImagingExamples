#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage;
	CResult res;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Moment/airEdge.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates imageview
		if(IsFail(res = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}
		 
		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(res = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		CResult res = EResult_UnknownError;

		// Moment ��ü ���� // Create Moment object
		CMoment moment;

		// ROI ���� ���� // Set ROI range
		CFLRect<int32_t> flrROI(15, 150, 420, 280);

		// ó���� �̹��� ���� // Set the image to process
		moment.SetSourceImage(fliImage);
		// ó���� ROI ���� // Set the ROI to be processed
		moment.SetSourceROI(flrROI);
		// ó���� �̹����� ����ȭ�̹����� �Ǵ� ���� ���� // Set whether to judge the image to be processed as a binarized image
		// ����ȭ�̹����� �Ǵ��� ��� 0�� �ƴ� ��� ȭ�Ұ��� 1�� ó���� // When judging as a binarized image, all non-zero pixel values ??are treated as 1.
		moment.EnableBinaryImage(true);

		bool bIsCalcGeometricMoment = true;
		bool bIsCalcCentroidMoment = true;
		bool bIsCalcCentralMoment = true;
		bool bIsCalcNormalizedCentralMoment= true;
		bool bIsCalcHuMoment= true;

		// ��� ��� �������� ���Ʈ�� �����մϴ�. // Include the geometric moment in the computed object.
		moment.EnableGeometricMoment(bIsCalcGeometricMoment);
		// ��� ��� ���� ���Ʈ�� �����մϴ�. // Include the centroid moment in the calculation target.
		moment.EnableCentroidMoment(bIsCalcCentroidMoment);
		// ��� ��� �߽� ���Ʈ�� �����մϴ�. // Include the central moment in the calculation target.
		moment.EnableCentralMoment(bIsCalcCentralMoment);
		// ��� ��� ����ȭ�� �߽� ���Ʈ�� �����մϴ�. // Include the normalized central moment in the computed target.
		moment.EnableNormalizedCentralMoment(bIsCalcNormalizedCentralMoment);
		// ��� ��� ��(�Һ�) ���Ʈ�� �����մϴ�. // Include the idle (invariant) moment in the calculation target.
		moment.EnableHuMoment(bIsCalcHuMoment);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = moment.Execute()))
		{
			ErrorPrint(res, "Failed to execute moment.\n");
			break;
		}

		// ���Ʈ ������� �����ɴϴ�. // Get the moment results.
		CMoment::SMoments results;
		moment.GetMoment(results);

		// ���Ʈ ����� Consoleâ�� ��� // Output the moment result to the console window
		if(bIsCalcGeometricMoment)
		{
			printf("< Geometric Moment > \n");
			printf(" Moment 00 : %lf\n", results.sGeometricMoments.f64GeometricM00);
			printf(" Moment 10 : %lf\n", results.sGeometricMoments.f64GeometricM10);
			printf(" Moment 01 : %lf\n", results.sGeometricMoments.f64GeometricM01);
			printf(" Moment 20 : %lf\n", results.sGeometricMoments.f64GeometricM20);
			printf(" Moment 11 : %lf\n", results.sGeometricMoments.f64GeometricM11);
			printf(" Moment 02 : %lf\n", results.sGeometricMoments.f64GeometricM02);
			printf(" Moment 30 : %lf\n", results.sGeometricMoments.f64GeometricM30);
			printf(" Moment 21 : %lf\n", results.sGeometricMoments.f64GeometricM21);
			printf(" Moment 12 : %lf\n", results.sGeometricMoments.f64GeometricM12);
			printf(" Moment 03 : %lf\n", results.sGeometricMoments.f64GeometricM03);
			printf("\n");
		}

		if(bIsCalcCentroidMoment)
		{
			printf("< Centroid Moment > \n");
			printf(" Moment Centroid X : %lf\n", results.sCentroidMoment.f64CentroidX);
			printf(" Moment Centroid Y : %lf\n", results.sCentroidMoment.f64CentroidY);
			printf("\n");
		}

		if(bIsCalcCentralMoment)
		{
			printf("< Central Moment > \n");
			printf(" Moment 00 : %lf\n", results.sCentralMoments.f64CentralM00);
			printf(" Moment 10 : %lf\n", results.sCentralMoments.f64CentralM10);
			printf(" Moment 01 : %lf\n", results.sCentralMoments.f64CentralM01);
			printf(" Moment 20 : %lf\n", results.sCentralMoments.f64CentralM20);
			printf(" Moment 11 : %lf\n", results.sCentralMoments.f64CentralM11);
			printf(" Moment 02 : %lf\n", results.sCentralMoments.f64CentralM02);
			printf(" Moment 30 : %lf\n", results.sCentralMoments.f64CentralM30);
			printf(" Moment 21 : %lf\n", results.sCentralMoments.f64CentralM21);
			printf(" Moment 12 : %lf\n", results.sCentralMoments.f64CentralM12);
			printf(" Moment 03 : %lf\n", results.sCentralMoments.f64CentralM03);
			printf("\n");
		}

		if(bIsCalcNormalizedCentralMoment)
		{
			printf("< Normalized Central Moment > \n");
			printf(" Moment 00 : %lf\n", results.sNormalizedCentralMoments.f64NormalizedCentralM00);
			printf(" Moment 10 : %lf\n", results.sNormalizedCentralMoments.f64NormalizedCentralM10);
			printf(" Moment 01 : %lf\n", results.sNormalizedCentralMoments.f64NormalizedCentralM01);
			printf(" Moment 20 : %lf\n", results.sNormalizedCentralMoments.f64NormalizedCentralM20);
			printf(" Moment 11 : %lf\n", results.sNormalizedCentralMoments.f64NormalizedCentralM11);
			printf(" Moment 02 : %lf\n", results.sNormalizedCentralMoments.f64NormalizedCentralM02);
			printf(" Moment 30 : %lf\n", results.sNormalizedCentralMoments.f64NormalizedCentralM30);
			printf(" Moment 21 : %lf\n", results.sNormalizedCentralMoments.f64NormalizedCentralM21);
			printf(" Moment 12 : %lf\n", results.sNormalizedCentralMoments.f64NormalizedCentralM12);
			printf(" Moment 03 : %lf\n", results.sNormalizedCentralMoments.f64NormalizedCentralM03);
			printf("\n");
		}

		if(bIsCalcHuMoment)
		{
			printf("< Hu Moment > \n");
			printf(" Hu 0 : %lf\n", results.sHuMoments.f64Hu0);
			printf(" Hu 1 : %lf\n", results.sHuMoments.f64Hu1);
			printf(" Hu 2 : %lf\n", results.sHuMoments.f64Hu2);
			printf(" Hu 3 : %lf\n", results.sHuMoments.f64Hu3);
			printf(" Hu 4 : %lf\n", results.sHuMoments.f64Hu4);
			printf(" Hu 5 : %lf\n", results.sHuMoments.f64Hu5);
			printf(" Hu 6 : %lf\n", results.sHuMoments.f64Hu6);
			printf("\n");
		}
		
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();

		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the ROI area is
		if(IsFail(res = layer.DrawFigureImage(&flrROI, BLUE)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
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