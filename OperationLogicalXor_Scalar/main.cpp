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
	// �̹��� ��ü ����
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ����
	CGUIViewImageWrap arrViewImage[ETypeCount];

    // ���� ��� ��ü ���� // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = arrFliImage[EType_Source].Load(L"../../ExampleImages/OperationLogicalOr/Cat.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create destination1 image as same as source image
		if((res = arrFliImage[EType_Destination1].Assign(arrFliImage[EType_Source])).IsFail())
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create destination1 image as same as source image
		if((res = arrFliImage[EType_Destination2].Assign(arrFliImage[EType_Source])).IsFail())
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((res = arrViewImage[EType_Source].Create(100, 0, 612, 512)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination1 �̹��� �� ���� // Create destination1 image view
		if((res = arrViewImage[EType_Destination1].Create(612, 0, 1124, 512)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination2 image view
		if((res = arrViewImage[EType_Destination2].Create(1124, 0, 1636, 512)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if((res = arrViewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination1])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination2])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination1])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination2])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Operation LogicalXor ��ü ���� // Create Operation LogicalXor object
		COperationLogicalXor logicalXor;
		// Source �̹��� ���� // Set the source image
		logicalXor.SetSourceImage(arrFliImage[EType_Source]);
		// Destination �̹��� ���� // Set the destination image
		logicalXor.SetDestinationImage(arrFliImage[EType_Destination1]);
		// Image Operation �ҽ��� ���� // Set Operation Source to image
		logicalXor.SetOperationSource(EOperationSource_Scalar);
		// Logical Xor �� ���� // Set Logical Xor value
		logicalXor.SetScalarValue(CMultiVar<double>(0, 0, 0));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = logicalXor.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute operation logical xor.");
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		logicalXor.SetDestinationImage(arrFliImage[EType_Destination2]);
		// Image Operation �ҽ��� ���� // Set Operation Source to image
		logicalXor.SetOperationSource(EOperationSource_Scalar);
		// LogicalXor �� ���� // Set LogicalXor value
		logicalXor.SetScalarValue(CMultiVar<double>(255, 255, 255));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = logicalXor.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute operation logical xor.");
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			// ������ Layer�� �׷��� �������� ����
			arrLayer[i].Clear();
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = arrLayer[EType_Source].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 25)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination1 Image(LogicalXor 0)", YELLOW, BLACK, 25)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination2 Image(LogicalXor 255)", YELLOW, BLACK, 25)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
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