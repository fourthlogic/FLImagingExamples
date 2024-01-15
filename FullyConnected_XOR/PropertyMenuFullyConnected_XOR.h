#pragma once

namespace FLImaging
{
	namespace GUI
	{
		class CPropertyMenuFullyConnectedXOR : public CGUIPropertyMenuBase
		{
			SupportToDuplicateObject(CPropertyMenuFullyConnectedXOR);

		public:
			CPropertyMenuFullyConnectedXOR();
			virtual ~CPropertyMenuFullyConnectedXOR();

			virtual const CResult ConfigureMenu() override;

		protected:
			// 가상의 작업 스레드
			static void AlgorithmThreadForSimpleDialog(CPropertyMenuFullyConnectedXOR* pInstance, std::future<void>** pContext);

			// 간단한 진행률 표시 창 다이얼로그 생성 함수
			const CResult CreateSimpleDialog();

			// 간단한 진행률 표시 창 업데이트 함수
			const CResult UpdateSimpleDialog();

			// 진행률 표시 창 다이얼로그 닫기 함수
			const CResult CloseProgressDialog();

			// 스레드 파라미터를 초기화 하는 함수
			void ResetThreadParams();

			// 버튼 클릭 이벤트 핸들러
			CPropertyButtonClickProcedure* TrainXOR();

		protected:
			// 스레드 완료 플래그
			volatile bool m_bThreadDone;

			// 현재 진행량
			volatile int32_t m_i32CurrentProgress;

			// 전체 양
			volatile int32_t m_i32TotalProgress;

			// 중지 진행 시 현재 진행량
			volatile int32_t m_i32CurrentStopProgress;

			// 중지 진행 시 전체 양
			volatile int32_t m_i32TotalStopProgress;

			// 스레드 포인터
			std::future<void>* m_pThread;

			// 다이얼로그 객체 포인터
			CGUIDialogProgress* m_pDlgProgress;

			// 프로그레스 바 객체 포인터
			CGUIProgressCtrlFloatingPoint* m_pProgressCtrlFP;

			// 중지 버튼 객체 포인터
			CGUIButton* m_pStopButton;

			// 메세지, 프로그레스 바, 시간 등이 표시되는 그리드
			CGUIGridLayout* m_pGridMain;

			// 버튼이 위치한 그리드
			CGUIGridLayout* m_pGridButtons;

			// 상세 창 그리드
			CGUIGridLayout* m_pGridDetails;

			// 중지 버튼 클릭 횟수
			int32_t m_i32BtnStopClickCount;

			// 그래프 뷰
			CGUIPaneViewGraph* m_pPane;
			CGUIViewGraph* m_pViewGraph;

			//
			CTensor<float>& m_tsrEvaluation;
			CTensor<float>& m_tsrCost;
			CTensor<float>& m_tsrAnswer;

			COptimizer<float>* m_pOpt;

			CComputationalGraph<float>* m_pGfCost;
			CComputationalGraph<float>* m_pGfEvaluation;

			int32_t m_i32Epoch = 0;
			int32_t m_i32PrevEpoch = 0;
			const int32_t m_i32MaxEpoch = 500;
		};
	}
}