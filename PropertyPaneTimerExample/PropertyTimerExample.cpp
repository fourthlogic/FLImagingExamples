#include "stdafx.h"
#include "PropertyTimerExample.h"

using namespace FLImaging;
using namespace FLImaging::Base;
using namespace FLImaging::GUI;
using namespace FLImaging::Foundation;

CPropertyTimerExample::CPropertyTimerExample()
{
}

CPropertyTimerExample::~CPropertyTimerExample()
{
}

const CResult CPropertyTimerExample::ConfigureMenu()
{
	CResult cReturn = EResult_UnknownError;

	do
	{
		CGUIPropertyButton* pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Move Position");
		pBtn->SetButtonWidthRatio(1.);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Reset Position");
		pBtn->SetButtonWidthRatio(1.);
		CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
		*pProcedure = MakePropertyButtonClickProcedure
		{
			CGUIViewImage * pImageView = nullptr;

			if(CGUIPropertyItemViewImageList* pIVL = dynamic_cast<CGUIPropertyItemViewImageList*>(FindItemByFullPath(L"Image View")))
				pImageView = pIVL->GetSelectedViewImage();

			if(pImageView)
			{
				pImageView->GetLayer(0)->Clear();
				pImageView->Invalidate();
			}

			m_ptPos = CPoint(0,0);
		};
		pBtn->SetPropertyButtonClickProcedure(pProcedure);
		AddButton(pBtn);

		CGUIPropertyItemViewImageList* pIVL = new CGUIPropertyItemViewImageList;
		pIVL->SetName(L"Image View");
		pIVL->SetDefaultValue(L"[1]");
		AddItem(pIVL);

		cReturn = EResult_OK;
	}
	while(false);

	return cReturn;
}

void FLImaging::GUI::CPropertyTimerExample::OnReceiveBroadcast(const Base::CBroadcastMessage* pMessage)
{
	do
	{
		if(!pMessage)
			break;

		// CGUIButton 의 마우스 이벤트에 대한 브로드캐스트 메시지
		// A broadcast message of CGUIButton mouse event
		CBroadcastMessage_GUI_Button_MouseEvent* pMsgMouseEventButton = dynamic_cast<CBroadcastMessage_GUI_Button_MouseEvent*>((Base::CBroadcastMessage*)pMessage);

		switch(pMessage->GetChannel())
		{
			// 마우스 왼쪽 버튼을 눌렀을 경우 // On mouse left button down
		case EGUIBroadcast_GUIButton_PostLButtonDown:
			{
				if(!pMsgMouseEventButton)
					break;

				if(!GetPaneProperties())
					break;

				// 브로드캐스트 메시지를 보낸 버튼 객체
				// The button object who sent the broadcast message
				CGUIButton* pBtn = (CGUIButton*)pMsgMouseEventButton->GetCaller();

				if(pBtn && pBtn->GetParentPropertyButton() && pBtn->GetParentPropertyButton()->GetGUIMenuItemProperty() == this && pBtn->GetParentPropertyButton()->GetName() == L"Move Position")
				{
					// interval = 10 ms
					// 10 ms 마다 한 번씩 수행됨을 의미
					int64_t i64Interval = 10;

					// Define what will be done every 10 ms
					// 10 ms 마다 수행될 내용을 정의
					CPropertyTimerProcedure* pTimerFunc = new CPropertyTimerProcedure;
					*pTimerFunc = MakePropertyTimerProcedure
					{
						CGUIViewImage * pImageView = nullptr;

						if(CGUIPropertyItemViewImageList* pIVL = dynamic_cast<CGUIPropertyItemViewImageList*>(FindItemByFullPath(L"Image View")))
							pImageView = pIVL->GetSelectedViewImage();

						if(pImageView)
						{
							pImageView->GetLayer(0)->Clear();
							pImageView->GetLayer(0)->DrawFigureCanvas(CFLRect<double>(m_ptPos.x - 5, m_ptPos.y, m_ptPos.x + 5, m_ptPos.y + 10), LIGHTRED, 1, PINK);
							pImageView->Invalidate();
						}

						CLog::Write(L"x=%d, y=%d", m_ptPos.x, m_ptPos.y);
						m_ptPos.x += 2;
					};

					// 타이머 핸들러 추가 // Add timer handler
					// Received id of added timer as m_u32TimerID
					// 추가된 타이머의 id 를 m_u32TimerID 로 받아 옴
					GetPaneProperties()->SetTimerHandler(i64Interval, pTimerFunc, &m_u32TimerID);
				}
			}
			break;

			// 마우스 왼쪽 버튼에서 떼었을 경우 // On left button up
			// 마우스 위치가 버튼에서 벗어났을 경우 // On mouse leave
		case EGUIBroadcast_GUIButton_PostMouseLeave:
		case EGUIBroadcast_GUIButton_PostLButtonUp:
			{
				if(!pMsgMouseEventButton)
					break;

				if(!GetPaneProperties())
					break;

				// 브로드캐스트 메시지를 보낸 버튼 객체
				// The button object who sent the broadcast message
				CGUIButton* pBtn = (CGUIButton*)pMsgMouseEventButton->GetCaller();

				if(pBtn && pBtn->GetParentPropertyButton() && pBtn->GetParentPropertyButton()->GetGUIMenuItemProperty() == this && pBtn->GetParentPropertyButton()->GetName() == L"Move Position")
				{
					// Kill added timer m_u32TimerID
					// 추가된 타이머 m_u32TimerID 를 종료 및 제거
					GetPaneProperties()->KillTimerHandler(m_u32TimerID);
				}
			}
			break;
		default:
			break;
		}
	}
	while(false);
}