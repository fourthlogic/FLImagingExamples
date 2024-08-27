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

	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = fliSourceImage.Load(L"../../ExampleImages/WeightedGuidedFilter/cat_450x480.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Operand �̹��� �ε� // Loads the operand image
		if((res = fliOperandImage.Load(L"../../ExampleImages/WeightedGuidedFilter/cat_450x480.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹��� �ε� // Load the destination image
		if((res = fliDestinationImage.Load(L"../../ExampleImages/WeightedGuidedFilter/cat_450x480.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((res = viewImageSource.Create(100, 0, 550, 480)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Operand �̹��� �� ���� // Creates operand image view
		if((res = viewImageOperand.Create(550, 0, 1000, 480)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if((res = viewImageDestination.Create(1000, 0, 1450, 480)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Source �̹��� ��� Operand �̹��� ���� ������ ����ȭ �Ѵ�
		if((res = viewImageSource.SynchronizePointOfView(&viewImageOperand)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� ��� Destination �̹��� ���� ������ ����ȭ �Ѵ�
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

		// Operand �̹��� �信 �̹����� ���÷���
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

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSource.SynchronizeWindow(&viewImageOperand)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSource.SynchronizeWindow(&viewImageDestination)).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		//�˰����� ���� ���
		CResult res = EResult_UnknownError;

		// WeightedGuidedFilter ��ü ���� // Create WeightedGuidedFilter object
		CWeightedGuidedFilter weightedGuidedFilter;

		// Source �̹��� ���� // Set the source image
		weightedGuidedFilter.SetSourceImage(fliSourceImage);

		// Operand �̹��� ���� // Set the operand image
		weightedGuidedFilter.SetOperandImage(fliOperandImage);

		// Destination �̹��� ���� // Set the destination image
		weightedGuidedFilter.SetDestinationImage(fliDestinationImage);

		// filter mask radius = 3 ����
		weightedGuidedFilter.SetRadius(3);

		// amplitude = 30 ����
		weightedGuidedFilter.SetAmplitude(30);

		// �ռ� ������ �Ķ���� ��� �˰����� ���� // Execute algorithm according to previously set parameters
		if((res = weightedGuidedFilter.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute weighted guided filter.\n");
			wprintf(res.GetString());
			ErrorPrint(res, "\n");
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
		if((res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = layerOperand.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Operand Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = layerDestination.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 30)).IsFail())
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