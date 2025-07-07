﻿#include <cstdio>
#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h"
#include <map>

int main()
{
	CFLImage fliDestination, fliDestinationTexture;
	CFL3DObject floSource;
	CGUIView3DWrap view3D;
	CGUIViewImageWrap viewDepthImage;
	CGUIViewImageWrap viewDestinationTextureImage;

	do
	{
		// 알고리즘 동작 결과 // Algorithm execution result
		CResult eResult = EResult_UnknownError;

		// 이미지 로드 // Load the image
		if((eResult = floSource.Load(L"../../ExampleImages/PointCloudToDepthMapConverter3D/Example.ply")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the image.\n");
			break;
		}

		// PointCloudToDepthMapConverter3D 객체 생성 // Create PointCloudToDepthMapConverter3D object
		CPointCloudToDepthMapConverter3D PointCloudToDepthMapConverter3D;

		// 이미지 크기 설정 // Set the image size.
		PointCloudToDepthMapConverter3D.SetImageSize(2064, 1544);

		// Destination 이미지 설정 // Set the Destination image.
		PointCloudToDepthMapConverter3D.SetDestinationImage(fliDestination);

		// Destination Texture 이미지 설정 // Set the texture image.
		PointCloudToDepthMapConverter3D.SetDestinationImageTexture(fliDestinationTexture);

		// Camera Matrix 설정 // Set the camera matrix
		CFLPoint<float> flpFocalLength;
		CFLPoint<float> flpPrincipalPoint;

		flpFocalLength.x = 2328.800049f;
		flpFocalLength.y = 2330.899902f;
		flpPrincipalPoint.x = 988.599976f;
		flpPrincipalPoint.y = 750.299988f;

		PointCloudToDepthMapConverter3D.SetIntrinsicParameter(flpFocalLength, flpPrincipalPoint);

		//왜곡 계수 설정 // Set the distortion coefficient
		CFLArray<double> flaDistortionCoefficient;

		flaDistortionCoefficient.PushBack(-0.2333453150000);
		flaDistortionCoefficient.PushBack(0.1352355330000);
		flaDistortionCoefficient.PushBack(0.0005843197580);
		flaDistortionCoefficient.PushBack(-0.0005675755000);
		flaDistortionCoefficient.PushBack(-0.0246060137000);

		PointCloudToDepthMapConverter3D.SetDistortionCoefficient(flaDistortionCoefficient);

		// Z축 방향 설정 // Set z-axis direction.
		PointCloudToDepthMapConverter3D.SetDirectionType(EDirectionType::EDirectionType_Increment);

		// Source 3D Object 설정 // Set the source 3D object
		PointCloudToDepthMapConverter3D.SetSourceObject(floSource);

		// 이미지 뷰 생성 // Create image view
		if(IsFail(eResult = viewDepthImage.Create(100, 0, 612, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Destination image view.\n");
			break;
		}

		if(IsFail(eResult = viewDestinationTextureImage.Create(612, 0, 1124, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Destination Texture image view.\n");
			break;
		}

		// 결과 3D 뷰 생성 // Create result 3D view
		if(IsFail(eResult = view3D.Create(100, 512, 612, 1024)))
		{
			ErrorPrint(eResult, "Failed to create the Result 3D view.\n");
			break;
		}

		// 이미지 포인터 설정 // Set image pointer
		viewDepthImage.SetImagePtr(&fliDestination);
		viewDestinationTextureImage.SetImagePtr(&fliDestinationTexture);

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately		
		CGUIViewImageLayerWrap layerViewDepth = viewDepthImage.GetLayer(0);
		CGUIViewImageLayerWrap layerViewDestinationTexture = viewDestinationTextureImage.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerViewDepth.Clear();
		layerViewDestinationTexture.Clear();

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((eResult = PointCloudToDepthMapConverter3D.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute Point Cloud To Depth Map Converter 3D.");
			break;
		}

		// View 정보를 디스플레이 한다. // Display view information
		// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((eResult = layerViewDepth.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		if((eResult = layerViewDestinationTexture.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Texture Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		view3D.PushObject(floSource);
		view3D.UpdateObject();
		view3D.UpdateScreen();
		view3D.ZoomFit();

		viewDepthImage.ZoomFit();
		viewDestinationTextureImage.ZoomFit();

		viewDepthImage.Invalidate();
		viewDestinationTextureImage.Invalidate();

		// 이미지 뷰가 종료될 때 까지 기다림
		while(viewDepthImage.IsAvailable() && viewDestinationTextureImage.IsAvailable() && view3D.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}