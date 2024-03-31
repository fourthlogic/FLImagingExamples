#include <cstdio>
#include "../CommomHeader/ErrorPrint.h"
#include <FLImaging.h>


enum EType
{
	EType_Source = 0,
	EType_DistanceTransform1,
	EType_DistanceTransform2,
	EType_DistanceTransform3,
	EType_DistanceTransform4,
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
		if(IsFail(res = arrFliImage[EType_Source].Load(L"../../ExampleImages/DistanceTransform/circle.flif")))
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create destination1 image as same as source image
		if(IsFail(res = arrFliImage[EType_DistanceTransform1].Assign(arrFliImage[EType_Source])))
		{
			ErrorPrint(res, L"Failed to assign the image file.\n");
			break;
		}

		// Destination2 �̹����� Source �̹����� ������ �̹����� ���� // Create destination2 image as same as source image
		if(IsFail(res = arrFliImage[EType_DistanceTransform2].Assign(arrFliImage[EType_Source])))
		{
			ErrorPrint(res, L"Failed to assign the image file.\n");
			break;
		}

		// Destination3 �̹����� Source �̹����� ������ �̹����� ���� // Create destination3 image as same as source image
		if(IsFail(res = arrFliImage[EType_DistanceTransform3].Assign(arrFliImage[EType_Source])))
		{
			ErrorPrint(res, L"Failed to assign the image file.\n");
			break;
		}

		// Destination4 �̹����� Source �̹����� ������ �̹����� ���� // Create destination4 image as same as source image
		if(IsFail(res = arrFliImage[EType_DistanceTransform4].Assign(arrFliImage[EType_Source])))
		{
			ErrorPrint(res, L"Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(res = arrViewImage[EType_Source].Create(100, 0, 612, 512)))
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination1 �̹��� �� ���� // Create destination1 image view
		if(IsFail(res = arrViewImage[EType_DistanceTransform1].Create(612, 0, 1124, 512)))
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination2 image view
		if(IsFail(res = arrViewImage[EType_DistanceTransform2].Create(100, 512, 612, 1024)))
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination3 �̹��� �� ���� // Create the destination3 image view
		if(IsFail(res = arrViewImage[EType_DistanceTransform3].Create(612, 512, 1124, 1024)))
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination4 �̹��� �� ���� // Create the destination4 image view
		if(IsFail(res = arrViewImage[EType_DistanceTransform4].Create(1124, 512, 1636, 1024)))
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}



		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if(IsFail(res = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(res, L"Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_DistanceTransform1])))
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_DistanceTransform2])))
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_DistanceTransform3])))
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_DistanceTransform4])))
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_DistanceTransform1])))
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_DistanceTransform2])))
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_DistanceTransform3])))
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_DistanceTransform4])))
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// Utility DistanceTransform ��ü ���� // Create Utility DistanceTransform object
		CDistanceTransform distance;
		// Source �̹��� ���� // Set the source image
		distance.SetSourceImage(arrFliImage[EType_Source]);
		// Destination �̹��� ���� // Set the destination image
		distance.SetDestinationImage(arrFliImage[EType_DistanceTransform1]);
		// Image DistanceTransform ���� ����� Euclid�� ����
		distance.SetMeasurementMethod(CDistanceTransform::EMeasurementMethod_Euclid);
		
		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = distance.Execute()))
		{
			ErrorPrint(res, L"Failed to execute distance.");
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		distance.SetDestinationImage(arrFliImage[EType_DistanceTransform2]);
		// Image DistanceTransform ���� ����� CityBlock�� ����
		distance.SetMeasurementMethod(CDistanceTransform::EMeasurementMethod_CityBlock);
		
		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = distance.Execute()))
		{
			ErrorPrint(res, L"Failed to execute distance.");
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		distance.SetDestinationImage(arrFliImage[EType_DistanceTransform3]);
		// Image DistanceTransform ���� ����� Chessboard�� ����
		distance.SetMeasurementMethod(CDistanceTransform::EMeasurementMethod_Chessboard);
		
		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = distance.Execute()))
		{
			ErrorPrint(res, L"Failed to execute distance.");
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		distance.SetDestinationImage(arrFliImage[EType_DistanceTransform4]);
		// Image DistanceTransform ���� ����� QuasiEuclid�� ����
		distance.SetMeasurementMethod(CDistanceTransform::EMeasurementMethod_QuasiEuclid);
		
		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = distance.Execute()))
		{
			ErrorPrint(res, L"Failed to execute distance.");
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
		if(IsFail(res = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_DistanceTransform1].DrawTextCanvas(&CFLPointD(0, 0), L"DistanceTransform1 Euclid", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_DistanceTransform2].DrawTextCanvas(&CFLPointD(0, 0), L"DistanceTransform2 CityBlock", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_DistanceTransform3].DrawTextCanvas(&CFLPointD(0, 0), L"DistanceTransform3 Chessboard", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_DistanceTransform4].DrawTextCanvas(&CFLPointD(0, 0), L"DistanceTransform4 Qusai-Euclid", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� // Update image view
		arrViewImage[EType_Source].Invalidate(true);
		arrViewImage[EType_DistanceTransform1].Invalidate(true);
		arrViewImage[EType_DistanceTransform2].Invalidate(true);
		arrViewImage[EType_DistanceTransform3].Invalidate(true);
		arrViewImage[EType_DistanceTransform4].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(arrViewImage[EType_Source].IsAvailable()
			  && arrViewImage[EType_DistanceTransform1].IsAvailable()
			  && arrViewImage[EType_DistanceTransform2].IsAvailable()
			  && arrViewImage[EType_DistanceTransform3].IsAvailable()
			  && arrViewImage[EType_DistanceTransform4].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}