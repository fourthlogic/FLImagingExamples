#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliDestinationImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewIamgeDst;

	 // ���� ��� ��ü ���� // Declare the execution result object
	CResult eResult = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((eResult = fliSourceImage.Load(L"../../ExampleImages/OperationComplexDivide/ExampleSource.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹����� Source �̹����� �����ϰ� ���� // Assign the Source image to Destination image
		if((eResult = fliDestinationImage.Assign(fliSourceImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((eResult = viewImageSrc.Create(100, 0, 600, 500)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the Destination image view
		if((eResult = viewIamgeDst.Create(600, 0,1100, 500)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((eResult = viewImageSrc.SynchronizePointOfView(&viewIamgeDst)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((eResult = viewImageSrc.SetImagePtr(&fliSourceImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((eResult = viewIamgeDst.SetImagePtr(&fliDestinationImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((eResult = viewImageSrc.SynchronizeWindow(&viewIamgeDst)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Operation ComplexDivide ��ü ���� // Create Operation ComplexDivide object
		COperationComplexDivide cd;

		// Source �̹��� ���� // Set the source image
		cd.SetSourceImage(fliSourceImage);
		cd.SetDestinationImage(fliDestinationImage);

		// Scalar Operation �ҽ��� ���� // Set Operation Source to scalar
		cd.SetOperationSource(EOperationSource_Scalar);

		// �����÷� ó�� ��� ���� // Set the overflow handling method
		cd.SetOverflowMethod(EOverflowMethod_Clamping);

		// ��Į�� �� ���� // Set the Scalar value
		cd.SetScalarValue(CMultiVar<double>(5, 1));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = cd.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute operation complex divide.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewIamgeDst.GetLayer(0);

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
		viewImageSrc.Invalidate(true);
		viewIamgeDst.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewIamgeDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}