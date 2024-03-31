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
		if(IsFail(res = fliISrcImage.Load(L"../../ExampleImages/Threshold/BallChip.flif")))
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

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// MultiVar ��ü ���� // Create MultiVar object
		CMultiVarL mvCondition;
		CMultiVar<double> mvThreshold;

		// Otsu Threshold ��ü ���� // Create Otsu Threshold object
		COtsuThreshold threshold;

		// Source �̹��� ���� // Set source image 
		threshold.SetSourceImage(fliISrcImage);

		// Destination �̹��� ���� // Set destination image
		threshold.SetDestinationImage(fliIDstImage);

		// 1ä�� ������ �Է� // 1Channel condition push
		mvCondition.PushBack(ELogicalCondition_Greater);

		// �� ���� ���� // Set condition value
		threshold.SetLogicalCondition(mvCondition);

		// ���� Threshold ���� ���� // get result threshold value
		mvThreshold = threshold.GetResultThreshold();

		// Consoleâ�� Threshold �� ��� // Output the threshold value to the console window
		printf("Result Threshold Value : %d", (int)mvThreshold[0]);

		// �˰��� ���� // Execute the algorithm
		if((res = threshold.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Otsu Threshold.");
			break;
		}

		// Threshold �� ���ڿ� ���� // Create threshold value String
		Base::CFLString<wchar_t> flStrThresholdValue;
		flStrThresholdValue.Format(L"%d < threshold", (int)mvThreshold[0]);

		// ���̾�� ���� �������� �ʾƵ� View�� ���� �� �� ���� �����ȴ�. // The layer is released together when View is released without releasing it separately.
		CGUIViewImageLayerWrap layer = viewImage[1].GetLayer(0);

		if(IsFail(res = layer.DrawTextCanvas(&CFLPointD(0, 0), flStrThresholdValue.GetBuffer(), YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImage[1].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}