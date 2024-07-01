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
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	bool bError = false;

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult res;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if(IsFail(res = arrFliImage[EType_Src].Load(L"../../ExampleImages/Morphology/Monkey.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// Destination �̹����� Src �̹����� ������ �̹����� ����
			if(i != EType_Src)
			{
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

			// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
			if(IsFail(res = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}

			if(i != EType_Src)
			{
				// �ΰ��� ���� ������ ����ȭ
				if(IsFail(res = arrViewImage[EType_Src].SynchronizePointOfView(&arrViewImage[i])))
				{
					ErrorPrint(res, "Failed to synchronize window.\n");
					bError = true;
					break;
				}

				// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
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

		// Morphology HitOrMiss ��ü ���� // Create Morphology HitOrMiss object
		CMorphologyHitOrMiss morphologyHitOrMiss;
		// ROI ���� ����
		CFLRect<int32_t> flrROI(90, 20, 430, 320);
		// ó���� �̹��� ����
		morphologyHitOrMiss.SetSourceImage(arrFliImage[EType_Src]);
		// ó���� ROI ����
		morphologyHitOrMiss.SetSourceROI(flrROI);

		// Destination �̹��� ���� // Set the destination image
		morphologyHitOrMiss.SetDestinationImage(arrFliImage[EType_Dst]);
		// Destination ROI ����
		morphologyHitOrMiss.SetDestinationROI(flrROI);

		CFLArray<CFLArray<float>> flarrKernel;

		CFLArray<float> flarrKernelElement;

		// Ŀ�� ����
		// 1,  1,  0,  0,  0,  1, 1
		// 1,  0, -1, -1, -1,  0, 1
		// 0, -1, -1, -1, -1, -1, 0
		// 0, -1, -1, -1, -1, -1, 0
		// 0, -1, -1, -1,  1, -1, 0
		// 1,  0, -1, -1, -1,  0, 1
		// 1,  1,  0,  0,  0,  1, 1

		flarrKernelElement.Append(1);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(-1);

		flarrKernel.Clear();
		flarrKernelElement.Clear();
		flarrKernelElement.Append(1);
		flarrKernelElement.Append(1);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(1);
		flarrKernelElement.Append(1);
		flarrKernel.Append(flarrKernelElement);

		flarrKernelElement.Clear();
		flarrKernelElement.Append(1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(1);
		flarrKernel.Append(flarrKernelElement);

		flarrKernelElement.Clear();
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(0);
		flarrKernel.Append(flarrKernelElement);

		flarrKernelElement.Clear();
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(0);
		flarrKernel.Append(flarrKernelElement);

		flarrKernelElement.Clear();
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(0);
		flarrKernel.Append(flarrKernelElement);

		flarrKernelElement.Clear();
		flarrKernelElement.Append(1);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(-1);
		flarrKernelElement.Append(1);
		flarrKernel.Append(flarrKernelElement);

		flarrKernelElement.Clear();
		flarrKernelElement.Append(1);
		flarrKernelElement.Append(1);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(0);
		flarrKernelElement.Append(1);
		flarrKernelElement.Append(1);
		flarrKernel.Append(flarrKernelElement);

		morphologyHitOrMiss.SetKernel(flarrKernel);

		// �ռ� ������ �Ķ���� ��� �˰����� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = morphologyHitOrMiss.Execute()))
		{
			ErrorPrint(res, "Failed to execute morphology HitorMiss.");
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			arrLayer[i].Clear();

			// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
			// FLImaging�� Figure ��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ���� // FLimaging's Figure objects can be displayed as a function regardless of the shape
			// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
			// �� ������ �ΰ��� �Ķ���ʹ� �������� ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
			// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(��������) -> �� ���İ� (��������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
			if(IsFail(res = arrLayer[i].DrawFigureImage(&flrROI, LIME)))
				ErrorPrint(res, "Failed to draw figure\n");
		}

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� ���������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(��������) -> �� ���İ� (��������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
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
			// �̹��� �並 ���� �մϴ�. // Update image view
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