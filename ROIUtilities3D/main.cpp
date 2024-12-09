#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"
#include "../CommomHeader/ErrorMessageBox.h"


int main()
{
	// 3D 뷰 선언
	// Declaration of the 3D view 
	CGUIView3DWrap view3DSrc;
	CGUIView3DWrap view3DInclude;
	CGUIView3DWrap view3DExclude;
	CGUIView3DWrap view3DAdd;
	CGUIView3DWrap view3DRemove;
	CGUIView3DWrap view3DXOR;
	CGUIView3DWrap* arrView3D[6] = { &view3DSrc, &view3DInclude, &view3DExclude, &view3DAdd, &view3DRemove, &view3DXOR };

	CResult res = EResult_UnknownError;

	do
	{
		// Create the 3D view 
		view3DSrc.Create(0, 0, 300, 300); // L, T, R, B(Left, Top, Right, Bottom)
		view3DInclude.Create(300, 0, 600, 300);
		view3DExclude.Create(600, 0, 900, 300);
		view3DAdd.Create(0, 300, 300, 600);
		view3DRemove.Create(300, 300, 600, 600);
		view3DXOR.Create(600, 300, 900, 600);

		// Declare a 3D object
		CFL3DObject fl3DObjLeft, fl3DObjRight;
		// Load a 3D object
		res = fl3DObjLeft.Load(L"../../ExampleImages/ROIUtilities3D/Left Cam.ply");
		res = fl3DObjRight.Load(L"../../ExampleImages/ROIUtilities3D/Right Cam.ply");

		// 3D 뷰에 3D 객체 추가
		for(int32_t i = 0; i < 6; ++i)
		{
			arrView3D[i]->PushObject(fl3DObjLeft);
			arrView3D[i]->PushObject(fl3DObjRight);
			arrView3D[i]->ZoomFit();
			arrView3D[i]->GetCamera()->SetPosition(CFLPoint3F(0.71, 0.02, 10.94));
			arrView3D[i]->GetCamera()->SetDirectionUp(CFLPoint3F(1., 0., 0.));
		}

		CFLFrustum<float> flfr;
		flfr.Load(L"../../ExampleImages/ROIUtilities3D/frustumROI.fig");

		// 3D 뷰에 ROI 추가
		for(int32_t i = 0; i < 6; ++i)
			arrView3D[i]->PushBackROI(&flfr);

		// CROIUtilities3D 객체 선언
		CROIUtilities3D roiUtil3D;

		// CROIUtilities3D 객체에 3D Object 추가
		roiUtil3D.PushBack3DObject(&fl3DObjLeft);
		roiUtil3D.PushBack3DObject(&fl3DObjRight);

		// CROIUtilities3D 객체에 ROI 추가
		roiUtil3D.PushBackROI(flfr);

		// 선택 타입 설정 : ROI 안에 포함되는 정점만 선택
		roiUtil3D.SetSelectionType(CROIUtilities3D::EResultSelectionType_Include);

		// CROIUtilities3D 실행
		if((res = roiUtil3D.Execute()).IsFail())
			break;

		// CROIUtilities3D 에서 결과 얻어 오기
		CFLArray<CFLArray<int32_t>> flfaResultROIIndexInclude;
		if((res = roiUtil3D.GetResult(flfaResultROIIndexInclude)).IsFail())
			break;

		CROIUtilities3D roiUtil3DAdd = roiUtil3D;

		if(flfaResultROIIndexInclude.GetCount())
		{
			int32_t i32ObjectIdx = 0;
			int32_t i32ObjectCount = view3DInclude.GetObjectCount();

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				CGUIView3DObject* pObj = (CGUIView3DObject*)view3DInclude.GetView3DObject(i);

				if(!pObj || !pObj->IsSelectionEnabled())
					continue;

				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				if(!pObjData)
					continue;

				CFLArray<int32_t>& flaCollisionIndex = flfaResultROIIndexInclude.GetAt(i32ObjectIdx);
				i32ObjectIdx++;

				if(flaCollisionIndex.IsEmpty())
					continue;

				int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
				CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

				for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
					pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(255, 0, 0)); // RED

				pObj->UpdateAll();
				view3DInclude.UpdateObject(i);
			}

			view3DInclude.UpdateScreen();
		}

		// 선택 타입 설정 : ROI 바깥의 정점만 선택
		roiUtil3D.SetSelectionType(CROIUtilities3D::EResultSelectionType_Exclude);

		// CROIUtilities3D 실행
		if((res = roiUtil3D.Execute()).IsFail())
			break;

		// CROIUtilities3D 에서 결과 얻어 오기
		CFLArray<CFLArray<int32_t>> flfaResultROIIndexExclude;
		if((res = roiUtil3D.GetResult(flfaResultROIIndexExclude)).IsFail())
			break;

		CROIUtilities3D roiUtil3DRemove = roiUtil3D;
		CROIUtilities3D roiUtil3DXOR = roiUtil3D;

		if(flfaResultROIIndexExclude.GetCount())
		{
			int32_t i32ObjectIdx = 0;
			int32_t i32ObjectCount = view3DExclude.GetObjectCount();

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				CGUIView3DObject* pObj = (CGUIView3DObject*)view3DExclude.GetView3DObject(i);

				if(!pObj || !pObj->IsSelectionEnabled())
					continue;

				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				if(!pObjData)
					continue;

				CFLArray<int32_t>& flaCollisionIndex = flfaResultROIIndexExclude.GetAt(i32ObjectIdx);
				i32ObjectIdx++;

				if(flaCollisionIndex.IsEmpty())
					continue;

				int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
				CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

				for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
					pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(0, 0, 255)); // BLUE

				pObj->UpdateAll();
				view3DExclude.UpdateObject(i);
			}

			view3DExclude.UpdateScreen();
		}

		// ROI를 모두 클리어
		roiUtil3DAdd.ClearROI();

		// 기존 선택 영역에 추가로 선택할 영역을 ROI로 설정
		CFLFrustum<float> flfrAdd;
		flfrAdd.Load(L"../../ExampleImages/ROIUtilities3D/frustumROI_Add.fig");
		roiUtil3DAdd.PushBackROI(&flfrAdd);

		// 선택 타입 설정 : 기존 결과에 ROI 안에 포함되는 정점을 추가
		roiUtil3DAdd.SetSelectionType(CROIUtilities3D::EResultSelectionType_Add);

		// CROIUtilities3D 실행
		if((res = roiUtil3DAdd.Execute()).IsFail())
			break;

		// CROIUtilities3D 에서 결과 얻어 오기
		CFLArray<CFLArray<int32_t>> flfaResultROIIndexAdd;
		if((res = roiUtil3DAdd.GetResult(flfaResultROIIndexAdd)).IsFail())
			break;

		if(flfaResultROIIndexAdd.GetCount())
		{
			int32_t i32ObjectIdx = 0;
			int32_t i32ObjectCount = view3DAdd.GetObjectCount();

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				CGUIView3DObject* pObj = (CGUIView3DObject*)view3DAdd.GetView3DObject(i);

				if(!pObj || !pObj->IsSelectionEnabled())
					continue;

				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				if(!pObjData)
					continue;

				CFLArray<int32_t>& flaCollisionIndex = flfaResultROIIndexAdd.GetAt(i32ObjectIdx);
				i32ObjectIdx++;

				if(flaCollisionIndex.IsEmpty())
					continue;

				int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
				CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

				for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
					pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(255, 0, 0)); // RED

				pObj->UpdateAll();
				view3DAdd.UpdateObject(i);
			}

			view3DAdd.PushBackROI(&flfrAdd);
			view3DAdd.UpdateScreen();
		}


		// ROI를 모두 클리어
		roiUtil3DRemove.ClearROI();

		// 기존 선택 영역에서 제거할 영역을 ROI로 설정
		CFLFrustum<float> flfrRemove1, flfrRemove2;
		flfrRemove1.Load(L"../../ExampleImages/ROIUtilities3D/frustumROI_Remove1.fig");
		flfrRemove2.Load(L"../../ExampleImages/ROIUtilities3D/frustumROI_Remove2.fig");
		roiUtil3DRemove.PushBackROI(&flfrRemove1);
		roiUtil3DRemove.PushBackROI(&flfrRemove2);

		// 선택 타입 설정 : 기존 결과에 ROI 안에 포함되는 정점을 추가
		roiUtil3DRemove.SetSelectionType(CROIUtilities3D::EResultSelectionType_Remove);

		// CROIUtilities3D 실행
		if((res = roiUtil3DRemove.Execute()).IsFail())
			break;

		// CROIUtilities3D 에서 결과 얻어 오기
		CFLArray<CFLArray<int32_t>> flfaResultROIIndexRemove;
		if((res = roiUtil3DRemove.GetResult(flfaResultROIIndexRemove)).IsFail())
			break;

		if(flfaResultROIIndexRemove.GetCount())
		{
			int32_t i32ObjectIdx = 0;
			int32_t i32ObjectCount = view3DRemove.GetObjectCount();

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				CGUIView3DObject* pObj = (CGUIView3DObject*)view3DRemove.GetView3DObject(i);

				if(!pObj || !pObj->IsSelectionEnabled())
					continue;

				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				if(!pObjData)
					continue;

				CFLArray<int32_t>& flaCollisionIndex = flfaResultROIIndexRemove.GetAt(i32ObjectIdx);
				i32ObjectIdx++;

				if(flaCollisionIndex.IsEmpty())
					continue;

				int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
				CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

				for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
					pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(0, 0, 255)); // BLUE

				pObj->UpdateAll();
				view3DRemove.UpdateObject(i);
			}

			view3DRemove.PushBackROI(&flfrRemove1);
			view3DRemove.PushBackROI(&flfrRemove2);
			view3DRemove.UpdateScreen();
		}

		// ROI를 모두 클리어
		roiUtil3DXOR.ClearROI();

		// 기존 선택 영역에서 XOR 선택할 영역을 ROI로 설정
		CFLFrustum<float> flfrXOR;
		flfrXOR.Load(L"../../ExampleImages/ROIUtilities3D/frustumROI_XOR.fig");
		roiUtil3DXOR.PushBackROI(&flfrXOR);

		// 선택 타입 설정 : 기존 결과에 ROI 안에 포함되는 정점을 추가
		roiUtil3DXOR.SetSelectionType(CROIUtilities3D::EResultSelectionType_XOR);

		// CROIUtilities3D 실행
		if((res = roiUtil3DXOR.Execute()).IsFail())
			break;

		// CROIUtilities3D 에서 결과 얻어 오기
		CFLArray<CFLArray<int32_t>> flfaResultROIIndexXOR;
		if((res = roiUtil3DXOR.GetResult(flfaResultROIIndexXOR)).IsFail())
			break;

		if(flfaResultROIIndexXOR.GetCount())
		{
			int32_t i32ObjectIdx = 0;
			int32_t i32ObjectCount = view3DXOR.GetObjectCount();

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				CGUIView3DObject* pObj = (CGUIView3DObject*)view3DXOR.GetView3DObject(i);

				if(!pObj || !pObj->IsSelectionEnabled())
					continue;

				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				if(!pObjData)
					continue;

				CFLArray<int32_t>& flaCollisionIndex = flfaResultROIIndexXOR.GetAt(i32ObjectIdx);
				i32ObjectIdx++;

				if(flaCollisionIndex.IsEmpty())
					continue;

				int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
				CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

				for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
					pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(0, 0, 255)); // BLUE

				pObj->UpdateAll();
				view3DXOR.UpdateObject(i);
			}

			view3DXOR.PushBackROI(&flfrXOR);
			view3DXOR.UpdateScreen();
		}

		// 아래 함수 DrawTextCanvas()는 screen좌표를 기준으로 하는 string을 drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		CFLPoint<double> flp(0, 0);
		if(IsFail(res = view3DSrc.GetLayer(0).DrawTextCanvas(&flp, L"Source", YELLOW, BLACK, 24)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3DInclude.GetLayer(0).DrawTextCanvas(&flp, L"Include", YELLOW, BLACK, 24)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3DExclude.GetLayer(0).DrawTextCanvas(&flp, L"Exclude", YELLOW, BLACK, 24)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3DAdd.GetLayer(0).DrawTextCanvas(&flp, L"Add(Include+Add)", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3DRemove.GetLayer(0).DrawTextCanvas(&flp, L"Remove(Exclude-Remove)", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3DXOR.GetLayer(0).DrawTextCanvas(&flp, L"XOR(Exclude^XOR)", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// 3D 뷰를 갱신 // Update 3D view
		for(int32_t i = 0; i < 6; ++i)
			arrView3D[i]->Invalidate(true);

		// 3D 뷰가 종료될 때 까지 기다림 // Wait for the 3D view 
		while(arrView3D[0]->IsAvailable() && arrView3D[1]->IsAvailable() && arrView3D[2]->IsAvailable() && arrView3D[3]->IsAvailable() && arrView3D[4]->IsAvailable() && arrView3D[5]->IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}