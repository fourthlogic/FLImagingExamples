#include <cstdio>
#include "../CommomHeader/ErrorPrint.h"
#include <FLImaging.h>


enum EType
{
	EType_Source = 0,
	EType_Destination1,
	EType_Destination2,
	EType_Destination3,
	EType_Destination4,
	ETypeCount,
};

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	do
	{
		// 알고리즘 동작 결과 // Algorithmic executation result
		CResult eResult = EResult_UnknownError;

		// Source 이미지 뷰 생성 // Create Source image view
		if(IsFail(eResult = arrViewImage[EType_Source].Create(100, 0, 356, 256)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination2 이미지 뷰 생성 // Create destination image view
		if(IsFail(eResult = arrViewImage[EType_Destination1].Create(100, 256, 356, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination2 이미지 뷰 생성 // Create destination image view
		if(IsFail(eResult = arrViewImage[EType_Destination2].Create(356, 256, 612, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination2 이미지 뷰 생성 // Create destination image view
		if(IsFail(eResult = arrViewImage[EType_Destination3].Create(100, 512, 356, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination2 이미지 뷰 생성 // Create destination image view
		if(IsFail(eResult = arrViewImage[EType_Destination4].Create(356, 512, 612, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if(IsFail(eResult = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(eResult, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination2])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination3])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination4])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination2])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination3])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination4])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Utility FigureMorphology 객체 생성 // Create Utility FigureMorphology object
		CFigureMorphology figureMorphology;
		// FigureMorphology Source ROI Setting
		CFLFigureArray flfSourceFigure;
		flfSourceFigure.PushBack(CFLRect<int32_t>(125, 100, 225, 150, 0.000000));
		flfSourceFigure.PushBack(CFLRect<int32_t>(15, 100, 115, 150, 0.000000));
		figureMorphology.SetSourceFigure(&flfSourceFigure);
		// FigureMorphology FigureMorphologyMethod Setting
		figureMorphology.SetFigureMorphologyMethod(CFigureMorphology::EFigureMorphologyMethod_Erode);
		figureMorphology.SetKernel(5,5);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(eResult = figureMorphology.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute figureMorphology.");
			
			break;
		}

		CFLFigureArray flfResultFigureErode;
		figureMorphology.GetResultFigure(flfResultFigureErode);

		// FigureMorphology FigureMorphologyMethod Setting
		figureMorphology.SetFigureMorphologyMethod(CFigureMorphology::EFigureMorphologyMethod_Dilate);
		figureMorphology.SetKernel(5, 5);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(eResult = figureMorphology.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute figureMorphology.");

			break;
		}

		CFLFigureArray flfResultFigureDilate;
		figureMorphology.GetResultFigure(flfResultFigureDilate);

		// FigureMorphology FigureMorphologyMethod Setting
		figureMorphology.SetFigureMorphologyMethod(CFigureMorphology::EFigureMorphologyMethod_Open);
		figureMorphology.SetKernel(5, 5);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(eResult = figureMorphology.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute figureMorphology.");

			break;
		}

		CFLFigureArray flfResultFigureOpen;
		figureMorphology.GetResultFigure(flfResultFigureOpen);

		// FigureMorphology FigureMorphologyMethod Setting
		figureMorphology.SetFigureMorphologyMethod(CFigureMorphology::EFigureMorphologyMethod_Close);
		figureMorphology.SetKernel(5, 5);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(eResult = figureMorphology.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute figureMorphology.");

			break;
		}

		CFLFigureArray flfResultFigureClose;
		figureMorphology.GetResultFigure(flfResultFigureClose);

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
			// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
			arrLayer[i].Clear();
		}

		// 이미지 뷰 정보 표시 // Display image view information
		// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic


		if(IsFail(arrLayer[EType_Source].DrawFigureImage(&flfSourceFigure, RED)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination1].DrawFigureImage(&flfSourceFigure, RED)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination2].DrawFigureImage(&flfSourceFigure, RED)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination3].DrawFigureImage(&flfSourceFigure, RED)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination4].DrawFigureImage(&flfSourceFigure, RED)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination1].DrawFigureImage(&flfResultFigureErode, LIME)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination2].DrawFigureImage(&flfResultFigureDilate, LIME)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination3].DrawFigureImage(&flfResultFigureOpen, LIME)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination4].DrawFigureImage(&flfResultFigureClose, LIME)))
			printf("Failed to draw figure\n");

		if(IsFail(eResult = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Figure", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPointD(0, 0), L"Erode Method", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPointD(0, 0), L"Dilate Method", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination3].DrawTextCanvas(&CFLPointD(0, 0), L"Close Method", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination4].DrawTextCanvas(&CFLPointD(0, 0), L"Open Method", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}


		// 이미지 뷰를 갱신 // Update image view
		arrViewImage[EType_Source].Invalidate(true);
		arrViewImage[EType_Destination1].Invalidate(true);
		arrViewImage[EType_Destination2].Invalidate(true);
		arrViewImage[EType_Destination3].Invalidate(true);
		arrViewImage[EType_Destination4].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(arrViewImage[EType_Source].IsAvailable()
			  && arrViewImage[EType_Destination1].IsAvailable() 
			  && arrViewImage[EType_Destination2].IsAvailable() 
			  && arrViewImage[EType_Destination3].IsAvailable()
			  && arrViewImage[EType_Destination4].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}