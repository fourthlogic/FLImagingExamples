#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

const CResult DrawResult(GUI::CGUIView3DWrap* pView3D, const Base::CFLArray<Base::TPoint3<float>>* pFlaPlyData, Base::CFLArray<Base::TPoint3<float>>& arrResult, Base::CFLString<wchar_t> strDirection)
{
	CResult er;

	do
	{
		float arr2F32DataRange[3][2] =
		{
			{ std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() },
			{ std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() },
			{ std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() },
		};

		for(int32_t i = 0; i < (int32_t)arrResult.GetCount(); ++i)
		{
			float* pF32Value = (float*)&arrResult[i];

			for(int32_t j = 0; j < 3; ++j)
			{
				arr2F32DataRange[j][0] = std::min<float>(arr2F32DataRange[j][0], *(pF32Value + j));
				arr2F32DataRange[j][1] = std::max<float>(arr2F32DataRange[j][1], *(pF32Value + j));
			}
		}

		if(!pFlaPlyData)
		{
			er = EResult_NoData;
			break;
		}

		if(pFlaPlyData->GetCount() != arrResult.GetCount())
		{
			er = EResult_NoResult;
			break;
		}

		CFLString<wchar_t> strRangeX = CFLString<wchar_t>().Format(L"X : [%.4f, %.4f]", arr2F32DataRange[0][0], arr2F32DataRange[0][1]);
		CFLString<wchar_t> strRangeY = CFLString<wchar_t>().Format(L"Y : [%.4f, %.4f]", arr2F32DataRange[1][0], arr2F32DataRange[1][1]);
		CFLString<wchar_t> strRangeZ = CFLString<wchar_t>().Format(L"Z : [%.4f, %.4f]", arr2F32DataRange[2][0], arr2F32DataRange[2][1]);

		pView3D->GetLayer(0).DrawTextCanvas(CFLPoint<double>(10, 20), L"Data Ranges", RGB(255, 255, 128), BLACK, 13);
		pView3D->GetLayer(0).DrawTextCanvas(CFLPoint<double>(10, 35), strRangeX, RGB(255, 128, 128), BLACK, 13);
		pView3D->GetLayer(0).DrawTextCanvas(CFLPoint<double>(10, 50), strRangeY, RGB(128, 255, 128), BLACK, 13);
		pView3D->GetLayer(0).DrawTextCanvas(CFLPoint<double>(10, 65), strRangeZ, RGB(128, 128, 255), BLACK, 13);

		int32_t i32SelectedAxis;

		if(strDirection == L"Delta X")
			i32SelectedAxis = 0;
		else if(strDirection == L"Delta Y")
			i32SelectedAxis = 1;
		else // dZ
			i32SelectedAxis = 2;

		auto lGetColor = [](float f32Intensity, float arrF32Color[3]) -> void
			{
				float f32Temp;
				float f32Segment = (1.f / 6.f);

				for(int32_t i = 0; i < 3; ++i)
				{
					arrF32Color[i] = (f32Intensity - (i * 2 - 1) * f32Segment) / f32Segment;
					arrF32Color[i] = arrF32Color[i] > 1 ? 1 : arrF32Color[i];
					arrF32Color[i] = arrF32Color[i] < 0 ? 0 : arrF32Color[i];
					f32Temp = (f32Intensity - (i * 2 + 1) * f32Segment) / f32Segment;
					f32Temp = f32Temp > 1 ? 1 : f32Temp;
					f32Temp = f32Temp < 0 ? 0 : f32Temp;
					arrF32Color[i] -= f32Temp;
				}
			};


		CFLArray<TPoint3<float> > flaColors;
		flaColors.Reserve(arrResult.GetCount());

		for(int64_t i = 0; i < arrResult.GetCount(); ++i)
		{
			float f32Intensity;
			TPoint3<float> tpColor;

			f32Intensity = (*((float*)&arrResult[i] + i32SelectedAxis) - arr2F32DataRange[i32SelectedAxis][0]) / (arr2F32DataRange[i32SelectedAxis][1] - arr2F32DataRange[i32SelectedAxis][0]);

			if(f32Intensity < 0) f32Intensity = 0;
			if(f32Intensity > 1) f32Intensity = 1;

			lGetColor(f32Intensity, (float*)&tpColor);
			flaColors.PushBack(tpColor);
		}

		pView3D->PushObject(CFL3DObject(*pFlaPlyData, flaColors));

		er = EResult_OK;
	}
	while(false);

	return er;
}

int main()
{
	// �̹��� �� ���� // Declare image view
	CGUIView3DWrap view3DSrc;
	CGUIView3DWrap view3DDst;
	CFL3DObject fl3DObject;

	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Source 3D �̹��� �� ���� // Create the Source 3D image view
		if((res = view3DSrc.Create(400, 200, 1300, 800)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination 3D �̹��� �� ���� // Create the destination 3D image view
		if((res = view3DDst.Create(400, 200, 1300, 800)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		fl3DObject.Load(L"../../ExampleImages/DistanceTransform3D/binary-vertex.ply");

		// Distance Transform 3D ��ü ���� // Create Distance Transform 3D object
		CDistanceTransform3D DistanceTransform3D;

		TPoint3<float> tpPosition = TPoint3<float>(0.000000, 0.000000, 0.000000);
		TPoint3<float> tpDirection = TPoint3<float>(-0.100000, 0.000000, -1.000000);
		TPoint3<float> tpUpVector = TPoint3<float>(0.000000, 1.000000, 0.000000);

		// Source ��ü ���� // Set the source object
		DistanceTransform3D.SetSourceObject(&fl3DObject);
		// ī�޶� ��ġ ���� // Set the camera position
		DistanceTransform3D.SetPosition(tpPosition);
		// ī�޶� ���� ���� // Set the camera direction
		DistanceTransform3D.SetDirection(tpDirection);
		// ī�޶� �� ���� ���� // Set the camera up vector
		DistanceTransform3D.SetUpVector(tpUpVector);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = DistanceTransform3D.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute multiFocus.");
			break;
		}

		CFLArray<TPoint3<float>> arrResult;
		// �Ÿ� ��� �������� // Get the distance
		DistanceTransform3D.GetResultDistanceAxis(arrResult);

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIView3DLayerWrap layer3DSrc = view3DSrc.GetLayer(0);
		CGUIView3DLayerWrap layer3DDst = view3DDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer3DSrc.Clear();
		layer3DDst.Clear();

		// �Ÿ� ����� �׷��ش� // Draw distance result
		DrawResult(&view3DDst, fl3DObject.GetVertices(), arrResult, L"Delta Z");

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		if((res = view3DDst.ZoomFit()).IsFail())
		{
			ErrorPrint(res, L"Failed to zoom fit of the image view.\n");
			break;
		}

		view3DSrc.PushObject(fl3DObject);
		view3DSrc.ZoomFit();

		if((res = layer3DSrc.DrawTextCanvas(&CFLPointD(0, 0), L"Source Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		if((res = layer3DDst.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		view3DSrc.Invalidate(true);
		view3DDst.Invalidate(true);

		// �̹��� ��, 3D �䰡 ����� �� ���� ��ٸ�
		while(view3DSrc.IsAvailable() || view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}