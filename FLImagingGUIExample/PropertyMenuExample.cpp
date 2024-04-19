#include "stdafx.h"
#include "PropertyMenuExample.h"

FLImaging::GUI::CPropertyMenuExample::CPropertyMenuExample() : CGUIPropertyMenuBase()
{
}

FLImaging::GUI::CPropertyMenuExample::~CPropertyMenuExample()
{
}

const CResult FLImaging::GUI::CPropertyMenuExample::ConfigureMenu()
{
	CResult eReturn = EResult_UnknownError;

	do
	{
		// �̹��� �ε� ��ư�� �߰��մϴ�.
		CGUIPropertyButton* pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Load Image");
		pBtn->SetPropertyButtonClickProcedure(CPropertyMenuExample::LoadImage());
		pBtn->SetButtonWidthRatio(.6);
		AddButton(pBtn);

		// Ŭ���� �� ��ư�� �߰��մϴ�.
		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Clear View");
		pBtn->SetPropertyButtonClickProcedure(CPropertyMenuExample::ClearView());
		pBtn->SetButtonWidthRatio(.4);
		AddButton(pBtn);

		// Draw Figure ��ư�� �߰��մϴ�.
		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Draw Figure");
		pBtn->SetPropertyButtonClickProcedure(CPropertyMenuExample::DrawFigure());
		pBtn->SetButtonWidthRatio(.6);
		SetShortcut(EKeyType_F5, pBtn);
		AddButton(pBtn);

		// Clear Layer ��ư�� �߰��մϴ�.
		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Clear Layer");
		pBtn->SetPropertyButtonClickProcedure(CPropertyMenuExample::ClearLayer());
		pBtn->SetButtonWidthRatio(.4);
		SetShortcut(EKeyType_F6, pBtn);
		AddButton(pBtn);

		// ī�װ� �������� �߰��մϴ�.
		CGUIPropertyItemCategory* pCatImageView = new CGUIPropertyItemCategory;
		pCatImageView->SetName(L"Example");
		AddItem(pCatImageView);
		{
			// Example ī�װ��� ���� ī�װ��� Image ī�װ��� �߰��մϴ�.
			pCatImageView = new CGUIPropertyItemCategory;
			pCatImageView->SetName(L"Image");
			pCatImageView->SetPath(L"Example");
			AddItem(pCatImageView);
			{
				// �� �̹��� ����Ʈ �������� �߰��մϴ�.
				CGUIPropertyItemViewImageList* pViewImageList = new CGUIPropertyItemViewImageList;
				pViewImageList->SetName(L"Image View");
				pViewImageList->SetPath(L"Example@Image");
				AddItem(pViewImageList);

				// ���� ���� �������� �߰��մϴ�.
				CGUIPropertyItemFile* pFile = new CGUIPropertyItemFile;
				pFile->SetName(L"File");
				pFile->SetPath(L"Example@Image");
				pFile->SetFileExtention(L"*.bmp;*.jpg;*.jpeg;*.tif;*.png;");
				pFile->SetFileFilter(L"All Supporting Files (*.flif;*.bmp;*.jpg;*.jpeg;*.tif;*.png;)|*.flif;*.bmp;*.jpg;*.jpeg;*.tif;*.png; |FLImaging Image Files(*.flif;) | *.flif; |Bitmap Files(*.bmp;) | *.bmp; |JPEG Files(*.jpg; *.jpeg;) | *.jpg; *.jpeg; |TIFF Files(*.tif; *.tiff;) | *.tif; *.tiff; |PNG Files(*.png;) | *.png; |All Files(*.*;) | *.*; ||");
				AddItem(pFile);

				// �� �̹��� ROI �������� �߰��մϴ�.
				CGUIPropertyItemViewImageFigure* pViewImageROI = new CGUIPropertyItemViewImageFigure;
				pViewImageROI->SetName(L"ROI");
				pViewImageROI->SetPath(L"Example@Image");
				pViewImageROI->SetViewImageList(pViewImageList);
				AddItem(pViewImageROI);
			}

			// Example ī�װ��� ���� ī�װ��� Parameters ī�װ��� �߰��մϴ�.
			pCatImageView = new CGUIPropertyItemCategory;
			pCatImageView->SetName(L"Parameters");
			pCatImageView->SetPath(L"Example");
			AddItem(pCatImageView);
			{
				// �ؽ�Ʈ �������� �߰��մϴ�.
				CGUIPropertyItemText* pText = new CGUIPropertyItemText;
				pText->SetName(L"Text");
				pText->SetPath(L"Example@Parameters");
				pText->SetDefaultValue(L"ABC");
				pText->SetDescription(L"Custom Description");
				AddItem(pText);

				// InputType�� ������ �ؽ�Ʈ �������� �߰��մϴ�.
				pText = new CGUIPropertyItemText;
				pText->SetName(L"Only Number");
				pText->SetPath(L"Example@Parameters");
				pText->SetInputType(CGUIEditBase::EInputType_Number);
				pText->SetDefaultValue(L"123");
				AddItem(pText);

				// �޺��ڽ� �������� �߰��մϴ�.
				CGUIPropertyItemDropdownList* pDropdownList = new CGUIPropertyItemDropdownList;
				pDropdownList->SetName(L"Dropdown List");
				pDropdownList->SetPath(L"Example@Parameters");
				pDropdownList->AddListItem(L"OK");
				pDropdownList->AddListItem(L"NG");
				pDropdownList->AddListItem(L"ALL");
				pDropdownList->SetDefaultValue(L"OK");
				AddItem(pDropdownList);

				// üũ�ڽ� �������� �߰��մϴ�.
				CGUIPropertyItemCheckBox* pCheckBox = new CGUIPropertyItemCheckBox;
				pCheckBox->SetName(L"Hide Image Category");
				pCheckBox->SetPath(L"Example@Parameters");
				// CallBack ó�� �Լ��� �߰��մϴ�.
				CPropertyCallback* pPropertyCallBack = new CPropertyCallback;
				*pPropertyCallBack = MakePropertyCallback
				{
					OnPropertyPaneChanged(pItem, strValue);
				};
				pCheckBox->SetPropertyCallback(pPropertyCallBack);
				pCheckBox->SetDefaultValue(L"Unchecked");
				AddItem(pCheckBox);

				// �����̴� �������� �߰��մϴ�.
				CGUIPropertyItemSlider* pSlider = new CGUIPropertyItemSlider;
				pSlider->SetName(L"Slider");
				pSlider->SetPath(L"Example@Parameters");
				pSlider->SetEditCtrlWidth(100);
				pSlider->SetRange(0, 255);
				pSlider->SetDefaultValue(L"0");
				AddItem(pSlider);

				// Spin �������� �߰��մϴ�.
				CGUIPropertyItemSpin* pSpin = new CGUIPropertyItemSpin;
				pSpin->SetName(L"Spin");
				pSpin->SetPath(L"Example@Parameters");
				pSpin->SetRange(0, 10);
				pSpin->SetDefaultValue(L"0");
				AddItem(pSpin);

				// IP Address �������� �߰��մϴ�.
				CGUIPropertyItemIPAddress* pIPAddress = new CGUIPropertyItemIPAddress;
				pIPAddress->SetName(L"IPAddress");
				pIPAddress->SetPath(L"Example@Parameters");
				AddItem(pIPAddress);
			}
		}

		eReturn = EResult_OK;
	}
	while(false);

	return eReturn;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuExample::LoadImage()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		do
		{
			CGUIPropertyItemViewImageList* pImageViewList = dynamic_cast<CGUIPropertyItemViewImageList*>(FindItemByFullPath(L"Example@Image@Image View"));

			CGUIPropertyItemBase* pFile = FindItemByFullPath(L"Example@Image@File");

			if(!pImageViewList || !pFile)
				break;

			if(!pImageViewList->GetSelectedViewImage())
				break;

			CFLString<wchar_t> flsFile = pFile->GetValue();

			pImageViewList->GetSelectedViewImage()->Load(flsFile);
		}
	    while(false);
	};

	return pProcedure;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuExample::ClearView()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		do
		{
			CGUIPropertyItemViewImageList* pImageViewList = dynamic_cast<CGUIPropertyItemViewImageList*>(FindItemByFullPath(L"Example@Image@Image View"));

			if(!pImageViewList)
				break;

			CGUIViewImage* pViewImage = pImageViewList->GetSelectedViewImage();

			if(!pViewImage)
				break;

			pViewImage->ClearFile();
			pViewImage->ClearAllLayers();
			pViewImage->Invalidate();
		}
		while(false);
	};

	return pProcedure;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuExample::DrawFigure()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		do
		{
			CModelUtilities mu(CGUIManagerModel::GetCurrentModelFullPath(), L"Menu@Example");

			if((mu.Load()).IsFail())
				break;

			CGUIPropertyItemViewImageList* pImageViewList = dynamic_cast<CGUIPropertyItemViewImageList*>(FindItemByFullPath(L"Example@Image@Image View"));

			if(!pImageViewList)
				break;

			CGUIViewImage* pViewImage = pImageViewList->GetSelectedViewImage();

			if(!pViewImage)
				break;

			CGUIViewImageLayer* pLayer = pViewImage->GetLayer(0);
			
			if(!pLayer)
				break;

			// ROI�� �޾ƿɴϴ�.
			CFLFigureArray flfaROI;
			CFigureUtilities::ConvertFigureStringToObject(&mu.GetValue(L"Example@Image", L"ROI"), &flfaROI);

			pLayer->DrawFigureImage(flfaROI, RED, 3);

			pViewImage->Invalidate();
		}
		while(false);
	};

	return pProcedure;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuExample::ClearLayer()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		do
		{
			CGUIPropertyItemViewImageList* pImageViewList = dynamic_cast<CGUIPropertyItemViewImageList*>(FindItemByFullPath(L"Example@Image@Image View"));

			if(!pImageViewList)
				break;

			CGUIViewImage* pViewImage = pImageViewList->GetSelectedViewImage();

			if(!pViewImage)
				break;

			pViewImage->ClearAllLayers();
			pViewImage->Invalidate();
		}
		while(false);
	};

	return pProcedure;
}

const CResult FLImaging::GUI::CPropertyMenuExample::OnPropertyPaneChanged(CGUIPropertyItemBase* pBase, Base::CFLString<wchar_t>& strValue)
{
	CResult er = EResult_UnknownError;

	do
	{
		if(!pBase)
			break;

		CGUIMenuItemProperty* pProp = dynamic_cast<CGUIMenuItemProperty*>(pBase->GetGUIMenuItemProperty());

		if(!pProp)
			break;

		if(!pBase->GetName().Compare(L"Hide Image Category"))
		{
			CGUIPropertyItemCategory* pCategory = dynamic_cast<CGUIPropertyItemCategory*>(pProp->FindItemByName(L"Image"));

			bool bHide = false;

			if(!strValue.Compare(L"Checked"))
				bHide = true;

			pCategory->SetVisible(!bHide);
		}

		if(pBase->GetParentWndList())
			pBase->GetParentWndList()->AdjustLayout();

		er = EResult_OK;
	}
	while(false);

	return er;
}
