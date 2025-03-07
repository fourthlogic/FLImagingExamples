#pragma once
#include "TreeCtrlExampleDlg.h"

namespace FLImaging
{
	namespace GUI
	{
		class CGUITreeCtrl;

		// 프로퍼티 메뉴 클래스
		class CPropertyMenuLight : public CGUIMenuItemBase
		{
			SupportToDuplicateObject(CPropertyMenuLight);

		public:
			CPropertyMenuLight();
			virtual ~CPropertyMenuLight();

			// 부모 트리 설정
			virtual void SetParentTree(CGUITreeCtrl* pTree);
			// 부모 다이얼로그 설정
			virtual void SetParentDialog(CTreeCtrlExampleDlg* m_pParentDlg);

			// Tree Item 의 LButtonDoubleClick 메시지 처리기
			virtual bool OnLButtonDoubleClick() override;
			// Tree Item 의 RButtonClick 메시지 처리기
			virtual bool OnRButtonClick()override;

		protected:
			CGUITreeCtrl* m_pTree;
			CTreeCtrlExampleDlg* m_pParentDlg;
		};
	}
}

