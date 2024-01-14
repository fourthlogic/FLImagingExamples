#include "stdafx.h"
#include "PropertyMenuDialogProgressExample.h"

using namespace FLImaging;
using namespace FLImaging::Base;
using namespace FLImaging::Foundation;
using namespace FLImaging::GUI;

namespace FLImaging
{
	namespace GUI
	{
		// ������ ���̾�α��� ���̾ƿ��� row �ε���
		enum class ERowLayout : int32_t
		{
			Main = 0,           // Main ���̾ƿ�
			Button = 4,         // Button ���̾ƿ�
			Details = 0,        // Details ���̾ƿ�

			Count = 5,          // �� ����
		};

		// ������ ���̾�α��� ���̾ƿ��� col �ε���
		enum class EColLayout : int32_t
		{
			Main = 0,           // Main ���̾ƿ�
			Button = 0,         // Button ���̾ƿ�
			Details = 1,        // Details ���̾ƿ�

			Count = 2,          // �÷� ����
		};

		// ������ ���̾�α��� ���� �׸��� ���� ��Ʈ���� row �ε���
		enum class ERowMainComplex : int32_t
		{
			Message = 0,        // �޼���
			ProgressCtrl = 3,   // ���α׷��� ��
			ElapsedTime = 4,    // �ð�

			Count = 5,          // �� ����
		};

		// ������ ���̾�α��� ���� �׸��� ���� ��Ʈ���� col �ε���
		enum class EColMainComplex : int32_t
		{
			Message = 1,        // �޼���
			ProgressCtrl = 1,   // ���α׷��� ��
			ElapsedTime = 1,    // �ð�

			Count = 6,          // �÷� ����
		};

		// �ܼ��� ���̾�α��� ���� �׸��� ���� ��Ʈ���� row �ε���
		enum class ERowMainSimple : int32_t
		{
			Message = 0,       // �޼���
			ProgressCtrl = 2,  // ���α׷��� ��
			ElapsedTime = 3,   // �ð�

			Count = 4,         // �� ����
		};

		// �ܼ��� ���̾�α��� ���� �׸��� ���� ��Ʈ���� col �ε���
		enum class EColMainSimple : int32_t
		{
			Message = 0,       // �޼���
			ProgressCtrl = 0,  // ���α׷��� ��
			ElapsedTime = 0,   // �ð�

			Count = 1,         // �÷� ����
		};

		// ������ ���̾�α��� ��ư �׸����� row �ε���
		enum class ERowButtonComplex : int32_t
		{
			Details = 0,       // ���� ���� ��ư
			Stop = 1,          // ���� ��ư

			Count = 2,         // �� ����
		};

		// �ܼ��� ���̾�α��� ��ư �׸����� row �ε���
		enum class ERowButtonSimple : int32_t
		{
			Stop = 0,          // ���� ��ư

			Count = 1,         // �� ����
		};

		// ���̾�α��� ��ư �׸����� col �ε���
		enum class EColButton : int32_t
		{
			Button = 2,        // ��ư

			Count = 3,         // �÷� ����
		};
	}
}

FLImaging::GUI::CPropertyMenuDialogProgressExample::CPropertyMenuDialogProgressExample()
	: CGUIPropertyMenuBase()
	, m_pDlgProgress(nullptr)
	, m_pThread(nullptr)
	, m_pProgressCtrlFP(nullptr)
	, m_pStopButton(nullptr)
	, m_pGridMain(nullptr)
	, m_pGridButtons(nullptr)
	, m_pGridDetails(nullptr)
	, m_i32BtnStopClickCount(0)
{
	m_vctVariousLengthMessages.push_back(L"consectetur adipiscing elit, sed do eiusmod tempor incididunt \nut labore et dolore magna aliqua. \nUt enim ad minim veniam, \nquis nostrud exercitation ullamco laboris nisi \nut aliquip ex ea commodo consequat. ");
	m_vctVariousLengthMessages.push_back(L"Duis");
	m_vctVariousLengthMessages.push_back(L"Lorem ipsum dolor sit amet,");
	m_vctVariousLengthMessages.push_back(L"aute irure dolor in reprehenderit in ");
	m_vctVariousLengthMessages.push_back(L"voluptate velit esse");
	m_vctVariousLengthMessages.push_back(L"cillum dolore eu fugiat nulla pariatur. Excepteur ");
	m_vctVariousLengthMessages.push_back(L"sint occaecat \ncupidatat non proident, \nsunt in culpa qui  \nofficia deserunt \n\n\n\n\n\nmollit anim id est laborum.\n");
}

FLImaging::GUI::CPropertyMenuDialogProgressExample::~CPropertyMenuDialogProgressExample()
{
	CloseProgressDialog();

	if(m_pThread)
	{
		delete m_pThread;
		m_pThread = nullptr;
	}
}

const CResult FLImaging::GUI::CPropertyMenuDialogProgressExample::ConfigureMenu()
{
	CResult cReturn = EResult_UnknownError;

	do
	{
		CGUIPropertyButton* pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Simple Dialog");
		pBtn->SetPropertyButtonClickProcedure(ButtonCreateSimpleDialog());
		pBtn->SetButtonWidthRatio(1.);
		AddButton(pBtn);
		SetShortcut(EKeyType_F6, pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Dialog With Details");
		pBtn->SetPropertyButtonClickProcedure(ButtonCreateComplexDialog());
		pBtn->SetButtonWidthRatio(1.);
		AddButton(pBtn);
		SetShortcut(EKeyType_F7, pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Param(Pivot, Alignment, ..) Applied Dialog");
		pBtn->SetPropertyButtonClickProcedure(ButtonCreateSimpleDialogWithSettings());
		pBtn->SetButtonWidthRatio(1.);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Dialog With CFLSheet - BoxLayout");
		pBtn->SetPropertyButtonClickProcedure(ButtonCreateComplexDialogWithSheetBoxLayout());
		pBtn->SetButtonWidthRatio(1.);
		AddButton(pBtn);

		pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"Dialog With CFLSheet - GridLayout");
		pBtn->SetPropertyButtonClickProcedure(ButtonCreateComplexDialogWithSheetGridLayout());
		pBtn->SetButtonWidthRatio(1.);
		AddButton(pBtn);

		CGUIPropertyItemDropdownList* pDrop = new CGUIPropertyItemDropdownList;
		pDrop->SetName(L"Text Alignment");
		pDrop->AddListItem(L"LEFT_TOP");
		pDrop->AddListItem(L"CENTER_TOP");
		pDrop->AddListItem(L"RIGHT_TOP");
		pDrop->AddListItem(L"LEFT_CENTER");
		pDrop->AddListItem(L"CENTER_CENTER");
		pDrop->AddListItem(L"RIGHT_CENTER");
		pDrop->AddListItem(L"LEFT_BOTTOM");
		pDrop->AddListItem(L"CENTER_BOTTOM");
		pDrop->AddListItem(L"RIGHT_BOTTOM");
		pDrop->SetDefaultValue(L"CENTER_CENTER");
		pDrop->SetDescription(L"���̾�α� â ���� �ؽ�Ʈ ���� �ɼ��� �����մϴ�.");
		AddItem(pDrop);

		pDrop = new CGUIPropertyItemDropdownList;
		pDrop->SetName(L"Resize Pivot");
		pDrop->AddListItem(L"LEFT_TOP");
		pDrop->AddListItem(L"CENTER_TOP");
		pDrop->AddListItem(L"RIGHT_TOP");
		pDrop->AddListItem(L"LEFT_CENTER");
		pDrop->AddListItem(L"CENTER_CENTER");
		pDrop->AddListItem(L"RIGHT_CENTER");
		pDrop->AddListItem(L"LEFT_BOTTOM");
		pDrop->AddListItem(L"CENTER_BOTTOM");
		pDrop->AddListItem(L"RIGHT_BOTTOM");
		pDrop->SetDefaultValue(L"LEFT_TOP");
		pDrop->SetDescription(L"���̾�α� â�� ũ�Ⱑ ����� �� ��� ��ġ�� �������� â�� Ȯ�� �Ǵ� ������� ���� ��ġ�� ���մϴ�.");
		AddItem(pDrop);

		CGUIPropertyItemCheckBox* pCheck = new CGUIPropertyItemCheckBox;
		pCheck->SetName(L"Keep Maximum Width");
		pCheck->SetDescription(L"���̾�α� â�� ũ�Ⱑ ����� �� ����Ǳ� �� â�� �ʺ� �� ũ��, â�� �ʺ� �������� �ʰ� ���� �ʺ� �������� ���� ����");
		AddItem(pCheck);

		pCheck = new CGUIPropertyItemCheckBox;
		pCheck->SetName(L"Keep Maximum Height");
		pCheck->SetDescription(L"���̾�α� â�� ũ�Ⱑ ����� �� ����Ǳ� �� â�� ���̰� �� ũ��, â�� ���̸� �������� �ʰ� ���� ���̸� �������� ���� ����");
		AddItem(pCheck);

		cReturn = EResult_OK;
	}
	while(false);

	return cReturn;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuDialogProgressExample::ButtonCreateSimpleDialog()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		CreateSimpleDialog();
		// ���̾�α� â ����
		m_pDlgProgress->OnInitDialog();

		// �۾� ������ ����
		m_pThread = new std::future<void>(std::async(CPropertyMenuDialogProgressExample::AlgorithmThreadForSimpleDialog, this, &m_pThread));
		SetThreadPriority(m_pThread, THREAD_PRIORITY_LOWEST);

		// ������ �Ķ���� �ʱ�ȭ
		ResetThreadParams();

		// �۾� �����忡�� ������Ʈ �� ���� �ݿ��Ͽ� ���̾�α� â ������Ʈ
		UpdateSimpleDialog();

		// ���̾�α� �ݱ�
		CloseProgressDialog();

		if(m_pThread)
		{
			delete m_pThread;
			m_pThread = nullptr;
		}
	};

	return pProcedure;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuDialogProgressExample::ButtonCreateComplexDialog()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		CreateComplexProgressDialog();
	};

	return pProcedure;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuDialogProgressExample::ButtonCreateSimpleDialogWithSettings()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		CreateSimpleDialogWithSettings();
	};

	return pProcedure;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuDialogProgressExample::ButtonCreateComplexDialogWithSheetBoxLayout()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		bool bBoxLayout = true;
		CreateComplexProgressDialogWithSheet(bBoxLayout);
	};

	return pProcedure;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuDialogProgressExample::ButtonCreateComplexDialogWithSheetGridLayout()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		bool bBoxLayout = false;
		CreateComplexProgressDialogWithSheet(bBoxLayout);
	};

	return pProcedure;
}

const CResult FLImaging::GUI::CPropertyMenuDialogProgressExample::CreateSimpleDialog()
{
	CResult cr;

	do
	{
		CloseProgressDialog();

		// ���̾�α� ����
		if(!m_pDlgProgress)
			m_pDlgProgress = CGUIDialogProgress::CreateModelessDialog(AfxGetApp()->m_pMainWnd);

		CFLString<wchar_t> strMessage;

		// �޽���, ���α׷��� ��, �ð� ���� ǥ�õǴ� ���� �׸��� ���̾ƿ� ����
		m_pGridMain = new CGUIGridLayout((int32_t)ERowMainSimple::Count, (int32_t)EColMainSimple::Count);
		{
			strMessage.Format
			(
				L"CGUIDialogProgress example thread is working...\n(%d/%d)",
				m_i32CurrentProgress, m_i32TotalProgress
			);

			// �޽����� �׸��� ���̾ƿ��� �߰�
			m_pGridMain->AddCtrl((int32_t)ERowMainSimple::Message, (int32_t)EColMainSimple::Message, 2, 1, strMessage);
			// ���α׷��� �ٸ� �׸��� ���̾ƿ��� �߰�
			m_pGridMain->AddCtrl((int32_t)ERowMainSimple::ProgressCtrl, (int32_t)EColMainSimple::ProgressCtrl, EControl_ProgressBarFloatingPoint);
			// �ҿ� �ð��� �׸��� ���̾ƿ��� �߰�
			m_pGridMain->AddCtrl((int32_t)ERowMainSimple::ElapsedTime, (int32_t)EColMainSimple::ElapsedTime, L"Elapsed Time      00 : 00 : 00");
			// �ҿ� �ð��� �ؽ�Ʈ ���� �ɼ��� ����, ���� ��� �߰� ���ķ� ����
			m_pGridMain->SetCtrlAlignment((int32_t)ERowMainSimple::ElapsedTime, (int32_t)EColMainSimple::ElapsedTime, EGUIAlignment_CENTER_CENTER);
			// �ҿ� �ð��� m_pGridMain ���̾ƿ��� (int32_t)ERowMainSimple::ElapsedTime, (int32_t)EColMainSimple::ElapsedTime ��ġ�� �ִٴ� ���� m_pDlgProgress �� �˷���
			m_pDlgProgress->SetElapsedTimePosition(m_pGridMain, (int32_t)ERowMainSimple::ElapsedTime, (int32_t)EColMainSimple::ElapsedTime);
			// ����� ������Ʈ�� ���� ���α׷��� �� ��ü �����͸� ��� ����
			m_pProgressCtrlFP = (CGUIProgressCtrlFloatingPoint*)m_pGridMain->GetCtrl((int32_t)ERowMainSimple::ProgressCtrl, (int32_t)EColMainSimple::ProgressCtrl);
		}

		// ��ư�� ǥ�õǴ� ��ư �׸��� ���̾ƿ� ����
		m_pGridButtons = new CGUIGridLayout((int32_t)ERowButtonSimple::Count, (int32_t)EColButton::Count);
		{
			// ���� ��ư�� �׸��� ���̾ƿ��� �߰�
			m_pGridButtons->AddCtrl((int32_t)ERowButtonSimple::Stop, (int32_t)EColButton::Button, EControl_Button, L"Stop");
			// ���� ��ư ��ü �����͸� ��� ����
			m_pStopButton = (CGUIButton*)m_pGridButtons->GetCtrl((int32_t)ERowButtonSimple::Stop, (int32_t)EColButton::Button);
		}

		// ���̾�α׿� ���� �׸��� ���̾ƿ� �߰�
		m_pDlgProgress->Add(m_pGridMain);
		// ���̾�α׿� ��ư �׸��� ���̾ƿ� �߰�
		m_pDlgProgress->Add(m_pGridButtons);

		cr = EResult_OK;
	}
	while(false);

	return cr;
}

const CResult FLImaging::GUI::CPropertyMenuDialogProgressExample::CreateComplexProgressDialog()
{
	CResult cr;

	do
	{
		CloseProgressDialog();

		CGUIPropertyItemDropdownList* pDropTextAlignment = dynamic_cast<CGUIPropertyItemDropdownList*>(FindItemByName(L"Text Alignment"));
		CGUIPropertyItemDropdownList* pDropResizePivot = dynamic_cast<CGUIPropertyItemDropdownList*>(FindItemByName(L"Resize Pivot"));
		CGUIPropertyItemCheckBox* pCheckKeepMaxWidth = dynamic_cast<CGUIPropertyItemCheckBox*>(FindItemByName(L"Keep Maximum Width"));
		CGUIPropertyItemCheckBox* pCheckKeepMaxHeight = dynamic_cast<CGUIPropertyItemCheckBox*>(FindItemByName(L"Keep Maximum Height"));

		EGUIAlignment eTextAlignment = GetAlignment(pDropTextAlignment->GetValue());
		EGUIAlignment eResizePivot = GetAlignment(pDropResizePivot->GetValue());
		bool bKeepMaxWidth = pCheckKeepMaxWidth->GetValue() == L"Checked";
		bool bKeepMaxHeight = pCheckKeepMaxHeight->GetValue() == L"Checked";

		// ���̾�α� ����
		if(!m_pDlgProgress)
			m_pDlgProgress = CGUIDialogProgress::CreateModelessDialog(AfxGetApp()->m_pMainWnd);

		CFLString<wchar_t> strMessage;
		CFLString<wchar_t> strMessageFormat =
			CFLString<wchar_t>(L"[Step 3/3] Learning...\n\n") +
			CFLString<wchar_t>(L"<Cost>\n") +
			CFLString<wchar_t>(L"0.012132\n\n") +
			CFLString<wchar_t>(L"<Iteration>\n") +
			CFLString<wchar_t>(L"(%d/%d)\n\n") + // m_i32CurrentProgress/m_i32TotalProgress
			CFLString<wchar_t>(L"<Epoch>\n") +
			CFLString<wchar_t>(L"1/100\n\n") +
			CFLString<wchar_t>(L"<Validation>\n") +
			CFLString<wchar_t>(L"96.12%%at 1 epoch\n\n\n");

		// �޽���, ���α׷��� ��, �ð� ���� ǥ�õǴ� ���� �׸��� ���̾ƿ� ����
		m_pGridMain = new CGUIGridLayout((int32_t)ERowMainComplex::Count, (int32_t)EColMainComplex::Count);
		{
			strMessage.Format
			(
				strMessageFormat,
				m_i32CurrentProgress, m_i32TotalProgress
			);

			// �޽����� �׸��� ���̾ƿ��� �߰�
			m_pGridMain->AddCtrl((int32_t)ERowMainComplex::Message, (int32_t)EColMainComplex::Message, 3, 4, strMessage);
			// �޽����� �ؽ�Ʈ ���� �ɼ��� "Text Alignment" ���� ��� �� ������ ����
			m_pGridMain->SetCtrlAlignment((int32_t)ERowMainComplex::Message, (int32_t)EColMainComplex::Message, eTextAlignment);
			// ���α׷��� �ٸ� �׸��� ���̾ƿ��� �߰�
			m_pGridMain->AddCtrl((int32_t)ERowMainComplex::ProgressCtrl, (int32_t)EColMainComplex::ProgressCtrl, 1, 4, EControl_ProgressBarFloatingPoint);
			// �ҿ� �ð��� �׸��� ���̾ƿ��� �߰�
			m_pGridMain->AddCtrl((int32_t)ERowMainComplex::ElapsedTime, (int32_t)EColMainComplex::ElapsedTime, 1, 4, L"Elapsed Time      00 : 00 : 00");
			// �ҿ� �ð��� �ؽ�Ʈ ���� �ɼ��� ����, ���� ��� �߰� ���ķ� ����
			m_pGridMain->SetCtrlAlignment((int32_t)ERowMainComplex::ElapsedTime, (int32_t)EColMainComplex::ElapsedTime, EGUIAlignment_CENTER_CENTER);
			// �ҿ� �ð��� m_pGridMain ���̾ƿ��� (int32_t)ERowMainComplex::ElapsedTime, (int32_t)EColMainComplex::ElapsedTime ��ġ�� �ִٴ� ���� m_pDlgProgress �� �˷���
			m_pDlgProgress->SetElapsedTimePosition(m_pGridMain, (int32_t)ERowMainComplex::ElapsedTime, (int32_t)EColMainComplex::ElapsedTime);
			// ����� ������Ʈ�� ���� ���α׷��� �� ��ü �����͸� ��� ����
			m_pProgressCtrlFP = (CGUIProgressCtrlFloatingPoint*)m_pGridMain->GetCtrl((int32_t)ERowMainComplex::ProgressCtrl, (int32_t)EColMainComplex::ProgressCtrl);
		}

		// ��ư�� ǥ�õǴ� ��ư �׸��� ���̾ƿ� ����
		m_pGridButtons = new CGUIGridLayout((int32_t)ERowButtonComplex::Count, (int32_t)EColButton::Count);
		{
			// �� ���� ��ư�� �׸��� ���̾ƿ��� �߰�
			m_pGridButtons->AddCtrl((int32_t)ERowButtonComplex::Details, (int32_t)EColButton::Button, EControl_Button, L"See Details");
			// "See Details" ��ư Ŭ�� �̺�Ʈ ó���� ����
			CWndCtrlProcedure* pVCP = new CWndCtrlProcedure;
			// "See Details" ��ư Ŭ�� �� ����Ǵ� ���� ����
			*pVCP = MakeWndCtrlProcedure
			{
				// "See Details" ��ư�� Ȧ�� �� ���� ���
				if(((CGUIButton*)m_pGridButtons->GetCtrl((int32_t)ERowButtonComplex::Details, (int32_t)EColButton::Button))->GetClickCount() % 2 == 1)
				{
					// ������ �׸��� ���̾ƿ��� show
					m_pGridDetails->SetVisible(true);
					// "See Details" ��ư�� �ؽ�Ʈ�� "Close Details" �� ����
					m_pGridButtons->SetCtrlValue((int32_t)ERowButtonComplex::Details, (int32_t)EColButton::Button, L"Close Details");
				}
				// "See Details" ��ư�� ¦�� �� ���� ���
				else
				{
					// ������ �׸��� ���̾ƿ��� hide
					m_pGridDetails->SetVisible(false);
					// "Close Details" ��ư�� �ؽ�Ʈ�� "See Details" �� ����
					m_pGridButtons->SetCtrlValue((int32_t)ERowButtonComplex::Details, (int32_t)EColButton::Button, L"See Details");
				}

				// ���̾�α��� ���̾ƿ��� ������Ʈ
				m_pDlgProgress->AdjustLayout();

				return true;
			};
			// "See Details" ��ư�� Ŭ�� �̺�Ʈ ó���� �߰�
			m_pGridButtons->SetWndCtrlProcedure((int32_t)ERowButtonComplex::Details, (int32_t)EColButton::Button, pVCP);

			// ���� ��ư�� �׸��� ���̾ƿ��� �߰�
			m_pGridButtons->AddCtrl((int32_t)ERowButtonComplex::Stop, (int32_t)EColButton::Button, EControl_Button, L"Stop");
			// ���� ��ư ��ü �����͸� ��� ����
			m_pStopButton = (CGUIButton*)m_pGridButtons->GetCtrl((int32_t)ERowButtonComplex::Stop, (int32_t)EColButton::Button);
		}

		// �� ������ ǥ�õǴ� �׸��� ���̾ƿ� ����
		m_pGridDetails = new CGUIGridLayout(7, 2, true);
		{
			// ���� ��Ʈ�ѵ� �߰�
			m_pGridDetails->AddCtrl(0, 0, 1, 2, L"In a general sense, continuity \ndescribes how two items \ncome together. In ACIS, \nthese items may be two curves \nthat meet in some way, \ntwo portions of the same curve, etc. (In the latter case, \none is usually describing \nthe smoothness of a curve, \nwhich is a global property, \nin terms of a local property.)");
			m_pGridDetails->SetCtrlAlignment(0, 0, EGUIAlignment_RIGHT_TOP);
			m_pGridDetails->AddCtrl(1, 0, 1, 2, EControl_Slider);
			m_pGridDetails->AddCtrl(2, 0, EControl_CheckBox, L"ACIS");
			m_pGridDetails->AddCtrl(2, 1, EControl_ComboBox);
			m_pGridDetails->AddComboBoxItem(3, 1, L"C0");
			m_pGridDetails->AddComboBoxItem(3, 1, L"C1");
			m_pGridDetails->AddComboBoxItem(3, 1, L"C2");
			m_pGridDetails->AddComboBoxItem(3, 1, L"G1");
			m_pGridDetails->AddComboBoxItem(3, 1, L"G2");
			m_pGridDetails->AddCtrl(4, 0, L"Expression");
			m_pGridDetails->AddCtrl(4, 1, EControl_EditCtrl);
			m_pGridDetails->AddCtrl(5, 0, L"Slider2");
			m_pGridDetails->AddCtrl(5, 1, EControl_SliderFloatingPoint);

			CGUIBoxLayout* pBoxInner = new CGUIBoxLayout(EOrientation_Vertical, true, L"Box");
			CGUIGridLayout* pGridInner = new CGUIGridLayout(3, 2, true, L"Grid");
			pBoxInner->AddCtrl(EControl_EditCtrl);
			pBoxInner->AddCtrl(EControl_EditCtrl);
			pBoxInner->AddCtrl(EControl_EditCtrl);
			pGridInner->AddCtrl(0, 0, L"Edit1");
			pGridInner->AddCtrl(1, 0, L"Edit2");
			pGridInner->AddCtrl(2, 0, L"Edit2");
			pGridInner->AddCtrl(0, 1, EControl_EditCtrl);
			pGridInner->AddCtrl(1, 1, EControl_EditCtrl);
			pGridInner->AddCtrl(2, 1, EControl_EditCtrl);
			m_pGridDetails->Add(6, 0, pBoxInner);
			m_pGridDetails->Add(6, 1, pGridInner);
		}

		// ���̾ƿ����� ��ġ�ϱ� ���� ��� ���̾ƿ� ����
		CGUIGridLayout* pGridBackground = new CGUIGridLayout((int32_t)ERowLayout::Count, (int32_t)EColLayout::Count);

		// ���� �׸��� ���̾ƿ��� ERowLayout::Main, EColLayout::Main ��ġ���� row span = 4, col span = 1 �� �����ϵ��� ��ġ
		pGridBackground->Add((int32_t)ERowLayout::Main, (int32_t)EColLayout::Main, 4, 1, m_pGridMain);
		// ��ư �׸��� ���̾ƿ��� ERowLayout::Button, EColLayout::Main ��ġ���� row span = 1, col span = 1 �� �����ϵ��� ��ġ
		pGridBackground->Add((int32_t)ERowLayout::Button, (int32_t)EColLayout::Button, m_pGridButtons);
		// ���� �׸��� ���̾ƿ��� ERowLayout::Main, EColLayout::Main ��ġ���� row span = 5, col span = 1 �� �����ϵ��� ��ġ
		pGridBackground->Add((int32_t)ERowLayout::Details, (int32_t)EColLayout::Details, 5, 1, m_pGridDetails);
		// �� ���� ���̾ƿ� ����� ó��
		m_pGridDetails->SetVisible(false);

		// ���̾�α׿� ��� ���̾ƿ� �߰�
		m_pDlgProgress->Add(pGridBackground);

		// ���̾�α� â�� ũ�Ⱑ ����� �� ��� ��ġ�� �������� â�� Ȯ�� �Ǵ� ������� ���� ��ġ�� ���մϴ�.
		m_pDlgProgress->SetResizeWindowPivot(eResizePivot);
		// ���̾�α� â�� ũ�Ⱑ ����� �� ����Ǳ� �� â�� �ʺ� �� ũ��, â�� �ʺ� �������� �ʰ� ���� �ʺ� �������� ���� ����
		m_pDlgProgress->KeepPreviousMaximumWidth(bKeepMaxWidth);
		// ���̾�α� â�� ũ�Ⱑ ����� �� ����Ǳ� �� â�� ���̰� �� ũ��, â�� ���̸� �������� �ʰ� ���� ���̸� �������� ���� ����
		m_pDlgProgress->KeepPreviousMaximumHeight(bKeepMaxHeight);
		// ���̾�α� â ����
		m_pDlgProgress->OnInitDialog();

		// �۾� ������ ����
		m_pThread = new std::future<void>(std::async(CPropertyMenuDialogProgressExample::AlgorithmThreadForComplexDialog, this, &m_pThread));
		SetThreadPriority(m_pThread, THREAD_PRIORITY_LOWEST);

		// ������ �Ķ���� �ʱ�ȭ
		ResetThreadParams();

		// �۾� �����忡�� ������Ʈ �� ���� �ݿ��Ͽ� ���̾�α� â ������Ʈ
		UpdateComplexDialog();

		// ���̾�α� �ݱ�
		CloseProgressDialog();

		cr = EResult_OK;
	}
	while(false);

	if(m_pThread)
	{
		delete m_pThread;
		m_pThread = nullptr;
	}

	return cr;
}

const CResult FLImaging::GUI::CPropertyMenuDialogProgressExample::CreateSimpleDialogWithSettings()
{
	CResult cr;

	do
	{
		CGUIPropertyItemDropdownList* pDropTextAlignment = dynamic_cast<CGUIPropertyItemDropdownList*>(FindItemByName(L"Text Alignment"));
		CGUIPropertyItemDropdownList* pDropResizePivot = dynamic_cast<CGUIPropertyItemDropdownList*>(FindItemByName(L"Resize Pivot"));
		CGUIPropertyItemCheckBox* pCheckKeepMaxWidth = dynamic_cast<CGUIPropertyItemCheckBox*>(FindItemByName(L"Keep Maximum Width"));
		CGUIPropertyItemCheckBox* pCheckKeepMaxHeight = dynamic_cast<CGUIPropertyItemCheckBox*>(FindItemByName(L"Keep Maximum Height"));

		EGUIAlignment eTextAlignment = GetAlignment(pDropTextAlignment->GetValue());
		EGUIAlignment eResizePivot = GetAlignment(pDropResizePivot->GetValue());
		bool bKeepMaxWidth = pCheckKeepMaxWidth->GetValue() == L"Checked";
		bool bKeepMaxHeight = pCheckKeepMaxHeight->GetValue() == L"Checked";

		// �ܼ��� ����� ǥ�� ���̾�α� ����
		CreateSimpleDialog();

		// ���̾�α��� �޽��� �ؽ�Ʈ ������ ����
		m_pGridMain->SetCtrlAlignment((int32_t)ERowMainSimple::Message, (int32_t)ERowMainSimple::Message, eTextAlignment);
		// ���̾�α� â�� ũ�Ⱑ ����� �� ��� ��ġ�� �������� â�� Ȯ�� �Ǵ� ������� ���� ��ġ�� ���մϴ�.
		m_pDlgProgress->SetResizeWindowPivot(eResizePivot);
		// ���̾�α� â�� ũ�Ⱑ ����� �� ����Ǳ� �� â�� �ʺ� �� ũ��, â�� �ʺ� �������� �ʰ� ���� �ʺ� �������� ���� ����
		m_pDlgProgress->KeepPreviousMaximumWidth(bKeepMaxWidth);
		// ���̾�α� â�� ũ�Ⱑ ����� �� ����Ǳ� �� â�� ���̰� �� ũ��, â�� ���̸� �������� �ʰ� ���� ���̸� �������� ���� ����
		m_pDlgProgress->KeepPreviousMaximumHeight(bKeepMaxHeight);
		// ���̾�α� â ����
		m_pDlgProgress->OnInitDialog();

		// �۾� ������ ����
		m_pThread = new std::future<void>(std::async(CPropertyMenuDialogProgressExample::AlgorithmThreadForSimpleDialog, this, &m_pThread));
		SetThreadPriority(m_pThread, THREAD_PRIORITY_LOWEST);

		// ������ �Ķ���� �ʱ�ȭ
		ResetThreadParams();

		// �۾� �����忡�� ������Ʈ �� ���� �ݿ��Ͽ� ���̾�α� â ������Ʈ
		UpdateSimpleDialogWithSettings();

		// ���̾�α� �ݱ�
		CloseProgressDialog();

		cr = EResult_OK;
	}
	while(false);

	if(m_pThread)
	{
		delete m_pThread;
		m_pThread = nullptr;
	}

	return cr;
}

const CResult FLImaging::GUI::CPropertyMenuDialogProgressExample::UpdateSimpleDialog()
{
	CResult cr;

	do
	{
		while(true)
		{
			// �����尡 �Ϸ�Ǹ� break
			if(m_bThreadDone)
				break;

			// ���� ��ư�� 1ȸ �̻� Ŭ�� �� break
			if(m_pStopButton->GetClickCount() >= 1)
				break;
			else
			{
				// ���� ������ ���ڿ��� ����
				CFLString<wchar_t> strMessage;
				strMessage.Format
				(
					L"CGUIDialogProgress example thread is working...\n(%d/%d)",
					m_i32CurrentProgress, m_i32TotalProgress
				);

				// m_pGridMain �� ERowMainSimple::Message, ERowMainSimple::Message ��ġ�� ���ڿ� �� �����Ͽ� ���̾�α� �󿡼� ���̴� �޼��� ����
				m_pGridMain->SetCtrlValue((int32_t)ERowMainSimple::Message, (int32_t)ERowMainSimple::Message, strMessage);

				// ���α׷��� �ٿ� ����� ǥ��
				m_pProgressCtrlFP->SetPos((double)m_i32CurrentProgress / (double)m_i32TotalProgress);
				// ���̾�α� â ũ�� ������Ʈ
				m_pDlgProgress->AdjustLayout();
			}
			// �޽��� ����
			CGUIManager::PeekAndPump();
			// Sleep �ð��� 1�� ����
			Sleep(1);
		}

		cr = EResult_OK;
	}
	while(false);

	return cr;
}

const CResult FLImaging::GUI::CPropertyMenuDialogProgressExample::UpdateSimpleDialogWithSettings()
{
	CResult cr;

	do
	{
		while(true)
		{
			// �����尡 �Ϸ�Ǹ� break
			if(m_bThreadDone)
				break;

			// ���� ��ư�� 1ȸ �̻� Ŭ�� �� break
			if(m_pStopButton->GetClickCount() >= 1)
				break;
			else
			{
				// ���� ������ ���ڿ��� ����
				CFLString<wchar_t> strMessage;
				strMessage.Format
				(
					m_vctVariousLengthMessages[m_i32CurrentProgress % (int32_t)m_vctVariousLengthMessages.size()] + L"\n(%d/%d)",
					m_i32CurrentProgress, m_i32TotalProgress
				);

				// m_pGridMain �� ERowMainSimple::Message, ERowMainSimple::Message ��ġ�� ���ڿ� �� �����Ͽ� ���̾�α� �󿡼� ���̴� �޼��� ����
				m_pGridMain->SetCtrlValue((int32_t)ERowMainSimple::Message, (int32_t)ERowMainSimple::Message, strMessage);

				// ���α׷��� �ٿ� ����� ǥ��
				m_pProgressCtrlFP->SetPos((double)m_i32CurrentProgress / (double)m_i32TotalProgress);
				// ���̾�α� â ũ�� ������Ʈ
				m_pDlgProgress->AdjustLayout();
			}
			// �޽��� ����
			CGUIManager::PeekAndPump();
			// Sleep �ð��� 100 ���� ����
			Sleep(100);
		}

		cr = EResult_OK;
	}
	while(false);

	return cr;
}

const CResult FLImaging::GUI::CPropertyMenuDialogProgressExample::UpdateComplexDialog()
{
	CResult cr;

	do
	{
		CFLString<wchar_t> strMessage;
		CFLString<wchar_t> strMessageFormat =
			CFLString<wchar_t>(L"[Step 3/3] Learning...\n\n") +
			CFLString<wchar_t>(L"<Cost>\n") +
			CFLString<wchar_t>(L"0.012132\n\n") +
			CFLString<wchar_t>(L"<Iteration>\n") +
			CFLString<wchar_t>(L"(%d/%d)\n\n") + // m_i32CurrentProgress/m_i32TotalProgress
			CFLString<wchar_t>(L"<Epoch>\n") +
			CFLString<wchar_t>(L"1/100\n\n") +
			CFLString<wchar_t>(L"<Validation>\n") +
			CFLString<wchar_t>(L"96.12%%at 1 epoch\n\n\n");

		while(true)
		{
			// �����尡 �Ϸ�Ǹ� while �� ���� ������
			if(m_bThreadDone)
				break;

			// ���� ��ư Ŭ�� Ƚ���� ����� ���
			if(m_pStopButton->GetClickCount() != m_i32BtnStopClickCount)
			{
				// ���� ��ư�� �� �� Ŭ���� ���
				if(m_pStopButton->GetClickCount() == 1)
				{
					// ���� ��ư �� ���ڿ��� "Continue.." �� ����
					m_pGridButtons->SetCtrlValue((int32_t)ERowButtonComplex::Stop, (int32_t)EColButton::Button, L"Continue ..");
				}
				// ���� ��ư�� �� �� Ŭ���� ��� ("Continue.." ��ư�� Ŭ���� ���)
				else if(m_pStopButton->GetClickCount() == 2)
				{
					// ���� ��ư �����
					m_pGridButtons->SetCtrlVisible((int32_t)ERowButtonComplex::Stop, (int32_t)EColButton::Button, false);
				}

				// "Details" ��ư �����
				m_pGridButtons->SetCtrlVisible((int32_t)ERowButtonComplex::Details, (int32_t)EColButton::Button, false);
				// �� ���� ���̾ƿ� �����
				m_pGridDetails->SetVisible(false);
				// ���� ��ư Ŭ�� Ƚ�� ������Ʈ
				m_i32BtnStopClickCount = m_pStopButton->GetClickCount();
				// ���̾�α��� ���̾ƿ� ������Ʈ
				m_pDlgProgress->AdjustLayout();
			}

			// ���� ��ư�� �� �� Ŭ���� ���
			if(m_pStopButton->GetClickCount() == 1)
			{
				// �޼��� ����
				strMessage.Format(L"[Step 3/3] Temp Message...\n\n<Temp Progress>");

				// m_pGridMain �� ERowMainComplex::Message, EColMainComplex::Message ��ġ�� ���ڿ� �� �����Ͽ� ���̾�α� �󿡼� ���̴� �޼��� ����
				m_pGridMain->SetCtrlValue((int32_t)ERowMainComplex::Message, (int32_t)EColMainComplex::Message, strMessage);

				// ���α׷��� �ٰ� 100% �� ä�������� ����
				m_pProgressCtrlFP->SetPos(1.);
				// ���̾�α� ȭ�� ����
				m_pDlgProgress->Invalidate();
				// �޽��� ����
				CGUIManager::PeekAndPump();
				Sleep(1);
				continue;
			}
			// ���� ��ư�� �� �� Ŭ���� ���
			else if(m_pStopButton->GetClickCount() == 2)
			{
				// ���� ���μ��� ������ ���� �Ϸ���� ���� ���
				if(m_i32CurrentStopProgress <= m_i32TotalStopProgress)
				{
					// �޽��� ����
					strMessage.Format(L"[Step 3/3] Learning is being stopped...\n\n<Stop Progress>\n(%d/%d)", m_i32CurrentStopProgress, m_i32TotalStopProgress);
					m_pGridMain->SetCtrlValue((int32_t)ERowMainComplex::Message, (int32_t)EColMainComplex::Message, strMessage);

					// ���α׷��� �ٿ� ����� ����
					m_pProgressCtrlFP->SetPos((double)m_i32CurrentStopProgress / (double)m_i32TotalStopProgress);

					// ���̾�α� ȭ�� ����
					m_pDlgProgress->Invalidate();
				}
				else // ���� ���μ��� ������ ���� �Ϸ�� ��� while ������ break
					break;

				// �޽��� ����
				CGUIManager::PeekAndPump();
				Sleep(1);
				continue;
			}

			// ���� ��ư�� �� ���� ������ ���� ���

			// �޽��� ����
			strMessage.Format
			(
				strMessageFormat,
				m_i32CurrentProgress, m_i32TotalProgress
			);
			m_pGridMain->SetCtrlValue((int32_t)ERowMainComplex::Message, (int32_t)EColMainComplex::Message, strMessage);

			// ���α׷��� �ٿ� ����� ����
			m_pProgressCtrlFP->SetPos((double)m_i32CurrentProgress / (double)m_i32TotalProgress);

			// ���̾�α� ȭ�� ����
			m_pDlgProgress->Invalidate();
			// �޽��� ����
			CGUIManager::PeekAndPump();
			Sleep(1);
		}

		cr = EResult_OK;
	}
	while(false);

	return cr;
}

const CResult FLImaging::GUI::CPropertyMenuDialogProgressExample::CreateComplexProgressDialogWithSheet(bool bBoxLayout)
{
	CResult cr;

	do
	{
		CloseProgressDialog();

		CGUIPropertyItemDropdownList* pDropTextAlignment = dynamic_cast<CGUIPropertyItemDropdownList*>(FindItemByName(L"Text Alignment"));
		CGUIPropertyItemDropdownList* pDropResizePivot = dynamic_cast<CGUIPropertyItemDropdownList*>(FindItemByName(L"Resize Pivot"));
		CGUIPropertyItemCheckBox* pCheckKeepMaxWidth = dynamic_cast<CGUIPropertyItemCheckBox*>(FindItemByName(L"Keep Maximum Width"));
		CGUIPropertyItemCheckBox* pCheckKeepMaxHeight = dynamic_cast<CGUIPropertyItemCheckBox*>(FindItemByName(L"Keep Maximum Height"));

		EGUIAlignment eTextAlignment = GetAlignment(pDropTextAlignment->GetValue());
		EGUIAlignment eResizePivot = GetAlignment(pDropResizePivot->GetValue());
		bool bKeepMaxWidth = pCheckKeepMaxWidth->GetValue() == L"Checked";
		bool bKeepMaxHeight = pCheckKeepMaxHeight->GetValue() == L"Checked";

		// ���̾�α� ����
		if(!m_pDlgProgress)
			m_pDlgProgress = CGUIDialogProgress::CreateModelessDialog(AfxGetApp()->m_pMainWnd);

		CFLString<wchar_t> strMessage;
		CFLString<wchar_t> strMessageFormat =
			CFLString<wchar_t>(L"[Step 3/3] Learning...\n\n") +
			CFLString<wchar_t>(L"<Cost>\n") +
			CFLString<wchar_t>(L"0.012132\n\n") +
			CFLString<wchar_t>(L"<Iteration>\n") +
			CFLString<wchar_t>(L"(%d/%d)\n\n") + // m_i32CurrentProgress/m_i32TotalProgress
			CFLString<wchar_t>(L"<Epoch>\n") +
			CFLString<wchar_t>(L"1/100\n\n") +
			CFLString<wchar_t>(L"<Validation>\n") +
			CFLString<wchar_t>(L"96.12%%at 1 epoch\n\n\n");

		// �޽���, ���α׷��� ��, �ð� ���� ǥ�õǴ� ���� �׸��� ���̾ƿ� ����
		m_pGridMain = new CGUIGridLayout((int32_t)ERowMainComplex::Count, (int32_t)EColMainComplex::Count);
		{
			strMessage.Format
			(
				strMessageFormat,
				m_i32CurrentProgress, m_i32TotalProgress
			);

			// �޽����� �׸��� ���̾ƿ��� �߰�
			m_pGridMain->AddCtrl((int32_t)ERowMainComplex::Message, (int32_t)EColMainComplex::Message, 3, 4, strMessage);
			// �޽����� �ؽ�Ʈ ���� �ɼ��� "Text Alignment" ���� ��� �� ������ ����
			m_pGridMain->SetCtrlAlignment((int32_t)ERowMainComplex::Message, (int32_t)EColMainComplex::Message, eTextAlignment);
			// ���α׷��� �ٸ� �׸��� ���̾ƿ��� �߰�
			m_pGridMain->AddCtrl((int32_t)ERowMainComplex::ProgressCtrl, (int32_t)EColMainComplex::ProgressCtrl, 1, 4, EControl_ProgressBarFloatingPoint);
			// �ҿ� �ð��� �׸��� ���̾ƿ��� �߰�
			m_pGridMain->AddCtrl((int32_t)ERowMainComplex::ElapsedTime, (int32_t)EColMainComplex::ElapsedTime, 1, 4, L"Elapsed Time      00 : 00 : 00");
			// �ҿ� �ð��� �ؽ�Ʈ ���� �ɼ��� ����, ���� ��� �߰� ���ķ� ����
			m_pGridMain->SetCtrlAlignment((int32_t)ERowMainComplex::ElapsedTime, (int32_t)EColMainComplex::ElapsedTime, EGUIAlignment_CENTER_CENTER);
			// �ҿ� �ð��� m_pGridMain ���̾ƿ��� (int32_t)ERowMainComplex::ElapsedTime, (int32_t)EColMainComplex::ElapsedTime ��ġ�� �ִٴ� ���� m_pDlgProgress �� �˷���
			m_pDlgProgress->SetElapsedTimePosition(m_pGridMain, (int32_t)ERowMainComplex::ElapsedTime, (int32_t)EColMainComplex::ElapsedTime);
			// ����� ������Ʈ�� ���� ���α׷��� �� ��ü �����͸� ��� ����
			m_pProgressCtrlFP = (CGUIProgressCtrlFloatingPoint*)m_pGridMain->GetCtrl((int32_t)ERowMainComplex::ProgressCtrl, (int32_t)EColMainComplex::ProgressCtrl);
		}

		// ��ư�� ǥ�õǴ� ��ư �׸��� ���̾ƿ� ����
		m_pGridButtons = new CGUIGridLayout((int32_t)ERowButtonComplex::Count, (int32_t)EColButton::Count);
		{
			// �� ���� ��ư�� �׸��� ���̾ƿ��� �߰�
			m_pGridButtons->AddCtrl((int32_t)ERowButtonComplex::Details, (int32_t)EColButton::Button, EControl_Button, L"See Details");
			// "See Details" ��ư Ŭ�� �̺�Ʈ ó���� ����
			CWndCtrlProcedure* pVCP = new CWndCtrlProcedure;
			// "See Details" ��ư Ŭ�� �� ����Ǵ� ���� ����
			*pVCP = MakeWndCtrlProcedure
			{
				// "See Details" ��ư�� Ȧ�� �� ���� ���
				if(((CGUIButton*)m_pGridButtons->GetCtrl((int32_t)ERowButtonComplex::Details, (int32_t)EColButton::Button))->GetClickCount() % 2 == 1)
				{
					// ��Ʈ�� ���Ե� ���̾ƿ��� show
					m_pLayoutForSheet->SetVisible(true);
					// "See Details" ��ư�� �ؽ�Ʈ�� "Close Details" �� ����
					m_pGridButtons->SetCtrlValue((int32_t)ERowButtonComplex::Details, (int32_t)EColButton::Button, L"Close Details");
				}
				// "See Details" ��ư�� ¦�� �� ���� ���
				else
				{
					// ��Ʈ�� ���Ե� ���̾ƿ��� hide
					m_pLayoutForSheet->SetVisible(false);
					// "Close Details" ��ư�� �ؽ�Ʈ�� "See Details" �� ����
					m_pGridButtons->SetCtrlValue((int32_t)ERowButtonComplex::Details, (int32_t)EColButton::Button, L"See Details");
				}

				// ���̾�α��� ���̾ƿ��� ������Ʈ
				m_pDlgProgress->AdjustLayout();

				return true;
			};
			// "See Details" ��ư�� Ŭ�� �̺�Ʈ ó���� �߰�
			m_pGridButtons->SetWndCtrlProcedure((int32_t)ERowButtonComplex::Details, (int32_t)EColButton::Button, pVCP);

			// ���� ��ư�� �׸��� ���̾ƿ��� �߰�
			m_pGridButtons->AddCtrl((int32_t)ERowButtonComplex::Stop, (int32_t)EColButton::Button, EControl_Button, L"Stop");
			// ���� ��ư ��ü �����͸� ��� ����
			m_pStopButton = (CGUIButton*)m_pGridButtons->GetCtrl((int32_t)ERowButtonComplex::Stop, (int32_t)EColButton::Button);
		}

		// CFLSheet �� ���Ե� �ڽ� ���̾ƿ�
		if(bBoxLayout)
		{
			m_pLayoutForSheet = new CGUIBoxLayout(EOrientation_Vertical, 300, 500, true);

			// ��Ʈ �߰�
			((CGUIBoxLayout*)m_pLayoutForSheet)->AddCtrl(EControl_Sheet);
			((CGUIBoxLayout*)m_pLayoutForSheet)->AddCtrl(EControl_Sheet);
		}
		else
		{
			m_pLayoutForSheet = new CGUIGridLayout(14, 2, 300, 500, true);

			((CGUIGridLayout*)m_pLayoutForSheet)->AddCtrl(0, 0, L"Detail 1");
			((CGUIGridLayout*)m_pLayoutForSheet)->AddCtrl(1, 0, L"Detail 2");
			((CGUIGridLayout*)m_pLayoutForSheet)->AddCtrl(2, 0, L"Detail 3");
			((CGUIGridLayout*)m_pLayoutForSheet)->AddCtrl(0, 1, EControl_EditCtrl);
			((CGUIGridLayout*)m_pLayoutForSheet)->AddCtrl(1, 1, EControl_EditCtrl);
			((CGUIGridLayout*)m_pLayoutForSheet)->AddCtrl(2, 1, EControl_EditCtrl);

			// ��Ʈ �߰�
			((CGUIGridLayout*)m_pLayoutForSheet)->AddCtrl(3, 0, 6, 2, EControl_Sheet);
			((CGUIGridLayout*)m_pLayoutForSheet)->AddCtrl(9, 0, 5, 2, EControl_Sheet);
		}

		{
			// 0 ��° ��Ʈ
			{
				// �߰��� ��Ʈ ��� ����
				CGUISheetInterface* pSheetInterface = nullptr;

				if(bBoxLayout)
					pSheetInterface = ((CGUIBoxLayout*)m_pLayoutForSheet)->GetSheetInterface(0);
				else
					pSheetInterface = ((CGUIGridLayout*)m_pLayoutForSheet)->GetSheetInterface(3, 0);

				// ��Ʈ�� ������ ������ ��� ����
				CFLArray<CFLArray<CFLString<wchar_t>>>* pData = pSheetInterface->GetDataPtr();

				int32_t i32ColCnt = 10; // column ����
				int32_t i32RowCnt = 10; // row ����
				int32_t i32FixedColCnt = 1; // ���� column ����
				int32_t i32FixedRowCnt = 1; // ���� row ����

				// ��Ʈ�� ���÷��� �� ���� �߰�
				for(int32_t r = 0; r < i32FixedRowCnt; r++)
				{
					pData->Append(CFLArray<CFLString<wchar_t>>());
					CFLArray<CFLString<wchar_t>>& arrRow = pData->Back();

					for(int32_t c = 0; c < i32ColCnt; c++)
						arrRow.Append(CFLString<wchar_t>().Format(L"Fixed (%d,%d)", r, c));
				}

				for(int32_t r = i32FixedRowCnt; r < i32RowCnt; r++)
				{
					pData->Append(CFLArray<CFLString<wchar_t>>());
					CFLArray<CFLString<wchar_t>>& arrRow = pData->Back();

					for(int32_t c = 0; c < i32FixedColCnt; c++)
						arrRow.Append(CFLString<wchar_t>().Format(L"Fixed (%d,%d)", r, c));

					for(int32_t c = i32FixedColCnt; c < i32ColCnt; c++)
						arrRow.Append(CFLString<wchar_t>().Format(L"(%d,%d)", r, c));
				}

				// �����Ϳ� �߰��� ������ ��Ʈ�� ������Ʈ
				pSheetInterface->UpdateSheetData();

				// ���� column, row ���� ����
				pSheetInterface->SetFixedRowCount(i32FixedRowCnt);
				pSheetInterface->SetFixedColumnCount(i32FixedColCnt);
			}

			// 1 ��° ��Ʈ
			{
				// �߰��� ��Ʈ ��� ����
				CGUISheetInterface* pSheetInterface = nullptr;

				if(bBoxLayout)
					pSheetInterface = ((CGUIBoxLayout*)m_pLayoutForSheet)->GetSheetInterface(1);
				else
					pSheetInterface = ((CGUIGridLayout*)m_pLayoutForSheet)->GetSheetInterface(9, 0);

				// ��Ʈ�� ������ ������ ��� ����
				CFLArray<CFLArray<CFLString<wchar_t>>>* pData = pSheetInterface->GetDataPtr();

				int32_t i32ColCnt = 10; // column ����
				int32_t i32RowCnt = 10; // row ����
				int32_t i32FixedColCnt = 1; // ���� column ����
				int32_t i32FixedRowCnt = 1; // ���� row ����

				// ��Ʈ�� ���÷��� �� ���� �߰�
				for(int32_t r = 0; r < i32FixedRowCnt; r++)
				{
					pData->Append(CFLArray<CFLString<wchar_t>>());
					CFLArray<CFLString<wchar_t>>& arrRow = pData->Back();

					for(int32_t c = 0; c < i32ColCnt; c++)
						arrRow.Append(CFLString<wchar_t>().Format(L"Fixed (%d,%d)", r, c));
				}

				for(int32_t r = i32FixedRowCnt; r < i32RowCnt; r++)
				{
					pData->Append(CFLArray<CFLString<wchar_t>>());
					CFLArray<CFLString<wchar_t>>& arrRow = pData->Back();

					for(int32_t c = 0; c < i32FixedColCnt; c++)
						arrRow.Append(CFLString<wchar_t>().Format(L"Fixed (%d,%d)", r, c));

					for(int32_t c = i32FixedColCnt; c < i32ColCnt; c++)
						arrRow.Append(CFLString<wchar_t>().Format(L"(%d,%d)", r, c));
				}

				// �����Ϳ� �߰��� ������ ��Ʈ�� ������Ʈ
				pSheetInterface->UpdateSheetData();

				// ���� column, row ���� ����
				pSheetInterface->SetFixedRowCount(i32FixedRowCnt);
				pSheetInterface->SetFixedColumnCount(i32FixedColCnt);
			}
		}

		// ���̾ƿ����� ��ġ�ϱ� ���� ��� ���̾ƿ� ����
		CGUIGridLayout* pGridBackground = new CGUIGridLayout((int32_t)ERowLayout::Count, (int32_t)EColLayout::Count);

		// ���� �׸��� ���̾ƿ��� ERowLayout::Main, EColLayout::Main ��ġ���� row span = 4, col span = 1 �� �����ϵ��� ��ġ
		pGridBackground->Add((int32_t)ERowLayout::Main, (int32_t)EColLayout::Main, 4, 1, m_pGridMain);
		// ��ư �׸��� ���̾ƿ��� ERowLayout::Button, EColLayout::Main ��ġ���� row span = 1, col span = 1 �� �����ϵ��� ��ġ
		pGridBackground->Add((int32_t)ERowLayout::Button, (int32_t)EColLayout::Button, m_pGridButtons);
		// ���� �׸��� ���̾ƿ��� ERowLayout::Main, EColLayout::Main ��ġ���� row span = 5, col span = 1 �� �����ϵ��� ��ġ
		pGridBackground->Add((int32_t)ERowLayout::Details, (int32_t)EColLayout::Details, 5, 1, m_pLayoutForSheet);
		// �� ���� ���̾ƿ� ����� ó��
		m_pLayoutForSheet->SetVisible(false);

		// ���̾�α׿� ��� ���̾ƿ� �߰�
		m_pDlgProgress->Add(pGridBackground);

		// ���̾�α� â�� ũ�Ⱑ ����� �� ��� ��ġ�� �������� â�� Ȯ�� �Ǵ� ������� ���� ��ġ�� ���մϴ�.
		m_pDlgProgress->SetResizeWindowPivot(eResizePivot);
		// ���̾�α� â�� ũ�Ⱑ ����� �� ����Ǳ� �� â�� �ʺ� �� ũ��, â�� �ʺ� �������� �ʰ� ���� �ʺ� �������� ���� ����
		m_pDlgProgress->KeepPreviousMaximumWidth(bKeepMaxWidth);
		// ���̾�α� â�� ũ�Ⱑ ����� �� ����Ǳ� �� â�� ���̰� �� ũ��, â�� ���̸� �������� �ʰ� ���� ���̸� �������� ���� ����
		m_pDlgProgress->KeepPreviousMaximumHeight(bKeepMaxHeight);
		// ���̾�α� â ����
		m_pDlgProgress->OnInitDialog();

		// �۾� ������ ����
		m_pThread = new std::future<void>(std::async(CPropertyMenuDialogProgressExample::AlgorithmThreadForComplexDialog, this, &m_pThread));
		SetThreadPriority(m_pThread, THREAD_PRIORITY_LOWEST);

		// ������ �Ķ���� �ʱ�ȭ
		ResetThreadParams();

		// �۾� �����忡�� ������Ʈ �� ���� �ݿ��Ͽ� ���̾�α� â ������Ʈ
		UpdateComplexDialogWithSheet();

		// ���̾�α� �ݱ�
		CloseProgressDialog();

		cr = EResult_OK;
	}
	while(false);

	if(m_pThread)
	{
		delete m_pThread;
		m_pThread = nullptr;
	}

	return cr;
}

const CResult FLImaging::GUI::CPropertyMenuDialogProgressExample::UpdateComplexDialogWithSheet()
{
	CResult cr;

	do
	{
		CFLString<wchar_t> strMessage;
		CFLString<wchar_t> strMessageFormat =
			CFLString<wchar_t>(L"[Step 3/3] Learning...\n\n") +
			CFLString<wchar_t>(L"<Cost>\n") +
			CFLString<wchar_t>(L"0.012132\n\n") +
			CFLString<wchar_t>(L"<Iteration>\n") +
			CFLString<wchar_t>(L"(%d/%d)\n\n") + // m_i32CurrentProgress/m_i32TotalProgress
			CFLString<wchar_t>(L"<Epoch>\n") +
			CFLString<wchar_t>(L"1/100\n\n") +
			CFLString<wchar_t>(L"<Validation>\n") +
			CFLString<wchar_t>(L"96.12%%at 1 epoch\n\n\n");

		while(true)
		{
			// �����尡 �Ϸ�Ǹ� while �� ���� ������
			if(m_bThreadDone)
				break;

			// ���� ��ư Ŭ�� Ƚ���� ����� ���
			if(m_pStopButton->GetClickCount() != m_i32BtnStopClickCount)
			{
				// ���� ��ư�� �� �� Ŭ���� ���
				if(m_pStopButton->GetClickCount() == 1)
				{
					// ���� ��ư �� ���ڿ��� "Continue.." �� ����
					m_pGridButtons->SetCtrlValue((int32_t)ERowButtonComplex::Stop, (int32_t)EColButton::Button, L"Continue ..");
				}
				// ���� ��ư�� �� �� Ŭ���� ��� ("Continue.." ��ư�� Ŭ���� ���)
				else if(m_pStopButton->GetClickCount() == 2)
				{
					// ���� ��ư �����
					m_pGridButtons->SetCtrlVisible((int32_t)ERowButtonComplex::Stop, (int32_t)EColButton::Button, false);
				}

				// "Details" ��ư �����
				m_pGridButtons->SetCtrlVisible((int32_t)ERowButtonComplex::Details, (int32_t)EColButton::Button, false);
				// Sheet �� ���Ե� ���̾ƿ� �����
				m_pLayoutForSheet->SetVisible(false);
				// ���� ��ư Ŭ�� Ƚ�� ������Ʈ
				m_i32BtnStopClickCount = m_pStopButton->GetClickCount();
				// ���̾�α��� ���̾ƿ� ������Ʈ
				m_pDlgProgress->AdjustLayout();
			}

			// ���� ��ư�� �� �� Ŭ���� ���
			if(m_pStopButton->GetClickCount() == 1)
			{
				// �޼��� ����
				strMessage.Format(L"[Step 3/3] Temp Message...\n\n<Temp Progress>");

				// m_pGridMain �� ERowMainComplex::Message, EColMainComplex::Message ��ġ�� ���ڿ� �� �����Ͽ� ���̾�α� �󿡼� ���̴� �޼��� ����
				m_pGridMain->SetCtrlValue((int32_t)ERowMainComplex::Message, (int32_t)EColMainComplex::Message, strMessage);

				// ���α׷��� �ٰ� 100% �� ä�������� ����
				m_pProgressCtrlFP->SetPos(1.);
				// ���̾�α� ȭ�� ����
				m_pDlgProgress->Invalidate();
				// �޽��� ����
				CGUIManager::PeekAndPump();
				Sleep(1);
				continue;
			}
			// ���� ��ư�� �� �� Ŭ���� ���
			else if(m_pStopButton->GetClickCount() == 2)
			{
				// ���� ���μ��� ������ ���� �Ϸ���� ���� ���
				if(m_i32CurrentStopProgress <= m_i32TotalStopProgress)
				{
					// �޽��� ����
					strMessage.Format(L"[Step 3/3] Learning is being stopped...\n\n<Stop Progress>\n(%d/%d)", m_i32CurrentStopProgress, m_i32TotalStopProgress);
					m_pGridMain->SetCtrlValue((int32_t)ERowMainComplex::Message, (int32_t)EColMainComplex::Message, strMessage);

					// ���α׷��� �ٿ� ����� ����
					m_pProgressCtrlFP->SetPos((double)m_i32CurrentStopProgress / (double)m_i32TotalStopProgress);

					// ���̾�α� ȭ�� ����
					m_pDlgProgress->Invalidate();
				}
				else // ���� ���μ��� ������ ���� �Ϸ�� ��� while ������ break
					break;

				// �޽��� ����
				CGUIManager::PeekAndPump();
				Sleep(1);
				continue;
			}

			// ���� ��ư�� �� ���� ������ ���� ���

			// �޽��� ����
			strMessage.Format
			(
				strMessageFormat,
				m_i32CurrentProgress, m_i32TotalProgress
			);
			m_pGridMain->SetCtrlValue((int32_t)ERowMainComplex::Message, (int32_t)EColMainComplex::Message, strMessage);

			// ���α׷��� �ٿ� ����� ����
			m_pProgressCtrlFP->SetPos((double)m_i32CurrentProgress / (double)m_i32TotalProgress);

			// ���̾�α� ȭ�� ����
			m_pDlgProgress->Invalidate();
			// �޽��� ����
			CGUIManager::PeekAndPump();
			Sleep(1);
		}

		cr = EResult_OK;
	}
	while(false);

	return cr;
}

const CResult FLImaging::GUI::CPropertyMenuDialogProgressExample::CloseProgressDialog()
{
	CResult cr;

	do
	{
		if(m_pDlgProgress)
		{
			m_pDlgProgress->DestroyWindow();
			m_pDlgProgress = nullptr;
		}

		cr = EResult_OK;
	}
	while(false);

	return cr;
}

void FLImaging::GUI::CPropertyMenuDialogProgressExample::AlgorithmThreadForComplexDialog(CPropertyMenuDialogProgressExample* pInstance, std::future<void>** pContext)
{
	// ������ ���̾�α׿��� ���Ǵ� ������

	if(pInstance->m_pDlgProgress)
	{
		while(true)
		{
			// ���� ���� �۾� ���෮�� ��ü ���� �۾� ���෮ �̻��� �� ���
			if(pInstance->m_i32CurrentStopProgress >= pInstance->m_i32TotalStopProgress)
			{
				// �����尡 ����Ǿ����� �˸��� �÷��׸� true �� ���� �� break
				pInstance->m_bThreadDone = true;
				break;
			}

			// ��ư�� 2ȸ �̻� Ŭ���� ���
			if(pInstance->m_pStopButton->GetClickCount() >= 2)
			{
				Sleep(1);
				// ���� �۾� ���෮ ���� ������Ŵ
				++pInstance->m_i32CurrentStopProgress;
			}
			else
			{
				// ���� �۾� ���෮�� ��ü �۾� ���෮ �̻��� �� ���
				if(pInstance->m_i32CurrentProgress >= pInstance->m_i32TotalProgress)
				{
					// �����尡 ����Ǿ����� �˸��� �÷��׸� true �� ���� �� break
					pInstance->m_bThreadDone = true;
					break;
				}

				Sleep(1);
				// �۾� ���෮�� ���� ������Ŵ
				++pInstance->m_i32CurrentProgress;
			}
		}
	}
}

void FLImaging::GUI::CPropertyMenuDialogProgressExample::AlgorithmThreadForSimpleDialog(CPropertyMenuDialogProgressExample* pInstance, std::future<void>** pContext)
{
	// �ܼ��� ���̾�α׿��� ���Ǵ� ������

	if(pInstance && pInstance->m_pDlgProgress)
	{
		while(true)
		{
			// ��ư�� 1ȸ �̻� Ŭ���� ���
			if(pInstance->m_pStopButton->GetClickCount() >= 1)
			{
				// �����尡 ����Ǿ����� �˸��� �÷��׸� true �� ���� �� break
				pInstance->m_bThreadDone = true;
				break;
			}
			else
			{
				// ���� �۾� ���෮�� ��ü �۾� ���෮ �̻��� �� ���
				if(pInstance->m_i32CurrentProgress >= pInstance->m_i32TotalProgress)
				{
					// �����尡 ����Ǿ����� �˸��� �÷��׸� true �� ���� �� break
					pInstance->m_bThreadDone = true;
					break;
				}

				Sleep(1);
				// �۾� ���෮�� ���� ������Ŵ
				++pInstance->m_i32CurrentProgress;
			}
		}
	}
}

EGUIAlignment FLImaging::GUI::CPropertyMenuDialogProgressExample::GetAlignment(const Base::CFLString<wchar_t>& strVal) const
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

void FLImaging::GUI::CPropertyMenuDialogProgressExample::ResetThreadParams()
{
	// ������ �Ķ���� ���� �ʱ�ȭ �մϴ�.
	m_bThreadDone = false;
	m_i32CurrentStopProgress = 0;
	m_i32CurrentProgress = 0;
	m_i32TotalProgress = 2000;
	m_i32TotalStopProgress = 20;
	m_i32BtnStopClickCount = 0;
}
