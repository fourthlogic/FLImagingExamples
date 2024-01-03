#pragma once

namespace FLImaging
{
	namespace GUI
	{
		class CPropertyLogInExample : public CGUIPropertyMenuBase
		{
			SupportToDuplicateObject(CPropertyLogInExample);

		public:
			CPropertyLogInExample();
			virtual ~CPropertyLogInExample();

			virtual const CResult ConfigureMenu() override;

		protected:
			// 버튼 클릭 이벤트 핸들러
			CPropertyButtonClickProcedure* TestButton();

		};
	}
}