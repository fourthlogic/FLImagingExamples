#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliDestinationImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageDestination;

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult eResult = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((eResult = fliSourceImage.Load(L"../../ExampleImages/EntropyFilter/Cat3Ch.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹��� �ε� // Load the destination image
		if((eResult = fliDestinationImage.Load(L"../../ExampleImages/EntropyFilter/Cat3Ch.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((eResult = viewImageSource.Create(100, 0, 550, 480)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if((eResult = viewImageDestination.Create(550, 0, 1000, 480)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Source �̹��� ��� Destination �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the source view and the destination view
		if((eResult = viewImageSource.SynchronizePointOfView(&viewImageDestination)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((eResult = viewImageSource.SetImagePtr(&fliSourceImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((eResult = viewImageDestination.SetImagePtr(&fliDestinationImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((eResult = viewImageSource.SynchronizeWindow(&viewImageDestination)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// EntropyFilter ��ü ���� // Create EntropyFilter object
		CEntropyFilter entropyFilter;

		// Source �̹��� ���� // Set the source image
		entropyFilter.SetSourceImage(fliSourceImage);

		// Destination �̹��� ���� // Set the destination image
		entropyFilter.SetDestinationImage(fliDestinationImage);

		// Kernel Width = 5 ���� // Set the Kernel Width = 5
		entropyFilter.SetKernelWidth(5);

		// Kernel Height = 5 ���� // Set the Kernel Height = 5
		entropyFilter.SetKernelHeight(5);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = entropyFilter.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute entropy filter.\n");
			wprintf(eResult.GetString());
			ErrorPrint(eResult, "\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImageDestination.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerDestination.Clear();

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if((eResult = layerDestination.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		viewImageSource.Invalidate(true);
		viewImageDestination.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSource.IsAvailable() && viewImageDestination.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}