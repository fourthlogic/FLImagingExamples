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
		if((res = fliSrcImage.Load(L"../../ExampleImages/MultiFocusMAPBased3D/")).IsFail())
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
		if((res = viewImageDst.Create(548, 0, 996, 448)).IsFail())
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

		// Multi Focus MAP Based ��ü ���� // Create Multi Focus MAP Based object
		CMultiFocusMAPBased3D algMultiFocusMAPBased3D;

		CFL3DObjectHeightMap fl3DOHM;


		// Source �̹��� ���� // Set the source image
		if((res = algMultiFocusMAPBased3D.SetSourceImage(fliSrcImage)).IsFail()) break;
		// ��� destination height map �̹��� ���� // Set the destination height map image
		if((res = algMultiFocusMAPBased3D.SetDestinationHeightMapImage(fliDstImage)).IsFail()) break;
		// ��� destination texture �̹��� ���� // Set the destination texture image
		if((res = algMultiFocusMAPBased3D.SetDestinationTextureImage(&fliTxtImage)).IsFail()) break;

		// Focus measure bias page index ���� // Set the focus measure bias page index
		if((res = algMultiFocusMAPBased3D.SetFMBiasPageIndex(3)).IsFail()) break;
		// Focus measure bias value ���� // Set the Focus measure bias value
		if((res = algMultiFocusMAPBased3D.SetFMBiasValue(0.02)).IsFail()) break;
		// Focus measure method ���� // Set focus measure method
		if((res = algMultiFocusMAPBased3D.SetFocusMeasureMethod(CMultiFocusMAPBased3D::EFocusMeasureMethod_DoG)).IsFail()) break;
		// Sigma1 ���� // Set the sigma1
		if((res = algMultiFocusMAPBased3D.SetSigma1(0.4)).IsFail()) break;
		// Sigma2 ���� // Set the sigma2
		if((res = algMultiFocusMAPBased3D.SetSigma2(0.8)).IsFail()) break;

		// Local regularization factor ���� // Set the local regularization factor
		if((res = algMultiFocusMAPBased3D.SetLocalRegularizationFactor(0.02)).IsFail()) break;
		// Global regularization factor ���� // Set the global regularization factor
		if((res = algMultiFocusMAPBased3D.SetGlobalRegularizationFactor(0.00000000001)).IsFail()) break;
		// Conjugate Gradient Method �� tolerance ���� // Set the tolerance for Conjugate Gradient Method
		if((res = algMultiFocusMAPBased3D.SetCGMTolerance(0.00001)).IsFail()) break;
		// Conjugate Gradient Method �� max iterations ���� // Set the max iterations for Conjugate Gradient Method
		if((res = algMultiFocusMAPBased3D.SetCGMMaxIterations(100)).IsFail()) break;

		// Page Direction ���� // Set the page direction
		if((res = algMultiFocusMAPBased3D.SetDirection(CMultiFocusMAPBased3D::EDirection_BottomToTop)).IsFail()) break;
		// Pixel Accuracy ���� // Set the pixel accuracy
		if((res = algMultiFocusMAPBased3D.SetPixelAccuracy(1.0)).IsFail()) break;
		// Depth Pitch ���� // Set the depth pitch
		if((res = algMultiFocusMAPBased3D.SetDepthPitch(2.0)).IsFail()) break;

		// Destionation 3D object ���� Ȱ��ȭ // Enable the destionation 3D object generation
		if((res = algMultiFocusMAPBased3D.Enable3DObjectGeneration(true)).IsFail()) break;
		// Destionation 3D object ���� // Set the destionation 3D object 
		if((res = algMultiFocusMAPBased3D.SetDestinationObject(fl3DOHM)).IsFail()) break;


		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = algMultiFocusMAPBased3D.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute multiFocus.");
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

		CFL3DObjectHeightMap* pFl3DOHM = dynamic_cast<CFL3DObjectHeightMap*>(algMultiFocusMAPBased3D.GetDestinationObject());
		pFl3DOHM->SetTextureImage(fliTxtImage);
		pFl3DOHM->ActivateVertexColorTexture(true);

		// 3D �̹��� �信 Height Map (Dst Image) �̹����� ���÷���
		if((res = view3DDst.PushObject(*pFl3DOHM)).IsFail())
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