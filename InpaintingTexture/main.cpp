#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSrcImage;
	CFLImage fliSrcImage2;
	CFLImage fliDstImage;
	CFLImage fliDstImage2;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageSrc2;
	CGUIViewImageWrap viewImageDst;
	CGUIViewImageWrap viewImageDst2;

	// �˰��� ���� ��� // Algorithm execution result
	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = fliSrcImage.Load(L"../../ExampleImages/InpaintingTexture/Seville.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Source �̹��� 2 �ε� // Load the source image
		if((res = fliSrcImage2.Load(L"../../ExampleImages/InpaintingTexture/Wood.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Destination �̹����� Source �̹����� ������ �̹����� ���� // Create destination image as same as source image
		if((res = fliDstImage.Assign(fliSrcImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to assign the image file.\n");
			break;
		}

		// Destination �̹����� Source �̹����� ������ �̹����� ���� // Create destination image as same as source image
		if((res = fliDstImage2.Assign(fliSrcImage2)).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create the source image view
		if((res = viewImageSrc.Create(400, 0, 800, 400)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �� 2 ���� // Create the source image view 2
		if((res = viewImageSrc2.Create(400, 400, 800, 800)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((res = viewImageSrc.SetImagePtr(&fliSrcImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Source �̹��� �� 2�� �̹����� ���÷��� // Display the image in the source image view
		if((res = viewImageSrc2.SetImagePtr(&fliSrcImage2)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the destination image view
		if((res = viewImageDst.Create(800, 0, 1200, 400)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the destination image view
		if((res = viewImageDst2.Create(800, 400, 1200, 800)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((res = viewImageDst.SetImagePtr(&fliDstImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((res = viewImageDst2.SetImagePtr(&fliDstImage2)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImageSrc.SynchronizePointOfView(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImageSrc2.SynchronizePointOfView(&viewImageDst2)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc2.SynchronizeWindow(&viewImageDst2)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageSrc2)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// InpaintingTexture ��ü ���� // Create InpaintingTexture object
		CInpaintingTexture InpaintingTexture;

		// Source �̹��� ���� // Set the source image
		InpaintingTexture.SetSourceImage(fliSrcImage);
		// Destination �̹��� ���� // Set the destination image
		InpaintingTexture.SetDestinationImage(fliDstImage);
		// Patching�� ������ Mask�� ũ�� ���� // Set the size of the mask for patching
		InpaintingTexture.SetMaskSize(13);
		// Patching�� Source�� �Ǵ� Mask�� ã�� ���� �˻� ������ ũ�� ���� // Set the size of the search area to find the mask that is the source of the patching
		InpaintingTexture.SetSearchSize(100);
		// Search step size ���� // Set the search step size
		InpaintingTexture.SetSearchStepSize(1);
		// ��ġ�� ���� Gradient Value �� ��� ���� // Set a coefficient multiplied by gradient value for match
		InpaintingTexture.SetAnisotropy(1);

		CFLFigureArray flfaPaintingRegion;

		// �̸� �׷��� Painting region Figure Array �ҷ����� // Load Pre-drawn Painting Region Figure Array
		if((res = flfaPaintingRegion.Load(L"../../ExampleImages/InpaintingTexture/PaintingRegion.fig")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the figure file.");
			break;
		}

		// Inpainting�� ���� Painting region ���� // Set painting region for Inpainting
		InpaintingTexture.SetPaintingRegion(&flfaPaintingRegion);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = InpaintingTexture.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute InpaintingTexture.");
			break;
		}

		// Source �̹��� ���� // Set the source image
		InpaintingTexture.SetSourceImage(fliSrcImage2);
		// Destination �̹��� ���� // Set the destination image
		InpaintingTexture.SetDestinationImage(fliDstImage2);
		// Patching�� ������ Mask�� ũ�� ���� // Set the size of the mask for patching
		InpaintingTexture.SetMaskSize(15);
		// Patching�� Source�� �Ǵ� Mask�� ã�� ���� �˻� ������ ũ�� ���� // Set the size of the search area to find the mask that is the source of the patching
		InpaintingTexture.SetSearchSize(-1);
		// Search step size ���� // Set the search step size
		InpaintingTexture.SetSearchStepSize(1);
		// ��ġ�� ���� Gradient Value �� ��� ���� // Set a coefficient multiplied by gradient value for match
		InpaintingTexture.SetAnisotropy(0);

		CFLFigureArray flfaPaintingRegion2;

		// �̸� �׷��� Painting region Figure Array �ҷ����� // Load Pre-drawn Painting Region Figure Array
		if((res = flfaPaintingRegion2.Load(L"../../ExampleImages/InpaintingTexture/PaintingRegion2.fig")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the figure file.");
			break;
		}

		// Inpainting�� ���� Painting region ���� // Set painting region for Inpainting
		InpaintingTexture.SetPaintingRegion(&flfaPaintingRegion2);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = InpaintingTexture.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute InpaintingTexture.");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((res = viewImageDst2.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit of the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerSrc2 = viewImageSrc2.GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);
		CGUIViewImageLayerWrap layerDst2 = viewImageDst2.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerSrc2.Clear();
		layerDst.Clear();
		layerDst2.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = layerSrc.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = layerSrc2.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image 2", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = layerDst.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = layerDst2.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image 2", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		// Painting region�� source image�� ���÷��� // Display painting region on the source image
		if(fliSrcImage.PushBackFigure(CFigureUtilities::ConvertFigureObjectToString(flfaPaintingRegion)) == -1)
		{
			ErrorPrint(res, L"Failed to push figure on image\n");
			break;
		}

		// Painting region�� source image�� ���÷��� // Display painting region on the source image
		if(fliSrcImage2.PushBackFigure(CFigureUtilities::ConvertFigureObjectToString(flfaPaintingRegion2)) == -1)
		{
			ErrorPrint(res, L"Failed to push figure on image\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);
		viewImageSrc2.Invalidate(true);
		viewImageDst2.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable() && viewImageSrc2.IsAvailable() && viewImageDst2.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}