#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


enum EType
{
	EType_Source = 0,
	EType_Destination,
	ETypeCount,
};

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	do
	{
		// ���� ��� // operation result
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(eResult = arrFliImage[EType_Source].Load(L"../../ExampleImages/RegionInterpolation/Sky_Damaged.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		bool bError = false;

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// �̹��� �� ���� // Create image view
			if(IsFail(eResult = arrViewImage[i].Create(i * 400 + 400, 0, i * 400 + 400 + 400, 400)))
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

			if(i == EType_Source)
				continue;

			// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views.
			if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[i])))
			{
				ErrorPrint(eResult, "Failed to synchronize view\n");
				bError = true;
				break;
			}

			// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
			if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[i])))
			{
				ErrorPrint(eResult, "Failed to synchronize window.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// Harmonic Interpolation ��ü ���� // Create  Harmonic Interpolation object
		CHarmonicInterpolation harmonic;
		// Source �̹��� ���� // Set source image
		harmonic.SetSourceImage(arrFliImage[EType_Source]);
		// Destination �̹��� ���� // Set destination image 
		harmonic.SetDestinationImage(arrFliImage[EType_Destination]);
		// ���е� ���� // Set precision value
		harmonic.SetPrecision(0.001);

		// �˰��� ���� // Execute the algorithm
		if((eResult = harmonic.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Harmonic Interpolation.");
			break;
		}

		// ROI Draw�� ���� CFLRectL ��ü ���� // Create a CFLRectL object for ROI Draw
		CFLRectL flrROI(164, 234, 339, 390);

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
			// ���� ������ �ʿ� ���� // No need to release separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			arrLayer[i].Clear();

			// ROI�� ���÷��� �մϴ�. // Display ROI range
			if(IsFail(eResult = arrLayer[i].DrawFigureImage(&flrROI, LIME)))
				ErrorPrint(eResult, "Failed to draw figure\n");
		}

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination].DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image (Harmonic)", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		for(int32_t i = 0; i < ETypeCount; ++i)
			arrViewImage[i].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		bool bAvailable = true;
		while(bAvailable)
		{
			for(int32_t i = 0; i < ETypeCount; ++i)
			{
				bAvailable = arrViewImage[i].IsAvailable();
				if(!bAvailable)
					break;
			}

			CThreadUtilities::Sleep(1);
		}
	}
	while(false);

	return 0;
}