#include "stdafx.h"
#include "PropertyMenuFullyConnected_XOR.h"

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

FLImaging::GUI::CPropertyMenuFullyConnectedXOR::CPropertyMenuFullyConnectedXOR()
	: CGUIPropertyMenuBase()
	, m_pDlgProgress(nullptr)
	, m_pThread(nullptr)
	, m_pProgressCtrlFP(nullptr)
	, m_pStopButton(nullptr)
	, m_pGridMain(nullptr)
	, m_pGridButtons(nullptr)
	, m_pGridDetails(nullptr)
	, m_i32BtnStopClickCount(0)
	, m_tsrEvaluation(*new CTensor<float>)
	, m_tsrCost(*new CTensor<float>)
	, m_tsrAnswer(*new CTensor<float>)
	, m_pOptAdam(nullptr)
	, m_pGfCost(nullptr)
	, m_pGfEvaluation(nullptr)
	, m_pPane(nullptr)
	, m_pViewGraph(nullptr)
{
}

FLImaging::GUI::CPropertyMenuFullyConnectedXOR::~CPropertyMenuFullyConnectedXOR()
{
	CloseProgressDialog();

	if(m_pThread)
	{
		delete m_pThread;
		m_pThread = nullptr;
	}

	delete& m_tsrCost;
	delete& m_tsrEvaluation;
	delete& m_tsrAnswer;
}

const CResult FLImaging::GUI::CPropertyMenuFullyConnectedXOR::ConfigureMenu()
{
	CResult cReturn = EResult_UnknownError;

	do
	{
		CGUIPropertyButton* pBtn = new CGUIPropertyButton;
		pBtn->SetName(L"XOR Test");
		pBtn->SetPropertyButtonClickProcedure(TrainXOR());
		pBtn->SetButtonWidthRatio(1.);
		AddButton(pBtn);
		SetShortcut(EKeyType_F6, pBtn);

		cReturn = EResult_OK;
	}
	while(false);

	return cReturn;
}

CPropertyButtonClickProcedure* FLImaging::GUI::CPropertyMenuFullyConnectedXOR::TrainXOR()
{
	CPropertyButtonClickProcedure* pProcedure = new CPropertyButtonClickProcedure;
	*pProcedure = MakePropertyButtonClickProcedure
	{
		CResult res = EResult_UnknownError;

		CGUIPaneProperties* pPane = GetPaneProperties();
		pPane->EnableWindow(false);

		do
		{
			CreateSimpleDialog();
			// ���̾�α� â ����
			m_pDlgProgress->OnInitDialog();
			m_pDlgProgress->SetWindowPos(nullptr, 850, 0, 300, 500, SWP_NOZORDER);

			float f32LearningRate = 1e-1f;
			float f32Beta1 = 0.9f;
			float f32Beta2 = 0.999f;
			float f32Epsilon = 1e-8f;

			//XOR Input
			CTensor<float> tsrX;

			tsrX =
			{
				{ 0.f, 0.f },
				{ 0.f, 1.f },
				{ 1.f, 0.f },
				{ 1.f, 1.f },
			};

			//XOR Output
			CTensor<float> tsrY;

			tsrY =
			{
				{ 0.f },
				{ 1.f },
				{ 1.f },
				{ 0.f },
			};

			CComputationalGraphPlaceholderF gfPlaceHolderX, gfPlaceHolderY;
			gfPlaceHolderX.SetID(L"X");
			gfPlaceHolderX.SetValueAttribute(EValueAttribute_Input);
			gfPlaceHolderY.SetID(L"Y");
			gfPlaceHolderY.SetValueAttribute(EValueAttribute_Input);

			gfPlaceHolderX.SetOperand(tsrX);
			gfPlaceHolderY.SetOperand(tsrY);

			m_tsrAnswer = tsrY;

			//Layer�� �ʿ��� Weight Tensor ����
			CTensor<float> tsrW1;
			tsrW1.Create({ 2, 10 });
			tsrW1.FillHe();
			tsrW1.SetReference(false);
			tsrW1.SetValueAttribute(EValueAttribute_Variable);
			CTensor<float> tsrB1;
			tsrB1.Create({ 10 });
			tsrB1.FillZero();
			tsrB1.SetReference(false);
			tsrB1.SetValueAttribute(EValueAttribute_Variable);

			CTensor<float> tsrWH1;
			tsrWH1.Create({ 10, 10 });
			tsrWH1.FillHe();
			tsrWH1.SetReference(false);
			tsrWH1.SetValueAttribute(EValueAttribute_Variable);
			CTensor<float> tsrBH1;
			tsrBH1.Create({ 10 });
			tsrBH1.FillZero();
			tsrBH1.SetReference(false);
			tsrBH1.SetValueAttribute(EValueAttribute_Variable);

			CTensor<float> tsrWH2;
			tsrWH2.Create({ 10, 10 });
			tsrWH2.FillHe();
			tsrWH2.SetReference(false);
			tsrWH2.SetValueAttribute(EValueAttribute_Variable);
			CTensor<float> tsrBH2;
			tsrBH2.Create({ 10 });
			tsrBH2.FillZero();
			tsrBH2.SetReference(false);
			tsrBH2.SetValueAttribute(EValueAttribute_Variable);

			CTensor<float> tsrW2;
			tsrW2.Create({ 10, 1 });
			tsrW2.FillHe();
			tsrW2.SetReference(false);
			tsrW2.SetValueAttribute(EValueAttribute_Variable);
			CTensor<float> tsrB2;
			tsrB2.Create({ 1 });
			tsrB2.FillZero();
			tsrB2.SetReference(false);
			tsrB2.SetValueAttribute(EValueAttribute_Variable);

			// XOR Graph Layer ����
			CCGFNodeVar(gfLayer0, CCGFSigmoid(CCGFAffine(gfPlaceHolderX, tsrW1, tsrB1)));
			CCGFNodeVar(gfLayerH1, CCGFSigmoid(CCGFAffine(gfLayer0, tsrWH1, tsrBH1)));
			CCGFNodeVar(gfLayerH2, CCGFSigmoid(CCGFAffine(gfLayerH1, tsrWH2, tsrBH2)));
			CCGFNodeVar(gfHypothesis, CCGFSigmoid(CCGFAffine(gfLayerH2, tsrW2, tsrB2)));
			CCGFNodeVar(gfCost, CCGFMultiply(CCGFReduceMean(CCGFAdd(CCGFMultiply(gfPlaceHolderY, CCGFLog(gfHypothesis)), CCGFMultiply(CCGFSubtract(1, gfPlaceHolderY), CCGFLog(CCGFSubtract(1, gfHypothesis)))), {0}), -1));

			CInternalOptimizerAdamGD<float> adam;
			CLRSConstant<float> lrs;

			//Optimizer parameter ����
			lrs.SetLearningRate(f32LearningRate);
			adam.SetLearningRateScheduler(lrs);

			m_pGfCost = &gfCost;
			m_pGfEvaluation = &gfHypothesis;

			adam.SetFunction(*m_pGfCost);
			adam.SetBeta1(f32Beta1);
			adam.SetBeta2(f32Beta2);
			adam.SetEpsilon(f32Epsilon);
			adam.Initialize();
			m_pOptAdam = &adam;

			// �۾� ������ ����
			m_pThread = new std::future<void>(std::async(CPropertyMenuFullyConnectedXOR::AlgorithmThreadForSimpleDialog, this, &m_pThread));
			SetThreadPriority(m_pThread, THREAD_PRIORITY_LOWEST);

			// ������ �Ķ���� �ʱ�ȭ
			ResetThreadParams();

			// �۾� �����忡�� ������Ʈ �� ���� �ݿ��Ͽ� ���̾�α� â ������Ʈ
			UpdateSimpleDialog();

			// ���̾�α� �ݱ�
			CloseProgressDialog();

			res = EResult_OK;
		}
		while(false);

		if(m_pThread)
		{
			delete m_pThread;
			m_pThread = nullptr;
		}

		pPane->EnableWindow(true);
	};

	return pProcedure;
}

const CResult FLImaging::GUI::CPropertyMenuFullyConnectedXOR::CreateSimpleDialog()
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
				L"Fully-Connected XOR Network Training...(%d / %d)\n",
				m_i32Epoch, m_i32MaxEpoch
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

		m_pPane = CGUIManagerPane::GetPaneViewGraph();

		if(m_pPane)
		{
			m_pViewGraph = m_pPane->GetViewGraph();

			if(m_pViewGraph)
			{
				m_pViewGraph->ClearAll();
				m_pViewGraph->Invalidate();
			}
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

const CResult FLImaging::GUI::CPropertyMenuFullyConnectedXOR::UpdateSimpleDialog()
{
	CResult cr;

	do
	{
		while(true)
		{
			// �����尡 �Ϸ�Ǹ� break
			if(m_bThreadDone)
			{
				if(m_tsrCost.IsCudaActivated())
				{
					m_tsrCost.CopyDeviceToHost();
					m_tsrEvaluation.CopyDeviceToHost();
				}

				// ���� ��ư �� ���ڿ��� "Continue.." �� ����
				m_pGridButtons->SetCtrlValue((int32_t)ERowButtonSimple::Stop, (int32_t)EColButton::Button, L"Close");

				if(m_pStopButton->GetClickCount() == 1)
				{
					m_tsrCost.CopyDeviceToHost();
					break;
				}

				const float* pF32Cost = m_tsrCost.GetMemory();
				const float* pF32Evaluate = m_tsrEvaluation.GetMemory();

				// ���� ������ ���ڿ��� ����
				CFLString<wchar_t> strMessage;
				CFLString<wchar_t> strEpochMessage;

				strEpochMessage.Format
				(
					L"Fully-Connected XOR Network Training Finished!\n\n",
					m_i32Epoch, m_i32MaxEpoch
				);

				CFLString<wchar_t> strTrainMessage =
					CFLString<wchar_t>(L"<Cost>\n%f\n\n") +
					CFLString<wchar_t>(L"<Validation>\n") +
					CFLString<wchar_t>(L"[ %f %f %f %f ]\n\n");

				strMessage = strEpochMessage + strTrainMessage;
				strMessage.Format
				(
					strMessage, pF32Cost[0], pF32Evaluate[0], pF32Evaluate[1], pF32Evaluate[2], pF32Evaluate[3]
				);

				// m_pGridMain �� ERowMainSimple::Message, ERowMainSimple::Message ��ġ�� ���ڿ� �� �����Ͽ� ���̾�α� �󿡼� ���̴� �޼��� ����
				m_pGridMain->SetCtrlValue((int32_t)ERowMainSimple::Message, (int32_t)ERowMainSimple::Message, strMessage);
				// ���α׷��� �ٰ� 100% �� ä�������� ����
				m_pProgressCtrlFP->SetPos(1.);
				// ���̾�α� ȭ�� ����
				m_pDlgProgress->Invalidate();
				// �޽��� ����
				CGUIManager::PeekAndPump();
				Sleep(1);
				continue;
			}
			else
			{
				// ���� ��ư Ŭ�� Ƚ���� ����� ���
				if(m_pStopButton->GetClickCount() != m_i32BtnStopClickCount)
				{
					// ���� ��ư�� �� �� Ŭ���� ���
					if(m_pStopButton->GetClickCount() == 1)
					{
						// ���� ��ư �� ���ڿ��� "Close" �� ����
						m_pGridButtons->SetCtrlValue((int32_t)ERowButtonSimple::Stop, (int32_t)EColButton::Button, L"Close");
						// ���α׷��� �ٰ� 100% �� ä�������� ����
						m_pProgressCtrlFP->SetPos(1.);
						// ���̾�α� ȭ�� ����
						m_pDlgProgress->Invalidate();
					}
					// ���� ��ư�� �� �� Ŭ���� ��� ("Continue.." ��ư�� Ŭ���� ���)
					else if(m_pStopButton->GetClickCount() == 2)
					{
						// ���� ��ư �����
						m_pGridButtons->SetCtrlVisible((int32_t)ERowButtonSimple::Stop, (int32_t)EColButton::Button, false);
					}

					// ���� ��ư Ŭ�� Ƚ�� ������Ʈ
					m_i32BtnStopClickCount = m_pStopButton->GetClickCount();

					// ���̾�α��� ���̾ƿ� ������Ʈ
					m_pDlgProgress->AdjustLayout();
				}

				// ���� ��ư�� 1ȸ �̻� Ŭ�� �� break
				if(m_pStopButton->GetClickCount() >= 2)
					break;
				else if(m_i32Epoch > 0)
				{
					if(m_tsrCost.IsCudaActivated())
					{
						m_tsrCost.CopyDeviceToHost();
						m_tsrEvaluation.CopyDeviceToHost();
					}

					const float* pF32Cost = m_tsrCost.GetMemory();
					const float* pF32Evaluate = m_tsrEvaluation.GetMemory();

					// ���� ������ ���ڿ��� ����
					CFLString<wchar_t> strMessage;
					CFLString<wchar_t> strEpochMessage;

					if(m_pStopButton->GetClickCount() == 0)
					{
						strEpochMessage.Format
						(
							L"Fully-Connected XOR Network Training...(%d / %d)\n\n",
							m_i32Epoch, m_i32MaxEpoch
						);
					}
					else
					{
						strEpochMessage.Format
						(
							L"Fully-Connected XOR Network Terminated!\n\nFinal Epoch : %d\n\n",
							m_i32Epoch
						);
					}

					CFLString<wchar_t> strTrainMessage =
						CFLString<wchar_t>(L"<Cost>\n%f\n\n") +
						CFLString<wchar_t>(L"<Validation>\n") +
						CFLString<wchar_t>(L"[ %f %f %f %f ]\n\n");

					strMessage = strEpochMessage + strTrainMessage;
					strMessage.Format
					(
						strMessage, pF32Cost[0], pF32Evaluate[0], pF32Evaluate[1], pF32Evaluate[2], pF32Evaluate[3]
					);

					// m_pGridMain �� ERowMainSimple::Message, ERowMainSimple::Message ��ġ�� ���ڿ� �� �����Ͽ� ���̾�α� �󿡼� ���̴� �޼��� ����
					m_pGridMain->SetCtrlValue((int32_t)ERowMainSimple::Message, (int32_t)ERowMainSimple::Message, strMessage);

					// ���α׷��� �ٿ� ����� ǥ��
					m_pProgressCtrlFP->SetPos((double)m_i32Epoch / (double)m_i32MaxEpoch);
					// ���̾�α� â ũ�� ������Ʈ
					m_pDlgProgress->AdjustLayout();
				}
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

const CResult FLImaging::GUI::CPropertyMenuFullyConnectedXOR::CloseProgressDialog()
{
	CResult cr;

	do
	{
		if(m_pDlgProgress)
		{
			m_pDlgProgress->DestroyWindow();
			m_pDlgProgress = nullptr;
		}

		if(m_pGfCost)
		{
			CComputationalGraphUtilities<float>::Delete((CComputationalBase<float>*)m_pGfCost, false);

			m_pGfCost = nullptr;
			m_pGfEvaluation = nullptr;
		}

		cr = EResult_OK;
	}
	while(false);

	return cr;
}

void FLImaging::GUI::CPropertyMenuFullyConnectedXOR::AlgorithmThreadForSimpleDialog(CPropertyMenuFullyConnectedXOR* pInstance, std::future<void>** pContext)
{
	// �ܼ��� ���̾�α׿��� ���Ǵ� ������

	if(pInstance && pInstance->m_pDlgProgress)
	{
		// �н� ��� ���� ���� ��� ����� �޾� �׷��� �信 ���  
		// Get the history of cost and validation and print it at graph view
		CFLArray<float> flaCosts;
		CFLArray<float> flaValidationFF;
		CFLArray<float> flaValidationFT;
		CFLArray<float> flaValidationTF;
		CFLArray<float> flaValidationTT;

		while(true)
		{
			if(pInstance->m_i32Epoch >= pInstance->m_i32MaxEpoch)
			{
				pInstance->m_bThreadDone = true;
				break;
			}

			if(pInstance->m_pStopButton->GetClickCount() == 1)
			{
				pInstance->m_bThreadDone = false;
				break;
			}

			pInstance->m_pViewGraph->LockUpdate();
			// ���� �׷����� �����͸� ����
			pInstance->m_pViewGraph->Clear();

			pInstance->m_tsrCost = pInstance->m_pGfCost->Evaluate();
			pInstance->m_tsrEvaluation = pInstance->m_pGfEvaluation->Evaluate();

			//cuda ������ ��� ȣ��Ʈ �޸𸮷� ������ ����
			if(pInstance->m_tsrCost.IsCudaActivated())
			{
				pInstance->m_tsrCost.CopyDeviceToHost();
				pInstance->m_tsrEvaluation.CopyDeviceToHost();
			}

			if(pInstance->m_tsrCost.GetMemory() != nullptr)
			{

				flaCosts.PushBack(pInstance->m_tsrCost.GetMemory()[0]);
				flaValidationFF.PushBack(pInstance->m_tsrEvaluation.GetMemory()[0]);
				flaValidationFT.PushBack(pInstance->m_tsrEvaluation.GetMemory()[1]);
				flaValidationTF.PushBack(pInstance->m_tsrEvaluation.GetMemory()[2]);
				flaValidationTT.PushBack(pInstance->m_tsrEvaluation.GetMemory()[3]);
			}

			// XOR ����� �Ϻ��ϰ� ��ġ�ϸ� ����
			if(pInstance->m_tsrAnswer.Equals(pInstance->m_tsrEvaluation))
			{
				pInstance->m_bThreadDone = true;
				break;
			}

			pInstance->m_pViewGraph->Plot(flaCosts, EChartType_Line, RED, L"Cost");
			pInstance->m_pViewGraph->Plot(flaValidationFF, EChartType_Line, PINK, L"FF");
			pInstance->m_pViewGraph->Plot(flaValidationFT, EChartType_Line, ORANGE, L"FT");
			pInstance->m_pViewGraph->Plot(flaValidationTF, EChartType_Line, SKYBLUE, L"TF");
			pInstance->m_pViewGraph->Plot(flaValidationTT, EChartType_Line, GREEN, L"TT");

			pInstance->m_pViewGraph->UnlockUpdate();
			pInstance->m_pViewGraph->UpdateWindow();
			pInstance->m_pViewGraph->RedrawWindow();
			pInstance->m_pViewGraph->Invalidate();

			pInstance->m_pOptAdam->Fit();
			pInstance->m_i32Epoch++;

			Sleep(1);
		}
	}
}

void FLImaging::GUI::CPropertyMenuFullyConnectedXOR::ResetThreadParams()
{
	// ������ �Ķ���� ���� �ʱ�ȭ �մϴ�.
	m_bThreadDone = false;
	m_i32CurrentStopProgress = 0;
	m_i32CurrentProgress = 0;
	m_i32TotalProgress = 2000;
	m_i32TotalStopProgress = 20;
	m_i32BtnStopClickCount = 0;
	m_i32Epoch = 0;
}
