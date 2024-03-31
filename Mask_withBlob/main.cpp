#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliSrcImage;
	CFLImage fliSrcImageMask;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageSrcMask;

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult res;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if(IsFail(res = fliSrcImage.Load(L"../../ExampleImages/Mask/ChessBoard.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// ���� �̹������� ��� �񱳸� ���� �̹����� ���� // Copy the image to compare the result with the original image
		if(IsFail(res = fliSrcImageMask.Assign(fliSrcImage)))
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create the source image view
		if(IsFail(res = viewImageSrc.Create(400, 0, 912, 612)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Mask �̹��� �� ���� // Create mask image view
		if(IsFail(res = viewImageSrcMask.Create(912, 0, 1424, 612)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ // Synchronize the viewpoints of the two image views
		if(IsFail(res = viewImageSrc.SynchronizePointOfView(&viewImageSrcMask)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageSrc.SynchronizeWindow(&viewImageSrcMask)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if(IsFail(res = viewImageSrc.SetImagePtr(&fliSrcImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Mask �̹��� �信 �̹����� ���÷��� // Display the image in the Mask image view
		if(IsFail(res = viewImageSrcMask.SetImagePtr(&fliSrcImageMask)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}


		// Blob ��ü ���� // Create Blob object
		CBlob blob;

		// ó���� �̹��� ���� // Set the image to process
		blob.SetSourceImage(fliSrcImage);
		// Threshold ��� ����. ���⼭�� 2�� Threshold�� �ΰ��� ������ And ������ ������ �����Ѵ�.
		// Threshold mode setting. Here, set the AND condition of the two conditions to true in the double threshold.
		blob.SetThresholdMode(EThresholdMode_Dual_And);
		// �� ���� ���� // Set logical condition
		blob.SetLogicalCondition(ELogicalCondition_Greater, ELogicalCondition_Less);
		// �Ӱ谪 ����,  ���� ���ǰ� �Ʒ��� ������ �������� 130���� ũ�� 240���� ���� ��ü�� ����
		// Threshold setting, when the above and below conditions are combined, an object greater than 130 and less than 240 is detected
		blob.SetThreshold(130, 240);

		// 2�� �Ӱ� �� ������ �Ʒ��� ���� �� ���� ����. // You can set the double threshold as below.
		//		blob.SetThreshold(130, EThresholdIndex_First);
		// 		blob.SetThreshold(240, EThresholdIndex_Second);

		// ��� ���۳� Contour�� �����ϱ� ���� Perforated ��� ����
		// Set perforated mode to support perforated contours
		blob.SetContourResultType(CBlob::EContourResultType_Perforated);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = blob.Execute()))
		{
			ErrorPrint(res, "Failed to execute Blob.");
			break;
		}

		// Blob ����� ������ ���� FigureArray ���� // Declare FigureArray to get blob result
		CFLFigureArray flfaContours;

		// Blob ����� �� Contour�� ���� // Get Contour from Blob results
		if(IsFail(res = blob.GetResultContours(flfaContours)))
		{
			ErrorPrint(res, "Failed to get boundary rects from the Blob object.");
			break;
		}


		// Mask ��ü ���� // Create Mask object
		CMask mask;

		// Source �̹��� ���� // Set the source image
		mask.SetSourceImage(fliSrcImageMask);
		// Blob ����� flfaContours�� Src ROI�� ���� // Set blob result flfaContours as Src ROI
		mask.SetSourceROI(flfaContours);
		// Mask ���� ���� // set mask color
		mask.SetMask(CMultiVar<double>(20., 227., 248.));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = mask.Execute()))
		{
			ErrorPrint(res, "Failed to execute mask.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerSrcMask = viewImageSrcMask.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerSrcMask.Clear();

		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
		// FLImaging�� Figure ��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ���� // FLimaging's Figure objects can be displayed as a function regardless of the shape
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(res = layerSrc.DrawFigureImage(&flfaContours, LIME)))
			ErrorPrint(res, "Failed to draw figure\n");

		if(IsFail(res = layerSrcMask.DrawFigureImage(&flfaContours, LIME)))
			ErrorPrint(res, "Failed to draw figure\n");


		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = layerSrc.DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerSrcMask.DrawTextCanvas(&CFLPointD(0, 0), L"Mask Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// Contours �������� ���� Ȯ���ϴ� �ڵ� // Code to check each Contours information value
		for(int64_t i = 0; i < flfaContours.GetCount(); ++i)
		{
			CFLRegion* pFlrg = (CFLRegion*)flfaContours.GetAt(i);

			// �������� ���� ������ �ֿܼ� ��� // Print polygon vertex information to the console
			printf("Blob Result No. %lld : [", i);
			for(int64_t j = 0; j < pFlrg->GetCount(); ++j)
			{
				if(j)
					printf(",");

				CFLPointL* pFlpVertex = (CFLPointL*)pFlrg->GetAt(j);

				printf("(%d,%d)", pFlpVertex->x, pFlpVertex->y);
			}

			printf("]\n\n");

			CFLRectD flr = pFlrg->GetBoundaryRect();

			CFLString<wchar_t> strNumber;
			strNumber.Format(L"%d", i);

			layerSrc.DrawTextImage(&CFLPointD(flr.left, (flr.top + flr.bottom) * .5), (wchar_t*)strNumber.GetString(), BLACK, YELLOW, 12, false, 0, EGUIViewImageTextAlignment_CENTER, nullptr, 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false);
		}

		// �̹��� �並 ���� // Update image view
		viewImageSrc.Invalidate(true);
		viewImageSrcMask.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewImageSrcMask.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}