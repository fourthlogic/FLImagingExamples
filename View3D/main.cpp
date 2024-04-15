#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


enum EType
{
	EType_Model = 0,
	EType_Texture,
	ETypeCount,
};

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];
	
	// 3D �� ����
	CGUIView3DWrap view3D;

	CResult res = EResult_UnknownError;

	do
	{
		// Model �̹��� �ε�
		if(IsFail(res = arrFliImage[EType_Model].Load(L"../../ExampleImages/View3D/mountain.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Texture �̹��� �ε�
		if(IsFail(res = arrFliImage[EType_Texture].Load(L"../../ExampleImages/View3D/mountain_texture.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Model �̹��� �� ����
		if(IsFail(res = arrViewImage[EType_Model].Create(100, 0, 612, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Texture �̹��� �� ����
		if(IsFail(res = arrViewImage[EType_Texture].Create(612, 0, 1124, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 3D �� ����
		if(IsFail(res = view3D.Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(res, "Failed to create the 3D view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if(IsFail(res = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// 3D �信 �𵨰� �ؽ�ó�� �ε��Ͽ� ���÷���
		if((res = view3D.PushObject(CFL3DObjectHeightMap(&arrFliImage[EType_Model], &arrFliImage[EType_Texture]))).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the 3D view.\n");
			break;
		}

		view3D.ZoomFit();

		// �� �̹��� ���� ������ ����ȭ �մϴ� // Synchronize the viewpoints of the two image views
		if(IsFail(res = arrViewImage[EType_Model].SynchronizePointOfView(&arrViewImage[EType_Texture])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(res = arrViewImage[EType_Model].SynchronizeWindow(&arrViewImage[EType_Texture])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �̹��� ��� 3D �� �������� ��ġ�� ����
		if(IsFail(res = arrViewImage[EType_Model].SynchronizeWindow(&view3D)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
			arrLayer[i].Clear();
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = arrLayer[EType_Model].DrawTextCanvas(&CFLPointD(0, 0), L"Model Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_Texture].DrawTextCanvas(&CFLPointD(0, 0), L"Texture Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3D.GetLayer(2).DrawTextCanvas(&CFLPointD(0, 0), L"3D View", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		arrViewImage[EType_Model].Invalidate(true);
		arrViewImage[EType_Texture].Invalidate(true);
		view3D.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(arrViewImage[EType_Model].IsAvailable()
			  && arrViewImage[EType_Texture].IsAvailable()
			  && view3D.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}