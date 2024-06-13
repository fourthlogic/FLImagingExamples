#include <cstdio>

#include <FLImaging.h>


enum EType
{
	EType_Source = 0,
	EType_Destination,
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
		// �̹��� �ε� // Loads image
		if(IsFail(arrFliImage[EType_Source].Load(L"../../ExampleImages/OperationLogicalNot/Gambling.flif")))
		{
			printf("Failed to load the image file.\n");
			break;
		}

		// �̹����� Copy // image copy
		if(IsFail(arrFliImage[EType_Destination].Assign(arrFliImage[EType_Source])))
		{
			printf("Failed to assign the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(arrViewImage[EType_Source].Create(100, 0, 612, 512)))
		{
			printf("Failed to create the image view.\n");
			break;
		}

		if(IsFail(arrViewImage[EType_Destination].Create(612, 0,1124, 512)))
		{
			printf("Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if(IsFail(arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				printf("Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views.
		if(IsFail(arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination])))
		{
			printf("Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination])))
		{
			printf("Failed to synchronize window.\n");
			break;
		}

		// Operation Logical Not ��ü ���� // Create Logical Not object
		COperationLogicalNot LogicalNot;
		// Source �̹��� ���� // Set source image
		LogicalNot.SetSourceImage(arrFliImage[EType_Source]);
		// Destination �̹��� ���� // Set destination image 
		LogicalNot.SetDestinationImage(arrFliImage[EType_Destination]);

		// Operation Logical Not ����
		if(IsFail(LogicalNot.Execute()))
		{
			printf("Failed to execute operation Logical Not.");
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
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(arrLayer[EType_Source].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			printf("Failed to draw text\n");
			break;
		}

		if(IsFail(arrLayer[EType_Destination].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
		{
			printf("Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		arrViewImage[EType_Source].Invalidate(true);
		arrViewImage[EType_Destination].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(arrViewImage[EType_Source].IsAvailable()
			&& arrViewImage[EType_Destination].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}