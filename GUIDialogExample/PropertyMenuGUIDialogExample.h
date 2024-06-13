#pragma once

namespace FLImaging
{
	namespace GUI
	{
		class CPropertyMenuGUIDialogExample : public CGUIPropertyMenuBase
		{
			SupportToDuplicateObject(CPropertyMenuGUIDialogExample);

		public:
			CPropertyMenuGUIDialogExample();
			virtual ~CPropertyMenuGUIDialogExample();

			virtual const CResult ConfigureMenu() override;

			DeclareGetClassType();

		protected:
			CGUIDialog* m_pModelessDlg = nullptr;
			CGUIGridLayout* m_pGridLayout1 = nullptr;
			CGUIGridLayout* m_pGridLayout2 = nullptr;
			int32_t m_i32ThresholdIndex = 0;
			int32_t m_i32ChannelConditionIndex = 0;
			std::vector < std::tuple<Base::CFLString<wchar_t >>> m_vctLogicalCondition;

		protected:
			virtual void OnReceiveBroadcast(const Base::CBroadcastMessage* pMessage) override;

			const CResult OnSliderPosChanged(int32_t i32SliderPos);
			const CResult CreateBoxLayoutVertical();
			const CResult CreateBoxLayoutHorizontal();
			const CResult CreateGridLayoutLikeVerticalBox();
			const CResult CreateGridLayoutLikeHorizontalBox();
			const CResult CreateSizedBoxLayoutVertical();
			const CResult CreateSizedBoxLayoutHorizontal();
			const CResult CreateSizedGridLayoutLikeVerticalBox();
			const CResult CreateSizedGridLayoutLikeHorizontalBox();
			const CResult CreateComplexLayout1();
			const CResult CreateComplexLayout1Sized();
			const CResult CreateComplexLayout2();
			const CResult CreateComplexLayout3();
			const CResult CreateBoxLayoutStaticTextAlignment();
			const CResult CreateListCtrlDialog1();
			const CResult CreateListCtrlDialog2();
			const CResult CreateGridLayoutStaticTextAlignment();
			EGUIAlignment GetAlignment(const Base::CFLString<wchar_t>& strVal) const;

			CPropertyButtonClickProcedure* MakeButton1Implement();
			CPropertyButtonClickProcedure* MakeButton2Implement();
			CPropertyButtonClickProcedure* MakeButton3Implement();
			CPropertyButtonClickProcedure* MakeButton4Implement();
			CPropertyButtonClickProcedure* MakeButton5Implement();
			CPropertyButtonClickProcedure* MakeButton6Implement();
			CPropertyButtonClickProcedure* MakeButton7Implement();
			CPropertyButtonClickProcedure* MakeButton8Implement();
			CPropertyButtonClickProcedure* MakeButton9Implement();
			CPropertyButtonClickProcedure* MakeButton10Implement();
			CPropertyButtonClickProcedure* MakeButton11Implement();
			CPropertyButtonClickProcedure* MakeButton12Implement();
			CPropertyButtonClickProcedure* MakeButton13Implement();
			CPropertyButtonClickProcedure* MakeButton15Implement();
			CPropertyButtonClickProcedure* MakeButton16Implement();
			CPropertyButtonClickProcedure* MakeButton17Implement();
			CPropertyButtonClickProcedure* MakeButton18Implement();
			CPropertyButtonClickProcedure* MakeButton19Implement();
		};
	}
}