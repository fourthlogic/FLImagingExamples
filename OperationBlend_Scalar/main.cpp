#include <cstdio>

#include "../CommomHeader/ErrorPrint.h"
#include <FLImaging.h>


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

	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = arrFliImage[EType_Source].Load(L"../../ExampleImages/OperationBlend/Sky.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create destination1 image as same as source image
		if((res = arrFliImage[EType_Destination1].Assign(arrFliImage[EType_Source])).IsFail())
		{
			ErrorPrint(res, L"Failed to assign the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create destination1 image as same as source image
		if((res = arrFliImage[EType_Destination2].Assign(arrFliImage[EType_Source])).IsFail())
		{
			ErrorPrint(res, L"Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((res = arrViewImage[EType_Source].Create(100, 0, 612, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination1 �̹��� �� ���� // Create destination1 image view
		if((res = arrViewImage[EType_Destination1].Create(612, 0, 1124, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination2 image view
		if((res = arrViewImage[EType_Destination2].Create(1124, 0, 1636, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if((res = arrViewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(res, L"Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination1])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination2])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination1])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination2])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �˰��� ���� ��� // Algorithm execution result
		res = EResult_UnknownError;

		// Operation Blend ��ü ���� // Create Operation Blend object
		COperationBlend blend;
		// Source �̹��� ���� // Set the source image
		blend.SetSourceImage(arrFliImage[EType_Source]);
		// Destination �̹��� ���� // Set the destination image
		blend.SetDestinationImage(arrFliImage[EType_Destination1]);
		// ���� ��� ���� // Set operation source
		blend.SetOperationSource(EOperationSource_Scalar);
		// blend �� ���� // Set Blend value
		blend.SetScalarValue(CMultiVar<double>(30, 0, 0));
		// Blend Ratio ���� // Set Blend Ratio
		blend.SetSourceRatio(0.75f);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = blend.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute operation blend.");
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		blend.SetDestinationImage(arrFliImage[EType_Destination2]);
		// ���� ��� ���� // Set operation source
		blend.SetOperationSource(EOperationSource_Scalar);
		// blend �� ���� // Set blend value
		blend.SetScalarValue(CMultiVar<double>(0, 30, 0));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = blend.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute operation blend.");
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
			// ���� ������ �ʿ� ���� // No need to release separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
			arrLayer[i].Clear();
		}

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPointD(0, 0), L"Destination1 Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}
		if((res = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPointD(0, 32), L"Source Ratio 0.75, Scalar(30,0,0) Ratio 0.25", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPointD(0, 0), L"Destination2 Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}
		if((res = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPointD(0, 32), L"Source Ratio 0.75, Scalar(0,30,0) Ratio 0.25", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
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