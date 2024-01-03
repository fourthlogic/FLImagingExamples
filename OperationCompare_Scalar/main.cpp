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
		CResult eResult = EResult_UnknownError;

		// Source �̹��� �ε� // Load the source image
		if(IsFail(eResult= fliSourceImage.Load(L"../../ExampleImages/OperationCompare/siamesecat3ch.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹��� �ε� // Load the destination image
		if(IsFail(eResult= fliDestinationImage.Load(L"../../ExampleImages/OperationCompare/siamesecat3ch.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(eResult= viewImageSource.Create(100, 0, 612, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if(IsFail(eResult= viewImageDestination.Create(612, 0,1124, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(eResult= viewImageSource.SynchronizePointOfView(&viewImageDestination)))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if(IsFail(eResult= viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if(IsFail(eResult= viewImageDestination.SetImagePtr(&fliDestinationImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult= viewImageSource.SynchronizeWindow(&viewImageDestination)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Operation Compare ��ü ���� // Create Operation Compare object
		COperationCompare compare;

		// Source �̹��� ���� // Set the source image
		compare.SetSourceImage(fliSourceImage);

		// Source �̹����� ROI ���� ����
		CFLCircleL flcSourceROI(284, 278, 206, 0, 0, 360, EArcClosingMethod_EachOther);

		// Source �̹����� ROI ����
		compare.SetSourceROI(flcSourceROI);

		// Source �̹����� Pivot ���� ����
		CFLPointL flpSourcePivot(258, 254);
		
		// Source �̹����� Pivot ����
		compare.SetSourcePivot(flpSourcePivot);

		// Destination �̹��� ���� // Set the destination image
		compare.SetDestinationImage(fliDestinationImage);

		// Destination �̹����� ROI ���� ����
		CFLCircleL flrDestinationROI(284, 278, 206, 0, 0, 360, EArcClosingMethod_EachOther);

		// Destination �̹����� ROI ����
		compare.SetDestinationROI(flrDestinationROI);

		// Destination �̹����� Pivot ���� ����
		CFLPointL flpDestinationPivot(258, 254);

		// Destination �̹����� Pivot ����
		compare.SetDestinationPivot(flpDestinationPivot);

		// Scalar Operation ���� ���� // Set operation mode to scalar
		compare.SetOperationSource(EOperationSource_Scalar);

		// ��Į�� ���� ����
		compare.SetScalarValue(CMultiVar<double>(100, 150, 200));

		// �׻� ���� ������ ������ ������ ä�쵵�� ����
		compare.EnableFillBlankColorMode(true);

		// ���� ���� ����
		compare.SetBlankColor(CMultiVar<double>(110, 50, 210));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult= eResult = compare.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute operation compare.\n");
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
		if(IsFail(eResult= layerSource.DrawFigureImage(&flcSourceROI, LIME)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		// Source Pivot ������ ������� �˱� ���� ���÷��� �Ѵ�
		CFLFigureArray flfaSourcePivotCrossHair = flpSourcePivot.MakeCrossHair(20, true);
		
		if(IsFail(eResult= layerSource.DrawFigureImage(&flfaSourcePivotCrossHair, BLACK, 3)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		if(IsFail(eResult= layerSource.DrawFigureImage(&flfaSourcePivotCrossHair, LIME)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		// Destination ROI ������ ������� �˱� ���� ���÷��� �Ѵ�
		if(IsFail(eResult= layerDestination.DrawFigureImage(&flrDestinationROI, LIME)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		// Destination Pivot ������ ������� �˱� ���� ���÷��� �Ѵ�
		CFLFigureArray flfaDestinationPivotCrossHair = flpDestinationPivot.MakeCrossHair(20, true);

		if(IsFail(eResult= layerDestination.DrawFigureImage(&flfaDestinationPivotCrossHair, BLACK, 3)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		if(IsFail(eResult= layerDestination.DrawFigureImage(&flfaDestinationPivotCrossHair, LIME)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		// �̹��� �� ���� ǥ�� // Display image view information
		if(IsFail(eResult= layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult= layerDestination.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
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