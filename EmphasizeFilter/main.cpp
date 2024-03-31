#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

enum EType
{
	EType_Source = 0,
	EType_Destination1,
	EType_Destination2,
	ETypeCount,
};

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res =arrFliImage[EType_Source].Load(L"../../ExampleImages/EmphasizeFilter/houses.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create the Destination1 image as the Source image.
		if((res =arrFliImage[EType_Destination1].Assign(arrFliImage[EType_Source])).IsFail())
		{
			ErrorPrint(res, L"Failed to assign the image file.\n");
			break;
		}

		// Destination2 �̹����� Source �̹����� ������ �̹����� ���� // Create the Destination2 image as the Source image.
		if((res =arrFliImage[EType_Destination2].Assign(arrFliImage[EType_Source])).IsFail())
		{
			ErrorPrint(res, L"Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create the source image view
		if((res =arrViewImage[EType_Source].Create(100, 0, 612, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination1 �̹��� �� ����  // Create the destination1 image
		if((res =arrViewImage[EType_Destination1].Create(612, 0,1124, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create the destination2 image
		if((res =arrViewImage[EType_Destination2].Create(1124, 0,1636, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷���  Displays the image in the image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if((res =arrViewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(res, L"Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res =arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination1])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views.
		if((res =arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination2])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res =arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination1])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res =arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination2])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// EmphasizeFilter ��ü ���� // Create EmphasizeFilter object
		CEmphasizeFilter emphasizeFilter;
		// Source �̹��� ���� // Set the source Image
		emphasizeFilter.SetSourceImage(arrFliImage[EType_Source]);
		// Destination �̹��� ���� // Set the destination Image
		emphasizeFilter.SetDestinationImage(arrFliImage[EType_Destination1]);
		// �Ķ���� �� ���� // Set parameters
 		emphasizeFilter.SetMaskWidth(7);
 		emphasizeFilter.SetMaskHeight(7);
 		emphasizeFilter.SetFactor(1.5);
		emphasizeFilter.SetPaddingMethod(EPaddingMethod_DecreasingKernel);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute the algorithm according to the previously set parameters
		if((res =emphasizeFilter.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute operation rolling.");
			break;
		}

		// Destination �̹��� ���� // Set the destination Image
		emphasizeFilter.SetDestinationImage(arrFliImage[EType_Destination2]);
		// �Ķ���� �� ���� // Set parameters
		emphasizeFilter.SetMaskWidth(3);
		emphasizeFilter.SetMaskHeight(3);
		emphasizeFilter.SetFactor(2.5);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute the algorithm according to the previously set parameters
		if((res =emphasizeFilter.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute operation rolling.");
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Get layer 0 from Image View to display on the screen
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // Since this object belongs to the ImageView, it does not need to be freed.
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			// ������ Layer�� �׷��� �������� ���� // Delete the shapes drawn on the existing layer
			arrLayer[i].Clear();
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas draws a String based on screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color. // If the color parameter is set as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by treating it as a background color
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res =arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res =arrLayer[EType_Destination1].DrawTextCanvas(&CFLPointD(0, 0), L"EmphasizeFilter Mask: 7x7 Factor: 1.50", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res =arrLayer[EType_Destination2].DrawTextCanvas(&CFLPointD(0, 0), L"EmphasizeFilter Mask: 3x3 Factor: 2.50", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image views
		arrViewImage[EType_Source].Invalidate(true);
		arrViewImage[EType_Destination1].Invalidate(true);
		arrViewImage[EType_Destination2].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the imageview to exit.
		while(arrViewImage[EType_Source].IsAvailable()
			&& arrViewImage[EType_Destination1].IsAvailable()
			&& arrViewImage[EType_Destination2].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}