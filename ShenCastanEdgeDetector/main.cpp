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
		if(IsFail(res = fliISrcImage.Load(L"../../ExampleImages/ShenCastanEdgeDetector_Img/Gear.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage[0].Create(300, 0, 300 + 520, 430)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(300 + 520, 0, 300 + 520 * 2, 430)))
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

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views. 
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		// ShenCastan Edge Detector ��ü ���� // Create ShenCastan Edge Detector object
		CShenCastanEdgeDetector ShenCastanEdge;

		// Source �̹��� ���� // Set source image 
		ShenCastanEdge.SetSourceImage(fliISrcImage);

		// Destination �̹��� ���� // Set destination image
		ShenCastanEdge.SetDestinationImage(fliIDstImage);

		// Smoothing ��ó�� ���� // Set Smoothing preprocessing
		ShenCastanEdge.SetSmoothingMode(CShenCastanEdgeDetector::ESmoothingMode_ISEF);

		// threshold �� // threshold value
		CMultiVar<double> mvThresholdValue;

		// �ּ� threshold �� �Է� // minimum threshold value push
		mvThresholdValue.PushBack(5.0);

		// �ִ� threshold �� �Է� // maximum threshold value push
		mvThresholdValue.PushBack(8.0);

		// �ּ�, �ִ� threshold ���� // Set minimum, maximum threshold
		ShenCastanEdge.SetThreshold(mvThresholdValue);

		// p �� ���� // Set p value
		ShenCastanEdge.SetPValue(0.5);

		// �˰��� ���� // Execute the algorithm
		if((res = ShenCastanEdge.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute ShenCastan Edge Detector.");

			break;
		}

		// �̹��� �� ��� ���ڿ� // image view string
		Base::CFLString<wchar_t> flsText1 = L"Source Image";
		Base::CFLString<wchar_t> flsText2 = L"Destination Image";

		// ���̾�� ���� �������� �ʾƵ� View�� ���� �� �� ���� �����ȴ�. // The layer is released together when View is released without releasing it separately.
		CGUIViewImageLayerWrap layer1 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layer2 = viewImage[1].GetLayer(0);

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = layer1.DrawTextCanvas(&CFLPoint<double>(0, 0), flsText1.GetBuffer(), YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(res = layer2.DrawTextCanvas(&CFLPoint<double>(0, 0), flsText2.GetBuffer(), YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
		{
			ErrorPrint(res, "Failed to draw text on the image view.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}