#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


class CMessageReceiver : public CFLBase
{
public:

	// CMessageReceiver ������
	CMessageReceiver(CGUIViewImageWrap* pViewImage) : m_pViewImage(pViewImage)
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

			// GetCaller() �� ����� �̹��������� Ȯ��
 			if(pMessage->GetCaller() != (const CFLBase*)m_pViewImage->GetMessageCallerPtr())
 				break;

			// �޼����� ä���� Ȯ��
			switch(pMessage->GetChannel())
			{
			case EGUIBroadcast_ViewImage_PostMouseMove:
				{
					// pMessage ��ü�� CBroadcastMessage_GUI_ViewImage_MouseEvent �� ĳ����
					CBroadcastMessage_GUI_ViewImage_MouseEvent* pMsgMouseEvent = dynamic_cast<CBroadcastMessage_GUI_ViewImage_MouseEvent*>((CBroadcastMessage*)pMessage);

					// pMsgMouseEvent �� nullptr ���� Ȯ��
					if(pMsgMouseEvent == nullptr)
						break;

					// �̹������� 0�� ���̾� ��������
					CGUIViewImageLayerWrap layer = m_pViewImage->GetLayer(0);

					// ������ Layer �� �׷��� �������� ����
					layer.Clear();

					// ���� ���콺�� ��ġ�� ������ ǥ���� ���ڿ� ����
					CFLString<wchar_t> strHitArea = L"";
					CFLString<wchar_t> str = L"";
					
					// ���� ���콺�� ��ġ�� ������ ��� �ɴϴ�.
					EGUIViewImageHitArea eHitArea = m_pViewImage->GetHitArea();

					if(eHitArea == EGUIViewImageHitArea_None)
						strHitArea = "None";
					else
					{
						strHitArea = "Mouse is ";
						str = "on ";

						if(eHitArea & (EGUIViewImageHitArea_MiniMap))
						{
							strHitArea += str + "MiniMap";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_MiniMapDisplayingArea))
						{
							strHitArea += str + "MiniMapDisplayingArea";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_ThumbnailView))
						{
							strHitArea += str + "ThumbnailView";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_ThumbnailViewTop))
						{
							strHitArea += str + "ThumbnailViewTop";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_Figure))
						{
							strHitArea += str + "Figure";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_MultiFigures))
						{
							strHitArea += str + "MultiFigures";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_ImageFigure))
						{
							strHitArea += str + "ImageROI";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_StatusBar))
						{
							strHitArea += str + "StatusBar";
							str = "and ";
						}

						if(eHitArea & (EGUIViewImageHitArea_PageIndex))
						{
							strHitArea += str + "PageIndex";
							str = "and ";
						}
						if(eHitArea & (EGUIViewImageHitArea_PrevPageArrow))
						{
							strHitArea += str + "PrevPageArrow";
							str = "and ";
						}
						if(eHitArea & (EGUIViewImageHitArea_NextPageArrow))
						{
							strHitArea += str + "NextPageArrow";
							str = "and ";
						}
						if(eHitArea & (EGUIViewImageHitArea_Measurement))
						{
							strHitArea += str + "Measurement";
							str = "and ";
						}
						if(eHitArea & (EGUIViewImageHitArea_MultiMeasurements))
						{
							strHitArea += str + "MultiMeasurements";
							str = "and ";
						}
						strHitArea += ".";
					}

					// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
					// ���� �Ķ���͸� EColor.TRANSPARENCY ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�.
					// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
					//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
					// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
					//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
					layer.DrawTextCanvas(CFLPoint<double>(80, 10), strHitArea, LIME, BLACK);

					// �̹����並 ����
					m_pViewImage->Invalidate();
				}
				break;
			}
		}
		while(false);
	}

	protected:
		CGUIViewImageWrap* m_pViewImage;
};

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	CMessageReceiver msgReceiver(&viewImage);

	CResult res = EResult_UnknownError;

	do
	{
		// �̹��� �ε� // Load image
		if(IsFail(res =fliImage.Load(L"../../ExampleImages/PagePooling/Multiple File_Min.flif")))
		{
			ErrorPrint(res,"Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage.Create(300, 0, 300 + 520, 430)))
		{
			ErrorPrint(res,"Failed to create the image view.\n");
			break;
		}
		
		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(res =viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res,"Failed to set image object on the image view.\n");
			break;
		}

		// Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�.
		if(IsFail(res =viewImage.ZoomFit()))
		{
			ErrorPrint(res,"Failed to zoom fit of the image view.\n");
			break;
		}

		// �̹��� ���� ĵ���� ������ ���´�.
		CFLRect<int32_t> flrlCanvas = viewImage.GetClientRectCanvasRegion();

		// ĵ���� ������ ��ǥ�踦 �̹��� ������ ��ǥ��� ��ȯ�Ѵ�.
		CFLRect<double> flrdImage = viewImage.ConvertCanvasCoordToImageCoord(flrlCanvas);

		// �̹��� ������ �������� ������ Figure �� ũ��� ����� �簢������ �����Ѵ�.
		double f64Width = flrdImage.GetWidth() / 10.;
		double f64Height = flrdImage.GetHeight() / 10.;
		double f64Size = __min(f64Width, f64Height);

		CFLPoint<double> flpdCenter(0, 0);
		flrdImage.GetCenter(flpdCenter);

		CFLRect<double> flrdFigureShape(flpdCenter.x - f64Size, flpdCenter.y - f64Size, flpdCenter.x + f64Size, flpdCenter.y + f64Size);

		// �̹��� �信 Figure object �� �����Ѵ�.
		// ���� ������ �Ķ���ʹ� Ȱ��ȭ �Ǵ� �޴��� �����̸�, EAvailableFigureContextMenu_All �� �⺻ �޴��� Ȱ��ȭ �Ѵ�.
		// Ȱ��ȭ �ϰ��� �ϴ� �޴��� �߰� Ȥ�� ���� �ϱ� ���ؼ��� enum ���� ��Ʈ �������� �־��ָ� �ȴ�.
		// ex) EAvailableFigureContextMenu_None -> Ȱ��ȭ �Ǵ� �޴� ����
		//     EAvailableFigureContextMenu_All -> ��ü �޴� Ȱ��ȭ
		//     EAvailableFigureContextMenu_DeclType | EAvailableFigureContextMenu_TemplateType -> Decl Type, Template Type ��ȯ �޴� Ȱ��ȭ
		viewImage.PushBackFigureObject(&flrdFigureShape, EAvailableFigureContextMenu_All);

		// �̹��� �並 ���� // Update image view
		viewImage.Invalidate(true);

		// ���� ������ �̹����� ����� �̸����� �並 ����
		viewImage.SetFixThumbnailView(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}