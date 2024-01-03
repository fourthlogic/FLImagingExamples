#pragma once

namespace FLImaging
{
	namespace GUI
	{
		class CPropertyMenuDialogProgressExample : public CGUIPropertyMenuBase
		{
			SupportToDuplicateObject(CPropertyMenuDialogProgressExample);

		public:
			CPropertyMenuDialogProgressExample();
			virtual ~CPropertyMenuDialogProgressExample();

			virtual const CResult ConfigureMenu() override;

		protected:
			// ������ �۾� ������
			static void AlgorithmThreadForComplexDialog(CPropertyMenuDialogProgressExample* pInstance, std::future<void>** pContext);

			// ������ �۾� ������
			static void AlgorithmThreadForSimpleDialog(CPropertyMenuDialogProgressExample* pInstance, std::future<void>** pContext);

			// ������ ����� ǥ�� â ���̾�α� ���� �Լ�
			const CResult CreateSimpleDialog();

			// Resize Pivot �� ������ �ݿ��� ������ ����� ǥ�� â ���̾�α� ���� �Լ�
			const CResult CreateSimpleDialogWithSettings();

			// ������ ����� ǥ�� â ���̾�α� ���� �Լ�
			const CResult CreateComplexProgressDialog();

			// ������ ����� ǥ�� â ������Ʈ �Լ�
			const CResult UpdateSimpleDialog();

			// ������ ����� ǥ�� â ������Ʈ �Լ�
			const CResult UpdateSimpleDialogWithSettings();

			// ������ ����� ǥ�� â ������Ʈ �Լ�
			const CResult UpdateComplexDialog();

			// ����� ǥ�� â ���̾�α� �ݱ� �Լ�
			const CResult CloseProgressDialog();

			// ���� �ɼ��� ��� ���� �Լ�
			EGUIAlignment GetAlignment(const Base::CFLString<wchar_t>& strVal) const;

			// ������ �Ķ���͸� �ʱ�ȭ �ϴ� �Լ�
			void ResetThreadParams();

			// ��ư Ŭ�� �̺�Ʈ �ڵ鷯
			CPropertyButtonClickProcedure* ButtonCreateSimpleDialog();
			CPropertyButtonClickProcedure* ButtonCreateComplexDialog();
			CPropertyButtonClickProcedure* ButtonCreateSimpleDialogWithSettings();

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

			// ���� ������ ���ڿ� ����
			std::vector<Base::CFLString<wchar_t>> m_vctVariousLengthMessages;

			// �޼���, ���α׷��� ��, �ð� ���� ǥ�õǴ� �׸���
			CGUIGridLayout* m_pGridMain;

			// ��ư�� ��ġ�� �׸���
			CGUIGridLayout* m_pGridButtons;

			// �� â �׸���
			CGUIGridLayout* m_pGridDetails;

			// ���� ��ư Ŭ�� Ƚ��
			int32_t m_i32BtnStopClickCount;
		};
	}
}