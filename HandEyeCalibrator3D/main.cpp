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
	CGUIView3DWrap view3D;
	CGUIViewImageWrap viewImage;

	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult eResult = EResult_UnknownError;

		// �̹��� �ε� // Load the image
		if((eResult = fliSource.Load(L"../../ExampleImages/HandEyeCalibrator3D/ChessBoard.flif")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the image.\n");
			break;
		}

		// HandEyeCalibrator3D ��ü ���� // Create HandEyeCalibrator3D object
		CHandEyeCalibrator3D HandEyeCalibrator3D;

		// ó���� �̹��� ����
		HandEyeCalibrator3D.SetSourceImage(fliSource);

		// ���� ������ ���� �ε� // Load the end effector pose
		if((eResult = HandEyeCalibrator3D.LoadEndEffectorPose(L"../../ExampleImages/HandEyeCalibrator3D/EndEffectorPose.csv")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the file.\n");
			break;
		}

		// Camera Matrix ���� // Set the camera matrix
		CFLPoint<double> flpFocalLength(428.668823242188, 428.268188476563);
		CFLPoint<double> flpPrincipalPoint(422.934997558594, 240.188659667969);

		HandEyeCalibrator3D.SetCalibrationCameraMatrix(flpPrincipalPoint, flpFocalLength);

		// �� ���� ���� // Set the board cell pitch
		HandEyeCalibrator3D.SetCalibrationBoardCellPitch(15, 15);

		// Ķ���극�̼� ��ü Ÿ�� ���� // Set the calibration object type
		HandEyeCalibrator3D.SetCalibrationObjectType(ECalibrationObjectType_ChessBoard);

		// ����ȭ ��� ���� // Set the optimization method
		HandEyeCalibrator3D.SetOptimizationMethod(CHandEyeCalibrator3D::EOptimizationMethod_Nonlinear);

		// ȸ�� Ÿ�� ���� // Set the rotation type
		HandEyeCalibrator3D.SetRotationType(CHandEyeCalibrator3D::ERotationType_Rodrigues);

		// ���� ������ �� ���� ���� // Set the end effector angle unit
		HandEyeCalibrator3D.SetEndEffectorAngleUnit(EAngleUnit_Radian);

		// ���Ϸ� �� ���� ���� // Set the euler sequence
		HandEyeCalibrator3D.SetEulerSequence(EEulerSequence_Extrinsic_XYZ);

		//�ְ� ��� ���� // Set the distortion coefficient
		CFLArray<double> flaDistortionCoefficient;

		flaDistortionCoefficient.PushBack(-0.0538526475429535);
		flaDistortionCoefficient.PushBack(0.0590364411473274);
		flaDistortionCoefficient.PushBack(0.000375126546714455);
		flaDistortionCoefficient.PushBack(0.000785713375080377);
		flaDistortionCoefficient.PushBack(-0.0189481563866138);

		HandEyeCalibrator3D.SetCalibrationDistortionCoefficient(flaDistortionCoefficient);

		const int32_t i32PageCount = fliSource.GetPageCount();

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImage.Create(100, 0, 612, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Source 3D view.\n");
			break;
		}

		// ��� 3D �� ���� // Create result 3D view
		if(IsFail(eResult = view3D.Create(612, 0, 1124, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Source 3D view.\n");
			break;
		}

		// �̹��� ������ ���� // Set image pointer
		viewImage.SetImagePtr(&fliSource);

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately		
		CGUIViewImageLayerWrap layerViewSource = viewImage.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layerViewSource.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((eResult = layerViewSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = HandEyeCalibrator3D.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute Camera Pose 3D.");
			break;
		}

		if(view3D.IsAvailable())
		{
			CGUIView3DLayerWrap view3DLayer = view3D.GetLayer(0);

			CMatrix<double> matResultRotationVector;
			Base::TPoint3<double> tp3ResultTranslationVector;
			CFLArray<double> flaResultEulerAngle;
			CFLString<wchar_t> strDisplay;
			double f64RotationError, f64TranslationError;

			// Ķ���극�̼� ��� ������ // Get the calibration result
			HandEyeCalibrator3D.GetResultRotationVector(matResultRotationVector, false);
			HandEyeCalibrator3D.GetResultTranslationVector(tp3ResultTranslationVector, false);
			HandEyeCalibrator3D.GetResultEulerAngle(flaResultEulerAngle, false);
			HandEyeCalibrator3D.GetResultRotationError(f64RotationError);
			HandEyeCalibrator3D.GetResultTranslationError(f64TranslationError);
			
			// 3D View�� canvas rect ���� ������ // Get the canvas rect region
			CFLRectL flrCanvasRegion = view3D.GetClientRectCanvasRegion();

			CFLPoint<double> flpImageSize(flrCanvasRegion.GetWidth(), flrCanvasRegion.GetHeight());

			strDisplay.Format(
				L"Rotation Vector(End effector to camera)\n"
				L"[%11.6lf]\n"
				L"[%11.6lf]\n"
				L"[%11.6lf]"
				, matResultRotationVector[0][0], matResultRotationVector[1][0], matResultRotationVector[2][0]);

			view3DLayer.DrawTextCanvas(CFLPoint<double>(0, 0), strDisplay, YELLOW, BLACK, 12, false, 0, EGUIViewImageTextAlignment_LEFT_TOP);

			strDisplay.Format(
				L"Translation Vector\n"
				L"     [%11.6lf]\n"
				L"     [%11.6lf]\n"
				L"     [%11.6lf]"
				, tp3ResultTranslationVector.x, tp3ResultTranslationVector.y, tp3ResultTranslationVector.z);

			view3DLayer.DrawTextCanvas(CFLPoint<double>(flpImageSize.x, flpImageSize.y), strDisplay, YELLOW, BLACK, 12, false, 0, EGUIViewImageTextAlignment_RIGHT_BOTTOM);

			strDisplay.Format(
				L"Rotation Error\n"
				L"     [%11.6lf]\n"
				L"Translation Error\n"
				L"     [%11.6lf]"
				, f64RotationError, f64TranslationError);

			view3DLayer.DrawTextCanvas(CFLPoint<double>(flpImageSize.x, 0), strDisplay, YELLOW, BLACK, 12, false, 0, EGUIViewImageTextAlignment_RIGHT_TOP);

			strDisplay.Format(
				L"Euler Angle (%s)\n"
				L"[%11.6lf]\n"
				L"[%11.6lf]\n"
				L"[%11.6lf]"
				, mapEulerString[HandEyeCalibrator3D.GetEulerSequence()].c_str()
				, flaResultEulerAngle[0], flaResultEulerAngle[1], flaResultEulerAngle[2]);

			view3DLayer.DrawTextCanvas(CFLPoint<double>(0, flpImageSize.y), strDisplay, YELLOW, BLACK, 12, false, 0, EGUIViewImageTextAlignment_LEFT_BOTTOM);

			CFL3DObject fl3DOCalibrationBoard;
			Base::TPoint3<double> tp3BoardCenter;

			HandEyeCalibrator3D.GetResultCalibration3DObject(fl3DOCalibrationBoard, tp3BoardCenter);
			CFLString<wchar_t> strIdx;

			strIdx.Format(L"Calibration Board");
			view3DLayer.DrawText3D(tp3BoardCenter, strIdx, RED, 0, 9);
			view3D.PushObject(fl3DOCalibrationBoard);

			for(int32_t i = 0; i < i32PageCount; i++)
			{
				Base::TPoint3<double> tp3RobotCenter, tp3CamCenter;
				CFL3DObject fl3DORobot, fl3DCam;
				CFLString<wchar_t> strIdx;

				Base::TPoint3<float> tp3Cam, tp3Board;

				// ��� 3D ��ü ������ // Get the result 3D object
				HandEyeCalibrator3D.GetEndEffector3DObject(i, &fl3DORobot, &tp3RobotCenter);
				HandEyeCalibrator3D.GetResultCamera3DObject(i, &fl3DCam, &tp3CamCenter);

				// ī�޶� ���� ������ ������ ��� NOK ��� // NOK output if camera pose estimation fails
				if((HandEyeCalibrator3D.GetResultReprojectionPoint(i, tp3Cam, tp3Board)).IsFail())
				{
					strIdx.Format(L"Reprojection(NOK) %d", i);
					view3DLayer.DrawText3D(tp3CamCenter, strIdx, CYAN, 0, 9);
				}
				else
					view3D.PushObject(CGUIView3DObjectLine(tp3Cam, tp3Board, CYAN));
				
				strIdx.Format(L"End Effector %d", i);
				view3DLayer.DrawText3D(tp3RobotCenter, strIdx, BLUE, 0, 9);
				view3D.PushObject(fl3DORobot);

				strIdx.Format(L"Cam %d", i);
				view3DLayer.DrawText3D(tp3CamCenter, strIdx, YELLOW, 0, 9);
				view3D.PushObject(fl3DCam);
			}

			view3D.Invalidate();
			view3D.ZoomFit();
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ�
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}