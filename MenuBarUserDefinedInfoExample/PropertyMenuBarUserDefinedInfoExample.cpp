#include "stdafx.h"
#include "PropertyMenuBarUserDefinedInfoExample.h"

using namespace FLImaging;
using namespace FLImaging::Base;
using namespace FLImaging::GUI;
using namespace FLImaging::Foundation;

FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::CPropertyMenuBarUserDefinedInfoExample()
	: m_bRunning(false)
{
}

FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::~CPropertyMenuBarUserDefinedInfoExample()
{
}

const CResult FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::ConfigureMenu()
{
	CResult cReturn = EResult_UnknownError;

	do
	{
		CGUIPropertyButton* pBtn = nullptr;

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Initialize");
		pBtn->SetPropertyButtonClickProcedure(Initialize());
		pBtn->SetButtonWidthRatio(1.);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Run");
		pBtn->SetPropertyButtonClickProcedure(Run());
		pBtn->SetButtonWidthRatio(.5);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Stop");
		pBtn->SetPropertyButtonClickProcedure(Stop());
		pBtn->SetButtonWidthRatio(.5);
		pBtn->Enable(false);
		AddButton(pBtn);


		CGUIPropertyItemCategory* pCat = nullptr;

		pCat = new CGUIPropertyItemCategory;
		pCat->SetName(L"Add");
		AddItem(pCat);
		{
			CGUIPropertyItemButton* pPIButton = nullptr;
			CGUIPropertyItemDropdownList* pPIDD = nullptr;
			CPropertyCallback* pCallback = nullptr;

			pPIButton = new CGUIPropertyItemButton;
			pPIButton->SetPath(L"Add");
			pPIButton->SetName(L"Add");
			pPIButton->SetButtonName(L"Apply Add");
			pPIButton->SetPropertyButtonClickProcedure(Add());
			AddItem(pPIButton);

			pPIDD = new CGUIPropertyItemDropdownList;
			pPIDD->SetPath(L"Add");
			pPIDD->SetName(L"Position");
			pPIDD->AddListItem(L"PushBack");
			pPIDD->AddListItem(L"PushFront");
			pPIDD->AddListItem(L"InsertAt");
			pPIDD->SetDefaultValue(L"PushBack");
			pCallback = new CPropertyCallback;
			*pCallback = MakePropertyCallback
			{
				CGUIPropertyItemText * pPITextIndex = (CGUIPropertyItemText*)FindItemByFullPath(L"Add@Index");
				pPITextIndex->SetVisible(strValue == L"InsertAt");
				pPITextIndex->GetParentWndList()->AdjustLayout();
			};
			pPIDD->SetPropertyCallback(pCallback);
			AddItem(pPIDD);

			AddItemForUserDefinedInfo(L"Add");
		}

		pCat = new CGUIPropertyItemCategory;
		pCat->SetName(L"Remove");
		AddItem(pCat);
		{
			CGUIPropertyItemButton* pPIButton = nullptr;
			CGUIPropertyItemDropdownList* pPIDD = nullptr;
			CPropertyCallback* pCallback = nullptr;

			pPIButton = new CGUIPropertyItemButton;
			pPIButton->SetPath(L"Remove");
			pPIButton->SetName(L"Remove");
			pPIButton->SetButtonName(L"Apply Remove");
			pPIButton->SetPropertyButtonClickProcedure(Remove());
			AddItem(pPIButton);

			pPIDD = new CGUIPropertyItemDropdownList;
			pPIDD->SetPath(L"Remove");
			pPIDD->SetName(L"Position");
			pPIDD->AddListItem(L"PopBack");
			pPIDD->AddListItem(L"PopFront");
			pPIDD->AddListItem(L"RemoveAt");
			pPIDD->SetDefaultValue(L"PopBack");
			pCallback = new CPropertyCallback;
			*pCallback = MakePropertyCallback
			{
				CGUIPropertyItemText * pPITextIndex = (CGUIPropertyItemText*)FindItemByFullPath(L"Remove@Index");
				pPITextIndex->SetVisible(strValue == L"RemoveAt");
				pPITextIndex->GetParentWndList()->AdjustLayout();
			};
			pPIDD->SetPropertyCallback(pCallback);
			AddItem(pPIDD);

			AddItemForUserDefinedInfo(L"Remove");
		}

		pCat = new CGUIPropertyItemCategory;
		pCat->SetName(L"Set");
		AddItem(pCat);
		{
			CGUIPropertyItemButton* pPIButton = nullptr;
			CGUIPropertyItemText* pPITIndex = nullptr;
			CPropertyCallback* pCallback = nullptr;

			pPIButton = new CGUIPropertyItemButton;
			pPIButton->SetPath(L"Set");
			pPIButton->SetName(L"Set");
			pPIButton->SetButtonName(L"Apply Set");
			pPIButton->SetPropertyButtonClickProcedure(Set());
			AddItem(pPIButton);

			AddItemForUserDefinedInfo(L"Set");

			pPITIndex = (CGUIPropertyItemText*)FindItemByFullPath(L"Set@Index");
			pCallback = new CPropertyCallback;
			*pCallback = MakePropertyCallback
			{
				UpdateSetParameters(strValue.ToInt32());
			};
			pPITIndex->SetPropertyCallback(pCallback);
		}

		cReturn = EResult_OK;
	}
	while(false);

	return cReturn;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::Initialize()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		do
		{
			CGUIMainFrame* pMF = AfxGetApp() && AfxGetApp()->m_pMainWnd ? dynamic_cast<CGUIMainFrame*>(AfxGetApp()->m_pMainWnd) : nullptr;

			if(!pMF)
				break;

			CGUIMenuBar* pMenuBar = pMF->GetMenuBar();

			if(!pMenuBar)
				break;

			pMenuBar->ClearUserDefinedInfo();

			// ��ü ���� �� ���ڿ�, ���� ���� 
			// Creating an object and setting background color and string
			CGUIMenuBarUserDefinedInfo udi(L"Stopped", RGB(128, 0, 0));

			// Current model name �簢���� ���� �������� �ϳ��� �߰��˴ϴ�.
			// Object is added in the left direction of the current model name rectangle.
			pMenuBar->PushBackUserDefinedInfo(udi);

			pMenuBar->Invalidate();
		}
		while(false);
	};

	return pProcedure;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::Run()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		do
		{
			CGUIPropertyButton* pBtnRun = (CGUIPropertyButton*)FindPropertyButtonByName(L"Run");
			CGUIPropertyButton* pBtnStop = (CGUIPropertyButton*)FindPropertyButtonByName(L"Stop");

			pBtnRun->Enable(false);
			pBtnStop->Enable(true);

			CGUIMainFrame* pMF = AfxGetApp() && AfxGetApp()->m_pMainWnd ? dynamic_cast<CGUIMainFrame*>(AfxGetApp()->m_pMainWnd) : nullptr;

			if(!pMF)
				break;

			CGUIMenuBar* pMenuBar = pMF->GetMenuBar();

			if(!pMenuBar)
				break;

			// 0�� �ε����� �������� ��� ����
			// Get the item at 0
			CGUIMenuBarUserDefinedInfo* pUDI = pMenuBar->GetUserDefinedInfoAt(0);

			if(pUDI && pUDI->GetText() == L"Stopped")
			{
				// ���ڿ� ����
				// Modify text
				pUDI->SetText(L"Running..");

				// ��� ���� ����
				// Modify background color
				pUDI->SetBackgroundColor(RGB(0, 128, 0));
			}

			pMenuBar->Invalidate();
		}
		while(false);
	};

	return pProcedure;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::Stop()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		do
		{
			CGUIPropertyButton* pBtnRun = (CGUIPropertyButton*)FindPropertyButtonByName(L"Run");
			CGUIPropertyButton* pBtnStop = (CGUIPropertyButton*)FindPropertyButtonByName(L"Stop");

			pBtnRun->Enable(true);
			pBtnStop->Enable(false);

			CGUIMainFrame* pMF = AfxGetApp() && AfxGetApp()->m_pMainWnd ? dynamic_cast<CGUIMainFrame*>(AfxGetApp()->m_pMainWnd) : nullptr;

			if(!pMF)
				break;

			CGUIMenuBar* pMenuBar = pMF->GetMenuBar();

			if(!pMenuBar)
				break;

			// 0�� �ε����� �������� ��� ����
			// Get the item at 0
			CGUIMenuBarUserDefinedInfo* pUDI = pMenuBar->GetUserDefinedInfoAt(0);

			if(pUDI && pUDI->GetText() == L"Running..")
			{
				// ���ڿ� ����
				// Modify text
				pUDI->SetText(L"Stopped");

				// ��� ���� ����
				// Modify background color
				pUDI->SetBackgroundColor(RGB(128, 0, 0));
			}

			pMenuBar->Invalidate();
		}
		while(false);
	};

	return pProcedure;
}


CPropertyItemButtonClickProcedure* FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::Add()
{
	CPropertyItemButtonClickProcedure* pProcedure = new CPropertyItemButtonClickProcedure;

	*pProcedure = MakePropertyItemButtonClickProcedure
	{
		do
		{
			CGUIMainFrame* pMF = AfxGetApp() && AfxGetApp()->m_pMainWnd ? dynamic_cast<CGUIMainFrame*>(AfxGetApp()->m_pMainWnd) : nullptr;

			if(!pMF)
				break;

			CGUIMenuBar* pMenuBar = pMF->GetMenuBar();

			if(!pMenuBar)
				break;

			CGUIPropertyItemDropdownList* pPIDD = (CGUIPropertyItemDropdownList*)FindItemByFullPath(L"Add@Position");

			if(!pPIDD)
				break;

			Base::CFLString<wchar_t> str;
			COLORREF clrBackground;
			COLORREF clrText;
			bool bShow;
			EGUIAlignment eAlign;
			EGUIStringTrimming eStringTrimming;
			int32_t i32Index;

			// "Add" ī�װ� ���� ������Ƽ���� �Է��� �� ��� ����
			// Obtain parameters from the properties under "Add" category
			GetParameters(L"Add", i32Index, str, clrBackground, clrText, bShow, eAlign, eStringTrimming);

			// ���� ������ CGUIMenuBarUserDefinedInfo ��ü ����
			// Create CGUIMenuBarUserDefinedInfo object with obtained value
			CGUIMenuBarUserDefinedInfo udi(str, clrBackground, clrText, bShow, eAlign, eStringTrimming);

			CFLString<wchar_t> strPos = pPIDD->GetValue();

			if(strPos == L"PushBack")
				pMenuBar->PushBackUserDefinedInfo(udi); // �� �ڿ� �߰� // Add to the back
			else if(strPos == L"PushFront")
				pMenuBar->PushFrontUserDefinedInfo(udi); // �� �տ� �߰� // Add to the front
			else if(strPos == L"InsertAt")
				pMenuBar->InsertUserDefinedInfoAt(i32Index, udi); // ������ �ε��� ��ġ�� ���� // Insert in the index you set

			pMenuBar->Invalidate();
		}
		while(false);
	};

	return pProcedure;
}

CPropertyItemButtonClickProcedure* FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::Remove()
{
	CPropertyItemButtonClickProcedure* pProcedure = new CPropertyItemButtonClickProcedure;

	*pProcedure = MakePropertyItemButtonClickProcedure
	{
		do
		{
			CGUIMainFrame* pMF = AfxGetApp() && AfxGetApp()->m_pMainWnd ? dynamic_cast<CGUIMainFrame*>(AfxGetApp()->m_pMainWnd) : nullptr;

			if(!pMF)
				break;

			CGUIMenuBar* pMenuBar = pMF->GetMenuBar();

			if(!pMenuBar)
				break;

			CGUIPropertyItemDropdownList* pPIDD = (CGUIPropertyItemDropdownList*)FindItemByFullPath(L"Remove@Position");

			if(!pPIDD)
				break;

			Base::CFLString<wchar_t> str;
			COLORREF clrBackground;
			COLORREF clrText;
			bool bShow;
			EGUIAlignment eAlign;
			EGUIStringTrimming eStringTrimming;
			int32_t i32Index;

			// "Remove" ī�װ� ���� ������Ƽ���� �Է��� �� ��� ����
			// Obtain parameters from the properties under "Remove" category
			GetParameters(L"Remove", i32Index, str, clrBackground, clrText, bShow, eAlign, eStringTrimming);

			CFLString<wchar_t> strPos = pPIDD->GetValue();

			if(strPos == L"PopBack")
			{
				// �� ���� ��ü �����ϸ� ��� ����
				// Obtain the last object by PopBack
				CGUIMenuBarUserDefinedInfo* pUdiReturn = pMenuBar->PopBackUserDefinedInfo(); 

				// �޸� ������ �����ϱ� ���� ���� ��ü ����
				// Destroy the object to prevent memory leakage
				if(pUdiReturn)
				{
					delete pUdiReturn;
					pUdiReturn = nullptr;
				}
			}
			else if(strPos == L"PopFront")
			{
				// �� ���� ��ü �����ϸ� ��� ����
				// Obtain the first object by PopFront
				CGUIMenuBarUserDefinedInfo* pUdiReturn = pMenuBar->PopFrontUserDefinedInfo(); 

				// �޸� ������ �����ϱ� ���� ���� ��ü ����
				// Destroy the object to prevent memory leakage
				if(pUdiReturn)
				{
					delete pUdiReturn;
					pUdiReturn = nullptr;
				}
			}
			else if(strPos == L"RemoveAt")
				pMenuBar->RemoveUserDefinedInfoAt(i32Index); // ������ �ε��� ��ġ�� ��ü ���� // Remove objects at the index you set

			pMenuBar->Invalidate();
		}
		while(false);
	};

	return pProcedure;
}

CPropertyItemButtonClickProcedure* FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::Set()
{
	CPropertyItemButtonClickProcedure* pProcedure = new CPropertyItemButtonClickProcedure;

	*pProcedure = MakePropertyItemButtonClickProcedure
	{
		do
		{
			CGUIMainFrame* pMF = AfxGetApp() && AfxGetApp()->m_pMainWnd ? dynamic_cast<CGUIMainFrame*>(AfxGetApp()->m_pMainWnd) : nullptr;

			if(!pMF)
				break;

			CGUIMenuBar* pMenuBar = pMF->GetMenuBar();

			if(!pMenuBar)
				break;

			Base::CFLString<wchar_t> str;
			COLORREF clrBackground;
			COLORREF clrText;
			bool bShow;
			EGUIAlignment eAlign;
			EGUIStringTrimming eStringTrimming;
			int32_t i32Index;

			// "Set" ī�װ� ���� ������Ƽ���� �Է��� �� ��� ����
			// Obtain parameters from the properties under "Set" category
			GetParameters(L"Set", i32Index, str, clrBackground, clrText, bShow, eAlign, eStringTrimming);

			// i32Index �ε����� CGUIMenuBarUserDefinedInfo ��ü ������ ��� ����
			// Obtain CGUIMenuBarUserDefinedInfo object pointer from i32Index index
			CGUIMenuBarUserDefinedInfo* pUdi = pMenuBar->GetUserDefinedInfoAt(i32Index);

			if(!pUdi)
				break;

			// ������Ƽ�� �Է��� ���� ���� ��ü�� ����
			// Set the value entered in the property to the object
			pUdi->Set(str, clrBackground, clrText, bShow, eAlign, eStringTrimming);

			pMenuBar->Invalidate();
		}
		while(false);
	};

	return pProcedure;
}

const CResult FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::AddItemForUserDefinedInfo(const Base::CFLString<wchar_t>& strPath)
{
	CResult cr;

	do
	{
		CGUIPropertyItemDropdownList* pPIDD = nullptr;
		CGUIPropertyItemText* pPIText = nullptr;
		CGUIPropertyItemColor* pPIColor = nullptr;
		CGUIPropertyItemCheckBox* pPICB = nullptr;

		pPIText = new CGUIPropertyItemText;
		pPIText->SetPath(strPath);
		pPIText->SetName(L"Index");
		pPIText->SetInputType(CGUIEditBase::EInputType_Number);
		pPIText->SetDefaultValue(L"0");
		AddItem(pPIText);

		pPIText = new CGUIPropertyItemText;
		pPIText->SetPath(strPath);
		pPIText->SetName(L"String");
		pPIText->SetDefaultValue(L"Sample Text");
		AddItem(pPIText);

		pPIDD = new CGUIPropertyItemDropdownList;
		pPIDD->SetPath(strPath);
		pPIDD->SetName(L"Text Alignment");
		pPIDD->AddListItem(L"LEFT_TOP");
		pPIDD->AddListItem(L"CENTER_TOP");
		pPIDD->AddListItem(L"RIGHT_TOP");
		pPIDD->AddListItem(L"LEFT_CENTER");
		pPIDD->AddListItem(L"CENTER_CENTER");
		pPIDD->AddListItem(L"RIGHT_CENTER");
		pPIDD->AddListItem(L"LEFT_BOTTOM");
		pPIDD->AddListItem(L"CENTER_BOTTOM");
		pPIDD->AddListItem(L"RIGHT_BOTTOM");
		pPIDD->SetDefaultValue(L"CENTER_CENTER");
		pPIDD->SetDescription(L"���ڿ��� ���� �ɼ�");
		AddItem(pPIDD);

		pPIDD = new CGUIPropertyItemDropdownList;
		pPIDD->SetPath(strPath);
		pPIDD->SetName(L"String Trimming");
		pPIDD->AddListItem(L"None");
		pPIDD->AddItemDescription(0, L"Trimming ���� ����");
		pPIDD->AddListItem(L"Character");
		pPIDD->AddItemDescription(1, L"���ڿ��� ���̾ƿ� �簢�� ���� �ִ� ������ ������ ��迡�� �������� �ǵ��� �����մϴ�.");
		pPIDD->AddListItem(L"Word");
		pPIDD->AddItemDescription(2, L"���ڿ��� ���̾ƿ� �簢�� ���� �ִ� ������ �ܾ��� ��迡�� �������� �ǵ��� �����մϴ�.");
		pPIDD->AddListItem(L"EllipsisCharacter");
		pPIDD->AddItemDescription(3, L"���ڿ��� ���̾ƿ� �簢�� ���� �ִ� ������ ������ ��迡�� �������� ���� �ڿ� ����ǥ(...)�� ���Եǵ��� �����մϴ�.");
		pPIDD->AddListItem(L"EllipsisWord");
		pPIDD->AddItemDescription(4, L"���ڿ��� ���̾ƿ� �簢�� �ȿ� �ִ� ������ �ܾ��� ��迡�� �������� �ܾ� �ڿ� ����ǥ(...)�� ���Եǵ��� �����մϴ�.");
		pPIDD->AddListItem(L"EllipsisPath");
		pPIDD->AddItemDescription(5, L"����� ���ڿ����� ���ŵǰ� ����ǥ�� ��ü�ǵ��� �����մϴ�. �˰����� ���ڿ��� ������ �κ��� �ִ��� ���� �����մϴ�.");
		pPIDD->SetDefaultValue(L"EllipsisCharacter");
		pPIDD->SetDescription(L"���ڿ��� ���̾ƿ� �簢���� ��� ��� �ڸ��� �ɼ�");
		AddItem(pPIDD);

		pPIColor = new CGUIPropertyItemColor;
		pPIColor->SetPath(strPath);
		pPIColor->SetName(L"Background Color");
		pPIColor->SetDefaultValueByColor(RGB(80, 80, 80));
		AddItem(pPIColor);

		pPIColor = new CGUIPropertyItemColor;
		pPIColor->SetPath(strPath);
		pPIColor->SetName(L"Text Color");
		pPIColor->SetDefaultValueByColor(RGB(255, 255, 255));
		AddItem(pPIColor);

		pPICB = new CGUIPropertyItemCheckBox;
		pPICB->SetPath(strPath);
		pPICB->SetName(L"Show");
		pPICB->SetDefaultValue(L"Checked");
		AddItem(pPICB);

		cr = EResult_OK;
	}
	while(false);

	return cr;
}

const CResult FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::GetParameters(const Base::CFLString<wchar_t>& strPath, int32_t& i32Index, Base::CFLString<wchar_t>& str, COLORREF& clrBackground, COLORREF& clrText, bool& bShow, EGUIAlignment& eAlign, EGUIStringTrimming& eStringTrimming)
{
	CResult cr;

	do
	{
		CGUIPropertyItemDropdownList* pPIDDTA = (CGUIPropertyItemDropdownList*)FindItemByFullPath(strPath + L"@Text Alignment");
		CGUIPropertyItemDropdownList* pPIDDST = (CGUIPropertyItemDropdownList*)FindItemByFullPath(strPath + L"@String Trimming");
		CGUIPropertyItemText* pPITextIndex = (CGUIPropertyItemText*)FindItemByFullPath(strPath + L"@Index");
		CGUIPropertyItemText* pPITextString = (CGUIPropertyItemText*)FindItemByFullPath(strPath + L"@String");
		CGUIPropertyItemColor* pPIColorBk = (CGUIPropertyItemColor*)FindItemByFullPath(strPath + L"@Background Color");
		CGUIPropertyItemColor* pPIColorText = (CGUIPropertyItemColor*)FindItemByFullPath(strPath + L"@Text Color");
		CGUIPropertyItemCheckBox* pPICB = (CGUIPropertyItemCheckBox*)FindItemByFullPath(strPath + L"@Show");

		if(pPIDDTA)
			eAlign = GetAlignment(pPIDDTA->GetValue());

		if(pPIDDST)
			eStringTrimming = GetStringTrimming(pPIDDST->GetValue());

		if(pPITextIndex)
			i32Index = pPITextIndex->GetValue().ToInt32();

		if(pPITextString)
			str = pPITextString->GetValue();

		if(pPIColorBk)
			clrBackground = pPIColorBk->GetColor();

		if(pPIColorText)
			clrText = pPIColorText->GetColor();

		if(pPICB)
			bShow = pPICB->GetValue() == L"Checked";

		cr = EResult_OK;
	}
	while(false);

	return cr;
}

const CResult FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::UpdateSetParameters(int32_t i32Index)
{
	CResult cr;

	do
	{
		CGUIMainFrame* pMF = AfxGetApp() && AfxGetApp()->m_pMainWnd ? dynamic_cast<CGUIMainFrame*>(AfxGetApp()->m_pMainWnd) : nullptr;

		if(!pMF)
			break;

		CGUIMenuBar* pMenuBar = pMF->GetMenuBar();

		if(!pMenuBar)
			break;

		CGUIPropertyItemDropdownList* pPIDDTA = (CGUIPropertyItemDropdownList*)FindItemByFullPath(L"Set@Text Alignment");
		CGUIPropertyItemDropdownList* pPIDDST = (CGUIPropertyItemDropdownList*)FindItemByFullPath(L"Set@String Trimming");
		CGUIPropertyItemText* pPITextIndex = (CGUIPropertyItemText*)FindItemByFullPath(L"Set@Index");
		CGUIPropertyItemText* pPITextString = (CGUIPropertyItemText*)FindItemByFullPath(L"Set@String");
		CGUIPropertyItemColor* pPIColorBk = (CGUIPropertyItemColor*)FindItemByFullPath(L"Set@Background Color");
		CGUIPropertyItemColor* pPIColorText = (CGUIPropertyItemColor*)FindItemByFullPath(L"Set@Text Color");
		CGUIPropertyItemCheckBox* pPICB = (CGUIPropertyItemCheckBox*)FindItemByFullPath(L"Set@Show");

		CGUIMenuBarUserDefinedInfo* pUdi = pMenuBar->GetUserDefinedInfoAt(i32Index);

		if(!pUdi)
		{
			if(pPITextString)
				pPITextString->SetValue(pPITextString->GetDefaultValue());

			if(pPIDDTA)
				pPIDDTA->SetValue(pPIDDTA->GetDefaultValue());

			if(pPIDDST)
				pPIDDST->SetValue(pPIDDST->GetDefaultValue());

			if(pPIColorBk)
				pPIColorBk->SetValue(pPIColorBk->GetDefaultValue());

			if(pPIColorText)
				pPIColorText->SetValue(pPIColorText->GetDefaultValue());

			if(pPICB)
				pPICB->SetValue(L"Checked");
		}
		else
		{
			if(pPITextString)
				pPITextString->SetValue(pUdi->GetText());

			if(pPIDDTA)
				pPIDDTA->SetValue(GetAlignmentText(pUdi->GetTextAlignment()));

			if(pPIDDST)
				pPIDDST->SetValue(GetStringTrimmingText(pUdi->GetStringTrimming()));

			if(pPIColorBk)
				pPIColorBk->SetColor(pUdi->GetBackgroundColor());

			if(pPIColorText)
				pPIColorText->SetColor(pUdi->GetTextColor());

			if(pPICB)
				pPICB->SetValue(pUdi->IsVisible() ? L"Checked" : L"Unchecked");
		}

		cr = EResult_OK;
	}
	while(false);

	return cr;
}

EGUIAlignment FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::GetAlignment(const Base::CFLString<wchar_t>& strVal) const
{
	EGUIAlignment eReturn = EGUIAlignment_CENTER_CENTER;

	if(strVal == L"CENTER_CENTER")
		eReturn = EGUIAlignment_CENTER_CENTER;
	else if(strVal == L"LEFT_TOP")
		eReturn = EGUIAlignment_LEFT_TOP;
	else if(strVal == L"RIGHT_TOP")
		eReturn = EGUIAlignment_RIGHT_TOP;
	else if(strVal == L"JUSTIFIED")
		eReturn = EGUIAlignment_JUSTIFIED;
	else if(strVal == L"CENTER_TOP")
		eReturn = EGUIAlignment_CENTER_TOP;
	else if(strVal == L"LEFT_BOTTOM")
		eReturn = EGUIAlignment_LEFT_BOTTOM;
	else if(strVal == L"RIGHT_BOTTOM")
		eReturn = EGUIAlignment_RIGHT_BOTTOM;
	else if(strVal == L"CENTER_BOTTOM")
		eReturn = EGUIAlignment_CENTER_BOTTOM;
	else if(strVal == L"LEFT_CENTER")
		eReturn = EGUIAlignment_LEFT_CENTER;
	else if(strVal == L"RIGHT_CENTER")
		eReturn = EGUIAlignment_RIGHT_CENTER;

	return eReturn;
}

Base::CFLString<wchar_t> FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::GetAlignmentText(EGUIAlignment eAlign) const
{
	Base::CFLString<wchar_t> strReturn;

	do
	{
		switch(eAlign)
		{
		case EGUIAlignment_CENTER_CENTER:
			strReturn = L"CENTER_CENTER";
			break;
		case EGUIAlignment_LEFT_TOP:
			strReturn = L"LEFT_TOP";
			break;
		case EGUIAlignment_RIGHT_TOP:
			strReturn = L"RIGHT_TOP";
			break;
		case EGUIAlignment_JUSTIFIED:
			strReturn = L"JUSTIFIED";
			break;
		case EGUIAlignment_CENTER_TOP:
			strReturn = L"CENTER_TOP";
			break;
		case EGUIAlignment_LEFT_BOTTOM:
			strReturn = L"LEFT_BOTTOM";
			break;
		case EGUIAlignment_RIGHT_BOTTOM:
			strReturn = L"RIGHT_BOTTOM";
			break;
		case EGUIAlignment_CENTER_BOTTOM:
			strReturn = L"CENTER_BOTTOM";
			break;
		case EGUIAlignment_LEFT_CENTER:
			strReturn = L"LEFT_CENTER";
			break;
		case EGUIAlignment_RIGHT_CENTER:
			strReturn = L"RIGHT_CENTER";
			break;
		}
	}
	while(false);

	return strReturn;
}

EGUIStringTrimming FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::GetStringTrimming(const Base::CFLString<wchar_t>& strVal) const
{
	EGUIStringTrimming eReturn = EGUIStringTrimming_EllipsisCharacter;

	if(strVal == L"EllipsisCharacter")
		eReturn = EGUIStringTrimming_EllipsisCharacter;
	else if(strVal == L"None")
		eReturn = EGUIStringTrimming_None;
	else if(strVal == L"Character")
		eReturn = EGUIStringTrimming_Character;
	else if(strVal == L"Word")
		eReturn = EGUIStringTrimming_Word;
	else if(strVal == L"EllipsisWord")
		eReturn = EGUIStringTrimming_EllipsisWord;
	else if(strVal == L"EllipsisPath")
		eReturn = EGUIStringTrimming_EllipsisPath;

	return eReturn;
}

Base::CFLString<wchar_t> FLImaging::GUI::CPropertyMenuBarUserDefinedInfoExample::GetStringTrimmingText(EGUIStringTrimming eStringTrimming) const
{
	Base::CFLString<wchar_t> strReturn;

	do
	{
		switch(eStringTrimming)
		{
		case EGUIStringTrimming_EllipsisCharacter:
			strReturn = L"EllipsisCharacter";
			break;
		case EGUIStringTrimming_None:
			strReturn = L"None";
			break;
		case EGUIStringTrimming_Character:
			strReturn = L"Character";
			break;
		case EGUIStringTrimming_Word:
			strReturn = L"Word";
			break;
		case EGUIStringTrimming_EllipsisWord:
			strReturn = L"EllipsisWord";
			break;
		case EGUIStringTrimming_EllipsisPath:
			strReturn = L"EllipsisPath";
			break;
		}
	}
	while(false);

	return strReturn;
}
