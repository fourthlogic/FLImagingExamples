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
	CResult res;

	do
	{
		// �̹��� �ε� // Load image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/HarrisCornerDetector/Chip.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage.Create(400, 0, 1168, 540)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(res = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
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
		if(IsFail(res = harris.SetSourceImage(fliImage)))
		{
			ErrorPrint(res, "Failed to set Source Image.");
			break;
		}

		// ó���� ROI ����
		if(IsFail(res = harris.SetSourceROI(flrROI)))
		{
			ErrorPrint(res, "Failed to set Source ROI.");
			break;
		}

		// �ڳʸ� �����ϴ� �̹����� Scale ���� ����
		if(IsFail(res = harris.SetScale(1.0)))
		{
			ErrorPrint(res, "Failed to set scale.");
			break;
		}

		// ������ �ִ� ���� ������ ����
		if(IsFail(res = harris.SetMaxPoints(500)))
		{
			ErrorPrint(res, "Failed to set max points.");
			break;
		}

		// ������ ������ �Ӱ谪�� ����
		if(IsFail(res = harris.SetScoreThreshold(0.8f)))
		{
			ErrorPrint(res, "Failed to set score threshold.");
			break;
		}

		// �ظ��� �ڳ� �������� �ĸ����� K�� ����
		if(IsFail(res = harris.SetParamK(0.04f)))
		{
			ErrorPrint(res, "Failed to set param K.");
			break;
		}

		// �ظ��� �ڳ� ������ ���� �Լ�
		if(IsFail(res = harris.Execute()))
		{
			ErrorPrint(res, "Failed to execute.");
			break;
		}

		// ���� ����� �޾ƿ��� ���� �����̳�
		Foundation::CFLFigureArray flfaResultPoints;

		// �ظ��� �ڳ� ������ ���� �Լ�
		if(IsFail(res = harris.GetResultPoints(&flfaResultPoints)))
		{
			ErrorPrint(res, "Failed to get result.");
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
		if(IsFail(res = layer.DrawFigureImage(&flrROI, BLUE)))
		{
			ErrorPrint(res, "Failed to draw figures objects on the image view.\n");
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