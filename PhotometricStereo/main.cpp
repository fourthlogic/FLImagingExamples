#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	CFLImage fliSrcImage;
	CFLImage fliDstImage;
	CFLImage fliTxtImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;
	CGUIView3DWrap view3DDst;

	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Source �̹��� �ε� // Load the source image
		if((res = fliSrcImage.Load(L"../../ExampleImages/PhotometricStereo/Source.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliSrcImage.SelectPage(0);

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
		if((res = viewImageSrc.SynchronizeWindow(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two view windows
		if((res = viewImageSrc.SynchronizeWindow(&view3DDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		viewImageSrc.SetFixThumbnailView(true);

		// PhotometricStereo ��ü ���� // Create PhotometricStereo object
		CPhotometricStereo photometric;

		CFL3DObjectHeightMap fl3DOHM;

		// Source �̹��� ���� // Set the source image
		photometric.SetSourceImage(fliSrcImage);
		// Destination �̹��� ���� // Set the destination image
		photometric.SetDestinationImage(fliDstImage);
		// ��� Texture �̹��� ���� // Set the res texture image
		photometric.SetResultTextureImage(&fliTxtImage);
		// Destionation 3D Object ���� // Set the Destionation 3D Object 
		photometric.SetDestinationObject(fl3DOHM);

		// �� �̹����� ���� Slant �� �Է�
		CMultiVar<double> mvdSlant;

		mvdSlant.PushBack(39.831506);
		mvdSlant.PushBack(28.682381);
		mvdSlant.PushBack(20.989625);
		mvdSlant.PushBack(19.346638);
		mvdSlant.PushBack(20.785800);
		mvdSlant.PushBack(26.005273);
		mvdSlant.PushBack(19.038004);
		mvdSlant.PushBack(9.253585);
		mvdSlant.PushBack(16.425454);
		mvdSlant.PushBack(23.712574);
		mvdSlant.PushBack(26.003058);
		mvdSlant.PushBack(19.069500);
		mvdSlant.PushBack(11.801071);
		mvdSlant.PushBack(20.484473);
		mvdSlant.PushBack(25.909730);
		mvdSlant.PushBack(43.055332);
		mvdSlant.PushBack(39.043981);
		mvdSlant.PushBack(30.041029);
		mvdSlant.PushBack(26.067657);
		mvdSlant.PushBack(26.126303);

		// �� �̹����� ���� Tilt �� �Է�
		CMultiVar<double> mvdTilt;

		mvdTilt.PushBack(123.359091);
		mvdTilt.PushBack(123.952892);
		mvdTilt.PushBack(154.836215);
		mvdTilt.PushBack(-173.353324);
		mvdTilt.PushBack(-147.483507);
		mvdTilt.PushBack(109.497340);
		mvdTilt.PushBack(115.825606);
		mvdTilt.PushBack(-169.019112);
		mvdTilt.PushBack(-119.343654);
		mvdTilt.PushBack(-109.319167);
		mvdTilt.PushBack(66.944279);
		mvdTilt.PushBack(48.136896);
		mvdTilt.PushBack(-5.157068);
		mvdTilt.PushBack(-54.033519);
		mvdTilt.PushBack(-66.856636);
		mvdTilt.PushBack(60.456870);
		mvdTilt.PushBack(53.388008);
		mvdTilt.PushBack(36.447691);
		mvdTilt.PushBack(13.056294);
		mvdTilt.PushBack(-5.976723);

		photometric.SetLightAngleDegrees(mvdSlant, mvdTilt);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = photometric.Execute()).IsFail())
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
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);

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

		// Height Map�� Texture Image ����
		CFL3DObjectHeightMap* pFl3DOHM = dynamic_cast<CFL3DObjectHeightMap*>(photometric.GetDestinationObject());
		pFl3DOHM->SetTextureImage(fliTxtImage);
		pFl3DOHM->ActivateVertexColorTexture(true);

		// 3D �̹��� �信 Height Map (Destination Image) �̹����� ���÷��� // Display the Height Map (Destination Image) on the 3D image view
		if((res = view3DDst.PushObject(*pFl3DOHM)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		view3DDst.ZoomFit();

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);
		view3DDst.Invalidate(true);

		// �̹��� ��, 3D �䰡 ����� �� ���� ��ٸ� // Wait for the image and 3D view to close
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable() && view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}