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
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/ScaleInvariantFeatureTransform/Chip.flif")))
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

		// SIFT ��ü ���� // Create SIFT object
		CScaleInvariantFeatureTransform SIFT;

		// ó���� �̹��� ����
		if(IsFail(eResult = SIFT.SetSourceImage(fliImage)))
		{
			ErrorPrint(eResult, "Failed to set Source Image.");
			break;
		}

		// Ư¡���� ������ Octave Layer ���� ����
		if(IsFail(eResult = SIFT.SetOctaveLayers(3)))
		{
			ErrorPrint(eResult, "Failed to set octave layers.");
			break;
		}

		// ������ Ư¡���� ��� �Ӱ谪 ����
		if(IsFail(eResult = SIFT.SetContrastThreshold(0.04f)))
		{
			ErrorPrint(eResult, "Failed to set contrast threshold.");
			break;
		}

		// ������ Ư¡���� ���� �Ӱ谪 ����
		if(IsFail(eResult = SIFT.SetEdgeThreshold(10.f)))
		{
			ErrorPrint(eResult, "Failed to set edge threshold.");
			break;
		}

		// SIFT�� �Ķ���� Sigma ����
		if(IsFail(eResult = SIFT.SetSigma(1.6f)))
		{
			ErrorPrint(eResult, "Failed to set param sigma.");
			break;
		}

		// SIFT ���� �Լ�
		if(IsFail(eResult = SIFT.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute.");
			break;
		}

		// ���� ����� �޾ƿ��� ���� �����̳�
		Foundation::CFLPointArray flfaResultPoints;

		// Ű ����Ʈ ���� ����
		if(IsFail(eResult = SIFT.GetResultPoints(&flfaResultPoints)))
		{
			ErrorPrint(eResult, "Failed to get result.");
			break;
		}

		// ����� ���� ���
		layer.DrawFigureImage(flfaResultPoints, RED, 1);

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}