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
		CResult res = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(res = arrFliImage[EType_Src].Load(L"../../ExampleImages/Integral/Lake.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

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

		// Integral ��ü ���� // Create Integral object
		CIntegral Integral;

		// Source �̹��� ���� // Set source image
		Integral.SetSourceImage(arrFliImage[EType_Src]);

		// Destination �̹��� ���� // Set destination image 
		Integral.SetDestinationImage(arrFliImage[EType_Dst]);

		// ������ �ڷ��� Ÿ���� �����մϴ�. // Set integral data type.
		Integral.SetDataType(CIntegral::EDataType_Uint32);

		// Integral ������ ���� ��� ���� // Set integration operation method.
		// ECalculationMode_Sum : bx ȭ�� �� // ECalculationMode_Sum : bx Sum
		Integral.SetCalculationMode(CIntegral::ECalculationMode_Sum);

		// ax^2 + bx + c ��� ����(a = 0, b = 2.1, c = 0) // ax^2 + bx + c Setting the coefficient (a = 0, b = 2.1, c = 0)
		Integral.SetCoefficients(CMultiVar<double>(0, 2.1, 0));

		// �˰��� ���� // Execute the algorithm
		if((res = Integral.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Integral.");
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
		if(IsFail(res = arrLayer[EType_Src].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_Dst].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		for(int32_t i = 0; i < ETypeCount; ++i)
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