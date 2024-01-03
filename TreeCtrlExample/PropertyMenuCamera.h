#pragma once
#include "TreeCtrlExampleDlg.h"

namespace FLImaging
{
	namespace GUI
	{
		class CGUITreeCtrl;

		// ������Ƽ �޴� Ŭ����
		class CPropertyMenuCamera : public CGUIMenuItemBase
		{
			SupportToDuplicateObject(CPropertyMenuCamera);

		public:
			CPropertyMenuCamera();
			virtual ~CPropertyMenuCamera();

			// �θ� Ʈ�� ����
			virtual void SetParentTree(CGUITreeCtrl* pTree);
			// �θ� ���̾�α� ����
			virtual void SetParentDialog(CTreeCtrlExampleDlg* pParentDlg);

			// Tree Item �� LButtonDoubleClick �޽��� ó����
			virtual bool OnLButtonDoubleClick() override;
			// Tree Item �� RButtonClick �޽��� ó����
			virtual bool OnRButtonClick()override;

		protected:
			CGUITreeCtrl* m_pTree;
			CTreeCtrlExampleDlg* m_pParentDlg;
		};
	}
}

