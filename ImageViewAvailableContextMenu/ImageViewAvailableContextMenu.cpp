#include <cstdio>

#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h"


int main()
{
	// You must call the following function once
	// before using any features of the FLImaging(R) library
	CLibraryUtilities::Initialize();

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[2];

	CResult res = EResult_UnknownError;

	do
	{
		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage[0].Create(300, 0, 300 + 520, 430)))
		{
			ErrorPrint(res,"Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage[1].Create(300 + 520, 0, 300 + 520 * 2, 430)))
		{
			ErrorPrint(res,"Failed to create the image view.\n");
			break;
		}

		// 뷰의 시점을 동기화 한다.
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res,"Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res,"Failed to synchronize window\n");
			break;
		}

		// 이미지 뷰에서 이용 가능한 컨텍스트 메뉴를 설정합니다. 
		// EAvailableViewImageContextMenu_All 이 기본값이며, 이 값으로 설정하면 모든 메뉴를 사용 가능한 상태가 됩니다.
		// 아래와 같이 EAvailableViewImageContextMenu_None 으로 설정할 경우 모든 메뉴가 비활성화됩니다.
		viewImage[0].SetAvailableViewImageContextMenu(EAvailableViewImageContextMenu_None);

		// 이미지뷰의 0번 레이어 가져오기
		CGUIViewImageLayerWrap layer = viewImage[0].GetLayer(0);

		// 기존에 Layer 에 그려진 도형들을 삭제
		layer.Clear();

		// 안내 문자열 생성
		CFLString<wchar_t> flsInformation = L"RIGHT BUTTON CLICK ON MOUSE AND SEE THE CONTEXT MENU";
		CFLString<wchar_t> flsInformation2 = L"Option : EAvailableViewImageContextMenu_None";

		// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EColor.TRANSPARENCY 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		layer.DrawTextCanvas(CFLPoint<double>(10, 10), flsInformation, LIME, BLACK, 15);
		layer.DrawTextCanvas(CFLPoint<double>(10, 30), flsInformation2, CYAN, BLACK, 15);


		// 이미지 뷰에서 이용 가능한 컨텍스트 메뉴를 설정합니다. 
		// EAvailableViewImageContextMenu_All 이 기본값이며, 이 값으로 설정하면 모든 메뉴를 사용 가능한 상태가 됩니다.
		// 아래와 같이 여러 조합을 이용하여 설정할 수 있으며, 
		// EAvailableViewImageContextMenu_All & ~(EAvailableViewImageContextMenu_Load | EAvailableViewImageContextMenu_ClearFile | EAvailableViewImageContextMenu_Save | EAvailableViewImageContextMenu_CreateImage) 으로 설정할 경우 
		// 파일 열기, 닫기, 저장, 이미지 생성 메뉴가 비활성화됩니다.
		viewImage[1].SetAvailableViewImageContextMenu(EAvailableViewImageContextMenu_All & ~(EAvailableViewImageContextMenu_Load | EAvailableViewImageContextMenu_ClearFile | EAvailableViewImageContextMenu_Save | EAvailableViewImageContextMenu_CreateImage));


		// 이미지뷰의 0번 레이어 가져오기
		layer = viewImage[1].GetLayer(0);

		// 기존에 Layer 에 그려진 도형들을 삭제
		layer.Clear();

		// 안내 문자열 지정
		flsInformation = L"RIGHT BUTTON CLICK ON MOUSE AND SEE THE CONTEXT MENU";
		flsInformation2 = L"Option : EAvailableViewImageContextMenu_All & \n           ~(EAvailableViewImageContextMenu_Load | \n              EAvailableViewImageContextMenu_ClearFile | \n              EAvailableViewImageContextMenu_Save | \n              EAvailableViewImageContextMenu_CreateImage)";

		// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EColor.TRANSPARENCY 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		layer.DrawTextCanvas(CFLPoint<double>(10, 10), flsInformation, LIME, BLACK, 15);
		layer.DrawTextCanvas(CFLPoint<double>(10, 30), flsInformation2, CYAN, BLACK, 15);

		for(int32_t i = 0; i < 2; ++i)
			viewImage[i].Invalidate();

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage[0].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}