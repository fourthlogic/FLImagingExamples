#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliSourceImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewGraphWrap viewGraph;

	do
	{
		// ���� ��� // operation result
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(eResult = fliSourceImage.Load(L"../../ExampleImages/Histogram/Escherichia coli.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImageSource.Create(100, 0, 100 + 440, 340)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Graph �� ���� // Create graph view
		if(IsFail(eResult = viewGraph.Create(100 + 440 * 1, 0, 100 + 440 * 2, 340)))
		{
			ErrorPrint(eResult, " Failed to create the graph view. \n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(eResult = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �������� ��ġ�� ����ȭ �Ѵ� // / Synchronize the positions of windows
		if(IsFail(eResult = viewImageSource.SynchronizeWindow(&viewGraph)))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		// Histogram ��ü ���� // Create Histogram object
		CHistogram Histogram;

		// Source �̹��� ���� // Set source image
		Histogram.SetSourceImage(fliSourceImage);

		// ROI ���� // Create ROI
		CFLRect<double> flrSrcROI(161, 181, 293, 302);

		// Source ROI ���� ���� // set Source ROI 
		Histogram.SetSourceROI(flrSrcROI);

		// �˰��� ���� // Execute the algorithm
		if((eResult = Histogram.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Histogram.");
			break;
		}

		// Result ��� ���� Ȯ�� // get result count
		int64_t i64IndexCount = Histogram.GetResultCount();

		// Channel �� ǥ�⸦ ���� String ���� // string variable to indicate Channel value
		Base::CFLString<wchar_t> strChannel;

		// �׷��� �� ���� // graph line color
		uint32_t arrColor[10] = { GRAPHVIEW_RED , GRAPHVIEW_GREEN , GRAPHVIEW_BLUE ,VIOLET,CYAN ,MAGENTA,AQUA,ORANGE,BLACK,YELLOW };

		// Histogram ����� // Histogram result object
		Base::CFLArray<uint64_t> flaResult;

		for(int64_t i = 0; i < i64IndexCount; ++i)
		{
			// ���� ������ ���� // data clear
			flaResult.Clear();

			// Histogram ��� �� �������� // get histogram result
			if(IsFail(eResult = Histogram.GetResult(i, flaResult)))
				break;

			// ä�� String // Channel String
			strChannel.Format(L"Ch%d", i);

			// Graph View ������ �Է� // Input Graph View Data
			viewGraph.Plot(flaResult, GUI::EChartType_Bar, arrColor[i], strChannel, nullptr);
		}

		// �׷��� �並 ���� �մϴ�. // Update the Graph view.
		viewGraph.Invalidate(true);

		CMultiVar<double> mvMean;
		CMultiVar<double> mvVariance;
		CMultiVar<double> mvStdDev;
		CMultiVar<double> mvMedian;

		// ���, �л�, ǥ������, �߾Ӱ� �ޱ� // get mean, variance, standard deviation, median
		Histogram.GetResultStdDev(mvMean);
		Histogram.GetResultVariance(mvVariance);
		Histogram.GetResultStdDev(mvStdDev);
		Histogram.GetResultMedian(mvMedian);

		// ��� ���� // get count
		int32_t i32Count = (int32_t)mvMean.GetCount();

		// ���, �л�, ǥ������, �߾Ӱ� ��� // display Mean, variance, standard deviation, median
		for(int32_t i = 0; i < i32Count; ++i)
		{
			printf("Channel %d\n", i + 1);
			printf("Mean %lf\n", mvMean[i]);
			printf("Variance %lf\n", mvVariance[i]);
			printf("Standard Deviation %lf\n", mvStdDev[i]);
			printf("Median %lf\n\n", mvMedian[i]);
		}

		// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
		// ���� ������ �ʿ� ���� // No need to release separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Delete the shapes drawn on the existing layer
		layerSource.Clear();

		// View ������ ���÷��� �մϴ�. // Display View information.
		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// View ROI ���÷��� �մϴ�. // Display View ROI.
		if(IsFail(eResult = layerSource.DrawFigureImage(&flrSrcROI, LIME)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImageSource.Invalidate(true);

		// �̹��� �� / �׷��� �䰡 ����� �� ���� ��ٸ�
		while(viewImageSource.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}