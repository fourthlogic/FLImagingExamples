#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliIndexImage;
	CFLImage fliDestinationImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageIndex;
	CGUIViewImageWrap viewImageDestination;

	// �˰��� ���� ��� // Algorithm execution result
	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = fliSourceImage.Load(L"../../ExampleImages/PagePixelPicker/MultiFile_Normal.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Index �̹��� �ε�
		if((res = fliIndexImage.Load(L"../../ExampleImages/PagePixelPicker/IndexMap.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((res = viewImageSource.Create(400, 0, 900, 500)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Index �̹��� �� ����
		if((res = viewImageIndex.Create(900, 0, 1400, 500)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if((res = viewImageDestination.Create(1400, 0, 1900, 500)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImageSource.SynchronizePointOfView(&viewImageDestination)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImageDestination.SynchronizePointOfView(&viewImageIndex)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = viewImageSource.SynchronizeWindow(&viewImageDestination)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = viewImageDestination.SynchronizeWindow(&viewImageIndex)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((res = viewImageSource.SetImagePtr(&fliSourceImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((res = viewImageDestination.SetImagePtr(&fliDestinationImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Index �̹��� �信 �̹����� ���÷��� // Display the image in the Index image view
		if((res = viewImageIndex.SetImagePtr(&fliIndexImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		viewImageSource.SetFixThumbnailView(true);

		// Page Pixel Picker ��ü ���� // Create Page Pixel Picker object
		CPagePixelPicker pagePixelPicker;

		// Source �̹��� ���� // Set the source image
		// ��� Source �̹����� ������ ������� ������ �������մϴ�. // All Source images must have the same size and format.
		pagePixelPicker.SetSourceImage(fliSourceImage);

		// Index �̹��� ���� // Set the index image
		// 8bit �� 16 bit �� �����ϸ� �ݵ�� �ԷµǾ�� �մϴ�. // 8 bit and 16 bit are supported and must be entered.
		// Index �̹����� Source �̹����� Destination �̹����� �ٸ��� �����ؾ� �մϴ�. // The Index image must be set differently from the source image or destination image.
		// Index �̹����� �� �ȼ� ���� �Էµ� Source �̹����� ������ �ε����� �ǹ��մϴ�. // Each pixel value of the index image means a page index of the input Source image.
		pagePixelPicker.SetIndexImage(fliIndexImage);

		// Destination �̹��� ���� // Set the destination image
		pagePixelPicker.SetDestinationImage(fliDestinationImage);

		// Source ROI, Source Pivot, Destination ROI, Destination Pivot ����� �����մϴ�. // Source ROI, Source Pivot, Destination ROI and Destination Pivot are supported
		// ROI �� Pivot �� Image Operation Scalar ����� ������ ������ �����մϴ�. // ROI and Pivot perform the same behavior as the Image Operation Scalar operation.
		 
		// Blank Color ���� // Set blank color
		// �⺻������ Image Operation �� Blank Color �� ������ ������ �մϴ�. // By default, it behaves the same as the Blank Color in Image Operation.
		// Index Image �ȼ� ���� �ش�Ǵ� ������ �ε����� �������� ���� ��� Blank color �� ä�����ϴ�. // If the page index corresponding to the index image pixel value does not exist, it is populated with Blank color.
		pagePixelPicker.SetBlankColor(200);

		//EnableFillBlankColorMode ����� �����ϸ�, Image Operation Scalar ����� ������ ������ �����մϴ�. // It supports the EnableFillBlankColorMode function and performs the same behavior as the Image Operation Scalar operation.
		//pagePixelPicker.EnableFillBlankColorMode(true);
		
		// Sampling �޼ҵ� ���� // Set sampling method
		//	- ESamplingMethod_Normal : Index �̹��� �� �ȼ��� ��ǥ�� �����Ǵ� Source �̹��� �ȼ� ���� ���� ����մϴ�. // - ESamplingMethod_Normal : Index Image Outputs the source image pixel color value corresponding to each pixel and coordinate.
		//	- ESamplingMethod_Gaussian : Index �̹��� �� �ȼ��� ��ǥ�� �����Ǵ� Source �̹����� �� �� ������ ���� ������ ����þ� ���� ����Ͽ� ����մϴ�. // - ESamplingMethod_Gaussian : Index image calculates and outputs Gaussian values with the source image corresponding to each pixel and coordinates and the front and rear page color values.
		pagePixelPicker.SetSamplingMethod(CPagePixelPicker::ESamplingMethod_Normal);
		
		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = pagePixelPicker.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute Page Pixel Picker.\n");
			
			printf("\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerIndex = viewImageIndex.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImageDestination.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerIndex.Clear();
		layerDestination.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = layerIndex.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Index Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = layerDestination.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSource.Invalidate(true);
		viewImageIndex.Invalidate(true);
		viewImageDestination.Invalidate(true);

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((res = viewImageIndex.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit\n");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((res = viewImageDestination.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit\n");
			break;
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSource.IsAvailable() && viewImageDestination.IsAvailable()&& viewImageIndex.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}