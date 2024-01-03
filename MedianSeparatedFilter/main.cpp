#include <cstdio>
#include "../CommomHeader/ErrorPrint.h"
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
        // �˰��� ���� ��� // Algorithmic executation result
		CResult eResult = EResult_UnknownError;

		// Source �̹��� �ε� // Load the source image
		if(IsFail(eResult = fliSrcImage.Load(L"../../ExampleImages/MedianFilter/Chip_Noise.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}
		
		// Destination�̹����� Src �̹����� ������ �̹����� ����
		if(IsFail(eResult = fliDstImage.Assign(fliSrcImage)))
		{
			ErrorPrint(eResult, "Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create the source image view
		if(IsFail(eResult = viewImageSrc.Create(400, 0, 912, 612)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if(IsFail(eResult = viewImageSrc.SetImagePtr(&fliSrcImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
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

		// Source�̹��� ��� Dst �̹��� ���� ������ ����
		if(IsFail(eResult = viewImageSrc.SynchronizePointOfView(&viewImageDst)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImageSrc.SynchronizeWindow(&viewImageDst)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Source Image ũ�⿡ �°� view�� ũ�⸦ ����
		if(IsFail(eResult = viewImageSrc.ZoomFit()))
		{
			ErrorPrint(eResult, "Failed to zoom fit\n");
			break;
		}

		// Destination Image ũ�⿡ �°� view�� ũ�⸦ ����
		if(IsFail(eResult = viewImageDst.ZoomFit()))
		{
			ErrorPrint(eResult, "Failed to zoom fit\n");
			break;
		}

		// MedianSeparatedFilter ��ü ���� // Create MedianSeparatedFilter object
		CMedianSeparatedFilter medianSeparateFilter;

		CFLRectL flrROI(100, 190, 360, 420);

		// Source �̹��� ���� // Set the source image
		medianSeparateFilter.SetSourceImage(fliSrcImage);

		// Source ROI ���� // Set the source ROI
		medianSeparateFilter.SetSourceROI(flrROI);

		// Destination �̹��� ���� // Set the destination image
		medianSeparateFilter.SetDestinationImage(fliDstImage);

		// Destination ROI ����
		medianSeparateFilter.SetDestinationROI(flrROI);

		// ó���� Filter�� Kernel Size ���� (KernelSize = 11 �� ���, Kernel Size : 11x11, Ȧ���� ��������)
		medianSeparateFilter.SetKernel(11);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = medianSeparateFilter.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute morphology MedianSeparate.");
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
		if(IsFail(eResult = layerSrc.DrawFigureImage(&flrROI, LIME)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		if(IsFail(eResult = layerDst.DrawFigureImage(&flrROI, LIME)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		if(IsFail(eResult = layerSrc.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerDst.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
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