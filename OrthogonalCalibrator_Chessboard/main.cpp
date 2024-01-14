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

	CResult eResult;

	do
	{ 
		// Learn 이미지 설정 // Learn image settings
		if(IsFail(eResult = sCC.SetLearnImageForCameraCalibration(&fliLearnImage)))
		{
			ErrorPrint(eResult, L"Failed to set image\n");
			break;
		}

		// Calibator할 대상 종류를 설정합니다. // Set the target type for Calibator.
		sCC.SetGridTypeForCameraCalibration(COrthogonalCalibrator::EGridType_ChessBoard);

		// 직교 보정 계산을 할 Learn 이미지 설정 // Learn image settings for orthogonal correction
		if(IsFail(eResult = sCC.SetLearnImageForOrthogonalCorrection(&fliLearnImage)))
		{
			ErrorPrint(eResult, L"Failed to set image\n");
			break;
		}

		// 직교 보정할 대상 종류를 설정합니다. // Set the target type for orthogonal correction.
		sCC.SetGridTypeForOrthogonalCorrection(COrthogonalCalibrator::EGridType_ChessBoard);

		// 결과에 대한 학습률을 설정합니다. // Set the learning rate for the result.
		sCC.SetOptimalSolutionAccuracy();

		// Calibration 실행 // Execute Calibration
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

bool Undistortion(COrthogonalCalibrator& sCC, CFLImage& fliSourceImage, CFLImage& fliDestinationImage, CGUIViewImageWrap& viewImageSource, CGUIViewImageWrap& viewImageDestination)
{
	bool bResult = false;

	CResult eFResult;
	CResult eIResult;
	CResult eResult;

	do
	{
		// Source 이미지 설정 // Set Source image
		if(IsFail(eFResult = sCC.SetSourceImage(&fliSourceImage)))
		{
			ErrorPrint(eResult, L"Failed to Loads image\n");
			break;
		}

		// Destination 이미지 설정 // Set destination image
		if(IsFail(eIResult = sCC.SetDestinationImage(&fliDestinationImage)))
		{
			ErrorPrint(eResult, L"Failed to Loads image\n");
			break;
		}

		// Interpolation 알고리즘 설정 // Set the Interpolation Algorithm
		if(IsFail(eResult = sCC.SetInterpolationMethod(ImageProcessing::EInterpolationMethod_Bilinear)))
		{
			ErrorPrint(eResult, L"Failed to set interpolation method\n");
			break;
		}

		// Undistortion 실행 // Execute Undistortion
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
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliLearnImage, fliSourceImage, fliDestinationImage;

	// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImageLearn, viewImageSource, viewImageDestination;

	// Camera Calibrator 객체 생성 // Create Camera Calibrator object
	COrthogonalCalibrator sCC;
	CResult eResult;

	do
	{
		// Learn 이미지 로드 // Load the Learn image
		if(IsFail(eResult = fliLearnImage.Load(L"../../ExampleImages/OrthogonalCalibrator/Orthogonal_ChessBoard.flif")))
		{
			ErrorPrint(eResult, L"Failed to load the image file.\n");
			break;
		}

		// Learn 이미지 뷰 생성 // Create the Learn image view
		if(IsFail(eResult = viewImageLearn.Create(300, 0, 300 + 480, 360)))
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Learn 이미지 뷰에 이미지를 디스플레이 // Display the image in the Learn image view
		if(IsFail(eResult = viewImageLearn.SetImagePtr(&fliLearnImage)))
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		printf("Processing.....\n");

		if(!Calibration(sCC, fliLearnImage))
			break;

		// Source 이미지 로드 // Load the source image
		if(IsFail(eResult = fliSourceImage.Load(L"../../ExampleImages/OrthogonalCalibrator/Orthogonal_ChessBoard.flif")))
		{
			ErrorPrint(eResult, L"Failed to load the image file.\n");
			break;
		}

		// Destination 이미지 생성 // Create destination image
		if(IsFail(eResult = fliDestinationImage.Create(fliSourceImage.GetWidth(), fliSourceImage.GetHeight(), CMultiVarLL(0), fliSourceImage.GetPixelFormat())))
		{
			ErrorPrint(eResult, L"Failed to create the image file.\n");
			break;
		}

		if(!Undistortion(sCC, fliSourceImage, fliDestinationImage, viewImageSource, viewImageDestination))
			break;

		// 화면에 격자 탐지 결과 출력 // Output the grid detection result on the screen
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

		CFLPointD flpGridPoint1;
		CFLPointD flpGridPoint2;

		for(int64_t i64Row = 0; i64Row < i64GridRow; ++i64Row)
		{
			for(int64_t i64Col = 0; i64Col < i64GridCol - 1; ++i64Col)
			{
				int64_t i64GridIdx = i64Row * i64GridCol + i64Col;
				TPointD* pFlpGridPoint1 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][i64Col]);
				TPointD* pFlpGridPoint2 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][i64Col + 1]);
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
					TPointD* pFlpGridPoint1 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][i64GridCol - 1]);
					TPointD* pFlpGridPoint2 = &(sArrGridDisplay.sGridData.arrGridData[(i64Row + 1)][0]);
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
			TPointD* pFlpGridPoint1 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][0]);
			TPointD* pFlpGridPoint2 = &(sArrGridDisplay.sGridData.arrGridData[i64Row][1]);
			CFLPointD flpGridPoint1(pFlpGridPoint1->x, pFlpGridPoint1->y);
			CFLPointD flpGridPoint2(pFlpGridPoint2->x, pFlpGridPoint2->y);
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

				if(IsFail(eResult = layerLearn.DrawTextImage(pFlpGridPoint1, wstrGridIdx, colorText, BLACK, (int32_t)(f64PointDist / 2), true, f64Angle)))
				{
					ErrorPrint(eResult, L"Failed to draw figure\n");
					break;
				}
			}
		}

		CFLQuadD flqBoardRegion = sArrGridDisplay.sGridData.flqBoardRegion;
		double f64Angle = flqBoardRegion.flpPoints[0].GetAngle(flqBoardRegion.flpPoints[1]);
		CFLString<wchar_t> wstringData = CFLString<wchar_t>().Format(L"[%d] (%d X %d)", (int32_t)sArrGridDisplay.i64ObjectIdx, (int32_t)i64GridCol, (int32_t)i64GridRow);

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

		viewImageLearn.Invalidate();
		
		// Source 이미지 뷰 생성 // Create Source image view
		if(IsFail(eResult = viewImageSource.Create(300, 360, 780, 720)))
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Destination 이미지 뷰 생성 // Creates the Destination image view
		if(IsFail(eResult = viewImageDestination.Create(780, 360, 1260, 720)))
		{
			ErrorPrint(eResult, L"Failed to create the image view.\n");
			break;
		}

		// Source 이미지 뷰에 이미지를 디스플레이 // Display the image in the Source ImageView
		if(IsFail(eResult = viewImageSource.SetImagePtr(&fliSourceImage)))
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		// Destination 이미지 뷰에 이미지를 디스플레이 // Display the image in the Destination image view
		if(IsFail(eResult = viewImageDestination.SetImagePtr(&fliDestinationImage)))
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다. // Synchronize the viewpoints of the two image views.
		if(IsFail(eResult = viewImageLearn.SynchronizePointOfView(&viewImageSource)))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다. // Synchronize the viewpoints of the two image views.
		if(IsFail(eResult = viewImageLearn.SynchronizePointOfView(&viewImageDestination)))
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImageLearn.SynchronizeWindow(&viewImageSource)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(eResult = viewImageLearn.SynchronizeWindow(&viewImageDestination)))
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}
		
		// calibration data 출력 // Output calibration data  // Output calibration data
		CGUIViewImageLayerWrap layerSource = viewImageSource.GetLayer(0);
		COrthogonalCalibrator::SIntrinsicParameters sIntrinsicParam = sCC.GetResultIntrinsicParameters();
		COrthogonalCalibrator::SDistortionCoefficients sDistortCoeef = sCC.GetResultDistortionCoefficients();

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

		// 이미지 뷰 정보 표시 // Display image view information
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

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageSource.IsAvailable() && viewImageDestination.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}