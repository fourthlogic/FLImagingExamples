﻿#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliISrcImage;
	CFLImage fliIDstImage;

	// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImage[2];

	do
	{
		// 동작 결과 // operation result
		CResult res = EResult_UnknownError;

		// 이미지 로드 // Loads image
		if(IsFail(res = fliISrcImage.Load(L"../../ExampleImages/CoherenceEnhancingDiffusion/fingerprint.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage[0].Create(300, 0, 300 + 384, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(300 + 384, 0, 300 + 384 * 2, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the image view
		if(IsFail(res = viewImage[0].SetImagePtr(&fliISrcImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].SetImagePtr(&fliIDstImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views. 
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		// CoherenceEnhancingDiffusionNonNeg 객체 생성 // Create CoherenceEnhancingDiffusionNonNeg object
		CCoherenceEnhancingDiffusionNonNeg CoherenceEnhancingDiffusionNonNeg;

		// Source 이미지 설정 // Set source image 
		CoherenceEnhancingDiffusionNonNeg.SetSourceImage(fliISrcImage);

		// Destination 이미지 설정 // Set destination image
		CoherenceEnhancingDiffusionNonNeg.SetDestinationImage(fliIDstImage);

		// 미분 평활화(가우사안) // Smoothing for derivative operator.(Gaussian)
		CoherenceEnhancingDiffusionNonNeg.SetSigma(0.5);

		// 확산 계수(가우사안) // Smoothing for diffusion coefficients.(Gaussian)
		CoherenceEnhancingDiffusionNonNeg.SetRho(4);

		// 업데이트 비율 설정 // Set update rate
		CoherenceEnhancingDiffusionNonNeg.SetTheta(0.5);

		// 반복횟수 설정 // Number of iterations
		CoherenceEnhancingDiffusionNonNeg.SetIteration(15);

		// 알고리즘 수행 // Execute the algorithm
		if((res = CoherenceEnhancingDiffusionNonNeg.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Coherence Enhancing Diffusion NonNeg.");
			break;
		}

		// 출력을 위한 이미지 레이어를 얻어옵니다. //  Gets the image layer for output.
		// 따로 해제할 필요 없음 // No need to release separately
		CGUIViewImageLayerWrap layerSource = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layerDestination = viewImage[1].GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Delete the shapes drawn on the existing layer
		layerSource.Clear();
		layerDestination.Clear();

		// View 정보를 디스플레이 합니다. // Display View information.
		// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerDestination.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update the image view.
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}