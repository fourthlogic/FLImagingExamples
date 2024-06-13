#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

class CMessageReceiver : public CFLBase
{
public:

	// CMessageReceiver ������
	CMessageReceiver(CGUIViewImageWrap* pViewImageSrc, CGUIViewImageWrap* pViewImageDst) : m_pViewImageSrc(pViewImageSrc), m_pViewImageDst(pViewImageDst), m_bViewImageAvailable(true)
	{
		// �޼����� ���� �ޱ� ���� CBroadcastManager �� ���� ���
		CBroadcastManager::Subscribe(this);
	}

	// CMessageReceiver �Ҹ���
	virtual ~CMessageReceiver()
	{
		// ��ü�� �Ҹ��Ҷ� �޼��� ������ �ߴ��ϱ� ���� ������ �����Ѵ�.
		CBroadcastManager::Unsubscribe(this);
	}

	DeclareGetClassType();

	// �޼����� ������ ȣ��Ǵ� �Լ� OnReceiveBroadcast �������̵� �Ͽ� ����
	void OnReceiveBroadcast(const CBroadcastMessage* pMessage) override
	{
		do
		{
			// pMessage �� nullptr ���� Ȯ��
			if(pMessage == nullptr)
				break;

			// �޼����� ä���� Ȯ��
			switch(pMessage->GetChannel())
			{
			case EGUIBroadcast_ViewImage_PreDestroy:
				{
					if(!m_bViewImageAvailable)
						break;

					// GetCaller() �� ����� �̹��������� Ȯ��
					if(m_pViewImageSrc && pMessage->GetCaller() == (const CFLBase*)m_pViewImageSrc->GetMessageCallerPtr())
					{
						m_bViewImageAvailable = false;
						m_pViewImageSrc = nullptr;
					}

					if(m_pViewImageDst && pMessage->GetCaller() == (const CFLBase*)m_pViewImageDst->GetMessageCallerPtr())
					{
						m_bViewImageAvailable = false;
						m_pViewImageDst = nullptr;
					}
				}
				break;
			}
		}
		while(false);
	}

	bool IsViewImageAvailable() const
	{
		return m_bViewImageAvailable;
	}

protected:
	CGUIViewImageWrap* m_pViewImageSrc;
	CGUIViewImageWrap* m_pViewImageDst;
	bool m_bViewImageAvailable;
};

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSrcImage;
	CFLImage fliDstImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	// �޼����� ���� ���� CMessageReceiver ��ü ���� // Create �޼����� ���� ���� CMessageReceiver object
	CMessageReceiver msgReceiver(&viewImageSrc, &viewImageDst);

	// �˰��� ���� ��� // Algorithm execution result
	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = fliSrcImage.Load(L"../../ExampleImages/OpticalFlowPolynomialExpansion/Highway.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// OpticalFlowPolynomialExpansion ��ü ���� // Create OpticalFlowPolynomialExpansion object
		COpticalFlowPolynomialExpansion OpticalFlowPolynomialExpansion;

		// Source �̹��� ���� // Set the source image
		OpticalFlowPolynomialExpansion.SetSourceImage(fliSrcImage);
		// Destination �̹��� ���� // Set the destination image
		OpticalFlowPolynomialExpansion.SetDestinationImage(fliDstImage);
		// Pyramid Level ����
		OpticalFlowPolynomialExpansion.SetPyramidLevel(2);
		// Iteration ����
		OpticalFlowPolynomialExpansion.SetIteration(3);
		// Window Size ����
		OpticalFlowPolynomialExpansion.SetWindowSize(15);

		printf("Processing.....\n");

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = OpticalFlowPolynomialExpansion.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute algorithm.");
			break;
		}

		// Source �̹��� �� ���� // Create the source image view
		if((res = viewImageSrc.Create(400, 0, 1012, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the destination image view
		if((res = viewImageDst.Create(1012, 0, 1624, 512)).IsFail())
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

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((res = viewImageDst.SetImagePtr(&fliDstImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImageSrc.SynchronizePointOfView(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� �������� ����ȭ �Ѵ�
		if((res = viewImageSrc.SynchronizePageIndex(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
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

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = layerSrc.DrawTextCanvas(&CFLPoint<double>(0, 30), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = layerDst.DrawTextCanvas(&CFLPoint<double>(0, 30), L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);

		Foundation::CFLPoint<double> m_flpStart, m_flpEnd;
		Foundation::CFLLine<double> m_fllDisplay;

		// ����� Optical Flow Vector ũ�� �ּҰ� ����
		double f64MinVectorSize = 1;

		int32_t i32FlowWidth = (int32_t)fliSrcImage.GetWidth();
		int32_t i32FlowHeight = (int32_t)fliSrcImage.GetHeight();

		// Optical Flow Vector ���� ����
		int32_t i32GridStep = i32FlowWidth > i32FlowHeight ? i32FlowWidth / 50 : i32FlowHeight / 50;

		// �̹��� ������ �������� ���� Auto Clear Mode ��Ȱ��ȭ
		viewImageSrc.SetLayerAutoClearMode(0, ELayerAutoClearMode_PageChanged, false);
		viewImageDst.SetLayerAutoClearMode(0, ELayerAutoClearMode_PageChanged, false);
		viewImageSrc.SetLayerAutoClearMode(1, ELayerAutoClearMode_PageChanged, false);

		// ù��° �̹��� ������ ����
		viewImageSrc.MoveToPage(0);
		viewImageDst.MoveToPage(0);

		viewImageSrc.GetLayer(1).SetLayerDrawingMethod(ELayerDrawingMethod_Manual);

		// �̹��� �信 Optical Flow ���
		while(msgReceiver.IsViewImageAvailable())
		{
			if(fliSrcImage.GetPageCount() - 1 == fliSrcImage.GetSelectedPageIndex())
			{
				viewImageSrc.MoveToPage(0);
				viewImageDst.MoveToPage(0);
				continue;
			}

			viewImageSrc.GetLayer(1).Clear();

			for(int32_t i32Width = 0; i32Width < i32FlowWidth; i32Width += i32GridStep)
			{
				for(int32_t i32Height = 0; i32Height < i32FlowHeight; i32Height += i32GridStep)
				{
					// Destination �̹��� Pixel�� �����ϱ� ���� Pointer �ޱ�
					float** ppF32FlowPtr = (float**)fliDstImage.GetYOffsetTable();

					// Start -> End Point �����
					m_flpStart.x = i32Width;
					m_flpStart.y = i32Height;

					m_flpEnd.x = i32Width + ppF32FlowPtr[i32Height][i32Width * 2];
					m_flpEnd.y = i32Height + ppF32FlowPtr[i32Height][i32Width * 2 + 1];

					// Points to Line
					m_fllDisplay.flpPoints[0] = m_flpStart;
					m_fllDisplay.flpPoints[1] = m_flpEnd;

					if(!msgReceiver.IsViewImageAvailable())
						break;

					// �̹��� �信 ȭ��ǥ(Optical Flow Vector) �׸���
					if(m_fllDisplay.GetLength() > f64MinVectorSize)
					{
						viewImageSrc.GetLayer(1).DrawFigureImage(m_fllDisplay.MakeArrowWithRatio(0.4, true, 30), BLACK, 3);
						viewImageSrc.GetLayer(1).DrawFigureImage(m_fllDisplay.MakeArrowWithRatio(0.4, true, 30), YELLOW, 1);
					}
				}
			}

			if(!msgReceiver.IsViewImageAvailable())
				break;

			viewImageSrc.MoveToNextPage();
			viewImageDst.MoveToNextPage();
			viewImageSrc.GetLayer(1).Update();
			viewImageSrc.RedrawWindow();
		}
	}
	while(false);

	return 0;
}