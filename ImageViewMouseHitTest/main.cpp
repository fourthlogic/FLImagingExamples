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

			// GetCaller() 가 등록한 이미지뷰인지 확인
 			if(pMessage->GetCaller() != (const CFLBase*)m_pViewImage->GetMessageCallerPtr())
 				break;

			// 메세지의 채널을 확인
			switch(pMessage->GetChannel())
			{
			case EGUIBroadcast_ViewImage_PostMouseMove:
				{
					// pMessage 객체를 CBroadcastMessage_GUI_ViewImage_MouseEvent 로 캐스팅
					CBroadcastMessage_GUI_ViewImage_MouseEvent* pMsgMouseEvent = dynamic_cast<CBroadcastMessage_GUI_ViewImage_MouseEvent*>((CBroadcastMessage*)pMessage);

					// pMsgMouseEvent 가 nullptr 인지 확인
					if(pMsgMouseEvent == nullptr)
						break;

					// 이미지뷰의 0번 레이어 가져오기
					CGUIViewImageLayerWrap layer = m_pViewImage->GetLayer(0);

					// 기존에 Layer 에 그려진 도형들을 삭제
					layer.Clear();

					// 현재 마우스가 위치한 영역을 표시할 문자열 생성
					CFLString<wchar_t> strHitArea = L"";
					CFLString<wchar_t> str = L"";
					
					// 현재 마우스가 위치한 영역을 얻어 옵니다.
					EGUIViewImageHitArea eHitArea = m_pViewImage->GetHitArea();

					if(eHitArea == EGUIViewImageHitArea_None)
						strHitArea = "None";
					else
					{
						strHitArea = "Mouse is ";
						str = "on ";

						if(eHitArea & (EGUIViewImageHitArea_MiniMap))
						{
							strHitArea += str + "MiniMap";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_MiniMapDisplayingArea))
						{
							strHitArea += str + "MiniMapDisplayingArea";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_ThumbnailView))
						{
							strHitArea += str + "ThumbnailView";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_ThumbnailViewTop))
						{
							strHitArea += str + "ThumbnailViewTop";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_Figure))
						{
							strHitArea += str + "Figure";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_MultiFigures))
						{
							strHitArea += str + "MultiFigures";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_ImageFigure))
						{
							strHitArea += str + "ImageROI";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_StatusBar))
						{
							strHitArea += str + "StatusBar";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_PageIndex))
						{
							strHitArea += str + "PageIndex";
							str = "and ";
						}
						if(eHitArea & (EGUIViewImageHitArea_PrevPageArrow))
						{
							strHitArea += str + "PrevPageArrow";
							str = "and ";
						}
						if(eHitArea & (EGUIViewImageHitArea_NextPageArrow))
						{
							strHitArea += str + "NextPageArrow";
							str = "and ";
						}
						if(eHitArea & (EGUIViewImageHitArea_Measurement))
						{
							strHitArea += str + "Measurement";
							str = "and ";
						}
						if(eHitArea & (EGUIViewImageHitArea_MultiMeasurements))
						{
							strHitArea += str + "MultiMeasurements";
							str = "and ";
						}
						strHitArea += ".";
					}

					// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
					// 색상 파라미터를 EColor.TRANSPARENCY 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다.
					// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
					//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
					// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
					//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
					layer.DrawTextCanvas(CFLPoint<double>(80, 10), strHitArea, LIME, BLACK);

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
	// 이미지 객체 선언 // Declare image object
	CFLImage fliImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage;

	CMessageReceiver msgReceiver(&viewImage);

	CResult res = EResult_UnknownError;

	do
	{
		// 이미지 로드 // Load image
		if(IsFail(res =fliImage.Load(L"../../ExampleImages/PagePooling/Multiple File_Min.flif")))
		{
			ErrorPrint(res,"Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage.Create(300, 0, 300 + 520, 430)))
		{
			ErrorPrint(res,"Failed to create the image view.\n");
			break;
		}
		
		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		if(IsFail(res =viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res,"Failed to set image object on the image view.\n");
			break;
		}

		// Zoom fit 을 통해 디스플레이 되는 이미지 배율을 화면에 맞춰준다.
		if(IsFail(res =viewImage.ZoomFit()))
		{
			ErrorPrint(res,"Failed to zoom fit of the image view.\n");
			break;
		}

		// 이미지 뷰의 캔버스 영역을 얻어온다.
		CFLRect<int32_t> flrlCanvas = viewImage.GetClientRectCanvasRegion();

		// 캔버스 영역의 좌표계를 이미지 영역의 좌표계로 변환한다.
		CFLRect<double> flrdImage = viewImage.ConvertCanvasCoordToImageCoord(flrlCanvas);

		// 이미지 영역을 기준으로 생성될 Figure 의 크기와 모양을 사각형으로 설정한다.
		double f64Width = flrdImage.GetWidth() / 10.;
		double f64Height = flrdImage.GetHeight() / 10.;
		double f64Size = __min(f64Width, f64Height);

		CFLPoint<double> flpdCenter(0, 0);
		flrdImage.GetCenter(flpdCenter);

		CFLRect<double> flrdFigureShape(flpdCenter.x - f64Size, flpdCenter.y - f64Size, flpdCenter.x + f64Size, flpdCenter.y + f64Size);

		// 이미지 뷰에 Figure object 를 생성한다.
		// 가장 마지막 파라미터는 활성화 되는 메뉴의 구성이며, EAvailableFigureContextMenu_All 가 기본 메뉴를 활성화 한다.
		// 활성화 하고자 하는 메뉴를 추가 혹은 제거 하기 위해서는 enum 값을 비트 연산으로 넣어주면 된다.
		// ex) EAvailableFigureContextMenu_None -> 활성화 되는 메뉴 없음
		//     EAvailableFigureContextMenu_All -> 전체 메뉴 활성화
		//     EAvailableFigureContextMenu_DeclType | EAvailableFigureContextMenu_TemplateType -> Decl Type, Template Type 변환 메뉴 활성화
		viewImage.PushBackFigureObject(&flrdFigureShape, EAvailableFigureContextMenu_All);

		// 이미지 뷰를 갱신 // Update image view
		viewImage.Invalidate(true);

		// 다중 페이지 이미지의 썸네일 미리보기 뷰를 고정
		viewImage.SetFixThumbnailView(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}