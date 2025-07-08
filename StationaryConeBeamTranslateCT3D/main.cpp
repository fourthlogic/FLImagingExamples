#include <cstdio>
#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h"

int main()
{
	CFLImage fliSrcImage;
	CFLImage fliDstImage;
	CFLImage fliDstSinoImage;
	CFL3DObject floDestination;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;
	CGUIViewImageWrap viewImageDstSino;
	CGUIView3DWrap view3DDst;

	do
	{
		// 알고리즘 동작 결과 // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Source 이미지 로드 // Load the source image
		if((res = fliSrcImage.Load(L"../../ExampleImages/StationaryConeBeamTranslateCT3D/")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliSrcImage.SelectPage(0);

		// Source 이미지 뷰 생성 // Create the source image view
		if((res = viewImageSrc.Create(100, 0, 548, 448)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰에 이미지를 디스플레이 // Display the image in the source image view
		if((res = viewImageSrc.SetImagePtr(&fliSrcImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination 이미지 뷰 생성 // Create the destination image view
		if((res = viewImageDst.Create(548, 0, 996, 448)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination 이미지 뷰에 이미지를 디스플레이 // Display the image in the destination image view
		if((res = viewImageDst.SetImagePtr(&fliDstImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination 이미지 뷰 생성 // Create the destination image view
		if((res = viewImageDstSino.Create(100, 448, 548, 996)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination 이미지 뷰에 이미지를 디스플레이 // Display the image in the destination image view
		if((res = viewImageDstSino.SetImagePtr(&fliDstSinoImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination 3D 이미지 뷰 생성 // Create the destination 3D image view
		if((res = view3DDst.Create(548, 448, 996, 996)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		viewImageSrc.SetFixThumbnailView(true);

		// StationaryConeBeamTranslateCT3D 객체 생성 // Create StationaryConeBeamTranslateCT3D object
		CStationaryConeBeamTranslateCT3D algStationaryConeBeamTranslateCT3D;



		// Source 이미지 설정 // Set the source image
		if((res = algStationaryConeBeamTranslateCT3D.SetSourceImage(&fliSrcImage)).IsFail()) break;
		// 결과 destination 이미지 설정 // Set the destination image
		if((res = algStationaryConeBeamTranslateCT3D.SetDestinationImage(&fliDstImage)).IsFail()) break;
		// 결과 destination sinogram 이미지 설정 // Set the destination sinogram image
		if((res = algStationaryConeBeamTranslateCT3D.SetDestinationSinogramImage(&fliDstSinoImage)).IsFail()) break;
		// 결과 destination object 설정 // Set the destination object
		if((res = algStationaryConeBeamTranslateCT3D.SetDestinationObject(&floDestination)).IsFail()) break;

		if((res = algStationaryConeBeamTranslateCT3D.SetDestinationSinogramIndex(15)).IsFail()) break;

		if((res = algStationaryConeBeamTranslateCT3D.SetDetectorCellSizeUnit(0.08354)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetObjectTranslateDirection(CStationaryConeBeamTranslateCT3D::EObjectTranslateDirection_RightToLeft)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetSourceObjectDistanceUnit(13.60)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetSourceDetectorDistanceUnit(60.00)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetObjectTranslationDistanceUnit(24.00)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetPrincipalDeltaXPixel(0.00)).IsFail()) break;

		if((res = algStationaryConeBeamTranslateCT3D.SetNormalizedAirThreshold(0.60)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetSinogramKeepRatio(0.30)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetInterpolationCoefficient(6)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetMergeCoefficient(21)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.EnableFrequencyRampFilter(true)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetFrequencyWindow(CStationaryConeBeamTranslateCT3D::EFrequencyWindow_Gaussian)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetSigma(0.50)).IsFail()) break;

		if((res = algStationaryConeBeamTranslateCT3D.SetReconstructionPlaneCount(140)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.EnableNegativeClip(true)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.EnableCircularMask(true)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.EnableSigmoid(true)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetSigmoidB(1.00)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetSigmoidM(1.00)).IsFail()) break;
		if((res = algStationaryConeBeamTranslateCT3D.SetIntensityThreshold(190)).IsFail()) break;

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((res = algStationaryConeBeamTranslateCT3D.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute algorithm.");
			break;
		}

		// 3D 이미지 뷰에 Height Map (Dst Image) 이미지를 디스플레이
		CFL3DObject* pFloDestinationAlg = dynamic_cast<CFL3DObject*>(algStationaryConeBeamTranslateCT3D.GetDestinationObject());
		if((res = view3DDst.PushObject(*pFloDestinationAlg)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);
		CGUIViewImageLayerWrap layerDstSino = viewImageDstSino.GetLayer(0);
		CGUIView3DLayerWrap layer3D = view3DDst.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerDst.Clear();
		layerDstSino.Clear();
		layer3D.Clear();
		// View 정보를 디스플레이 한다. // Display view information
		// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
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
		if((res = layerDstSino.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Sinogram Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}
		if((res = layer3D.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		// Zoom Fit
		if((res = viewImageSrc.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit of the image view.\n");
			break;
		}
		if((res = viewImageDst.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit of the image view.\n");
			break;
		}
		if((res = viewImageDstSino.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit of the image view.\n");
			break;
		}
		if((res = view3DDst.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit of the image view.\n");
			break;
		}
				
		CGUIView3DCamera cameraNew(*view3DDst.GetCamera());
		CFLPoint3<float> flpPositionOld = cameraNew.GetPosition();
		flpPositionOld.y = flpPositionOld.z;
		cameraNew.SetPosition(flpPositionOld, false);
		view3DDst.SetCamera(cameraNew);

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);
		viewImageDstSino.Invalidate(true);
		view3DDst.Invalidate(true);

		// 이미지 뷰, 3D 뷰가 종료될 때 까지 기다림
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable() && viewImageDstSino.IsAvailable() && view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}