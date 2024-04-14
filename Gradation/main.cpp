#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliISrcImage;
	CFLImage fliIDstImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage[2];

	do
	{
		// ���� ��� // operation result
		CResult res = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliISrcImage.Load(L"../../ExampleImages/Gradation/House.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage[0].Create(300, 0, 300 + 384, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(300 + 384, 0, 300 + 384 * 2, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(res = viewImage[0].SetImagePtr(&fliISrcImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].SetImagePtr(&fliIDstImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views. 
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		// Gradation ��ü ���� // Create Gradation object
		CGradation Gradation;

		// Source �̹��� ���� // Set source image 
		Gradation.SetSourceImage(fliISrcImage);

		// Destination �̹��� ���� // Set destination image
		Gradation.SetDestinationImage(fliIDstImage);

		// ���� Alpha �� ���� // Set start alpha value
		CMultiVar<double> mvStartAlpha(0., 0., 0.);
		Gradation.SetStartAlpha(mvStartAlpha);

		// �� Alpha �� ���� // Set end alpha value
		CMultiVar<double> mvEndAlpha(0.1, 0.6, 0.9);
		Gradation.SetEndAlpha(mvEndAlpha);

		// Gradation Start Value ����(3Ch) // Set Gradation Start Value(3Ch)
		CMultiVar<double> mvStartValue(255, 0, 0);
		Gradation.SetStartValue(mvStartValue);

		// Gradation End Value ����(3Ch) // Set Gradation End Value(3Ch)
		CMultiVar<double> mvEndValue(0, 0, 255);
		Gradation.SetEndValue(mvEndValue);

		// Gradation Vector Figure ��ü // Gradation Vector Figure object
		CFLLine<double> fllVector;
		fllVector.Load(L"../../ExampleImages/Gradation/Vector.fig");

		Gradation.SetVector(fllVector);

		// �˰��� ���� // Execute the algorithm
		if((res = Gradation.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Gradation.");
			break;
		}

		// ���̾�� ���� �������� �ʾƵ� View�� ���� �� �� ���� �����ȴ�. // The layer is released together when View is released without releasing it separately.
		CGUIViewImageLayerWrap layer1 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layer2 = viewImage[1].GetLayer(0);

		// Draw Figure ��ü // Gradation Vector Figure object
		CFLFigureArray flfaDrawVector;
		flfaDrawVector.Load(L"../../ExampleImages/Gradation/DrawVector.fig");

		CFLRect<double> fllRect1(fllVector.flpPoints[0] - 15, fllVector.flpPoints[0] + 15);
		CFLRect<double> fllRect2(fllVector.flpPoints[1] - 15, fllVector.flpPoints[1] + 15);

		if(IsFail(res = layer1.DrawFigureImage(&fllRect1, BLUE, 5, BLUE, EGUIViewImagePenStyle_Solid)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(res = layer1.DrawFigureImage(&fllRect2, RED, 5, RED, EGUIViewImagePenStyle_Solid)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// Gradation Vector ��� // Draw gradation vector
		if(IsFail(res = layer1.DrawFigureImage(&flfaDrawVector, BLACK, 5)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(res = layer1.DrawFigureImage(&flfaDrawVector, LIME, 3)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// text�� ����մϴ�. // Display text.
		if(IsFail(res = layer1.DrawTextImage(&fllVector.flpPoints[0], L"Start Value(255, 0, 0)/Start Alpha(0, 0, 0)", YELLOW, BLACK, 15, false, EGUIViewImageTextAlignment_RIGHT)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layer1.DrawTextImage(&fllVector.flpPoints[1], L"End(0, 0, 255)/Start Alpha(0.1, 0.6, 0.9)", YELLOW, BLACK, 15)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// View ������ ���÷��� �մϴ�. // Display View information.
		if(IsFail(res = layer1.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layer2.DrawTextCanvas(&CFLPointD(0, 0), L"Gradation Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}