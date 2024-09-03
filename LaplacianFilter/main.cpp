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
	ETypeCount,
};

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult res;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if(IsFail(res = arrFliImage[EType_Source].Load(L"../../ExampleImages/Convolution/Moss.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		bool bError = false;

		for(int32_t i = EType_Destination1; i < ETypeCount; ++i)
		{
			// Destination �̹����� Source �̹����� ������ �̹����� ���� // Create destination image as same as source image
			if(IsFail(res = arrFliImage[i].Assign(arrFliImage[EType_Source])))
			{
				ErrorPrint(res, "Failed to assign the image file.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			int32_t x = i % 3;
			int32_t y = i / 3;

			// �̹��� �� ���� // Create image view
			if(IsFail(res = arrViewImage[i].Create(x * 400 + 400, y * 400, x * 400 + 400 + 400, y * 400 + 400)))
			{
				ErrorPrint(res, "Failed to create the image view.\n");
				bError = true;
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
			if(IsFail(res = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
			
			if(i == EType_Source)
				continue;

			// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
			if(IsFail(res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[i])))
			{
				ErrorPrint(res, "Failed to synchronize view\n");
				bError = true;
				break;
			}

			// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
			if(IsFail(res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[i])))
			{
				ErrorPrint(res, "Failed to synchronize window.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;		

		// ROI ������ ���� CFLRect ��ü ���� // Create a CFLRect object for setting ROI
		CFLRect<int32_t> flrROI(200, 200, 500, 500);

 		// Convolution Laplacian ��ü ���� // Create Convolution Laplacian object
 		CConvolutionLaplacian convolutionLaplacian;
 		// Source �̹��� ���� // Set the source image
 		convolutionLaplacian.SetSourceImage(arrFliImage[EType_Source]); 
 		// Source ROI ���� // Set the source ROI
 		convolutionLaplacian.SetSourceROI(flrROI);

 		// Destination1 �̹��� ���� // Set the destination1 image
 		convolutionLaplacian.SetDestinationImage(arrFliImage[EType_Destination1]);
 		// Destination1 ROI ���� // Set the destination1 ROI
 		convolutionLaplacian.SetDestinationROI(flrROI);

		// Ŀ�� ���� ��� ���� // Set kernel operation method
		convolutionLaplacian.SetKernelMethod(CConvolutionLaplacian::EKernel_LaplacianX);
 
 		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
 		if(IsFail(res = convolutionLaplacian.Execute()))
 		{
 			ErrorPrint(res, "Failed to execute convolution Laplacian.");
 			break;
 		}

		// Destination2 �̹��� ���� // Set the destination2 image
		convolutionLaplacian.SetDestinationImage(arrFliImage[EType_Destination2]);
		// Destination2 ROI ���� // Set the destination2 ROI
		convolutionLaplacian.SetDestinationROI(flrROI);

		// Ŀ�� ���� ��� ���� // Set kernel operation method
		convolutionLaplacian.SetKernelMethod(CConvolutionLaplacian::EKernel_LaplacianY);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = convolutionLaplacian.Execute()))
		{
			ErrorPrint(res, "Failed to execute convolution Laplacian.");
			break;
		}

		// Destination3 �̹��� ���� // Set the destination3 image
		convolutionLaplacian.SetDestinationImage(arrFliImage[EType_Destination3]);
		// Destination3 ROI ���� // Set Destination3 ROI
		convolutionLaplacian.SetDestinationROI(flrROI);

		// Ŀ�� ���� ��� ���� // Set kernel operation method
		convolutionLaplacian.SetKernelMethod(CConvolutionLaplacian::EKernel_Laplacian4);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = convolutionLaplacian.Execute()))
		{
			ErrorPrint(res, "Failed to execute convolution Laplacian.");
			break;
		}

		// Destination4 �̹��� ���� // Set the destination4 image
		convolutionLaplacian.SetDestinationImage(arrFliImage[EType_Destination4]);
		// Destination4 ROI ���� // Set Destination4 ROI
		convolutionLaplacian.SetDestinationROI(flrROI);

		// Ŀ�� ���� ��� ���� // Set kernel operation method
		convolutionLaplacian.SetKernelMethod(CConvolutionLaplacian::EKernel_Laplacian8);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = convolutionLaplacian.Execute()))
		{
			ErrorPrint(res, "Failed to execute convolution Laplacian.");
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			arrLayer[i].Clear();

			// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
			// FLImaging�� Figure ��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ���� // FLimaging's Figure objects can be displayed as a function regardless of the shape
			// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
			// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
			// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
			if(IsFail(res = arrLayer[i].DrawFigureImage(&flrROI, LIME)))
				ErrorPrint(res, "Failed to draw figure\n");
		}

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = arrLayer[EType_Source].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination1 Image (LaplacianX)", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination2 Image (LaplacianY)", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_Destination3].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination3 Image (Laplacian4)", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_Destination4].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination4 Image (Laplacian8)", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		for(int32_t i = 0; i < ETypeCount; ++i)
			arrViewImage[i].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		bool bAvailable = true;
		while(bAvailable)
		{
			for(int32_t i = 0; i < ETypeCount; ++i)
			{
				bAvailable = arrViewImage[i].IsAvailable();
				if(!bAvailable)
					break;
			}

			CThreadUtilities::Sleep(1);
		}
	}
	while(false);

	return 0;
}