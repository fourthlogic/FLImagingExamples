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
		if(IsFail(res = fliISrcImage.Load(L"../../ExampleImages/RadialGradation/Moon.flif")))
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

		// Load Radial Gradation Region ��ü // Load Radial Gradation Region Figure object
		CFLCircle<double> flcRadialRegion;
		flcRadialRegion.Load(L"../../ExampleImages/RadialGradation/RadialRegion.fig");

		// Radial Gradation ��ü ���� // Create Radial Gradation object
		CRadialGradation RadialGradation;

		// Source �̹��� ���� // Set source image 
		RadialGradation.SetSourceImage(fliISrcImage);

		// Destination �̹��� ���� // Set destination image
		RadialGradation.SetDestinationImage(fliIDstImage);

		// Source ROI ���� // Set source ROI 
		RadialGradation.SetSourceROI(flcRadialRegion);

		// ���� Alpha �� ���� // Set start alpha value
		CMultiVar<double> mvStartAlpha(1., 0.3, 0.3);
		RadialGradation.SetStartAlpha(mvStartAlpha);

		// �� Alpha �� ���� // Set end alpha value
		CMultiVar<double> mvEndAlpha(0.1, 0.5, 0.5);
		RadialGradation.SetEndAlpha(mvEndAlpha);

		// RadialGradation Start Value ����(3Ch) // Set RadialGradation Start Value(3Ch)
		CMultiVar<double> mvStartValue(0, 0, 0);
		RadialGradation.SetStartValue(mvStartValue);

		// RadialGradation End Value ����(3Ch) // Set RadialGradation End Value(3Ch)
		CMultiVar<double> mvEndValue(100, 255, 255);
		RadialGradation.SetEndValue(mvEndValue);

		// RadialGradation Region ���� // Set RadialGradation Region 
		RadialGradation.SetRadialRegion(flcRadialRegion);

		// �˰��� ���� // Execute the algorithm
		if((res = RadialGradation.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute RadialGradation.");
			break;
		}

		// ���̾�� ���� �������� �ʾƵ� View�� ���� �� �� ���� �����ȴ�. // The layer is released together when View is released without releasing it separately.
		CGUIViewImageLayerWrap layer1 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layer2 = viewImage[1].GetLayer(0);

		// Draw Figure ��ü // RadialGradation Vector Figure object
		CFLFigureArray flfaDrawArrow;
		CFLLine<double> fllArrow;
		CFLPoint<double> flpCenter;

		flpCenter = flcRadialRegion.GetCenter();
		flpCenter.y += flcRadialRegion.radius - 10;
		fllArrow.flpPoints[0] = flcRadialRegion.GetCenter();
		fllArrow.flpPoints[1] = flcRadialRegion.GetCenter();
		fllArrow.flpPoints[1].y += flcRadialRegion.radius;
		flfaDrawArrow = fllArrow.MakeArrowWithLength(5);

		if(IsFail(res = layer1.DrawFigureImage(flfaDrawArrow, RED, 5, RED, EGUIViewImagePenStyle_Solid)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// text�� ����մϴ�. // Display text.
		if(IsFail(res = layer1.DrawTextImage(flcRadialRegion.GetCenter(), L"Start Value(255, 0, 0)\nStart Alpha(1.0, 0.3, 0.3)", YELLOW, BLACK, 13, false, EGUIViewImageTextAlignment_RIGHT)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layer1.DrawTextImage(flpCenter, L"End Value(100, 255, 255)\nEnd Alpha(0.1, 0.5, 0.5)", YELLOW, BLACK, 13)))
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

		if(IsFail(res = layer2.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
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