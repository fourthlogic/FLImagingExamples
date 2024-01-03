#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliISrcImage;
	CFLImage fliIFFTImage;
	CFLImage fliIFilterImage;
	CFLImage fliIMultiplyImage;
	CFLImage fliIDstImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage[5];

	do
	{
		// ���� ��� // operation result
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(eResult = fliISrcImage.Load(L"../../ExampleImages/FilterGeneratorFD/Sea1Ch.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImage[0].Create(300, 0, 300 + 384, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].Create(300 + 384, 0, 300 + 384 * 2, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[2].Create(300 + 384 * 2, 0, 300 + 384 * 3, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[3].Create(300, 0 + 384, 300 + 384, 384 + 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[4].Create(300 + 384 * 1, 0 + 384, 300 + 384 * 2, 384 + 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(eResult = viewImage[0].SetImagePtr(&fliISrcImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].SetImagePtr(&fliIFFTImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[2].SetImagePtr(&fliIFilterImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[3].SetImagePtr(&fliIMultiplyImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[4].SetImagePtr(&fliIDstImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views. 
		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[2])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[3])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[4])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[2])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[3])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[4])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		// Fourier Transform ��ü ���� // Create Fourier Transform object
		CFourierTransform fourierTransform;

		// Source �̹��� ���� // Set source image 
		fourierTransform.SetSourceImage(fliISrcImage);

		// Destination �̹��� ����(FFT image) // Set destination image(FFT image) 
		fourierTransform.SetDestinationImage(fliIFFTImage);

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

		// FilterGeneratorMeanFD ��ü ���� // Create FilterGeneratorMeanFD object
		CFilterGeneratorMeanFD filterGenerator;

		// Source �̹��� ���� // Set source image 
		filterGenerator.SetSourceImage(fliISrcImage);

		// Destination �̹��� ���� // Set destination image
		filterGenerator.SetDestinationImage(fliIFilterImage);

		// ���е� ���� // Set Accuracy
		filterGenerator.SetAccuracy(EFloatingPointAccuracy_Bit32);

		// ���� Ÿ�� ���� // set Filter type
		filterGenerator.SetType(CFilterGeneratorMeanFD::EFilterBaseFDType_FFT_Shift);

		// Mask Shape ���� // set Mask Shape
		filterGenerator.SetMaskShape(CFilterGeneratorMeanFD::EMaskShape_Rectangle);
		
		// Diameter1 ���� // set Diameter1
		filterGenerator.SetDiameter1(11);

		// Diameter2 ���� // set Diameter2
		filterGenerator.SetDiameter2(5);

		// Phi ���� // set Phi
		filterGenerator.SetPhi(0.785398f);

		// �˰��� ���� // Execute the algorithm
		if((eResult = filterGenerator.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute FilterGeneratorMeanFD.");
			break;
		}

		// Operation Multiply ��ü ���� // Create Operation Multiply object
		COperationMultiply multiply;
		// Source �̹��� ���� // Set the source image
		multiply.SetSourceImage(fliIFFTImage);
		// Operand �̹��� ���� // Set the operand image
		multiply.SetOperandImage(fliIFilterImage);
		// Destination �̹��� ���� // Set the destination image
		multiply.SetDestinationImage(fliIMultiplyImage);
		// ���� ��� ���� // Set operation source
		multiply.SetOperationSource(EOperationSource_Image);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = multiply.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute operation multiply.");
			break;
		}

		// Source �̹��� ����(FFT image) // Set source image (FFT image)
		fourierTransform.SetSourceImage(fliIMultiplyImage);

		// Destination �̹��� ����(IFFT image) // Set destination image(IFFT image)
		fourierTransform.SetDestinationImage(fliIDstImage);

		// �˰��� ����(IFFT) // Execute the algorithm(IFFT)
		if((eResult = fourierTransform.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Fourier Transform.");
			break;
		}

		// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
		// ���� ������ �ʿ� ���� // No need to release separately
		CGUIViewImageLayerWrap layerSource = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layerFFT = viewImage[1].GetLayer(0);
		CGUIViewImageLayerWrap layerFilter = viewImage[2].GetLayer(0);
		CGUIViewImageLayerWrap layerMultiply = viewImage[3].GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImage[4].GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Delete the shapes drawn on the existing layer
		layerSource.Clear();
		layerFFT.Clear();
		layerFilter.Clear();
		layerMultiply.Clear();
		layerDestination.Clear();

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerFFT.DrawTextCanvas(&CFLPointD(0, 0), L"FFT Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerFilter.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image(Filter)", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerMultiply.DrawTextCanvas(&CFLPointD(0, 0), L"Filtering FFT Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerDestination.DrawTextCanvas(&CFLPointD(0, 0), L"Filtering Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);
		viewImage[2].Invalidate(true);
		viewImage[3].Invalidate(true);
		viewImage[4].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable() && viewImage[2].IsAvailable() && viewImage[3].IsAvailable() && viewImage[4].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}