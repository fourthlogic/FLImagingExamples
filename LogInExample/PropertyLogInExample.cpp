#include "stdafx.h"
#include "PropertyLogInExample.h"

using namespace FLImaging;
using namespace FLImaging::Base;
using namespace FLImaging::Foundation;
using namespace FLImaging::GUI;

FLImaging::GUI::CPropertyLogInExample::CPropertyLogInExample()
	: CGUIPropertyMenuBase()
{
}

FLImaging::GUI::CPropertyLogInExample::~CPropertyLogInExample()
{
}

const CResult FLImaging::GUI::CPropertyLogInExample::ConfigureMenu()
{
	CResult cReturn = EResult_UnknownError;

	do
	{
		CGUIPropertyButton* pBtn = nullptr;

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"None1 (1.0)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(1.);
		// 버튼을 실행하기 위한 최소 권한을 설정합니다. 
		// EAuthorityLevel_None 이므로 누구나 이 버튼을 실행할 수 있습니다.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_None, EPermission_Modify_Execute);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"User1 (1.0)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(1.);
		// 버튼을 실행하기 위한 최소 권한을 설정합니다. 
		// EAuthorityLevel_User 이므로 User 레벨 이상인 사용자가 이 버튼을 실행할 수 있습니다.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_User, EPermission_Modify_Execute);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Power User1 (1.0)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(1.);
		// 버튼을 실행하기 위한 최소 권한을 설정합니다. 
		// EAuthorityLevel_PowerUser 이므로 PowerUser 레벨 이상인 사용자가 이 버튼을 실행할 수 있습니다.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_PowerUser, EPermission_Modify_Execute);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Administrator1 (1.0)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(1.);
		// 버튼을 실행하기 위한 최소 권한을 설정합니다. 
		// EAuthorityLevel_Administrator 이므로 Administrator 레벨 이상인 사용자가 이 버튼을 실행할 수 있습니다.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Administrator, EPermission_Modify_Execute);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Developer1 (1.0)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(1.);
		// 버튼을 실행하기 위한 최소 권한을 설정합니다. 
		// EAuthorityLevel_Developer 이므로 Developer 레벨 이상인 사용자가 이 버튼을 실행할 수 있습니다.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Developer, EPermission_Modify_Execute);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Root1 (1.0)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(1.);
		// 버튼을 실행하기 위한 최소 권한을 설정합니다. 
		// EAuthorityLevel_Root 이므로 Root 레벨 이상인 사용자가 이 버튼을 실행할 수 있습니다.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Root, EPermission_Modify_Execute);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"None2 (0.3)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.3);
		// 버튼을 볼 수 있는 최소 권한을 설정합니다. 
		// EAuthorityLevel_None 이므로 모든 사용자가 이 버튼을 볼 수 있습니다.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_None, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"User2 (0.3)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.3);
		// 버튼을 볼 수 있는 최소 권한을 설정합니다. 
		// EAuthorityLevel_User 이므로 User 레벨 이상인 사용자가 이 버튼을 볼 수 있습니다.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_User, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Power User2 (0.4)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.4);
		// 버튼을 볼 수 있는 최소 권한을 설정합니다. 
		// EAuthorityLevel_PowerUser 이므로 PowerUser 레벨 이상인 사용자가 이 버튼을 볼 수 있습니다.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_PowerUser, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Administrator2 (0.3)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.3);
		// 버튼을 볼 수 있는 최소 권한을 설정합니다. 
		// EAuthorityLevel_Administrator 이므로 Administrator 레벨 이상인 사용자가 이 버튼을 볼 수 있습니다.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Administrator, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Developer2 (0.3)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.3);
		// 버튼을 볼 수 있는 최소 권한을 설정합니다. 
		// EAuthorityLevel_Developer 이므로 Developer 레벨 이상인 사용자가 이 버튼을 볼 수 있습니다.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Developer, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Root2 (0.3)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.3);
		// 버튼을 볼 수 있는 최소 권한을 설정합니다. 
		// EAuthorityLevel_Root 이므로 Root 레벨 이상인 사용자가 이 버튼을 볼 수 있습니다.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Root, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Root3 (0.2)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.2);
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Root, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Developer3 (0.2)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.2);
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Developer, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"User3 (0.2)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.2);
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_User, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Administrator3 (0.2)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.2);
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Administrator, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"None3 (0.8)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.8);
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_None, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Power User3 (0.2)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.2);
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_PowerUser, EPermission_View);
		AddButton(pBtn);

		CGUIPropertyItemViewImageList* pIVL = nullptr;
		CGUIPropertyItemViewImageFigure* pIVR = nullptr;

		CGUIPropertyItemCategory* pCat = new CGUIPropertyItemCategory;
		pCat->SetName(L"Category");
		AddItem(pCat);
		{
			pIVL = new CGUIPropertyItemViewImageList;
			pIVL->SetName(L"None View");
			pIVL->SetPath(L"Category");
			// Image View List 를 사용 가능한 최소 권한을 설정합니다. 
			// EAuthorityLevel_None 이므로 모든 사용자가 이 Image View List 를 사용할 수 있습니다.
			pIVL->SetMinimalAuthorityLevel(EAuthorityLevel_None, EPermission_Modify_Execute);
			AddItem(pIVL);

			pIVR = new CGUIPropertyItemViewImageFigure;
			pIVR->SetName(L"None Figure");
			pIVR->SetPath(L"Category");
			pIVR->SetViewImageList(pIVL);
			// Image View Figure 를 볼 수 있는 최소 권한을 설정합니다. 
			// EAuthorityLevel_None 이므로 모든 사용자가 이 Image View Figure 를 볼 수 있습니다.
			pIVR->SetMinimalAuthorityLevel(EAuthorityLevel_None, EPermission_View);
			AddItem(pIVR);

			pIVL = new CGUIPropertyItemViewImageList;
			pIVL->SetName(L"User View");
			pIVL->SetPath(L"Category");
			// Image View List 를 사용 가능한 최소 권한을 설정합니다. 
			// EAuthorityLevel_User 이므로 Root 레벨 이상인 사용자가 이 Image View List 를 사용할 수 있습니다.
			pIVL->SetMinimalAuthorityLevel(EAuthorityLevel_User, EPermission_Modify_Execute);
			AddItem(pIVL);

			pIVR = new CGUIPropertyItemViewImageFigure;
			pIVR->SetName(L"User Figure");
			pIVR->SetPath(L"Category");
			pIVR->SetViewImageList(pIVL);
			// Image View Figure 를 볼 수 있는 최소 권한을 설정합니다. 
			// EAuthorityLevel_User 이므로 User 레벨 이상인 사용자가 이 Image View Figure 를 볼 수 있습니다.
			// User 레벨 미만인 사용자는 이 Image View Figure 를 볼 수 없습니다.
			pIVR->SetMinimalAuthorityLevel(EAuthorityLevel_User, EPermission_View);
			AddItem(pIVR);

			pIVL = new CGUIPropertyItemViewImageList;
			pIVL->SetName(L"Power User View");
			pIVL->SetPath(L"Category");
			// Image View List 를 사용 가능한 최소 권한을 설정합니다. 
			// EAuthorityLevel_PowerUser 이므로 PowerUser 레벨 이상인 사용자가 이 Image View List 를 사용할 수 있습니다.
			pIVL->SetMinimalAuthorityLevel(EAuthorityLevel_PowerUser, EPermission_Modify_Execute);
			AddItem(pIVL);

			pIVR = new CGUIPropertyItemViewImageFigure;
			pIVR->SetName(L"Power User Figure");
			pIVR->SetPath(L"Category");
			pIVR->SetViewImageList(pIVL);
			// Image View Figure 를 볼 수 있는 최소 권한을 설정합니다. 
			// EAuthorityLevel_PowerUser 이므로 PowerUser 레벨 이상인 사용자가 이 Image View Figure 를 볼 수 있습니다.
			// PowerUser 레벨 미만인 사용자는 이 Image View Figure 를 볼 수 없습니다.
			pIVR->SetMinimalAuthorityLevel(EAuthorityLevel_PowerUser, EPermission_View);
			AddItem(pIVR);

			pIVL = new CGUIPropertyItemViewImageList;
			pIVL->SetName(L"Administrator View");
			pIVL->SetPath(L"Category");
			// Image View List 를 사용 가능한 최소 권한을 설정합니다. 
			// EAuthorityLevel_Administrator 이므로 Administrator 레벨 이상인 사용자가 이 Image View List 를 사용할 수 있습니다.
			pIVL->SetMinimalAuthorityLevel(EAuthorityLevel_Administrator, EPermission_Modify_Execute);
			AddItem(pIVL);

			pIVR = new CGUIPropertyItemViewImageFigure;
			pIVR->SetName(L"Administrator Figure");
			pIVR->SetPath(L"Category");
			pIVR->SetViewImageList(pIVL);
			// Image View Figure 를 볼 수 있는 최소 권한을 설정합니다. 
			// EAuthorityLevel_Administrator 이므로 Administrator 레벨 이상인 사용자가 이 Image View Figure 를 볼 수 있습니다.
			// Administrator 레벨 미만인 사용자는 이 Image View Figure 를 볼 수 없습니다.
			pIVR->SetMinimalAuthorityLevel(EAuthorityLevel_Administrator, EPermission_View);
			AddItem(pIVR);

			pIVL = new CGUIPropertyItemViewImageList;
			pIVL->SetName(L"Developer View");
			pIVL->SetPath(L"Category");
			// Image View List 를 사용 가능한 최소 권한을 설정합니다. 
			// EAuthorityLevel_Developer 이므로 Developer 레벨 이상인 사용자가 이 Image View List 를 사용할 수 있습니다.
			pIVL->SetMinimalAuthorityLevel(EAuthorityLevel_Developer, EPermission_Modify_Execute);
			AddItem(pIVL);

			pIVR = new CGUIPropertyItemViewImageFigure;
			pIVR->SetName(L"Developer Figure");
			pIVR->SetPath(L"Category");
			pIVR->SetViewImageList(pIVL);
			// Image View Figure 를 볼 수 있는 최소 권한을 설정합니다. 
			// EAuthorityLevel_Developer 이므로 Developer 레벨 이상인 사용자가 이 Image View Figure 를 볼 수 있습니다.
			// Developer 레벨 미만인 사용자는 이 Image View Figure 를 볼 수 없습니다.
			pIVR->SetMinimalAuthorityLevel(EAuthorityLevel_Developer, EPermission_View);
			AddItem(pIVR);

			pIVL = new CGUIPropertyItemViewImageList;
			pIVL->SetName(L"Root View");
			pIVL->SetPath(L"Category");
			// Image View List 를 사용 가능한 최소 권한을 설정합니다. 
			// EAuthorityLevel_Root 이므로 Root 레벨 이상인 사용자가 이 Image View List 를 사용할 수 있습니다.
			pIVL->SetMinimalAuthorityLevel(EAuthorityLevel_Root, EPermission_Modify_Execute);
			AddItem(pIVL);

			pIVR = new CGUIPropertyItemViewImageFigure;
			pIVR->SetName(L"Root Figure");
			pIVR->SetPath(L"Category");
			pIVR->SetViewImageList(pIVL);
			// Image View Figure 를 볼 수 있는 최소 권한을 설정합니다. 
			// EAuthorityLevel_Root 이므로 Root 레벨 이상인 사용자가 이 Image View Figure 를 볼 수 있습니다.
			// Root 레벨 미만인 사용자는 이 Image View Figure 를 볼 수 없습니다.
			pIVR->SetMinimalAuthorityLevel(EAuthorityLevel_Root, EPermission_View);
			AddItem(pIVR);
		}

		pCat = new CGUIPropertyItemCategory;
		pCat->SetName(L"View Category");
		// Category 및 하위 아이템들을 볼 수 있는 최소 권한을 설정합니다. 
		// EAuthorityLevel_Administrator 이므로 Administrator 레벨 이상인 사용자가 이 Category 및 하위 아이템들을 볼 수 있습니다.
		// Administrator 레벨 미만인 사용자는 이 Category 및 하위 아이템들을 볼 수 없습니다.
		pCat->SetMinimalAuthorityLevel(EAuthorityLevel_Administrator, EPermission_View);
		AddItem(pCat);
		{
			CGUIPropertyItemText* pText = nullptr;

			pText = new CGUIPropertyItemText;
			pText->SetName(L"User Text");
			pText->SetPath(L"View Category");
			// 이 Text 아이템을 볼 수 있는 최소 권한은 EAuthorityLevel_User 이지만, 
			// user 로 로그인 했을 때 볼 수 없습니다.
			// 이 Text 아이템의 상위 카테고리를 볼 수 있는 최소 권한이 EAuthorityLevel_Administrator 이므로
			// user 로 로그인하면 부모 카테고리가 invisible 상태가 되어
			// 이 아이템을 볼 수 없습니다.
			pText->SetMinimalAuthorityLevel(EAuthorityLevel_User, EPermission_View);
			AddItem(pText);

			pText = new CGUIPropertyItemText;
			pText->SetName(L"Power User Text");
			pText->SetPath(L"View Category");
			pText->SetMinimalAuthorityLevel(EAuthorityLevel_PowerUser, EPermission_View);
			AddItem(pText);

			pText = new CGUIPropertyItemText;
			pText->SetName(L"Administrator Text");
			pText->SetPath(L"View Category");
			pText->SetMinimalAuthorityLevel(EAuthorityLevel_Administrator, EPermission_View);
			AddItem(pText);

			pText = new CGUIPropertyItemText;
			pText->SetName(L"Developer Text");
			pText->SetPath(L"View Category");
			pText->SetMinimalAuthorityLevel(EAuthorityLevel_Developer, EPermission_View);
			AddItem(pText);

			pText = new CGUIPropertyItemText;
			pText->SetName(L"Root Text");
			pText->SetPath(L"View Category");
			pText->SetMinimalAuthorityLevel(EAuthorityLevel_Root, EPermission_View);
			AddItem(pText);
		}

		cReturn = EResult_OK;
	}
	while(false);

	return cReturn;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyLogInExample::TestButton()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
	};

	return pProcedure;
}