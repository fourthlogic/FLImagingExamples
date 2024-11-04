#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

enum EType
{
	EType_Source = 0,
	EType_Destination1,
	EType_Destination2,
	EType_Destination3,
	ETypeCount,
};

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage[ETypeCount];

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = arrFliImage[EType_Source].Load(L"../../ExampleImages/NoiseImage/NoiseImage1.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		bool bError = false;

		for(int32_t i = EType_Destination1; i < ETypeCount; ++i)
		{
			// Destination �̹����� Source �̹����� ������ �̹����� ���� // Create destination image as same as source image
			if(IsFail(res = arrFliImage[i].Assign(arrFliImage[EType_Source])))
			{
				ErrorPrint(res, "Failed to assign the image file.\n");
				bError = true;
				break;
			}
		}

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			int32_t x = i % 2;
			int32_t y = i / 2;

			// �̹��� �� ���� // Create image view
			if(IsFail(res = viewImage[i].Create(x * 400 + 400, y * 400, x * 400 + 400 + 400, y * 400 + 400)))
			{
				ErrorPrint(res, "Failed to create the image view.\n");
				bError = true;
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
			if(IsFail(res = viewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}

			if(i == EType_Source)
				continue;

			// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
			if(IsFail(res = viewImage[EType_Source].SynchronizePointOfView(&viewImage[i])))
			{
				ErrorPrint(res, "Failed to synchronize view\n");
				bError = true;
				break;
			}

			// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
			if(IsFail(res = viewImage[EType_Source].SynchronizeWindow(&viewImage[i])))
			{
				ErrorPrint(res, "Failed to synchronize window.\n");
				bError = true;
				break;
			}
		}

		// AnisotropicDiffusion ��ü ���� // Create AnisotropicDiffusion object
		CAnisotropicDiffusion AnisotropicDiffusion;

		// Source �̹��� ���� // Set the source image
		AnisotropicDiffusion.SetSourceImage(arrFliImage[EType_Source]);

		// Destination1 �̹��� ���� // Set the destination image
		AnisotropicDiffusion.SetDestinationImage(arrFliImage[EType_Destination1]);

		// diffusion coefficient ��� ����(Parabolic) // Set the diffusion coefficient mode
		AnisotropicDiffusion.SetDiffusionMode(CAnisotropicDiffusion::EDiffusionCoefficientMode_Parabolic);

		// Contrast = 3 ���� // Set the Contrast = 3
		AnisotropicDiffusion.SetContrast(3.0);

		// Theta = 3 ���� // Set the Theta = 3
		AnisotropicDiffusion.SetTheta(3.0);

		// Iteration = 5 ���� // Set the Iteration = 5
		AnisotropicDiffusion.SetIteration(5);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = AnisotropicDiffusion.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute AnisotropicDiffusion.\n");
			wprintf(res.GetString());
			break;
		}

		// Destination2 �̹��� ���� // Set the destination image
		AnisotropicDiffusion.SetDestinationImage(arrFliImage[EType_Destination2]);

		// diffusion coefficient ��� ����(Perona-Malik) // Set the diffusion coefficient mode
		AnisotropicDiffusion.SetDiffusionMode(CAnisotropicDiffusion::EDiffusionCoefficientMode_PeronaMalik);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = AnisotropicDiffusion.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute AnisotropicDiffusion.\n");
			wprintf(res.GetString());
			break;
		}

		// Destination3 �̹��� ���� // Set the destination image
		AnisotropicDiffusion.SetDestinationImage(arrFliImage[EType_Destination3]);

		// diffusion coefficient ��� ����(Weickert) // Set the diffusion coefficient mode
		AnisotropicDiffusion.SetDiffusionMode(CAnisotropicDiffusion::EDiffusionCoefficientMode_Weickert);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = AnisotropicDiffusion.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute AnisotropicDiffusion.\n");
			wprintf(res.GetString());
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately

			arrLayer[i] = viewImage[i].GetLayer(0);
			arrLayer[i].Clear();
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		if((res = arrLayer[EType_Source].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination1 Image (Parabolic Mode)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination2 Image (PeronaMalik Mode)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination3].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination3 Image (Weickert Mode)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		for(int32_t i = 0; i < ETypeCount; ++i)
			viewImage[i].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		bool bAvailable = true;
		while(bAvailable)
		{
			for(int32_t i = 0; i < ETypeCount; ++i)
			{
				bAvailable = viewImage[i].IsAvailable();
				if(!bAvailable)
					break;
			}

			CThreadUtilities::Sleep(1);
		}
	}
	while(false);

	return 0;
}