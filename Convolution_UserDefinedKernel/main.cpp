#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


enum EType
{
	EType_Src = 0,
	EType_Dst1,
	EType_Dst2,
	EType_Dst3,
	ETypeCount,
};

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	bool bError = false;

	do
	{
		CResult res = EResult_UnknownError;
		// Source �̹��� �ε� // Load the source image
		if(IsFail(res = arrFliImage[EType_Src].Load(L"../../ExampleImages/Convolution/Sun.flif")))
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

			// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
			if(i != EType_Src)
			{
				if(IsFail(res = arrViewImage[EType_Src].SynchronizePointOfView(&arrViewImage[i])))
				{
					ErrorPrint(res, "Failed to synchronize view\n");
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

		// Convolution UserDefinedKernel ��ü ���� // Create Convolution UserDefinedKernel object
		CConvolutionUserDefinedKernel convolution;

		// Source �̹��� ���� // Set the source image
		convolution.SetSourceImage(arrFliImage[EType_Src]);

		// Ŀ���� �����ϱ� ���� FLArray ����
		CFLArray<CFLArray<float>> flarrKernel;
		CFLArray<float> flarrKernelElement;

		// Ŀ�� ���� (Gaussian blur)
		// 1.f, 2.f, 1.f
		// 2.f, 4.f, 2.f
		// 1.f, 2.f, 1.f
		flarrKernel.Clear();

		flarrKernelElement.Clear();
		flarrKernelElement.Append(1.f);
		flarrKernelElement.Append(2.f);
		flarrKernelElement.Append(1.f);
		flarrKernel.Append(flarrKernelElement);

		flarrKernelElement.Clear();
		flarrKernelElement.Append(2.f);
		flarrKernelElement.Append(4.f);
		flarrKernelElement.Append(2.f);
		flarrKernel.Append(flarrKernelElement);

		flarrKernelElement.Clear();
		flarrKernelElement.Append(1.f);
		flarrKernelElement.Append(2.f);
		flarrKernelElement.Append(1.f);
		flarrKernel.Append(flarrKernelElement);

		// Ŀ�� ����
		convolution.SetKernel(flarrKernel);

		// Destination �̹��� ���� // Set the destination image
		convolution.SetDestinationImage(arrFliImage[EType_Dst1]);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = convolution.Execute()))
		{
			ErrorPrint(res, "Failed to execute convolution.");
			break;
		}

		// Ŀ�� ���� (Sharpen)
		// 0.f, -1.f, 0.f
		// -1.f, 5.f, -1.f
		// 0.f, -1.f, 0.f
		flarrKernel.Clear();

		flarrKernelElement.Clear();
		flarrKernelElement.Append(0.f);
		flarrKernelElement.Append(-1.f);
		flarrKernelElement.Append(0.f);
		flarrKernel.Append(flarrKernelElement);

		flarrKernelElement.Clear();
		flarrKernelElement.Append(-1.f);
		flarrKernelElement.Append(5.f);
		flarrKernelElement.Append(-1.f);
		flarrKernel.Append(flarrKernelElement);

		flarrKernelElement.Clear();
		flarrKernelElement.Append(0.f);
		flarrKernelElement.Append(-1.f);
		flarrKernelElement.Append(0.f);
		flarrKernel.Append(flarrKernelElement);

		// Ŀ�� ����
		convolution.SetKernel(flarrKernel);

		// Destination �̹��� ���� // Set the destination image
		convolution.SetDestinationImage(arrFliImage[EType_Dst2]);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = convolution.Execute()))
		{
			ErrorPrint(res, "Failed to execute convolution.");
			break;
		}

		// Ŀ�� ���� (Edge detection)
		// -1.f, -1.f, -1.f
		// -1.f, 8.f, -1.f
		// -1.f, -1.f, -1.f
		flarrKernel.Clear();

		flarrKernelElement.Clear();
		flarrKernelElement.Append(-1.f);
		flarrKernelElement.Append(-1.f);
		flarrKernelElement.Append(-1.f);
		flarrKernel.Append(flarrKernelElement);

		flarrKernelElement.Clear();
		flarrKernelElement.Append(-1.f);
		flarrKernelElement.Append(8.f);
		flarrKernelElement.Append(-1.f);
		flarrKernel.Append(flarrKernelElement);

		flarrKernelElement.Clear();
		flarrKernelElement.Append(-1.f);
		flarrKernelElement.Append(-1.f);
		flarrKernelElement.Append(-1.f);
		flarrKernel.Append(flarrKernelElement);

		// Ŀ�� ����
		convolution.SetKernel(flarrKernel);

		// Destination �̹��� ���� // Set the destination image
		convolution.SetDestinationImage(arrFliImage[EType_Dst3]);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = convolution.Execute()))
		{
			ErrorPrint(res, "Failed to execute convolution.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap arrLayer[ETypeCount];
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			arrLayer[i] = arrViewImage[i].GetLayer(0);
		}

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = arrLayer[EType_Src].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_Dst1].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image1", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_Dst2].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image2", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_Dst3].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image3", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}


		// �̹��� �並 ���� �մϴ�. // Update image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
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