#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliSourceImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageSource;

	do
	{
		// ���� ��� // operation result
		CResult res = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliSourceImage.Load(L"../../ExampleImages/HoughTransform/Sudoku.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImageSource.Create(100, 0, 100 + 440, 340)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(res = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// HoughTransform  ��ü ���� // Create HoughTransform object
		CHoughTransform HoughTransform;

		// Source �̹��� ���� // Set source image
		HoughTransform.SetSourceImage(fliSourceImage);

		// HoughTransform Line ��ȯ ���� // Set houghTransform line transform
		HoughTransform.SetHoughShape(CHoughTransform::EHoughShape_Line);

		// ���� ��� ���� // Set calculation method
		HoughTransform.SetExecuteMode(CHoughTransform::EExecuteMode_Image);

		// Threshold �� ���� // set threshold value
		HoughTransform.SetPixelThreshold(10);

		// ���� Ÿ�� ���� Less (Threshold �� ������ �ȼ�) // set logical condition(pixels below the threshold value)
		HoughTransform.SetLogicalCondition(ELogicalCondition_Less);

		// �ּ� �ȼ� ī���� �� (�ȼ� ī���� ���غ��� ���� ��� ���͸�) // Minimum number of pixel counters (Filter if lower than pixel counter criteria)
		HoughTransform.SetMinPixelCount(200);

		// ���� �Ÿ� ���͸� ���� (�Ÿ� +-5, ���� +-5 ���� ���� ī������ ���̵� �ȼ� ���� Line���� ����) 
		// Neighbor Distance Filtering Settings(Based on distance +-5, angle +-5, the most counted pixel value is selected as a line)
		HoughTransform.SetNearbyLineFilter(5);

		// �˰��� ���� // Execute the algorithm
		if((res = HoughTransform.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute HoughTransform .");
			break;
		}

		// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
		// ���� ������ �ʿ� ���� // No need to release separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Delete the shapes drawn on the existing layer
		layerSource.Clear();

		// Result ���� �������� // get result count
		int64_t i64ResultCount = HoughTransform.GetResultLinesCount();

		for(int64_t i = 0; i < i64ResultCount; i++) // ���
		{
			CFLLineD fllLine;

			// line ��� �������� // get result line
			HoughTransform.GetResultLine(i, fllLine);

			// line ��� // display line
			if(IsFail(res = layerSource.DrawFigureImage(fllLine, LIGHTGREEN, 1)))
			{
				ErrorPrint(res, "Failed to draw Figure\n");
				break;
			}
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImageSource.Invalidate(true);

		// �̹��� �� / �׷��� �䰡 ����� �� ���� ��ٸ�
		while(viewImageSource.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);


	return 0;
}