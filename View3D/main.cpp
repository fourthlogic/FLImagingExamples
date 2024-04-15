#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


enum EType
{
	EType_Model = 0,
	EType_Texture,
	ETypeCount,
};

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];
	
	// 3D 뷰 선언
	CGUIView3DWrap view3D;

	CResult res = EResult_UnknownError;

	do
	{
		// Model 이미지 로드
		if(IsFail(res = arrFliImage[EType_Model].Load(L"../../ExampleImages/View3D/mountain.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Texture 이미지 로드
		if(IsFail(res = arrFliImage[EType_Texture].Load(L"../../ExampleImages/View3D/mountain_texture.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Model 이미지 뷰 생성
		if(IsFail(res = arrViewImage[EType_Model].Create(100, 0, 612, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Texture 이미지 뷰 생성
		if(IsFail(res = arrViewImage[EType_Texture].Create(612, 0, 1124, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 3D 뷰 생성
		if(IsFail(res = view3D.Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(res, "Failed to create the 3D view.\n");
			break;
		}

		bool bError = false;

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if(IsFail(res = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// 3D 뷰에 모델과 텍스처를 로드하여 디스플레이
		if((res = view3D.PushObject(CFL3DObjectHeightMap(&arrFliImage[EType_Model], &arrFliImage[EType_Texture]))).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the 3D view.\n");
			break;
		}

		view3D.ZoomFit();

		// 두 이미지 뷰의 시점을 동기화 합니다 // Synchronize the viewpoints of the two image views
		if(IsFail(res = arrViewImage[EType_Model].SynchronizePointOfView(&arrViewImage[EType_Texture])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if(IsFail(res = arrViewImage[EType_Model].SynchronizeWindow(&arrViewImage[EType_Texture])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// 이미지 뷰와 3D 뷰 윈도우의 위치를 맞춤
		if(IsFail(res = arrViewImage[EType_Model].SynchronizeWindow(&view3D)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

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
		if(IsFail(res = arrLayer[EType_Model].DrawTextCanvas(&CFLPointD(0, 0), L"Model Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_Texture].DrawTextCanvas(&CFLPointD(0, 0), L"Texture Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3D.GetLayer(2).DrawTextCanvas(&CFLPointD(0, 0), L"3D View", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 // Update image view
		arrViewImage[EType_Model].Invalidate(true);
		arrViewImage[EType_Texture].Invalidate(true);
		view3D.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(arrViewImage[EType_Model].IsAvailable()
			  && arrViewImage[EType_Texture].IsAvailable()
			  && view3D.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}