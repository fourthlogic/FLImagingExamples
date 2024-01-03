#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main() {
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliOperandImage1;
	CFLImage fliOperandImage2;
	CFLImage fliDestinationImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageOperand1;
	CGUIViewImageWrap viewImageOperand2;
	CGUIViewImageWrap viewImageDestination;

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult eResult = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((eResult = fliSourceImage.Load(L"../../ExampleImages/OperationLinearPolynomial/Space.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Operand1 �̹��� �ε� // Loads the operand image
		if((eResult = fliOperandImage1.Load(L"../../ExampleImages/OperationLinearPolynomial/circle.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		if((eResult = fliOperandImage2.Load(L"../../ExampleImages/OperationLinearPolynomial/Sky.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹��� �ε� // Loads the destination image
		if((eResult = fliDestinationImage.Create(250, 250)).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((eResult = viewImageSource.Create(100, 0, 400, 300)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Operand1 �̹��� �� ���� // Creates operand image view
		if((eResult = viewImageOperand1.Create(400, 0, 700, 300)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Operand2 �̹��� �� ���� // Creates operand image view
		if((eResult = viewImageOperand2.Create(700, 0, 1000, 300)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if((eResult = viewImageDestination.Create(1000, 0, 1300, 300)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Source �̹��� ��� Operand1 �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the source view and the operand view
		if((eResult = viewImageSource.SynchronizePointOfView(&viewImageOperand1)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� ��� Operand2 �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the source view and the operand view
		if((eResult = viewImageSource.SynchronizePointOfView(&viewImageOperand2)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� ��� Destination �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the source view and the destination view
		if((eResult = viewImageSource.SynchronizePointOfView(&viewImageDestination)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((eResult = viewImageSource.SetImagePtr(&fliSourceImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Operand1 �̹��� �信 �̹����� ���÷��� // Display the image in the operand image view
		if((eResult = viewImageOperand1.SetImagePtr(&fliOperandImage1)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Operand2 �̹��� �信 �̹����� ���÷��� // Display the image in the operand image view
		if((eResult = viewImageOperand2.SetImagePtr(&fliOperandImage2)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((eResult = viewImageDestination.SetImagePtr(&fliDestinationImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((eResult = viewImageSource.SynchronizeWindow(&viewImageOperand1)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((eResult = viewImageSource.SynchronizeWindow(&viewImageOperand2)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((eResult = viewImageSource.SynchronizeWindow(&viewImageDestination)).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Operation Linear Polynomial ��ü ���� // Create Operation Linear Polynomial object
		COperationLinear linear;

		// Source �̹��� ���� // Set the source image
		linear.SetSourceImage(fliSourceImage);

		// Operand �̹��� ���� // Set the operand image
		linear.SetOperandImage(fliOperandImage1);
		linear.SetOperandImage2(fliOperandImage2);


		linear.SetOperationOrder(EOperationOrder_CoefficientConstant);
		linear.SetOperationMode(EOperationMode_Overflow);

		// Destination �̹��� ���� // Set the destination image
		linear.SetDestinationImage(fliDestinationImage);

		// Image Operation �ҽ��� ���� // Set Operation Source to image
		linear.SetOperationSource(EOperationSource_Image);

		// ���� ���� ĥ�ϱ� ��� ���� // Set the Fill blank color mode false
		// ��� �̹����� �̹� ������ ��� ������� ���� ������ ���� �������� ĥ���� �ʰ� ���� �״�� �д�. // If the destination image already exists, the uncomputed area is left intact without being painted in a blank color.
		linear.EnableFillBlankColorMode(false);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = linear.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute operation Linear Polynomial.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerOperand1 = viewImageOperand1.GetLayer(0);
		CGUIViewImageLayerWrap layerOperand2 = viewImageOperand2.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImageDestination.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerOperand1.Clear();
		layerOperand2.Clear();
		layerDestination.Clear();

		// �̹��� �� ���� ǥ�� // Display image view information
		if((eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if((eResult = layerOperand1.DrawTextCanvas(&CFLPointD(0, 0), L"Operand1 Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if((eResult = layerOperand2.DrawTextCanvas(&CFLPointD(0, 0), L"Operand2 Image", YELLOW, BLACK, 30)).IsFail())
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
		viewImageSource.Invalidate(true);
		viewImageOperand1.Invalidate(true);
		viewImageOperand2.Invalidate(true);
		viewImageDestination.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSource.IsAvailable() && viewImageOperand1.IsAvailable() && viewImageOperand2.IsAvailable() && viewImageDestination.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}