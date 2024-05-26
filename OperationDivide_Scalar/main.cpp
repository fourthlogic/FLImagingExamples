#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliDestinationImage1;
	CFLImage fliDestinationImage2;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageDestination1;
	CGUIViewImageWrap viewImageDestination2;

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult res;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if(IsFail(res = fliSourceImage.Load(L"../../ExampleImages/OperationDivide/FlowerWall.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹����� Src �̹����� ������ �̹����� ����
		if(IsFail(res = fliDestinationImage1.Assign(fliSourceImage)))
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		if(IsFail(res = fliDestinationImage2.Assign(fliSourceImage)))
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(res = viewImageSource.Create(100, 0, 612, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination1 �̹��� �� ���� // Create destination1 image view
		if(IsFail(res = viewImageDestination1.Create(612, 0,1124, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination2 image view
		if(IsFail(res = viewImageDestination2.Create(1124, 0,1636, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(res = viewImageSource.SynchronizePointOfView(&viewImageDestination1)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(res = viewImageSource.SynchronizePointOfView(&viewImageDestination2)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if(IsFail(res = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination1 �̹��� �信 �̹����� ���÷���
		if(IsFail(res = viewImageDestination1.SetImagePtr(&fliDestinationImage1)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination2 �̹��� �信 �̹����� ���÷���
		if(IsFail(res = viewImageDestination2.SetImagePtr(&fliDestinationImage2)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageSource.SynchronizeWindow(&viewImageDestination1)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageSource.SynchronizeWindow(&viewImageDestination2)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Operation Divide ��ü ���� // Create Operation Divide object
		COperationDivide divide;
		// Source �̹��� ���� // Set the source image
		divide.SetSourceImage(fliSourceImage);
		// ���� ��� ���� // Set operation source
		divide.SetOperationSource(EOperationSource_Scalar);
		// Destination1 �̹��� ���� // Set the destination1 image
		divide.SetDestinationImage(fliDestinationImage1);
		// Scalar �� ���� // Set Scalar value
		divide.SetScalarValue(CMultiVar<double>(1.5, 1.5, 1.5));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = divide.Execute()))
		{
			ErrorPrint(res, "Failed to execute operation divide.");
			break;
		}

		// Destination1 �̹��� ���� // Set the destination1 image
		divide.SetDestinationImage(fliDestinationImage2);
		// Scalar �� ���� // Set Scalar value
		divide.SetScalarValue(CMultiVar<double>(3.0, 3.0, 3.0));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = divide.Execute()))
		{
			ErrorPrint(res, "Failed to execute operation divide.");
			break;
		}
		

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination1 = viewImageDestination1.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination2 = viewImageDestination2.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerDestination1.Clear();
		layerDestination2.Clear();

		// �̹��� �� ���� ǥ�� // Display image view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerDestination1.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination1 Image(Divide 1.5)", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerDestination2.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination2 Image(Divide 3.0)", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		viewImageSource.Invalidate(true);
		viewImageDestination1.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSource.IsAvailable()
			&& viewImageDestination1.IsAvailable()
			&& viewImageDestination2.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}