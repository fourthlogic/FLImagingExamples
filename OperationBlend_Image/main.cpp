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
	// �̹��� ��ü ���� // Declare the image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap arrViewImage[ETypeCount];
	
	// ���� ��� // operation result
	CResult eResult = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((eResult = arrFliImage[EType_Source].Load(L"../../ExampleImages/OperationBlend/Sky.flif")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the image file.\n");
			break;
		}

		// Operand �̹��� �ε� // Load the operand image
		if((eResult = arrFliImage[EType_Operand].Load(L"../../ExampleImages/OperationBlend/Flower.flif")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the image file.\n");
			break;
		}

		// Destination �̹����� Source �̹����� ������ �̹����� ���� // Create destination image as same as source image
		if((arrFliImage[EType_Destination].Assign(arrFliImage[EType_Source])).IsFail())
		{
			ErrorPrint(eResult, L"Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create source image view
		if((eResult = arrViewImage[EType_Source].Create(100, 0, 612, 512)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Operand �̹��� �� ���� // Create operand image view
		if((eResult = arrViewImage[EType_Operand].Create(612, 0, 1124, 512)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if((eResult = arrViewImage[EType_Destination].Create(1124, 0, 1636, 512)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if((eResult = arrViewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Operand])).IsFail())
		{
			ErrorPrint(eResult, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination])).IsFail())
		{
			ErrorPrint(eResult, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Operand])).IsFail())
		{
			ErrorPrint(eResult, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination])).IsFail())
		{
			ErrorPrint(eResult, L"Failed to synchronize window.\n");
			break;
		}

		// Operation Blend ��ü ���� // Create Operation Blend object
		COperationBlend blend;
		// Source �̹��� ���� // Set the source image
		blend.SetSourceImage(arrFliImage[EType_Source]);
		// Operand �̹��� ���� // Set the operand image
		blend.SetOperandImage(arrFliImage[EType_Operand]);
		// Destination �̹��� ���� // Set the destination image
		blend.SetDestinationImage(arrFliImage[EType_Destination]);
		// ���� ��� ���� // Set operation source
		blend.SetOperationSource(EOperationSource_Image);
		// Blend Ratio ���� // Set Blend Ratio
		blend.SetSourceRatio(0.75f);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = blend.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute operation blend.");
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
		if((eResult = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		if((eResult = arrLayer[EType_Operand].DrawTextCanvas(&CFLPointD(0, 0), L"Operand Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		if((eResult = arrLayer[EType_Destination].DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}
		if((eResult = arrLayer[EType_Destination].DrawTextCanvas(&CFLPointD(0, 32), L"Source Ratio 0.75, Operand Ratio 0.25", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
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