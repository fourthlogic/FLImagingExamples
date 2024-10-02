#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	CFLImage fliSrcImage;
	CFLImage fliCalImage;
	CFLImage fliDstImage;
	CFLImage fliTxtImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageCal;
	CGUIViewImageWrap viewImageTxt;
	CGUIView3DWrap view3DDst;

	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Source �̹��� �ε� // Load the source image
		if((res = fliSrcImage.Load(L"../../ExampleImages/PhotometricStereo3D/Source.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliSrcImage.SelectPage(0);

		// Source �̹��� �� ���� // Create the source image view
		if((res = viewImageSrc.Create(100, 0, 498, 398)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the source image view
		if((res = viewImageSrc.SetImagePtr(&fliSrcImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Calibration �̹��� �ε� // Load the calibration image
		if((res = fliCalImage.Load(L"../../ExampleImages/PhotometricStereo3D/Calibrate.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		fliCalImage.SelectPage(0);

		// Calibration �̹��� �� ���� // Create the calibration image view
		if((res = viewImageCal.Create(498, 0, 896, 398)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Calibration �̹��� �信 �̹����� ���÷��� // Display the image in the calibration image view
		if((res = viewImageCal.SetImagePtr(&fliCalImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create the destination image view
		if((res = viewImageTxt.Create(100, 398, 498, 796)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the destination image view
		if((res = viewImageTxt.SetImagePtr(&fliTxtImage)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination 3D �̹��� �� ���� // Create the destination 3D image view
		if((res = view3DDst.Create(896, 0, 1692, 769)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageCal)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&viewImageTxt)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if((res = viewImageSrc.SynchronizeWindow(&view3DDst)).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		viewImageSrc.SetFixThumbnailView(true);

		// PhotometricStereo ��ü ���� // Create PhotometricStereo object
		CPhotometricStereo3D photometric;

		CFL3DObjectHeightMap fl3DOHM;

		// Source �̹��� ���� // Set the source image
		photometric.SetSourceImage(fliSrcImage);
		// Calibration �̹��� ���� // Set the calibration image
		photometric.SetCalibrationImage(fliCalImage);
		// Destination �̹��� ���� // Set the destination image
		photometric.SetDestinationImage(fliDstImage);
		// ��� Texture �̹��� ���� // Set the result texture image
		photometric.SetResultTextureImage(fliTxtImage);
		// Destionation 3D Object ���� // Set the destionation 3D object 
		photometric.SetDestinationObject(fl3DOHM);
		// Calibration ������ ���� // Set Calibration Settings
		photometric.SetCalibrationCircleROI(CFLCircle<double>(386.439657, 346.491239, 259.998140, 0.000000, 0.000000, 360.000000, EArcClosingMethod_EachOther));
		// ���� ��� ���� // Set operation mode
		photometric.SetReconstructionMode(CPhotometricStereo3D::EReconstructionMode_RFT_FP32);

		CMatrix<double> cmatdTemp(3, 3);

		// Angle Degrees ���� ������� ���� // Set operation method as angle degrees
		photometric.SetLightAngleDegrees(cmatdTemp);

		// �˰��� Calibration ���� // Execute calibration of the algorithm
		if((res = photometric.Calibrate()).IsFail())
		{
			ErrorPrint(res, "Failed to calibrate algorithm.\n");
			break;
		}

		// Calibrate �� Angle Degree ������ ���� // Save calibrated angle degree data
		CMultiVar<double> cmvdSlant;
		CMultiVar<double> cmvdTilt;

		photometric.GetLightAngleDegrees(cmvdSlant, cmvdTilt);

		// ��ġ ������ ���� ������� ���� // Set operation method as positions
		photometric.SetLightPositions(cmatdTemp);

		// �˰��� Calibration ���� // Execute calibration of the algorithm
		if((res = photometric.Calibrate()).IsFail())
		{
			ErrorPrint(res, "Failed to calibrate algorithm.\n");
			break;
		}

		// Calibrate �� ��ġ ������ ���� // Save calibrated position data
		CMatrix<double> cmatdPosition;

		photometric.GetLightPositions(cmatdPosition);

		// Calibrate�� ������ ����� Consoleâ�� ����մϴ�. // Output the calibration result to the console window.
		int32_t i32CalibPageNum = fliCalImage.GetPageCount();

		printf(" < Calibration Angle - Degrees >\n");

		for(int i = 0; i < i32CalibPageNum; i++)
			printf("Image %d ->\tSlant: %0.7lf\tTilt: %0.7lf\n", i, cmvdSlant.GetAt(i), cmvdTilt.GetAt(i));

		printf("\n");

		// �������� ���� ���� ����� Consoleâ�� ����մϴ�. // Output the geometric pattern detection result to the console window.
		printf(" < Calibration Angle - Degrees >\n");

		for(int i = 0; i < i32CalibPageNum; i++)
			printf("Image %d ->\tX: %0.7lf\tY: %0.7lf \tZ: %0.7lf\n", i, *cmatdPosition.GetValue(i, 0), *cmatdPosition.GetValue(i, 1), *cmatdPosition.GetValue(i, 2));

		// �˰��� ���� // Execute algorithm
		if((res = photometric.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to calibrate algorithm.\n");
			break;
		}

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((res = viewImageTxt.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit of the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerCal = viewImageCal.GetLayer(0);
		CGUIViewImageLayerWrap layerTxt = viewImageTxt.GetLayer(0);
		CGUIView3DLayerWrap layer3D = view3DDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerSrc.Clear();
		layerCal.Clear();
		layerTxt.Clear();
		layer3D.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = layerSrc.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		if((res = layerCal.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Calibration Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		if((res = layerTxt.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Texture Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		// 3D View ���� ���ʷ��� // Display 3D view information
		float f32CenterX = (float)fliSrcImage.GetWidth() / 2;
		float f32CenterY = (float)fliSrcImage.GetHeight() / 2;
		float f32CenterZ = (float)fliDstImage.GetBuffer()[(long)(f32CenterY * fliSrcImage.GetWidth() + f32CenterX)];

		TPoint3<float> tp3dFrom(f32CenterX, f32CenterY, f32CenterZ);

		float f32MulNum = 2000.;

		for(long i = 0; i < i32CalibPageNum; i++)
		{
			CFLString<wchar_t> strText;

			strText.Format(L"X: %.4lf    \nY: %.4lf    \nZ: %.4lf\n", *cmatdPosition.GetValue(i, 0), *cmatdPosition.GetValue(i, 1), *cmatdPosition.GetValue(i, 2));

			TPoint3<float> tp3dTo(f32MulNum * (float)*cmatdPosition.GetValue(i, 0) + f32CenterX, f32MulNum * (float)*cmatdPosition.GetValue(i, 1) + f32CenterY, f32MulNum * (float)*cmatdPosition.GetValue(i, 2) + f32CenterZ);

			TPoint3<double> tp3dTod(f32MulNum * *cmatdPosition.GetValue(i, 0) + f32CenterX, f32MulNum * *cmatdPosition.GetValue(i, 1) + f32CenterY, f32MulNum * *cmatdPosition.GetValue(i, 2) + f32CenterZ);

			CGUIView3DObjectLine cgui3dlineTemp(tp3dFrom, tp3dTo, YELLOW, 1);

			layer3D.DrawText3D(tp3dTod, strText, BLACK, YELLOW);
			view3DDst.PushObject(cgui3dlineTemp);
		}

		// Height Map�� Texture Image ����
		CFL3DObjectHeightMap* pFl3DOHM = dynamic_cast<CFL3DObjectHeightMap*>(photometric.GetDestinationObject());
		pFl3DOHM->SetTextureImage(fliTxtImage);
		pFl3DOHM->ActivateVertexColorTexture(true);

		// 3D �̹��� �信 Height Map (Destination Image) �̹����� ���÷��� // Display the Height Map (Destination Image) on the 3D image view
		if((res = view3DDst.PushObject(*pFl3DOHM)).IsFail())
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		view3DDst.ZoomFit();

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImageSrc.Invalidate(true);
		viewImageTxt.Invalidate(true);
		viewImageCal.Invalidate(true);
		view3DDst.Invalidate(true);

		// �̹��� ��, 3D �䰡 ����� �� ���� ��ٸ� // Wait for the image and 3D view to close
		while(viewImageSrc.IsAvailable() && viewImageTxt.IsAvailable() && viewImageCal.IsAvailable() && view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}