#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	CFLArray<CFLImage*> flaFliLrnImage;
	CFLArray<CFLImage*> flaFliSrcImage;
	CFLImage fliLearnImage[2];
	CFLImage fliSourceImage[2];
	CFLImage fliDstImage;
	CFLImage fliTxtImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageLrn[2];
	CGUIViewImageWrap viewImageSrc[2];
	CGUIViewImageWrap viewImageDst;
	CGUIView3DWrap view3DDst;

	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Learn �̹��� �ε� // Load the reference plane image for calibration
		if((res = fliLearnImage[0].Load(L"../../ExampleImages/Moire3D/Learn0/")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliLearnImage[0].SelectPage(0);

		// Source �̹��� �ε� // Load the source image
		if((res = fliSourceImage[0].Load(L"../../ExampleImages/Moire3D/Object0/")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliSourceImage[0].SelectPage(0);

		// Learn �̹��� �ε� // Load the reference plane image for calibration
		if((res = fliLearnImage[1].Load(L"../../ExampleImages/Moire3D/Learn1/")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliLearnImage[1].SelectPage(0);

		// Source �̹��� �ε� // Load the source image
		if((res = fliSourceImage[1].Load(L"../../ExampleImages/Moire3D/Object1/")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliSourceImage[1].SelectPage(0);

		// CFLArray<CFLImage*> ��ü�� CFImage* �߰� // Push back CFImage* to CFLArray<CFLImage*> 
		flaFliLrnImage.PushBack(&fliLearnImage[0]);
		flaFliLrnImage.PushBack(&fliLearnImage[1]);
		flaFliSrcImage.PushBack(&fliSourceImage[0]);
		flaFliSrcImage.PushBack(&fliSourceImage[1]);

		// Learn �̹��� �� ���� // Create the learn image view
		if((res = viewImageLrn[0].Create(100, 0, 548, 348)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Learn �̹��� �� ���� // Create the destination image view
		if((res = viewImageLrn[1].Create(548, 0, 996, 348)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �� ���� // Create the Source image view
		if((res = viewImageSrc[0].Create(100, 348, 548, 696)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �� ���� // Create the destination image view
		if((res = viewImageSrc[1].Create(548, 348, 996, 696)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Dst �̹��� �� ���� // Create the destination image view
		if((res = viewImageDst.Create(996, 348, 1444, 696)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Learn �̹��� �信 �̹����� ���÷��� // Display the image in the Learn image view
		for(int32_t i32I = 0; i32I < 2; ++i32I)
		{
			if((res = viewImageLrn[i32I].SetImagePtr(flaFliLrnImage[i32I])).IsFail())
			{
				ErrorPrint(res, L"Failed to set image object on the image view.\n");
				break;
			}
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the Source image view
		for(int32_t i32I = 0; i32I < 2; ++i32I)
		{
			if((res = viewImageSrc[i32I].SetImagePtr(flaFliSrcImage[i32I])).IsFail())
			{
				ErrorPrint(res, L"Failed to set image object on the image view.\n");
				break;
			}
		}

		// Dst �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((res = viewImageDst.SetImagePtr(&fliDstImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// �� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two view windows
		if((res = viewImageLrn[1].SynchronizeWindow(&viewImageLrn[0])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two view windows
		for(int32_t i32I = 0; i32I < 2; ++i32I)
		{
			if((res = viewImageSrc[i32I].SynchronizeWindow(&viewImageLrn[0])).IsFail())
			{
				ErrorPrint(res, L"Failed to synchronize window.\n");
				break;
			}
		}

		// �� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two view windows
		if((res = viewImageDst.SynchronizeWindow(&viewImageLrn[0])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ�. // Synchronize the viewpoints of the two image views. 
		if((res = viewImageLrn[1].SynchronizePointOfView(&viewImageLrn[0])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ�. // Synchronize the viewpoints of the two image views. 
		for(int32_t i32I = 0; i32I < 2; ++i32I)
		{
			if((res = viewImageSrc[i32I].SynchronizePointOfView(&viewImageLrn[0])).IsFail())
			{
				ErrorPrint(res, L"Failed to synchronize view.\n");
				break;
			}
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ�. // Synchronize the viewpoints of the two image views. 
		if((res = viewImageDst.SynchronizePointOfView(&viewImageLrn[0])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� ���� �������� ����ȭ �Ѵ�. // Synchronize the page of the two image views. 
		if((res = viewImageLrn[1].SynchronizePageIndex(&viewImageLrn[0])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view.\n");
			break;
		}

		// �� �̹��� ���� �������� ����ȭ �Ѵ�. // Synchronize the page of the two image views. 
		for(int32_t i32I = 0; i32I < 2; ++i32I)
		{
			if((res = viewImageSrc[i32I].SynchronizePageIndex(&viewImageLrn[0])).IsFail())
			{
				ErrorPrint(res, L"Failed to synchronize view.\n");
				break;
			}
		}

		// Destination 3D �̹��� �� ���� // Create the destination 3D image view
		if((res = view3DDst.Create(400, 200, 1300, 800)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Moire 3D ��ü ���� // Create Moire 3D object
		CMoire3D Moire3D;

		CFL3DObjectHeightMap fl3DOHM;

		// Learn �̹��� ���� // Set the learn image
		Moire3D.SetLearnImage(flaFliLrnImage);
		// Source �̹��� ���� // Set the source image
		Moire3D.SetSourceImage(flaFliSrcImage);
		// Destination �̹��� ���� // Set the destination image
		Moire3D.SetDestinationImage(fliDstImage);
		// Destionation 3D Object ���� // Set the Destionation 3D Object 
		Moire3D.SetDestinationObject(fl3DOHM);
		// ī�޶��� working distance ���� // Set working distance of the camera
		Moire3D.SetWorkingDistance(330);
		// ī�޶��� field of view ���� // Set field of view of the camera
		Moire3D.SetFieldOfView(400);
		// �������� ���� ���� // Set angle of projector
		Moire3D.SetAngleOfProjector(CMultiVar<double>(73, 105));
		// Phase Unwrap ������׷� bin ���� ���� // Set histogram bin range for phase unwrapping
		Moire3D.SetBinInterval(CMultiVar<double>(1, 1));
		// ���� Ÿ�� ���� // Set Pattern Type
		Moire3D.SetPatternType(CMoire3D::EPatternType_SquareWave);
		// Noise ���� ��� Ȱ��ȭ // Enable noise reduction mode
		Moire3D.EnableNoiseReduction(true);

		// �ռ� ������ �Ķ���� ��� Calibration ���� // Calibrate algorithm according to previously set parameters
		if((res = Moire3D.Calibrate()).IsFail())
		{
			ErrorPrint(res, L"Failed to calibrate.");
			break;
		}

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = Moire3D.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute algorithm.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc0 = viewImageSrc[0].GetLayer(0);
		CGUIViewImageLayerWrap layerSrc1 = viewImageSrc[1].GetLayer(0);
		CGUIViewImageLayerWrap layerLrn0 = viewImageLrn[0].GetLayer(0);
		CGUIViewImageLayerWrap layerLrn1 = viewImageLrn[1].GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);
		CGUIView3DLayerWrap layer3D = view3DDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSrc0.Clear();
		layerSrc1.Clear();
		layerLrn0.Clear();
		layerLrn1.Clear();
		layerDst.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = layerLrn0.DrawTextCanvas(&CFLPointD(0, 0), L"Learn Image[0]", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		if((res = layerLrn1.DrawTextCanvas(&CFLPointD(0, 0), L"Learn Image[1]", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		if((res = layerSrc0.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image[0]", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		if((res = layerSrc1.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image[1]", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		CFLImage fliTexture;

		// Texture �̹��� �ε� // Load the texture image
		if((res = fliTexture.Load(L"../../ExampleImages/Moire3D/text.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Height Map�� Texture Image ����
		CFL3DObjectHeightMap* pFl3DOHM = dynamic_cast<CFL3DObjectHeightMap *>(Moire3D.GetDestinationObject());
		pFl3DOHM->SetTextureImage(fliTexture);

		// 3D �̹��� �信 Height Map (Dst Image) �̹����� ���÷���
		if((res = view3DDst.PushObject(CFL3DObjectHeightMap(&fliDstImage, &fliTexture))).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((res = view3DDst.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit of the 3D view.\n");
			break;
		}

		if((res = layer3D.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		viewImageLrn[0].SetLayerAutoClearMode(ELayerAutoClearMode_PageChanged, false);
		viewImageLrn[1].SetLayerAutoClearMode(ELayerAutoClearMode_PageChanged, false);
		viewImageSrc[0].SetLayerAutoClearMode(ELayerAutoClearMode_PageChanged, false);
		viewImageSrc[1].SetLayerAutoClearMode(ELayerAutoClearMode_PageChanged, false);

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageLrn[0].Invalidate(true);
		viewImageLrn[1].Invalidate(true);
		viewImageSrc[0].Invalidate(true);
		viewImageSrc[1].Invalidate(true);
		viewImageDst.Invalidate(true);
		view3DDst.Invalidate(true);

		// �̹��� ��, 3D �䰡 ����� �� ���� ��ٸ�
		while(viewImageLrn[0].IsAvailable() && viewImageLrn[1].IsAvailable() && viewImageSrc[0].IsAvailable() && viewImageSrc[1].IsAvailable() && view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}