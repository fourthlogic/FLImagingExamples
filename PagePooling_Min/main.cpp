#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliDestinationImage;
	CFLImage fliIndexImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageDestination;
	CGUIViewImageWrap viewImageIndex;

	// �˰��� ���� ��� // Algorithm execution result
	CResult eResult = EResult_UnknownError;

	do
	{
		// �̹��� �ε� // Load image
		if((eResult = fliSourceImage.Load(L"../../ExampleImages/PagePooling/Multiple File_Min.flif")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((eResult = viewImageSource.Create(200, 0, 712, 512)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if((eResult = viewImageDestination.Create(712, 0, 1224, 512)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Index �̹��� �� ����
		if((eResult = viewImageIndex.Create(1224, 0, 1736, 512)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((eResult = viewImageSource.SynchronizePointOfView(&viewImageDestination)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((eResult = viewImageDestination.SynchronizePointOfView(&viewImageIndex)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((eResult = viewImageSource.SynchronizeWindow(&viewImageDestination)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((eResult = viewImageDestination.SynchronizeWindow(&viewImageIndex)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to synchronize window.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((eResult = viewImageSource.SetImagePtr(&fliSourceImage)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((eResult = viewImageDestination.SetImagePtr(&fliDestinationImage)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		// Index �̹��� �信 �̹����� ���÷��� // Display the image in the Index image view
		if((eResult = viewImageIndex.SetImagePtr(&fliIndexImage)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		viewImageSource.SetFixThumbnailView(true);

		// Page Pooling ��ü ���� // Create Page Pooling object
		CPagePooling pagePooling;

		// Source �̹��� ���� // Set the source image
		pagePooling.SetSourceImage(fliSourceImage);

		// Destination �̹��� ���� // Set the destination image
		pagePooling.SetDestinationImage(fliDestinationImage);

		// Index �̹��� // Index image
		//	- �� �ȼ����� ������ ��� ���� ��ġ�� ������ �ε��� ���� �����Ǵ� ��ǥ�� �ȼ��� ����մϴ�. // For each pixel, output the page index value where the resulting value is located to the pixel of the corresponding coordinates.
		//	- Sampling �޼ҵ尡 Min Gaussian, Max Gaussian ����� ��� ��µǴ� �ε��� �̹����� ���� Min, Max ���� ������ �� ��µǴ� �ε��� �̹����� �����մϴ�. // If the sampling method is in Min Gaussian and Max Gaussian modes, the output index image is the same as the output index image when operating in Min and Max modes, respectively.
		//	- Sampling �޼ҵ尡 Mean ����� ���� �ε��� �̹��� ����� �������� �ʽ��ϴ�. // Index image output is not supported when the Sampling method is in Mean mode.
		//	- ������ ��� ���� ���� �������� �����ϰ� ������ ���, ���� ���� �ε����� ����մϴ�. // Outputs the leading index if the resulting values to be extracted are equally present on multiple pages.
		//	- SetIndexImage �� SetSourceImage �� SetDestinationImage ���� ������ �̹����� �����ϸ� �������� �ʽ��ϴ�. // - SetIndexImage will not work if it is the same as the image set in SetSourceImage or SetDestinationImage.
		//	- Index Image �� �������� ���� ��� �ε��� �̹����� ������� �ʴ� ���� �����մϴ�. // If SetIndexImage is not specified, it operates in a mode that does not output the index image.
		//	- �ε��� �̹��� ������ �ִ� 65535 �� ������ �����˴ϴ�. // Index image extraction is supported up to 65535 pages
		//	- Source ROI ���� �ۿ� �ش��ϴ� �ε����� ��ȿ ������ 8bit �ε��� �̹��������� 255, 16bit �ε��� �̹��������� 65535 �� �Էµ˴ϴ�. // Indexes outside the Source ROI area are invalid values, with 255 for an 8-bit index image and 65535 for a 16-bit index image.

		// Index �̹��� ����
		pagePooling.SetIndexImage(fliIndexImage);

		// Sampling �޼ҵ� ���� // Set the sampling method
		//	- Max : �Էµ� �̹��� ��� �ִ� ���� ����մϴ�. // Max : Outputs the maximum value of the entered image.
		//	- MaxGaussian : �Էµ� �̹��� ��� ���� �� �� �ε����� ��ġ�� �ִ� ���� �������� ����þ� ���� ����մϴ�. // MaxGaussian : Outputs the Gaussian value based on the maximum value located in the leading index of the entered image.
		//	- Min : �Էµ� �̹��� ��� �ּ� ���� ����մϴ�. // Min : Outputs the minimum value of the entered image.
		//	- MinGaussian : �Էµ� �̹��� ��� ���� �� �� �ε����� ��ġ�� �ּ� ���� �������� ����þ� ���� ����մϴ�. // MinGaussian : Outputs the Gaussian value based on the minimum value located in the leading index of the entered image.
		//	- Mean : �Էµ� �̹������� ��� ���� ����մϴ�. (�ִ� 16843009 �� ���� �����˴ϴ�.) // Mean: Outputs the average value of the entered images. (Up to 16843009 pages are supported.)
		pagePooling.SetSamplingMethod(CPagePooling::ESamplingMethod_Min);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = pagePooling.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute Page Pooling.\n");
			
			printf("\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImageDestination.GetLayer(0);
		CGUIViewImageLayerWrap layerIndex = viewImageIndex.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerDestination.Clear();
		layerIndex.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		if((eResult = layerDestination.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		if((eResult = layerIndex.DrawTextCanvas(&CFLPointD(0, 0), L"Index Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSource.Invalidate(true);
		viewImageDestination.Invalidate(true);
		viewImageIndex.Invalidate(true);

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((eResult = viewImageDestination.ZoomFit()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to zoom fit\n");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((eResult = viewImageIndex.ZoomFit()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to zoom fit\n");
			break;
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSource.IsAvailable() && viewImageDestination.IsAvailable()&& viewImageIndex.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}