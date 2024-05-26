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
	CResult res;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/NoiseGenerator/Plate.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliDestinationImage.Load(L"../../ExampleImages/NoiseGenerator/Plate.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates imageview
		if(IsFail(res = viewImage[0].Create(400, 0, 912, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(912, 0, 1424, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ�. // Synchronize the viewpoints of the two image views.
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(res = viewImage[0].SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].SetImagePtr(&fliDestinationImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		CResult res = EResult_UnknownError;

		// Noise Generator ��ü ���� // Create Noise Generator object
		CNoiseGenerator noiseGenerator;

		// ROI ���� ���� // Set ROI range
		CFLRect<int32_t> flrROI(61, 63, 583, 376);

		// ó���� �̹��� ���� // Set the image to process
		noiseGenerator.SetSourceImage(fliImage);
		noiseGenerator.SetDestinationImage(fliDestinationImage);
		// ó���� ROI ���� // Set the ROI to be processed
		noiseGenerator.SetSourceROI(flrROI);
		noiseGenerator.SetDestinationROI(flrROI);

		// ������ ������ ���� // Set the noise to generate
		noiseGenerator.SetNoiseType(CNoiseGenerator::ENoiseType_PhotonNoise);
		// ���ھ� ������ ���� ��վ� ���� // Set the average photon amount of photon noise
		noiseGenerator.SetPhotonNoise(100000000);

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = noiseGenerator.Execute()))
		{
			ErrorPrint(res, "Failed to execute noise generator.\n");
			break;
		}

		CGUIViewImageLayerWrap layer = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImage[1].GetLayer(0);

		layer.Clear();
		layerDst.Clear();

		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to know where the ROI area is
		if(IsFail(res = layer.DrawFigureImage(&flrROI, LIME)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		if(IsFail(res = layerDst.DrawFigureImage(&flrROI, LIME)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if(IsFail(res = layer.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerDst.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
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