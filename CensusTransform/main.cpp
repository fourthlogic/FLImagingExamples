#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliDestinationImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	 // ���� ��� ��ü ���� // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = fliSourceImage.Load(L"../../ExampleImages/CensusTransform/Src.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹����� Source �̹����� �����ϰ� ���� // Assign the Source image to Destination image
		if((res = fliDestinationImage.Assign(fliSourceImage)).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((res = viewImageSrc.Create(100, 0, 600, 545)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the Destination image view
		if((res = viewImageDst.Create(600, 0,1100, 545)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ�Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImageSrc.SynchronizePointOfView(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((res = viewImageSrc.SetImagePtr(&fliSourceImage)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((res = viewImageDst.SetImagePtr(&fliDestinationImage)).IsFail())
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

		// CensusTransform ��ü ���� // Create CensusTransform object
		CCensusTransform censusTransform;

		// Source �̹��� ���� // Set the source image
		censusTransform.SetSourceImage(fliSourceImage);

		// Destination �̹��� ���� // Set the destination image
		censusTransform.SetDestinationImage(fliDestinationImage);

		// Kernel�� ���� ���� // Set the height of the kernel
		censusTransform.SetKernelHeight(7);

		// Kernel�� ���� ���� // Set the width of the kernel
		censusTransform.SetKernelWidth(9);

		// Kernel�� �ȼ� ���� ũ�⸦ ���� // Set the width between each pixel in the kernel
		censusTransform.SetSparseLength(1);

		// �̷� ��� ���� // Set mirror mode
		censusTransform.EnableMirrorMode(true);

		// ������ ���� ���� ���� ���� // Set data alignment direction
		censusTransform.EnableMSBShiftMode(true);

		// �ռ� ������ �Ķ���� ��� �˰����� ���� // Execute algorithm according to previously set parameters
		if((res = censusTransform.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute census transform.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImageDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerDestination.Clear();

		// �̹��� �� ���� ǥ�� // Display image view information
		CFLPoint<double> flpPoint = CFLPoint<double>(0, 0);

		if((res = layerSource.DrawTextCanvas(&flpPoint, L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = layerDestination.DrawTextCanvas(&flpPoint, L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

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