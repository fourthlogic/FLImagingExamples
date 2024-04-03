#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliOperandImage;
	CFLImage fliDestinationImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageOperand;
	CGUIViewImageWrap viewImageDestination;

	 // ���� ��� ��ü ���� // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = fliSourceImage.Load(L"../../ExampleImages/OperationComplexMultiply/ExampleSource.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Operand �̹��� �ε� // Loads the operand image
		if((res = fliOperandImage.Load(L"../../ExampleImages/OperationComplexMultiply/ExampleOperand.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹����� Source �̹����� �����ϰ� ���� // Assign the Source image to Destination image
		if((res = fliDestinationImage.Assign(fliSourceImage)).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((res = viewImageSource.Create(100, 0, 600, 545)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Operand �̹��� �� ���� // Creates operand image view
		if((res = viewImageOperand.Create(600, 0, 1100, 545)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if((res = viewImageDestination.Create(1100, 0,1600, 545)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Source �̹��� ��� Operand �̹��� ���� ������ ����ȭ�Ѵ� // Synchronize the viewpoints of the source view and the operand view
		if((res = viewImageSource.SynchronizePointOfView(&viewImageOperand)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� ��� Destination �̹��� ���� ������ ����ȭ�Ѵ� // Synchronize the viewpoints of the source view and the destination view
		if((res = viewImageSource.SynchronizePointOfView(&viewImageDestination)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((res = viewImageSource.SetImagePtr(&fliSourceImage)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Operand �̹��� �信 �̹����� ���÷��� // Display the image in the operand image view
		if((res = viewImageOperand.SetImagePtr(&fliOperandImage)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((res = viewImageDestination.SetImagePtr(&fliDestinationImage)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ�Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSource.SynchronizeWindow(&viewImageOperand)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ�Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSource.SynchronizeWindow(&viewImageDestination)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// OperationComplexMultiply ��ü ���� // Create OperationComplexMultiply object
		COperationComplexMultiply cd;

		// Source �̹��� ���� // Set the source image
		cd.SetSourceImage(fliSourceImage);

		// Operand �̹��� ���� // Set the operand image
		cd.SetOperandImage(fliOperandImage);
		
		// Destination �̹��� ���� // Set the destination image
		cd.SetDestinationImage(fliDestinationImage);

		// �����÷� ó�� ��� ���� // Set the overflow handling method
		cd.SetOverflowMethod(EOverflowMethod_Wrapping);

		// ���� ��� �̹����� ���� // Set operation source to image
		cd.SetOperationSource(EOperationSource_Image);

		// ���� ���� ĥ�ϱ� ��� ���� // Set the Fill blank color mode false
		// ��� �̹����� �̹� ������ ��� ������� ���� ������ ���� �������� ĥ���� �ʰ� ���� �״�� �д�. // If the destination image already exists, the uncomputed area is left intact without being painted in a blank color.
		cd.EnableFillBlankColorMode(false);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = cd.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute operation complex multiply.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerOperand = viewImageOperand.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImageDestination.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerOperand.Clear();
		layerDestination.Clear();

		// �̹��� �� ���� ǥ�� // Display image view information
		CFLPoint<double> flpPoint = CFLPoint<double>(0, 0);

		if((res = layerSource.DrawTextCanvas(&flpPoint, L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = layerOperand.DrawTextCanvas(&flpPoint, L"Operand Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = layerDestination.DrawTextCanvas(&flpPoint, L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		viewImageSource.Invalidate(true);
		viewImageOperand.Invalidate(true);
		viewImageDestination.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSource.IsAvailable() && viewImageOperand.IsAvailable() && viewImageDestination.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}