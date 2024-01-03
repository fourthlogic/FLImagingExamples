#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage;

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult eResult = EResult_UnknownError;

	do
	{
		// 이미지 로드 // Load image
		if((eResult = fliImage.Load(L"../../ExampleImages/FuzzyEntropy/FuzzyEntropySource.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if((eResult = viewImage.Create(400, 0, 912, 612)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		if((eResult = viewImage.SetImagePtr(&fliImage)).IsFail())
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.");
			break;
		}

		// fuzzyentropy 객체 생성 // Create fuzzyentropy object
		CFuzzyEntropy fuzzyentropy;

		// ROI 범위 설정 // Set the ROI value		
		CFLCircle<double> flrROI(310.466830, 81.769042, 81.769042, 0.000000, 0.000000, 360.000000, EArcClosingMethod_EachOther);
		// Source 이미지 설정 // Set the Source Image
		fuzzyentropy.SetSourceImage(fliImage);
		// Source ROI 설정 // Set the Source ROI
		fuzzyentropy.SetSourceROI(flrROI);

		// Parameter A, C 생성 (A: 0, C: 255) // Create the parameter A and C (A: 0, C: 255)
		const Base::CMultiVar<double> flmvParameterA = Base::CMultiVar<double>(0.);
		const Base::CMultiVar<double> flmvParameterC = Base::CMultiVar<double>(255.);

		// Parameter 설정(A: 0, C: 255) // Set the parameter(A: 0, C: 255)
		fuzzyentropy.SetParameterA(flmvParameterA);
		fuzzyentropy.SetParameterC(flmvParameterC);

		// 알고리즘 수행 // Execute the algorithm
		if((eResult = fuzzyentropy.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute FuzzyEntropy.");
			break;
		}

		// 결과값을 받아올 CMultiVar<double> 컨테이너 생성 // Create the CMultiVar<double> object to push the result
		CMultiVar<double> mvFuzzyEntropy;

		// 이미지 전체(혹은 ROI 영역) 픽셀값의 Fuzzy Entropy를 구하는 함수 // Function that calculate the fuzzy entropy of the image(or the region of ROI)
		if((eResult = fuzzyentropy.GetResultFuzzyEntropy(mvFuzzyEntropy)).IsFail())
		{
			ErrorPrint(eResult, "Failed to process.");
			break;
		}

		// 이미지 전체(혹은 ROI 영역) 픽셀값의 최소값을 구하는 함수 // Function that calculate the min of the pixel value of the image(or the region of ROI)
		if((eResult = fuzzyentropy.GetResultFuzzyEntropy(mvFuzzyEntropy)).IsFail())
		{
			ErrorPrint(eResult, "Failed to process.");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layer.Clear();

		// ROI영역이 어디인지 알기 위해 디스플레이 한다 // Display to find out where ROI is
		if((eResult = layer.DrawFigureImage(&flrROI, LIME)).IsFail())
			ErrorPrint(eResult, "Failed to draw figure\n");

		CFLString<wchar_t> strFuzzyEntropy;
		strFuzzyEntropy.Format(L"Fuzzy Entropy Of Region : %lf", mvFuzzyEntropy.GetAt(0));

		wprintf_s(L"%s\n", strFuzzyEntropy.GetString());

		// 이미지 뷰 정보 표시 // Display image view information
		if((eResult = layer.DrawTextCanvas(&CFLPointD(0, 0), strFuzzyEntropy.GetString(), YELLOW, BLACK, 25)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if((eResult = layer.DrawFigureImage(&flrROI, LIME)).IsFail())
			ErrorPrint(eResult, "Failed to draw figure\n");

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImage.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}