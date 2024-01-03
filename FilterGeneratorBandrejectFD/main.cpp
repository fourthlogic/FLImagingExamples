#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliSrcImage;
	CFLImage fliFFTImage;
	CFLImage fliIdealFilter;
	CFLImage fliIdealDst;
	CFLImage fliButterworthFilter;
	CFLImage fliButterworthDst;
	CFLImage fliGaussianFilter;
	CFLImage fliGaussianDst;

	// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImage[8];

	do
	{
		// 동작 결과 // operation result
		CResult eResult = EResult_UnknownError;

		// 이미지 로드 // Loads image
		if(IsFail(eResult = fliSrcImage.Load(L"../../ExampleImages/FilterGeneratorFD/Sea1Ch.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(eResult = viewImage[0].Create(300, 0, 300 + 384, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].Create(300 + 384, 0, 300 + 384 * 2, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[2].Create(300 + 384 * 2, 0, 300 + 384 * 3, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[3].Create(300 + 384 * 3, 0, 300 + 384 * 4, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[4].Create(300, 384, 300 + 384, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[5].Create(300 + 384, 384, 300 + 384 * 2, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[6].Create(300 + 384 * 2, 384, 300 + 384 * 3, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[7].Create(300 + 384 * 3, 384, 300 + 384 * 4, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the image view
		if(IsFail(eResult = viewImage[0].SetImagePtr(&fliSrcImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].SetImagePtr(&fliIdealFilter)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[2].SetImagePtr(&fliButterworthFilter)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[3].SetImagePtr(&fliGaussianFilter)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[4].SetImagePtr(&fliFFTImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[5].SetImagePtr(&fliIdealDst)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[6].SetImagePtr(&fliButterworthDst)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[7].SetImagePtr(&fliGaussianDst)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views. 
		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[2])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[3])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[4])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[5])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[6])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[7])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[2])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[3])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[4])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[5])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[6])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[7])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		// Fourier Transform 객체 생성 // Create Fourier Transform object
		CFourierTransform fourierTransform;

		// Source 이미지 설정 // Set source image 
		fourierTransform.SetSourceImage(fliSrcImage);

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

		// FilterGeneratorBandrejectFD 객체 생성 // Create FilterGeneratorBandrejectFD object
		CFilterGeneratorBandrejectFD filterGenerator;

		// Source 이미지 설정 // Set source image 
		filterGenerator.SetSourceImage(fliFFTImage);

		// Destination 이미지 설정 // Set destination image
		filterGenerator.SetDestinationImage(fliIdealFilter);

		// 정밀도 설정 // Set Accuracy
		filterGenerator.SetAccuracy(EFloatingPointAccuracy_Bit32);

		// MinFrequency 설정 // set MinFrequency
		filterGenerator.SetMinFrequency(0.1);

		// MaxFrequency 설정 // set MaxFrequency
		filterGenerator.SetMaxFrequency(0.6);

		// Filter Shape 설정 // set Filter Shape
		filterGenerator.SetFilterShape(CFilterGeneratorBandrejectFD::EFilterShape_Ideal);

		// 알고리즘 수행 // Execute the algorithm
		if((eResult = filterGenerator.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute FilterGeneratorBandrejectFD.");
			break;
		}

		// Destination 이미지 설정 // Set destination image
		filterGenerator.SetDestinationImage(fliButterworthFilter);

		// Filter Shape 설정 // set Filter Shape
		filterGenerator.SetFilterShape(CFilterGeneratorBandrejectFD::EFilterShape_Butterworth);

		// Distance 설정 // set Distance
		filterGenerator.SetDistance(256);

		// Degree 설정 // set Degree
		filterGenerator.SetDegree(2);

		// 알고리즘 수행 // Execute the algorithm
		if((eResult = filterGenerator.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute FilterGeneratorBandrejectFD.");
			break;
		}

		// Destination 이미지 설정 // Set destination image
		filterGenerator.SetDestinationImage(fliGaussianFilter);

		// Filter Shape 설정 // set Filter Shape
		filterGenerator.SetFilterShape(CFilterGeneratorBandrejectFD::EFilterShape_Gaussian);

		// Sigma1 설정 // set Sigma1
		filterGenerator.SetSigma1(1);

		// Sigma2 설정 // set Sigma2
		filterGenerator.SetSigma2(1);

		// Phi 설정 // set Phi
		filterGenerator.SetPhi(0);

		// 알고리즘 수행 // Execute the algorithm
		if((eResult = filterGenerator.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute FilterGeneratorBandrejectFD.");
			break;
		}

		// Operation Multiply 객체 생성 // Create Operation Multiply object
		COperationMultiply multiply;
		// Source 이미지 설정 // Set the source image
		multiply.SetSourceImage(fliFFTImage);
		// Operand 이미지 설정 // Set the operand image
		multiply.SetOperandImage(fliIdealFilter);
		// Destination 이미지 설정 // Set the destination image
		multiply.SetDestinationImage(fliIdealDst);
		// 연산 방식 설정 // Set operation source
		multiply.SetOperationSource(EOperationSource_Image);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(eResult = multiply.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute operation multiply.");
			break;
		}

		// Operand 이미지 설정 // Set the operand image
		multiply.SetOperandImage(fliButterworthFilter);
		// Destination 이미지 설정 // Set the destination image
		multiply.SetDestinationImage(fliButterworthDst);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(eResult = multiply.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute operation multiply.");
			break;
		}

		// Operand 이미지 설정 // Set the operand image
		multiply.SetOperandImage(fliGaussianFilter);
		// Destination 이미지 설정 // Set the destination image
		multiply.SetDestinationImage(fliGaussianDst);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(eResult = multiply.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute operation multiply.");
			break;
		}

		// Source 이미지 설정(FFT image) // Set source image (FFT image)
		fourierTransform.SetSourceImage(fliIdealDst);

		// Destination 이미지 설정(IFFT image) // Set destination image(IFFT image)
		fourierTransform.SetDestinationImage(fliIdealDst);

		// 알고리즘 수행(IFFT) // Execute the algorithm(IFFT)
		if((eResult = fourierTransform.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Fourier Transform.");
			break;
		}

		// Source 이미지 설정(FFT image) // Set source image (FFT image)
		fourierTransform.SetSourceImage(fliButterworthDst);

		// Destination 이미지 설정(IFFT image) // Set destination image(IFFT image)
		fourierTransform.SetDestinationImage(fliButterworthDst);

		// 알고리즘 수행(IFFT) // Execute the algorithm(IFFT)
		if((eResult = fourierTransform.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Fourier Transform.");
			break;
		}

		// Source 이미지 설정(FFT image) // Set source image (FFT image)
		fourierTransform.SetSourceImage(fliGaussianDst);

		// Destination 이미지 설정(IFFT image) // Set destination image(IFFT image)
		fourierTransform.SetDestinationImage(fliGaussianDst);

		// 알고리즘 수행(IFFT) // Execute the algorithm(IFFT)
		if((eResult = fourierTransform.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Fourier Transform.");
			break;
		}

		// 출력을 위한 이미지 레이어를 얻어옵니다. //  Gets the image layer for output.
		// 따로 해제할 필요 없음 // No need to release separately
		CGUIViewImageLayerWrap layerSource = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layerFFT = viewImage[4].GetLayer(0);
		CGUIViewImageLayerWrap layerIdealFilter = viewImage[1].GetLayer(0);
		CGUIViewImageLayerWrap layerButterworthFilter = viewImage[2].GetLayer(0);
		CGUIViewImageLayerWrap layerGaussianFilter = viewImage[3].GetLayer(0);
		CGUIViewImageLayerWrap layerIdealDst = viewImage[5].GetLayer(0);
		CGUIViewImageLayerWrap layerButterworthDst = viewImage[6].GetLayer(0);
		CGUIViewImageLayerWrap layerGaussianDst = viewImage[7].GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Delete the shapes drawn on the existing layer
		layerSource.Clear();
		layerFFT.Clear();
		layerIdealFilter.Clear();
		layerButterworthFilter.Clear();
		layerGaussianFilter.Clear();
		layerIdealDst.Clear();
		layerButterworthDst.Clear();
		layerGaussianDst.Clear();

		// View 정보를 디스플레이 합니다. // Display View information.
		// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerFFT.DrawTextCanvas(&CFLPointD(0, 0), L"FFT Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerIdealFilter.DrawTextCanvas(&CFLPointD(0, 0), L"Ideal, Min = 0.1, Max = 0.6", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerButterworthFilter.DrawTextCanvas(&CFLPointD(0, 0), L"Butterworth, Distance = 256, Degree = 2", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerGaussianFilter.DrawTextCanvas(&CFLPointD(0, 0), L"Gaussian, Sigma1 = Sigma2 = 1, Phi = 0", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerIdealDst.DrawTextCanvas(&CFLPointD(0, 0), L"Ideal Filtering Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerButterworthDst.DrawTextCanvas(&CFLPointD(0, 0), L"Butterworth Filtering Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerGaussianDst.DrawTextCanvas(&CFLPointD(0, 0), L"Gaussian Filtering Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update the image view.
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);
		viewImage[2].Invalidate(true);
		viewImage[3].Invalidate(true);
		viewImage[4].Invalidate(true);
		viewImage[5].Invalidate(true);
		viewImage[6].Invalidate(true);
		viewImage[7].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable() && viewImage[2].IsAvailable() && viewImage[3].IsAvailable() && viewImage[4].IsAvailable() && viewImage[5].IsAvailable() && viewImage[6].IsAvailable() && viewImage[7].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}