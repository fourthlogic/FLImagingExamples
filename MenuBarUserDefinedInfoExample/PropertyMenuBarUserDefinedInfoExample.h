#pragma once

namespace FLImaging
{
	namespace GUI
	{
		// ������Ƽ �޴� Ŭ����
		class CPropertyMenuBarUserDefinedInfoExample : public CGUIPropertyMenuBase
		{
			SupportToDuplicateObject(CPropertyMenuBarUserDefinedInfoExample);

		public:
			CPropertyMenuBarUserDefinedInfoExample();
			virtual ~CPropertyMenuBarUserDefinedInfoExample();

			// ������Ƽ �޴� ������ ����
			virtual const CResult ConfigureMenu() override;

			DeclareGetClassType();

		protected:
			// �ʱ�ȭ ���ν���
			CPropertyButtonClickProcedure* Initialize();
			// Run ��ư Ŭ�� ���ν���
			CPropertyButtonClickProcedure* Run();
			// Stop ��ư Ŭ�� ���ν���
			CPropertyButtonClickProcedure* Stop();
			// Add ��ư Ŭ�� ���ν���
			CPropertyItemButtonClickProcedure* Add();
			// Remove ��ư Ŭ�� ���ν���
			CPropertyItemButtonClickProcedure* Remove();
			// Set ��ư Ŭ�� ���ν���
			CPropertyItemButtonClickProcedure* Set();


			/* Utilities */
			// ������Ƽ �������� �߰��ϱ� ���� �Լ�
			const CResult AddItemForUserDefinedInfo(const Base::CFLString<wchar_t>& strPath);
			// ������Ƽ ���������κ��� ���� ��� ���� �Լ�
			const CResult GetParameters(const Base::CFLString<wchar_t>& strPath, int32_t& i32Index, Base::CFLString<wchar_t>& str, COLORREF& clrBackground, COLORREF& clrText, bool& bShow, EGUIAlignment& eAlign, EGUIStringTrimming& eStringTrimming);
			// "Set" ī�װ� �� �����۵鿡 ����, 
			// ������ �ε���(i32Index)�� UserDefinedInfo �� ������Ƽ ���� ������Ʈ �ϴ� �Լ�
			const CResult UpdateSetParameters(int32_t i32Index);
			// ���ڿ��� EGUIAlignment �� ��ȯ�ϴ� �Լ�
			EGUIAlignment GetAlignment(const Base::CFLString<wchar_t>& strVal) const;
			// EGUIAlignment �� ���ڿ��� ��ȯ�ϴ� �Լ�
			Base::CFLString<wchar_t> GetAlignmentText(EGUIAlignment eAlign) const;
			// ���ڿ��� EGUIStringTrimming �� ��ȯ�ϴ� �Լ�
			EGUIStringTrimming GetStringTrimming(const Base::CFLString<wchar_t>& strVal) const;
			// EGUIStringTrimming �� ���ڿ��� ��ȯ�ϴ� �Լ�
			Base::CFLString<wchar_t> GetStringTrimmingText(EGUIStringTrimming eStringTrimming) const;

		protected:
			bool m_bRunning;
		};
	}
}