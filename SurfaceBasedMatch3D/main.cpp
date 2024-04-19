#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 3D 객체 선언 // Declare 3D object
	CFL3DObject fl3DOLearnObject;
	CFL3DObject fl3DOSourceObject;
	CFL3DObject fl3DODestinationObject;

	// 3D 뷰 선언 // Declare 3D view	
	CGUIView3DWrap view3DDst;
	CGUIView3DWrap view3DLearn;
	CGUIView3DWrap view3DSource;


	do
	{
		// 알고리즘 동작 결과 // Algorithm execution result
		CResult eResult = EResult_UnknownError;

		// ply 파일 리더 // ply file reader
		CPlyReader plyReader;

		// Learn Object 로드 // Load the learn object
		if((eResult = plyReader.Load(L"../../ExampleImages/SurfaceBasedMatch3D/ReferencePoints.ply")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the object file.\n");
			break;
		}

		if((eResult = plyReader.GetResult3DObject(fl3DOLearnObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the object file.\n");
			break;
		}

		// Source Object 로드 // Load the Source object
		if((eResult = plyReader.Load(L"../../ExampleImages/SurfaceBasedMatch3D/MeasuredPoints.ply")).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the object file.\n");
			break;
		}

		if((eResult = plyReader.GetResult3DObject(fl3DOSourceObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to load the object file.\n");
			break;
		}

		// Learn 3D 뷰 생성
		if(IsFail(eResult = view3DLearn.Create(100, 0, 612, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Source 3D view.\n");
			break;
		}

		// Source 3D 뷰 생성
		if(IsFail(eResult = view3DSource.Create(612, 0, 1124, 512)))
		{
			ErrorPrint(eResult, "Failed to create the Learn 3D view.\n");
			break;
		}

		// Dst 3D 뷰 생성
		if(IsFail(eResult = view3DDst.Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(res, "Failed to create the Dst 3D view.\n");
			break;
		}

		// Learn Object 3D 뷰 생성 // Create the learn object 3D view
		if((eResult = view3DLearn.PushObject(fl3DOLearnObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to display the 3D object.\n");
			break;
		}

		// Source Object 3D 뷰 생성 // Create the source object 3D view
		if((eResult = view3DSource.PushObject(fl3DOSourceObject)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to display the 3D object.\n");
			break;
		}

		// SurfaceBasedMatch3D 객체 생성 // Create SurfaceBasedMatch3D object
		CSurfaceBasedMatch3D SurfaceBasedMatch3D;

		// Learn object 설정 // Set the learn object
		SurfaceBasedMatch3D.CAlgorithmFeature_3D_LO_SO_DO::SetLearnObject(fl3DOLearnObject);
		// Source object 설정 // Set the source object
		SurfaceBasedMatch3D.CAlgorithmFeature_3D_LO_SO_DO::SetSourceObject(fl3DOSourceObject);
		// Destination object 설정 // Set the destination object
		SurfaceBasedMatch3D.CAlgorithmFeature_3D_LO_SO_DO::SetDestinationObject(fl3DODestinationObject);
		// 오브젝트 타입 설정 // Set the type of the object
		SurfaceBasedMatch3D.SetObjectType(CSurfaceBasedMatch3D::EObjectType_Object3D);
		// Min score 설정 // Set the min score
		SurfaceBasedMatch3D.SetMinScore(0.2);
		// 최대 point cloud 개수 설정 // Set the max count of point cloud
		SurfaceBasedMatch3D.SetMaxFeatureCount(64);
		// 최대 결과 개수 설정 // Set the max count of match result
		SurfaceBasedMatch3D.SetMaxObject(1);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((eResult = SurfaceBasedMatch3D.Learn()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute Laser Triangulation.");
			break;
		}

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((eResult = SurfaceBasedMatch3D.Execute()).IsFail())
		{
			ErrorPrint(eResult, L"Failed to execute Laser Triangulation.");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately		
		CGUIView3DLayerWrap layer3DDst = view3DDst.GetLayer(0);
		CGUIView3DLayerWrap layer3DLearn = view3DLearn.GetLayer(0);
		CGUIView3DLayerWrap layer3DSource = view3DSource.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layer3DDst.Clear();
		layer3DLearn.Clear();
		layer3DSource.Clear();

		// View 정보를 디스플레이 한다. // Display view information
		// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
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

		// 3D 오브젝트 뷰에 결과 Object와 비교를 위한 Source 오브젝트 디스플레이
		if((eResult = view3DDst.PushObject((CFL3DObject)SurfaceBasedMatch3D.GetSourceObject())).IsFail())
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		// 3D 오브젝트 뷰에 결과 Object 디스플레이
		if((eResult = view3DDst.PushObject((CFL3DObject)SurfaceBasedMatch3D.GetDestinationObject())).IsFail())
		{
			ErrorPrint(eResult, L"Failed to set image object on the image view.\n");
			break;
		}

		CFLArray<double> flaF64Score, flaF64PoseMatrix;
		TPoint3<double> tp3F64Rotation;
		CFLString<wchar_t> strText;
		CFLPoint3<double> flp3F64Translation;
		double f64Score;

		// 매치 결과 가져오기
		if((eResult = SurfaceBasedMatch3D.GetResultScore(flaF64Score)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to estimate pose matrix.\n");
			break;
		}

		// 추정된 포즈 행렬 가져오기
		if((eResult = SurfaceBasedMatch3D.GetResultPoseMatrix(flaF64PoseMatrix)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to estimate pose matrix.\n");
			break;
		}

		f64Score = flaF64Score.GetAt(0);
		tp3F64Rotation.x = flaF64PoseMatrix.GetAt(0);
		tp3F64Rotation.y = flaF64PoseMatrix.GetAt(1);
		tp3F64Rotation.z = flaF64PoseMatrix.GetAt(2);
		flp3F64Translation.x = flaF64PoseMatrix.GetAt(3);
		flp3F64Translation.y = flaF64PoseMatrix.GetAt(4);
		flp3F64Translation.z = flaF64PoseMatrix.GetAt(5);

		// 추정한 포즈 결과를 Console창에 출력한다 // Print the estimated pose matrix to the console window
		printf(" ▷ Pose Matrix\n");
		printf("  1. R : Rotation, T : Translation\n");
		printf("    Rx   : %.3lf\n", tp3F64Rotation.x);
		printf("    Ry   : %.3lf\n", tp3F64Rotation.y);
		printf("    Rz   : %.3lf\n", tp3F64Rotation.z);
		printf("    Tx   : %.3lf\n", flp3F64Translation.x);
		printf("    Ty   : %.3lf\n", flp3F64Translation.y);
		printf("    Tz   : %.3lf\n", flp3F64Translation.z);
		printf("  2. Score : %.3lf\n", f64Score);
		printf("\n");


		strText.Format(L"Rx : %lf, Ry : %lf, Rz : %lf, \nTx : %lf, Ty : %lf, Tz : %lf\nScore : %lf"
					   , tp3F64Rotation.x, tp3F64Rotation.y, tp3F64Rotation.z, flp3F64Translation.x, flp3F64Translation.y, flp3F64Translation.z, f64Score);

		// 추정된 포즈 행렬 및 score 출력
		if((eResult = layer3DDst.DrawTextCanvas(&CFLPointD(0, 32), strText, YELLOW, BLACK, 15)).IsFail())
		{
			ErrorPrint(eResult, L"Failed to draw text.\n");
			break;
		}

		view3DLearn.ZoomFit();
		view3DSource.ZoomFit();
		view3DDst.ZoomFit();

		// 이미지 뷰를 갱신 합니다. // Update image view
		view3DLearn.Invalidate(true);
		view3DSource.Invalidate(true);
		view3DDst.Invalidate(true);

		// 이미지 뷰, 3D 뷰가 종료될 때 까지 기다림
		while(view3DLearn.IsAvailable() && view3DSource.IsAvailable() && view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}