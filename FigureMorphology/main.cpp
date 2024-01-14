#include <cstdio>
#include "../CommomHeader/ErrorPrint.h"
#include <FLImaging.h>


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

	do
	{
		// �˰��� ���� ��� // Algorithmic executation result
		CResult eResult = EResult_UnknownError;

		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(eResult = arrViewImage[EType_Source].Create(100, 0, 356, 256)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination image view
		if(IsFail(eResult = arrViewImage[EType_Destination1].Create(100, 256, 356, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination image view
		if(IsFail(eResult = arrViewImage[EType_Destination2].Create(356, 256, 612, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination image view
		if(IsFail(eResult = arrViewImage[EType_Destination3].Create(100, 512, 356, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination image view
		if(IsFail(eResult = arrViewImage[EType_Destination4].Create(356, 512, 612, 768)))
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

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination3])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination4])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination2])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination3])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination4])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Utility FigureMorphology ��ü ���� // Create Utility FigureMorphology object
		CFigureMorphology figureMorphology;
		// FigureMorphology Source ROI Setting
		CFLFigureArray flfSourceFigure;
		flfSourceFigure.PushBack(CFLRect<int32_t>(125, 100, 225, 150, 0.000000));
		flfSourceFigure.PushBack(CFLRect<int32_t>(15, 100, 115, 150, 0.000000));
		figureMorphology.SetSourceFigure(&flfSourceFigure);
		// FigureMorphology FigureMorphologyMethod Setting
		figureMorphology.SetFigureMorphologyMethod(CFigureMorphology::EFigureMorphologyMethod_Erode);
		figureMorphology.SetKernel(5,5);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = figureMorphology.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute figureMorphology.");
			
			break;
		}

		CFLFigureArray flfResultFigureErode;
		figureMorphology.GetResultFigure(flfResultFigureErode);

		// FigureMorphology FigureMorphologyMethod Setting
		figureMorphology.SetFigureMorphologyMethod(CFigureMorphology::EFigureMorphologyMethod_Dilate);
		figureMorphology.SetKernel(5, 5);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = figureMorphology.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute figureMorphology.");

			break;
		}

		CFLFigureArray flfResultFigureDilate;
		figureMorphology.GetResultFigure(flfResultFigureDilate);

		// FigureMorphology FigureMorphologyMethod Setting
		figureMorphology.SetFigureMorphologyMethod(CFigureMorphology::EFigureMorphologyMethod_Open);
		figureMorphology.SetKernel(5, 5);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = figureMorphology.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute figureMorphology.");

			break;
		}

		CFLFigureArray flfResultFigureOpen;
		figureMorphology.GetResultFigure(flfResultFigureOpen);

		// FigureMorphology FigureMorphologyMethod Setting
		figureMorphology.SetFigureMorphologyMethod(CFigureMorphology::EFigureMorphologyMethod_Close);
		figureMorphology.SetKernel(5, 5);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = figureMorphology.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute figureMorphology.");

			break;
		}

		CFLFigureArray flfResultFigureClose;
		figureMorphology.GetResultFigure(flfResultFigureClose);

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


		if(IsFail(arrLayer[EType_Source].DrawFigureImage(&flfSourceFigure, RED)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination1].DrawFigureImage(&flfSourceFigure, RED)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination2].DrawFigureImage(&flfSourceFigure, RED)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination3].DrawFigureImage(&flfSourceFigure, RED)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination4].DrawFigureImage(&flfSourceFigure, RED)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination1].DrawFigureImage(&flfResultFigureErode, LIME)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination2].DrawFigureImage(&flfResultFigureDilate, LIME)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination3].DrawFigureImage(&flfResultFigureOpen, LIME)))
			printf("Failed to draw figure\n");

		if(IsFail(arrLayer[EType_Destination4].DrawFigureImage(&flfResultFigureClose, LIME)))
			printf("Failed to draw figure\n");

		if(IsFail(eResult = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Figure", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPointD(0, 0), L"Erode Method", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPointD(0, 0), L"Dilate Method", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination3].DrawTextCanvas(&CFLPointD(0, 0), L"Close Method", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination4].DrawTextCanvas(&CFLPointD(0, 0), L"Open Method", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}


		// �̹��� �並 ���� // Update image view
		arrViewImage[EType_Source].Invalidate(true);
		arrViewImage[EType_Destination1].Invalidate(true);
		arrViewImage[EType_Destination2].Invalidate(true);
		arrViewImage[EType_Destination3].Invalidate(true);
		arrViewImage[EType_Destination4].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(arrViewImage[EType_Source].IsAvailable()
			  && arrViewImage[EType_Destination1].IsAvailable() 
			  && arrViewImage[EType_Destination2].IsAvailable() 
			  && arrViewImage[EType_Destination3].IsAvailable()
			  && arrViewImage[EType_Destination4].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}