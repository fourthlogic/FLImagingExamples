#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[2];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[2];

	// �˰��� ���� ��� // Algorithm execution result
	CResult result = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((result = arrFliImage[0].Load(L"../../ExampleImages/OperationLesser/block.flif")).IsFail())
		{
			ErrorPrint(result, L"Failed to load the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create destination1 image as same as source image
		if((result = arrFliImage[1].Assign(arrFliImage[0])).IsFail())
		{
			ErrorPrint(result, L"Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((result = arrViewImage[0].Create(100, 0, 612, 512)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if((result = arrViewImage[1].Create(612, 0, 1124, 512)).IsFail())
		{
			ErrorPrint(result, L"Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		for(int32_t i = 0; i < 2; ++i)
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
		if((result = arrViewImage[0].SynchronizePointOfView(&arrViewImage[1])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((result = arrViewImage[0].SynchronizeWindow(&arrViewImage[1])).IsFail())
		{
			ErrorPrint(result, L"Failed to synchronize window.\n");
			break;
		}

		// COperationGreater ��ü ���� // Create COperationGreater object
		COperationGreater greater;
		// Source �̹��� ���� // Set source image
		greater.SetSourceImage(arrFliImage[0]);
		// Destination �̹��� ���� // Set destination image
		greater.SetDestinationImage(arrFliImage[1]);
		// Scalar Operation ���� ���� // Set operation mode to scalar
		greater.SetOperationSource(EOperationSource_Scalar);
		// Greater Scalar �� ���� // Set comparsion value of greater operation
		greater.SetScalarValue(CMultiVar<double>(128));

		// Source�� Scalar���� Ŭ ��� �� ���� // Set output value if source is greater than scalar
		greater.SetRangeValue(CMultiVar<double>(255));
		// Source�� Scalar���� �۰ų� ���� ��� �� ���� // Set output value if source is less than or equal to scalar
		greater.SetOutOfRangeValue(CMultiVar<double>(50));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((result = greater.Execute()).IsFail())
		{
			ErrorPrint(result, L"Failed to execute operation greater.");
			
			break;
		}

		CGUIViewImageLayerWrap arrLayer[2];

		for(int32_t i = 0; i < 2; ++i)
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
		if((result = arrLayer[0].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		if((result = arrLayer[1].DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image(Greater 128)", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(result, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		arrViewImage[0].Invalidate(true);
		arrViewImage[1].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to exit
		while(arrViewImage[0].IsAvailable()
			  && arrViewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}