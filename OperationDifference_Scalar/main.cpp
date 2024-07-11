#include <cstdio>
#include "../CommomHeader/ErrorPrint.h"

#include <FLImaging.h>


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSourceImage;
	CFLImage fliDestinationImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSource;
	CGUIViewImageWrap viewImageDestination;

	do
	{
        // �˰��� ���� ��� // Algorithmic executation result
		CResult res = EResult_UnknownError;

		// Source �̹��� �ε� // Load the source image
		if(IsFail(res= fliSourceImage.Load(L"../../ExampleImages/OperationDifference/siamesecat3ch.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹��� �ε� // Load the destination image
		if(IsFail(res= fliDestinationImage.Load(L"../../ExampleImages/OperationDifference/siamesecat3ch.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(res= viewImageSource.Create(100, 0, 612, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if(IsFail(res= viewImageDestination.Create(612, 0,1124, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(res= viewImageSource.SynchronizePointOfView(&viewImageDestination)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if(IsFail(res= viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if(IsFail(res= viewImageDestination.SetImagePtr(&fliDestinationImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res= viewImageSource.SynchronizeWindow(&viewImageDestination)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Operation Difference ��ü ���� // Create Operation Difference object
		COperationDifference difference;

		// Source �̹��� ���� // Set the source image
		difference.SetSourceImage(fliSourceImage);

		// Source �̹����� ROI ���� ����
		CFLCircle<int32_t> flcSourceROI(284, 278, 206, 0, 0, 360, EArcClosingMethod_EachOther);

		// Source �̹����� ROI ����
		difference.SetSourceROI(flcSourceROI);

		// Source �̹����� Pivot ���� ����
		CFLPoint<int32_t> flpSourcePivot(258, 254);
		
		// Source �̹����� Pivot ����
		difference.SetSourcePivot(flpSourcePivot);

		// Destination �̹��� ���� // Set the destination image
		difference.SetDestinationImage(fliDestinationImage);

		// Destination �̹����� ROI ���� ����
		CFLCircle<int32_t> flrDestinationROI(284, 278, 206, 0, 0, 360, EArcClosingMethod_EachOther);

		// Destination �̹����� ROI ����
		difference.SetDestinationROI(flrDestinationROI);

		// Destination �̹����� Pivot ���� ����
		CFLPoint<int32_t> flpDestinationPivot(258, 254);

		// Destination �̹����� Pivot ����
		difference.SetDestinationPivot(flpDestinationPivot);

		// Scalar Operation ���� ���� // Set operation mode to scalar
		difference.SetOperationSource(EOperationSource_Scalar);

		// ��Į�� ���� ����
		difference.SetScalarValue(CMultiVar<double>(100, 150, 200));

		// ���� ������ ������ ������ ä���� �ʵ��� ����
		difference.EnableFillBlankColorMode(false);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res= res = difference.Execute()))
		{
			ErrorPrint(res, "Failed to execute operation difference.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImageDestination.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSource.Clear();
		layerDestination.Clear();

		// FLImaging�� Figure��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ����
		// Source ROI ������ ������� �˱� ���� ���÷��� �Ѵ�
		if(IsFail(res= layerSource.DrawFigureImage(&flcSourceROI, LIME)))
			ErrorPrint(res, "Failed to draw figure\n");

		// Source Pivot ������ ������� �˱� ���� ���÷��� �Ѵ�
		CFLFigureArray flfaSourcePivotCrossHair = flpSourcePivot.MakeCrossHair(20, true);
		
		if(IsFail(res= layerSource.DrawFigureImage(&flfaSourcePivotCrossHair, BLACK, 3)))
			ErrorPrint(res, "Failed to draw figure\n");

		if(IsFail(res= layerSource.DrawFigureImage(&flfaSourcePivotCrossHair, LIME)))
			ErrorPrint(res, "Failed to draw figure\n");

		// Destination ROI ������ ������� �˱� ���� ���÷��� �Ѵ�
		if(IsFail(res= layerDestination.DrawFigureImage(&flrDestinationROI, LIME)))
			ErrorPrint(res, "Failed to draw figure\n");

		// Destination Pivot ������ ������� �˱� ���� ���÷��� �Ѵ�
		CFLFigureArray flfaDestinationPivotCrossHair = flpDestinationPivot.MakeCrossHair(20, true);

		if(IsFail(res= layerDestination.DrawFigureImage(&flfaDestinationPivotCrossHair, BLACK, 3)))
			ErrorPrint(res, "Failed to draw figure\n");

		if(IsFail(res= layerDestination.DrawFigureImage(&flfaDestinationPivotCrossHair, LIME)))
			ErrorPrint(res, "Failed to draw figure\n");

		// �̹��� �� ���� ǥ�� // Display image view information
		if(IsFail(res= layerSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res= layerDestination.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		viewImageSource.Invalidate(true);
		viewImageDestination.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSource.IsAvailable() && viewImageDestination.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}