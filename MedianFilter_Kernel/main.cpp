#include <cstdio>

#include <FLImaging.h>


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSrcImage;
	CFLImage fliDstImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if(IsFail(fliSrcImage.Load(L"../../ExampleImages/MedianFilter/Chip_Noise.flif")))
		{
			printf("Failed to load the image file.\n");
			break;
		}
		
		// Destination�̹����� Src �̹����� ������ �̹����� ����
		if(IsFail(fliDstImage.Assign(fliSrcImage)))
		{
			printf("Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create the source image view
		if(IsFail(viewImageSrc.Create(400, 0, 912, 612)))
		{
			printf("Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if(IsFail(viewImageSrc.SetImagePtr(&fliSrcImage)))
		{
			printf("Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the destination image view
		if(IsFail(viewImageDst.Create(912, 0, 1424, 612)))
		{
			printf("Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if(IsFail(viewImageDst.SetImagePtr(&fliDstImage)))
		{
			printf("Failed to set image object on the image view.\n");
			break;
		}

		// Source�̹��� ��� Dst �̹��� ���� ������ ����
		if(IsFail(viewImageSrc.SynchronizePointOfView(&viewImageDst)))
		{
			printf("Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(viewImageSrc.SynchronizeWindow(&viewImageDst)))
		{
			printf("Failed to synchronize window.\n");
			break;
		}

		// Source Image ũ�⿡ �°� view�� ũ�⸦ ����
		if(IsFail(viewImageSrc.ZoomFit()))
		{
			printf("Failed to zoom fit\n");
			break;
		}

		// Destination Image ũ�⿡ �°� view�� ũ�⸦ ����
		if(IsFail(viewImageDst.ZoomFit()))
		{
			printf("Failed to zoom fit\n");
			break;
		}

		// MedianFilter ��ü ���� // Create MedianFilter object
		CMedianFilter medianFilter;

		CFLRectL flrROI(100, 190, 360, 420);

		// Source �̹��� ���� // Set the source image
		medianFilter.SetSourceImage(fliSrcImage);

		// Source ROI ���� // Set the source ROI
		medianFilter.SetSourceROI(flrROI);

		// Destination �̹��� ���� // Set the destination image
		medianFilter.SetDestinationImage(fliDstImage);

		// Destination ROI ����
		medianFilter.SetDestinationROI(flrROI);

		// ó���� Filter�� Kernel Size ���� (KernelSize = 5 �� ���)
		medianFilter.SetKernel(5);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(medianFilter.Execute()))
		{
			printf("Failed to execute median filter.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerDst.Clear();

		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
		// FLImaging�� Figure��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ����
		if(IsFail(layerSrc.DrawFigureImage(&flrROI, LIME)))
			printf("Failed to draw figure\n");

		if(IsFail(layerDst.DrawFigureImage(&flrROI, LIME)))
			printf("Failed to draw figure\n");

		if(IsFail(layerSrc.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			printf("Failed to draw text\n");
			break;
		}

		if(IsFail(layerDst.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 20)))
		{
			printf("Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}