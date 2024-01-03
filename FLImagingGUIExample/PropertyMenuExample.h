#pragma once

namespace FLImaging
{
	namespace GUI
	{
		// ������Ƽ �޴� Ŭ����
		class CPropertyMenuExample : public CGUIPropertyMenuBase
		{
			SupportToDuplicateObject(CPropertyMenuExample);

		public:
			CPropertyMenuExample();
			virtual ~CPropertyMenuExample();

			// ������Ƽ �޴� ������ ����
			virtual const CResult ConfigureMenu() override;

		protected:
			// �̹��� �ε� ���ν���
			CPropertyButtonClickProcedure* LoadImage();
			// Ŭ���� �� ���ν���
			CPropertyButtonClickProcedure* ClearView();
			// Execute ���ν���
			CPropertyButtonClickProcedure* DrawFigure();
			// Ŭ���� ���̾� ���ν���
			CPropertyButtonClickProcedure* ClearLayer();
			// ������Ƽ Pane ���� �Լ�
			const CResult OnPropertyPaneChanged(GUI::CGUIPropertyItemBase* pBase, Base::CFLString<wchar_t>& strValue);
		};
	}
}

