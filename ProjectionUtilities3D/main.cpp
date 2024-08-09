#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage[3];

	CResult res = EResult_UnknownError;

	do
	{
		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage[0].Create(0, 0, 400, 440)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(400, 0, 800, 440)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[2].Create(800, 0, 1200, 440)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 3D Object ���� �ε� // Load 3D Object file
		CFL3DObject* pObj3D = new CFL3DObject();
		pObj3D->Load(L"../../ExampleImages/ProjectionUtilities3D/Cylinder.step");

		CFLImage fliFinal0, fliFinal1, fliFinal2;
		CFLImage fliRes;
		CFLFigureText<int32_t> figureText;

		// CProjectionUtilities3D ��ü ���� // Create CProjectionUtilities3D object
		CProjectionUtilities3D pu;

		// CProjectionUtilities3D ��ü�� 3D Object �� �߰� // Add 3D Object to CProjectionUtilities3D object
		pu.PushBack3DObject(pObj3D);
 
 		if(pObj3D)
 		{
 			delete pObj3D;
 			pObj3D = nullptr;
 		}

		// ��� �̹��� ũ�� ���� // Set result image size
		pu.SetResultImageSize(400, 400);

		// ��� �̹��� ��� ���� ���� // Set background color of result image
		pu.SetBackgroundColorOfResultImage(21, 21, 21);

		// 1-1. Ư�� ������ ���� �̹��� ��� // 1-1. Get projection image from specific viewpoint
		// ī�޶� ���� ���� // Set camera viewpoint
		CFL3DCamera camSet1;
		camSet1.SetProjectionType(E3DCameraProjectionType_Perspective);
		camSet1.SetPosition(CFLPoint3F(-1.41f, -317.67f, 280.92f));
		camSet1.SetDirection(CFLPoint3F(0.01f, 0.87f, -0.50f));
		camSet1.SetDirectionUp(CFLPoint3F(-0.01f, 0.50f, 0.87f));
		camSet1.SetAngleOfViewY(45.f);
		camSet1.SetTarget(CFLPoint3F(2.13f, -59.49f, 132.75f));
		camSet1.SetNearZ(271.84f);
		camSet1.SetFarZ(459.30f);

		// ī�޶� ���� // Set camera
		pu.SetCamera(camSet1);

		// �������� ���� // Perform projection
		res = pu.Execute();

		// ��� �̹��� ��� // Get result image
		res = pu.GetResult(fliRes);

		// ��� �̹����� ���� �ؽ�Ʈ �߰� // Add information text to result image
		figureText.Set(CFLPoint<int32_t>(10, 10), L"1. Projection(Camera Set 1)", YELLOW, BLACK, 20, false, 0., EFigureTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EFigureTextFontWeight_BOLD, false);
		fliRes.PushBackFigure(CFigureUtilities::ConvertFigureObjectToString(&figureText));

		// ���� �̹����� ���� ��� �̹��� ���� // Copy projection result image to final image
		fliFinal0.Assign(fliRes);

		// 1-2. Ư�� ������ ���� �̹��� ��� // 1-2. Get projection image from another specific viewpoint
		// ī�޶� ���� ���� // Set camera viewpoint
		CFL3DCamera camSet2;
		camSet2.SetProjectionType(E3DCameraProjectionType_Perspective);
		camSet2.SetPosition(CFLPoint3F(-80.38f, 97.35f, 341.92f));
		camSet2.SetDirection(CFLPoint3F(0.42f, -0.27f, -0.86f));
		camSet2.SetDirectionUp(CFLPoint3F(0.77f, 0.61f, 0.19f));
		camSet2.SetAngleOfViewY(45.f);
		camSet2.SetTarget(CFLPoint3F(-5.45f, 49.05f, 189.72f));
		camSet2.SetNearZ(148.33f);
		camSet2.SetFarZ(390.77f);

		// ī�޶� ���� // Set camera
		pu.SetCamera(camSet2);

		// �������� ���� // Perform projection
		res = pu.Execute();

		// ��� �̹��� ��� // Get result image
		res = pu.GetResult(fliRes);

		// ��� �̹����� ���� �ؽ�Ʈ �߰� // Add information text to result image
		figureText.Set(CFLPoint<int32_t>(10, 10), L"1. Projection(Camera Set 2)", YELLOW, BLACK, 20, false, 0., EFigureTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EFigureTextFontWeight_BOLD, false);
		fliRes.PushBackFigure(CFigureUtilities::ConvertFigureObjectToString(&figureText));

		// ���� �̹����� ���� ��� �̹��� �߰� // Add projection result image to final image
		fliFinal0.PushBackPage(fliRes);
		// ��� �̹����� �̹��� �信 �ε� // Load result image into image view		
		viewImage[0].GetIntrinsicImage()->Assign(fliFinal0);
		viewImage[0].SetImagePtr(viewImage[0].GetIntrinsicImage());
		viewImage[0].SetFixThumbnailView(true);


		// 2. ī�޶� 1�� ī�޶� 2 ������ ������ ���� �������� // 2. Projection for viewpoints between Camera 1 and Camera 2		
		for(int32_t i = 0; i <= 10; ++i)
		{
			// ī�޶� ���� ���� // Set camera viewpoint
			CFL3DCamera camInterpolation;
			float f32T = (float)i * 0.1f;
			CFL3DCamera::Interpolate(camSet1, camSet2, f32T, camInterpolation);

			// ī�޶� ���� // Set camera
			pu.SetCamera(camInterpolation);

			// �������� ���� // Perform projection
			res = pu.Execute();

			// ��� �̹��� ��� // Get result image
			res = pu.GetResult(fliRes);

			// ��� �̹����� ���� �ؽ�Ʈ �߰� // Add information text to result image
			CFLString<wchar_t> str;
			str.Format(L"2. Projection(Camera Interpolation T=%.1f)", f32T);
			figureText.Set(CFLPoint<int32_t>(10, 10), str, YELLOW, BLACK, 15, false, 0., EFigureTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EFigureTextFontWeight_SEMIBOLD, false);
			fliRes.PushBackFigure(CFigureUtilities::ConvertFigureObjectToString(&figureText));

			// ���� �̹����� ���� ��� �̹��� �߰� // Add projection result image to final image
			if(i == 0)
				fliFinal1.Assign(fliRes);
			else
				fliFinal1.PushBackPage(fliRes);
		}

		// ��� �̹����� �̹��� �信 �ε� // Load result image into image view		
		viewImage[1].GetIntrinsicImage()->Assign(fliFinal1);
		viewImage[1].SetImagePtr(viewImage[1].GetIntrinsicImage());
		viewImage[1].SetFixThumbnailView(true);


		// 3. Zoom Fit ������ �̹��� ��� // 3. Get image at Zoom Fit viewpoint
		pu.ZoomFitCamera();

		// �������� ���� // Perform projection
		res = pu.Execute();

		// ��� �̹��� ��� // Get result image
		res = pu.GetResult(fliFinal2);

		// ��� �̹����� ���� �ؽ�Ʈ �߰� // Add information text to result image
		figureText.Set(CFLPoint<int32_t>(10, 10), L"3. Projection(ZoomFit)", YELLOW, BLACK, 20, false, 0., EFigureTextAlignment_LEFT_TOP, L"", 1.f, 1.f, EFigureTextFontWeight_BOLD, false);
		fliFinal2.PushBackFigure(CFigureUtilities::ConvertFigureObjectToString(&figureText));

		// ��� �̹����� �̹��� �信 �ε� // Load result image into image view		
		viewImage[2].GetIntrinsicImage()->Assign(fliFinal2);
		viewImage[2].SetImagePtr(viewImage[2].GetIntrinsicImage());
		viewImage[2].SetFixThumbnailView(true);

		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable() && viewImage[2].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}