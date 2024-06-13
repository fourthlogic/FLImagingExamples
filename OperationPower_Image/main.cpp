#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


enum EType
{
	EType_Source = 0,
	EType_Operand,
	EType_Destination,
	ETypeCount,
};

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage0[ETypeCount];
	CFLImage arrFliImage1[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage0[ETypeCount];
	CGUIViewImageWrap arrViewImage1[ETypeCount];

	// �˰��� ���� ��� // Algorithm execution result
	CResult result = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((result = arrFliImage0[EType_Source].Load(L"../../ExampleImages/OperationPower/Sea3Ch.flif")).IsFail())
		{
			ErrorPrint(result, L"Failed to load the image file.\n");
			break;
		}

		// Operand �̹��� �ε� // Loads the operand image
		if((result = arrFliImage0[EType_Operand].Load(L"../../ExampleImages/OperationPower/Gradation.flif")).IsFail())
		{
			ErrorPrint(result, L"Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �ε� // Load the source image
		if((result = arrFliImage1[EType_Source].Load(L"../../ExampleImages/OperationPower/Sea3ChF32.flif")).IsFail())
		{
			ErrorPrint(result, L"Failed to load the image file.\n");
			break;
		}

		// Operand �̹��� �ε� // Loads the operand image
		if((result = arrFliImage1[EType_Operand].Load(L"../../ExampleImages/OperationPower/Gradation.flif")).IsFail())
		{
			ErrorPrint(result, L"Failed to load the image file.\n");
			break;
		}

		// Destination �̹����� Source �̹����� ������ �̹����� ���� // Create destination image as same as source image
		if((result = arrFliImage0[EType_Destination].Assign(arrFliImage0[EType_Source])).IsFail())
		{
			ErrorPrint(result, L"Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((result = arrViewImage0[EType_Source].Create(100, 0, 548, 448)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		// Operand �̹��� �� ���� // Creates operand image view
		if((result = arrViewImage0[EType_Operand].Create(548, 0, 996, 448)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if((result = arrViewImage0[EType_Destination].Create(996, 0, 1444, 448)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}
		// Source �̹��� �� ���� // Create Source image view
		if((result = arrViewImage1[EType_Source].Create(100, 448, 548, 896)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		// Operand �̹��� �� ���� // Creates operand image view
		if((result = arrViewImage1[EType_Operand].Create(548, 448, 996, 896)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if((result = arrViewImage1[EType_Destination].Create(996, 448, 1444, 896)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if((result = arrViewImage0[i].SetImagePtr(&arrFliImage0[i])).IsFail())
			{
				ErrorPrint(result, L"Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if((result = arrViewImage1[i].SetImagePtr(&arrFliImage1[i])).IsFail())
			{
				ErrorPrint(result, L"Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((result = arrViewImage0[EType_Source].SynchronizePointOfView(&arrViewImage0[EType_Operand])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((result = arrViewImage0[EType_Source].SynchronizePointOfView(&arrViewImage0[EType_Destination])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((result = arrViewImage0[EType_Source].SynchronizeWindow(&arrViewImage0[EType_Operand])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((result = arrViewImage0[EType_Source].SynchronizeWindow(&arrViewImage0[EType_Destination])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((result = arrViewImage0[EType_Source].SynchronizePointOfView(&arrViewImage1[EType_Source])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((result = arrViewImage0[EType_Source].SynchronizePointOfView(&arrViewImage1[EType_Operand])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((result = arrViewImage0[EType_Source].SynchronizePointOfView(&arrViewImage1[EType_Destination])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((result = arrViewImage0[EType_Source].SynchronizeWindow(&arrViewImage1[EType_Source])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((result = arrViewImage0[EType_Source].SynchronizeWindow(&arrViewImage1[EType_Operand])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((result = arrViewImage0[EType_Source].SynchronizeWindow(&arrViewImage1[EType_Destination])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// Operation Power ��ü ���� // Create Operation Power object
		COperationPower power;
		// Source �̹��� ���� // Set the source image
		power.SetSourceImage(arrFliImage0[EType_Source]);
		// Operand �̹��� ���� // Set the operand image
		power.SetOperandImage(arrFliImage0[EType_Operand]);
		// Destination �̹��� ���� // Set the destination image
		power.SetDestinationImage(arrFliImage0[EType_Destination]);
		// Image Operation ���� ���� // Set operation mode to image
		power.SetOperationSource(EOperationSource_Image);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((result = power.Execute()).IsFail())
		{
			ErrorPrint(result, L"Failed to execute operation power.");
			
			break;
		}

		// Source �̹��� ���� // Set the source image
		power.SetSourceImage(arrFliImage1[EType_Source]);
		// Operand �̹��� ���� // Set the operand image
		power.SetOperandImage(arrFliImage1[EType_Operand]);
		// Destination �̹��� ���� // Set the destination image
		power.SetDestinationImage(arrFliImage1[EType_Destination]);
		// Overflow Method Wrapping �ɼ����� ���� // Set Overflow Method to Wrapping option
		power.SetOverflowMethod(EOverflowMethod_Wrapping);
		// Image Operation ���� ���� // Set operation mode to image
		power.SetOperationSource(EOperationSource_Image);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((result = power.Execute()).IsFail())
		{
			ErrorPrint(result, L"Failed to execute operation power.");
			
			break;
		}

		CGUIViewImageLayerWrap arrLayer0[ETypeCount];
		CGUIViewImageLayerWrap arrLayer1[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
			arrLayer0[i] = arrViewImage0[i].GetLayer(0);

			// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
			arrLayer0[i].Clear();
		}

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
			arrLayer1[i] = arrViewImage1[i].GetLayer(0);

			// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
			arrLayer1[i].Clear();
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((result = arrLayer0[EType_Source].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer0[EType_Operand].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Operand Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer0[EType_Destination].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer1[EType_Source].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer1[EType_Operand].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Operand Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer1[EType_Destination].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		arrViewImage0[EType_Source].Invalidate(true);
		arrViewImage0[EType_Operand].Invalidate(true);
		arrViewImage0[EType_Destination].Invalidate(true);
		arrViewImage1[EType_Source].Invalidate(true);
		arrViewImage1[EType_Operand].Invalidate(true);
		arrViewImage1[EType_Destination].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(arrViewImage0[EType_Source].IsAvailable()
			  && arrViewImage0[EType_Operand].IsAvailable()
			  && arrViewImage0[EType_Destination].IsAvailable()
			  && arrViewImage1[EType_Source].IsAvailable()
			  && arrViewImage1[EType_Operand].IsAvailable()
			  && arrViewImage1[EType_Destination].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}