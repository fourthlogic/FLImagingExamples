#include <cstdio>

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
		if(IsFail(res = fliISrcImage.Load(L"../../ExampleImages/ActiveContour/Grid Of Cross.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage[0].Create(300, 0, 300 + 520, 430)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(300 + 520, 0, 300 + 520 * 2, 430)))
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

		// Active Contour 객체 생성 // Create Active Contour object
		CActiveContour ac;

		// Source 이미지 설정 // Set source image 
		res = ac.SetSourceImage(fliISrcImage);

		// Source ROI 설정 // Set source ROI
		CFLFigure* pFlfRegion = CFigureUtilities::ConvertFigureStringToObject(L"RG[D(129.22800000000007,126.67680000000001),D(731.22800000000007,120.67680000000001),D(733.22800000000007,262.67680000000001),D(253.22800000000007,246.67680000000001),D(265.22800000000007,600.67679999999996),D(603.22800000000007,594.67679999999996),D(607.22800000000007,400.67680000000001),D(403.22800000000007,396.67680000000001),D(409.22800000000007,448.67680000000001),D(565.22800000000007,450.67680000000001),D(549.22800000000007,556.67679999999996),D(289.22800000000007,558.67679999999996),D(291.22800000000007,292.67680000000001),D(721.22800000000007,294.67680000000001),D(721.22800000000007,720.67679999999996),D(119.22800000000007,718.67679999999996),D(113.22800000000007,142.67680000000001)]");
		res = ac.SetSourceROI(pFlfRegion);

		// Destination 이미지 설정 // Set destination image
		res = ac.SetDestinationImage(fliIDstImage);

		// Point Count 설정 // Set Point Count
		res = ac.SetPointCount(3000);

		// Max Length 설정 // Set Max Length
		res = ac.SetMaxLength(3);

		// Low Threshold 설정 // Set Low Threshold
		res = ac.SetLowThreshold(20);

		// High Threshold 설정 // Set High Threshold
		res = ac.SetHighThreshold(50);

		// Fit Margin 설정 // Set Fit Margin
		res = ac.SetFitMargin(5);

		// 알고리즘 수행 // Execute the algorithm
		if((res = ac.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Active Contour.");

			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update the image view.
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);

		for(int32_t i32Iteration = 0; i32Iteration < 20; ++i32Iteration)
		{
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Spacing();
			ac.Spacing();
			ac.Spacing();
			ac.Spacing();
			ac.Spacing();

			/* PushBack Figure */
			{
				viewImage[0].ClearFigureObject();
				viewImage[0].PushBackFigureObject(ac.GetContourFigure());
				viewImage[0].Invalidate(true);

				Sleep(50);
			}
		}

		viewImage[0].PushBackFigureObject((CFLFigure*)ac.GetSourceROI());

		// 이미지 뷰 출력 문자열 // image view string
		Base::CFLString<wchar_t> flsText1 = L"Source Image";
		Base::CFLString<wchar_t> flsText2 = L"Destination Image";

		// 레이어는 따로 해제하지 않아도 View가 해제 될 때 같이 해제된다. // The layer is released together when View is released without releasing it separately.
		CGUIViewImageLayerWrap layer1 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layer2 = viewImage[1].GetLayer(0);

		// View 정보를 디스플레이 합니다. // Display View information.
		// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = layer1.DrawTextCanvas(&CFLPoint<double>(0, 0), flsText1.GetBuffer(), YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(res = layer2.DrawTextCanvas(&CFLPoint<double>(0, 0), flsText2.GetBuffer(), YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update the image view.
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);

		delete pFlfRegion;
	}
	while(false);

	return 0;
}