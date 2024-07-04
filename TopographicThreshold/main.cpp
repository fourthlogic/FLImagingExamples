#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[4];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage[4];

    // ���� ��� ��ü ���� // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = arrFliImage[0].Load(L"../../ExampleImages/TopographicThreshold/Lena_1Ch_F32.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create destination1 image as same as source image
		if((res = arrFliImage[1].Assign(arrFliImage[0])).IsFail())
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Destination2 �̹����� Source �̹����� ������ �̹����� ���� // Create destination2 image as same as source image
		if((res = arrFliImage[2].Assign(arrFliImage[0])).IsFail())
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Destination3 �̹����� Source �̹����� ������ �̹����� ���� // Create destination3 image as same as source image
		if((res = arrFliImage[3].Assign(arrFliImage[0])).IsFail())
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((res = viewImage[0].Create(100, 0, 548, 448)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination1 �̹��� �� ���� // Create destination1 image view
		if((res = viewImage[1].Create(548, 0, 996, 448)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination2 image view
		if((res = viewImage[2].Create(996, 0, 1444, 448)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination3 �̹��� �� ���� // Create destination3 image view
		if((res = viewImage[3].Create(1444, 0, 1892, 448)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		for(int32_t i = 0; i < 4; ++i)
		{
			if((res = viewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImage[0].SynchronizePointOfView(&viewImage[1])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImage[0].SynchronizePointOfView(&viewImage[2])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImage[0].SynchronizePointOfView(&viewImage[3])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = viewImage[0].SynchronizeWindow(&viewImage[1])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = viewImage[0].SynchronizeWindow(&viewImage[2])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = viewImage[0].SynchronizeWindow(&viewImage[3])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Topographic Threshold ��ü ���� // Create Topographic Threshold object
		CTopographicThreshold topographicThreshold;
		// Source �̹��� ���� // Set the source image
		topographicThreshold.SetSourceImage(arrFliImage[0]);
		// Destination �̹��� ���� // Set the destination image
		topographicThreshold.SetDestinationImage(arrFliImage[1]);

		// Radius ���� // Set the Radius value
		topographicThreshold.SetRadius(2);

		// �տ��� ������ �Ķ���� ��� Topographic Threshold ���� // Execute Topographic Threshold algorithm according to previously set parameters
		if((res = topographicThreshold.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Topographic Threshold.");
			wprintf(res.GetString());
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		topographicThreshold.SetDestinationImage(arrFliImage[2]);

		// Radius ���� // Set the Radius value
		topographicThreshold.SetRadius(3);

		// �տ��� ������ �Ķ���� ��� Topographic Threshold ���� // Execute Topographic Threshold algorithm according to previously set parameters
		if((res = topographicThreshold.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Topographic Threshold.");
			wprintf(res.GetString());
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		topographicThreshold.SetDestinationImage(arrFliImage[3]);

		// Radius ���� // Set the Radius value
		topographicThreshold.SetRadius(4);

		// �տ��� ������ �Ķ���� ��� Topographic Threshold ���� // Execute Topographic Threshold algorithm according to previously set parameters
		if((res = topographicThreshold.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Topographic Threshold.");
			wprintf(res.GetString());
			break;
		}

		CGUIViewImageLayerWrap arrLayer[4];

		for(int32_t i = 0; i < 4; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = viewImage[i].GetLayer(0);

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
		if((res = arrLayer[0].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = arrLayer[1].DrawTextCanvas(&CFLPointD(0, 0), L"Destination1 Image\nRadius: 2", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = arrLayer[2].DrawTextCanvas(&CFLPointD(0, 0), L"Destination2 Image\nRadius: 3", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = arrLayer[3].DrawTextCanvas(&CFLPointD(0, 0), L"Destination3 Image\nRadius: 4", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// Destination �̹��� ���� Pixel ���� �÷����Ͽ� �� �� �ֵ��� ���� // Show Colored Pixel Values on Destination Image Views
		for(int32_t i = 1; i < 4; ++i)
			viewImage[i].EnablePixelSegmentationMode(true);

		// �̹��� �並 ���� // Update image view
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);
		viewImage[2].Invalidate(true);
		viewImage[3].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable()
			  && viewImage[1].IsAvailable()
			  && viewImage[2].IsAvailable()
			  && viewImage[3].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}