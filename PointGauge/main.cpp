#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	const int32_t i32ExampleCount = 12;
	const wchar_t* arrWcsViewText[] =
	{
		L"Dark To Bright Or Bright To Dark\nBegin 0",
		L"Dark To Bright Or Bright To Dark\nBegin 1",
		L"Dark To Bright Or Bright To Dark\nBegin 2",
		L"Dark To Bright Or Bright To Dark\nLargest Area",
		L"Dark To Bright Or Bright To Dark\nEnd 0",
		L"Dark To Bright Or Bright To Dark\nEnd 1",
		L"Dark To Bright Or Bright To Dark\nEnd 2",
		L"Dark To Bright Or Bright To Dark\nLargest Amplitude",
		L"Dark To Bright Or Bright To Dark\nClosest",
		L"Dark To Bright\nClosest",
		L"Bright To Dark\nClosest",
		L"Dark To Bright To Dark\nClosest",
	};

	CPointGauge::ETransitionType arrTransitionType[] =
	{
		CPointGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CPointGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CPointGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CPointGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CPointGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CPointGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CPointGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CPointGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CPointGauge::ETransitionType_DarkToBrightOrBrightToDark,
		CPointGauge::ETransitionType_DarkToBright,
		CPointGauge::ETransitionType_BrightToDark,
		CPointGauge::ETransitionType_DarkToBrightToDark,
	};

	CPointGauge::ETransitionChoice arrTransitionChoice[] =
	{
		CPointGauge::ETransitionChoice_Begin,
		CPointGauge::ETransitionChoice_Begin,
		CPointGauge::ETransitionChoice_Begin,
		CPointGauge::ETransitionChoice_LargestArea,
		CPointGauge::ETransitionChoice_End,
		CPointGauge::ETransitionChoice_End,
		CPointGauge::ETransitionChoice_End,
		CPointGauge::ETransitionChoice_LargestAmplitude,
		CPointGauge::ETransitionChoice_Closest,
		CPointGauge::ETransitionChoice_Closest,
		CPointGauge::ETransitionChoice_Closest,
		CPointGauge::ETransitionChoice_Closest,
	};

	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage[i32ExampleCount];
	CResult res;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Gauge/stripe.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates imageview
		for(int32_t i = 0; i < i32ExampleCount; ++i)
		{
			int32_t i32X = 300 * (i % 4);
			int32_t i32Y = 300 * (i / 4);

			if(IsFail(res = viewImage[i].Create(i32X, i32Y, i32X + 300, i32Y + 300)))
			{
				ErrorPrint(res, "Failed to create the image view.\n");
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
			if(IsFail(res = viewImage[i].SetImagePtr(&fliImage)))
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				break;
			}

			// �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the all image views. 
			if(i)
			{
				if(IsFail(res = viewImage[i].SynchronizePointOfView(&viewImage[0])))
				{
					ErrorPrint(res, "Failed to set image object on the image view.\n");
					break;
				}				
			}
		}

		CResult res = EResult_UnknownError;

		// Point Gauge ��ü ���� // Create Point Gauge object
		CPointGauge pointGauge;

		// ó���� �̹��� ���� // Set the image to process
		pointGauge.SetSourceImage(fliImage);

		// ������ ������ �����մϴ�. // Set the area to measure.
		CFLPoint<double> measureCenter(267, 240);
		double tolerance = 400.;
		double angle = 25.;
		pointGauge.SetMeasurementRegion(measureCenter, tolerance, angle);

		// �����ϱ����� �Ķ���͸� �����մϴ�. // Set parameters for extraction.
		// ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� ���� �����մϴ�. // Set the threshold change of the boundary point to be extracted to estimate the point.
		pointGauge.SetThreshold(20);
		// ���� �����ϱ����� ������ ������� ��ȭ �Ӱ谪�� �������� �����մϴ�. // Set the correction value to the threshold change of the boundary point to be extracted to estimate the point.
		pointGauge.SetMinimumAmplitude(10);
		// ���� �����ϱ����� ������ ��������� ��ǥ�� ǥ�� ������ �����մϴ�. // Set the number of representative sample values ??of the boundary points to be extracted to estimate the points.
		pointGauge.SetThickness(1);


		for(int32_t i = 0; i < i32ExampleCount; ++i)
		{
			// ���� �����ϱ����� ������ ����� ��ȭ ���⿡ ���� �����մϴ�. // Set the boundary point change direction to extract to estimate the point.
			pointGauge.SetTransitionType(arrTransitionType[i]);
			// ������ ����� �� ���������� ������ϴ� ����� ������ �����մϴ�. // Select the boundary point type you want to finally get among the extracted boundary points.
			pointGauge.SetTransitionChoice(arrTransitionChoice[i]);

			// �˰��� ���� // Execute the algorithm
			if(IsFail(res = pointGauge.Execute()))
			{
				ErrorPrint(res, "Failed to execute Line gauge.\n");
				break;
			}

			// ���� ����� �����ɴϴ�. // Get the execution result.
			int64_t i64Count = pointGauge.GetMeasuredObjectCount();
			// ���������� ���� ���� �����ɴϴ�. // Get the line used in the estimation process.
			CFLLine<double> fllLine = pointGauge.GetMeasurementRegion();

			CGUIViewImageLayerWrap layer = viewImage[i].GetLayer(0);

			layer.Clear();

			if(IsFail(res = layer.DrawTextImage(CFLPoint<double>(0., 0.), arrWcsViewText[i], YELLOW, BLUE, 20, true)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			// ���� �߽� ��ġ�� ���÷����Ѵ�. // Display the measurement center position.
			if(IsFail(res = layer.DrawFigureImage(measureCenter.MakeCrossHair(10), RED)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			// ����� ���� ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the extracted point is
			for(int32_t i32Index = 0; i32Index < (int32_t)i64Count; ++i32Index)
			{
				CFLPoint<double> flp;
				pointGauge.GetMeasuredObject(flp, i32Index);

				if(IsFail(res = layer.DrawFigureImage(flp.MakeCrossHair(10, true), BLACK, 3)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}

				// begin, end�� ��� ������ index(0~2)�� point�� CYAN�������� ǥ���ϰ�, ������ index�� YELLOW�� ǥ���Ѵ�. // In the case of begin, end, the point of the set index (0-2) is displayed in CYAN color, and the remaining index is displayed in YELLOW.
				COLORREF col = (arrTransitionChoice[i] == CPointGauge::ETransitionChoice_Begin || arrTransitionChoice[i] == CPointGauge::ETransitionChoice_End) && i32Index != i % 4 ? YELLOW : CYAN;

				if(IsFail(res = layer.DrawFigureImage(flp.MakeCrossHair(10, true), col, 3)))
				{
					ErrorPrint(res, "Failed to draw figure\n");
					break;
				}

				printf("Index %d : (%lf, %lf)\n", i32Index, flp.x, flp.y);
			}

			// ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the measurement area is
			if(IsFail(res = layer.DrawFigureImage(&fllLine, BLUE)))
			{
				ErrorPrint(res, "Failed to draw figures objects on the image view.\n");
				break;
			}

			// �̹��� �並 ���� �մϴ�. // Update the image view.
			viewImage[i].Invalidate(true);
		}

		bool bTerminated = false;
		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(!bTerminated)
		{
			for(int32_t i = 0; i < i32ExampleCount; ++i)
				bTerminated |= !viewImage[i].IsAvailable();

			CThreadUtilities::Sleep(1);
		}
	}
	while(false);

	return 0;
}