#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


enum EType
{
	EType_Src = 0,
	EType_Dst,
	ETypeCount,
};

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	bool bError = false;

	do
	{
		// ���� ��� // operation result
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(eResult = arrFliImage[EType_Src].Load(L"../../ExampleImages/RandomImageGenerator/Figure.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// �̹����� Copy // image copy
		arrFliImage[EType_Dst].Assign(arrFliImage[EType_Src]);

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if(i != EType_Src)
			{
				// �̹����� Copy // image copy
				if(IsFail(eResult = arrFliImage[i].Assign(arrFliImage[EType_Src])))
				{
					ErrorPrint(eResult, "Failed to assign the image file.\n");
					bError = true;
					break;
				}
			}

			int32_t i32X = i % 2;
			int32_t i32Y = i / 2;

			// �̹��� �� ���� // Create image view
			if(IsFail(eResult = arrViewImage[i].Create(i32X * 400 + 400, i32Y * 400, i32X * 400 + 400 + 400, i32Y * 400 + 400)))
			{
				ErrorPrint(eResult, "Failed to create the image view.\n");
				bError = true;
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
			if(IsFail(eResult = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(eResult, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}

			if(i != EType_Src)
			{
				// �ΰ��� ���� ������ ����ȭ  // Synchronize the viewpoints of the two image views. 
				if(IsFail(eResult = arrViewImage[EType_Src].SynchronizePointOfView(&arrViewImage[i])))
				{
					ErrorPrint(eResult, "Failed to synchronize window.\n");
					bError = true;
					break;
				}

				// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
				if(IsFail(eResult = arrViewImage[EType_Src].SynchronizeWindow(&arrViewImage[i])))
				{
					ErrorPrint(eResult, "Failed to synchronize window.\n");
					bError = true;
					break;
				}
			}
		}

		if(bError)
			break;

		// Random Image Generator ��ü ���� // Create Random Image Generator object
		CRandomImageGenerator RandomImageGenerator;

		// Random Image Generator ROI ���� // Create ROI range
		CFLFigureArray flfaSrcROI;

		flfaSrcROI.Load(L"../../ExampleImages/RandomImageGenerator/FigureROI.fig");

		// Source �̹��� ���� // Set source image 
		RandomImageGenerator.SetSourceImage(arrFliImage[EType_Dst]);

		// Random Image Generator ���� ���� // set source ROI 
		RandomImageGenerator.SetSourceROI(flfaSrcROI);

		// �˰��� ���� // Execute the algorithm
		if((eResult = RandomImageGenerator.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Random Image Generator.");
			break;
		}

		// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
		// ���� ������ �ʿ� ���� // No need to release separately
		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			arrLayer[i] = arrViewImage[i].GetLayer(0);
		}

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = arrLayer[EType_Src].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Dst].DrawTextCanvas(&CFLPointD(0, 0), L"RandomImageGenerator Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// RandomImageGenerator ���� ǥ�� // RandomImageGenerator Area draw
		if(IsFail(eResult = arrLayer[EType_Src].DrawFigureImage(&flfaSrcROI, LIME)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		for(int32_t i = 1; i >= 0; --i)
		{
			// �̹��� �並 ���� �մϴ�. // Update the image view.
			arrViewImage[i].ZoomFit();
			arrViewImage[i].Invalidate(true);
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		bool bRun = true;
		while(bRun)
		{
			for(int32_t i = 0; i < ETypeCount; ++i)
			{
				bRun &= arrViewImage[i].IsAvailable();
			}

			CThreadUtilities::Sleep(1);
		}
	}
	while(false);

	return 0;
}