#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage[2];

	CResult res = EResult_UnknownError;

	do
	{
		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage[0].Create(300, 0, 300 + 520, 430)))
		{
			ErrorPrint(res,"Failed to create the image view.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage[1].Create(300 + 520, 0, 300 + 520 * 2, 430)))
		{
			ErrorPrint(res,"Failed to create the image view.\n");
			break;
		}

		// ���� ������ ����ȭ �Ѵ�.
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res,"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res,"Failed to synchronize window\n");
			break;
		}

		// �̹��� �信�� �̿� ������ ���ؽ�Ʈ �޴��� �����մϴ�. 
		// EAvailableViewImageContextMenu_All �� �⺻���̸�, �� ������ �����ϸ� ��� �޴��� ��� ������ ���°� �˴ϴ�.
		// �Ʒ��� ���� EAvailableViewImageContextMenu_None ���� ������ ��� ��� �޴��� ��Ȱ��ȭ�˴ϴ�.
		viewImage[0].SetAvailableViewImageContextMenu(EAvailableViewImageContextMenu_None);

		// �̹������� 0�� ���̾� ��������
		CGUIViewImageLayerWrap layer = viewImage[0].GetLayer(0);

		// ������ Layer �� �׷��� �������� ����
		layer.Clear();

		// �ȳ� ���ڿ� ����
		CFLString<wchar_t> flsInformation = L"RIGHT BUTTON CLICK ON MOUSE AND SEE THE CONTEXT MENU";
		CFLString<wchar_t> flsInformation2 = L"Option : EAvailableViewImageContextMenu_None";

		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EColor.TRANSPARENCY ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		layer.DrawTextCanvas(CFLPoint<double>(10, 10), flsInformation, LIME, BLACK, 15);
		layer.DrawTextCanvas(CFLPoint<double>(10, 30), flsInformation2, CYAN, BLACK, 15);


		// �̹��� �信�� �̿� ������ ���ؽ�Ʈ �޴��� �����մϴ�. 
		// EAvailableViewImageContextMenu_All �� �⺻���̸�, �� ������ �����ϸ� ��� �޴��� ��� ������ ���°� �˴ϴ�.
		// �Ʒ��� ���� ���� ������ �̿��Ͽ� ������ �� ������, 
		// EAvailableViewImageContextMenu_All & ~(EAvailableViewImageContextMenu_Load | EAvailableViewImageContextMenu_ClearFile | EAvailableViewImageContextMenu_Save | EAvailableViewImageContextMenu_CreateImage) ���� ������ ��� 
		// ���� ����, �ݱ�, ����, �̹��� ���� �޴��� ��Ȱ��ȭ�˴ϴ�.
		viewImage[1].SetAvailableViewImageContextMenu(EAvailableViewImageContextMenu_All & ~(EAvailableViewImageContextMenu_Load | EAvailableViewImageContextMenu_ClearFile | EAvailableViewImageContextMenu_Save | EAvailableViewImageContextMenu_CreateImage));


		// �̹������� 0�� ���̾� ��������
		layer = viewImage[1].GetLayer(0);

		// ������ Layer �� �׷��� �������� ����
		layer.Clear();

		// �ȳ� ���ڿ� ����
		flsInformation = L"RIGHT BUTTON CLICK ON MOUSE AND SEE THE CONTEXT MENU";
		flsInformation2 = L"Option : EAvailableViewImageContextMenu_All & \n           ~(EAvailableViewImageContextMenu_Load | \n              EAvailableViewImageContextMenu_ClearFile | \n              EAvailableViewImageContextMenu_Save | \n              EAvailableViewImageContextMenu_CreateImage)";

		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EColor.TRANSPARENCY ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		layer.DrawTextCanvas(CFLPoint<double>(10, 10), flsInformation, LIME, BLACK, 15);
		layer.DrawTextCanvas(CFLPoint<double>(10, 30), flsInformation2, CYAN, BLACK, 15);

		for(int32_t i = 0; i < 2; ++i)
			viewImage[i].Invalidate();

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}