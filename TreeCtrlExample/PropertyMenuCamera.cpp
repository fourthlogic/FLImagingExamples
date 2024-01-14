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
					// ���� Ʈ������ ���õ� ������
					HTREEITEM hItemThis = m_pTree->GetSelectedItem();

					// �ڽ� �� ���ϱ�
					int32_t i32ChildCount = 0;
					{
						// ���õ� �������� ù ��° �ڽ� ������
						HTREEITEM hChild = m_pTree->GetChildItem(hItemThis);

						while(hChild)
						{
							hChild = m_pTree->GetNextSiblingItem(hChild);
							i32ChildCount++;
						}
					}

					// �� ������ �߰�
					{
						HTREEITEM hNewItem = m_pTree->InsertItem(
							CFLString<wchar_t>().Format(L"NewItem %d", i32ChildCount), // �� Ʈ�� ������ �̸�
							(int32_t)ETreeItemIcon_DocEmpty, // ������
							(int32_t)ETreeItemIcon_DocEmpty_Selected, // ���õ� ������ ������
							hItemThis // �� Ʈ�� �������� �θ� ������
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
					// Ʈ������ Ʈ�������� ����
					if(m_pTree)
						m_pTree->DeleteItem(m_pTree->GetSelectedItem());

					// ���� ��ü(this) �� CTreeCtrlExampleDlg �� m_vctItems ���� ���� �� ��ü �Ҹ�
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
