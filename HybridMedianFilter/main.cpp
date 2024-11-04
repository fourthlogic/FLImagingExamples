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
		CResult res = EResult_UnknownError;

		// Source �̹��� �ε� // Load the source image
		if(IsFail(res = fliSrcImage.Load(L"../../ExampleImages/NoiseImage/NoiseImage1.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}
		
		// Destination�̹����� Source �̹����� ������ �̹����� ���� // Create the destination image as the same image as the source image
		if(IsFail(res = fliDstImage.Assign(fliSrcImage)))
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create the source image view
		if(IsFail(res = viewImageSrc.Create(400, 0, 912, 612)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if(IsFail(res = viewImageSrc.SetImagePtr(&fliSrcImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the destination image view
		if(IsFail(res = viewImageDst.Create(912, 0, 1424, 612)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if(IsFail(res = viewImageDst.SetImagePtr(&fliDstImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Source �̹��� ��� Destination �̹��� ���� ������ ���� // Synchronize the focus of the source image view and destination image view
		if(IsFail(res = viewImageSrc.SynchronizePointOfView(&viewImageDst)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageSrc.SynchronizeWindow(&viewImageDst)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Source Image ũ�⿡ �°� view�� ũ�⸦ ����
		if(IsFail(res = viewImageSrc.ZoomFit()))
		{
			ErrorPrint(res, "Failed to zoom fit\n");
			break;
		}

		// Destination Image ũ�⿡ �°� view�� ũ�⸦ ����
		if(IsFail(res = viewImageDst.ZoomFit()))
		{
			ErrorPrint(res, "Failed to zoom fit\n");
			break;
		}

		// HybridMedianFilter ��ü ���� // Create HybridMedianFilter object
		CHybridMedianFilter hybridMedianFilter;

		CFLRect<int32_t> flrROI(100, 190, 360, 420);

		// Source �̹��� ���� // Set the source image
		hybridMedianFilter.SetSourceImage(fliSrcImage);

		// Source ROI ���� // Set the source ROI
		hybridMedianFilter.SetSourceROI(flrROI);

		// Destination �̹��� ���� // Set the destination image
		hybridMedianFilter.SetDestinationImage(fliDstImage);

		// Destination ROI ����
		hybridMedianFilter.SetDestinationROI(flrROI);

		// ó���� Filter�� Kernel Size ���� (KernelSize = 11 �� ���, Kernel Size : 11x11, Ȧ���� ��������)
		hybridMedianFilter.SetKernel(7);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = hybridMedianFilter.Execute()))
		{
			ErrorPrint(res, "Failed to execute HybridMedianFilter.");
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
		if(IsFail(res = layerSrc.DrawFigureImage(&flrROI, LIME)))
			ErrorPrint(res, "Failed to draw figure\n");

		if(IsFail(res = layerDst.DrawFigureImage(&flrROI, LIME)))
			ErrorPrint(res, "Failed to draw figure\n");

		if(IsFail(res = layerSrc.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerDst.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
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