#include "stdafx.h"
#include "PropertyMenuBarMenuItem.h"

using namespace FLImaging;
using namespace FLImaging::Base;
using namespace FLImaging::GUI;
using namespace FLImaging::Foundation;

CPropertyMenuBarMenuItem::CPropertyMenuBarMenuItem()
{
}

CPropertyMenuBarMenuItem::~CPropertyMenuBarMenuItem()
{
}

const CResult CPropertyMenuBarMenuItem::ConfigureMenu()
{
	CResult cReturn = EResult_UnknownError;

	do
	{
		EnableStatusMessageBox(true);
		SetMessageDisplayMode(EMessageDisplayMode_AutoScroll);

		CGUIPropertyItemButton* pPIButton = nullptr;

		pPIButton = new CGUIPropertyItemButton;
		pPIButton->SetPath(L"");
		pPIButton->SetName(L"Add User Defined Menu Item");
		pPIButton->SetButtonName(L"Add");
		pPIButton->SetPropertyButtonClickProcedure(AddUserDefinedMenuItem());
		pPIButton->SetDescription(L"Adds a user-defined menu item under the specified parent path.\n지정된 부모 경로 아래에 사용자 정의 메뉴 아이템을 추가");
		AddItem(pPIButton);

		pPIButton = new CGUIPropertyItemButton;
		pPIButton->SetPath(L"");
		pPIButton->SetName(L"Remove User Defined Menu Item");
		pPIButton->SetButtonName(L"Remove");
		pPIButton->SetPropertyButtonClickProcedure(RemoveUserDefinedMenuItem());
		pPIButton->SetDescription(L"Removes a user-defined menu item.\n사용자 정의 메뉴 아이템을 제거");
		AddItem(pPIButton);

		cReturn = EResult_OK;
	}
	while(false);

	return cReturn;
}

CPropertyItemButtonClickProcedure* FLImaging::GUI::CPropertyMenuBarMenuItem::AddUserDefinedMenuItem()
{
	CPropertyItemButtonClickProcedure* pProcedure = new CPropertyItemButtonClickProcedure;

	*pProcedure = MakePropertyItemButtonClickProcedure
	{
		// 클릭하면 메시지박스를 띄우는 콜백 함수 만들기
		// Create a callback function that displays a message box when clicked.
		CPopupMenuItemUserDefinedCallback* pCallback = new CPopupMenuItemUserDefinedCallback;
		*pCallback = MakePopupMenuItemUserDefinedCallback
		{
			CGUIMessageBox::DoModal(L"New Menu Item");
		};

		// 상단 메뉴 바의 "Window" 위치에 메뉴 추가
		// Add a menu item under the "Window" section in the top menu bar.
		CGUIManagerMainFrameMenuBar::AddUserDefinedMenuItem(
			L"New Menu Item",  // 메뉴 아이템 이름 // Menu item name
			L"Window",  // 경로 // Path
			pCallback,  // 콜백 함수 포인터 // Pointer to the callback function
			true, // 콜백 함수 객체 자동 삭제 여부(default 값은 true) // Whether to automatically delete the callback function object (default: true)
			true, // Enable 상태로 메뉴 아이템 표시할지 여부 // Whether to display the menu item in enabled state
			false // Checked 상태로 메뉴 아이템 표시할지 여부 // Whether to display the menu item in checked state
		);
	};

	return pProcedure;
}

CPropertyItemButtonClickProcedure* FLImaging::GUI::CPropertyMenuBarMenuItem::RemoveUserDefinedMenuItem()
{
	CPropertyItemButtonClickProcedure* pProcedure = new CPropertyItemButtonClickProcedure;

	*pProcedure = MakePropertyItemButtonClickProcedure
	{
		// Removes a user-defined menu item.
		// 사용자 정의 메뉴 아이템을 제거
		CGUIManagerMainFrameMenuBar::RemoveUserDefinedMenuItem(
			CGUIManagerMainFrameMenuBar::GetUserDefinedMenuItem(
			L"New Menu Item",  // 메뉴 아이템 이름 // Menu item name
			L"Window"  // 경로 // Path
		));
	};

	return pProcedure;
}