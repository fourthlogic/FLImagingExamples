#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliSrcImage;
	CFLImage fliDstImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res =fliSrcImage.Load(L"../../ExampleImages/FreiFilter/Gear.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create the source image view
		if((res =viewImageSrc.Create(400, 0, 912, 612)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((res =viewImageSrc.SetImagePtr(&fliSrcImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the destination image
		if((res =viewImageDst.Create(912, 0, 1424, 612)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((res =viewImageDst.SetImagePtr(&fliDstImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res =viewImageSrc.SynchronizePointOfView(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res =viewImageSrc.SynchronizeWindow(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// FreiFilter ��ü ���� // Create FreiFilter object
		CFreiFilter FreiFilter;

		// Source �̹��� ���� // Set the source Image
		FreiFilter.SetSourceImage(fliSrcImage);
		// Destination �̹��� ���� // Set the destination Image
		FreiFilter.SetDestinationImage(fliDstImage);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute the algorithm according to the previously set parameters
		if((res =FreiFilter.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute FreiFilter.");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // As the Dst image is newly created, the scale of the displayed image is adjusted to the screen through Zoom fit
		if((res =viewImageDst.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit of the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Get layer 0 from Image View to display on the screen
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // Since this object belongs to the ImageView, it does not need to be freed.
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Delete the shapes drawn on the existing layer
		layerSrc.Clear();
		layerDst.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas draws a String based on screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color. // If the color parameter is set as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by treating it as a background color
		
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� -> ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic

		if((res =layerSrc.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res =layerDst.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image views
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the imageview to exit.
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}