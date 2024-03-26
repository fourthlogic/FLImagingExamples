#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


#include <iostream>

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

	// �˰��� ���� ��� // Algorithm execution result
	CResult eResult = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((eResult = arrFliImage[EType_Source].Load(L"../../ExampleImages/WienerDeconvolutionFilter/bird.flif")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the image file.\n");
			break;
		}

		bool bError = false;

		for(int32_t i = EType_Destination1; i < ETypeCount; ++i)
		{
			// Destination �̹����� Source �̹����� ������ �̹����� ���� // Create destination image as same as source image
			if((eResult = arrFliImage[i].Assign(arrFliImage[EType_Source])).IsFail())
			{
				ErrorPrint(eResult, L"Failed to assign the image file.\n");
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
			if((eResult = arrViewImage[i].Create(x * 400 + 400, y * 400, x * 400 + 400 + 400, y * 400 + 400)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to create the image view.\n");
				bError = true;
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
			if((eResult = arrViewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
			
			if(i == EType_Source)
				continue;

			// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
			if((eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[i])).IsFail())
			{
				ErrorPrint(eResult, L"Failed to synchronize view\n");
				bError = true;
				break;
			}

			// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
			if((eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[i])).IsFail())
			{
				ErrorPrint(eResult, L"Failed to synchronize window.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;		

 		// WienerDeconvolutionFilter ��ü ���� // Create WienerDeconvolutionFilter filter object
 		CWienerDeconvolutionFilter WienerDeconvolutionFilter;
 		// Source �̹��� ���� // Set the source image
 		WienerDeconvolutionFilter.SetSourceImage(arrFliImage[EType_Source]); 

 		// Destination1 �̹��� ���� // Set the destination1 image
 		WienerDeconvolutionFilter.SetDestinationImage(arrFliImage[EType_Destination1]);

		// Destination2 �̹��� ���� // Set the destination2 image
		WienerDeconvolutionFilter.SetResultFrequency(arrFliImage[EType_Destination3]);

		// SNR �� ���� // Set the SNR value
		WienerDeconvolutionFilter.SetSNR(0.00001);

		// Length �� ���� // Set the Length value
		WienerDeconvolutionFilter.SetLength(135);

		// Angle �� ���� // Set the Angle value
		WienerDeconvolutionFilter.SetAngle(45);

		// Motion Blur �� ���� // Set the Motion Blur value
		WienerDeconvolutionFilter.SetOperationType(CWienerDeconvolutionFilter::EOperationType_Convolution);
 
 		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
 		if((eResult = WienerDeconvolutionFilter.Execute()).IsFail())
 		{
 			ErrorPrint(eResult, L"Failed to execute algorithm.");
 			break;
 		}

		// Source �̹��� ���� // Set the source image
		WienerDeconvolutionFilter.SetSourceImage(arrFliImage[EType_Destination1]); 

		// Destination2 �̹��� ���� // Set the destination2 image
		WienerDeconvolutionFilter.SetDestinationImage(arrFliImage[EType_Destination2]);

		// Destination4 �̹��� ���� // Set the destination2 image
		WienerDeconvolutionFilter.SetResultFrequency(arrFliImage[EType_Destination4]);

		// WienerDeconvolutionFilter Motion Blur ���� / Set the Motion Blur value
		WienerDeconvolutionFilter.SetOperationType(CWienerDeconvolutionFilter::EOperationType_Deconvolution);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = WienerDeconvolutionFilter.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute algorithm.");
			break;
		}	

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			arrLayer[i].Clear();
		}

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((eResult = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		if((eResult = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPointD(0, 0), L"Destination1 Image (Motion Blur)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		if((eResult = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPointD(0, 0), L"Destination2 Image (Deconvolution)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		if((eResult = arrLayer[EType_Destination3].DrawTextCanvas(&CFLPointD(0, 0), L"Destination3 Image (Blur Frequency)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		if((eResult = arrLayer[EType_Destination4].DrawTextCanvas(&CFLPointD(0, 0), L"Destination4 Image (Deconv Frequency)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
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