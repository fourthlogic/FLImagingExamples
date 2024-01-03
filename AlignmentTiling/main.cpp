#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main() // Merge
{
	const int32_t i32SrcImageCount = 4;

	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSrcImage;
	CFLImage fliDstImage;
	CFLImage fliSrcImages[4];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImageSrc[i32SrcImageCount];
	CGUIViewImageWrap viewImageDst;

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult eResult;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if(IsFail(eResult = fliSrcImage.Load(L"../../ExampleImages/AlignmentTiling/AlignmentTilingExampleImage.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create the source image view
		for(int32_t i = 0; i < i32SrcImageCount; ++i)
		{
			if(i < 2)
			{
				if(IsFail(eResult = arrViewImageSrc[i].Create(100 + 300 * i, 0, 400 + 300 * i, 300)))
				{
					ErrorPrint(eResult, "Failed to create the image view.\n");
					break;
				}
			}
			else
			{
				if(IsFail(eResult = arrViewImageSrc[i].Create(100 + 300 * (i - 2), 300, 400 + 300 * (i - 2), 600)))
				{
					ErrorPrint(eResult, "Failed to create the image view.\n");
					break;
				}
			}

			arrViewImageSrc[i].SetFixThumbnailView(true);
		}

		// Source �̹��� �信 �������� ���� �����ϴ� �̹����� ������ �信 ���÷��� �ϱ� ���� ���� ����� ������ �̹��� ��ü�� �Ҵ�.
		// Assign each image object to each image object by shallow copying to display each image on the page in the source image view to each view.
		for(int32_t i = 0; i < i32SrcImageCount; ++i)
		{
			// �����̹����� ���� 4���� ���������� �� ������ ���� ���÷��� �ϱ� ���� ������ �������� ���� ����(Shallow copy)�� ����
			// Perform a shallow copy based on the original to display the 4 pages of the original image for each page
			fliSrcImages[i].Assign(fliSrcImage, false);
			// ���� ����� �̹��� ��ü�� ���� ǥ���� �������� ���� (�̹��� �����ʹ� ����������, ������ ���� ������ ���������� ����)
			// Select a page to display for shallow copied image objects (Image data is shared, but settings such as pages work individually)
			fliSrcImages[i].SelectPage(i);

			// ���� ����� �ش� �������� ������ �̹����� �信 ���÷���
			// Display the selected image of the shallow-copied page in the view
			if(IsFail(eResult = arrViewImageSrc[i].SetImagePtr(&fliSrcImages[i])))
			{
				ErrorPrint(eResult, "Failed to set image object on the image view.\n");
				break;
			}
		}


		// Destination �̹��� �� ���� // Create the destination image view
		if(IsFail(eResult = viewImageDst.Create(912, 0, 1424, 612)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if(IsFail(eResult = viewImageDst.SetImagePtr(&fliDstImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �̹��� �� �������� ��ġ�� ���� // Align the position of the image view window
		for(int32_t i = 1; i < i32SrcImageCount; ++i)
		{
			if(IsFail(eResult = arrViewImageSrc[0].SynchronizeWindow(&arrViewImageSrc[i])))
			{
				ErrorPrint(eResult, "Failed to synchronize window.\n");
				break;
			}
		}

		if(IsFail(eResult = arrViewImageSrc[0].SynchronizeWindow(&viewImageDst)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// AlignmentTiling ��ü ���� // Create AlignmentTiling object
		CAlignmentTiling alignmentTiling;

		// Source �̹��� ���� // Set the source image
		alignmentTiling.SetSourceImage(fliSrcImage);
		// Destination �̹��� ���� // Set the destination image
		alignmentTiling.SetDestinationImage(fliDstImage);
		// ���� ���� ���� ���� // Set the blank color
		alignmentTiling.SetBlankColor(CMultiVar<double>(200.));
		// Interpolation �ɼ� ���� // Set the Interpolation option
		alignmentTiling.SetInterpolationMethod(EInterpolationMethod_Bilinear);
		// ������ �̹����� �迭 ���� (Row, Column) // Set the array of images to be merged (Row, Column)
		alignmentTiling.SetImageArrangement(2, 2);
		// ��� �̹����� ũ�� �������� �ɼ� ���� // Set whether or not to specify the size of the resulting image
		alignmentTiling.SetResizeMethod(EResizeMethod_Resize);
		// Resize�� ���, ��� �̹����� ũ�� ���� // In case of Resize, set the size of the resulting image
		alignmentTiling.SetResultImageSize(1500, 1500);
		// ������ ����� Accuracy ���� // Set the Accuracy to be used for calibration
		alignmentTiling.SetAccuracy(EFloatingPointAccuracy_Bit32);
		// ��� �̹����� �߽��� �� ��ġ�� ���� // Set the location to be the center of the resulting image
		alignmentTiling.SetSourcePivot(Foundation::CFLPointL(192, 778));
		// Pivot�� ������ Page Index�� ���� // Set the Page Index with Pivot specified
		alignmentTiling.SetPivotPageIndex(1);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = alignmentTiling.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute merge.");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if(IsFail(eResult = viewImageDst.ZoomFit()))
		{
			ErrorPrint(eResult, "Failed to zoom fit of the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc[i32SrcImageCount];
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);

		for(int32_t i = 0; i < i32SrcImageCount; ++i)
		{
			layerSrc[i] = arrViewImageSrc[i].GetLayer(0);

			// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
			layerSrc[i].Clear();
		}

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerDst.Clear();

		// ���յ� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to see where the merged area is
		// FLImaging�� Figure ��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ���� // FLImaging's Figure objects can be displayed as a function regardless of the shape
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)

		CFLQuad<double> flqResult;

		for(int32_t i = 0; i < i32SrcImageCount; ++i)
		{
			// ��� �̹��� �������� ������ �ε����� �ش��ϴ� ���յ� ��� ������ �޾ƿ´�.
			// Receive the merged result area corresponding to each index based on the result image.
			alignmentTiling.GetPartialResultRegion(i, flqResult);

			if(IsFail(eResult = layerDst.DrawFigureImage(flqResult, LIME)))
				ErrorPrint(eResult, "Failed to draw figure\n");
		}



		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		for(int32_t i = 0; i < i32SrcImageCount; ++i)
		{
			CFLString<wchar_t> fls;
			fls.Format(L"Source Image #%d", i);

			if(IsFail(eResult = layerSrc[i].DrawTextCanvas(&CFLPointD(0, 0), fls, YELLOW, BLACK, 20)))
			{
				ErrorPrint(eResult, "Failed to draw text\n");
				break;
			}
		}

		if(IsFail(eResult = layerDst.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 �����Ѵ�. // Update the image view.
		for(int32_t i = 0; i < i32SrcImageCount; ++i)
			arrViewImageSrc[i].RedrawWindow();

		viewImageDst.Invalidate(false);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		bool bRun = true;
		while(bRun)
		{
			for(int32_t i = 0; i < 4; ++i)
			{
				bRun &= arrViewImageSrc[i].IsAvailable();
			}

			bRun &= viewImageDst.IsAvailable();

			CThreadUtilities::Sleep(1);
		}
	}
	while(false);

	return 0;
}