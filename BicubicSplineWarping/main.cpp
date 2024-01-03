#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[4];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[4];

	CResult eResult = EResult_UnknownError;

	do
	{
		int32_t arrLeft[4] = {400, 912, 400, 912};
		int32_t arrTop[4] = {0, 0, 500, 500};
		int32_t arrRight[4] = {912, 1424, 912, 1424};
		int32_t arrBottom[4] = {500, 500, 1000, 1000};

		// Source �̹��� �ε� // Load the source image
		if((eResult =arrFliImage[0].Load(L"../../ExampleImages/BicubicSplineWarping/chess.flif")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the image file.\n");
			break;
		}

		int32_t i = 0;
		
		for(; i < 4; ++i)
		{
			// �̹��� �� ���� // Create image view
			if((eResult =arrViewImage[i].Create(arrLeft[i], arrTop[i], arrRight[i], arrBottom[i])).IsFail())
			{
				ErrorPrint(eResult, L"Failed to create the image view.\n");
				break;
			} 
			//�̹��� �信 �̹����� ���÷��� // Display the image in the image view
			if((eResult =arrViewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(eResult, L"Failed to create the image view.\n");
				break;
			}
		}

		if (i < 4)
			break;

		i = 0;

		for(; i < 4; i += 2)
		{
		    // �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
			if((eResult =arrViewImage[i].SynchronizePointOfView(&arrViewImage[i + 1])).IsFail())
			{
				ErrorPrint(eResult, L"Failed to synchronize view\n");
				break;
			}

			// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
			if((eResult =arrViewImage[i].SynchronizeWindow(&arrViewImage[i + 1])).IsFail())
			{
				ErrorPrint(eResult, L"Failed to synchronize window.\n");
				break;
			}
		}

		if(i < 4)
			break;
	
		// BicubicSplineWarping ��ü ���� // Create BicubicSplineWarping object
		CBicubicSplineWarping BicubicSplineWarping;

		// Source �̹��� ���� // Set the source image
		BicubicSplineWarping.SetSourceImage(arrFliImage[0]);
		// Destination �̹��� ���� // Set the destination image
		BicubicSplineWarping.SetDestinationImage(arrFliImage[1]);
		// Interpolation Method ���� // Set the interpolation method
		BicubicSplineWarping.SetInterpolationMethod(EInterpolationMethod_Bilinear);
		// Extension ����
		BicubicSplineWarping.SetExtension(2);
		// �׸��带 (5,5)�� �ʱ�ȭ // Initialize the grid to (5,5)
		CFLPointL flpGridSize(5, 5);

		CFLPointL flpGridIndex;
		CFLPointArray flpaSourcePoints;
		CFLPointArray flpaTargetPoints;

		double f64ScaleX = arrFliImage[0].GetWidth() / 4.;
		double f64ScaleY = arrFliImage[0].GetHeight() / 4.;

		for(int y = 0; y < flpGridSize.y; ++y)
		{
			flpGridIndex.y = y;

			for(int x = 0; x < flpGridSize.x; ++x)
			{
				flpGridIndex.x = x;

				// Grid Index�� ���� ��ǥ�� Source ��ǥ�� ���� // Set Source coordinates to the same coordinates as Grid Index
				CFLPointD flpSource(flpGridIndex.x * f64ScaleX, flpGridIndex.y * f64ScaleY);


				double f64RandomX = CRandomGenerator::Double(-0.2, 0.2);
				double f64RandomY = CRandomGenerator::Double(-0.2, 0.2);

				// �ܰ��� ��ǥ�� �������� ���� �ǵ��� ���� // Set the outer coordinates to be transformed inward
				if(y == 0)
					f64RandomY = f64RandomY < 0 ? -f64RandomY : f64RandomY;

				if(x == 0)
					f64RandomX = f64RandomX < 0 ? -f64RandomX : f64RandomX;

				if(y == flpGridSize.y - 1)
					f64RandomY = f64RandomY > 0 ? -f64RandomY : f64RandomY;

				if(x == flpGridSize.x - 1)
					f64RandomX = f64RandomX > 0 ? -f64RandomX : f64RandomX;

	           // Grid Index�� ���� ��ǥ���� �̼��� ���� ���� �ο��ؼ� ��ǥ�� �ְ� // Distort coordinates by giving fine random values at the same coordinates as Grid Index 
				CFLPoint<double> flpDistortion = new CFLPoint<double>((flpGridIndex.x + f64RandomX) * f64ScaleX, (flpGridIndex.y + f64RandomY) * f64ScaleY);

				flpaSourcePoints.PushBack(flpSource);
				flpaTargetPoints.PushBack(flpDistortion);
			}
		}

		BicubicSplineWarping.SetCalibrationPointArray(flpaSourcePoints, flpaTargetPoints);

		// �ռ� ������ Source Image, Calibration Point Array�� ������� Calibrate ���� // Calibrate based on the previously set Source Image and Calibration Point Array
		if((eResult =BicubicSplineWarping.Calibrate()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute BicubicSplineWarping.");
			break;
		}

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult =BicubicSplineWarping.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute BicubicSplineWarping.");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((eResult =arrViewImage[1].ZoomFit()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to zoom fit of the image view.\n");
			break;
		}

		//��ȯ ����� ������� ��ȯ�ϴ� ���� ���� // Perform the process of converting the conversion result back to its original state
		arrFliImage[2].Assign(arrFliImage[1]);
		// ��Ʈ�� ����Ʈ ���� // Set control points
		BicubicSplineWarping.SetCalibrationPointArray(flpaTargetPoints, flpaSourcePoints);
		// Source �̹��� ���� // Set the source image
		BicubicSplineWarping.SetSourceImage(arrFliImage[2]);
		// Destination �̹��� ���� // Set the destination image
		BicubicSplineWarping.SetDestinationImage(arrFliImage[3]);
		// Interpolation Method ���� // Set the interpolation method
		BicubicSplineWarping.SetInterpolationMethod(EInterpolationMethod_Bilinear);
		// Extension ����
		BicubicSplineWarping.SetExtension(2);

		// �ռ� ������ Source Image, Calibration Point Array�� ������� Calibrate ����
		if((eResult =BicubicSplineWarping.Calibrate()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute BicubicSplineWarping.");
			break;
		}

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult =BicubicSplineWarping.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute BicubicSplineWarping.");
			break;
		}

		// BicubicSplineTrnasform Ŭ������ ������ Vertex ������ ȭ�鿡 Display // Display the vertex information set in the BicubicSplineTrnasform class on the screen

		CGUIViewImageLayerWrap layer;
		CGUIViewImageLayerWrap layer2;

		layer = arrViewImage[0].GetLayer(0);
		layer2 = arrViewImage[2].GetLayer(0);

		for(int64_t i = 0; i < flpaSourcePoints.GetCount(); ++i)
		{
			CFLPointD flpSource = flpaSourcePoints.GetAt(i);
			CFLPointD flpTarget = flpaTargetPoints.GetAt(i);

			CFLLine<double> fllDrawLine(flpSource, flpTarget);
			CFLFigureArray flfaArrow = fllDrawLine.MakeArrowWithRatio(0.25, true, 20);

			// Source Vertex�� ��/���� View Layer�� Drawing // Drawing the source vertex on the left/right view layer
			if((eResult =layer.DrawFigureImage(&flpSource, RED, 1)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to draw figure objects on the image view.\n");
				break;
			}

			if((eResult =layer.DrawFigureImage(&flpTarget, BLUE, 1)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to draw figure objects on the image view.\n");
				break;
			}

			if((eResult =layer.DrawFigureImage(&flfaArrow, YELLOW, 1)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to draw figure objects on the image view.\n");
				break;
			}

			fllDrawLine = CFLLine<double>(flpTarget, flpSource);
			flfaArrow = fllDrawLine.MakeArrowWithRatio(0.25, true, 20);

			// Source Vertex�� ��/���� View Layer�� Drawing // Drawing the source vertex on the left/right view layer
			if((eResult =layer2.DrawFigureImage(&flpSource, RED, 1)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to draw figure objects on the image view.\n");
				break;
			}

			if((eResult =layer2.DrawFigureImage(&flpTarget, BLUE, 1)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to draw figure objects on the image view.\n");
				break;
			}

			if((eResult =layer2.DrawFigureImage(&flfaArrow, YELLOW, 1)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to draw figure objects on the image view.\n");
				break;
			}
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = arrViewImage[0].GetLayer(1);
		CGUIViewImageLayerWrap layerDst = arrViewImage[1].GetLayer(1);

		CGUIViewImageLayerWrap layerSrc2 = arrViewImage[2].GetLayer(1);
		CGUIViewImageLayerWrap layerDst2 = arrViewImage[3].GetLayer(1);

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
		if((eResult =layerSrc.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		if((eResult =layerDst.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		if((eResult =layerSrc2.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image2", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		if((eResult =layerDst2.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image2", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		for(int32_t i = 0; i < 4; ++i)
			arrViewImage[i].Invalidate(true);
	
		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(arrViewImage[0].IsAvailable() && arrViewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}