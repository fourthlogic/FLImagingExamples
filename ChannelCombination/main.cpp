#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliSourceImage[3];
	CFLImage fliDestinationImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageSource[3];
	CGUIViewImageWrap viewImageDestination;

	do
	{
		// ���� ��� // operation result
		CResult res = EResult_UnknownError;

		// �̹��� �ε� // Loads image
		if(IsFail(res = fliSourceImage[0].Load(L"../../ExampleImages/ChannelCombination/Valley1.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliSourceImage[1].Load(L"../../ExampleImages/ChannelCombination/Valley2.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliSourceImage[2].Load(L"../../ExampleImages/ChannelCombination/Valley3.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImageSource[0].Create(100, 0, 100 + 440, 340)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageSource[1].Create(100 + 440, 0, 100 + 440 * 2, 340)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageSource[2].Create(100 + 440 * 2, 0, 100 + 440 * 3, 340)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageDestination.Create(100 + 440 * 3, 0, 100 + 440 * 4, 340)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views. 
		if(IsFail(res = viewImageSource[0].SynchronizePointOfView(&viewImageSource[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(res = viewImageSource[1].SynchronizePointOfView(&viewImageSource[2])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		if(IsFail(res = viewImageSource[2].SynchronizePointOfView(&viewImageDestination)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageSource[0].SynchronizeWindow(&viewImageSource[1])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageSource[1].SynchronizeWindow(&viewImageSource[2])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		if(IsFail(res = viewImageSource[2].SynchronizeWindow(&viewImageDestination)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the image view
		if(IsFail(res = viewImageSource[0].SetImagePtr(&fliSourceImage[0])))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageSource[1].SetImagePtr(&fliSourceImage[1])))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageSource[2].SetImagePtr(&fliSourceImage[2])))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageDestination.SetImagePtr(&fliDestinationImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Channel Combination ��ü ���� // Create Channel Combination object
		CChannelCombination channelCombination;

		// Source �̹����� ������ Array ���� // Declare an Array to store the source image
		Base::CFLArray<CFLImage*> vctSrcImages;

		// ������ ä���� ������ Array ���� // Declare an Array to store the channels
		Base::CFLArray<int64_t> vctSrcChannels;

		// Source �̹��� �Է� // source images push
		vctSrcImages.PushBack(&fliSourceImage[0]);
		vctSrcImages.PushBack(&fliSourceImage[1]);
		vctSrcImages.PushBack(&fliSourceImage[2]);

		// �̹����� ������ ä���� �Է� // channels push
		vctSrcChannels.PushBack(EChannelSelection_Channel_0);
		vctSrcChannels.PushBack(EChannelSelection_Channel_0);
		vctSrcChannels.PushBack(EChannelSelection_Channel_0);

		// ������ �̹��� �� ä���Է� // Set images, channels
		channelCombination.SetSourceImage(vctSrcImages, vctSrcChannels);

		// ���� ����� ������ �̹��� ���� // Set destination image
		channelCombination.SetDestinationImage(fliDestinationImage);

		// �˰��� ���� // Execute the algorithm
		if((res = channelCombination.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Channel Combination.");
			break;
		}

		// ����� ���� �̹��� ���̾ ���ɴϴ�. //  Gets the image layer for output.
		// ���� ������ �ʿ� ���� // No need to release separately
		CGUIViewImageLayerWrap layerSource[3];
		CGUIViewImageLayerWrap layerDestination = viewImageDestination.GetLayer(1);
		layerSource[0] = viewImageSource[0].GetLayer(0);
		layerSource[1] = viewImageSource[1].GetLayer(0);
		layerSource[2] = viewImageSource[2].GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Delete the shapes drawn on the existing layer
		layerSource[0].Clear();
		layerSource[1].Clear();
		layerSource[2].Clear();
		layerDestination.Clear();

		// View ������ ���÷��� �մϴ�. // Display View information.
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.// The function DrawTextCanvas below draws a String based on the screen coordinates.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = layerSource[0].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image 1", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}
		if(IsFail(res = layerSource[1].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image 2", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}
		if(IsFail(res = layerSource[2].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image 3", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}
		if(IsFail(res = layerDestination.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image 1 + 2 + 3", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImageSource[0].Invalidate(true);
		viewImageSource[1].Invalidate(true);
		viewImageSource[2].Invalidate(true);
		viewImageDestination.Invalidate(true);

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((res = viewImageDestination.ZoomFit()).IsFail())
		{
			ErrorPrint(res, "Failed to zoom fit\n");
			break;
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSource[0].IsAvailable() && viewImageSource[1].IsAvailable() && viewImageSource[2].IsAvailable() && viewImageDestination.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}