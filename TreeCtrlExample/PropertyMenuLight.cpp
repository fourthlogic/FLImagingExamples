#include "stdafx.h"
#include "PropertyMenuLight.h"

FLImaging::GUI::CPropertyMenuLight::CPropertyMenuLight() : CGUIMenuItemBase()
{
}

FLImaging::GUI::CPropertyMenuLight::~CPropertyMenuLight()
{
}

void FLImaging::GUI::CPropertyMenuLight::SetParentTree(CGUITreeCtrl* pTree)
{
	m_pTree = pTree;
}

void FLImaging::GUI::CPropertyMenuLight::SetParentDialog(CTreeCtrlExampleDlg* pParentDlg)
{
	m_pParentDlg = pParentDlg;
}

bool FLImaging::GUI::CPropertyMenuLight::OnLButtonDoubleClick()
{
	bool bReturn = false;

	do
	{
		AfxMessageBox(L"[" + GetName() + L"] LButtonDoubleClick", MB_OKCANCEL);
	}
	while(false);

	return bReturn;
}

bool FLImaging::GUI::CPropertyMenuLight::OnRButtonClick()
{
	bool bReturn = false;

	do
	{
		CMenu popupMenu;
		popupMenu.CreatePopupMenu();

		enum ETreeMenuItem
		{
			ETreeMenuItem_Insert = 1,
			ETreeMenuItem_Remove,
			ETreeMenuItem_Move,
			ETreeMenuItem_Copy,
			ETreeMenuItem_Paste,
		};

		popupMenu.AppendMenu(MF_ENABLED | MF_STRING, ETreeMenuItem_Insert, L"Insert");
		popupMenu.AppendMenu(MF_ENABLED | MF_STRING, ETreeMenuItem_Remove, L"Remove");
		popupMenu.AppendMenu(MF_ENABLED | MF_STRING, ETreeMenuItem_Move, L"Move");
		popupMenu.AppendMenu(MF_SEPARATOR);
		popupMenu.AppendMenu(MF_ENABLED | MF_STRING, ETreeMenuItem_Copy, L"Copy");
		popupMenu.AppendMenu(MF_ENABLED | MF_STRING, ETreeMenuItem_Paste, L"Paste");

		CPoint ptPoint;
		GetCursorPos(&ptPoint);

		ETreeMenuItem eSelectedMenu = (ETreeMenuItem)popupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, ptPoint.x, ptPoint.y, m_pTree->GetParent());

		switch(eSelectedMenu)
		{
		case ETreeMenuItem_Insert:
			{
				// 현재 트리에서 선택된 아이템
				HTREEITEM hItemThis = m_pTree->GetSelectedItem();

				// 형제 수 구하기
				int32_t i32SiblingCount = 0;
				{
					HTREEITEM hChild = m_pTree->GetChildItem(m_pTree->GetParentItem(hItemThis));

					while(hChild)
					{
						hChild = m_pTree->GetNextSiblingItem(hChild);
						i32SiblingCount++;
					}
				}

				// 새 아이템 추가
				CPropertyMenuLight* pItem = new CPropertyMenuLight;
				pItem->SetParentTree(m_pTree);
				pItem->SetParentDialog(m_pParentDlg);
				// 새 트리 아이템 경로
				pItem->SetPath(GetPath()); 
				// 새 트리 아이템 이름
				pItem->SetName(CFLString<wchar_t>().Format(L"Light %d", i32SiblingCount)); 
				m_pParentDlg->AddMenuItem(pItem);

				HTREEITEM hNewItem = m_pTree->InsertItem(
					pItem->GetName(), // 새 트리 아이템 이름
					(int32_t)ETreeItemIcon_Light, // 아이콘
					(int32_t)ETreeItemIcon_Light_Selected, // 선택된 상태의 아이콘
					m_pTree->GetParentItem(hItemThis), // 새 트리 아이템의 부모 아이템
					hItemThis // 이 아이템 아래에 새 아이템을 삽입
				);

				if(hNewItem)
					m_pTree->SetItemData(hNewItem, (DWORD_PTR)pItem);
			}
			break;
		case ETreeMenuItem_Remove:
			{
				UINT u32Answer = AfxMessageBox(L"[" + GetName() + L"]\nDo you really want to remove this item?", MB_YESNO);

				if(u32Answer == IDYES)
				{
					// 트리에서 트리아이템 제거
					if(m_pTree)
						m_pTree->DeleteItem(m_pTree->GetSelectedItem());

					// 현재 객체(this) 를 CTreeCtrlExampleDlg 의 m_vctItems 에서 제거 및 객체 소멸
					if(m_pParentDlg)
						m_pParentDlg->PostMessage(ETreeCtrlExampleMessage_DeleteMenuItem, 0, (LPARAM)this);
				}
			}
			break;
		case ETreeMenuItem_Move:
			AfxMessageBox(L"[" + GetName() + L"] Move", MB_OK);
			break;
		case ETreeMenuItem_Copy:
			AfxMessageBox(L"[" + GetName() + L"] Copy", MB_OK);
			break;
		case ETreeMenuItem_Paste:
			AfxMessageBox(L"[" + GetName() + L"] Paste", MB_OK);
			break;
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}
