#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


enum EType
{
	EType_Src = 0,
	EType_Crop,
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
		CResult res = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(res = arrFliImage[EType_Src].Load(L"../../ExampleImages/Crop/bacteria.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹����� Copy // image copy
		arrFliImage[EType_Crop].Assign(arrFliImage[EType_Src]);

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if(i != EType_Src)
			{
				// �̹����� Copy // image copy
				if(IsFail(res = arrFliImage[i].Assign(arrFliImage[EType_Src])))
				{
					ErrorPrint(res, "Failed to assign the image file.\n");
					bError = true;
					break;
				}
			}

			int32_t i32X = i % 2;
			int32_t i32Y = i / 2;

			// �̹��� �� ���� // Create image view
			if(IsFail(res = arrViewImage[i].Create(i32X * 400 + 400, i32Y * 400, i32X * 400 + 400 + 400, i32Y * 400 + 400)))
			{
				ErrorPrint(res, "Failed to create the image view.\n");
				bError = true;
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
			if(IsFail(res = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}

			if(i != EType_Src)
			{
				// �ΰ��� ���� ������ ����ȭ  // Synchronize the viewpoints of the two image views. 
				if(IsFail(res = arrViewImage[EType_Src].SynchronizePointOfView(&arrViewImage[i])))
				{
					ErrorPrint(res, "Failed to synchronize window.\n");
					bError = true;
					break;
				}

				// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
				if(IsFail(res = arrViewImage[EType_Src].SynchronizeWindow(&arrViewImage[i])))
				{
					ErrorPrint(res, "Failed to synchronize window.\n");
					bError = true;
					break;
				}
			}
		}

		if(bError)
			break;

		// Crop ��ü ���� // Create Crop object
		CCrop Crop;

		// Crop ROI ���� // Create ROI range
		CFLRect<double> flrCropROI(161, 181, 293, 302);

		// Source �̹��� ���� // Set source image 
		Crop.SetSourceImage(arrFliImage[EType_Crop]);

		// Crop ���� ���� // set source ROI 
		Crop.SetSourceROI(flrCropROI);

		// �˰��� ���� // Execute the algorithm
		if((res = Crop.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Crop.");
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
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = arrLayer[EType_Src].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_Crop].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Crop Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// Crop ���� ǥ�� // Crop Area draw
		if(IsFail(res = arrLayer[EType_Src].DrawFigureImage(&flrCropROI, LIME)))
			ErrorPrint(res, "Failed to draw figure\n");

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