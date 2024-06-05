#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 3D 뷰 선언
	CGUIView3DWrap view3D;

	CResult res = EResult_UnknownError;

	do
	{
		// 3D 뷰 생성
		if(IsFail(res = view3D.Create(100, 0, 612, 512)))
		{
			ErrorPrint(res, "Failed to create the 3D view.\n");
			break;
		}

		// 3D 뷰에 PLY 파일 디스플레이
		if((res = view3D.Load(L"../../ExampleImages/ROIUtilities3D/Right Cam.ply")).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the 3D view.\n");
			break;
		}

		view3D.ZoomFit();

		int32_t i32ObjectCount = view3D.GetObjectCount();

		if(!i32ObjectCount)
		{
			res = EResult_EmptyElement;
			ErrorPrint(res, "The 3D view doesn't have any 3D objects.\n");
			break;
		}

		// 3D 뷰에 ROI 추가
		TRect<float> trROI(137, 96, 239, 369);
		view3D.PushBackROI(&trROI);

		// Figure 객체를 로드하여 3D 뷰에 ROI 추가
		CFLFigure* pFlf = CFigureUtilities::LoadFigure(L"../../ExampleImages/ROIUtilities3D/frustumROI.fig");

		if(pFlf)
		{
			view3D.PushBackROI(pFlf);

			delete pFlf;
			pFlf = nullptr;
		}

		int32_t i32ROICount = view3D.GetROICount();

		if(i32ROICount == 0)
		{
			res = EResult_EmptyElement;
			ErrorPrint(res, "There is no ROI in the 3D view.\n");
			break;
		}

		CROIUtilities3D roiUtil3D;

		for(size_t i = 0; i < i32ROICount; ++i)
		{
			CFLFrustum<float> flfr;
			view3D.GetROI(i, flfr);

			roiUtil3D.PushBackROI(flfr);
		}

		for(int32_t i = 0; i < i32ObjectCount; ++i)
		{
			const CGUIView3DObject* pObj = view3D.GetView3DObject(i);

			if(!pObj || !pObj->IsSelectionEnabled())
				continue;

			CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

			if(!pObjData)
				continue;

			roiUtil3D.PushBack3DObject(pObjData);
		}

		CFLArray<CFLArray<int32_t>> flfaResultROIIndexInclude, flfaResultROIIndexExclude;

		roiUtil3D.SetSelectionType(CROIUtilities3D::EResultSelectionType_Include);

		if((res = roiUtil3D.Execute()).IsFail())
			break;

		if((res = roiUtil3D.GetResult(flfaResultROIIndexInclude)).IsFail())
			break;

		Foundation::CFL3DObject resultObject3D;

		if((res = roiUtil3D.GetResult(resultObject3D)).IsFail())
			break;

		roiUtil3D.SetSelectionType(CROIUtilities3D::EResultSelectionType_Exclude);

		if((res = roiUtil3D.Execute()).IsFail())
			break;

		if((res = roiUtil3D.GetResult(flfaResultROIIndexExclude)).IsFail())
			break;

		if(flfaResultROIIndexInclude.GetCount())
		{
			int32_t i32IndexRes = 0;

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				const CGUIView3DObject* pObj = view3D.GetView3DObject(i);

				if(!pObj || !pObj->IsSelectionEnabled())
					continue;

				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				if(!pObjData)
					continue;

				CFLArray<int32_t>& flaCollisionIndex = flfaResultROIIndexInclude.GetAt(i32IndexRes);
				i32IndexRes++;

				if(flaCollisionIndex.IsEmpty())
					continue;

				int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
				CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

				for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
					pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(255, 0, 0)); // RED

				pObj->UpdateAll();
				view3D.UpdateObject(i);
			}
		}

		if(flfaResultROIIndexExclude.GetCount())
		{
			int32_t i32IndexRes = 0;

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				CGUIView3DObject* pObj = (CGUIView3DObject*)view3D.GetView3DObject(i);

				if(!pObj || !pObj->IsSelectionEnabled())
					continue;

				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				if(!pObjData)
					continue;

				CFLArray<int32_t>& flaCollisionIndex = flfaResultROIIndexExclude.GetAt(i32IndexRes);
				i32IndexRes++;

				if(flaCollisionIndex.IsEmpty())
					continue;

				int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
				CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

				for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
					pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(0, 0, 255)); // BLUE

				pObj->UpdateAll();
				view3D.UpdateObject(i);
			}
		}

		view3D.UpdateScreen();

		// 아래 함수 DrawTextCanvas()는 screen좌표를 기준으로 하는 string을 drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = view3D.GetLayer(2).DrawTextCanvas(&CFLPoint<double>(0, 0), L"3D View", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// 3D 뷰를 갱신 // Update 3D view
		view3D.Invalidate(true);

		// 3D 뷰가 종료될 때 까지 기다림 // Wait for the 3D view 
		while(view3D.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}