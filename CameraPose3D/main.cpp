#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"
#include <map>

int main()
{
	std::map<Foundation::EEulerSequence, std::wstring> mapEulerString;

	mapEulerString[Foundation::EEulerSequence_Extrinsic_XYZ] = L"XYZ";
	mapEulerString[Foundation::EEulerSequence_Extrinsic_XZY] = L"XZY";
	mapEulerString[Foundation::EEulerSequence_Extrinsic_YZX] = L"YZX";
	mapEulerString[Foundation::EEulerSequence_Extrinsic_YXZ] = L"YXZ";
	mapEulerString[Foundation::EEulerSequence_Extrinsic_ZXY] = L"ZXY";
	mapEulerString[Foundation::EEulerSequence_Extrinsic_ZYX] = L"ZYX";
	mapEulerString[Foundation::EEulerSequence_Extrinsic_XYX] = L"XYX";
	mapEulerString[Foundation::EEulerSequence_Extrinsic_XZX] = L"XZX";
	mapEulerString[Foundation::EEulerSequence_Extrinsic_YZY] = L"YZY";
	mapEulerString[Foundation::EEulerSequence_Extrinsic_YXY] = L"YXY";
	mapEulerString[Foundation::EEulerSequence_Extrinsic_ZYZ] = L"ZYZ";
	mapEulerString[Foundation::EEulerSequence_Extrinsic_ZXZ] = L"ZXZ";
	
	CFLImage fliSource;
	CGUIViewImageWrap viewImage;

	do
	{
		// 알고리즘 동작 결과 // Algorithm execution result
		CResult eResult = EResult_UnknownError;

		// 이미지 로드 // Load the image
		if((eResult = fliSource.Load(L"../../ExampleImages/CameraPose3D/ChessBoard(9).flif")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the object file.\n");
			break;
		}
		
		// CameraPose3D 객체 생성 // Create CameraPose3D object
		CCameraPose3D CameraPose3D;

		// Camera Matrix 설정 // Set the camera matrix
		CFLPoint<double> flpFocalLength(617.8218, 618.2815);
		CFLPoint<double> flpPrincipalPoint(319.05237, 243.0472);
		CameraPose3D.SetCameraMatrix(flpPrincipalPoint, flpFocalLength);

		// 셀 간격 설정 // Set the board cell pitch
		CameraPose3D.SetBoardCellPitch(5, 5);

		// 캘리브레이션 객체 타입 설정 // Set the calibration object type
		CameraPose3D.SetCalibrationObjectType(ECalibrationObjectType_ChessBoard);

		// 이미지 전처리 타입 설정 // Set the image preprocessing method
		CameraPose3D.SetPreprocessingMethod(ECalibrationPreprocessingMethod_ShadingCorrection);
		
		const int32_t i32PageCount = fliSource.GetPageCount();

		for(int32_t i = 0; i < i32PageCount; i++)
		{
			// 이미지 뷰 생성 // Create image view
			if(IsFail(eResult = viewImage.Create(100, 0, 612, 512)))
			{
				ErrorPrint(eResult, "Failed to create the Source 3D view.\n");
				break;
			}

			// 페이지 선택
			fliSource.SelectPage(i);

			// 처리할 이미지 설정
			CameraPose3D.SetSourceImage(fliSource);

			// 이미지 포인터 설정 // Set image pointer
			viewImage.SetImagePtr(&fliSource);

			// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
			if((eResult = CameraPose3D.Execute()).IsFail())
			{
				ErrorPrint(eResult, L"Failed to execute Camera Pose 3D.");
				break;
			}

			// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
			// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately		
			CGUIViewImageLayerWrap layerViewSource = viewImage.GetLayer(0);

			// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
			layerViewSource.Clear();

			// View 정보를 디스플레이 한다. // Display view information
			// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
			// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
			// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
			//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
			// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
			//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
			if((eResult = layerViewSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to draw text.\n");
				break;
			}

			// 결과 객체 영역 가져오기 // Get the result board region
			CFLQuad<double> flqBoardRegion;
			CameraPose3D.GetResultBoardRegion(flqBoardRegion);

			// 결과 코너점 가져오기 // Get the result corner points
			CFLFigureArray flfaCornerPoints;
			CameraPose3D.GetResultCornerPoints(flfaCornerPoints);

			// 결과 객체 영역 그리기 // Draw the result board region
			layerViewSource.DrawFigureImage(flqBoardRegion, BLUE, 3);

			// 결과 코너점 그리기 // Draw the result corner points
			flfaCornerPoints.Flatten();

			for(int64_t k = 0; k < flfaCornerPoints.GetCount(); ++k)
				layerViewSource.DrawFigureImage(flfaCornerPoints[k]->GetCenter().MakeCrossHair(5, true), ORANGE, 1);

			CFLString<wchar_t> strDisplay;

			// 결과 가져오기 // Get the results
			CFLArray<double> flaResultRotationVector, flaResultTranslationVector, flaResultEulerAngle;
			CMatrix<double> matResultRotationMatrix;

			// 오일러 각 순서 설정 // Set the euler sequence
			EEulerSequence eEulerSequence = EEulerSequence_Extrinsic_XYZ;

			CameraPose3D.GetResultRotationVector(flaResultRotationVector);
			CameraPose3D.GetResultRotationMatrix(matResultRotationMatrix);
			CameraPose3D.GetResultTranslationVector(flaResultTranslationVector);
			CameraPose3D.GetResultEulerAngle(eEulerSequence, flaResultEulerAngle);

			CFLPoint<double> flpImageSize(fliSource);
			flpImageSize *= 2;

			strDisplay.Format(
				L"Rotation Vector\n"
				L"[%11.6lf]\n"
				L"[%11.6lf]\n"
				L"[%11.6lf]"
				, flaResultRotationVector[0], flaResultRotationVector[1], flaResultRotationVector[2]);
			layerViewSource.DrawTextImage(CFLPoint<double>(), strDisplay, YELLOW, BLACK, 12, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"Courier New");

			strDisplay.Format(
				L"Translation Vector\n"
				L"     [%11.6lf]\n"
				L"     [%11.6lf]\n"
				L"     [%11.6lf]"
				, flaResultTranslationVector[0], flaResultTranslationVector[1], flaResultTranslationVector[2]);

			layerViewSource.DrawTextImage(CFLPoint<double>(flpImageSize.x, 0), strDisplay, YELLOW, BLACK, 12, false, 0, EGUIViewImageTextAlignment_RIGHT_TOP, L"Courier New");

			strDisplay.Format(
				L"                  Rotation Matrix\n"
				L"[%9.6lf, %9.6lf, %9.6lf]\n"
				L"[%9.6lf, %9.6lf, %9.6lf]\n"
				L"[%9.6lf, %9.6lf, %9.6lf]"
				, matResultRotationMatrix[0][0], matResultRotationMatrix[0][1], matResultRotationMatrix[0][2]
				, matResultRotationMatrix[1][0], matResultRotationMatrix[1][1], matResultRotationMatrix[1][2]
				, matResultRotationMatrix[2][0], matResultRotationMatrix[2][1], matResultRotationMatrix[2][2]);

			layerViewSource.DrawTextImage(CFLPoint<double>(flpImageSize.x, flpImageSize.y), strDisplay, YELLOW, BLACK, 12, false, 0, EGUIViewImageTextAlignment_RIGHT_BOTTOM, L"Courier New");

			strDisplay.Format(
				L"Euler Angle (%s)\n"
				L"[%11.6lf]\n"
				L"[%11.6lf]\n"
				L"[%11.6lf]"
				, mapEulerString[eEulerSequence]
				, flaResultEulerAngle[0], flaResultEulerAngle[1], flaResultEulerAngle[2]);

			layerViewSource.DrawTextImage(CFLPoint<double>(0, flpImageSize.y), strDisplay, YELLOW, BLACK, 12, false, 0, EGUIViewImageTextAlignment_LEFT_BOTTOM, L"Courier New");

			if(viewImage.IsAvailable())
				viewImage.Invalidate();

			// 이미지 뷰가 종료될 때 까지 기다림
			while(viewImage.IsAvailable())
				CThreadUtilities::Sleep(1);
		}
	}
	while(false);

	return 0;
}