#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliSourceImage;
	CFLImage fliConvertedImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageConverted;

	do
	{
		// ���� ��� // operation result
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(eResult = fliSourceImage.Load(L"../../ExampleImages/Decolorization/Pattern.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImageSource.Create(112, 0, 912, 534)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageConverted.Create(913, 0, 1713, 534)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(eResult = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageConverted.SetImagePtr(&fliConvertedImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views. 
		if(IsFail(eResult = viewImageSource.SynchronizePointOfView(&viewImageConverted)))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImageSource.SynchronizeWindow(&viewImageConverted)))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		// Decolorization ��ü ���� // Create Decolorization object
		CDecolorization Decolorization;

		// Source �̹��� ���� // Set source image 
		if(IsFail(eResult = Decolorization.SetSourceImage(fliSourceImage)))
		{
			ErrorPrint(eResult, "Failed to set Source Image.");
			break;
		}

		// Destination �̹��� ���� // Set destination image
		if(IsFail(eResult = Decolorization.SetDestinationImage(fliConvertedImage)))
		{
			ErrorPrint(eResult, "Failed to set Destination Image.");
			break;
		}

		// ���� �ӵ� ���� // Set the operation speed.
		if(IsFail(eResult = Decolorization.SetOperationSpeed(CDecolorization::EOperationSpeed_Normal)))
		{
			ErrorPrint(eResult, "Failed to set Operation Speed.");
			break;
		}

		// �˰��� ���� // Execute the algorithm
		if((eResult = Decolorization.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to process.");
			break;
		}

		// �̹��� ���� zoom fit // image view zoom fit
		if((eResult = viewImageConverted.ZoomFit()).IsFail())
		{
			ErrorPrint(eResult, "Failed to zoom fit\n");
			break;
		}

		// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
		// ���� ������ �ʿ� ���� // No need to release separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerConverted = viewImageConverted.GetLayer(1);

		// ������ Layer�� �׷��� �������� ���� // Delete the shapes drawn on the existing layer
		layerSource.Clear();
		layerConverted.Clear();

		// View ������ ���÷��� �մϴ�. // Display View information.
		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerConverted.DrawTextCanvas(&CFLPointD(0, 0), L"Converted Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImageSource.Invalidate(true);
		viewImageConverted.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSource.IsAvailable() && viewImageConverted.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}