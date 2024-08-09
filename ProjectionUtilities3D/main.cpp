#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage;

	CResult res = EResult_UnknownError;

	do
	{
		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage.Create(400, 0, 800, 440)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 3D Object 파일 로드 // Load 3D Object file
		CFL3DObject* pObj3D = new CFL3DObject();
		pObj3D->Load(L"../../ExampleImages/ProjectionUtilities3D/Cylinder.step");

		CFLImage fliFinal;
		CFLImage fliRes;
		CFLFigureText<int32_t> figureText;

		// CProjectionUtilities3D 객체 생성 // Create CProjectionUtilities3D object
		CProjectionUtilities3D pu;

		// CProjectionUtilities3D 객체에 3D Object 를 추가 // Add 3D Object to CProjectionUtilities3D object
		pu.PushBack3DObject(pObj3D);

		if(pObj3D)
		{
			delete pObj3D;
			pObj3D = nullptr;
		}

		// 결과 이미지 크기 설정 // Set result image size
		pu.SetResultImageSize(400, 400);

		// 결과 이미지 배경 색상 설정 // Set background color of result image
		pu.SetBackgroundColorOfResultImage(21, 21, 21);

		// 1-1. 특정 시점의 투영 이미지 얻기 // 1-1. Get projection image from specific viewpoint
		// 카메라 시점 설정 // Set camera viewpoint
		CFL3DCamera camSet1;
		camSet1.SetProjectionType(E3DCameraProjectionType_Perspective);
		camSet1.SetPosition(CFLPoint3F(-1.41f, -317.67f, 280.92f));
		camSet1.SetDirection(CFLPoint3F(0.01f, 0.87f, -0.50f));
		camSet1.SetDirectionUp(CFLPoint3F(-0.01f, 0.50f, 0.87f));
		camSet1.SetAngleOfViewY(45.f);
		camSet1.SetTarget(CFLPoint3F(2.13f, -59.49f, 132.75f));
		camSet1.SetNearZ(271.84f);
		camSet1.SetFarZ(459.30f);

		// 카메라 설정 // Set camera
		pu.SetCamera(camSet1);

		// 프로젝션 수행 // Perform projection
		res = pu.Execute();

		// 결과 이미지 얻기 // Get result image
		res = pu.GetResult(fliRes);

		// 결과 이미지에 정보 텍스트 추가 // Add information text to result image
		figureText.Set(CFLPoint<int32_t>(10, 10), L"1. Projection(Camera Set 1)", YELLOW, BLACK, 20, false, 0., EFigureTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EFigureTextFontWeight_BOLD, false);
		fliRes.PushBackFigure(CFigureUtilities::ConvertFigureObjectToString(&figureText));

		// 최종 이미지에 투영 결과 이미지 복사 // Copy projection result image to final image
		fliFinal.Assign(fliRes);

		// 1-2. 특정 시점의 투영 이미지 얻기 // 1-2. Get projection image from another specific viewpoint
		// 카메라 시점 설정 // Set camera viewpoint
		CFL3DCamera camSet2;
		camSet2.SetProjectionType(E3DCameraProjectionType_Perspective);
		camSet2.SetPosition(CFLPoint3F(-80.38f, 97.35f, 341.92f));
		camSet2.SetDirection(CFLPoint3F(0.42f, -0.27f, -0.86f));
		camSet2.SetDirectionUp(CFLPoint3F(0.77f, 0.61f, 0.19f));
		camSet2.SetAngleOfViewY(45.f);
		camSet2.SetTarget(CFLPoint3F(-5.45f, 49.05f, 189.72f));
		camSet2.SetNearZ(148.33f);
		camSet2.SetFarZ(390.77f);

		// 카메라 설정 // Set camera
		pu.SetCamera(camSet2);

		// 프로젝션 수행 // Perform projection
		res = pu.Execute();

		// 결과 이미지 얻기 // Get result image
		res = pu.GetResult(fliRes);

		// 결과 이미지에 정보 텍스트 추가 // Add information text to result image
		figureText.Set(CFLPoint<int32_t>(10, 10), L"1. Projection(Camera Set 2)", YELLOW, BLACK, 20, false, 0., EFigureTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EFigureTextFontWeight_BOLD, false);
		fliRes.PushBackFigure(CFigureUtilities::ConvertFigureObjectToString(&figureText));

		// 최종 이미지에 투영 결과 이미지 추가 // Add projection result image to final image
		fliFinal.PushBackPage(fliRes);

		// 2. 카메라 1과 카메라 2 사이의 시점에 대한 프로젝션 // 2. Projection for viewpoints between Camera 1 and Camera 2		
		for(int32_t i = 0; i < 10; ++i)
		{
			// 카메라 시점 설정 // Set camera viewpoint
			CFL3DCamera camInterpolation;
			float f32T = (float)i * 0.1f;
			CFL3DCamera::Interpolate(camSet1, camSet2, f32T, camInterpolation);

			// 카메라 설정 // Set camera
			pu.SetCamera(camInterpolation);

			// 프로젝션 수행 // Perform projection
			res = pu.Execute();

			// 결과 이미지 얻기 // Get result image
			res = pu.GetResult(fliRes);

			// 결과 이미지에 정보 텍스트 추가 // Add information text to result image
			CFLString<wchar_t> str;
			str.Format(L"2. Projection(Camera Interpolation T=%.1f)", f32T);
			figureText.Set(CFLPoint<int32_t>(10, 10), str, YELLOW, BLACK, 15, false, 0., EFigureTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EFigureTextFontWeight_SEMIBOLD, false);
			fliRes.PushBackFigure(CFigureUtilities::ConvertFigureObjectToString(&figureText));

			// 최종 이미지에 투영 결과 이미지 추가 // Add projection result image to final image
			fliFinal.PushBackPage(fliRes);
		}

		// 3. Zoom Fit 시점의 이미지 얻기 // 3. Get image at Zoom Fit viewpoint
		pu.ZoomFitCamera();

		// 프로젝션 수행 // Perform projection
		res = pu.Execute();

		// 결과 이미지 얻기 // Get result image
		res = pu.GetResult(fliRes);

		// 결과 이미지에 정보 텍스트 추가 // Add information text to result image
		figureText.Set(CFLPoint<int32_t>(10, 10), L"3. Projection(ZoomFit)", YELLOW, BLACK, 20, false, 0., EFigureTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EFigureTextFontWeight_BOLD, false);
		fliRes.PushBackFigure(CFigureUtilities::ConvertFigureObjectToString(&figureText));

		// 최종 이미지에 투영 결과 이미지 추가 // Add projection result image to final image
		fliFinal.PushBackPage(fliRes);

		// 결과 이미지를 이미지 뷰에 로드 // Load result image into image view		
		viewImage.GetIntrinsicImage()->Assign(fliFinal);
		viewImage.SetImagePtr(viewImage.GetIntrinsicImage());
		viewImage.SetFixThumbnailView(true);

		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}