#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	CFLImage fliSrcImage;
	CFLImage fliSrcImage2;
	CFLImage fliDstImage;
	CFLImage fliTxtImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageSrc2;
	CGUIViewImageWrap viewImageDst;
	CGUIView3DWrap view3DDst;

	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Source �̹��� �ε� // Load the source image
		if((res = fliSrcImage.Load(L"../../ExampleImages/StereoDisparity/Left.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Source �̹��� 2 �ε� // Load the source image 2
		if((res = fliSrcImage2.Load(L"../../ExampleImages/StereoDisparity/Right.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliSrcImage.SelectPage(0);
		fliSrcImage2.SelectPage(0);

		// Source �̹��� �� ���� // Create the source image view
		if((res = viewImageSrc.Create(100, 0, 548, 448)).IsFail())
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

		// Source �̹��� 2 �� ���� // Create the source image view 2
		if((res = viewImageSrc2.Create(548, 0, 996, 448)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� 2 �信 �̹����� ���÷��� // Display the image in the source image view 2
		if((res = viewImageSrc2.SetImagePtr(&fliSrcImage2)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the destination image view
		if((res = viewImageDst.Create(100, 448, 548, 896)).IsFail())
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

		// Destination 3D �̹��� �� ���� // Create the destination 3D image view
		if((res = view3DDst.Create(548, 448, 996, 896)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageSrc2)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two view windows
		if((res = viewImageSrc.SynchronizeWindow(&view3DDst)).IsFail())
		{
			ErrorPrint(res,L"Failed to synchronize window.\n");
			break;
		}

		viewImageSrc.SetFixThumbnailView(true);
		viewImageSrc2.SetFixThumbnailView(true);

		// StereoDisparity ��ü ���� // Create StereoDisparity object
		CStereoDisparity disparity;

		// Source �̹��� ���� // Set the source image
		disparity.SetSourceImage(fliSrcImage);
		// Source �̹��� 2 ����
		disparity.SetSourceImage2(fliSrcImage2);
		// Destination �̹��� ���� // Set the destination image
		disparity.SetDestinationImage(fliDstImage);
		// ��� Texture �̹��� ���� // Set the res texture image
		disparity.SetResultTextureImage(&fliTxtImage);
		// �ּ� ��� Disparity �� ���� // Set the minimum allowed disparity value
		disparity.SetMinimumDisparity(0);
		// Disparity ���� ���� // Set the range of disparity
		disparity.SetNumberOfDisparities(15);
		// Matched Block ũ�� ���� // Set the matched block size
		disparity.SetMatchBlockSize(7);
		// �¿� �� �ִ� ��� ���� �� ���� // Set maximum allowed difference value between left and right
		disparity.SetMaximumDifference(10);
		// ������ �� ���� // Set the uniqueness ratio value
		disparity.SetUniquenessRatio(1);
		// P1 �� ���� // Set P1 Value
		disparity.SetP1(1000);
		// P2 �� ���� // Set P2 Value
		disparity.SetP2(4000);
		// Median Morphology Ŀ�� ������ ���� // Set the median morphology kernel size
		disparity.SetMedianKernelSize(3);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = disparity.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute algorithm.\n");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((res = viewImageDst.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit of the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerSrc2 = viewImageSrc2.GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerSrc2.Clear();
		layerDst.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = layerSrc.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		if((res = layerSrc2.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image 2", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		if((res = layerDst.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		// 3D �̹��� �信 Height Map (Destination Image) �̹����� ���÷��� // Display the Height Map (Destination Image) on the 3D image view
		if((res = view3DDst.PushObject(CFL3DObjectHeightMap(&fliDstImage, &fliTxtImage))).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageSrc2.Invalidate(true);
		viewImageDst.Invalidate(true);
		view3DDst.Invalidate(true);

		// �̹��� ��, 3D �䰡 ����� �� ���� ��ٸ� // Wait for the image and 3D view to close
		while(viewImageSrc.IsAvailable() && viewImageSrc2.IsAvailable() && viewImageDst.IsAvailable() && view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}