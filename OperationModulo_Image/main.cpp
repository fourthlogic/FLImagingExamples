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
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult eResult;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if(IsFail(eResult = arrFliImage[EType_Source].Load(L"../../ExampleImages/OperationModulo/Sunset.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Operand �̹��� �ε� // Loads the operand image
		if(IsFail(eResult = arrFliImage[EType_Operand].Load(L"../../ExampleImages/OperationModulo/Sun.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹����� Source �̹����� ������ �̹����� ���� // Create destination image as same as source image
		if(IsFail(eResult = arrFliImage[EType_Destination].Assign(arrFliImage[EType_Source])))
		{
			ErrorPrint(eResult, "Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(eResult = arrViewImage[EType_Source].Create(100, 0, 612, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Operand �̹��� �� ���� // Creates operand image view
		if(IsFail(eResult = arrViewImage[EType_Operand].Create(612, 0,1124, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if(IsFail(eResult = arrViewImage[EType_Destination].Create(1124, 0,1636, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
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

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Operand])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Operand])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Operation Modulo ��ü ���� // Create Operation Modulo object
		COperationModulo modulo;
		// Source �̹��� ���� // Set the source image
		modulo.SetSourceImage(arrFliImage[EType_Source]);
		// Operand �̹��� ���� // Set the operand image
		modulo.SetOperandImage(arrFliImage[EType_Operand]);
		// Destination �̹��� ���� // Set the destination image
		modulo.SetDestinationImage(arrFliImage[EType_Destination]);
		// ���� ��� ���� // Set operation source
		modulo.SetOperationSource(EOperationSource_Image);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = modulo.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute operation modulo.");
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
		if(IsFail(eResult = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Operand].DrawTextCanvas(&CFLPointD(0, 0), L"Operand Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination].DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		arrViewImage[EType_Source].Invalidate(true);
		arrViewImage[EType_Operand].Invalidate(true);
		arrViewImage[EType_Destination].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(arrViewImage[EType_Source].IsAvailable()
			&& arrViewImage[EType_Operand].IsAvailable()
			&& arrViewImage[EType_Destination].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}