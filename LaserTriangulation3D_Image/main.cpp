#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	CFLImage fliSrcImage;
	CFLImage fliDstImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;
	CGUIView3DWrap view3DDst;

	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Source �̹��� �ε� // Load the source image
		if((res = fliSrcImage.Load(L"../../ExampleImages/LaserTriangulation3D/SrcProfile.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliSrcImage.SelectPage(0);

		// Source �̹��� �� ���� // Create the source image view
		if((res = viewImageSrc.Create(100, 0, 600, 448)).IsFail())
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

		// Destination �̹��� �� ���� // Create the destination image view
		if((res = viewImageDst.Create(600, 0, 1100, 448)).IsFail())
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
		if((res = view3DDst.Create(400, 200, 1300, 800)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// �� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		viewImageSrc.SetFixThumbnailView(true);

		// Multi Focus ��ü ���� // Create Multi Focus object
		CLaserTriangulation3D LaserTriangulation;

		CFL3DObjectHeightMap fl3DOHM;

		// Source �̹��� ���� // Set the source image
		LaserTriangulation.SetSourceImage(fliSrcImage);
		// Destination Height Map �̹��� ���� // Set the destination height map image
		LaserTriangulation.SetDestinationHeightMapImage(fliDstImage);
		// Destionation 3D Object ���� // Set the Destionation 3D Object 
		LaserTriangulation.SetDestinationObject(fl3DOHM);
		// Source �̹��� Ÿ�� ���� // Set the type of the source image
		LaserTriangulation.SetSourceType(CLaserTriangulation3D::ESourceType_Profile);
		// Pixel Accuracy ���� // Set the pixel accuracy
		LaserTriangulation.SetPixelAccuracy(0.33);
		// Scan Accuracy ���� // Set the scan accuracy
		LaserTriangulation.SetScanAccuracy(0.2);
		// Working Distance ���� // Set the working distance
		LaserTriangulation.SetWorkingDistance(214.7);
		// ������ ���� ���� // Set the angle of laser
		LaserTriangulation.SetAngleOfLaser(60);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = LaserTriangulation.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute Laser Triangulation.");
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
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);
		CGUIView3DLayerWrap layer3D = view3DDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerDst.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = layerSrc.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		if((res = layerDst.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		// 3D �̹��� �信 Height Map (Dst Image) �̹����� ���÷���
		if((res = view3DDst.PushObject(*LaserTriangulation.GetDestinationObject())).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		view3DDst.ZoomFit();

		if((res = layer3D.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);
		view3DDst.Invalidate(true);

		// �̹��� ��, 3D �䰡 ����� �� ���� ��ٸ�
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable() && view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}