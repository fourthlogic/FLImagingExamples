#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


enum EType
{
	EType_Source = 0,
	EType_MedianWeightedFilter1,
	EType_MedianWeightedFilter2,
	ETypeCount,
};


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	do
	{
		// �˰��� ���� ��� // Algorithmic executation result
		CResult res = EResult_UnknownError;

		// Source �̹��� �ε� // Load the source image
		if(IsFail(res = arrFliImage[EType_Source].Load(L"../../ExampleImages/MedianFilter/Chip_Noise.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create destination1 image as same as source image
		if(IsFail(res = arrFliImage[EType_MedianWeightedFilter1].Assign(arrFliImage[EType_Source])))
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Destination2 �̹����� Source �̹����� ������ �̹����� ���� // Create destination2 image as same as source image
		if(IsFail(res = arrFliImage[EType_MedianWeightedFilter2].Assign(arrFliImage[EType_Source])))
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(res = arrViewImage[EType_Source].Create(100, 0, 612, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination1 �̹��� �� ���� // Create destination1 image view
		if(IsFail(res = arrViewImage[EType_MedianWeightedFilter1].Create(612, 0, 1124, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination2 image view
		if(IsFail(res = arrViewImage[EType_MedianWeightedFilter2].Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if(IsFail(res = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_MedianWeightedFilter1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_MedianWeightedFilter2])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_MedianWeightedFilter1])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_MedianWeightedFilter2])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Utility MedianWeightedFilter ��ü ���� // Create Utility MedianWeightedFilter object
		CMedianWeightedFilter medianWeightedFilter;
		// Source ROI ���� // Set the source ROI
		CFLRect<int32_t> flrROI(100, 190, 360, 420);
		// Source �̹��� ���� // Set the source image
		medianWeightedFilter.SetSourceImage(arrFliImage[EType_Source]);
		// Source ROI ���� // Set the source ROI
		medianWeightedFilter.SetSourceROI(flrROI);
		// Destination �̹��� ���� // Set the destination image
		medianWeightedFilter.SetDestinationImage(arrFliImage[EType_MedianWeightedFilter1]);
		// Destination ROI ����
		medianWeightedFilter.SetDestinationROI(flrROI);
		// Image MedianWeightedFilter ���� ����� Gauss�� ����
		medianWeightedFilter.SetWeightedMethod(CMedianWeightedFilter::EWeightedMethod_Gauss);
		
		// ó���� Filter�� Kernel Size ���� (KernelSize = 11 �� ���, Kernel Size : 11x11, Ȧ���� ��������)
		medianWeightedFilter.SetKernel(11);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = medianWeightedFilter.Execute()))
		{
			ErrorPrint(res, "Failed to execute medianWeightedFilter.");
			
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		medianWeightedFilter.SetDestinationImage(arrFliImage[EType_MedianWeightedFilter2]);
		// Image MedianWeightedFilter ���� ����� Inner�� ����
		medianWeightedFilter.SetWeightedMethod(CMedianWeightedFilter::EWeightedMethod_Inner);
				
		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = medianWeightedFilter.Execute()))
		{
			ErrorPrint(res, "Failed to execute medianWeightedFilter.");
			
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

			// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
			// FLImaging�� Figure��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ����
			if(IsFail(res = arrLayer[i].DrawFigureImage(&flrROI, LIME)))
				ErrorPrint(res, "Failed to draw figure\n");

		}

		// �̹��� �� ���� ǥ�� // Display image view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = arrLayer[EType_Source].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_MedianWeightedFilter1].DrawTextCanvas(&CFLPoint<double>(0, 0), L"MedianWeightedFilter1 Gauss", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_MedianWeightedFilter2].DrawTextCanvas(&CFLPoint<double>(0, 0), L"MedianWeightedFilter2 Inner", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		arrViewImage[EType_Source].Invalidate(true);
		arrViewImage[EType_MedianWeightedFilter1].Invalidate(true);
		arrViewImage[EType_MedianWeightedFilter2].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(arrViewImage[EType_Source].IsAvailable()
			  && arrViewImage[EType_MedianWeightedFilter1].IsAvailable()
			  && arrViewImage[EType_MedianWeightedFilter2].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}