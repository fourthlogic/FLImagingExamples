#pragma once

namespace FLImaging
{
	namespace GUI
	{
		// 프로퍼티 메뉴 클래스
		// Property menu class
		class CPropertyMenuBarUserDefinedInfoExample : public CGUIPropertyMenuBase
		{
			SupportToDuplicateObject(CPropertyMenuBarUserDefinedInfoExample);

		public:
			CPropertyMenuBarUserDefinedInfoExample();
			virtual ~CPropertyMenuBarUserDefinedInfoExample();

			// 프로퍼티 메뉴 아이템 구성
			// Configure property menu items
			virtual const CResult ConfigureMenu() override;

			DeclareGetClassType();

		protected:
			// 초기화 프로시저
			// Initialization procedure
			CPropertyButtonClickProcedure* Initialize();
			// Run 버튼 클릭 프로시저
			// Procedure for Run button click
			CPropertyButtonClickProcedure* Run();
			// Stop 버튼 클릭 프로시저
			// Procedure for Stop button click
			CPropertyButtonClickProcedure* Stop();
			// Add 버튼 클릭 프로시저
			// Procedure for Add button click
			CPropertyItemButtonClickProcedure* Add();
			// Remove 버튼 클릭 프로시저
			// Procedure for Remove button click
			CPropertyItemButtonClickProcedure* Remove();
			// Set 버튼 클릭 프로시저
			// Procedure for Set button click
			CPropertyItemButtonClickProcedure* Set();

			/* Utilities */
			// 프로퍼티 아이템을 추가하기 위한 함수
			// Function to add a property item
			const CResult AddItemForUserDefinedInfo(const Base::CFLString<wchar_t>& strPath);

			// 프로퍼티 아이템으로부터 값을 얻어 오는 함수
			// Function to retrieve values from a property item
			const CResult GetParameters(const Base::CFLString<wchar_t>& strPath, int32_t& i32Index, Base::CFLString<wchar_t>& str, COLORREF& clrBackground, COLORREF& clrText, bool& bShow, EGUIAlignment& eAlign, EGUIStringTrimming& eStringTrimming);

			// "Set" 카테고리 내 아이템들에 대해, 
			// 선택한 인덱스(i32Index)의 UserDefinedInfo 로 프로퍼티 값을 업데이트 하는 함수
			// Function to update property values for selected index (i32Index) in the "Set" category using UserDefinedInfo
			const CResult UpdateSetParameters(int32_t i32Index);

			// 문자열을 EGUIAlignment 로 변환하는 함수
			// Function to convert a string to EGUIAlignment
			EGUIAlignment GetAlignment(const Base::CFLString<wchar_t>& strVal) const;

			// EGUIAlignment 를 문자열로 변환하는 함수
			// Function to convert EGUIAlignment to a string
			Base::CFLString<wchar_t> GetAlignmentText(EGUIAlignment eAlign) const;

			// 문자열을 EGUIStringTrimming 로 변환하는 함수
			// Function to convert a string to EGUIStringTrimming
			EGUIStringTrimming GetStringTrimming(const Base::CFLString<wchar_t>& strVal) const;

			// EGUIStringTrimming 를 문자열로 변환하는 함수
			// Function to convert EGUIStringTrimming to a string
			Base::CFLString<wchar_t> GetStringTrimmingText(EGUIStringTrimming eStringTrimming) const;
		
			virtual void OnReceiveBroadcast(const Base::CBroadcastMessage* pMessage) override;
		protected:
			bool m_bRunning;
		};
	}
}