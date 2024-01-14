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
			// ��ư Ŭ�� �̺�Ʈ �ڵ鷯
			CPropertyButtonClickProcedure* TestButton();

		};
	}
}