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
		if(IsFail(res = fliISrcImage.Load(L"../../ExampleImages/ActiveContour/Grid Of Cross.flif")))
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

		// Active Contour ��ü ���� // Create Active Contour object
		CActiveContour ac;

		// Source �̹��� ���� // Set source image 
		res = ac.SetSourceImage(fliISrcImage);

		// Source ROI ���� // Set source ROI
		CFLFigure* pFlfRegion = CFigureUtilities::ConvertFigureStringToObject(L"RG[D(129.22800000000007,126.67680000000001),D(731.22800000000007,120.67680000000001),D(733.22800000000007,262.67680000000001),D(253.22800000000007,246.67680000000001),D(265.22800000000007,600.67679999999996),D(603.22800000000007,594.67679999999996),D(607.22800000000007,400.67680000000001),D(403.22800000000007,396.67680000000001),D(409.22800000000007,448.67680000000001),D(565.22800000000007,450.67680000000001),D(549.22800000000007,556.67679999999996),D(289.22800000000007,558.67679999999996),D(291.22800000000007,292.67680000000001),D(721.22800000000007,294.67680000000001),D(721.22800000000007,720.67679999999996),D(119.22800000000007,718.67679999999996),D(113.22800000000007,142.67680000000001)]");
		res = ac.SetSourceROI(pFlfRegion);

		// Destination �̹��� ���� // Set destination image
		res = ac.SetDestinationImage(fliIDstImage);

		// Point Count ���� // Set Point Count
		res = ac.SetPointCount(3000);

		// Max Length ���� // Set Max Length
		res = ac.SetMaxLength(3);

		// Low Threshold ���� // Set Low Threshold
		res = ac.SetLowThreshold(20);

		// High Threshold ���� // Set High Threshold
		res = ac.SetHighThreshold(50);

		// Fit Margin ���� // Set Fit Margin
		res = ac.SetFitMargin(5);

		// �˰��� ���� // Execute the algorithm
		if((res = ac.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Active Contour.");

			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);

		for(int32_t i32Iteration = 0; i32Iteration < 20; ++i32Iteration)
		{
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Fit();
			ac.Spacing();
			ac.Spacing();
			ac.Spacing();
			ac.Spacing();
			ac.Spacing();

			/* PushBack Figure */
			{
				viewImage[0].ClearFigureObject();
				viewImage[0].PushBackFigureObject(ac.GetContourFigure());
				viewImage[0].Invalidate(true);

				Sleep(50);
			}
		}

		viewImage[0].PushBackFigureObject((CFLFigure*)ac.GetSourceROI());

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

		delete pFlfRegion;
	}
	while(false);

	return 0;
}