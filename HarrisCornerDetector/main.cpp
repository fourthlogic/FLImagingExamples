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
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/HarrisCornerDetector/Chip.flif")))
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

		// harris ��ü ���� // Create harris object
		CHarrisCornerDetector harris;

		// ROI ���� ����
		CFLRectL flrROI(100, 50, 450, 450);

		// ó���� �̹��� ����
		if(IsFail(eResult = harris.SetSourceImage(fliImage)))
		{
			ErrorPrint(eResult, "Failed to set Source Image.");
			break;
		}

		// ó���� ROI ����
		if(IsFail(eResult = harris.SetSourceROI(flrROI)))
		{
			ErrorPrint(eResult, "Failed to set Source ROI.");
			break;
		}

		// �ڳʸ� �����ϴ� �̹����� Scale ���� ����
		if(IsFail(eResult = harris.SetScale(1.0)))
		{
			ErrorPrint(eResult, "Failed to set scale.");
			break;
		}

		// ������ �ִ� ���� ������ ����
		if(IsFail(eResult = harris.SetMaxPoints(500)))
		{
			ErrorPrint(eResult, "Failed to set max points.");
			break;
		}

		// ������ ������ �Ӱ谪�� ����
		if(IsFail(eResult = harris.SetScoreThreshold(0.8f)))
		{
			ErrorPrint(eResult, "Failed to set score threshold.");
			break;
		}

		// �ظ��� �ڳ� �������� �ĸ����� K�� ����
		if(IsFail(eResult = harris.SetParamK(0.04f)))
		{
			ErrorPrint(eResult, "Failed to set param K.");
			break;
		}

		// �ظ��� �ڳ� ������ ���� �Լ�
		if(IsFail(eResult = harris.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute.");
			break;
		}

		// ���� ����� �޾ƿ��� ���� �����̳�
		Foundation::CFLFigureArray flfaResultPoints;

		// �ظ��� �ڳ� ������ ���� �Լ�
		if(IsFail(eResult = harris.GetResultPoints(&flfaResultPoints)))
		{
			ErrorPrint(eResult, "Failed to get result.");
			break;
		}

		// ����� ���� ������ �������� �Լ�
		int64_t i64Count = harris.GetResultCount();

		for(int64_t i = 0; i < i64Count; i++)
		{
			// ����� ���� ���
			CFLPointD* pFlpTemp = dynamic_cast<CFLPointD*>(flfaResultPoints.GetAt(i));
			layer.DrawFigureImage(pFlpTemp, RED, 1);
		}

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