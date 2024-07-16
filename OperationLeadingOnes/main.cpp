#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSrcImage;
	CFLImage fliDstImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	 // ���� ��� ��ü ���� // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		uint16_t arrU16[16];

		for(int32_t i = 0; i < 16; ++i)
			arrU16[i] = ~(1 << i);

		// ���۷κ��� Source �̹��� ���� // Create the source image from the buffer
		if ((res = fliSrcImage.Create(4, 4, (uint8_t *)arrU16, EPixelFormat_C1_U16)).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image views
		if ((res = viewImageSrc.Create(100, 0,  600, 545)).IsFail() ||
			(res = viewImageDst.Create(600, 0, 1100, 545)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ�Ѵ� // Synchronize the viewpoints of the two image views
		if ((res = viewImageSrc.SynchronizePointOfView(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the images in the image views
		if ((res = viewImageSrc.SetImagePtr(&fliSrcImage)).IsFail() ||
			(res = viewImageDst.SetImagePtr(&fliDstImage)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ�Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Operation Leading Ones ��ü ���� // Create Operation Leading Ones object
		COperationLeadingOnes to;

		// �̹��� ���� // Set the images
		to.SetSourceImage(fliSrcImage);
		to.SetDestinationImage(fliDstImage);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = to.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute operation leading ones.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerDst.Clear();

		// �̹��� �� ���� ǥ�� // Display image view information
		CFLPoint<double> flpPoint = CFLPoint<double>(0, 0);

		if ((res = layerSrc.DrawTextCanvas(&flpPoint, L"Source Image", YELLOW, BLACK, 20)).IsFail() ||
			(res = layerDst.DrawTextCanvas(&flpPoint, L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		viewImageSrc.SetPixelNumberMode(EPixelNumberMode_Hexadecimal);

		// �̹��� �並 ���� // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}