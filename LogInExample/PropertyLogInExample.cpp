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
		// ��ư�� �����ϱ� ���� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_None �̹Ƿ� ������ �� ��ư�� ������ �� �ֽ��ϴ�.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_None, EPermission_Modify_Execute);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"User1 (1.0)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(1.);
		// ��ư�� �����ϱ� ���� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_User �̹Ƿ� User ���� �̻��� ����ڰ� �� ��ư�� ������ �� �ֽ��ϴ�.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_User, EPermission_Modify_Execute);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Power User1 (1.0)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(1.);
		// ��ư�� �����ϱ� ���� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_PowerUser �̹Ƿ� PowerUser ���� �̻��� ����ڰ� �� ��ư�� ������ �� �ֽ��ϴ�.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_PowerUser, EPermission_Modify_Execute);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Administrator1 (1.0)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(1.);
		// ��ư�� �����ϱ� ���� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_Administrator �̹Ƿ� Administrator ���� �̻��� ����ڰ� �� ��ư�� ������ �� �ֽ��ϴ�.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Administrator, EPermission_Modify_Execute);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Developer1 (1.0)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(1.);
		// ��ư�� �����ϱ� ���� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_Developer �̹Ƿ� Developer ���� �̻��� ����ڰ� �� ��ư�� ������ �� �ֽ��ϴ�.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Developer, EPermission_Modify_Execute);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Root1 (1.0)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(1.);
		// ��ư�� �����ϱ� ���� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_Root �̹Ƿ� Root ���� �̻��� ����ڰ� �� ��ư�� ������ �� �ֽ��ϴ�.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Root, EPermission_Modify_Execute);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"None2 (0.3)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.3);
		// ��ư�� �� �� �ִ� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_None �̹Ƿ� ��� ����ڰ� �� ��ư�� �� �� �ֽ��ϴ�.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_None, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"User2 (0.3)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.3);
		// ��ư�� �� �� �ִ� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_User �̹Ƿ� User ���� �̻��� ����ڰ� �� ��ư�� �� �� �ֽ��ϴ�.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_User, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Power User2 (0.4)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.4);
		// ��ư�� �� �� �ִ� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_PowerUser �̹Ƿ� PowerUser ���� �̻��� ����ڰ� �� ��ư�� �� �� �ֽ��ϴ�.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_PowerUser, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Administrator2 (0.3)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.3);
		// ��ư�� �� �� �ִ� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_Administrator �̹Ƿ� Administrator ���� �̻��� ����ڰ� �� ��ư�� �� �� �ֽ��ϴ�.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Administrator, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Developer2 (0.3)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.3);
		// ��ư�� �� �� �ִ� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_Developer �̹Ƿ� Developer ���� �̻��� ����ڰ� �� ��ư�� �� �� �ֽ��ϴ�.
		pBtn->SetMinimalAuthorityLevel(EAuthorityLevel_Developer, EPermission_View);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Root2 (0.3)");
		pBtn->SetPropertyButtonClickProcedure(TestButton());
		pBtn->SetButtonWidthRatio(0.3);
		// ��ư�� �� �� �ִ� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_Root �̹Ƿ� Root ���� �̻��� ����ڰ� �� ��ư�� �� �� �ֽ��ϴ�.
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
			// Image View List �� ��� ������ �ּ� ������ �����մϴ�. 
			// EAuthorityLevel_None �̹Ƿ� ��� ����ڰ� �� Image View List �� ����� �� �ֽ��ϴ�.
			pIVL->SetMinimalAuthorityLevel(EAuthorityLevel_None, EPermission_Modify_Execute);
			AddItem(pIVL);

			pIVR = new CGUIPropertyItemViewImageFigure;
			pIVR->SetName(L"None Figure");
			pIVR->SetPath(L"Category");
			pIVR->SetViewImageList(pIVL);
			// Image View Figure �� �� �� �ִ� �ּ� ������ �����մϴ�. 
			// EAuthorityLevel_None �̹Ƿ� ��� ����ڰ� �� Image View Figure �� �� �� �ֽ��ϴ�.
			pIVR->SetMinimalAuthorityLevel(EAuthorityLevel_None, EPermission_View);
			AddItem(pIVR);

			pIVL = new CGUIPropertyItemViewImageList;
			pIVL->SetName(L"User View");
			pIVL->SetPath(L"Category");
			// Image View List �� ��� ������ �ּ� ������ �����մϴ�. 
			// EAuthorityLevel_User �̹Ƿ� Root ���� �̻��� ����ڰ� �� Image View List �� ����� �� �ֽ��ϴ�.
			pIVL->SetMinimalAuthorityLevel(EAuthorityLevel_User, EPermission_Modify_Execute);
			AddItem(pIVL);

			pIVR = new CGUIPropertyItemViewImageFigure;
			pIVR->SetName(L"User Figure");
			pIVR->SetPath(L"Category");
			pIVR->SetViewImageList(pIVL);
			// Image View Figure �� �� �� �ִ� �ּ� ������ �����մϴ�. 
			// EAuthorityLevel_User �̹Ƿ� User ���� �̻��� ����ڰ� �� Image View Figure �� �� �� �ֽ��ϴ�.
			// User ���� �̸��� ����ڴ� �� Image View Figure �� �� �� �����ϴ�.
			pIVR->SetMinimalAuthorityLevel(EAuthorityLevel_User, EPermission_View);
			AddItem(pIVR);

			pIVL = new CGUIPropertyItemViewImageList;
			pIVL->SetName(L"Power User View");
			pIVL->SetPath(L"Category");
			// Image View List �� ��� ������ �ּ� ������ �����մϴ�. 
			// EAuthorityLevel_PowerUser �̹Ƿ� PowerUser ���� �̻��� ����ڰ� �� Image View List �� ����� �� �ֽ��ϴ�.
			pIVL->SetMinimalAuthorityLevel(EAuthorityLevel_PowerUser, EPermission_Modify_Execute);
			AddItem(pIVL);

			pIVR = new CGUIPropertyItemViewImageFigure;
			pIVR->SetName(L"Power User Figure");
			pIVR->SetPath(L"Category");
			pIVR->SetViewImageList(pIVL);
			// Image View Figure �� �� �� �ִ� �ּ� ������ �����մϴ�. 
			// EAuthorityLevel_PowerUser �̹Ƿ� PowerUser ���� �̻��� ����ڰ� �� Image View Figure �� �� �� �ֽ��ϴ�.
			// PowerUser ���� �̸��� ����ڴ� �� Image View Figure �� �� �� �����ϴ�.
			pIVR->SetMinimalAuthorityLevel(EAuthorityLevel_PowerUser, EPermission_View);
			AddItem(pIVR);

			pIVL = new CGUIPropertyItemViewImageList;
			pIVL->SetName(L"Administrator View");
			pIVL->SetPath(L"Category");
			// Image View List �� ��� ������ �ּ� ������ �����մϴ�. 
			// EAuthorityLevel_Administrator �̹Ƿ� Administrator ���� �̻��� ����ڰ� �� Image View List �� ����� �� �ֽ��ϴ�.
			pIVL->SetMinimalAuthorityLevel(EAuthorityLevel_Administrator, EPermission_Modify_Execute);
			AddItem(pIVL);

			pIVR = new CGUIPropertyItemViewImageFigure;
			pIVR->SetName(L"Administrator Figure");
			pIVR->SetPath(L"Category");
			pIVR->SetViewImageList(pIVL);
			// Image View Figure �� �� �� �ִ� �ּ� ������ �����մϴ�. 
			// EAuthorityLevel_Administrator �̹Ƿ� Administrator ���� �̻��� ����ڰ� �� Image View Figure �� �� �� �ֽ��ϴ�.
			// Administrator ���� �̸��� ����ڴ� �� Image View Figure �� �� �� �����ϴ�.
			pIVR->SetMinimalAuthorityLevel(EAuthorityLevel_Administrator, EPermission_View);
			AddItem(pIVR);

			pIVL = new CGUIPropertyItemViewImageList;
			pIVL->SetName(L"Developer View");
			pIVL->SetPath(L"Category");
			// Image View List �� ��� ������ �ּ� ������ �����մϴ�. 
			// EAuthorityLevel_Developer �̹Ƿ� Developer ���� �̻��� ����ڰ� �� Image View List �� ����� �� �ֽ��ϴ�.
			pIVL->SetMinimalAuthorityLevel(EAuthorityLevel_Developer, EPermission_Modify_Execute);
			AddItem(pIVL);

			pIVR = new CGUIPropertyItemViewImageFigure;
			pIVR->SetName(L"Developer Figure");
			pIVR->SetPath(L"Category");
			pIVR->SetViewImageList(pIVL);
			// Image View Figure �� �� �� �ִ� �ּ� ������ �����մϴ�. 
			// EAuthorityLevel_Developer �̹Ƿ� Developer ���� �̻��� ����ڰ� �� Image View Figure �� �� �� �ֽ��ϴ�.
			// Developer ���� �̸��� ����ڴ� �� Image View Figure �� �� �� �����ϴ�.
			pIVR->SetMinimalAuthorityLevel(EAuthorityLevel_Developer, EPermission_View);
			AddItem(pIVR);

			pIVL = new CGUIPropertyItemViewImageList;
			pIVL->SetName(L"Root View");
			pIVL->SetPath(L"Category");
			// Image View List �� ��� ������ �ּ� ������ �����մϴ�. 
			// EAuthorityLevel_Root �̹Ƿ� Root ���� �̻��� ����ڰ� �� Image View List �� ����� �� �ֽ��ϴ�.
			pIVL->SetMinimalAuthorityLevel(EAuthorityLevel_Root, EPermission_Modify_Execute);
			AddItem(pIVL);

			pIVR = new CGUIPropertyItemViewImageFigure;
			pIVR->SetName(L"Root Figure");
			pIVR->SetPath(L"Category");
			pIVR->SetViewImageList(pIVL);
			// Image View Figure �� �� �� �ִ� �ּ� ������ �����մϴ�. 
			// EAuthorityLevel_Root �̹Ƿ� Root ���� �̻��� ����ڰ� �� Image View Figure �� �� �� �ֽ��ϴ�.
			// Root ���� �̸��� ����ڴ� �� Image View Figure �� �� �� �����ϴ�.
			pIVR->SetMinimalAuthorityLevel(EAuthorityLevel_Root, EPermission_View);
			AddItem(pIVR);
		}

		pCat = new CGUIPropertyItemCategory;
		pCat->SetName(L"View Category");
		// Category �� ���� �����۵��� �� �� �ִ� �ּ� ������ �����մϴ�. 
		// EAuthorityLevel_Administrator �̹Ƿ� Administrator ���� �̻��� ����ڰ� �� Category �� ���� �����۵��� �� �� �ֽ��ϴ�.
		// Administrator ���� �̸��� ����ڴ� �� Category �� ���� �����۵��� �� �� �����ϴ�.
		pCat->SetMinimalAuthorityLevel(EAuthorityLevel_Administrator, EPermission_View);
		AddItem(pCat);
		{
			CGUIPropertyItemText* pText = nullptr;

			pText = new CGUIPropertyItemText;
			pText->SetName(L"User Text");
			pText->SetPath(L"View Category");
			// �� Text �������� �� �� �ִ� �ּ� ������ EAuthorityLevel_User ������, 
			// user �� �α��� ���� �� �� �� �����ϴ�.
			// �� Text �������� ���� ī�װ��� �� �� �ִ� �ּ� ������ EAuthorityLevel_Administrator �̹Ƿ�
			// user �� �α����ϸ� �θ� ī�װ��� invisible ���°� �Ǿ�
			// �� �������� �� �� �����ϴ�.
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