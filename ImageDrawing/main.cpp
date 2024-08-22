#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� ����� ��ü ���� // Declare image drawing object
	CFLImageDrawingWrap fliImageDrawing;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	do
	{
		CResult res = EResult_UnknownError;

		// �̹��� �ε� // Load image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Blob/AlignBall.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Drawing �̹����� Src �̹����� ������ �̹����� ����
		if(IsFail(res = fliImageDrawing.Assign(fliImage)))
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImageSrc.Create(400, 0, 800, 400)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImageDst.Create(800, 0, 1200, 400)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ�. // Synchronize the viewpoints of the two image views.
		if(IsFail(res = viewImageSrc.SynchronizePointOfView(&viewImageDst)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageSrc.SynchronizeWindow(&viewImageDst)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(res = viewImageSrc.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// ���̾�� ���� �������� �ʾƵ� View�� ���� �� �� ���� �����ȴ�. // The layer is released together when View is released without releasing it separately.
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);

		// �̹����� ����ϱ� ���� �̹��� ����� ��ä���� ���̾ ���� // Gets layers from image drawing object for output to image
		CGUIViewImageLayerWrap layerDst = fliImageDrawing.GetLayer();

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerDst.Clear();


		// �̹����� ���� ǥ�� // Display image information
		layerSrc.DrawTextImage(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30);
		layerDst.DrawTextImage(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 30);

		CFLPoint<double> flpDraw(10., 10.);

		layerSrc.DrawFigureImage(flpDraw, RED, 3);
		layerDst.DrawFigureImage(flpDraw, RED, 3);

		CFLLine<double> fllDraw(15., 15., 80., 30.);

		layerSrc.DrawFigureImage(fllDraw, ORANGE, 3);
		layerDst.DrawFigureImage(fllDraw, ORANGE, 3);

		CFLRect<double> flrDraw(80., 80., 150., 150.);

		layerSrc.DrawFigureImage(flrDraw, YELLOW, 3);
		layerDst.DrawFigureImage(flrDraw, YELLOW, 3);

		CFLQuad<double> flqDraw(170., 170., 200., 180., 220., 210., 180., 230.);

		layerSrc.DrawFigureImage(flqDraw, GREEN, 3);
		layerDst.DrawFigureImage(flqDraw, GREEN, 3);

		CFLCircle<double> flcDraw(250., 250., 50.);

		layerSrc.DrawFigureImage(flcDraw, BLUE, 3);
		layerDst.DrawFigureImage(flcDraw, BLUE, 3);

		CFLEllipse<double> fleDraw(350., 350., 50., 80., 25.);

		layerSrc.DrawFigureImage(fleDraw, VIOLET, 3);
		layerDst.DrawFigureImage(fleDraw, VIOLET, 3);


		// �̹����� �׸��ϴ�. // Draw in the image.
		if(IsFail(res = fliImageDrawing.Draw()))
		{
			ErrorPrint(res, "Failed to draw.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(res = viewImageDst.SetImagePtr(&fliImageDrawing)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate();
		viewImageDst.Invalidate();

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSrc.IsAvailable() || viewImageDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}