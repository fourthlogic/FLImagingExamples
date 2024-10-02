#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap arrViewImage[2];

	// 이미지 객체 선언 // Declare image object
	CFLImage arrFliImage[2];

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	do
	{
		// View 1 생성 // Create View 1
		if(IsFail(res = arrViewImage[0].Create(200, 0, 700, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// View 2 생성 // Create View 2
		if(IsFail(res = arrViewImage[1].Create(700, 0, 1200, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

 		// 각 이미지 뷰의 시점을 동기화 한다. // Synchronize the viewpoint of each image view.
 		if(IsFail(res = arrViewImage[0].SynchronizePointOfView(&arrViewImage[1])))
 		{
 			ErrorPrint(res, "Failed to synchronize view\n");
 			break;
 		}

		// 각 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the position of each image view window
		if(IsFail(res = arrViewImage[0].SynchronizeWindow(&arrViewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Source 이미지 로드 // Load the source image
		if((res = arrFliImage[0].Load(L"../../ExampleImages/Figure/ImageWithFigure2.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		CFLFigureArray flfaSource;

		// Source Image 에 있는 Figure 추출 // Extract figure from source image
		if((res = CFigureUtilities::ConvertImageFiguresToFigureArray(arrFliImage[0], flfaSource)).IsFail())
		{
			ErrorPrint(res, "Failed to convert image figures to figure array.\n");
			break;
		}

		// Source Image 에 있는 Figure Clear // Figure Clear in Source Image
		arrFliImage[0].ClearFigures();

		// arrFliImage[1] 이미지를 arrFliImage[0] 이미지와 동일한 이미지로 생성
		// Create the arrFliImage[1] image as the same image as the arrFliImage[0] image.
		if((res = arrFliImage[1].Assign(arrFliImage[0])).IsFail())
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		for(int32_t i = 0 ; i < 2; ++i)
		{
			if((res = arrViewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				break;
			}
		}

		if(res.IsFail())
			break;
		
		// Source Image 에 Source FigureArray 를 추가 // Add Source FigureArray to Source Image
		if((res = CFigureUtilities::ConvertFigureArrayToImageFigures(flfaSource, arrFliImage[0])).IsFail())
		{
			ErrorPrint(res, "Failed to convert figure array to image figures.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View 에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer[2];

		for(int32_t i = 0; i < 2; ++i)
			layer[i] = arrViewImage[i].GetLayer(0);

		// 화면상 좌표(고정 좌표)에 View 의 이름을 표시
		// Indicates view name on screen coordinates (fixed coordinates)
		layer[0].DrawTextCanvas(CFLPoint<int32_t>(0, 0), L"Source View", YELLOW, BLACK, 30);
		layer[1].DrawTextCanvas(CFLPoint<int32_t>(0, 0), L"Change Attribute", YELLOW, BLACK, 30);

		CFLFigureArray flfaAttribute(flfaSource);

		// ImageFigureAttribute 클래스 선언 // ImageFigureAttribute class declaration
		CImageFigureAttribute ifa;

		for(int64_t i = 0 ; i < flfaAttribute.GetCount(); ++i)
		{
		    // Source figure 설정 // Source figure settings
			ifa.SetSourceFigure(flfaAttribute.GetAt(i));

			// Figure 이름 추출 // Get figure name
			CFLString<wchar_t> strFigureName(flfaAttribute.GetAt(i)->GetName());

			// Figure 이름별 속성 지정 // Specifying properties by figure name
			if(!strFigureName.Compare(L"Rubber"))
			{
				// 채우기 색상 설정 // Set fill color
				ifa.SetFillColor(LIME);
				// 이름 설정 // Set name
				ifa.SetName(L"Block");
			}
			else if(!strFigureName.Compare(L"SN"))
			{
				// 채우기 색상 설정 // Set fill color
				ifa.SetFillColor(KHAKI);
				// 채우기 색상 투명도 설정 // Set fill color alpha ratio
				ifa.SetFillColorAlphaRatio(0.15f);
				// 이름 설정 // Set name
				ifa.SetName(L"S/N");
			}
			else if(!strFigureName.Compare(L"Flux"))
			{
				// 채우기 색상 설정 // Set fill color
				ifa.SetFillColor(BLUE);
				// 선 색상 설정 // Set line color
				ifa.SetLineColor(WHITE);
				// 선 두께 설정 // Set line width
				ifa.SetLineWidth(3);
				// 이름 설정 // Set name
				ifa.SetName(L"");
			}

			// 알고리즘 동작 // Execute algorithm
			if((res = ifa.Execute()).IsFail())
			{
				ErrorPrint(res, "Failed to execute.\n");
				break;
			}
		}

		if(res.IsFail())
			break;

		// arrFliImage[1] Image 에 flfaAttribute 를 추가 // Add flfaAttribute to arrFliImage[1] Image
		if((res = CFigureUtilities::ConvertFigureArrayToImageFigures(flfaAttribute, arrFliImage[1])).IsFail())
		{
			ErrorPrint(res, "Failed to convert figure array to image figures.\n");
			break;
		}

		// 이미지 뷰들을 갱신 합니다. // Update the image views.
		for(int32_t i = 0; i < 2; ++i)
			arrViewImage[i].Invalidate(true);

		// 이미지 뷰가 둘중에 하나라도 꺼지면 종료로 간주 // Consider closed when any of the two image views are turned off
		while(arrViewImage[0].IsAvailable() && arrViewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}