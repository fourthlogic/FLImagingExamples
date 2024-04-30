#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 3D ��ü ���� // Declare 3D object
	CFL3DObject fl3DOLearnObject;
	CFL3DObject fl3DOSourceObject;

	// 3D �� ���� // Declare 3D view	
	CGUIView3DWrap view3DDst;
	CGUIView3DWrap view3DLearn;
	CGUIView3DWrap view3DSource;


	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult eResult = EResult_UnknownError;


		// Learn Object �ε� // Load the learn object
		if((eResult = fl3DOLearnObject.Load(L"../../ExampleImages/SurfaceBasedMatch3D/ReferencePoints.ply")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the object file.\n");
			break;
		}

		// Source Object �ε� // Load the Source object
		if((eResult = fl3DOSourceObject.Load(L"../../ExampleImages/SurfaceBasedMatch3D/MeasuredPoints.ply")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the object file.\n");
			break;
		}

		// Learn 3D �� ����
		if(IsFail(eResult = view3DLearn.Create(100, 0, 612, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Source 3D view.\n");
			break;
		}

		// Source 3D �� ����
		if(IsFail(eResult = view3DSource.Create(612, 0, 1124, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Learn 3D view.\n");
			break;
		}

		// Dst 3D �� ����
		if(IsFail(eResult = view3DDst.Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Dst 3D view.\n");
			break;
		}

		// Learn Object 3D �� ���� // Create the learn object 3D view
		if((eResult = view3DLearn.PushObject(fl3DOLearnObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to display the 3D object.\n");
			break;
		}

		// Source Object 3D �� ���� // Create the source object 3D view
		if((eResult = view3DSource.PushObject(fl3DOSourceObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to display the 3D object.\n");
			break;
		}

		// SurfaceBasedMatch3D ��ü ���� // Create SurfaceBasedMatch3D object
		CSurfaceBasedMatch3D SurfaceBasedMatch3D;

		// Learn object ���� // Set the learn object
		SurfaceBasedMatch3D.CAlgorithmFeature_3D_LO_SO::SetLearnObject(fl3DOLearnObject);
		// Source object ���� // Set the source object
		SurfaceBasedMatch3D.CAlgorithmFeature_3D_LO_SO::SetSourceObject(fl3DOSourceObject);
		// ������Ʈ Ÿ�� ���� // Set the type of the object
		SurfaceBasedMatch3D.SetObjectType(CSurfaceBasedMatch3D::EObjectType_Object3D);
		// Min score ���� // Set the min score
		SurfaceBasedMatch3D.SetMinScore(0.1);
		// �ִ� ��� ���� ���� // Set the max count of match result
		SurfaceBasedMatch3D.SetMaxObject(1);
		// �н� ���ø� �Ÿ� ���� // Set the learn sampling distance
		SurfaceBasedMatch3D.SetLearnSamplingDistance(0.03);
		// ��� ���ø� �Ÿ� ���� // Set the scene sampling distance
		SurfaceBasedMatch3D.SetSceneSamplingDistance(0.03);
		// Ű����Ʈ ���� ���� // Set the keypoint ratio.
		SurfaceBasedMatch3D.SetKeypointRatio(0.5);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = SurfaceBasedMatch3D.Learn()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute Surface Based Match 3D.");
			break;
		}

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = SurfaceBasedMatch3D.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute Surface Based Match 3D.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately		
		CGUIView3DLayerWrap layer3DDst = view3DDst.GetLayer(0);
		CGUIView3DLayerWrap layer3DLearn = view3DLearn.GetLayer(0);
		CGUIView3DLayerWrap layer3DSource = view3DSource.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer3DDst.Clear();
		layer3DLearn.Clear();
		layer3DSource.Clear();

		// View ������ ���÷��� �Ѵ�. // Display view information
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((eResult = layer3DLearn.DrawTextCanvas(&CFLPointD(0, 0), L"Learn Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		if((eResult = layer3DSource.DrawTextCanvas(&CFLPointD(0, 0), L"Source Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		if((eResult = layer3DDst.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		// 3D ������Ʈ �信 ��� Object�� �񱳸� ���� Source ������Ʈ ���÷���
		if((eResult = view3DDst.PushObject((CFL3DObject)SurfaceBasedMatch3D.GetSourceObject())).IsFail())
		{
			ErrorPrint(eResult, L"Failed to set object on the 3d view.\n");
			break;
		}


		CSurfaceBasedMatch3D::SPoseMatrixParameters sResult;
		int64_t i64ResultCount = SurfaceBasedMatch3D.GetResultCount();
		TPoint3<double> tp3F64Rotation;
		CFLString<wchar_t> strText;
		CFLPoint3<double> flp3F64Translation;
		double f64Score;

		if(i64ResultCount == 0)
		{
			ErrorPrint(eResult, L"Failed to estimate pose matrix.\n");
			break;
		}

		for(int64_t i = 0; i < i64ResultCount; i++)
		{
			CFL3DObject fl3DOLearnTransform;
			Base::TPoint3<double> tp3Center;

			// ������ ���� ��� ��������
			if((eResult = SurfaceBasedMatch3D.GetResultPoseMatrix(i, sResult)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to estimate pose matrix.\n");
				break;
			}

			// ������ ���� ����� ����� ��� ������Ʈ ��������
			if((eResult = SurfaceBasedMatch3D.GetResultObject(i, fl3DOLearnTransform, tp3Center)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to estimate pose matrix.\n");
				break;
			}

			f64Score = sResult.f64Score;
			tp3F64Rotation.x = sResult.f64Rx;
			tp3F64Rotation.y = sResult.f64Ry;
			tp3F64Rotation.z = sResult.f64Rz;
			flp3F64Translation.x = sResult.f64Tx;
			flp3F64Translation.y = sResult.f64Ty;
			flp3F64Translation.z = sResult.f64Tz;

			// ������ ���� ����� Consoleâ�� ����Ѵ� // Print the estimated pose matrix to the console window
			printf(" �� Pose Matrix %d\n", (int32_t)i);
			printf("  1. R : Rotation, T : Translation\n");
			printf("    Rx   : %.3lf\n", tp3F64Rotation.x);
			printf("    Ry   : %.3lf\n", tp3F64Rotation.y);
			printf("    Rz   : %.3lf\n", tp3F64Rotation.z);
			printf("    Tx   : %.3lf\n", flp3F64Translation.x);
			printf("    Ty   : %.3lf\n", flp3F64Translation.y);
			printf("    Tz   : %.3lf\n", flp3F64Translation.z);
			printf("  2. Score : %.3lf\n", f64Score);
			printf("\n");

			if((eResult = view3DDst.PushObject(fl3DOLearnTransform)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to set object on the 3d view.\n");
				break;
			}

			strText.Format(L"Rx : %lf, Ry : %lf, Rz : %lf, \nTx : %lf, Ty : %lf, Tz : %lf\nScore : %lf"
						   , tp3F64Rotation.x, tp3F64Rotation.y, tp3F64Rotation.z, flp3F64Translation.x, flp3F64Translation.y, flp3F64Translation.z, f64Score);

			// ������ ���� ��� �� score ���
			if((eResult = layer3DDst.DrawText3D(tp3Center, strText, YELLOW, 0, 9)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to draw text.\n");
				break;
			}
		}

		view3DLearn.ZoomFit();
		view3DSource.ZoomFit();
		view3DDst.ZoomFit();

		// �̹��� �並 ���� �մϴ�. // Update image view
		view3DLearn.Invalidate(true);
		view3DSource.Invalidate(true);
		view3DDst.Invalidate(true);

		// �̹��� ��, 3D �䰡 ����� �� ���� ��ٸ�
		while(view3DLearn.IsAvailable() && view3DSource.IsAvailable() && view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}