#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	do
	{
		CResult res = EResult_UnknownError;
		// �̹��� �ε� // Load image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Blob/Perforated.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(res = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// Blob ��ü ���� // Create Blob object
		CBlob blob;

		// ROI ���� ����
		CFLRect<int32_t> flrROI(300, 250, 750, 650);

		// ó���� �̹��� ����
		blob.SetSourceImage(fliImage);
		// ó���� ROI ����
		blob.SetSourceROI(flrROI);
		// Threshold ��� ����. ���⼭�� 2�� Threshold�� �ΰ��� ������ And ������ ������ �����Ѵ�.
		blob.SetThresholdMode(EThresholdMode_Dual_And);
		// �� ���� ����
		blob.SetLogicalCondition(ELogicalCondition_Greater, ELogicalCondition_Less);
		// �Ӱ谪 ����,  ���� ���ǰ� �Ʒ��� ������ �������� 127���� ũ�� 240���� ���� ��ü�� ����
		blob.SetThreshold(127, 240);

		// 2�� �Ӱ� �� ������ �Ʒ��� ���� �� ���� ����.
//		blob.SetThreshold(127, EThresholdIndex_First);
// 		blob.SetThreshold(240, EThresholdIndex_Second);

		// ��� ���۳� Contour�� �����ϱ� ���� Perforated ��� ����
		blob.SetContourResultType(CBlob::EContourResultType_Perforated);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = blob.Execute()))
		{
			ErrorPrint(res, "Failed to execute Blob.");
			break;
		}

		// Blob ����� ������ ���� FigureArray ����
		CFLFigureArray flfaContours;

		// Blob ����� �� Contour�� ����
		if(IsFail(res = blob.GetResultContours(flfaContours)))
		{
			ErrorPrint(res, "Failed to get boundary rects from the Blob object.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();


		// ROI������ ������� �˱� ���� ���÷��� �Ѵ� // Display to find out where ROI is
		// FLImaging�� Figure��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ����
		if(IsFail(res = layer.DrawFigureImage(&flrROI, BLUE)))
		{
			ErrorPrint(res, "Failed to draw figures objects on the image view.\n");
			break;
		}

		// flfaContours �� Figure���� �迭�̱� ������ Layer�� �ֱ⸸ �ص� ��� ������� �����ϴ�.
		// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// ���⼭ 0.25�̹Ƿ� ���� ������ ���¶�� �� �� �ִ�.
		// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(res = layer.DrawFigureImage(&flfaContours, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, .2f)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// Contours �������� ���� Ȯ���ϴ� �ڵ�
		for(int64_t i = 0; i < flfaContours.GetCount(); ++i)
		{
			CFLRegion* pFlrg = (CFLRegion*)flfaContours.GetAt(i);

			// �������� ���� ������ �ֿܼ� ���
			printf("No. %lld : [", i);

			for(int64_t j = 0; j < pFlrg->GetCount(); ++j)
			{
				if(j)
					printf(",");

				CFLPoint<double>* pFlpVertex = (CFLPoint<double>*)pFlrg->GetAt(j);

				printf("(%lf,%lf)", pFlpVertex->x, pFlpVertex->y);
			}

			if(pFlrg->GetExclusiveRegion())
			{
				printf("\nExclusive region\n{ ");

				CFLFigureArray* pFlfaExclusive = pFlrg->GetExclusiveRegion();

				for(int64_t j = 0; j < pFlfaExclusive->GetCount(); ++j)
				{
					CFLRegion* pFlrgExclusive = (CFLRegion*)pFlfaExclusive->GetAt(j);

					printf("No. %lld : [", j);

					for(int64_t k = 0; k < pFlrgExclusive->GetCount(); ++k)
					{
						if(k)
							printf(",");

						CFLPoint<double>* pFlpVertex = (CFLPoint<double>*)pFlrg->GetAt(k);

						printf("(%lf,%lf)", pFlpVertex->x, pFlpVertex->y);
					}

					printf("]\n");
				}

				printf(" }\n");
			}

			printf("]\n\n");

			CFLRect<double> flr = pFlrg->GetBoundaryRect();

			CFLString<wchar_t> strNumber;
			strNumber.Format(L"%d", i);

			layer.DrawTextImage(&CFLPoint<double>(flr.left, (flr.top + flr.bottom) * .5), (wchar_t*)strNumber.GetString(), BLACK, YELLOW, 12, false, 0, EGUIViewImageTextAlignment_CENTER, nullptr, 1.f, 1.f, EGUIViewImageFontWeight_BOLD, false);
		}


		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}