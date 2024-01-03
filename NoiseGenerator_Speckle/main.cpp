#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliImage;
	CFLImage fliDestinationImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage[2];
	CResult eResult;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/NoiseGenerator/Plate.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(eResult = fliDestinationImage.Load(L"../../ExampleImages/NoiseGenerator/Plate.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates imageview
		if(IsFail(eResult = viewImage[0].Create(400, 0, 912, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].Create(912, 0, 1424, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ�. // Synchronize the viewpoints of the two image views.
		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(eResult = viewImage[0].SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].SetImagePtr(&fliDestinationImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		CResult eResult = EResult_UnknownError;

		// Noise Generator ��ü ���� // Create Noise Generator object
		CNoiseGenerator noiseGenerator;

		// ROI ���� ���� // Set ROI range
		CFLRectL flrROI(61, 63, 583, 376);

		// ó���� �̹��� ���� // Set the image to process
		noiseGenerator.SetSourceImage(fliImage);
		noiseGenerator.SetDestinationImage(fliDestinationImage);
		// ó���� ROI ���� // Set the ROI to be processed
		noiseGenerator.SetSourceROI(flrROI);
		noiseGenerator.SetDestinationROI(flrROI);

		// ������ ������ ���� // Set the noise to generate
		noiseGenerator.SetNoiseType(CNoiseGenerator::ENoiseType_SpeckleNoise);
		// ���� ������ ���� ���� // Set the ratio of speckle noise
		noiseGenerator.SetSpeckleNoise(0.05);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(eResult = noiseGenerator.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute noise generator.\n");
			break;
		}

		CGUIViewImageLayerWrap layer = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImage[1].GetLayer(0);

		layer.Clear();
		layerDst.Clear();

		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the ROI area is
		if(IsFail(eResult = layer.DrawFigureImage(&flrROI, LIME)))
		{
			ErrorPrint(eResult, "Failed to draw figure\n");
			break;
		}

		if(IsFail(eResult = layerDst.DrawFigureImage(&flrROI, LIME)))
		{
			ErrorPrint(eResult, "Failed to draw figure\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if(IsFail(eResult = layer.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerDst.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}