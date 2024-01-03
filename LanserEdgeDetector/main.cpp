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
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(eResult = fliISrcImage.Load(L"../../ExampleImages/LanserEdgeDetector/Circuit Board.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImage[0].Create(300, 0, 300 + 520, 430)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].Create(300 + 520, 0, 300 + 520 * 2, 430)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(eResult = viewImage[0].SetImagePtr(&fliISrcImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(eResult = viewImage[1].SetImagePtr(&fliIDstImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views. 
		if(IsFail(eResult = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window\n");
			break;
		}

		// Lanser Edge Detector ��ü ���� // Create Lanser Edge Detector object
		CLanserEdgeDetector LanserEdge;

		// Source �̹��� ���� // Set source image 
		LanserEdge.SetSourceImage(fliISrcImage);

		// Destination �̹��� ���� // Set destination image
		LanserEdge.SetDestinationImage(fliIDstImage);

		// Edge Detector Mode ���� // Set Edge Detector Mode
		LanserEdge.SetEdgeDetectorMode(CLanserEdgeDetector::EEdgeDetectorMode_Shen);

		// Preprocessing Mode ���� // Set Preprocessing Mode
		LanserEdge.SetNoiseReduction(CLanserEdgeDetector::ENoiseReduction_Smoothing);

		// threshold �� // threshold value
		CMultiVar<double> mvThresholdValue;

		// �ּ� threshold �� �Է� // minimum threshold value push
		mvThresholdValue.PushBack(15);

		// �ִ� threshold �� �Է� // maximum threshold value push
		mvThresholdValue.PushBack(30);

		// �ּ�, �ִ� threshold ���� // Set minimum, maximum threshold
		LanserEdge.SetThreshold(mvThresholdValue);

		// Alpha �� ���� // Set alpha value
		LanserEdge.SetAlphaValue(2.0f);

		// �˰��� ���� // Execute the algorithm
		if((eResult = LanserEdge.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Lanser Edge Detector.");

			break;
		}

		// �̹��� �� ��� ���ڿ� // image view string
		Base::CFLString<wchar_t> flsText1 = L"Source Image";
		Base::CFLString<wchar_t> flsText2 = L"Destination Image";

		// ���̾�� ���� �������� �ʾƵ� View�� ���� �� �� ���� �����ȴ�. // The layer is released together when View is released without releasing it separately.
		CGUIViewImageLayerWrap layer1 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap layer2 = viewImage[1].GetLayer(0);

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = layer1.DrawTextCanvas(&CFLPointD(0, 0), flsText1.GetBuffer(), YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
		{
			ErrorPrint(eResult, "Failed to draw text on the image view.\n");
			break;
		}

		if(IsFail(eResult = layer2.DrawTextCanvas(&CFLPointD(0, 0), flsText2.GetBuffer(), YELLOW, BLACK, 30, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
		{
			ErrorPrint(eResult, "Failed to draw text on the image view.\n");
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