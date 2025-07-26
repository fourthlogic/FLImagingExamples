#pragma once

namespace FLImaging
{
	namespace GUI
	{
		class CPropertyTimerExample : public CGUIPropertyMenuBase
		{
			SupportToDuplicateObject(CPropertyTimerExample);

		public:
			CPropertyTimerExample();
			virtual ~CPropertyTimerExample();

			virtual const CResult ConfigureMenu() override;

			virtual void OnReceiveBroadcast(const Base::CBroadcastMessage* pMessage) override;

			CPoint m_ptPos;
			uint32_t m_u32TimerID;

			DeclareGetClassType();
		};
	}
}