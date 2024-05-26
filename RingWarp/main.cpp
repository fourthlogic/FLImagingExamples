#include <cstdio>
#include "../CommomHeader/ErrorPrint.h"
#include <FLImaging.h>


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliDestinationImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageDestination;

	do
	{
		// �˰��� ���� ��� // Algorithmic executation result
		CResult res = EResult_UnknownError;
		// Source �̹��� �ε� // Load the source image
		if(IsFail(res = fliSourceImage.Load(L"../../ExampleImages/RingWarping/CircleColor.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(res = viewImageSource.Create(400, 0, 400 + 512, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if(IsFail(res = viewImageDestination.Create(400 + 512, 0, 400 + 512 * 2, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Ring Warping ��ü ���� // Create Ring Warping object
		CRingWarping RingWarping;

		// Source �̹��� ���� // Set the source image
		RingWarping.SetSourceImage(fliSourceImage);

		// Source �̹��� ���� ���� �Ķ���� ����
		CFLDoughnut<double> fldSourceROI(257.071130, 257.071130, 216.368201, 118.521494, 0.000000, -17.159659, 213.494067, EArcClosingMethod_Center);
		// Source �̹��� ���� ���� ����
		RingWarping.SetSourceROI(fldSourceROI);

		// Destination �̹��� ���� // Set the destination image
		RingWarping.SetDestinationImage(fliDestinationImage);

		// ������ ���� (Bicubic / Bilinear / NearestNeighbor / Lanczos)
		RingWarping.SetInterpolationMethod(EInterpolationMethod_Bilinear);

		// ���� ���� ���� �� ���� // Set ���� ���� ���� value
		CMultiVar<double> mvBlankColor(10, 160, 20);

		// ���� ���� ���� ����
		RingWarping.SetBlankColor(mvBlankColor);

		// �׻� ���� ������ ������ ������ ä�쵵�� ����
		RingWarping.EnableFillBlankColorMode(true);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = RingWarping.Execute()))
		{
			ErrorPrint(res, "Failed to execute RingWarping.\n");
			
			
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if(IsFail(res = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if(IsFail(res = viewImageDestination.SetImagePtr(&fliDestinationImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageSource.SynchronizeWindow(&viewImageDestination)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImageDestination.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerDestination.Clear();

		// FLImaging�� Figure��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ����
		// Source ROI ������ ������� �˱� ���� ���÷��� �Ѵ�
		if(IsFail(res = layerSource.DrawFigureImage(&fldSourceROI, LIME, 3)))
			ErrorPrint(res, "Failed to draw figure\n");

		// �̹��� �� ���� ǥ�� // Display image view information
		if(IsFail(res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerDestination.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
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