#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliImage;
	CFLImage fliRFTImage;
	CFLImage fliIRFTImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageOriginal;
	CGUIViewImageWrap viewImageRFT;
	CGUIViewImageWrap viewImageIRFT;

	do
	{
		// ���� ��� // operation result
		CResult res = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/FourierTransform/TempleNoise.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImageOriginal.Create(300, 0, 300 + 512, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageRFT.Create(300 + 512, 0, 300 + 512 * 2, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageIRFT.Create(300 + 512 * 2, 0, 300 + 512 * 3, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(res = viewImageOriginal.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageIRFT.SetImagePtr(&fliIRFTImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageOriginal.SynchronizeWindow(&viewImageRFT)))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(res = viewImageOriginal.SynchronizeWindow(&viewImageIRFT)))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		// FourierTransformReal ��ü ���� // Create FourierTransformReal object
		CFourierTransformReal fourierTransformReal;
		
		// Source �̹��� ���� // Set source image 
		fourierTransformReal.SetSourceImage(fliImage);

		// Destination �̹��� ����(RFT image) // Set destination image(RFT image) 
		fourierTransformReal.SetDestinationImage(fliRFTImage);

		// ��� �̹��� ���� ���� (RFT image, 32/64 bit Floating Point ���� ����) // Set Result image format(RFT image, 32/64 bit Floating Point) 
		fourierTransformReal.SetResultType(EFloatingPointAccuracy_Bit32);

		// �˰��� ����(RFT) // Execute the algorithm(RFT)
		if((res = fourierTransformReal.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Fourier Transform Real.");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(res = viewImageRFT.SetImagePtr(&fliRFTImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �̹����� ����� �����ϱ�(Mask ���) // Reduce the noise in the image (using Mask)
		// Mask ��ü ���� // Create Mask object
		CMask imageMask;

		// ��ȯ �̹����� ����(RFT) // Set source image(RFT image)
		imageMask.SetSourceImage(fliRFTImage);

		// CFLFigureArray ��ü�� ���� // Create CFLFigureArray object
		CFLFigureArray flaROIs;

		// �̸� �׷��� Mask region Figure Array �ҷ����� // Load Pre-drawn Mask Region Figure Array
		if((res = flaROIs.Load(L"../../ExampleImages/FourierTransform/RFTRegion.fig")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the figure file.");
			break;
		}

		// ������ ROI�� �Է� // Set mask ROI
		imageMask.SetSourceROI(&flaROIs);

		// ����ũ ���� �Է� // set mask value
		imageMask.SetMask(CMultiVar<double>(0.0));

		// �˰��� ����(mask) // Execute the algorithm(mask)		
		if((res = imageMask.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Image Mask.");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImageRFT.Invalidate(true);

		// Source �̹��� ����(RFT image) // Set source image (RFT image)
		fourierTransformReal.SetSourceImage(fliRFTImage);

		// Destination �̹��� ����(IRFT image) // Set destination image(IRFT image)
		fourierTransformReal.SetDestinationImage(fliIRFTImage);

		// �˰��� ����(IRFT) // Execute the algorithm(IRFT)
		if((res = fourierTransformReal.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Fourier Transform.");
			break;
		}

		// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
		// ���� ������ �ʿ� ���� // No need to release separately
		CGUIViewImageLayerWrap layer1 = viewImageOriginal.GetLayer(0);
		CGUIViewImageLayerWrap layer2 = viewImageRFT.GetLayer(0);
		CGUIViewImageLayerWrap layer3 = viewImageIRFT.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Delete the shapes drawn on the existing layer
		layer1.Clear();

		// Mask ROI ������ ��� // Display the mask ROI area
		if(IsFail(res = layer2.DrawFigureImage(&flaROIs, LIME)))
			ErrorPrint(res, "Failed to draw figure\n");

		// View ������ ���÷��� �մϴ�. // Display View information.
		if(IsFail(res = layer1.DrawTextCanvas(&CFLPointD(0, 0), L"Spatial Domain", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layer2.DrawTextCanvas(&CFLPointD(0, 0), L"Frequency Domain", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layer3.DrawTextCanvas(&CFLPointD(0, 0), L"Inverse RFT Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImageOriginal.ZoomFit();
		viewImageRFT.ZoomFit();
		viewImageIRFT.ZoomFit();
		viewImageOriginal.Invalidate(true);
		viewImageRFT.Invalidate(true);
		viewImageIRFT.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageOriginal.IsAvailable() && viewImageRFT.IsAvailable() && viewImageIRFT.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}