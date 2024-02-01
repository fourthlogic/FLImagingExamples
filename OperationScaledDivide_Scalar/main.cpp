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
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult eResult;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if(IsFail(eResult = arrFliImage[EType_Source].Load(L"../../ExampleImages/OperationScaledDivide/Generator.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create destination1 image as same as source image
		if(IsFail(eResult = arrFliImage[EType_Destination1].Assign(arrFliImage[EType_Source])))
		{
			ErrorPrint(eResult, "Failed to assign the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create destination1 image as same as source image
		if(IsFail(eResult = arrFliImage[EType_Destination2].Assign(arrFliImage[EType_Source])))
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

		// Destination1 �̹��� �� ���� // Create destination1 image view
		if(IsFail(eResult = arrViewImage[EType_Destination1].Create(612, 0,1124, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination2 image view
		if(IsFail(eResult = arrViewImage[EType_Destination2].Create(1124, 0,1636, 512)))
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
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
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

		// Operation ScaledDivide ��ü ���� // Create Operation ScaledDivide object
		COperationScaledDivide scaledDivide;
		// Source �̹��� ���� // Set the source image
		scaledDivide.SetSourceImage(arrFliImage[EType_Source]);
		// Destination �̹��� ���� // Set the destination image
		scaledDivide.SetDestinationImage(arrFliImage[EType_Destination1]);
		// ���� ��� ���� // Set operation source
		scaledDivide.SetOperationSource(EOperationSource_Scalar);
		// ScaledDivide �� ���� // Set ScaledDivide value
		scaledDivide.SetScalarValue(CMultiVar<double>(128, 128, 128));

		// �ռ� ������ �Ķ���� ��� �˰����� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = scaledDivide.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute operation ScaledDivide.");
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		scaledDivide.SetDestinationImage(arrFliImage[EType_Destination2]);
		// ���� ��� ���� // Set operation source
		scaledDivide.SetOperationSource(EOperationSource_Scalar);
		// ScaledDivide �� ���� // Set ScaledDivide value
		scaledDivide.SetScalarValue(CMultiVar<double>(512, 512, 512));

		// �ռ� ������ �Ķ���� ��� �˰����� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = scaledDivide.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute operation ScaledDivide.");
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
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� ���������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(��������) -> �� ���İ� (��������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPointD(0, 0), L"Destination1 Image(ScaledDivide 128)", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPointD(0, 0), L"Destination2 Image(ScaledDivide 512)", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
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