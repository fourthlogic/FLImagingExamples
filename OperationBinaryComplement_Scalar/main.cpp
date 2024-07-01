#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliSourceDestinationImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	 // ���� ��� ��ü ���� // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = fliSourceImage.Load(L"../../ExampleImages/OperationBinaryComplement/gradation.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹����� Source �̹����� �����ϰ� ���� // Assign the Source image to Destination image
		if((res = fliSourceDestinationImage.Assign(fliSourceImage)).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((res = viewImageSrc.Create(100, 0, 600, 545)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the Destination image view
		if((res = viewImageDst.Create(600, 0,1100, 545)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImageSrc.SynchronizePointOfView(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((res = viewImageSrc.SetImagePtr(&fliSourceImage)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((res = viewImageDst.SetImagePtr(&fliSourceDestinationImage)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Operation BitwiseAnd ��ü ���� // Create Operation BitwiseAnd object
		COperationBinaryComplement bc;

		// Source �̹��� ���� // Set the source image
		bc.SetSourceImage(fliSourceDestinationImage);

		// Source �̹����� ROI ���� ���� // Set the Source ROI value
		CFLCircle<int32_t> flcSourceROI(128, 128, 80, 0, 0, 360, EArcClosingMethod_EachOther);

		// Source �̹����� ROI ���� // Set the Source ROI
		bc.SetSourceROI(flcSourceROI);

		// Scalar Operation �ҽ��� ���� // Set Operation Source to scalar
		bc.SetOperationSource(EOperationSource_Scalar);

		// ��Į�� �� ���� // Set the Scalar value
		bc.SetScalarValue(CMultiVar<double>(2));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = bc.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute operation binary complement.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImageDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerDestination.Clear();

		// FLImaging�� Figure��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ����
		// Source ROI ������ ������� �˱� ���� ���÷��� �Ѵ�
		if((res = layerSource.DrawFigureImage(&flcSourceROI, LIME)).IsFail())
			ErrorPrint(res, "Failed to draw figure\n");
		
		if((res = layerDestination.DrawFigureImage(&flcSourceROI, LIME)).IsFail())
			ErrorPrint(res, "Failed to draw figure\n");

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = layerDestination.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source & Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}