#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliImage;
	CFLImage fliFFTImage;
	CFLImage fliIFFTImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageOriginal;
	CGUIViewImageWrap viewImageFFT;
	CGUIViewImageWrap viewImageIFFT;

	do
	{
		// ���� ��� // operation result
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/FourierTransform/TempleNoise.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImageOriginal.Create(300, 0, 300 + 512, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageFFT.Create(300 + 512, 0, 300 + 512 * 2, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageIFFT.Create(300 + 512 * 2, 0, 300 + 512 * 3, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(eResult = viewImageOriginal.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageIFFT.SetImagePtr(&fliIFFTImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImageOriginal.SynchronizeWindow(&viewImageFFT)))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(eResult = viewImageOriginal.SynchronizeWindow(&viewImageIFFT)))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		// Fourier Transform ��ü ���� // Create Fourier Transform object
		CFourierTransform fourierTransform;

		// Source �̹��� ���� // Set source image 
		fourierTransform.SetSourceImage(fliImage);

		// Destination �̹��� ����(FFT image) // Set destination image(FFT image) 
		fourierTransform.SetDestinationImage(fliFFTImage);

		// ��� �̹��� ���� ���� (FFT image, 32/64 bit Floating Point ���� ����) // Set Result image format(FFT image, 32/64 bit Floating Point) 
		fourierTransform.SetResultType(EFloatingPointAccuracy_Bit32);

		// Ǫ���� ��ȯ ��� �̹����� ����Ʈ�ؼ� �޵��� ���� // Set to receive a shifted image of the Fourier transform result
		fourierTransform.SetShiftSpectrum(EFourierTransformShiftSpectrum_Shift);

		// �˰��� ����(FFT) // Execute the algorithm(FFT)
		if((eResult = fourierTransform.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Fourier Transform.");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(eResult = viewImageFFT.SetImagePtr(&fliFFTImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �̹����� ����� �����ϱ�(Mask ���) // Reduce the noise in the image (using Mask)
		// Mask ��ü ���� // Create Mask object
		CMask imageMask;

		// ��ȯ �̹����� ����(FFT) // Set source image(FFT image)
		imageMask.SetSourceImage(fliFFTImage);

		// CFLFigureArray ��ü�� ���� // Create CFLFigureArray object
		CFLFigureArray flaROIs;

		// �̸� �׷��� Mask region Figure Array �ҷ����� // Load Pre-drawn Mask Region Figure Array
		if((eResult = flaROIs.Load(L"../../ExampleImages/FourierTransform/FFTRegion.fig")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the figure file.");
			break;
		}

		// ������ ROI�� �Է� // Set mask ROI
		imageMask.SetSourceROI(&flaROIs);

		// ����ũ ���� �Է� // set mask value
		imageMask.SetMask(CMultiVar<double>(0.0));

		// �˰��� ����(mask) // Execute the algorithm(mask)		
		if((eResult = imageMask.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Image Mask.");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImageFFT.Invalidate(true);

		// Source �̹��� ����(FFT image) // Set source image (FFT image)
		fourierTransform.SetSourceImage(fliFFTImage);

		// Destination �̹��� ����(IFFT image) // Set destination image(IFFT image)
		fourierTransform.SetDestinationImage(fliIFFTImage);

		// �˰��� ����(IFFT) // Execute the algorithm(IFFT)
		if((eResult = fourierTransform.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Fourier Transform.");
			break;
		}

		// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
		// ���� ������ �ʿ� ���� // No need to release separately
		CGUIViewImageLayerWrap layer1 = viewImageOriginal.GetLayer(0);
		CGUIViewImageLayerWrap layer2 = viewImageFFT.GetLayer(0);
		CGUIViewImageLayerWrap layer3 = viewImageIFFT.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Delete the shapes drawn on the existing layer
		layer1.Clear();

		// Mask ROI ������ ��� // Display the mask ROI area
		if(IsFail(eResult = layer2.DrawFigureImage(&flaROIs, LIME)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		// View ������ ���÷��� �մϴ�. // Display View information.
		if(IsFail(eResult = layer1.DrawTextCanvas(&CFLPointD(0, 0), L"Spatial Domain", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layer2.DrawTextCanvas(&CFLPointD(0, 0), L"Frequency Domain", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layer3.DrawTextCanvas(&CFLPointD(0, 0), L"Inverse FFT Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImageOriginal.ZoomFit();
		viewImageFFT.ZoomFit();
		viewImageIFFT.ZoomFit();
		viewImageOriginal.Invalidate(true);
		viewImageFFT.Invalidate(true);
		viewImageIFFT.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageOriginal.IsAvailable() && viewImageFFT.IsAvailable() && viewImageIFFT.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}