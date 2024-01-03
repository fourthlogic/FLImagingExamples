#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

struct SGridDisplay
{
	int64_t i64ImageIdx;
	int64_t i64ObjectIdx;
	AdvancedFunctions::CCameraCalibrator::SGridResult sGridData;
};
 
bool Calibration(CCameraCalibrator& sCC, CFLImage& fliLearnImage)
{
	bool bResult = false;

	CResult eResult;

	do
	{
		// Learn �̹��� ���� // Learn image settings
		if(IsFail(eResult = sCC.SetLearnImageForCameraCalibration(&fliLearnImage)))
		{
			ErrorPrint(eResult, L"Failed to set image\n");
			break;
		}

		// Calibator�� ��� ������ �����մϴ�. // Set the target type for Calibator.
		sCC.SetGridType(CCameraCalibrator::EGridType_ChessBoard);
		// ����� ���� �н����� �����մϴ�. // Set the learning rate for the result.
		sCC.SetOptimalSolutionAccuracy();

		// Calibration ���� // Execute Calibration
		if(IsFail(eResult = sCC.Calibrate()))
		{
			ErrorPrint(eResult, L"Calibration failed\n");
			break;
		}

		bResult = true;
	}
	while(false);

	return bResult;
}

bool Undistortion(CCameraCalibrator& sCC, CFLImage& fliSourceImage, CFLImage& fliDestinationImage, CGUIViewImageWrap& viewImageSource, CGUIViewImageWrap& viewImageDestination)
{
	bool bResult = false;

	CResult eResult;

	do
	{
		// Source �̹��� ���� // Set Source image
		if(IsFail(eResult = sCC.SetSourceImage(&fliSourceImage)))
		{
			ErrorPrint(eResult, L"Failed to Loads image\n");
			break;
		}

		// Destination �̹��� ���� // Set destination image
		if(IsFail(eResult = sCC.SetDestinationImage(&fliDestinationImage)))
		{
			ErrorPrint(eResult, L"Failed to Loads image\n");
			break;
		}

		// Interpolation �˰��� ���� // Set the Interpolation Algorithm
		if(IsFail(eResult = sCC.SetInterpolationMethod(ImageProcessing::EInterpolationMethod_Bilinear)))
		{
			ErrorPrint(eResult, L"Failed to set interpolation method\n");
			break;
		}
		 
		// Undistortion ���� // Execute Undistortion
		if(IsFail(eResult = sCC.Execute()))
		{
			ErrorPrint(eResult, L"Undistortion failed\n");
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
	CFLImage fliDisplay;
	CFLImage arrFliDisplay[3];

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn[3], viewImageSource, viewImageDestination;

	// Camera Calibrator ��ü ���� // Create Camera Calibrator object
	CCameraCalibrator sCC;
	CResult eResult;

	do
	{
		// Learn �̹��� �ε� // Load the Learn image
		if(IsFail(eResult = fliLearnImage.Load(L"../../ExampleImages/CameraCalibrator/ChessBoard/Multiple ChessBoard.flif")))
		{
			ErrorPrint(eResult, L"Failed to load the image file.\n");
			break;
		}

		printf("Processing.....\n");
		fliDisplay = fliLearnImage;
		fliDisplay.SwapPage(&arrFliDisplay[0], 0);
		fliDisplay.SwapPage(&arrFliDisplay[1], 1);
		fliDisplay.SwapPage(&arrFliDisplay[2], 2);

		for(int32_t i = 0; i < 3; ++i)
		{
			// Learn �̹��� �� ���� // Create the Learn image view
			if(IsFail(eResult = viewImageLearn[i].Create(300 + 480 * i, 0, 300 + 480 * (i + 1), 360)))
			{
				ErrorPrint(eResult, L"Failed to create the image view.\n");
				break;
			}

			// Learn �̹��� �信 �̹����� ���÷��� // Display the image in the Learn image view
			if(IsFail(eResult = viewImageLearn[i].SetImagePtr(&arrFliDisplay[i])))
			{
				ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
				break;
			}
		}

		if(!Calibration(sCC, fliLearnImage))
			break;

		// Source �̹��� �ε� // Load the source image
		if(IsFail(eResult = fliSourceImage.Load(L"../../ExampleImages/CameraCalibrator/ChessBoard/UndistortionImage/Undistortion_Image.flif")))
		{
			ErrorPrint(eResult, L"Failed to load the image file.\n");
			break;
		}

		if(!Undistortion(sCC, fliSourceImage, fliDestinationImage, viewImageSource, viewImageDestination))
			break;

		// ȭ�鿡 ���� Ž�� ��� ��� // Output the grid detection result on the screen
		SGridDisplay sArrGridDisplay[3];

		for(int64_t i64ImgIdx = 0; i64ImgIdx < (int64_t)fliLearnImage.GetPageCount(); ++i64ImgIdx)
		{
			int64_t i64ObjectCount = sCC.GetResultGridPointsObjectCnt(i64ImgIdx);

			for(int64_t i64ObjectIdx = 0; i64ObjectIdx < i64ObjectCount; ++i64ObjectIdx)
			{
				SGridDisplay& sGridDisplay = sArrGridDisplay[i64ImgIdx];
				sCC.GetResultGridPoints(i64ObjectIdx, i64ImgIdx, &sGridDisplay.sGridData);
				sGridDisplay.i64ImageIdx = i64ImgIdx;
				sGridDisplay.i64ObjectIdx = sGridDisplay.sGridData.i64ID;
			}
		}

		for(int32_t i = 0; i < 3; ++i)
		{
			CGUIViewImageLayerWrap layerLearn = viewImageLearn[i].GetLayer(0);

			layerLearn.Clear();

			uint32_t colorPool[3] = { RED, LIME, BLUE };

			int64_t i64GridRow = sArrGridDisplay[i].sGridData.i64Rows;
			int64_t i64GridCol = sArrGridDisplay[i].sGridData.i64Columns;
			int64_t i64ColorPoolIdx = 0;

			CFLPointD flpGridPoint1;
			CFLPointD flpGridPoint2;

			for(int64_t i64Row = 0; i64Row < i64GridRow; ++i64Row)
			{
				for(int64_t i64Col = 0; i64Col < i64GridCol - 1; ++i64Col)
				{
					int64_t i64GridIdx = i64Row * i64GridCol + i64Col;
					TPointD* pFlpGridPoint1 = &(sArrGridDisplay[i].sGridData.arrGridData[i64Row][i64Col]);
					TPointD* pFlpGridPoint2 = &(sArrGridDisplay[i].sGridData.arrGridData[i64Row][i64Col + 1]);
					CFLPointD flpGridPoint1(pFlpGridPoint1->x, pFlpGridPoint1->y);
					CFLPointD flpGridPoint2(pFlpGridPoint2->x, pFlpGridPoint2->y);
					CFLLineD fllDrawLine(flpGridPoint1, flpGridPoint2);

					if(IsFail(eResult = layerLearn.DrawFigureImage(fllDrawLine, BLACK, 5)))
					{
						ErrorPrint(eResult, L"Failed to draw figure\n");
						break;
					}

					if(IsFail(eResult = layerLearn.DrawFigureImage(fllDrawLine, colorPool[i64GridIdx % 3], 3)))
					{
						ErrorPrint(eResult, L"Failed to draw figure\n");
						break;
					}


					if(i64Row < i64GridRow - 1)
					{
						TPointD* pFlpGridPoint1 = &(sArrGridDisplay[i].sGridData.arrGridData[i64Row][i64GridCol - 1]);
						TPointD* pFlpGridPoint2 = &(sArrGridDisplay[i].sGridData.arrGridData[i64Row + 1][0]);
						CFLPointD flpGridPoint1(pFlpGridPoint1->x, pFlpGridPoint1->y);
						CFLPointD flpGridPoint2(pFlpGridPoint2->x, pFlpGridPoint2->y);
						CFLLineD fllDrawLine(flpGridPoint1, flpGridPoint2);

						if(IsFail(eResult = layerLearn.DrawFigureImage(fllDrawLine, BLACK, 5)))
						{
							ErrorPrint(eResult, L"Failed to draw figure\n");
							break;
						}

						if(IsFail(eResult = layerLearn.DrawFigureImage(fllDrawLine, YELLOW, 3)))
						{
							ErrorPrint(eResult, L"Failed to draw figure\n");
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
				TPointD* pFlpGridPoint1 = &(sArrGridDisplay[i].sGridData.arrGridData[i64Row][0]);
				TPointD* pFlpGridPoint2 = &(sArrGridDisplay[i].sGridData.arrGridData[i64Row][1]);
				CFLPointD flpGridPoint1(pFlpGridPoint1->x, pFlpGridPoint1->y);
				CFLPointD flpGridPoint2(pFlpGridPoint2->x, pFlpGridPoint2->y);
				double f64Angle = flpGridPoint1.GetAngle(flpGridPoint2);

				for(int64_t i64Col = 0; i64Col < i64GridCol; ++i64Col)
				{
					int64_t i64GridIdx = i64Row * i64GridCol + i64Col;

					if(i64Col < i64GridCol - 1)
					{
						pFlpGridPoint1 = &(sArrGridDisplay[i].sGridData.arrGridData[i64Row][i64Col]);
						pFlpGridPoint2 = &(sArrGridDisplay[i].sGridData.arrGridData[i64Row][i64Col + 1]);

						f64Dx = pFlpGridPoint2->x - pFlpGridPoint1->x;
						f64Dy = pFlpGridPoint2->y - pFlpGridPoint1->y;
						f64PointDist = sqrt(f64Dx * f64Dx + f64Dy * f64Dy);
					}

					if(i64Row)
					{
						pFlpGridPoint1 = &(sArrGridDisplay[i].sGridData.arrGridData[i64Row][i64Col]);
						pFlpGridPoint2 = &(sArrGridDisplay[i].sGridData.arrGridData[i64Row - 1][i64Col]);

						f64Dx = pFlpGridPoint2->x - pFlpGridPoint1->x;
						f64Dy = pFlpGridPoint2->y - pFlpGridPoint1->y;
						f64PointDist = std::min<double>(f64PointDist, sqrt(f64Dx * f64Dx + f64Dy * f64Dy));
					}
					else
					{
						pFlpGridPoint1 = &(sArrGridDisplay[i].sGridData.arrGridData[0][i64Col]);
						pFlpGridPoint2 = &(sArrGridDisplay[i].sGridData.arrGridData[1][i64Col]);

						f64Dx = pFlpGridPoint2->x - pFlpGridPoint1->x;
						f64Dy = pFlpGridPoint2->y - pFlpGridPoint1->y;
						f64PointDist = std::min<double>(f64PointDist, sqrt(f64Dx * f64Dx + f64Dy * f64Dy));
					}

					CFLString<wchar_t> wstrGridIdx;
					wstrGridIdx.Format(L"%d", i64GridIdx);
					colorText = colorPool[i64GridIdx % 3];

					if(i64Col == i64GridCol - 1)
						colorText = YELLOW;

					if(IsFail(eResult = layerLearn.DrawTextImage(pFlpGridPoint1, wstrGridIdx, colorText, BLACK, (int32_t)(f64PointDist / 2), true, f64Angle)))
					{
						ErrorPrint(eResult, L"Failed to draw figure\n");
						break;
					}
				}
			}

			CFLQuadD flqBoardRegion = sArrGridDisplay[i].sGridData.flqBoardRegion;
			double f64Angle = flqBoardRegion.flpPoints[0].GetAngle(flqBoardRegion.flpPoints[1]);
			CFLString<wchar_t> wstringData = CFLString<wchar_t>().Format(L"[%d] (%d X %d)", (int32_t)sArrGridDisplay[i].i64ObjectIdx, (int32_t)i64GridCol, (int32_t)i64GridRow);

			if(IsFail(eResult = layerLearn.DrawFigureImage(flqBoardRegion, YELLOW, 3)))
			{
				ErrorPrint(eResult, L"Failed to draw figure\n");
				break;
			}

			if(IsFail(eResult = layerLearn.DrawTextImage(flqBoardRegion.flpPoints[0], wstringData, YELLOW, BLACK, 15, false, f64Angle, EGUIViewImageTextAlignment_LEFT_BOTTOM, nullptr, 1.f, 1.f, EGUIViewImageFontWeight_EXTRABOLD)))
			{
				ErrorPrint(eResult, L"Failed to draw text\n");
				break;
			}

			viewImageLearn[i].Invalidate();
		}

		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(eResult = viewImageSource.Create(300, 360, 780, 720)))
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Creates the Destination image view
		if(IsFail(eResult = viewImageDestination.Create(780, 360, 1260, 720)))
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Source �̹��� �信 �̹����� ���÷��� // Display the image in the Source ImageView
		if(IsFail(eResult = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination �̹��� �信 �̹����� ���÷��� // Display the image in the Destination image view
		if(IsFail(eResult = viewImageDestination.SetImagePtr(&fliDestinationImage)))
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		for(int32_t i = 0; i < 3; ++i)
		{
			// �� �̹��� ���� ������ ����ȭ �Ѵ�. // Synchronize the viewpoints of the two image views.
			if(IsFail(eResult = viewImageLearn[i].SynchronizePointOfView(&viewImageSource)))
			{
				ErrorPrint(eResult, "Failed to synchronize view\n");
				break;
			}

			// �� �̹��� ���� ������ ����ȭ �Ѵ�. // Synchronize the viewpoints of the two image views.
			if(IsFail(eResult = viewImageLearn[i].SynchronizePointOfView(&viewImageDestination)))
			{
				ErrorPrint(eResult, "Failed to synchronize view\n");
				break;
			}

			// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
			if(IsFail(eResult = viewImageLearn[i].SynchronizeWindow(&viewImageSource)))
			{
				ErrorPrint(eResult, "Failed to synchronize window.\n");
				break;
			}

			// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
			if(IsFail(eResult = viewImageLearn[i].SynchronizeWindow(&viewImageDestination)))
			{
				ErrorPrint(eResult, "Failed to synchronize window.\n");
				break;
			}
		}

		// calibration data ��� // Output calibration data  // Output calibration data
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		CCameraCalibrator::SIntrinsicParameters sIntrinsicParam = sCC.GetResultIntrinsicParameters();
		CCameraCalibrator::SDistortionCoefficients sDistortCoeef = sCC.GetResultDistortionCoefficients();

		CFLString<wchar_t> strMatrix, strDistVal;

		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64FocalLengthX);
		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64Skew);
		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64PrincipalPointX);
		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64Padding1);
		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64FocalLengthY);
		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64PrincipalPointY);
		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64Padding2);
		strMatrix.AppendFormat(L"%.2lf, ", sIntrinsicParam.f64Padding3);
		strMatrix.AppendFormat(L"%.2lf", sIntrinsicParam.f64Padding4);

		strDistVal.AppendFormat(L"%.2lf, ", sDistortCoeef.f64F1);
		strDistVal.AppendFormat(L"%.2lf, ", sDistortCoeef.f64F2);
		strDistVal.AppendFormat(L"%.2lf, ", sDistortCoeef.f64P1);
		strDistVal.AppendFormat(L"%.2lf, ", sDistortCoeef.f64P2);
		strDistVal.AppendFormat(L"%.2lf", sDistortCoeef.f64F3);

		wprintf(L"Intrinsic parameters : %s\n", strMatrix.GetString());
		wprintf(L"Distortion Coefficients : %s\n", strDistVal.GetString());

		CFLPointD ptIntrTitle(80, 300);
		CFLPointD ptIntr(100, 320);
		CFLPointD ptDistTitle(80, 350);
		CFLPointD ptDist(100, 370);

		// �̹��� �� ���� ǥ�� // Display image view information
		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 0), L"Intrinsic Parameters: ", YELLOW, BLACK, 13)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 20), strMatrix, YELLOW, BLACK, 13)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 40), L"Distortion Coefficients: ", YELLOW, BLACK, 13)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(0, 60), strDistVal, YELLOW, BLACK, 13)))
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		CGUIViewImageLayerWrap layerDestination = viewImageDestination.GetLayer(0);

		CFLPointD ptTop(20, 20);

		if(IsFail(eResult = layerDestination.DrawTextImage(&ptTop, L"Undistortion - Bilinear method", GREEN, BLACK, 20)))
		{
			ErrorPrint(eResult, L"Failed to draw text\n");
			break;
		}

		viewImageSource.Invalidate(true);
		viewImageDestination.Invalidate(true);

	    // �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn[0].IsAvailable() && viewImageLearn[1].IsAvailable() && viewImageLearn[2].IsAvailable() && viewImageSource.IsAvailable() && viewImageDestination.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}