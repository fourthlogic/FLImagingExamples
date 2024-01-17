#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliISrcImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImage1;

	do
	{
		// ���� ��� // operation result
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Loads image 
		if(IsFail(eResult = fliISrcImage.Load(L"../../ExampleImages/PixelCounter/Semiconductor.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImage1.Create(300, 0, 300 + 512, 384)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(eResult = viewImage1.SetImagePtr(&fliISrcImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Pixel Counter ��ü ���� // Create Pixel Counter object
		CPixelCounter PixelCounter;

		// Source �̹��� ���� // Set source image 
		PixelCounter.SetSourceImage(fliISrcImage);

		// Source ROI ���� // Set source ROI 
		CFLQuad<double> flfSourceROI(170.550171, 102.400000, 380.243003, 135.950853, 341.100341, 312.092833, 124.417747, 265.960410);
		PixelCounter.SetSourceROI(flfSourceROI);

		// PixelCounter ��� ����(Dual) // Set Threshold Mode(Dual)
		PixelCounter.SetThresholdMode(EThresholdMode_Dual_And);

		// MultiVar ��ü ���� // Create MultiVar object
		CMultiVar<double> mvThresholdValue1, mvThresholdValue2;
		CMultiVarL mvCondition1, mvCondition2;

		// �Ӱ谪 �Է� // PixelCounter value push
		mvThresholdValue1.PushBack(120);
		mvThresholdValue2.PushBack(230);

		// �Ӱ谪 ���� (��ä�� ��� CMultiVar<double> ���) // Set PixelCounter value(Use CMultiVar<double> for multi-channel)
		PixelCounter.SetThreshold(mvThresholdValue1, mvThresholdValue2);

		// 1ä�� ������ �Է� // 1Channel condition push
		mvCondition1.PushBack(ELogicalCondition_Greater);
		mvCondition2.PushBack(ELogicalCondition_Less);

		// �� ���� ���� // Set condition value
		PixelCounter.SetLogicalCondition(mvCondition1, mvCondition2);

		// �˰��� ���� // Execute the algorithm
		if((eResult = PixelCounter.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Pixel Counter.");
			break;
		}

		int64_t i64TotalPixel = PixelCounter.GetResultTotalPixelCount();
		int64_t i64ValidPixel = PixelCounter.GetResultValidPixelCount();
		int64_t i64InvalidPixel = PixelCounter.GetResultInvalidPixelCount();

		// ��ü �ȼ�, ��ȿ�� �ȼ�, ��ȿ���� ���� �ȼ� ���� ��� // display Total, Valid, Invalid Pixel Count
		{
			printf("Total Pixel Count : %lld\n", i64TotalPixel);
			printf("Valid Pixel Count : %lld\n", i64ValidPixel);
			printf("Invalid Pixel Count : %lld\n", i64InvalidPixel);
		}

		// Text ��� // Draw Text 
		CFLString<wchar_t> flsDrawText;
		flsDrawText.Format(L"Source Image\n120 < threshold < 230\nTotal Pixel Count: %lld\nValid Pixel Count: %lld\nInvalid Pixel Count: %lld", i64TotalPixel, i64ValidPixel, i64InvalidPixel);

		// ���̾�� ���� �������� �ʾƵ� View�� ���� �� �� ���� �����ȴ�. // The layer is released together when View is released without releasing it separately.
		CGUIViewImageLayerWrap layer = viewImage1.GetLayer(0);

		// PixelCounter ���� ���÷��� �մϴ�. // Display PixelCounter value.
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = layer.DrawTextCanvas(&CFLPointD(0, 0), flsDrawText, YELLOW, BLACK, 15, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false)))
		{
			ErrorPrint(eResult, "Failed to draw text on the image view.\n");
			break;
		}

		// Source ROI �׸��� // Draw Source ROI
		if(IsFail(eResult = layer.DrawFigureImage(flfSourceROI, LIME)))
		{
			ErrorPrint(eResult, "Failed to draw Source ROI on the image view.\n");
			break;
		}

		viewImage1.Invalidate(false);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage1.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}