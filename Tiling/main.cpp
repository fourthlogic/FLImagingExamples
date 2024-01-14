#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main() // Tiling
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
		for(int32_t i = 0; i<i32SrcImageCount; ++i)
		{
			CFLStringW flsFileName;

			flsFileName.Format(L"TilingSourceImage%d.flif", i);

			if(IsFail(eResult = fliSrcImages[i].Load(L"../../ExampleImages/Tiling/" + flsFileName)))
			{
				ErrorPrint(eResult, "Failed to load the image file.\n");
				break;
			}
		}		

		// ���� ���� �̹����� �ϳ��� FLImage�� ���� // Create multiple images into one FLImage
		fliSrcImage = fliSrcImages[0];

		for(int32_t i = 1; i < i32SrcImageCount; ++i)
		{
			fliSrcImage.PushBackPage(fliSrcImages[i]);
		}

		// Destination �̹��� �ε� // Load the destination image
		if(IsFail(eResult = fliDstImage.Load(L"../../ExampleImages/Tiling/TilingDestinationImage.flif")))
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
		for(int32_t i = 1; i< i32SrcImageCount; ++i)
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
		
		// Source �̹����� ROI �߰� // Add ROI to source image
		CFLRectD flRect(30, 68, 200, 235);
		
		flRect.SetName(L"0");
		fliSrcImage.SelectPage(0);
		fliSrcImage.PushBackFigure(CROIUtilities::ConvertFigureObjectToString(flRect));
		fliSrcImages[0].PushBackFigure(CROIUtilities::ConvertFigureObjectToString(flRect));

		flRect = CFLRectD(260, 135, 415, 440);
		flRect.SetName(L"1");
		fliSrcImage.PushBackFigure(CROIUtilities::ConvertFigureObjectToString(flRect));
		fliSrcImages[0].PushBackFigure(CROIUtilities::ConvertFigureObjectToString(flRect));

		flRect = CFLRectD(280, 250, 480, 480);
		flRect.SetName(L"0");
		fliSrcImage.SelectPage(1);
		fliSrcImage.PushBackFigure(CROIUtilities::ConvertFigureObjectToString(flRect));
		fliSrcImages[1].PushBackFigure(CROIUtilities::ConvertFigureObjectToString(flRect));

		flRect = CFLRectD(110, 150, 350, 440);
		flRect.SetName(L"0");
		fliSrcImage.SelectPage(2);
		fliSrcImage.PushBackFigure(CROIUtilities::ConvertFigureObjectToString(flRect));
		fliSrcImages[2].PushBackFigure(CROIUtilities::ConvertFigureObjectToString(flRect));

		flRect = CFLRectD(220, 230, 470, 450);
		flRect.SetName(L"0");
		fliSrcImage.SelectPage(3);
		fliSrcImage.PushBackFigure(CROIUtilities::ConvertFigureObjectToString(flRect));
		fliSrcImages[3].PushBackFigure(CROIUtilities::ConvertFigureObjectToString(flRect));

		// Destination �̹����� ROI �߰� // Add ROI to destination image
		fliDstImage.PushBackFigure(L"D(79.292035, 67.964602, 292.247788, 267.327434, INFO[NAME(0_0)])");
		fliDstImage.PushBackFigure(L"D(296.778761, 271.858407, 459.893805, 444.035398, INFO[NAME(0_1)])");
		fliDstImage.PushBackFigure(L"D(88.353982, 738.548673, 337.557522, 956.035398, INFO[NAME(1_0)])");
		fliDstImage.PushBackFigure(L"D(482.548673, 457.628319, 659.256637, 675.115044, INFO[NAME(2_0)])");
		fliDstImage.PushBackFigure(L"D(659.256638, 222.017700, 835.964602, 439.504425, INFO[NAME(3_0)])");

		// Tiling ��ü ���� // Create Tiling object
		CTiling tiling;

		// Source �̹��� ���� // Set the source image
		tiling.SetSourceImage(fliSrcImage);
		// Destination �̹��� ���� // Set the destination image
		tiling.SetDestinationImage(fliDstImage);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = tiling.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute tiling.");
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

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		for(int32_t i = 0 ; i< i32SrcImageCount; ++i)
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
		for(int32_t i = 0 ; i< i32SrcImageCount; ++i)
			arrViewImageSrc[i].Invalidate(true);

		viewImageDst.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		bool bRun = true;
		while(bRun)
		{
			for(int32_t i = 0 ; i < 4; ++i)
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