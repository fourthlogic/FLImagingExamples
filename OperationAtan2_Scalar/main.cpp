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

	do
	{
		// ���� ��� // operation result
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(eResult = arrFliImage[EType_Source].Load(L"../../ExampleImages/OperationAtan2/Flower.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = arrViewImage[EType_Source].Create(100, 0, 612, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = arrViewImage[EType_Destination1].Create(612, 0, 1124, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = arrViewImage[EType_Destination2].Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if(IsFail(eResult = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(eResult, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views.
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views.
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination2])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination2])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Operation atan2 ��ü ���� // Create Atan2 object
		COperationAtan2 atan2;
		// Source �̹��� ���� // Set source image
		atan2.SetSourceImage(arrFliImage[EType_Source]);
		// Destination �̹��� ���� // Set destination image 
		atan2.SetDestinationImage(arrFliImage[EType_Destination1]);
		// ���� ��� ���� // Set operation source
		atan2.SetOperationSource(EOperationSource_Scalar);
		// Scalar �� ���� // Set Scalar value
		atan2.SetScalarValue(CMultiVar<double>(1, 1, 1));

		// �˰��� ���� // Execute the algorithm
		if((eResult = atan2.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute operation atan2.");
			break;
		}

		// Destination �̹��� ���� // Set destination image 
		atan2.SetDestinationImage(arrFliImage[EType_Destination2]);
		// ���� ��� ���� // Set operation source
		atan2.SetOperationSource(EOperationSource_Scalar);
		// Scalar �� ���� // Set Scalar value
		atan2.SetScalarValue(CMultiVar<double>(65535, 65535, 65535));

		// �˰��� ���� // Execute the algorithm
		if((eResult = atan2.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute operation atan2.");
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
			// ���� ������ �ʿ� ���� // No need to release separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			// ������ Layer�� �׷��� �������� ���� // Delete the shapes drawn on the existing layer
			arrLayer[i].Clear();
		}

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 17)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPointD(0, 0), L"Destination1 Image(Atan2 1, 1, 1)", YELLOW, BLACK, 17)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPointD(0, 0), L"Destination2 Image(Atan2 65535, 65535, 65535)", YELLOW, BLACK, 17)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
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