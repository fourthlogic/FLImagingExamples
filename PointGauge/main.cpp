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
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/Gauge/Plate.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates imageview
		if(IsFail(eResult = viewImage.Create(400, 0, 1424, 768)))
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

		// Point Gauge ��ü ���� // Create Point Gauge object
		CPointGauge pointGauge;

		// ó���� �̹��� ���� // Set the image to process
		pointGauge.SetSourceImage(fliImage);

		// ������ ������ �����մϴ�. // Set the area to measure.
		CFLPointD measureCenter(267.481728,240.846156);
		double tolerance = 400.;
		double angle = 25.;
		pointGauge.SetMeasurementRegion(measureCenter, tolerance, angle);

		// �����ϱ����� �Ķ���͸� �����մϴ�. // Set parameters for extraction.
		// ���� �����ϱ����� ������ ����� ��ȭ ���⿡ ���� �����մϴ�. // Set the boundary point change direction to extract to estimate the point.
		pointGauge.SetTransitionType(CPointGauge::ETransitionType_DarkToBrightOrBrightToDark);
		// ������ ����� �� ���������� ������ϴ� ����� ������ �����մϴ�. // Select the boundary point type you want to finally get among the extracted boundary points.
		pointGauge.SetTransitionChoice(CPointGauge::ETransitionChoice_Begin);
		// ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� ���� �����մϴ�. // Set the threshold change of the boundary point to be extracted to estimate the point.
		pointGauge.SetThreshold(20);
		// ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� �������� �����մϴ�. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the point.
		pointGauge.SetMinimumAmplitude(10);
		// ���� �����ϱ����� ������ ��������� ��ǥ�� ǥ�� ������ �����մϴ�. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the points.
		pointGauge.SetThickness(1);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(eResult = pointGauge.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute point gauge.\n");
			break;
		}

		// ���� ����� �����ɴϴ�. // Get the execution result.
		int64_t i64Count = pointGauge.GetMeasuredObjectCount();
		// ���������� ���� ���� �����ɴϴ�. // Get the line used in the estimation process.
		CFLLineD fllLine = pointGauge.GetMeasurementRegion();

		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();

		// ����� ���� ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the extracted point is
		for(int32_t i32Index = 0; i32Index < (int32_t)i64Count; ++i32Index)
		{
			CFLPointD flp;
			pointGauge.GetMeasuredObject(flp, i32Index);

			if(IsFail(eResult = layer.DrawFigureImage(flp.MakeCrossHair(7, true), BLACK, 3)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			if(IsFail(eResult = layer.DrawFigureImage(flp.MakeCrossHair(7, true), YELLOW)))
			{
				ErrorPrint(eResult, "Failed to draw figure\n");
				break;
			}

			printf("Index %d : (%lf, %lf)\n", i32Index, flp.x, flp.y);
		}

		// ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the measurement area is
		if(IsFail(eResult = layer.DrawFigureImage(&fllLine, BLUE)))
		{
			ErrorPrint(eResult, "Failed to draw figures objects on the image view.\n");
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