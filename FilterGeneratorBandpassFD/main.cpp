#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliSrcImage;
	CFLImage fliFFTImage;
	CFLImage fliIdealFilter;
	CFLImage fliIdealDst;
	CFLImage fliButterworthFilter;
	CFLImage fliButterworthDst;
	CFLImage fliGaussianFilter;
	CFLImage fliGaussianDst;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage[8];

	do
	{
		// ���� ��� // operation result
		CResult res = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliSrcImage.Load(L"../../ExampleImages/FilterGeneratorFD/Sea1Ch.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage[0].Create(300, 0, 300 + 384, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(300 + 384, 0, 300 + 384 * 2, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[2].Create(300 + 384 * 2, 0, 300 + 384 * 3, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[3].Create(300 + 384 * 3, 0, 300 + 384 * 4, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[4].Create(300, 384, 300 + 384, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[5].Create(300 + 384, 384, 300 + 384 * 2, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[6].Create(300 + 384 * 2, 384, 300 + 384 * 3, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[7].Create(300 + 384 * 3, 384, 300 + 384 * 4, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(res = viewImage[0].SetImagePtr(&fliSrcImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].SetImagePtr(&fliIdealFilter)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[2].SetImagePtr(&fliButterworthFilter)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[3].SetImagePtr(&fliGaussianFilter)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[4].SetImagePtr(&fliFFTImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[5].SetImagePtr(&fliIdealDst)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[6].SetImagePtr(&fliButterworthDst)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[7].SetImagePtr(&fliGaussianDst)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views. 
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[2])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[3])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[4])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[5])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[6])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[7])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[2])))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[3])))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[4])))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[5])))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[6])))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[7])))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		// Fourier Transform ��ü ���� // Create Fourier Transform object
		CFourierTransform fourierTransform;

		// Source �̹��� ���� // Set source image 
		fourierTransform.SetSourceImage(fliSrcImage);

		// Destination �̹��� ����(FFT image) // Set destination image(FFT image) 
		fourierTransform.SetDestinationImage(fliFFTImage);

		// ��� �̹��� ���� ���� (FFT image, 32/64 bit Floating Point ���� ����) // Set Result image format(FFT image, 32/64 bit Floating Point) 
		fourierTransform.SetResultType(EFloatingPointAccuracy_Bit32);

		// Ǫ���� ��ȯ ��� �̹����� ����Ʈ�ؼ� �޵��� ���� // Set to receive a shifted image of the Fourier transform result
		fourierTransform.SetShiftSpectrum(EFourierTransformShiftSpectrum_Shift);

		// �˰��� ����(FFT) // Execute the algorithm(FFT)
		if((res = fourierTransform.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Fourier Transform.");
			break;
		}

		// FilterGeneratorBandpassFD ��ü ���� // Create FilterGeneratorBandpassFD object
		CFilterGeneratorBandpassFD filterGenerator;

		// Source �̹��� ���� // Set source image 
		filterGenerator.SetSourceImage(fliFFTImage);

		// Destination �̹��� ���� // Set destination image
		filterGenerator.SetDestinationImage(fliIdealFilter);

		// ���е� ���� // Set Accuracy
		filterGenerator.SetAccuracy(EFloatingPointAccuracy_Bit32);

		// MinFrequency ���� // set MinFrequency
		filterGenerator.SetMinFrequency(0.1);

		// MaxFrequency ���� // set MaxFrequency
		filterGenerator.SetMaxFrequency(0.6);

		// Filter Shape ���� // set Filter Shape
		filterGenerator.SetFilterShape(CFilterGeneratorBandpassFD::EFilterShape_Ideal);

		// �˰��� ���� // Execute the algorithm
		if((res = filterGenerator.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute FilterGeneratorBandpassFD.");
			break;
		}

		// Destination �̹��� ���� // Set destination image
		filterGenerator.SetDestinationImage(fliButterworthFilter);

		// Filter Shape ���� // set Filter Shape
		filterGenerator.SetFilterShape(CFilterGeneratorBandpassFD::EFilterShape_Butterworth);

		// Distance ���� // set Distance
		filterGenerator.SetDistance(256);

		// Degree ���� // set Degree
		filterGenerator.SetDegree(2);

		// �˰��� ���� // Execute the algorithm
		if((res = filterGenerator.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute FilterGeneratorBandpassFD.");
			break;
		}

		// Destination �̹��� ���� // Set destination image
		filterGenerator.SetDestinationImage(fliGaussianFilter);

		// Filter Shape ���� // set Filter Shape
		filterGenerator.SetFilterShape(CFilterGeneratorBandpassFD::EFilterShape_Gaussian);

		// Sigma1 ���� // set Sigma1
		filterGenerator.SetSigma1(1);

		// Sigma2 ���� // set Sigma2
		filterGenerator.SetSigma2(1);

		// Phi ���� // set Phi
		filterGenerator.SetPhi(0);

		// �˰��� ���� // Execute the algorithm
		if((res = filterGenerator.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute FilterGeneratorBandpassFD.");
			break;
		}

		// Operation Multiply ��ü ���� // Create Operation Multiply object
		COperationMultiply multiply;
		// Source �̹��� ���� // Set the source image
		multiply.SetSourceImage(fliFFTImage);
		// Operand �̹��� ���� // Set the operand image
		multiply.SetOperandImage(fliIdealFilter);
		// Destination �̹��� ���� // Set the destination image
		multiply.SetDestinationImage(fliIdealDst);
		// ���� ��� ���� // Set operation source
		multiply.SetOperationSource(EOperationSource_Image);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = multiply.Execute()))
		{
			ErrorPrint(res, "Failed to execute operation multiply.");
			break;
		}

		// Operand �̹��� ���� // Set the operand image
		multiply.SetOperandImage(fliButterworthFilter);
		// Destination �̹��� ���� // Set the destination image
		multiply.SetDestinationImage(fliButterworthDst);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = multiply.Execute()))
		{
			ErrorPrint(res, "Failed to execute operation multiply.");
			break;
		}

		// Operand �̹��� ���� // Set the operand image
		multiply.SetOperandImage(fliGaussianFilter);
		// Destination �̹��� ���� // Set the destination image
		multiply.SetDestinationImage(fliGaussianDst);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = multiply.Execute()))
		{
			ErrorPrint(res, "Failed to execute operation multiply.");
			break;
		}

		// Source �̹��� ����(FFT image) // Set source image (FFT image)
		fourierTransform.SetSourceImage(fliIdealDst);

		// Destination �̹��� ����(IFFT image) // Set destination image(IFFT image)
		fourierTransform.SetDestinationImage(fliIdealDst);

		// �˰��� ����(IFFT) // Execute the algorithm(IFFT)
		if((res = fourierTransform.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Fourier Transform.");
			break;
		}

		// Source �̹��� ����(FFT image) // Set source image (FFT image)
		fourierTransform.SetSourceImage(fliButterworthDst);

		// Destination �̹��� ����(IFFT image) // Set destination image(IFFT image)
		fourierTransform.SetDestinationImage(fliButterworthDst);

		// �˰��� ����(IFFT) // Execute the algorithm(IFFT)
		if((res = fourierTransform.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Fourier Transform.");
			break;
		}

		// Source �̹��� ����(FFT image) // Set source image (FFT image)
		fourierTransform.SetSourceImage(fliGaussianDst);

		// Destination �̹��� ����(IFFT image) // Set destination image(IFFT image)
		fourierTransform.SetDestinationImage(fliGaussianDst);

		// �˰��� ����(IFFT) // Execute the algorithm(IFFT)
		if((res = fourierTransform.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Fourier Transform.");
			break;
		}

		// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
		// ���� ������ �ʿ� ���� // No need to release separately
		CGUIViewImageLayerWrap layerSource = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layerFFT = viewImage[4].GetLayer(0);
		CGUIViewImageLayerWrap layerIdealFilter = viewImage[1].GetLayer(0);
		CGUIViewImageLayerWrap layerButterworthFilter = viewImage[2].GetLayer(0);
		CGUIViewImageLayerWrap layerGaussianFilter = viewImage[3].GetLayer(0);
		CGUIViewImageLayerWrap layerIdealDst = viewImage[5].GetLayer(0);
		CGUIViewImageLayerWrap layerButterworthDst = viewImage[6].GetLayer(0);
		CGUIViewImageLayerWrap layerGaussianDst = viewImage[7].GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Delete the shapes drawn on the existing layer
		layerSource.Clear();
		layerFFT.Clear();
		layerIdealFilter.Clear();
		layerButterworthFilter.Clear();
		layerGaussianFilter.Clear();
		layerIdealDst.Clear();
		layerButterworthDst.Clear();
		layerGaussianDst.Clear();

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerFFT.DrawTextCanvas(&CFLPointD(0, 0), L"FFT Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerIdealFilter.DrawTextCanvas(&CFLPointD(0, 0), L"Ideal, Min = 0.1, Max = 0.6", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerButterworthFilter.DrawTextCanvas(&CFLPointD(0, 0), L"Butterworth, Distance = 256, Degree = 2", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerGaussianFilter.DrawTextCanvas(&CFLPointD(0, 0), L"Gaussian, Sigma1 = Sigma2 = 1, Phi = 0", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerIdealDst.DrawTextCanvas(&CFLPointD(0, 0), L"Ideal Filtering Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerButterworthDst.DrawTextCanvas(&CFLPointD(0, 0), L"Butterworth Filtering Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerGaussianDst.DrawTextCanvas(&CFLPointD(0, 0), L"Gaussian Filtering Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);
		viewImage[2].Invalidate(true);
		viewImage[3].Invalidate(true);
		viewImage[4].Invalidate(true);
		viewImage[5].Invalidate(true);
		viewImage[6].Invalidate(true);
		viewImage[7].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable() && viewImage[2].IsAvailable() && viewImage[3].IsAvailable() && viewImage[4].IsAvailable() && viewImage[5].IsAvailable() && viewImage[6].IsAvailable() && viewImage[7].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}