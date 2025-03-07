#pragma once

namespace FLImaging
{
	namespace GUI
	{
		// 프로퍼티 메뉴 클래스
		class CPropertyMenuExample : public CGUIPropertyMenuBase
		{
			SupportToDuplicateObject(CPropertyMenuExample);

		public:
			CPropertyMenuExample();
			virtual ~CPropertyMenuExample();

			// 프로퍼티 메뉴 아이템 구성
			virtual const CResult ConfigureMenu() override;

		protected:
			// 이미지 로드 프로시저
			CPropertyButtonClickProcedure* LoadImage();
			// 클리어 뷰 프로시저
			CPropertyButtonClickProcedure* ClearView();
			// Execute 프로시저
			CPropertyButtonClickProcedure* DrawFigure();
			// 클리어 레이어 프로시저
			CPropertyButtonClickProcedure* ClearLayer();
			// 프로퍼티 Pane 변경 함수
			const CResult OnPropertyPaneChanged(GUI::CGUIPropertyItemBase* pBase, Base::CFLString<wchar_t>& strValue);
		};
	}
}

