#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 3D ��ü ���� // Declare 3D object
	CFL3DObject fl3DOLearnObject;
	CFL3DObject fl3DOSourceObject;
	CFL3DObject fl3DOSampleObject;

	// 3D �� ���� // Declare 3D view	
	CGUIView3DWrap view3DDst;
	CGUIView3DWrap view3DLearn;
	CGUIView3DWrap view3DSource;


	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult eResult = EResult_UnknownError;


		// Learn Object �ε� // Load the Learn object
		if((eResult = fl3DOLearnObject.Load(L"../../ExampleImages/VertexMatch3D/ResultPoints.ply")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the object file.\n");
			break;
		}

		// Source Object �ε� // Load the Source object
		if((eResult = fl3DOSourceObject.Load(L"../../ExampleImages/VertexMatch3D/ReferencePoints.ply")).IsFail())
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

		// Source Object 3D �� ���� // Create the source object 3D view
		if((eResult = view3DSource.PushObject(fl3DOSourceObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to display the 3D object.\n");
			break;
		}

		// VertexMatch3D ��ü ���� // Create VertexMatch3D object
		CVertexMatch3D VertexMatch3D;

		// Learn Object ���� // Set the learn object
		VertexMatch3D.SetLearnObject(fl3DOLearnObject);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = VertexMatch3D.Learn()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute Vertex Match 3D.");
			break;
		}

		// �н� ������ ���� // Save the learnt data
		if((eResult = VertexMatch3D.Save(L"../../ExampleImages/VertexMatch3D/Example FLVM Data.flvm")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the object file.\n");
			break;
		}

		// �н� ������ �ҷ����� // Load the learnt data
		if((eResult = VertexMatch3D.Load(L"../../ExampleImages/VertexMatch3D/Example FLVM Data.flvm")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the object file.\n");
			break;
		}

		VertexMatch3D.GetSampledLearn3DObject(fl3DOSampleObject);

		// Sampled data ��� // Display the sampled data
		if((eResult = view3DLearn.PushObject(fl3DOSampleObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to display the 3D object.\n");
			break;
		}

		// Learn Object ��� // Display the learn object
		if((eResult = view3DLearn.PushObject(fl3DOLearnObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to display the 3D object.\n");
			break;
		}


		// Learn object ���� // Set the learn object
		VertexMatch3D.SetLearnObject(fl3DOLearnObject);
		// Source object ���� // Set the source object
		VertexMatch3D.SetSourceObject(fl3DOSourceObject);
		// Min score ���� // Set the min score
		VertexMatch3D.SetMinScore(0.3);
		// �ִ� ��� ���� ���� // Set the max count of match result
		VertexMatch3D.SetMaxObject(4);
		// �н� ���ø� �Ÿ� ���� // Set the learn sampling distance
		VertexMatch3D.SetLearnSamplingDistance(0.03);
		// ��� ���ø� �Ÿ� ���� // Set the scene sampling distance
		VertexMatch3D.SetSceneSamplingDistance(0.03);
		// Ű����Ʈ ���� ���� // Set the keypoint ratio.
		VertexMatch3D.SetKeypointRatio(0.5);
		// ���� �н� ���� ���� // Set the edge train
		VertexMatch3D.EnableTrainEdge(false);
		// ��� ���� ���� ���� // Set the background removal
		VertexMatch3D.EnableBackgroundRemoval(false);
		// Ŭ�����͸� ���� ���� // Set the clustering range
		VertexMatch3D.SetClusterRange(0.02);
		// ���� ���� �ݺ� Ƚ�� ���� // Set the iteration value of pose refinement
		VertexMatch3D.SetIteration(5);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((eResult = VertexMatch3D.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute Vertex Match 3D.");
			break;
		}

		view3DLearn.SetShadingType(EShadingType3D_Flat);
		view3DSource.SetShadingType(EShadingType3D_Flat);
		view3DDst.SetShadingType(EShadingType3D_Flat);


		view3DLearn.ZoomFit();
		view3DSource.ZoomFit();
		view3DDst.ZoomFit();


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
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((eResult = layer3DLearn.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Learn Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		if((eResult = layer3DSource.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		if((eResult = layer3DDst.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		// 3D ������Ʈ �信 ��� Object�� �񱳸� ���� Source ������Ʈ ���÷���
		if((eResult = view3DDst.PushObject((CFL3DObject)VertexMatch3D.GetSourceObject())).IsFail())
		{
			ErrorPrint(eResult, L"Failed to set object on the 3d view.\n");
			break;
		}


		CVertexMatch3D::SPoseMatrixParameters sResult;
		int64_t i64ResultCount = VertexMatch3D.GetResultCount();
		TPoint3<double> tp3F64Rotation;
		CFLString<wchar_t> strText;
		CFLPoint3<double> flp3F64Translation, tp3F64RotVec;
		double f64Score, f64Residual;

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
			if((eResult = VertexMatch3D.GetResultPoseMatrix(i, sResult)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to estimate pose matrix.\n");
				break;
			}

			// ������ ���� ����� ����� ��� ������Ʈ ��������
			if((eResult = VertexMatch3D.GetResultObject(i, fl3DOLearnTransform, tp3Center)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to estimate pose matrix.\n");
				break;
			}

			f64Residual = sResult.f64Residual;
			f64Score = sResult.f64Score;
			tp3F64Rotation.x = sResult.f64Rx;
			tp3F64Rotation.y = sResult.f64Ry;
			tp3F64Rotation.z = sResult.f64Rz;
			tp3F64RotVec.x = sResult.f64RotationVectorX;
			tp3F64RotVec.y = sResult.f64RotationVectorY;
			tp3F64RotVec.z = sResult.f64RotationVectorZ;
			flp3F64Translation.x = sResult.f64Tx;
			flp3F64Translation.y = sResult.f64Ty;
			flp3F64Translation.z = sResult.f64Tz;

			// ������ ���� ����� Consoleâ�� ����Ѵ� // Print the estimated pose matrix to the console window
			printf(" �� Pose Matrix %d\n", (int32_t)i);
			printf("  1. R : Rotation, T : Translation\n");
			printf("    Rx   : %.3lf\n", tp3F64Rotation.x);
			printf("    Ry   : %.3lf\n", tp3F64Rotation.y);
			printf("    Rz   : %.3lf\n", tp3F64Rotation.z);
			printf("    Rotation Vector X   : %.3lf\n", tp3F64RotVec.x);
			printf("    Rotation Vector Y   : %.3lf\n", tp3F64RotVec.y);
			printf("    Rotation Vector Z   : %.3lf\n", tp3F64RotVec.z);
			printf("    Tx   : %.3lf\n", flp3F64Translation.x);
			printf("    Ty   : %.3lf\n", flp3F64Translation.y);
			printf("    Tz   : %.3lf\n", flp3F64Translation.z);
			printf("    Score : %.3lf\n", f64Score);
			printf("    Residual : %.3lf\n", f64Residual);
			printf("\n");

			if((eResult = view3DDst.PushObject(fl3DOLearnTransform)).IsFail())
			{
				ErrorPrint(eResult, L"Failed to set object on the 3d view.\n");
				break;
			}

			strText.Format(L"R(%.04lf,%.04lf,%.04lf)\nT(%.04lf,%.04lf,%.04lf)\nScore : %lf\nResidual : %lf"
						   , tp3F64Rotation.x, tp3F64Rotation.y, tp3F64Rotation.z, flp3F64Translation.x, flp3F64Translation.y, flp3F64Translation.z, f64Score, f64Residual);

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