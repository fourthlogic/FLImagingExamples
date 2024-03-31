#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
		// �̹��� ��ü ���� // Declare the image object
	CFLImage fliISrcImage;
	CFLImage fliIDstImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage1;
	CGUIViewImageWrap viewImage2;

	do
	{
		// ���� ��� // operation result
		CResult res = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliISrcImage.Load(L"../../ExampleImages/AnisotropicDiffusion/Source.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage1.Create(300, 0, 300 + 512, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage2.Create(300 + 512, 0, 300 + 512 * 2, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(res = viewImage1.SetImagePtr(&fliISrcImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImage2.SetImagePtr(&fliIDstImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views. 
		if(IsFail(res = viewImage1.SynchronizePointOfView(&viewImage2)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImage1.SynchronizeWindow(&viewImage2)))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		// Watersheds Threshold Marker ��ü ���� // Create Watersheds Threshold Marker object
		CWatershedsThresholdMarker threshold;

		// Source �̹��� ���� // Set source image 
		threshold.SetSourceImage(fliISrcImage);

		// Destination �̹��� ���� // Set destination image
		threshold.SetDestinationImage(fliIDstImage);

		// threshold ��� ����(Single) // Set threshold mode(Single)
		threshold.SetThresholdMode(EThresholdMode_Single);

		// MultiVar ��ü ���� // Create MultiVar object
		CMultiVar<double> mvThresholdValue;
		CMultiVarL mvCondition;

		// �Ӱ谪 �Է� // threshold value push
		mvThresholdValue.PushBack(10);

		// �Ӱ谪 ���� (��ä�� ��� CMultiVar<double> ���) // Set threshold value(Use CMultiVar<double> for multi-channel)
		threshold.SetThreshold(mvThresholdValue);

		// 1ä�� ������ �Է� // 1Channel condition push
		mvCondition.PushBack(ELogicalCondition_GreaterEqual);

		// �� ���� ���� // Set condition value
		threshold.SetLogicalCondition(mvCondition);

		// �˰��� ���� // Execute the algorithm
		if((res = threshold.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Watersheds Threshold.");
			break;
		}

		// ���̾�� ���� �������� �ʾƵ� View�� ���� �� �� ���� �����ȴ�. // The layer is released together when View is released without releasing it separately.
		CGUIViewImageLayerWrap layer = viewImage2.GetLayer(0);

		// threshold ���� ���÷��� �մϴ�. // Display threshold value.
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = layer.DrawTextCanvas(&CFLPointD(0, 0), L"0 <= threshold", YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImage2.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage1.IsAvailable() && viewImage2.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}