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
		if(IsFail(res = fliISrcImage.Load(L"../../ExampleImages/Gradation/House.flif")))
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

		// Gradation 객체 생성 // Create Gradation object
		CGradation Gradation;

		// Source 이미지 설정 // Set source image 
		Gradation.SetSourceImage(fliISrcImage);

		// Destination 이미지 설정 // Set destination image
		Gradation.SetDestinationImage(fliIDstImage);

		// 시작 Alpha 값 설정 // Set start alpha value
		CMultiVar<double> mvStartAlpha(0., 0., 0.);
		Gradation.SetStartAlpha(mvStartAlpha);

		// 끝 Alpha 값 설정 // Set end alpha value
		CMultiVar<double> mvEndAlpha(0.1, 0.6, 0.9);
		Gradation.SetEndAlpha(mvEndAlpha);

		// Gradation Start Value 설정(3Ch) // Set Gradation Start Value(3Ch)
		CMultiVar<double> mvStartValue(255, 0, 0);
		Gradation.SetStartValue(mvStartValue);

		// Gradation End Value 설정(3Ch) // Set Gradation End Value(3Ch)
		CMultiVar<double> mvEndValue(0, 0, 255);
		Gradation.SetEndValue(mvEndValue);

		// Gradation Vector Figure 객체 // Gradation Vector Figure object
		CFLLine<double> fllVector;
		fllVector.Load(L"../../ExampleImages/Gradation/Vector.fig");

		Gradation.SetVector(fllVector);

		// 알고리즘 수행 // Execute the algorithm
		if((res = Gradation.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Gradation.");
			break;
		}

		// 레이어는 따로 해제하지 않아도 View가 해제 될 때 같이 해제된다. // The layer is released together when View is released without releasing it separately.
		CGUIViewImageLayerWrap layer1 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layer2 = viewImage[1].GetLayer(0);

		// Draw Figure 객체 // Gradation Vector Figure object
		CFLFigureArray flfaDrawVector;
		flfaDrawVector.Load(L"../../ExampleImages/Gradation/DrawVector.fig");

		CFLRect<double> fllRect1(fllVector.flpPoints[0] - 15, fllVector.flpPoints[0] + 15);
		CFLRect<double> fllRect2(fllVector.flpPoints[1] - 15, fllVector.flpPoints[1] + 15);

		if(IsFail(res = layer1.DrawFigureImage(&fllRect1, BLUE, 5, BLUE, EGUIViewImagePenStyle_Solid)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(res = layer1.DrawFigureImage(&fllRect2, RED, 5, RED, EGUIViewImagePenStyle_Solid)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// Gradation Vector 출력 // Draw gradation vector
		if(IsFail(res = layer1.DrawFigureImage(&flfaDrawVector, BLACK, 5)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(res = layer1.DrawFigureImage(&flfaDrawVector, LIME, 3)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// text를 출력합니다. // Display text.
		if(IsFail(res = layer1.DrawTextImage(&fllVector.flpPoints[0], L"Start Value(255, 0, 0)/Start Alpha(0, 0, 0)", YELLOW, BLACK, 15, false, EGUIViewImageTextAlignment_RIGHT)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layer1.DrawTextImage(&fllVector.flpPoints[1], L"End(0, 0, 255)/Start Alpha(0.1, 0.6, 0.9)", YELLOW, BLACK, 15)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// View 정보를 디스플레이 합니다. // Display View information.
		if(IsFail(res = layer1.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layer2.DrawTextCanvas(&CFLPointD(0, 0), L"Gradation Image", YELLOW, BLACK, 30)))
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