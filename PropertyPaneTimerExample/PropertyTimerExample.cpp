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

		// CGUIButton �� ���콺 �̺�Ʈ�� ���� ��ε�ĳ��Ʈ �޽���
		// A broadcast message of CGUIButton mouse event
		CBroadcastMessage_GUI_Button_MouseEvent* pMsgMouseEventButton = dynamic_cast<CBroadcastMessage_GUI_Button_MouseEvent*>((Base::CBroadcastMessage*)pMessage);

		switch(pMessage->GetChannel())
		{
			// ���콺 ���� ��ư�� ������ ��� // On mouse left button down
		case EGUIBroadcast_GUIButton_PostLButtonDown:
			{
				if(!pMsgMouseEventButton)
					break;

				if(!GetPaneProperties())
					break;

				// ��ε�ĳ��Ʈ �޽����� ���� ��ư ��ü
				// The button object who sent the broadcast message
				CGUIButton* pBtn = (CGUIButton*)pMsgMouseEventButton->GetCaller();

				if(pBtn && pBtn->GetParentPropertyButton() && pBtn->GetParentPropertyButton()->GetGUIMenuItemProperty() == this && pBtn->GetParentPropertyButton()->GetName() == L"Move Position")
				{
					// interval = 10 ms
					// 10 ms ���� �� ���� ������� �ǹ�
					int64_t i64Interval = 10;

					// Define what will be done every 10 ms
					// 10 ms ���� ����� ������ ����
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

					// Ÿ�̸� �ڵ鷯 �߰� // Add timer handler
					// Received id of added timer as m_u32TimerID
					// �߰��� Ÿ�̸��� id �� m_u32TimerID �� �޾� ��
					GetPaneProperties()->SetTimerHandler(i64Interval, pTimerFunc, &m_u32TimerID);
				}
			}
			break;

			// ���콺 ���� ��ư���� ������ ��� // On left button up
			// ���콺 ��ġ�� ��ư���� ����� ��� // On mouse leave
		case EGUIBroadcast_GUIButton_PostMouseLeave:
		case EGUIBroadcast_GUIButton_PostLButtonUp:
			{
				if(!pMsgMouseEventButton)
					break;

				if(!GetPaneProperties())
					break;

				// ��ε�ĳ��Ʈ �޽����� ���� ��ư ��ü
				// The button object who sent the broadcast message
				CGUIButton* pBtn = (CGUIButton*)pMsgMouseEventButton->GetCaller();

				if(pBtn && pBtn->GetParentPropertyButton() && pBtn->GetParentPropertyButton()->GetGUIMenuItemProperty() == this && pBtn->GetParentPropertyButton()->GetName() == L"Move Position")
				{
					// Kill added timer m_u32TimerID
					// �߰��� Ÿ�̸� m_u32TimerID �� ���� �� ����
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