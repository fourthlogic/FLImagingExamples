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

	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Load the image
		if((eResult = fliSource.Load(L"../../ExampleImages/CameraPose3D/ChessBoard(9).flif")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the image.\n");
			break;
		}
		
		// CameraPose3D ��ü ���� // Create CameraPose3D object
		CCameraPose3D CameraPose3D;

		// Camera Matrix ���� // Set the camera matrix
		CFLPoint<double> flpFocalLength(617.8218, 618.2815);
		CFLPoint<double> flpPrincipalPoint(319.05237, 243.0472);
		CameraPose3D.SetCameraMatrix(flpPrincipalPoint, flpFocalLength);

		// �� ���� ���� // Set the board cell pitch
		CameraPose3D.SetBoardCellPitch(5, 5);

		// Ķ���극�̼� ��ü Ÿ�� ���� // Set the calibration object type
		CameraPose3D.SetCalibrationObjectType(ECalibrationObjectType_ChessBoard);

		// �̹��� ��ó�� Ÿ�� ���� // Set the image preprocessing method
		CameraPose3D.SetPreprocessingMethod(ECalibrationPreprocessingMethod_ShadingCorrection);
		
		const int32_t i32PageCount = fliSource.GetPageCount();

		CGUIViewImageWrap arrViewWrap[9];
		int32_t i32WindowWidth = 300;
		int32_t i32WindowHeight = 300;

		for(int32_t i = 0; i < i32PageCount / 3; ++i)
		{
			int32_t i32Height = i32WindowHeight * i;

			for(int32_t j = 0 ; j < i32PageCount / 3; ++j)
			{
				int32_t i32Width = i32WindowWidth * j;

				arrViewWrap[i * 3 + j].Create(10 + i32Height, i32Width, 10 + i32Height + i32WindowHeight, i32Width + i32WindowWidth);
			}
		}

		for(int32_t i = 1; i < i32PageCount; ++i)
			arrViewWrap[0].SynchronizeWindow(&arrViewWrap[i]);

		CFLImage fliPage[9];

		for(int32_t i = 0; i < i32PageCount; i++)
		{			
			// ������ ����
			fliPage[i] = fliSource.GetPage(i);

			// ó���� �̹��� ����
			CameraPose3D.SetSourceImage(fliPage[i]);

			// �̹��� ������ ���� // Set image pointer
			arrViewWrap[i].SetImagePtr(&fliPage[i]);

			// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
			if((eResult = CameraPose3D.Execute()).IsFail())
			{
				ErrorPrint(eResult, L"Failed to execute Camera Pose 3D.");
				break;
			}

			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately		
			CGUIViewImageLayerWrap layerViewSource = arrViewWrap[i].GetLayer(0);

			// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
			layerViewSource.Clear();

			// View ������ ���÷��� �Ѵ�. // Display view information
			// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
			// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
			// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
			//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
			// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
			//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
			if((eResult = layerViewSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 15)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to draw text.\n");
				break;
			}

			// ��� ��ü ���� �������� // Get the result board region
			CFLQuad<double> flqBoardRegion;
			CameraPose3D.GetResultBoardRegion(flqBoardRegion);

			// ��� �ڳ��� �������� // Get the result corner points
			CFLFigureArray flfaCornerPoints;
			CameraPose3D.GetResultCornerPoints(flfaCornerPoints);

			// ��� ��ü ���� �׸��� // Draw the result board region
			layerViewSource.DrawFigureImage(flqBoardRegion, BLUE, 3);

			// ��� �ڳ��� �׸��� // Draw the result corner points
			flfaCornerPoints.Flatten();

			for(int64_t k = 0; k < flfaCornerPoints.GetCount(); ++k)
				layerViewSource.DrawFigureImage(flfaCornerPoints[k]->GetCenter().MakeCrossHair(5, true), ORANGE, 1);

			CFLString<wchar_t> strDisplay;

			// ��� �������� // Get the results
			CFLArray<double> flaResultRotationVector, flaResultTranslationVector, flaResultEulerAngle;
			CMatrix<double> matResultRotationMatrix;

			// ���Ϸ� �� ���� ���� // Set the euler sequence
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
			layerViewSource.DrawTextImage(CFLPoint<double>(), strDisplay, YELLOW, BLACK, 11, false, 0, EGUIViewImageTextAlignment_LEFT_TOP, L"Courier New");

			strDisplay.Format(
				L"Translation Vector\n"
				L"     [%11.6lf]\n"
				L"     [%11.6lf]\n"
				L"     [%11.6lf]"
				, flaResultTranslationVector[0], flaResultTranslationVector[1], flaResultTranslationVector[2]);

			layerViewSource.DrawTextImage(CFLPoint<double>(flpImageSize.x, 0), strDisplay, YELLOW, BLACK, 11, false, 0, EGUIViewImageTextAlignment_RIGHT_TOP, L"Courier New");

			strDisplay.Format(
				L"                  Rotation Matrix\n"
				L"[%9.6lf, %9.6lf, %9.6lf]\n"
				L"[%9.6lf, %9.6lf, %9.6lf]\n"
				L"[%9.6lf, %9.6lf, %9.6lf]"
				, matResultRotationMatrix[0][0], matResultRotationMatrix[0][1], matResultRotationMatrix[0][2]
				, matResultRotationMatrix[1][0], matResultRotationMatrix[1][1], matResultRotationMatrix[1][2]
				, matResultRotationMatrix[2][0], matResultRotationMatrix[2][1], matResultRotationMatrix[2][2]);

			layerViewSource.DrawTextImage(CFLPoint<double>(flpImageSize.x, flpImageSize.y), strDisplay, YELLOW, BLACK, 11, false, 0, EGUIViewImageTextAlignment_RIGHT_BOTTOM, L"Courier New");

			strDisplay.Format(
				L"Euler Angle (%s)\n"
				L"[%11.6lf]\n"
				L"[%11.6lf]\n"
				L"[%11.6lf]"
				, mapEulerString[eEulerSequence]
				, flaResultEulerAngle[0], flaResultEulerAngle[1], flaResultEulerAngle[2]);

			layerViewSource.DrawTextImage(CFLPoint<double>(0, flpImageSize.y), strDisplay, YELLOW, BLACK, 11, false, 0, EGUIViewImageTextAlignment_LEFT_BOTTOM, L"Courier New");
		}

		if(arrViewWrap[0].IsAvailable())
			arrViewWrap[0].Invalidate();

		// �̹��� �䰡 ����� �� ���� ��ٸ�
		while(arrViewWrap[0].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}