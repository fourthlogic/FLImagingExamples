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

		// �̹��� 1 �ε�
		if(IsFail(eResult = fliSourceImage.Load(L"../../ExampleImages/Projection/mountains.flif")))
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
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Projection ��ü ���� // Create Projection object
		CProjection Projection;

		// Source �̹��� ���� // Set source image
		Projection.SetSourceImage(fliSourceImage);

		// ���� ���� ���� // Set operation direction
		Projection.SetProjectionMode(CProjection::EProjectionDirection_Column);

		// �˰��� ���� // Execute the algorithm
		if((eResult = Projection.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Projection.");
			break;
		}

		// Result ��� ���� Ȯ�� // get result count
		int64_t i64Count = Projection.GetResultCount();

		// Channel �� ǥ�⸦ ���� String ���� // string variable to indicate Channel value
		Base::CFLString<wchar_t> strChannel;

		// �׷��� �� ���� // Graph line color
		uint32_t arrColor[10] = { GRAPHVIEW_RED , GRAPHVIEW_GREEN , GRAPHVIEW_BLUE ,VIOLET,CYAN ,MAGENTA,AQUA,ORANGE,BLACK,YELLOW };

		// Projection ����� // Projection Result Object
		Base::CFLArray<double> flaResult;

		for(int64_t i = 0; i < i64Count; ++i)
		{
			// ���� ������ ���� // data clear
			flaResult.Clear();

			// Projection ��� �� �������� // get projection result
			if(IsFail(eResult = Projection.GetResult(i, flaResult)))
				break;

			// ä�� String // Channel String
			strChannel.Format(L"Ch%d", i);

			// Graph View ������ �Է� // Input Graph View Data
			viewGraph.Plot(flaResult, GUI::EChartType_Line, arrColor[i], strChannel, nullptr);
		}

		// �׷��� �並 ���� �մϴ�. // Update the Graph view.
		viewGraph.Invalidate(true);

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

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImageSource.Invalidate(true);

		// �̹��� �� / �׷��� �䰡 ����� �� ���� ��ٸ�
		while(viewImageSource.IsAvailable() && viewGraph.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}