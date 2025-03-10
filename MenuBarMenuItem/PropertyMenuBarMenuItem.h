#pragma once

namespace FLImaging
{
	namespace GUI
	{
		class CPropertyMenuBarMenuItem : public CGUIPropertyMenuBase
		{
			SupportToDuplicateObject(CPropertyMenuBarMenuItem);

		public:
			CPropertyMenuBarMenuItem();
			virtual ~CPropertyMenuBarMenuItem();

			virtual const CResult ConfigureMenu() override;

			// 버튼 클릭 프로시저
			CPropertyItemButtonClickProcedure* AddUserDefinedMenuItem();
			CPropertyItemButtonClickProcedure* RemoveUserDefinedMenuItem();

			DeclareGetClassType();
		};
	}
}