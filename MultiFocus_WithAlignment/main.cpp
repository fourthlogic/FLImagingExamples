#include <cstdio>
#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h"

class CMessageReceiver : public CFLBase
{
public:

	// CMessageReceiver 생성자
	CMessageReceiver(CGUIViewImageWrap* pViewImage) : m_pViewImage(pViewImage)
	{
		// 메세지를 전달 받기 위해 CBroadcastManager 에 구독 등록 //Subscribe to CBroadcast Manager to receive messages
		CBroadcastManager::Subscribe(this);

		m_pFliFirstPageAlignment = nullptr;
		m_pFliLastPageAlignment = nullptr;
	}

	// CMessageReceiver 소멸자
	virtual ~CMessageReceiver()
	{
		// 객체가 소멸할때 메세지 수신을 중단하기 위해 구독을 해제한다. // Unsubscribe to stop receiving messages when the object disappears.
		CBroadcastManager::Unsubscribe(this);
	}

	DeclareGetClassType();

	// 메세지가 들어오면 호출되는 함수 OnReceiveBroadcast 오버라이드 하여 구현
	void OnReceiveBroadcast(const CBroadcastMessage* pMessage) override
	{
		do
		{
			// pMessage 가 nullptr 인지 확인 // Verify message is nullptr
			if(pMessage == nullptr)
				break;

			// GetCaller() 가 등록한 이미지뷰인지 확인 // Verify that GetCaller() is a registered image view
			if(pMessage->GetCaller() != (const CFLBase*)m_pViewImage->GetMessageCallerPtr())
				break;

			// 메세지의 채널을 확인
			switch(pMessage->GetChannel())
			{
			case EGUIBroadcast_ViewImage_PostPageChange:
				{
					CFLImage* pFliSrcImage = (CFLImage*)m_pViewImage->GetImagePtr();
					int64_t i64CurPage = pFliSrcImage->GetSelectedPageIndex();

					if(i64CurPage == 0)
					{
					    // 이미지뷰의 3번 레이어 가져오기 // Get layer 3rd of image view
						CGUIViewImageLayerWrap wrapImageLayer = m_pViewImage->GetLayer((int32_t)(3));
						wrapImageLayer.DrawFigureImage(m_pFliFirstPageAlignment, LIME, 1);

						CFLQuad<double>* pFlqAlignment = dynamic_cast<CFLQuad<double>*>(m_pFliFirstPageAlignment);

						if(!pFlqAlignment)
							break;

						wrapImageLayer.DrawTextImage(Base::TPoint<double>(pFlqAlignment->flpPoints[0].x, pFlqAlignment->flpPoints[0].y), L"First Page Alignment", CYAN, 1);
					}
					else if(i64CurPage == pFliSrcImage->GetPageCount() - 1)
					{
						// 이미지뷰의 4번 레이어 가져오기 // Get layer 4th of image view
						CGUIViewImageLayerWrap wrapImageLayer = m_pViewImage->GetLayer((int32_t)(4));
						wrapImageLayer.DrawFigureImage(m_pFliLastPageAlignment, LIME, 1);

						CFLQuad<double>* pFlqAlignment = dynamic_cast<CFLQuad<double>*>(m_pFliLastPageAlignment);
						
						if (!pFlqAlignment)
							break;

						wrapImageLayer.DrawTextImage(Base::TPoint<double>(pFlqAlignment->flpPoints[0].x, pFlqAlignment->flpPoints[0].y), L"Last Page Alignment", CYAN, 1);
					}

					// 이미지뷰를 갱신 // Update the image view.
					m_pViewImage->Invalidate();
				}
				break;
			}
		}
		while(false);
	}

	CFLFigure* m_pFliFirstPageAlignment;
	CFLFigure* m_pFliLastPageAlignment;

protected:
	CGUIViewImageWrap* m_pViewImage;
};

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliSrcImage;
	CFLImage fliDstImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	// Message Reciever 객체 생성 // Create Message Reciever object
	CMessageReceiver msgReceiver(&viewImageSrc);

	do
	{
		// 알고리즘 동작 결과 // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Source 이미지 로드 // Load the source image
		if((res = fliSrcImage.Load(L"../../ExampleImages/MultiFocus/SourceAlignment.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliSrcImage.SelectPage(0);

		CFLQuad<double> flqFirstPageAlignment;
		CFLQuad<double> flqLastPageAlignment;

		if((res = flqFirstPageAlignment.Load(L"../../ExampleImages/MultiFocus/FirstPageAlignment.fig")).IsFail())
		{
			ErrorPrint(res, L"Failed to load Source Projection Figure.");
			break;
		}

		if((res = flqLastPageAlignment.Load(L"../../ExampleImages/MultiFocus/LastPageAlignment.fig")).IsFail())
		{
			ErrorPrint(res, L"Failed to load Destination Projection Figure.");
			break;
		}
		
		// 메시지 리시버에 Figure Pointer 설정 // Set Figure Point to message receiver
		msgReceiver.m_pFliFirstPageAlignment = &flqFirstPageAlignment;
		msgReceiver.m_pFliLastPageAlignment = &flqLastPageAlignment;

		// Source 이미지 뷰 생성 // Create the source image view
		if((res = viewImageSrc.Create(400, 0, 800, 400)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰에 이미지를 디스플레이 // Display the image in the source image view
		if((res = viewImageSrc.SetImagePtr(&fliSrcImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Source 이미지 뷰 썸네일 뷰 높이 설정 // Set thumbnail view height
		viewImageSrc.SetThumbnailViewHeight(0.05);

		// Destination 이미지 뷰 생성 // Create the destination image view
		if((res = viewImageDst.Create(800, 0, 1200, 400)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination 이미지 뷰에 이미지를 디스플레이 // Display the image in the destination image view
		if((res = viewImageDst.SetImagePtr(&fliDstImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		viewImageSrc.SetFixThumbnailView(true);

		// Multi Focus 객체 생성 // Create Multi Focus object
		CMultiFocus multiFocus;

		// Source 이미지 설정 // Set the source image
		multiFocus.SetSourceImage(fliSrcImage);
		// Destination 이미지 설정 // Set the destination image
		multiFocus.SetDestinationImage(fliDstImage);
		// 필터 커널 사이즈 설정 // Set the kernel size
		multiFocus.SetKernel(23);
		// 첫번째 페이지 Alignment 설정 // Set first page alignment
		multiFocus.SetFirstPageAlignment(flqFirstPageAlignment);
		// 마지막 페이지 Alignment 설정 // Set last page alignment
		multiFocus.SetLastPageAlignment(flqLastPageAlignment);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((res = multiFocus.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute algorithm.");
			break;
		}

		// Destination 이미지가 새로 생성됨으로 Zoom fit 을 통해 디스플레이 되는 이미지 배율을 화면에 맞춰준다. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((res = viewImageDst.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit of the image view.\n");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerDst.Clear();

		layerSrc.SetAutoClearMode(ELayerAutoClearMode_PageChanged, false);

		// View 정보를 디스플레이 한다. // Display view information
		// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = layerSrc.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = layerDst.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}