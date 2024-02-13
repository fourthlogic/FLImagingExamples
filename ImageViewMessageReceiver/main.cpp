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

			// �޼����� ä���� Ȯ��
			switch(pMessage->GetChannel())
			{
			case EGUIBroadcast_ViewImage_PostMouseMove:
				{
					// GetCaller() �� ����� �̹��������� Ȯ��
					if(pMessage->GetCaller() != (const CFLBase*)m_pViewImage->GetMessageCallerPtr())
						break;

					// pMessage ��ü�� CBroadcastMessage_GUI_ViewImage_MouseEvent �� ĳ����
					CBroadcastMessage_GUI_ViewImage_MouseEvent* pMsgMouseEvent = dynamic_cast<CBroadcastMessage_GUI_ViewImage_MouseEvent*>((CBroadcastMessage*)pMessage);

					// pMsgMouseEvent �� nullptr ���� Ȯ��
					if(pMsgMouseEvent == nullptr)
						break;

					// �̹������� 0�� ���̾� ��������
					CGUIViewImageLayerWrap layer = m_pViewImage->GetLayer(0);

					// ������ Layer �� �׷��� �������� ����
					layer.Clear();

					// ���콺 ��ǥ�� ǥ���� ���ڿ� ����
					CFLString<wchar_t> flsPosition = L"";
					flsPosition.Format(L"Move X: %d, Y: %d", pMsgMouseEvent->GetCursor()->x, pMsgMouseEvent->GetCursor()->y);

					// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
					// ���� �Ķ���͸� EColor.TRANSPARENCY ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�.
					// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
					//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
					// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
					//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
					layer.DrawTextCanvas(CFLPoint<double>(10, 10), flsPosition, LIME, BLACK);

					// �̹����並 ����
					m_pViewImage->Invalidate();
				}
				break;

			case EGUIBroadcast_ViewImage_PostLButtonDown:
				{
					// GetCaller() �� ����� �̹��������� Ȯ��
					if(pMessage->GetCaller() != (const CFLBase*)m_pViewImage->GetMessageCallerPtr())
						break;

					// pMessage ��ü�� CBroadcastMessage_GUI_ViewImage_MouseEvent �� ĳ����
					CBroadcastMessage_GUI_ViewImage_MouseEvent* pMsgMouseEvent = dynamic_cast<CBroadcastMessage_GUI_ViewImage_MouseEvent*>((CBroadcastMessage*)pMessage);

					// pMsgMouseEvent �� nullptr ���� Ȯ��
					if(pMsgMouseEvent == nullptr)
						break;

					// �̹������� 0�� ���̾� ��������
					CGUIViewImageLayerWrap layer = m_pViewImage->GetLayer(0);

					// ������ Layer �� �׷��� �������� ����
					layer.Clear();

					// ���콺 ��ǥ�� ǥ���� ���ڿ� ����
					CFLString<wchar_t> flsPosition = L"";
					flsPosition.Format(L"LButtonDown X: %d, Y: %d", pMsgMouseEvent->GetCursor()->x, pMsgMouseEvent->GetCursor()->y);

					// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
					// ���� �Ķ���͸� EColor.TRANSPARENCY ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�.
					// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
					//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
					// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
					//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
					layer.DrawTextCanvas(CFLPoint<double>(10, 10), flsPosition, RED, BLACK);

					// �̹����並 ����
					m_pViewImage->Invalidate();
				}
				break;

			case EGUIBroadcast_ViewImage_PostLButtonUp:
				{
					// GetCaller() �� ����� �̹��������� Ȯ��
					if(pMessage->GetCaller() != (const CFLBase*)m_pViewImage->GetMessageCallerPtr())
						break;

					// pMessage ��ü�� CBroadcastMessage_GUI_ViewImage_MouseEvent �� ĳ����
					CBroadcastMessage_GUI_ViewImage_MouseEvent* pMsgMouseEvent = dynamic_cast<CBroadcastMessage_GUI_ViewImage_MouseEvent*>((CBroadcastMessage*)pMessage);

					// pMsgMouseEvent �� nullptr ���� Ȯ��
					if(pMsgMouseEvent == nullptr)
						break;

					// �̹������� 0�� ���̾� ��������
					CGUIViewImageLayerWrap layer = m_pViewImage->GetLayer(0);

					// ������ Layer �� �׷��� �������� ����
					layer.Clear();

					// ���콺 ��ǥ�� ǥ���� ���ڿ� ����
					CFLString<wchar_t> flsPosition = L"";
					flsPosition.Format(L"LButtonUp X: %d, Y: %d", pMsgMouseEvent->GetCursor()->x, pMsgMouseEvent->GetCursor()->y);

					// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
					// ���� �Ķ���͸� EColor.TRANSPARENCY ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�.
					// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
					//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
					// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
					//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
					layer.DrawTextCanvas(CFLPoint<double>(10, 10), flsPosition, BLUE, BLACK);

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
	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage[2];

	// �޼����� ���� ���� CMessageReceiver ��ü ���� // Create �޼����� ���� ���� CMessageReceiver object
	CMessageReceiver msgReceiver(&viewImage[0]);

	CResult eResult = EResult_UnknownError;

	do
	{
		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImage[0].Create(300, 0, 300 + 520, 430)))
		{
			ErrorPrint(eResult,"Failed to create the image view.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImage[1].Create(300 + 520, 0, 300 + 520 * 2, 430)))
		{
			ErrorPrint(eResult,"Failed to create the image view.\n");
			break;
		}

		// ���� ������ ����ȭ �Ѵ�
		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(eResult,"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(eResult,"Failed to synchronize window\n");
			break;
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}