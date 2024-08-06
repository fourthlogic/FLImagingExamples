#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

struct SGridDisplay
{
	int64_t i64ImageIdx;
	int64_t i64ObjectIdx;
	AdvancedFunctions::COrthogonalCalibrator::SGridResult sGridData;
};
 
bool Calibration(COrthogonalCalibrator& sCC, CFLImage& fliLearnImage)
{
	bool bResult = false;

	CResult res;

	do
	{ 
		// Learn �̹��� ���� // Learn image settings
		if(IsFail(res = sCC.SetLearnImageForCameraCalibration(&fliLearnImage)))
		{
			ErrorPrint(res, L"Failed to set image\n");
			break;
		}

		// Calibator�� ��� ������ �����մϴ�. // Set the target type for Calibator.
		sCC.SetGridTypeForCameraCalibration(COrthogonalCalibrator::EGridType_ChessBoard);

		// ���� ���� ����� �� Learn �̹��� ���� // Learn image settings for orthogonal correction
		if(IsFail(res = sCC.SetLearnImageForOrthogonalCorrection(&fliLearnImage)))
		{
			ErrorPrint(res, L"Failed to set image\n");
			break;
		}

		// ���� ������ ��� ������ �����մϴ�. // Set the target type for orthogonal correction.
		sCC.SetGridTypeForOrthogonalCorrection(COrthogonalCalibrator::EGridType_ChessBoard);

		// ����� ���� �н����� �����մϴ�. // Set the learning rate for the result.
		sCC.SetOptimalSolutionAccuracy();

		// Calibration ���� // Execute Calibration
		if(IsFail(res = sCC.Calibrate()))
		{
			ErrorPrint(res, L"Calibration failed\n");
			break;
		}

		bResult = true;
	}
	while(false);

	return bResult;
}

bool Undistortion(COrthogonalCalibrator& sCC, CFLImage& fliSourceImage, CFLImage& fliDestinationImage, CGUIViewImageWrap& viewImageSource, CGUIViewImageWrap& viewImageDestination)
{
	bool bResult = false;

	CResult eFResult;
	CResult eIResult;
	CResult res;

	do
	{
		// Source �̹��� ���� // Set Source image
		if(IsFail(eFResult = sCC.SetSourceImage(&fliSourceImage)))
		{
			ErrorPrint(res, L"Failed to Loads image\n");
			break;
		}

		// Destination �̹��� ���� // Set destination image
		if(IsFail(eIResult = sCC.SetDestinationImage(&fliDestinationImage)))
		{
			ErrorPrint(res, L"Failed to Loads image\n");
			break;
		}

		// Interpolation �˰��� ���� // Set the Interpolation Algorithm
		if(IsFail(res = sCC.SetInterpolationMethod(ImageProcessing::EInterpolationMethod_Bilinear)))
		{
			ErrorPrint(res, L"Failed to set interpolation method\n");
			break;
		}

		// Undistortion ���� // Execute Undistortion
		if(IsFail(res = sCC.Execute()))
		{
			ErrorPrint(res, L"Undistortion failed\n");
			break;
		}

		bResult = true;
	}
	while(false);

	return bResult;
}

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliLearnImage, fliSourceImage, fliDestinationImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn, viewImageSource, viewImageDestination;

	// Camera Calibrator ��ü ���� // Create Camera Calibrator object
	COrthogonalCalibrator sCC;
	CResult res;

	do
	{
		// Learn �̹��� �ε� // Load the Learn image
		if(IsFail(res = fliLearnImage.Load(L"../../ExampleImages/OrthogonalCalibrator/Orthogonal_ChessBoard.flif")))
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Learn �̹��� �� ���� // Create the Learn image view
		if(IsFail(res = viewImageLearn.Create(300, 0, 300 + 480, 360)))
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Learn �̹��� �信 �̹����� ���÷��� // Display the image in the Learn image view
		if(IsFail(res = viewImageLearn.SetImagePtr(&fliLearnImage)))
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		printf("Processing.....\n");

		if(!Calibration(sCC, fliLearnImage))
			break;

		// Source �̹��� �ε� // Load the source image
		if(IsFail(res = fliSourceImage.Load(L"../../ExampleImages/OrthogonalCalibrator/Orthogonal_ChessBoard.flif")))
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Destination �̹��� ���� // Create destination image
		if(IsFail(res = fliDestinationImage.Create(fliSourceImage.GetWidth(), fliSourceImage.GetHeight(), CMultiVarLL(0), fliSourceImage.GetPixelFormat())))
		{
			ErrorPrint(res, L"Failed to create the image file.\n");
			break;
		}

		if(!Undistortion(sCC, fliSourceImage, fliDestinationImage, viewImageSource, viewImageDestination))
			break;

		// ȭ�鿡 ���� Ž�� ��� ��� // Output the grid detection result on the screen
		SGridDisplay sArrGridDisplay;
		int64_t i64ObjectCount = sCC.GetResultGridPointsObjectCnt(0);

		for(int64_t i64ObjectIdx = 0; i64ObjectIdx < i64ObjectCount; ++i64ObjectIdx)
		{
			SGridDisplay& sGridDisplay = sArrGridDisplay;
			sCC.GetResultGridPoints(i64ObjectIdx, 0, &sGridDisplay.sGridData);
			sGridDisplay.i64ImageIdx = 0;
			sGridDisplay.i64ObjectIdx = sGridDisplay.sGridData.i64ID;
		}

		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);

		layerLearn.Clear();

		uint32_t colorPool[3] = { RED, LIME, BLUE };

		int64_t i64GridRow = sArrGridDisplay.sGridData.i64Rows;
		int64_t i64GridCol = sArrGridDisplay.sGridData.i64Columns;
		int64_t i64ColorPoolIdx = 0;

		CFLPoint<double> flpGridPoint1;
		CFLPoint<double> flpGridPoint2;

		for(int64_t i64Row = 0; i64Row < i64GridRow; ++i64Row)
		{
			for(int64_t i64Col = 0; i64Col < i64GridCol - 1; ++i64Col)
			{
				int64_t i64GridIdx = i64Row * i64GridCol + i64Col;
				TPointD* pFlpGridPoint1 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][i64Col]);
				TPointD* pFlpGridPoint2 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][i64Col + 1]);
				CFLPoint<double> flpGridPoint1(pFlpGridPoint1->x, pFlpGridPoint1->y);
				CFLPoint<double> flpGridPoint2(pFlpGridPoint2->x, pFlpGridPoint2->y);
				CFLLine<double> fllDrawLine(flpGridPoint1, flpGridPoint2);

				if(IsFail(res = layerLearn.DrawFigureImage(fllDrawLine, BLACK, 5)))
				{
					ErrorPrint(res, L"Failed to draw figure\n");
					break;
				}

				if(IsFail(res = layerLearn.DrawFigureImage(fllDrawLine, colorPool[i64GridIdx % 3], 3)))
				{
					ErrorPrint(res, L"Failed to draw figure\n");
					break;
				}


				if(i64Row < i64GridRow - 1)
				{
					TPointD* pFlpGridPoint1 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][i64GridCol - 1]);
					TPointD* pFlpGridPoint2 = &(sArrGridDisplay.sGridData.arrGridData[(i64Row + 1)][0]);
					CFLPoint<double> flpGridPoint1(pFlpGridPoint1->x, pFlpGridPoint1->y);
					CFLPoint<double> flpGridPoint2(pFlpGridPoint2->x, pFlpGridPoint2->y);
					CFLLine<double> fllDrawLine(flpGridPoint1, flpGridPoint2);

					if(IsFail(res = layerLearn.DrawFigureImage(fllDrawLine, BLACK, 5)))
					{
						ErrorPrint(res, L"Failed to draw figure\n");
						break;
					}

					if(IsFail(res = layerLearn.DrawFigureImage(fllDrawLine, YELLOW, 3)))
					{
						ErrorPrint(res, L"Failed to draw figure\n");
						break;
					}
				}
			}
		}

		uint32_t colorText = YELLOW;
		colorPool[2] = CYAN;
		double f64PointDist = 0.;
		double f64Dx = 0.;
		double f64Dy = 0.;

		for(int64_t i64Row = 0; i64Row < i64GridRow; ++i64Row)
		{
			TPointD* pFlpGridPoint1 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][0]);
			TPointD* pFlpGridPoint2 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][1]);
			CFLPoint<double> flpGridPoint1(pFlpGridPoint1->x, pFlpGridPoint1->y);
			CFLPoint<double> flpGridPoint2(pFlpGridPoint2->x, pFlpGridPoint2->y);
			double f64Angle = flpGridPoint1.GetAngle(flpGridPoint2);

			for(int64_t i64Col = 0; i64Col < i64GridCol; ++i64Col)
			{
				int64_t i64GridIdx = i64Row * i64GridCol + i64Col;

				if(i64Col < i64GridCol - 1)
				{
					pFlpGridPoint1 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][i64Col]);
					pFlpGridPoint2 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][i64Col + 1]);

					f64Dx = pFlpGridPoint2->x - pFlpGridPoint1->x;
					f64Dy = pFlpGridPoint2->y - pFlpGridPoint1->y;
					f64PointDist = sqrt(f64Dx * f64Dx + f64Dy * f64Dy);
				}

				if(i64Row)
				{
					pFlpGridPoint1 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][i64Col]);
					pFlpGridPoint2 = &(sArrGridDisplay.sGridData.arrGridData[i64Row - 1][i64Col]);

					f64Dx = pFlpGridPoint2->x - pFlpGridPoint1->x;
					f64Dy = pFlpGridPoint2->y - pFlpGridPoint1->y;
					f64PointDist = std::min<double>(f64PointDist, sqrt(f64Dx * f64Dx + f64Dy * f64Dy));
				}
				else
				{
					pFlpGridPoint1 = &(sArrGridDisplay.sGridData.arrGridData[0][i64Col]);
					pFlpGridPoint2 = &(sArrGridDisplay.sGridData.arrGridData[1][i64Col]);

					f64Dx = pFlpGridPoint2->x - pFlpGridPoint1->x;
					f64Dy = pFlpGridPoint2->y - pFlpGridPoint1->y;
					f64PointDist = std::min<double>(f64PointDist, sqrt(f64Dx * f64Dx + f64Dy * f64Dy));
				}

				CFLString<wchar_t> wstrGridIdx;
				wstrGridIdx.Format(L"%d", i64GridIdx);
				colorText = colorPool[i64GridIdx % 3];

				if(i64Col == i64GridCol - 1)
					colorText = YELLOW;

				if(IsFail(res = layerLearn.DrawTextImage(pFlpGridPoint1, wstrGridIdx, colorText, BLACK, (float)(f64PointDist / 2), true, f64Angle)))
				{
					ErrorPrint(res, L"Failed to draw figure\n");
					break;
				}
			}
		}

		CFLQuad<double> flqBoardRegion = sArrGridDisplay.sGridData.flqBoardRegion;
		double f64Angle = flqBoardRegion.flpPoints[0].GetAngle(flqBoardRegion.flpPoints[1]);
		CFLString<wchar_t> wstringData = CFLString<wchar_t>().Format(L"[%d] (%d X %d)", (int32_t)sArrGridDisplay.i64ObjectIdx, (int32_t)i64GridCol, (int32_t)i64GridRow);

		if(IsFail(res = layerLearn.DrawFigureImage(flqBoardRegion, YELLOW, 3)))
		{
			ErrorPrint(res, L"Failed to draw figure\n");
			break;
		}

		if(IsFail(res = layerLearn.DrawTextImage(flqBoardRegion.flpPoints[0], wstringData, YELLOW, BLACK, 15, false, f64Angle, EGUIViewImageTextAlignment_LEFT_BOTTOM, nullptr, 1.f, 1.f, EGUIViewImageFontWeight_EXTRABOLD)))
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		viewImageLearn.Invalidate();
		
		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(res = viewImageSource.Create(300, 360, 780, 720)))
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Creates the Destination image view
		if(IsFail(res = viewImageDestination.Create(780, 360, 1260, 720)))
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the Source ImageView
		if(IsFail(res = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the Destination image view
		if(IsFail(res = viewImageDestination.SetImagePtr(&fliDestinationImage)))
		{
			ErrorPrint(res, L"Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ�. // Synchronize the viewpoints of the two image views.
		if(IsFail(res = viewImageLearn.SynchronizePointOfView(&viewImageSource)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ�. // Synchronize the viewpoints of the two image views.
		if(IsFail(res = viewImageLearn.SynchronizePointOfView(&viewImageDestination)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImageSource)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImageDestination)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}
		
		// calibration data ��� // Output calibration data  // Output calibration data
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		COrthogonalCalibrator::SIntrinsicParameters sIntrinsicParam = sCC.GetResultIntrinsicParameters();
		COrthogonalCalibrator::SDistortionCoefficients sDistortCoeef = sCC.GetResultDistortionCoefficients();

		CFLString<wchar_t> strMatrix, strDistVal;

		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64FocalLengthX);
		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64Skew);
		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64PrincipalPointX);
		strMatrix.AppendFormat(L"%.2lf, ", 0);
		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64FocalLengthY);
		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64PrincipalPointY);
		strMatrix.AppendFormat(L"%.2lf, ", 0);
		strMatrix.AppendFormat(L"%.2lf, ", 0);
		strMatrix.AppendFormat(L"%.2lf", 1);

		strDistVal.AppendFormat(L"%.2lf, ", sDistortCoeef.f64K1);
		strDistVal.AppendFormat(L"%.2lf, ", sDistortCoeef.f64K2);
		strDistVal.AppendFormat(L"%.2lf, ", sDistortCoeef.f64P1);
		strDistVal.AppendFormat(L"%.2lf, ", sDistortCoeef.f64P2);
		strDistVal.AppendFormat(L"%.2lf", sDistortCoeef.f64K3);

		wprintf(L"Intrinsic parameters : %s\n", strMatrix.GetString());
		wprintf(L"Distortion Coefficients : %s\n", strDistVal.GetString());

		CFLPoint<double> ptIntrTitle(80, 300);
		CFLPoint<double> ptIntr(100, 320);
		CFLPoint<double> ptDistTitle(80, 350);
		CFLPoint<double> ptDist(100, 370);

		// �̹��� �� ���� ǥ�� // Display image view information
		if(IsFail(res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Intrinsic Parameters: ", YELLOW, BLACK, 13)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 20), strMatrix, YELLOW, BLACK, 13)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 40), L"Distortion Coefficients: ", YELLOW, BLACK, 13)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerSource.DrawTextCanvas(&CFLPoint<double>(0, 60), strDistVal, YELLOW, BLACK, 13)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		CGUIViewImageLayerWrap layerDestination = viewImageDestination.GetLayer(0);

		CFLPoint<double> ptTop(20, 20);

		if(IsFail(res = layerDestination.DrawTextImage(&ptTop, L"Undistortion - Bilinear method", GREEN, BLACK, 20)))
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		viewImageSource.Invalidate(true);
		viewImageDestination.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageSource.IsAvailable() && viewImageDestination.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}