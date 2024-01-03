#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliImage;
	CFLImage fliFFTImage;
	CFLImage fliIFFTImage;

	// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImageOriginal;
	CGUIViewImageWrap viewImageFFT;
	CGUIViewImageWrap viewImageIFFT;

	do
	{
		// 동작 결과 // operation result
		CResult eResult = EResult_UnknownError;

		// 이미지 로드 // Loads image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/FourierTransform/TempleNoise.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(eResult = viewImageOriginal.Create(300, 0, 300 + 512, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageFFT.Create(300 + 512, 0, 300 + 512 * 2, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageIFFT.Create(300 + 512 * 2, 0, 300 + 512 * 3, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the image view
		if(IsFail(eResult = viewImageOriginal.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImageIFFT.SetImagePtr(&fliIFFTImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImageOriginal.SynchronizeWindow(&viewImageFFT)))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(eResult = viewImageOriginal.SynchronizeWindow(&viewImageIFFT)))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		// Fourier Transform 객체 생성 // Create Fourier Transform object
		CFourierTransform fourierTransform;

		// Source 이미지 설정 // Set source image 
		fourierTransform.SetSourceImage(fliImage);

		// Destination 이미지 설정(FFT image) // Set destination image(FFT image) 
		fourierTransform.SetDestinationImage(fliFFTImage);

		// 결과 이미지 포멧 설정 (FFT image, 32/64 bit Floating Point 설정 가능) // Set Result image format(FFT image, 32/64 bit Floating Point) 
		fourierTransform.SetResultType(EFloatingPointAccuracy_Bit32);

		// 푸리에 변환 결과 이미지를 쉬프트해서 받도록 설정 // Set to receive a shifted image of the Fourier transform result
		fourierTransform.SetShiftSpectrum(EFourierTransformShiftSpectrum_Shift);

		// 알고리즘 수행(FFT) // Execute the algorithm(FFT)
		if((eResult = fourierTransform.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Fourier Transform.");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the image view
		if(IsFail(eResult = viewImageFFT.SetImagePtr(&fliFFTImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 이미지의 노이즈를 감소하기(Mask 사용) // Reduce the noise in the image (using Mask)
		// Mask 객체 생성 // Create Mask object
		CMask imageMask;

		// 변환 이미지를 설정(FFT) // Set source image(FFT image)
		imageMask.SetSourceImage(fliFFTImage);

		// CFLFigureArray 객체를 생성 // Create CFLFigureArray object
		CFLFigureArray flaROIs;

		// 미리 그려둔 Mask region Figure Array 불러오기 // Load Pre-drawn Mask Region Figure Array
		if((eResult = flaROIs.Load(L"../../ExampleImages/FourierTransform/FFTRegion.fig")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the figure file.");
			break;
		}

		// 지정한 ROI를 입력 // Set mask ROI
		imageMask.SetSourceROI(&flaROIs);

		// 마스크 값을 입력 // set mask value
		imageMask.SetMask(CMultiVar<double>(0.0));

		// 알고리즘 수행(mask) // Execute the algorithm(mask)		
		if((eResult = imageMask.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Image Mask.");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update the image view.
		viewImageFFT.Invalidate(true);

		// Source 이미지 설정(FFT image) // Set source image (FFT image)
		fourierTransform.SetSourceImage(fliFFTImage);

		// Destination 이미지 설정(IFFT image) // Set destination image(IFFT image)
		fourierTransform.SetDestinationImage(fliIFFTImage);

		// 알고리즘 수행(IFFT) // Execute the algorithm(IFFT)
		if((eResult = fourierTransform.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Fourier Transform.");
			break;
		}

		// 출력을 위한 이미지 레이어를 얻어옵니다. //  Gets the image layer for output.
		// 따로 해제할 필요 없음 // No need to release separately
		CGUIViewImageLayerWrap layer1 = viewImageOriginal.GetLayer(0);
		CGUIViewImageLayerWrap layer2 = viewImageFFT.GetLayer(0);
		CGUIViewImageLayerWrap layer3 = viewImageIFFT.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Delete the shapes drawn on the existing layer
		layer1.Clear();

		// Mask ROI 영역을 출력 // Display the mask ROI area
		if(IsFail(eResult = layer2.DrawFigureImage(&flaROIs, LIME)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		// View 정보를 디스플레이 합니다. // Display View information.
		if(IsFail(eResult = layer1.DrawTextCanvas(&CFLPointD(0, 0), L"Spatial Domain", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layer2.DrawTextCanvas(&CFLPointD(0, 0), L"Frequency Domain", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layer3.DrawTextCanvas(&CFLPointD(0, 0), L"Inverse FFT Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update the image view.
		viewImageOriginal.ZoomFit();
		viewImageFFT.ZoomFit();
		viewImageIFFT.ZoomFit();
		viewImageOriginal.Invalidate(true);
		viewImageFFT.Invalidate(true);
		viewImageIFFT.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageOriginal.IsAvailable() && viewImageFFT.IsAvailable() && viewImageIFFT.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}