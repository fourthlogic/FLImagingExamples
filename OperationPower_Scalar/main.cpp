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
	EType_Destination5,
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
		if((result = arrFliImage[EType_Source].Load(L"../../ExampleImages/OperationPower/Space3Ch.flif")).IsFail())
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

		// Destination2 �̹����� Source �̹����� ������ �̹����� ���� // Create destination2 image as same as source image
		if((result = arrFliImage[EType_Destination2].Assign(arrFliImage[EType_Source])).IsFail())
		{
			ErrorPrint(result, L"Failed to assign the image file.\n");
			break;
		}

		// Destination3 �̹��� �ε�. // Load the destination3 image
		if((result = arrFliImage[EType_Destination3].Load(L"../../ExampleImages/OperationPower/Dst16Depth.flif")).IsFail())
		{
			ErrorPrint(result, L"Failed to load the image file.\n");
			break;
		}

		// Destination4 �̹��� �ε�. // Load the destination4 image
		if((result = arrFliImage[EType_Destination4].Load(L"../../ExampleImages/OperationPower/Dst16Depth.flif")).IsFail())
		{
			ErrorPrint(result, L"Failed to load the image file.\n");
			break;
		}

		// Destination5 �̹��� �ε�. // Load the destination5 image
		if((result = arrFliImage[EType_Destination5].Load(L"../../ExampleImages/OperationPower/Dst64Depth.flif")).IsFail())
		{
			ErrorPrint(result, L"Failed to load the image file.\n");
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

		// Destination3 �̹��� �� ���� // Create the destination3 image view
		if((result = arrViewImage[EType_Destination3].Create(100, 448, 548, 896)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		// Destination4 �̹��� �� ���� // Create the destination4 image view
		if((result = arrViewImage[EType_Destination4].Create(548, 448, 996, 896)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		// Destination5 �̹��� �� ���� // Create the destination5 image view
		if((result = arrViewImage[EType_Destination5].Create(996, 448, 1444, 896)).IsFail())
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

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((result = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination3])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((result = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination4])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((result = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination5])).IsFail())
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

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((result = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination3])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((result = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination4])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((result = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination5])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// Operation Power ��ü ���� // Create Operation Power object
		COperationPower power;
		// Source �̹��� ���� // Set the source image
		power.SetSourceImage(arrFliImage[EType_Source]);
		// Destination �̹��� ���� // Set the destination image
		power.SetDestinationImage(arrFliImage[EType_Destination1]);
		// Scalar Operation ���� ���� // Set operation mode to scalar
		power.SetOperationSource(EOperationSource_Scalar);
		// Operation Mode Saturation �ɼ����� ���� // Set Operation Mode to saturation option
		power.SetOperationMode(EOperationMode_Saturated);

		// exponent �� ���� // Set exponent value
		power.SetScalarValue(CMultiVar<double>(1.1, 1.2, 1.5));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((result = power.Execute()).IsFail())
		{
			ErrorPrint(result, L"Failed to execute operation power.");
			
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		power.SetDestinationImage(arrFliImage[EType_Destination2]);
		// Scalar Operation ���� ���� // Set operation mode to scalar
		power.SetOperationSource(EOperationSource_Scalar);
		// Operation Mode Saturation �ɼ����� ���� // Set Operation Mode to saturation option
		power.SetOperationMode(EOperationMode_Saturated);

		// exponent �� ���� // Set exponent value
		power.SetScalarValue(CMultiVar<double>(0.8, 0.8, 1.1));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((result = power.Execute()).IsFail())
		{
			ErrorPrint(result, L"Failed to execute operation power.");
			
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		power.SetDestinationImage(arrFliImage[EType_Destination3]);
		// Scalar Operation ���� ���� // Set operation mode to scalar
		power.SetOperationSource(EOperationSource_Scalar);
		// Operation Mode Overflow �ɼ����� ���� // Set Operation Mode to overflow option
		power.SetOperationMode(EOperationMode_Overflow);

		// power �� ���� // Set power value
		power.SetScalarValue(CMultiVar<double>(2.5, 2.5, 2.5));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((result = power.Execute()).IsFail())
		{
			ErrorPrint(result, L"Failed to execute operation power.");
			
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		power.SetDestinationImage(arrFliImage[EType_Destination4]);
		// Scalar Operation ���� ���� // Set operation mode to scalar
		power.SetOperationSource(EOperationSource_Scalar);
		// Operation Mode Saturation �ɼ����� ���� // Set Operation Mode to saturation option
		power.SetOperationMode(EOperationMode_Saturated);

		// exponent �� ���� // Set exponent value
		power.SetScalarValue(CMultiVar<double>(2.5, 2.5, 2.5));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((result = power.Execute()).IsFail())
		{
			ErrorPrint(result, L"Failed to execute operation power.");
			
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		power.SetDestinationImage(arrFliImage[EType_Destination5]);
		// Scalar Operation ���� ���� // Set operation mode to scalar
		power.SetOperationSource(EOperationSource_Scalar);
		// Operation Mode Overflow �ɼ����� ���� // Set Operation Mode to overflow option
		power.SetOperationMode(EOperationMode_Overflow);

		// exponent �� ���� // Set exponent value
		power.SetScalarValue(CMultiVar<double>(10, 10, 10));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((result = power.Execute()).IsFail())
		{
			ErrorPrint(result, L"Failed to execute operation power.");
			
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
		if((result = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(5, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPointD(5, 0), L"Destination1 Image(Power 1.1, 1.2, 1.5)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}
		
		if((result = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPointD(5, 22), L"Unsigned Int / 8 / Saturation", YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPointD(5, 0), L"Destination2 Image(Power 0.8, 0.8, 1.1)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}
		
		if((result = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPointD(5, 22), L"Unsigned Int / 8 / Saturation", YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer[EType_Destination3].DrawTextCanvas(&CFLPointD(5, 0), L"Destination3 Image(Power 2.5, 2.5, 2.5)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer[EType_Destination3].DrawTextCanvas(&CFLPointD(5, 22), L"Unsigned Int / 16 / Overflow", YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer[EType_Destination4].DrawTextCanvas(&CFLPointD(5, 0), L"Destination4 Image(Power 2.5, 2.5, 2.5)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer[EType_Destination4].DrawTextCanvas(&CFLPointD(5, 22), L"Unsigned Int / 16 / Saturation", YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer[EType_Destination5].DrawTextCanvas(&CFLPointD(5, 0), L"Destination5 Image(Power 10, 10, 10)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer[EType_Destination5].DrawTextCanvas(&CFLPointD(5, 22), L"Unsigned Int / 64 / Overflow", YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		arrViewImage[EType_Source].Invalidate(true);
		arrViewImage[EType_Destination1].Invalidate(true);
		arrViewImage[EType_Destination2].Invalidate(true);
		arrViewImage[EType_Destination3].Invalidate(true);
		arrViewImage[EType_Destination4].Invalidate(true);
		arrViewImage[EType_Destination5].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(arrViewImage[EType_Source].IsAvailable()
			  && arrViewImage[EType_Destination1].IsAvailable()
			  && arrViewImage[EType_Destination2].IsAvailable()
			  && arrViewImage[EType_Destination3].IsAvailable()
			  && arrViewImage[EType_Destination4].IsAvailable()
			  && arrViewImage[EType_Destination5].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}