#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliISrcImage;
	CFLImage fliIDstImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage[2];

	do
	{
		// ���� ��� // operation result
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(eResult = fliISrcImage.Load(L"../../ExampleImages/CoherenceEnhancingDiffusion/fingerprint.flif")))
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

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(eResult = viewImage[0].SetImagePtr(&fliISrcImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].SetImagePtr(&fliIDstImage)))
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

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		// CoherenceEnhancingDiffusionNonNeg ��ü ���� // Create CoherenceEnhancingDiffusionNonNeg object
		CCoherenceEnhancingDiffusionNonNeg CoherenceEnhancingDiffusionNonNeg;

		// Source �̹��� ���� // Set source image 
		CoherenceEnhancingDiffusionNonNeg.SetSourceImage(fliISrcImage);

		// Destination �̹��� ���� // Set destination image
		CoherenceEnhancingDiffusionNonNeg.SetDestinationImage(fliIDstImage);

		// �̺� ��Ȱȭ(������) // Smoothing for derivative operator.(Gaussian)
		CoherenceEnhancingDiffusionNonNeg.SetSigma(0.5);

		// Ȯ�� ���(������) // Smoothing for diffusion coefficients.(Gaussian)
		CoherenceEnhancingDiffusionNonNeg.SetRho(4);

		// ������Ʈ ���� ���� // Set update rate
		CoherenceEnhancingDiffusionNonNeg.SetTheta(0.5);

		// �ݺ�Ƚ�� ���� // Number of iterations
		CoherenceEnhancingDiffusionNonNeg.SetIteration(15);

		// �˰��� ���� // Execute the algorithm
		if((eResult = CoherenceEnhancingDiffusionNonNeg.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Coherence Enhancing Diffusion NonNeg.");
			break;
		}

		// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
		// ���� ������ �ʿ� ���� // No need to release separately
		CGUIViewImageLayerWrap layerSource = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImage[1].GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Delete the shapes drawn on the existing layer
		layerSource.Clear();
		layerDestination.Clear();

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerDestination.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
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