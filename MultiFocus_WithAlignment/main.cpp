#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

class CMessageReceiver : public CFLBase
{
public:

	// CMessageReceiver ������
	CMessageReceiver(CGUIViewImageWrap* pViewImage) : m_pViewImage(pViewImage)
	{
		// �޼����� ���� �ޱ� ���� CBroadcastManager �� ���� ��� //Subscribe to CBroadcast Manager to receive messages
		CBroadcastManager::Subscribe(this);

		m_pFliFirstPageAlignment = nullptr;
		m_pFliLastPageAlignment = nullptr;
	}

	// CMessageReceiver �Ҹ���
	virtual ~CMessageReceiver()
	{
		// ��ü�� �Ҹ��Ҷ� �޼��� ������ �ߴ��ϱ� ���� ������ �����Ѵ�. // Unsubscribe to stop receiving messages when the object disappears.
		CBroadcastManager::Unsubscribe(this);
	}

	DeclareGetClassType();

	// �޼����� ������ ȣ��Ǵ� �Լ� OnReceiveBroadcast �������̵� �Ͽ� ����
	void OnReceiveBroadcast(const CBroadcastMessage* pMessage) override
	{
		do
		{
			// pMessage �� nullptr ���� Ȯ�� // Verify message is nullptr
			if(pMessage == nullptr)
				break;

			// GetCaller() �� ����� �̹��������� Ȯ�� // Verify that GetCaller() is a registered image view
			if(pMessage->GetCaller() != (const CFLBase*)m_pViewImage->GetMessageCallerPtr())
				break;

			// �޼����� ä���� Ȯ��
			switch(pMessage->GetChannel())
			{
			case EGUIBroadcast_ViewImage_PostPageChange:
				{
					CFLImage* pFliSrcImage = (CFLImage*)m_pViewImage->GetImagePtr();
					int64_t i64CurPage = pFliSrcImage->GetSelectedPageIndex();

					if(i64CurPage == 0)
					{
					    // �̹������� 3�� ���̾� �������� // Get layer 3rd of image view
						CGUIViewImageLayerWrap wrapImageLayer = m_pViewImage->GetLayer((int32_t)(3));
						wrapImageLayer.DrawFigureImage(m_pFliFirstPageAlignment, LIME, 1);

						CFLQuad<double>* pFlqAlignment = dynamic_cast<CFLQuad<double>*>(m_pFliFirstPageAlignment);

						if(!pFlqAlignment)
							break;

						wrapImageLayer.DrawTextImage(Base::TPoint<double>(pFlqAlignment->flpPoints[0].x, pFlqAlignment->flpPoints[0].y), L"First Page Alignment", CYAN, 1);
					}
					else if(i64CurPage == pFliSrcImage->GetPageCount() - 1)
					{
						// �̹������� 4�� ���̾� �������� // Get layer 4th of image view
						CGUIViewImageLayerWrap wrapImageLayer = m_pViewImage->GetLayer((int32_t)(4));
						wrapImageLayer.DrawFigureImage(m_pFliLastPageAlignment, LIME, 1);

						CFLQuad<double>* pFlqAlignment = dynamic_cast<CFLQuad<double>*>(m_pFliLastPageAlignment);
						
						if (!pFlqAlignment)
							break;

						wrapImageLayer.DrawTextImage(Base::TPoint<double>(pFlqAlignment->flpPoints[0].x, pFlqAlignment->flpPoints[0].y), L"Last Page Alignment", CYAN, 1);
					}

					// �̹����並 ���� // Update the image view.
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
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSrcImage;
	CFLImage fliDstImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	// Message Reciever ��ü ���� // Create Message Reciever object
	CMessageReceiver msgReceiver(&viewImageSrc);

	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Source �̹��� �ε� // Load the source image
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
		
		// �޽��� ���ù��� Figure Pointer ���� // Set Figure Point to message receiver
		msgReceiver.m_pFliFirstPageAlignment = &flqFirstPageAlignment;
		msgReceiver.m_pFliLastPageAlignment = &flqLastPageAlignment;

		// Source �̹��� �� ���� // Create the source image view
		if((res = viewImageSrc.Create(400, 0, 800, 400)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((res = viewImageSrc.SetImagePtr(&fliSrcImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Source �̹��� �� ����� �� ���� ���� // Set thumbnail view height
		viewImageSrc.SetThumbnailViewHeight(0.05);

		// Destination �̹��� �� ���� // Create the destination image view
		if((res = viewImageDst.Create(800, 0, 1200, 400)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((res = viewImageDst.SetImagePtr(&fliDstImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		viewImageSrc.SetFixThumbnailView(true);

		// Multi Focus ��ü ���� // Create Multi Focus object
		CMultiFocus multiFocus;

		// Source �̹��� ���� // Set the source image
		multiFocus.SetSourceImage(fliSrcImage);
		// Destination �̹��� ���� // Set the destination image
		multiFocus.SetDestinationImage(fliDstImage);
		// ���� Ŀ�� ������ ���� // Set the kernel size
		multiFocus.SetKernelHalfSize(9);
		// ù��° ������ Alignment ���� // Set first page alignment
		multiFocus.SetFirstPageAlignment(flqFirstPageAlignment);
		// ������ ������ Alignment ���� // Set last page alignment
		multiFocus.SetLastPageAlignment(flqLastPageAlignment);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = multiFocus.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute algorithm.");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((res = viewImageDst.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit of the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerDst.Clear();

		layerSrc.SetAutoClearMode(ELayerAutoClearMode_PageChanged, false);

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
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

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}