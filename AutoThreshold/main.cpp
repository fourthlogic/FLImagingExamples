#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


#include <iostream>

enum EType
{
	EType_Source = 0,
	EType_Destination1,
	EType_Destination2,
	ETypeCount,
};

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	// �˰��� ���� ��� // Algorithm execution result
	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = arrFliImage[EType_Source].Load(L"../../ExampleImages/Threshold/Mountain.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		bool bError = false;

		for(int32_t i = EType_Destination1; i < ETypeCount; ++i)
		{
			// Destination �̹����� Source �̹����� ������ �̹����� ���� // Create destination image as same as source image
			if((res = arrFliImage[i].Assign(arrFliImage[EType_Source])).IsFail())
			{
				ErrorPrint(res, L"Failed to assign the image file.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			int32_t x = i % 3;
			int32_t y = i / 3;

			// �̹��� �� ���� // Create image view
			if((res = arrViewImage[i].Create(x * 400 + 400, y * 400, x * 400 + 400 + 400, y * 400 + 400)).IsFail())
			{
				ErrorPrint(res, L"Failed to create the image view.\n");
				bError = true;
				break;
			}

			// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
			if((res = arrViewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(res, L"Failed to set image object on the image view.\n");
				bError = true;
				break;
			}

			if(i == EType_Source)
				continue;

			// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
			if((res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[i])).IsFail())
			{
				ErrorPrint(res, L"Failed to synchronize view\n");
				bError = true;
				break;
			}

			// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
			if((res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[i])).IsFail())
			{
				ErrorPrint(res, L"Failed to synchronize window.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// ROI ������ ���� CFLRect ��ü ���� // Create a CFLRect object for setting ROI
		CFLRect<int32_t> flrROI(300, 200, 700, 600);

		// auto threshold ��ü ���� // Create auto threshold object
		CAutoThreshold AutoThreshold;
		// Source �̹��� ���� // Set the source image
		AutoThreshold.SetSourceImage(arrFliImage[EType_Source]);
		// Source ROI ���� // Set the source ROI
		AutoThreshold.SetSourceROI(flrROI);

		// Destination1 �̹��� ���� // Set the destination1 image
		AutoThreshold.SetDestinationImage(arrFliImage[EType_Destination1]);
		// Destination1 ROI ���� // Set the destination1 ROI
		AutoThreshold.SetDestinationROI(flrROI);

		// Sigma �� ���� // Set the sigma value
		AutoThreshold.SetSigma(2);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = AutoThreshold.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute algorithm.");
			break;
		}

		// Result ��� ���� Ȯ�� // get result count
		int64_t i64IndexCount = AutoThreshold.GetResultBatchCount();

		// Threshold �� ǥ�⸦ ���� String ���� // string variable to indicate Threshold value
		CFLArray<CFLString<wchar_t>> flaThresholdStr;

		// AutoThreshold ����� // auto threshold result object
		CFLArray<int32_t> flaResult;

		for(int64_t i = 0; i < i64IndexCount; ++i)
		{
			// ���� ������ ���� // data clear
			flaThresholdStr.Clear();

			// Channel �� �������� // get channel result
			int64_t i64ChannelCount = AutoThreshold.GetChannelCount(i);

			for(int j = 0; j < i64ChannelCount; ++j)
			{
				// ���� ������ ���� // data clear
				flaResult.Clear();

				// AutoThreshold ��� �� �������� // get auto threshold result
				if((res = (AutoThreshold.GetResultBatch(i, j, flaResult))).IsFail())
				{
					ErrorPrint(res, L"No Result.");
					break;
				}

				int64_t i64ListCount = flaResult.GetCount();

				// Threshold �� ǥ�⸦ ���� String �ӽú��� // string variable to indicate threshold temporary value
				CFLString<wchar_t> strTemp;
				strTemp.Format(L"Ch%d ", j);

				for(int k = 0; k < i64ListCount; ++k)
					strTemp.AppendFormat(L"%d ", flaResult[k]);

				flaThresholdStr.PushBack(strTemp);
			}
		}

		// Destination2 �̹��� ���� // Set the destination2 image
		AutoThreshold.SetDestinationImage(arrFliImage[EType_Destination2]);
		// Destination2 ROI ���� // Set the destination2 ROI
		AutoThreshold.SetDestinationROI(flrROI);

		// auto threshold Ŀ�� ũ�� ����
		AutoThreshold.SetSigma(4);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = AutoThreshold.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute algorithm.");
			break;
		}
		
		// Result ��� ���� Ȯ�� // get result count
		i64IndexCount = AutoThreshold.GetResultBatchCount();

		// Threshold �� ǥ�⸦ ���� String ���� // string variable to indicate Threshold value
		CFLArray<CFLString<wchar_t>> flaThresholdStr2;

		for(int64_t i = 0; i < i64IndexCount; ++i)
		{
			// ���� ������ ���� // data clear
			flaThresholdStr2.Clear();

			// Channel �� �������� // get channel result
			int64_t i64ChannelCount = AutoThreshold.GetChannelCount(i);

			for(int j = 0; j < i64ChannelCount; ++j)
			{
				// ���� ������ ���� // data clear
				flaResult.Clear();

				// AutoThreshold ��� �� �������� // get auto threshold result
				if((res = (AutoThreshold.GetResultBatch(i, j, flaResult))).IsFail())
				{
					ErrorPrint(res, L"No Result.");
					break;
				}

				int64_t i64ListCount = flaResult.GetCount();

				// Threshold �� ǥ�⸦ ���� String �ӽú��� // string variable to indicate threshold temporary value
				CFLString<wchar_t> strTemp;
				strTemp.Format(L"Ch%d ", j);

				for(int k = 0; k < i64ListCount; ++k)
					strTemp.AppendFormat(L"%d ", flaResult[k]);

				flaThresholdStr2.PushBack(strTemp);
			}
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
			// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
			// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
			if((res = arrLayer[i].DrawFigureImage(&flrROI, LIME)).IsFail())
				ErrorPrint(res, L"Failed to draw figure\n");
		}

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic

		CFLPoint<double> flpThreshold(20, 320);

		if((res = arrLayer[EType_Source].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination1 Image (Sigma 2)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination1].DrawTextCanvas(flpThreshold, flaThresholdStr[0], YELLOW, BLACK, 16)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text.\n");
			break;
		}

		if((res = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination2 Image (Sigma 4)", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination2].DrawTextCanvas(flpThreshold, flaThresholdStr2[0], YELLOW, BLACK, 16)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
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