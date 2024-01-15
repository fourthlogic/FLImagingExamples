#pragma once

namespace FLImaging
{
	namespace GUI
	{
		class CPropertyMenuFullyConnectedXOR : public CGUIPropertyMenuBase
		{
			SupportToDuplicateObject(CPropertyMenuFullyConnectedXOR);

		public:
			CPropertyMenuFullyConnectedXOR();
			virtual ~CPropertyMenuFullyConnectedXOR();

			virtual const CResult ConfigureMenu() override;

		protected:
			// ������ �۾� ������
			static void AlgorithmThreadForSimpleDialog(CPropertyMenuFullyConnectedXOR* pInstance, std::future<void>** pContext);

			// ������ ����� ǥ�� â ���̾�α� ���� �Լ�
			const CResult CreateSimpleDialog();

			// ������ ����� ǥ�� â ������Ʈ �Լ�
			const CResult UpdateSimpleDialog();

			// ����� ǥ�� â ���̾�α� �ݱ� �Լ�
			const CResult CloseProgressDialog();

			// ������ �Ķ���͸� �ʱ�ȭ �ϴ� �Լ�
			void ResetThreadParams();

			// ��ư Ŭ�� �̺�Ʈ �ڵ鷯
			CPropertyButtonClickProcedure* TrainXOR();

		protected:
			// ������ �Ϸ� �÷���
			volatile bool m_bThreadDone;

			// ���� ���෮
			volatile int32_t m_i32CurrentProgress;

			// ��ü ��
			volatile int32_t m_i32TotalProgress;

			// ���� ���� �� ���� ���෮
			volatile int32_t m_i32CurrentStopProgress;

			// ���� ���� �� ��ü ��
			volatile int32_t m_i32TotalStopProgress;

			// ������ ������
			std::future<void>* m_pThread;

			// ���̾�α� ��ü ������
			CGUIDialogProgress* m_pDlgProgress;

			// ���α׷��� �� ��ü ������
			CGUIProgressCtrlFloatingPoint* m_pProgressCtrlFP;

			// ���� ��ư ��ü ������
			CGUIButton* m_pStopButton;

			// �޼���, ���α׷��� ��, �ð� ���� ǥ�õǴ� �׸���
			CGUIGridLayout* m_pGridMain;

			// ��ư�� ��ġ�� �׸���
			CGUIGridLayout* m_pGridButtons;

			// �� â �׸���
			CGUIGridLayout* m_pGridDetails;

			// ���� ��ư Ŭ�� Ƚ��
			int32_t m_i32BtnStopClickCount;

			// �׷��� ��
			CGUIPaneViewGraph* m_pPane;
			CGUIViewGraph* m_pViewGraph;

			//
			CTensor<float>& m_tsrEvaluation;
			CTensor<float>& m_tsrCost;
			CTensor<float>& m_tsrAnswer;

			COptimizer<float>* m_pOpt;

			CComputationalGraph<float>* m_pGfCost;
			CComputationalGraph<float>* m_pGfEvaluation;

			int32_t m_i32Epoch = 0;
			int32_t m_i32PrevEpoch = 0;
			const int32_t m_i32MaxEpoch = 500;
		};
	}
}