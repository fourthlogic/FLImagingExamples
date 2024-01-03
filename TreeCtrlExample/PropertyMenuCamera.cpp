#include "stdafx.h"
#include "PropertyMenuCamera.h"

FLImaging::GUI::CPropertyMenuCamera::CPropertyMenuCamera() : CGUIMenuItemBase()
{
}

FLImaging::GUI::CPropertyMenuCamera::~CPropertyMenuCamera()
{
}

void FLImaging::GUI::CPropertyMenuCamera::SetParentTree(CGUITreeCtrl* pTree)
{
	m_pTree = pTree;
}

void FLImaging::GUI::CPropertyMenuCamera::SetParentDialog(CTreeCtrlExampleDlg* pParentDlg)
{
	m_pParentDlg = pParentDlg;
}

bool FLImaging::GUI::CPropertyMenuCamera::OnLButtonDoubleClick()
{
	bool bReturn = false;

	do
	{
		AfxMessageBox(L"[" + GetName() + L"] LButtonDoubleClick", MB_OKCANCEL);
	}
	while(false);

	return bReturn;
}

bool FLImaging::GUI::CPropertyMenuCamera::OnRButtonClick()
{
	bool bReturn = false;

	do
	{
		CMenu popupMenu;
		popupMenu.CreatePopupMenu();

		enum ETreeMenuItem
		{
			ETreeMenuItem_AddChild = 1,
			ETreeMenuItem_Remove,
		};

		popupMenu.AppendMenu(MF_ENABLED | MF_STRING, ETreeMenuItem_AddChild, L"Add Child");
		popupMenu.AppendMenu(MF_ENABLED | MF_STRING, ETreeMenuItem_Remove, L"Remove");

		CPoint ptPoint;
		GetCursorPos(&ptPoint);

		ETreeMenuItem eSelectedMenu = (ETreeMenuItem)popupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, ptPoint.x, ptPoint.y, m_pTree->GetParent());

		switch(eSelectedMenu)
		{
		case ETreeMenuItem_AddChild:
			{
				if(m_pTree)
				{
					// 현재 트리에서 선택된 아이템
					HTREEITEM hItemThis = m_pTree->GetSelectedItem();

					// 자식 수 구하기
					int32_t i32ChildCount = 0;
					{
						// 선택된 아이템의 첫 번째 자식 아이템
						HTREEITEM hChild = m_pTree->GetChildItem(hItemThis);

						while(hChild)
						{
							hChild = m_pTree->GetNextSiblingItem(hChild);
							i32ChildCount++;
						}
					}

					// 새 아이템 추가
					{
						HTREEITEM hNewItem = m_pTree->InsertItem(
							CFLString<wchar_t>().Format(L"NewItem %d", i32ChildCount), // 새 트리 아이템 이름
							(int32_t)ETreeItemIcon_DocEmpty, // 아이콘
							(int32_t)ETreeItemIcon_DocEmpty_Selected, // 선택된 상태의 아이콘
							hItemThis // 새 트리 아이템의 부모 아이템
						);

						m_pTree->RecursiveExpand(hItemThis, TVE_EXPAND);
					}
				}
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
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}
