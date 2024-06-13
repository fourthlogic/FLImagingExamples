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
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	// �˰��� ���� ��� // Algorithm execution result
	CResult result = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((result = arrFliImage[EType_Source].Load(L"../../ExampleImages/OperationSquare/Sun.flif")).IsFail())
		{
			ErrorPrint(result, L"Failed to load the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create destination1 image as same as source image
		if((result = arrFliImage[EType_Destination1].Assign(arrFliImage[EType_Source])).IsFail())
		{
			ErrorPrint(result, L"Failed to assign the image file.\n");
			break;
		}

		// Destination2 �̹��� �ε�. // Load the destination2 image
		if((result = arrFliImage[EType_Destination2].Load(L"../../ExampleImages/OperationSquare/DstF32Depth.flif")).IsFail())
		{
			ErrorPrint(result, L"Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((result = arrViewImage[EType_Source].Create(100, 0, 548, 448)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		// Destination1 �̹��� �� ���� // Create destination1 image view
		if((result = arrViewImage[EType_Destination1].Create(548, 0, 996, 448)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination2 image view
		if((result = arrViewImage[EType_Destination2].Create(996, 0, 1444, 448)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if((result = arrViewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(result, L"Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((result = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination1])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((result = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination2])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((result = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination1])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((result = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination2])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// Operation Square ��ü ���� // Create Operation Square object
		COperationSquare square;
		// Source �̹��� ���� // Set the source image
		square.SetSourceImage(arrFliImage[EType_Source]);
		// Destination �̹��� ���� // Set the destination image
		square.SetDestinationImage(arrFliImage[EType_Destination1]);
		// Overflow Method Clamping �ɼ����� ���� // Set Overflow Method to Clamping option
		square.SetOverflowMethod(EOverflowMethod_Clamping);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((result = square.Execute()).IsFail())
		{
			ErrorPrint(result, L"Failed to execute operation square.");
			
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		square.SetDestinationImage(arrFliImage[EType_Destination2]);
		// Overflow Method Clamping �ɼ����� ���� // Set Overflow Method to Clamping option
		square.SetOverflowMethod(EOverflowMethod_Clamping);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((result = square.Execute()).IsFail())
		{
			ErrorPrint(result, L"Failed to execute operation square.");
			
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
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((result = arrLayer[EType_Source].DrawTextCanvas(&CFLPoint<double>(5, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPoint<double>(5, 0), L"Destination1 Image", YELLOW, BLACK, 25)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}
		
		if((result = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPoint<double>(5, 27), L"Unsigned Int / 8 / Clamping", YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPoint<double>(5, 0), L"Destination2 Image", YELLOW, BLACK, 25)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}
		
		if((result = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPoint<double>(5, 27), L"Floating Point / 32", YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		arrViewImage[EType_Source].Invalidate(true);
		arrViewImage[EType_Destination1].Invalidate(true);
		arrViewImage[EType_Destination2].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(arrViewImage[EType_Source].IsAvailable()
			  && arrViewImage[EType_Destination1].IsAvailable()
			  && arrViewImage[EType_Destination2].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}