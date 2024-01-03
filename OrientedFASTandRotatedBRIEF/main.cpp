#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult eResult;

	do
	{
		// �̹��� �ε� // Load image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/OrientedFASTandRotatedBRIEF/Chip.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImage.Create(400, 0, 1168, 540)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(eResult = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// ORB ��ü ���� // Create ORB object
		COrientedFASTandRotatedBRIEF ORB;

		// ROI ���� ����
		CFLRectL flrROI(100, 50, 450, 450);

		// ó���� �̹��� ����
		if(IsFail(eResult = ORB.SetSourceImage(fliImage)))
		{
			ErrorPrint(eResult, "Failed to set Source Image.");
			break;
		}

		// ó���� ROI ����
		if(IsFail(eResult = ORB.SetSourceROI(flrROI)))
		{
			ErrorPrint(eResult, "Failed to set Source ROI.");
			break;
		}

		// Ư¡���� ������ NLevels ���� ����
		if(IsFail(eResult = ORB.SetNLevels(4)))
		{
			ErrorPrint(eResult, "Failed to set NLevels.");
			break;
		}

		// Ư¡���� ������ Nfeature ���� ����
		if(IsFail(eResult = ORB.SetNfeature(500)))
		{
			ErrorPrint(eResult, "Failed to set Nfeature.");
			break;
		}

		// Ư¡���� ������ ScoreType ����
		if(IsFail(eResult = ORB.SetScoreType(COrientedFASTandRotatedBRIEF::EScoreType_FastScore)))
		{
			ErrorPrint(eResult, "Failed to set Score Type.");
			break;
		}

		// ������ Ư¡���� FAST Threshold ����
		if(IsFail(eResult = ORB.SetFASTThreshold(10)))
		{
			ErrorPrint(eResult, "Failed to set FAST Threshold.");
			break;
		}

		// ORB�� �Ķ���� Scale Factor ����
		if(IsFail(eResult = ORB.SetScaleFactor(1.2f)))
		{
			ErrorPrint(eResult, "Failed to set Scale Factor.");
			break;
		}

		// ORB ���� �Լ�
		if(IsFail(eResult = ORB.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute.");
			break;
		}

		// ���� ����� �޾ƿ��� ���� �����̳�
		Foundation::CFLPointArray flfaResultPoints;

		// Ű ����Ʈ ���� ����
		if(IsFail(eResult = ORB.GetResultPoints(&flfaResultPoints)))
		{
			ErrorPrint(eResult, "Failed to get result.");
			break;
		}

		// ����� ���� ���
		layer.DrawFigureImage(flfaResultPoints, RED, 1);

		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
		// FLImaging�� Figure��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ����
		if(IsFail(eResult = layer.DrawFigureImage(&flrROI, BLUE)))
		{
			ErrorPrint(eResult, "Failed to draw figures objects on the image view.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}