#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSrcImage;
	CFLImage fliDstImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	// �˰��� ���� ��� // Algorithm execution result
	CResult eResult = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((eResult = fliSrcImage.Load(L"../../ExampleImages/ThinPlateSplineWarping/Undistortion.flif")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create the source image view
		if((eResult = viewImageSrc.Create(400, 0, 912, 612)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((eResult = viewImageSrc.SetImagePtr(&fliSrcImage)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the destination image view
		if((eResult = viewImageDst.Create(912, 0, 1424, 612)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((eResult = viewImageDst.SetImagePtr(&fliDstImage)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((eResult = viewImageSrc.SynchronizePointOfView(&viewImageDst)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((eResult = viewImageSrc.SynchronizeWindow(&viewImageDst)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to synchronize window.\n");
			break;
		}

		// ThinPlateSplineWarping ��ü ���� // Create ThinPlateSplineWarping object
		CThinPlateSplineWarping ThinPlateSplineWarping;

		// Source �̹��� ���� // Set the source image
		ThinPlateSplineWarping.SetSourceImage(fliSrcImage);
		// Destination �̹��� ���� // Set the destination image
		ThinPlateSplineWarping.SetDestinationImage(fliDstImage);
		// Interpolation Method ���� // Set the interpolation method
		ThinPlateSplineWarping.SetInterpolationMethod(EInterpolationMethod_Bilinear);

		// �׸��带 (5,5)�� �ʱ�ȭ // Initialize grid to (5, 5)
		CFLPointL flpGridSize(5, 5);

		CFLPointL flpGridIndex;
		CFLPointArray flpaSourcePoints;
		CFLPointArray flpaTargetPoints;

		double f64ScaleX = fliSrcImage.GetWidth() / 4.;
		double f64ScaleY = fliSrcImage.GetHeight() / 4.;

		for(int y = 0; y < flpGridSize.y; ++y)
		{
			flpGridIndex.y = y;

			for(int x = 0; x < flpGridSize.x; ++x)
			{
				flpGridIndex.x = x;

				// Grid Index�� ���� ��ǥ�� Source ��ǥ�� ���� // Set source vertex same as the grid index
				CFLPointD flpSource(flpGridIndex.x * f64ScaleX, flpGridIndex.y * f64ScaleY);
				// Grid Index�� ���� ��ǥ���� �̼��� ���� ���� �ο��ؼ� �ְ�� Target ��ǥ ���� // Set distorted target coordinates by giving fine random values in coordinates such as Grid Index
				CFLPointD flpDistortion((flpGridIndex.x + CRandomGenerator::Double(-.2, .2)) * f64ScaleX, (flpGridIndex.y + CRandomGenerator::Double(-.2, .2)) * f64ScaleY);

				// ������ ������ ��ǥ���� �������� Calibration Point Array�� Point �߰� // Add points to the calibration point array based on the coordinates set above
				flpaSourcePoints.PushBack(flpSource);
				flpaTargetPoints.PushBack(flpDistortion);
			}
		}

		ThinPlateSplineWarping.SetCalibrationPointArray(flpaSourcePoints, flpaTargetPoints);

		// Calibration Point Array�� ȭ�鿡 Display // Display calibration point array

		CGUIViewImageLayerWrap layer;

		layer = viewImageSrc.GetLayer(0);

		for(int64_t i = 0; i < flpaSourcePoints.GetCount(); ++i)
		{
			CFLPointD flpSource = flpaSourcePoints.GetAt(i);
			CFLPointD flpTarget = flpaTargetPoints.GetAt(i);

			// Source Vertex�� Source �̹��� �� Layer�� �׸��� // Draw the source vertex on the source image view layer
			CFLLine<double> fllDrawLine(flpSource, flpTarget);
			// ������ ȭ��ǥ�� ���� // Change a line to an arrow
			CFLFigureArray flfaArrow = fllDrawLine.MakeArrowWithRatio(0.25, true, 20);

			if((eResult = layer.DrawFigureImage(&flpSource, RED, 1)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to draw figure objects on the image view.\n");
				break;
			}

			if((eResult = layer.DrawFigureImage(&flpTarget, BLUE, 1)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to draw figure objects on the image view.\n");
				break;
			}

			if((eResult = layer.DrawFigureImage(&flfaArrow, YELLOW, 1)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to draw figure objects on the image view.\n");
				break;
			}
		}

		// �ռ� ������ Source Image, Calibration Point Array�� ������� Calibrate ���� // Calibrate based on previously set Source Image, Calibration Point Array
		if((eResult = ThinPlateSplineWarping.Calibrate()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute ThinPlateSplineWarping.");
			break;
		}

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = ThinPlateSplineWarping.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute ThinPlateSplineWarping.");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((eResult = viewImageDst.ZoomFit()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to zoom fit of the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(1);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(1);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerDst.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((eResult = layerSrc.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		if((eResult = layerDst.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}