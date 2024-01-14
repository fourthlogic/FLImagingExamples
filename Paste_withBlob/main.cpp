#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


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

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult eResult;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if(IsFail(eResult = arrFliImage[EType_Source].Load(L"../../ExampleImages/Paste/ChessBoard.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination1 �̹��� �ε�. // Load the destination1 image
		if(IsFail(eResult = arrFliImage[EType_Destination1].Load(L"../../ExampleImages/Paste/Floor.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination2 �̹����� Destination1 �̹����� ������ �̹����� ����
		if(IsFail(eResult = arrFliImage[EType_Destination2].Assign(arrFliImage[EType_Destination1])))
		{
			ErrorPrint(eResult, "Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(eResult = arrViewImage[EType_Source].Create(100, 0, 612, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination1 �̹��� �� ���� // Create destination1 image view
		if(IsFail(eResult = arrViewImage[EType_Destination1].Create(612, 0, 1124, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination2 image view
		if(IsFail(eResult = arrViewImage[EType_Destination2].Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if(IsFail(eResult = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(eResult, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination1])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination2])))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination1])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if(IsFail(eResult = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination2])))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Blob ��ü ���� // Create Blob object
		CBlob blob;

		// ó���� �̹��� ���� // Set the image to process
		blob.SetSourceImage(arrFliImage[EType_Source]);
		// Threshold ��� ����. ���⼭�� 2�� Threshold�� �ΰ��� ������ And ������ ������ �����Ѵ�.
		// Threshold mode setting. Here, set the AND condition of the two conditions to true in the double threshold.
		blob.SetThresholdMode(EThresholdMode_Dual_And);
		// �� ���� ���� // Set logical condition
		blob.SetLogicalCondition(ELogicalCondition_Greater, ELogicalCondition_Less);
		// �Ӱ谪 ����,  ���� ���ǰ� �Ʒ��� ������ �������� 130���� ũ�� 240���� ���� ��ü�� ����
		// Set the threshold, when the above and below conditions are combined, objects greater than 130 and less than 240 are detected
		blob.SetThreshold(130, 240);

		// ��� ���۳� Contour�� �����ϱ� ���� Perforated ��� ����
		// Set perforated mode to support perforated contours
		blob.SetContourResultType(CBlob::EContourResultType_Perforated);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = blob.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute Blob.");
			break;
		}

		// Blob ����� ������ ���� FigureArray ���� // Declare FigureArray to get blob result
		CFLFigureArray flfaContours;

		// Blob ����� �� Contour�� ���� // Get Contour from Blob results
		if(IsFail(eResult = blob.GetResultContours(flfaContours)))
		{
			ErrorPrint(eResult, "Failed to get boundary rects from the Blob object.");
			break;
		}


		// Paste ��ü ���� // Create Paste object
		CPaste paste;

		// Source �̹��� ���� // Set the source image
		paste.SetSourceImage(arrFliImage[EType_Source]);
		// Blob ����� flfaContours�� Src ROI�� ���� // Set blob result flfaContours as Src ROI
		paste.SetSourceROI(flfaContours);
		// Destination �̹��� ���� // Set the destination image
		paste.SetDestinationImage(arrFliImage[EType_Destination2]);
		// Destination Pivot ���� // Set the destination pivot
		paste.SetDestinationPivot(flfaContours.GetCenter());
		// �׻� ���� ������ ������ ������ ä�쵵�� ������ ������ ���� // Choose whether to always fill blank areas with the specified color
		paste.EnableFillBlankColorMode(false);
		// ���� ���� ���� ���� // Set the blank color
		paste.SetBlankColor(CMultiVar<double>(0., 0., 0.));

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(eResult = paste.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute paste.");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if(IsFail(eResult = arrViewImage[EType_Destination2].ZoomFit()))
		{
			ErrorPrint(eResult, "Failed to zoom fit of the image view.\n");
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
			arrLayer[i].Clear();
		}

		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
		// FLImaging�� Figure ��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ���� // FLimaging's Figure objects can be displayed as a function regardless of the shape
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(eResult = arrLayer[EType_Source].DrawFigureImage(&flfaContours, LIME)))
			ErrorPrint(eResult, "Failed to draw figure\n");

		if(IsFail(eResult = arrLayer[EType_Destination2].DrawFigureImage(&flfaContours, LIME)))
			ErrorPrint(eResult, "Failed to draw figure\n");


		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(eResult = arrLayer[EType_Source].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPointD(0, 0), L"Result Image", YELLOW, BLACK, 20)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
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

			arrLayer[EType_Source].DrawTextImage(&CFLPointD(flr.left, (flr.top + flr.bottom) * .5), (wchar_t*)strNumber.GetString(), BLACK, YELLOW, 12, false, 0, EGUIViewImageTextAlignment_CENTER, nullptr, 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false);
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