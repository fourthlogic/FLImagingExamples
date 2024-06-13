#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSrcImage;
	CFLImage fliAmpDstImage;
	CFLImage fliDirDstImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageAmpDst;
	CGUIViewImageWrap viewImageDirDst;

	// �˰��� ���� ��� // Algorithm execution result
	CResult result = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((result = fliSrcImage.Load(L"../../ExampleImages/RobinsonFilter/Gear.flif")).IsFail())
		{
			ErrorPrint(result, L"Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create the source image view
		if((result = viewImageSrc.Create(400, 0, 912, 612)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((result = viewImageSrc.SetImagePtr(&fliSrcImage)).IsFail())
		{
			ErrorPrint(result, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the destination image view
		if((result = viewImageAmpDst.Create(912, 0, 1424, 612)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((result = viewImageAmpDst.SetImagePtr(&fliAmpDstImage)).IsFail())
		{
			ErrorPrint(result, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the destination image view
		if((result = viewImageDirDst.Create(1424, 0, 1936, 612)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((result = viewImageDirDst.SetImagePtr(&fliDirDstImage)).IsFail())
		{
			ErrorPrint(result, L"Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((result = viewImageSrc.SynchronizePointOfView(&viewImageAmpDst)).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((result = viewImageSrc.SynchronizePointOfView(&viewImageDirDst)).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((result = viewImageSrc.SynchronizeWindow(&viewImageAmpDst)).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((result = viewImageSrc.SynchronizeWindow(&viewImageDirDst)).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// CRobinsonFilter ��ü ���� // Create CRobinsonFilter object
		CRobinsonFilter robinsonFilter;

		// Source �̹��� ���� // Set source image
		robinsonFilter.SetSourceImage(fliSrcImage);
		// Destination �̹��� ���� // Set destination image
		robinsonFilter.SetDestinationImage(fliAmpDstImage);
		// ���� ��� ���� // Set calculating mode
		robinsonFilter.SetRobinsonMode(CRobinsonFilter::ERobinsonMode_AMP);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((result = robinsonFilter.Execute()).IsFail())
		{
			ErrorPrint(result, L"Failed to execute RobinsonFilter.");
			break;
		}

		// Destination �̹��� ���� // Set destination image
		robinsonFilter.SetDestinationImage(fliDirDstImage);
		// ���� ��� ���� // Set calculating mode
		robinsonFilter.SetRobinsonMode(CRobinsonFilter::ERobinsonMode_DIR);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((result = robinsonFilter.Execute()).IsFail())
		{
			ErrorPrint(result, L"Failed to execute RobinsonFilter.");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((result = viewImageAmpDst.ZoomFit()).IsFail())
		{
			ErrorPrint(result, L"Failed to zoom fit of the image view.\n");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((result = viewImageDirDst.ZoomFit()).IsFail())
		{
			ErrorPrint(result, L"Failed to zoom fit of the image view.\n");
			break;
		}

		if((result = viewImageDirDst.SetFloatingImageValueRange(-180.f, 180.f)).IsFail())
		{
			ErrorPrint(result, L"Failed to set value range of the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerAmpDst = viewImageAmpDst.GetLayer(0);
		CGUIViewImageLayerWrap layerDirDst = viewImageDirDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerAmpDst.Clear();
		layerDirDst.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((result = layerSrc.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = layerAmpDst.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image(AMP)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = layerDirDst.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image(DIR)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageAmpDst.Invalidate(true);
		viewImageDirDst.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to exit
		while(viewImageSrc.IsAvailable() && viewImageAmpDst.IsAvailable() && viewImageDirDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}