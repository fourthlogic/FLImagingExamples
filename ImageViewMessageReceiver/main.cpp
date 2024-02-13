#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


class CMessageReceiver : public CFLBase
{
public:

	// CMessageReceiver 생성자
	CMessageReceiver(CGUIViewImageWrap* pViewImage) : m_pViewImage(pViewImage)
	{
		// 메세지를 전달 받기 위해 CBroadcastManager 에 구독 등록
		CBroadcastManager::Subscribe(this);
	}

	// CMessageReceiver 소멸자
	virtual ~CMessageReceiver()
	{
		// 객체가 소멸할때 메세지 수신을 중단하기 위해 구독을 해제한다.
		CBroadcastManager::Unsubscribe(this);
	}

	DeclareGetClassType();

	// 메세지가 들어오면 호출되는 함수 OnReceiveBroadcast 오버라이드 하여 구현
	void OnReceiveBroadcast(const CBroadcastMessage* pMessage) override
	{
		do
		{
			// pMessage 가 nullptr 인지 확인
			if(pMessage == nullptr)
				break;

			// 메세지의 채널을 확인
			switch(pMessage->GetChannel())
			{
			case EGUIBroadcast_ViewImage_PostMouseMove:
				{
					// GetCaller() 가 등록한 이미지뷰인지 확인
					if(pMessage->GetCaller() != (const CFLBase*)m_pViewImage->GetMessageCallerPtr())
						break;

					// pMessage 객체를 CBroadcastMessage_GUI_ViewImage_MouseEvent 로 캐스팅
					CBroadcastMessage_GUI_ViewImage_MouseEvent* pMsgMouseEvent = dynamic_cast<CBroadcastMessage_GUI_ViewImage_MouseEvent*>((CBroadcastMessage*)pMessage);

					// pMsgMouseEvent 가 nullptr 인지 확인
					if(pMsgMouseEvent == nullptr)
						break;

					// 이미지뷰의 0번 레이어 가져오기
					CGUIViewImageLayerWrap layer = m_pViewImage->GetLayer(0);

					// 기존에 Layer 에 그려진 도형들을 삭제
					layer.Clear();

					// 마우스 좌표를 표시할 문자열 생성
					CFLString<wchar_t> flsPosition = L"";
					flsPosition.Format(L"Move X: %d, Y: %d", pMsgMouseEvent->GetCursor()->x, pMsgMouseEvent->GetCursor()->y);

					// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
					// 색상 파라미터를 EColor.TRANSPARENCY 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다.
					// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
					//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
					// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
					//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
					layer.DrawTextCanvas(CFLPoint<double>(10, 10), flsPosition, LIME, BLACK);

					// 이미지뷰를 갱신
					m_pViewImage->Invalidate();
				}
				break;

			case EGUIBroadcast_ViewImage_PostLButtonDown:
				{
					// GetCaller() 가 등록한 이미지뷰인지 확인
					if(pMessage->GetCaller() != (const CFLBase*)m_pViewImage->GetMessageCallerPtr())
						break;

					// pMessage 객체를 CBroadcastMessage_GUI_ViewImage_MouseEvent 로 캐스팅
					CBroadcastMessage_GUI_ViewImage_MouseEvent* pMsgMouseEvent = dynamic_cast<CBroadcastMessage_GUI_ViewImage_MouseEvent*>((CBroadcastMessage*)pMessage);

					// pMsgMouseEvent 가 nullptr 인지 확인
					if(pMsgMouseEvent == nullptr)
						break;

					// 이미지뷰의 0번 레이어 가져오기
					CGUIViewImageLayerWrap layer = m_pViewImage->GetLayer(0);

					// 기존에 Layer 에 그려진 도형들을 삭제
					layer.Clear();

					// 마우스 좌표를 표시할 문자열 생성
					CFLString<wchar_t> flsPosition = L"";
					flsPosition.Format(L"LButtonDown X: %d, Y: %d", pMsgMouseEvent->GetCursor()->x, pMsgMouseEvent->GetCursor()->y);

					// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
					// 색상 파라미터를 EColor.TRANSPARENCY 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다.
					// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
					//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
					// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
					//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
					layer.DrawTextCanvas(CFLPoint<double>(10, 10), flsPosition, RED, BLACK);

					// 이미지뷰를 갱신
					m_pViewImage->Invalidate();
				}
				break;

			case EGUIBroadcast_ViewImage_PostLButtonUp:
				{
					// GetCaller() 가 등록한 이미지뷰인지 확인
					if(pMessage->GetCaller() != (const CFLBase*)m_pViewImage->GetMessageCallerPtr())
						break;

					// pMessage 객체를 CBroadcastMessage_GUI_ViewImage_MouseEvent 로 캐스팅
					CBroadcastMessage_GUI_ViewImage_MouseEvent* pMsgMouseEvent = dynamic_cast<CBroadcastMessage_GUI_ViewImage_MouseEvent*>((CBroadcastMessage*)pMessage);

					// pMsgMouseEvent 가 nullptr 인지 확인
					if(pMsgMouseEvent == nullptr)
						break;

					// 이미지뷰의 0번 레이어 가져오기
					CGUIViewImageLayerWrap layer = m_pViewImage->GetLayer(0);

					// 기존에 Layer 에 그려진 도형들을 삭제
					layer.Clear();

					// 마우스 좌표를 표시할 문자열 생성
					CFLString<wchar_t> flsPosition = L"";
					flsPosition.Format(L"LButtonUp X: %d, Y: %d", pMsgMouseEvent->GetCursor()->x, pMsgMouseEvent->GetCursor()->y);

					// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
					// 색상 파라미터를 EColor.TRANSPARENCY 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다.
					// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
					//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
					// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
					//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
					layer.DrawTextCanvas(CFLPoint<double>(10, 10), flsPosition, BLUE, BLACK);

					// 이미지뷰를 갱신
					m_pViewImage->Invalidate();
				}
				break;
			}
		}
		while(false);
	}

	protected:
		CGUIViewImageWrap* m_pViewImage;
};

int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[2];

	// 메세지를 전달 받을 CMessageReceiver 객체 생성 // Create 메세지를 전달 받을 CMessageReceiver object
	CMessageReceiver msgReceiver(&viewImage[0]);

	CResult eResult = EResult_UnknownError;

	do
	{
		// 이미지 뷰 생성 // Create image view
		if(IsFail(eResult = viewImage[0].Create(300, 0, 300 + 520, 430)))
		{
			ErrorPrint(eResult,"Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(eResult = viewImage[1].Create(300 + 520, 0, 300 + 520 * 2, 430)))
		{
			ErrorPrint(eResult,"Failed to create the image view.\n");
			break;
		}

		// 뷰의 시점을 동기화 한다
		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(eResult,"Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(eResult,"Failed to synchronize window\n");
			break;
		}

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage[0].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}