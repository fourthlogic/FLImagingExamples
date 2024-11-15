#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliDestinationImage0;
	CFLImage fliDestinationImage1;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst0;
	CGUIViewImageWrap viewImageDst1;

	 // ���� ��� ��ü ���� // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(res = fliSourceImage.Load(L"../../ExampleImages/OperationHardShrinkage/Coord1D.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image views
		if ((res = viewImageSrc.Create(100, 0, 600, 545)).IsFail() ||
			(res = viewImageDst0.Create(600, 0, 1100, 545)).IsFail() ||
			(res = viewImageDst1.Create(1100, 0, 1600, 545)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ�Ѵ� // Synchronize the viewpoints of the two image views
		if ((res = viewImageSrc.SynchronizePointOfView(&viewImageDst0)).IsFail() ||
			(res = viewImageSrc.SynchronizePointOfView(&viewImageDst1)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the images in the image views
		if ((res = viewImageSrc.SetImagePtr(&fliSourceImage)).IsFail() ||
			(res = viewImageDst0.SetImagePtr(&fliDestinationImage0)).IsFail() ||
			(res = viewImageDst1.SetImagePtr(&fliDestinationImage1)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ�Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageDst0)).IsFail() ||
		   (res = viewImageSrc.SynchronizeWindow(&viewImageDst1)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Operation HardShrinkage ��ü ���� // Create Operation HardShrinkage object
		COperationHardShrinkage algObject;

		// �̹��� ���� // Set the images
		algObject.SetSourceImage(fliSourceImage);
		algObject.SetDestinationImage(fliDestinationImage0);
		algObject.SetOperationMode(COperationHardShrinkage::EOperationMode_Forward);
		algObject.SetLambda(20);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = algObject.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute operation HardShrinkage.");
			break;
		}

		algObject.SetDestinationImage(fliDestinationImage1);
		algObject.SetOperationMode(COperationHardShrinkage::EOperationMode_Backward);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = algObject.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute operation HardShrinkage.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View ���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination0 = viewImageDst0.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination1 = viewImageDst1.GetLayer(0);

		// ������ Layer �� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerDestination0.Clear();
		layerDestination1.Clear();

		// �̹��� �� ���� ǥ�� // Display image view information
		CFLPoint<double> flpPoint = CFLPoint<double>(0, 0);

		if ((res = layerSource.DrawTextCanvas(&flpPoint, L"Source Image", YELLOW, BLACK, 20)).IsFail() ||
			(res = layerDestination0.DrawTextCanvas(&flpPoint, L"Destination Forward Image", YELLOW, BLACK, 20)).IsFail() ||
			(res = layerDestination1.DrawTextCanvas(&flpPoint, L"Destination Backward Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		viewImageSrc.SetPixelNumberMode(EPixelNumberMode_Decimal);

		// �̹��� �並 ���� // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst0.Invalidate(true);
		viewImageDst1.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewImageDst0.IsAvailable() && viewImageDst1.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}