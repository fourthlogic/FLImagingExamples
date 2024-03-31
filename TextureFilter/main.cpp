#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

enum EType
{
	EType_Source = 0,
	EType_Destination1,
	EType_Destination2,
	EType_Destination3,
	EType_Destination4,
	ETypeCount,
};

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	// �˰��� ���� ��� // Algorithm execution result
	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = arrFliImage[EType_Source].Load(L"../../ExampleImages/TextureFilter/Owl.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		bool bError = false;

		for(int32_t i = EType_Destination1; i < ETypeCount; ++i)
		{
			// Destination �̹����� Source �̹����� ������ �̹����� ���� // Create destination image as same as source image
			if((res = arrFliImage[i].Assign(arrFliImage[EType_Source])).IsFail())
			{
				ErrorPrint(res, L"Failed to assign the image file.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			int32_t x = i % 3;
			int32_t y = i / 3;

			// �̹��� �� ���� // Create image view
			if((res = arrViewImage[i].Create(x * 400 + 400, y * 400, x * 400 + 400 + 400, y * 400 + 400)).IsFail())
			{
				ErrorPrint(res, L"Failed to create the image view.\n");
				bError = true;
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
			if((res = arrViewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(res, L"Failed to set image object on the image view.\n");
				bError = true;
				break;
			}

			if(i == EType_Source)
				continue;

			// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
			if((res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[i])).IsFail())
			{
				ErrorPrint(res, L"Failed to synchronize view\n");
				bError = true;
				break;
			}

			// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
			if((res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[i])).IsFail())
			{
				ErrorPrint(res, L"Failed to synchronize window.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// Texture filter ��ü ���� // Create Texture filter object
		CTextureFilter textureFilter;
		// Source �̹��� ���� // Set the source image
		textureFilter.SetSourceImage(arrFliImage[EType_Source]);

		// Destination1 �̹��� ���� // Set the destination1 image
		textureFilter.SetDestinationImage(arrFliImage[EType_Destination1]);

		// Filter Width ���� // Set the Filter Width
		textureFilter.SetFilterWidth(3);

		// Filter Height �� ���� // Set the Filter Height
		textureFilter.SetFilterHeight(3);

		// Filter Type Width ���� // Set the Filter Type Width
		textureFilter.SetFilterTypeWidth(CTextureFilter::EFilterType_Level);

		// Filter Type Height �� ���� // Set the Filter Type Height
		textureFilter.SetFilterTypeHeight(CTextureFilter::EFilterType_Edge);

		// Shift value ���� // Set the Shift value
		textureFilter.SetShift(0);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = textureFilter.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute algorithm.");
			break;
		}

		// Destination2 �̹��� ���� // Set the destination2 image
		textureFilter.SetDestinationImage(arrFliImage[EType_Destination2]);

		// Filter Width ���� // Set the Filter Width
		textureFilter.SetFilterWidth(5);

		// Filter Height �� ���� // Set the Filter Height
		textureFilter.SetFilterHeight(5);

		// Filter Type Width ���� // Set the Filter Type Width
		textureFilter.SetFilterTypeWidth(CTextureFilter::EFilterType_Spot);

		// Filter Type Height �� ���� // Set the Filter Type Height
		textureFilter.SetFilterTypeHeight(CTextureFilter::EFilterType_Spot);

		//Shift value ���� // Set the Shift value
		textureFilter.SetShift(1);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = textureFilter.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute algorithm.");
			break;
		}

		// Destination3 �̹��� ���� // Set the destination3 image
		textureFilter.SetDestinationImage(arrFliImage[EType_Destination3]);

		// Filter Width ���� // Set the Filter Width
		textureFilter.SetFilterWidth(5);

		// Filter Height �� ���� // Set the Filter Height
		textureFilter.SetFilterHeight(5);

		// Filter Type Width ���� // Set the Filter Type Width
		textureFilter.SetFilterTypeWidth(CTextureFilter::EFilterType_Level);

		// Filter Type Height �� ���� // Set the Filter Type Height
		textureFilter.SetFilterTypeHeight(CTextureFilter::EFilterType_Ripple);

		// Shift value ���� // Set the Shift value
		textureFilter.SetShift(-3);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = textureFilter.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute Texture filter.");
			break;
		}

		// Destination4 �̹��� ���� // Set the destination4 image
		textureFilter.SetDestinationImage(arrFliImage[EType_Destination4]);

		// Filter Width ���� // Set the Filter Width
		textureFilter.SetFilterWidth(7);

		// Filter Height �� ���� // Set the Filter Height
		textureFilter.SetFilterHeight(7);

		// Filter Type Width ���� // Set the Filter Type Width
		textureFilter.SetFilterTypeWidth(CTextureFilter::EFilterType_Undulation);

		// Filter Type Height �� ���� // Set the Filter Type Height
		textureFilter.SetFilterTypeHeight(CTextureFilter::EFilterType_Undulation);

		// Shift value ���� // Set the Shift value
		textureFilter.SetShift(-2);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = textureFilter.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute algorithm.");
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			arrLayer[i].Clear();
		}

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPointD(0, 0), L"Destination1 Image (Level, Edge)", YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPointD(0, 0), L"Destination2 Image (Spot, Spot)", YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination3].DrawTextCanvas(&CFLPointD(0, 0), L"Destination3 Image (Level, Ripple)", YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination4].DrawTextCanvas(&CFLPointD(0, 0), L"Destination4 Image (Undulation, Undulation)", YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		for(int32_t i = 0; i < ETypeCount; ++i)
			arrViewImage[i].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		bool bAvailable = true;
		while(bAvailable)
		{
			for(int32_t i = 0; i < ETypeCount; ++i)
			{
				bAvailable = arrViewImage[i].IsAvailable();
				if(!bAvailable)
					break;
			}

			CThreadUtilities::Sleep(1);
		}
	}
	while(false);

	return 0;
}