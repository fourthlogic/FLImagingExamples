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

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImageLrn[2];
	CGUIViewImageWrap viewImageSrc[2];
	CGUIViewImageWrap viewImageDst;
	CGUIView3DWrap view3DDst;

	do
	{
		// 알고리즘 동작 결과 // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Learn 이미지 로드 // Load the reference plane image for calibration
		if((res = fliLearnImage[0].Load(L"../../ExampleImages/Moire3D/Learn0/")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliLearnImage[0].SelectPage(0);

		// Source 이미지 로드 // Load the source image
		if((res = fliSourceImage[0].Load(L"../../ExampleImages/Moire3D/Object0/")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliSourceImage[0].SelectPage(0);

		// Learn 이미지 로드 // Load the reference plane image for calibration
		if((res = fliLearnImage[1].Load(L"../../ExampleImages/Moire3D/Learn1/")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliLearnImage[1].SelectPage(0);

		// Source 이미지 로드 // Load the source image
		if((res = fliSourceImage[1].Load(L"../../ExampleImages/Moire3D/Object1/")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliSourceImage[1].SelectPage(0);

		// CFLArray<CFLImage*> 객체에 CFImage* 추가 // Push back CFImage* to CFLArray<CFLImage*> 
		flaFliLrnImage.PushBack(&fliLearnImage[0]);
		flaFliLrnImage.PushBack(&fliLearnImage[1]);
		flaFliSrcImage.PushBack(&fliSourceImage[0]);
		flaFliSrcImage.PushBack(&fliSourceImage[1]);

		// Learn 이미지 뷰 생성 // Create the learn image view
		if((res = viewImageLrn[0].Create(100, 0, 548, 348)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Learn 이미지 뷰 생성 // Create the destination image view
		if((res = viewImageLrn[1].Create(548, 0, 996, 348)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰 생성 // Create the Source image view
		if((res = viewImageSrc[0].Create(100, 348, 548, 696)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰 생성 // Create the destination image view
		if((res = viewImageSrc[1].Create(548, 348, 996, 696)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Dst 이미지 뷰 생성 // Create the destination image view
		if((res = viewImageDst.Create(996, 348, 1444, 696)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Learn 이미지 뷰에 이미지를 디스플레이 // Display the image in the Learn image view
		for(int32_t i32I = 0; i32I < 2; ++i32I)
		{
			if((res = viewImageLrn[i32I].SetImagePtr(flaFliLrnImage[i32I])).IsFail())
			{
				ErrorPrint(res, L"Failed to set image object on the image view.\n");
				break;
			}
		}

		// Source 이미지 뷰에 이미지를 디스플레이 // Display the image in the Source image view
		for(int32_t i32I = 0; i32I < 2; ++i32I)
		{
			if((res = viewImageSrc[i32I].SetImagePtr(flaFliSrcImage[i32I])).IsFail())
			{
				ErrorPrint(res, L"Failed to set image object on the image view.\n");
				break;
			}
		}

		// Dst 이미지 뷰에 이미지를 디스플레이 // Display the image in the destination image view
		if((res = viewImageDst.SetImagePtr(&fliDstImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// 두 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two view windows
		if((res = viewImageLrn[1].SynchronizeWindow(&viewImageLrn[0])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// 두 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two view windows
		for(int32_t i32I = 0; i32I < 2; ++i32I)
		{
			if((res = viewImageSrc[i32I].SynchronizeWindow(&viewImageLrn[0])).IsFail())
			{
				ErrorPrint(res, L"Failed to synchronize window.\n");
				break;
			}
		}

		// 두 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two view windows
		if((res = viewImageDst.SynchronizeWindow(&viewImageLrn[0])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다. // Synchronize the viewpoints of the two image views. 
		if((res = viewImageLrn[1].SynchronizePointOfView(&viewImageLrn[0])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다. // Synchronize the viewpoints of the two image views. 
		for(int32_t i32I = 0; i32I < 2; ++i32I)
		{
			if((res = viewImageSrc[i32I].SynchronizePointOfView(&viewImageLrn[0])).IsFail())
			{
				ErrorPrint(res, L"Failed to synchronize view.\n");
				break;
			}
		}

		// 두 이미지 뷰의 시점을 동기화 한다. // Synchronize the viewpoints of the two image views. 
		if((res = viewImageDst.SynchronizePointOfView(&viewImageLrn[0])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// 두 이미지 뷰의 페이지를 동기화 한다. // Synchronize the page of the two image views. 
		if((res = viewImageLrn[1].SynchronizePageIndex(&viewImageLrn[0])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view.\n");
			break;
		}

		// 두 이미지 뷰의 페이지를 동기화 한다. // Synchronize the page of the two image views. 
		for(int32_t i32I = 0; i32I < 2; ++i32I)
		{
			if((res = viewImageSrc[i32I].SynchronizePageIndex(&viewImageLrn[0])).IsFail())
			{
				ErrorPrint(res, L"Failed to synchronize view.\n");
				break;
			}
		}

		// Destination 3D 이미지 뷰 생성 // Create the destination 3D image view
		if((res = view3DDst.Create(400, 200, 1300, 800)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Moire 3D 객체 생성 // Create Moire 3D object
		CMoire3D Moire3D;

		CFL3DObjectHeightMap fl3DOHM;

		// Learn 이미지 설정 // Set the learn image
		Moire3D.SetLearnImage(flaFliLrnImage);
		// Source 이미지 설정 // Set the source image
		Moire3D.SetSourceImage(flaFliSrcImage);
		// Destination 이미지 설정 // Set the destination image
		Moire3D.SetDestinationImage(fliDstImage);
		// Destionation 3D Object 설정 // Set the Destionation 3D Object 
		Moire3D.SetDestinationObject(fl3DOHM);
		// 카메라의 working distance 설정 // Set working distance of the camera
		Moire3D.SetWorkingDistance(330);
		// 카메라의 field of view 설정 // Set field of view of the camera
		Moire3D.SetFieldOfView(400);
		// 프로젝터 각도 설정 // Set angle of projector
		Moire3D.SetAngleOfProjector(CMultiVar<double>(73, 105));
		// Phase Unwrap 히스토그램 bin 범위 설정 // Set histogram bin range for phase unwrapping
		Moire3D.SetBinInterval(CMultiVar<double>(1, 1));
		// 패턴 타입 설정 // Set Pattern Type
		Moire3D.SetPatternType(CMoire3D::EPatternType_SquareWave);
		// Noise 감쇠 모드 활성화 // Enable noise reduction mode
		Moire3D.EnableNoiseReduction(true);

		// 앞서 설정된 파라미터 대로 Calibration 수행 // Calibrate algorithm according to previously set parameters
		if((res = Moire3D.Calibrate()).IsFail())
		{
			ErrorPrint(res, L"Failed to calibrate.");
			break;
		}

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((res = Moire3D.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute algorithm.");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc0 = viewImageSrc[0].GetLayer(0);
		CGUIViewImageLayerWrap layerSrc1 = viewImageSrc[1].GetLayer(0);
		CGUIViewImageLayerWrap layerLrn0 = viewImageLrn[0].GetLayer(0);
		CGUIViewImageLayerWrap layerLrn1 = viewImageLrn[1].GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);
		CGUIView3DLayerWrap layer3D = view3DDst.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerSrc0.Clear();
		layerSrc1.Clear();
		layerLrn0.Clear();
		layerLrn1.Clear();
		layerDst.Clear();

		// View 정보를 디스플레이 한다. // Display view information
		// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
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

		// Texture 이미지 로드 // Load the texture image
		if((res = fliTexture.Load(L"../../ExampleImages/Moire3D/text.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Height Map에 Texture Image 설정
		CFL3DObjectHeightMap* pFl3DOHM = dynamic_cast<CFL3DObjectHeightMap *>(Moire3D.GetDestinationObject());
		pFl3DOHM->SetTextureImage(fliTexture);

		// 3D 이미지 뷰에 Height Map (Dst Image) 이미지를 디스플레이
		if((res = view3DDst.PushObject(CFL3DObjectHeightMap(&fliDstImage, &fliTexture))).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination 이미지가 새로 생성됨으로 Zoom fit 을 통해 디스플레이 되는 이미지 배율을 화면에 맞춰준다. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
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

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImageLrn[0].Invalidate(true);
		viewImageLrn[1].Invalidate(true);
		viewImageSrc[0].Invalidate(true);
		viewImageSrc[1].Invalidate(true);
		viewImageDst.Invalidate(true);
		view3DDst.Invalidate(true);

		// 이미지 뷰, 3D 뷰가 종료될 때 까지 기다림
		while(viewImageLrn[0].IsAvailable() && viewImageLrn[1].IsAvailable() && viewImageSrc[0].IsAvailable() && viewImageSrc[1].IsAvailable() && view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}