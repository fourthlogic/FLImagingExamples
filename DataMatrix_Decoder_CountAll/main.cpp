#include <cstdio>
#include <time.h>

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
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/DataMatrix/Tray.flif")))
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

		// Data Matrix Decoder ��ü ���� // Create Data Matrix Decoder object
		CDataMatrixDecoder dataMatrixDecoder;

		// ó���� �̹��� ����
		dataMatrixDecoder.SetSourceImage(fliImage);
		// �˻� ���� ��ü�� ����
		dataMatrixDecoder.SetDetectingCount(EDataCodeDecoderDetectingCount_All);
		// Decode ������ ���� ���� ����
		// EDataCodeColor_Auto �� ���� �� �ڵ����� Decode �ȴ�.
		dataMatrixDecoder.SetColorMode(EDataCodeColor_Auto);
		// ��ó�� Blur ����
		// EDataCodeDecoderImageProcessingMethod_Normal �Է� �� ��ó���� ���� �ʴ´�.
		dataMatrixDecoder.SetDenoisingMethod(EDataCodeDecoderDenoisingMethod_Gaussian);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if(IsFail(res = dataMatrixDecoder.Execute()))
		{
			ErrorPrint(res, "Failed to execute data matrix decoder.");

			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer.Clear();

		// Data Matrix Decoder ��� ������ ��´�.
		int64_t i64Results = dataMatrixDecoder.GetResultCount();

		srand((unsigned int)time(nullptr));

		for(int64_t i = 0; i < i64Results; ++i)
		{
			// Data Matrix Decoder ����� ������ ���� FLQuadD ����
			CFLQuad<double> flqdRegion;

			// Data Matrix Decoder ����� �� Data Region �� ����
			if(IsFail(res = dataMatrixDecoder.GetResultDataRegion(i, flqdRegion)))
			{
				ErrorPrint(res, "Failed to get data region from the data matrix decoder object.");
				continue;
			}

			// Data Matrix �� ������ ���÷��� �Ѵ�.
			// FLImaging�� Figure ��ü���� � ��������̵� ������� �ϳ��� �Լ��� ���÷��̰� ���� // FLimaging's Figure objects can be displayed as a function regardless of the shape
			// �Ʒ� �Լ� DrawFigureImage�� Image��ǥ�� �������� �ϴ� Figure�� Drawing �Ѵٴ� ���� �ǹ��ϸ� // The function DrawFigureImage below means drawing a picture based on the image coordinates
			// �� ������ �ΰ��� �Ķ���ʹ� ������ ���̰� 1�ϰ�� ������, 0�ϰ�� ���� ������ �ǹ��Ѵ�. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
			// �Ķ���� ���� : ���̾� -> Figure ��ü -> �� �� -> �� �β� -> �� �� -> �� ��Ÿ�� -> �� ���İ�(������) -> �� ���İ� (������) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
			if(IsFail(res = layer.DrawFigureImage(&flqdRegion, LIME, 2)))
			{
				ErrorPrint(res, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// Data Matrix Decoder ����� ������ ���� FigureArray ����
			CFLFigureArray flfaGridRegion;

			// Data Matrix Decoder ����� �� Grid Region �� ����
			if(IsFail(res = dataMatrixDecoder.GetResultGridRegion(i, flfaGridRegion)))
			{
				ErrorPrint(res, "Failed to get grid region from the data matrix decoder object.");
				continue;
			}

			// Data Matrix �� Grid Region �� ���÷��� �Ѵ�.
			if(IsFail(res = layer.DrawFigureImage(&flfaGridRegion, COLOR(rand() % 256, rand() % 256, rand() % 256), 2)))
			{
				ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
				continue;
			}

			// Data Matrix Decoder ����� ������ ���� FLStringW ����
			CFLString<wchar_t> flstrDecoded;

			// Data Matrix Decoder ����� �� Decoded String �� ����
			if(IsFail(res = dataMatrixDecoder.GetResultDecodedString(i, flstrDecoded)))
			{
				ErrorPrint(res, "Failed to get decoded string from the data matrix decoder object.");
				continue;
			}

			wprintf(L"No. %lld Code : ", i);
			wprintf(L"%s\n", flstrDecoded.GetString());

			// String �� ���÷��� �ϱ� ���� ���� ��ǥ FLPointL ����
			CFLPoint<int32_t> flplPos(flqdRegion.flpPoints[3]);

			// Decoded String �� ���÷��� �Ѵ�.
			// �Ʒ� �Լ� DrawTextImage�� Image��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�.
			// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
			// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
			//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
			// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
			//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
			if(IsFail(res = layer.DrawTextImage(&flplPos, flstrDecoded, CYAN, BLACK, 12, false, flqdRegion.flpPoints[3].GetAngle(flqdRegion.flpPoints[2]))))
			{
				ErrorPrint(res, "Failed to draw string object on the image view.\n");
				continue;
			}
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