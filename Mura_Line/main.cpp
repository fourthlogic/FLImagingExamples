#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImageSrc;
	CFLImage fliImageDst;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	do
	{
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Load image
		if(IsFail(eResult = fliImageSrc.Load(L"../../ExampleImages/Mura/Line.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImageSrc.Create(100, 0, 548, 448)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(eResult = viewImageSrc.SetImagePtr(&fliImageSrc)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImageDst.Create(548, 0, 996, 448)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(eResult = viewImageDst.SetImagePtr(&fliImageDst)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views.
		if(IsFail(eResult = viewImageSrc.SynchronizePointOfView(&viewImageDst)))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImageSrc.SynchronizeWindow(&viewImageDst)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Mura ��ü ���� // Create Mura object
		CMura Mura;

		// ó���� �̹��� ����
		Mura.SetSourceImage(fliImageSrc);
		// Auto Threshold ��� ���� // Set auto threshold mode
		Mura.EnableAutoThresholdMode(false);
		// Kernel Size Rate ���� // Set kernel size rate
		Mura.SetKernelSizeRate(0.25);
		// Mura Color Type ���� // Set mura color type
		Mura.SetMuraColorType(CMura::EMuraColorType_BlackOnWhite);

		// �� ���� ����
		Mura.SetLogicalCondition(ELogicalCondition_GreaterEqual);

		// �Ӱ谪 ����,  ���� ���ǰ� �Ʒ��� ������ �������� 100���� ���ų� ū ��ü�� ����
		Mura.SetThreshold(0.8);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = Mura.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute Mura.");
			break;
		}

		// ��� ��ü�� �� �ش�Ǵ� ������ ���� ��ü�� ����

		// 50���� ���ų� ū ¡�� ���̸� ���� ��ü���� ����
		if(IsFail(eResult = Mura.Filter(CBlob::EFilterItem_MinimumEnclosingRectangleLongSideLength, 50, ELogicalCondition_LessEqual)))
		{
			ErrorPrint(eResult, "Blob filtering algorithm error occurs.");
			break;
		}

		Mura.GetResultMuraImage(fliImageDst);

		// Mura ����� ������ ���� FigureArray ����
		CFLFigureArray flfaContours;

		// Mura ����� �� Contour�� ����
		if(IsFail(eResult = Mura.GetResultContours(flfaContours)))
		{
			ErrorPrint(eResult, "Failed to get boundary rects from the Mura object.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImageDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// flfaContours �� Figure���� �迭�̱� ������ Layer�� �ֱ⸸ �ص� ��� ������� �����ϴ�.
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// ���⼭ 0.25�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(eResult = layer.DrawFigureImage(&flfaContours, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(eResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSrc.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}