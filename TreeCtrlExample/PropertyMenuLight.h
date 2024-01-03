#pragma once
#include "TreeCtrlExampleDlg.h"

namespace FLImaging
{
	namespace GUI
	{
		class CGUITreeCtrl;

		// ������Ƽ �޴� Ŭ����
		class CPropertyMenuLight : public CGUIMenuItemBase
		{
			SupportToDuplicateObject(CPropertyMenuLight);

		public:
			CPropertyMenuLight();
			virtual ~CPropertyMenuLight();

			// �θ� Ʈ�� ����
			virtual void SetParentTree(CGUITreeCtrl* pTree);
			// �θ� ���̾�α� ����
			virtual void SetParentDialog(CTreeCtrlExampleDlg* m_pParentDlg);

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

