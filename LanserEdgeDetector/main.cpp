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
		CResult eResult = EResult_UnknownError;

		// 이미지 로드 // Loads image
		if(IsFail(eResult = fliISrcImage.Load(L"../../ExampleImages/LanserEdgeDetector/Circuit Board.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(eResult = viewImage[0].Create(300, 0, 300 + 520, 430)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].Create(300 + 520, 0, 300 + 520 * 2, 430)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the image view
		if(IsFail(eResult = viewImage[0].SetImagePtr(&fliISrcImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].SetImagePtr(&fliIDstImage)))
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

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		// Lanser Edge Detector 객체 생성 // Create Lanser Edge Detector object
		CLanserEdgeDetector LanserEdge;

		// Source 이미지 설정 // Set source image 
		LanserEdge.SetSourceImage(fliISrcImage);

		// Destination 이미지 설정 // Set destination image
		LanserEdge.SetDestinationImage(fliIDstImage);

		// Edge Detector Mode 설정 // Set Edge Detector Mode
		LanserEdge.SetEdgeDetectorMode(CLanserEdgeDetector::EEdgeDetectorMode_Shen);

		// Preprocessing Mode 설정 // Set Preprocessing Mode
		LanserEdge.SetNoiseReduction(CLanserEdgeDetector::ENoiseReduction_Smoothing);

		// threshold 값 // threshold value
		CMultiVar<double> mvThresholdValue;

		// 최소 threshold 값 입력 // minimum threshold value push
		mvThresholdValue.PushBack(15);

		// 최대 threshold 값 입력 // maximum threshold value push
		mvThresholdValue.PushBack(30);

		// 최소, 최대 threshold 설정 // Set minimum, maximum threshold
		LanserEdge.SetThreshold(mvThresholdValue);

		// Alpha 값 설정 // Set alpha value
		LanserEdge.SetAlphaValue(2.0f);

		// 알고리즘 수행 // Execute the algorithm
		if((eResult = LanserEdge.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Lanser Edge Detector.");

			break;
		}

		// 이미지 뷰 출력 문자열 // image view string
		Base::CFLString<wchar_t> flsText1 = L"Source Image";
		Base::CFLString<wchar_t> flsText2 = L"Destination Image";

		// 레이어는 따로 해제하지 않아도 View가 해제 될 때 같이 해제된다. // The layer is released together when View is released without releasing it separately.
		CGUIViewImageLayerWrap layer1 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layer2 = viewImage[1].GetLayer(0);

		// View 정보를 디스플레이 합니다. // Display View information.
		// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = layer1.DrawTextCanvas(&CFLPointD(0, 0), flsText1.GetBuffer(), YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
		{
			ErrorPrint(eResult, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(eResult = layer2.DrawTextCanvas(&CFLPointD(0, 0), flsText2.GetBuffer(), YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
		{
			ErrorPrint(eResult, "Failed to draw text on the image view.\n");
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