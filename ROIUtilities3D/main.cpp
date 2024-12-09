#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"
#include "../CommomHeader/ErrorMessageBox.h"


int main()
{
	// 3D �� ����
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
		// 3D �� ���� // Create the 3D view
		view3DInclude.Create(0, 0, 300, 300); // L, T, R, B(Left, Top, Right, Bottom) 
		view3DSrc.Create(300, 0, 600, 300);
		view3DExclude.Create(600, 0, 900, 300);
		view3DAdd.Create(0, 300, 300, 600);
		view3DRemove.Create(300, 300, 600, 600);
		view3DXOR.Create(600, 300, 900, 600);

		// 3D ��ü ���� // Declare a 3D object
		CFL3DObject fl3DObjLeft, fl3DObjRight;
		// 3D ��ü �ε� // Load a 3D object
		res = fl3DObjLeft.Load(L"../../ExampleImages/ROIUtilities3D/Left Cam.ply");
		res = fl3DObjRight.Load(L"../../ExampleImages/ROIUtilities3D/Right Cam.ply");
		
		for(int32_t i = 0; i < 6; ++i)
		{
			// 3D �信 3D ��ü �߰� // Add 3D objects to the 3D view
			arrView3D[i]->PushObject(fl3DObjLeft);
			arrView3D[i]->PushObject(fl3DObjRight);

			// �߰��� 3D ��ü�� ȭ�� �ȿ� �������� Zoom Fit // Perform Zoom Fit to ensure added 3D objects are within the view
			arrView3D[i]->ZoomFit();

			// 3D ����� ����(ī�޶�) ���� // Change the viewpoint (camera) of the 3D viewer
			arrView3D[i]->GetCamera()->SetPosition(CFLPoint3F(0.71, 0.02, 10.94));
			arrView3D[i]->GetCamera()->SetDirectionUp(CFLPoint3F(1., 0., 0.));

			if(i > 0)
			{
				// 3D �� ���� ����ȭ // Synchronize the viewpoint of the 3D view
				arrView3D[i]->SynchronizePointOfView(arrView3D[i - 1]);
				// ������ ����ȭ // Synchronize the window
				arrView3D[i]->SynchronizeWindow(arrView3D[i - 1]);
			}
		}

		// ����ü ROI ���� // Declare the frustum ROI
		CFLFrustum<float> flfr;
		// ���Ͽ��� ����ü ROI �ε� // Load the frustum ROI from a file
		flfr.Load(L"../../ExampleImages/ROIUtilities3D/frustumROI.fig");

		// 3D �信 ROI �߰� // Add the ROI to the 3D view
		for(int32_t i = 1; i < 6; ++i)
			arrView3D[i]->PushBackROI(&flfr);

		// CROIUtilities3D ��ü ���� // Declare the CROIUtilities3D object
		CROIUtilities3D roiUtil3D;

		// CROIUtilities3D ��ü�� 3D Object �߰� // Add 3D objects to the CROIUtilities3D object
		roiUtil3D.PushBack3DObject(&fl3DObjLeft);
		roiUtil3D.PushBack3DObject(&fl3DObjRight);

		// CROIUtilities3D ��ü�� ����ü ROI �߰� // Add the frustum ROI to the CROIUtilities3D object
		roiUtil3D.PushBackROI(flfr);

		// ���� Ÿ�� ���� : ROI �ȿ� ���ԵǴ� ������ ���� // Set the selection type to include only vertices inside the ROI
		roiUtil3D.SetSelectionType(CROIUtilities3D::EResultSelectionType_Include);

		// CROIUtilities3D ���� // Execute the CROIUtilities3D object
		if((res = roiUtil3D.Execute()).IsFail())
			break;

		// CROIUtilities3D ���� ��� ��� ���� // Retrieve the results from CROIUtilities3D
		CFLArray<CFLArray<int32_t>> flfaResultROIIndexInclude;
		if((res = roiUtil3D.GetResult(flfaResultROIIndexInclude)).IsFail())
			break;

		// EResultSelectionType.Add ������ ���� CROIUtilities3D ��ü ���� �� roiUtil3D �� ���� ����. Include �������� ���� ��������� �����
		// Declare and copy construct a CROIUtilities3D object for the EResultSelectionType.Add operation. 
		// The results from the Include operation are also copied.
		CROIUtilities3D roiUtil3DAdd = roiUtil3D;

		// ������ ��ü���� ROI�� ��� Ŭ���� // Clear all ROIs from the copied object
		roiUtil3DAdd.ClearROI();

		if(flfaResultROIIndexInclude.GetCount())
		{
			int32_t i32ObjectIdx = 0;

			// 3D �� �߰��� 3D ��ü�� ���� // Number of 3D objects added to the 3D viewer
			int32_t i32ObjectCount = view3DInclude.GetObjectCount();

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				// 3D �� �߰��� i��° 3D ��ü // The i-th 3D object added to the 3D viewer
				CGUIView3DObject* pObj = (CGUIView3DObject*)view3DInclude.GetView3DObject(i);

				// �ش� ��ü�� ���ų�, �ش� ��ü�� ���� ������ ��Ȱ��ȭ �Ǿ� �ִٸ� continue // Skip if the object is null or selection is disabled
				if(!pObj || !pObj->IsSelectionEnabled())
					continue;

				// i��° 3D ��ü�� ������(CFL3DObject) // Data of the i-th 3D object (CFL3DObject)
				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				// �ش� ��ü�� ���ٸ� continue // Skip if the object data is null
				if(!pObjData)
					continue;

				// i��° 3D ��ü�� ���� ����� �迭. �� �迭�� i��° 3D ��ü�� ����, ROI ���ο� ��ġ�� ������ �ε����� �̷���� // Result array for the i-th 3D object. Contains indices of vertices within the ROI.
				CFLArray<int32_t>& flaCollisionIndex = flfaResultROIIndexInclude.GetAt(i32ObjectIdx);
				i32ObjectIdx++;

				if(flaCollisionIndex.IsEmpty())
					continue;

				int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
				CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

				// i��° 3D ��ü�� ����, ROI ���ο� ��ġ�� ������ ���������� ǥ�� // Mark vertices within the ROI of the i-th 3D object in red
				for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
					pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(255, 0, 0)); // RED

				// 3D �� �߰��� i��° 3D ��ü�� ���� ������ ������Ʈ // Update rendering for the i-th 3D object added to the 3D viewer
				pObj->UpdateAll();
				view3DInclude.UpdateObject(i);
			}

			// 3D ��� ������Ʈ // Update the 3D viewer
			view3DInclude.UpdateScreen();
		}

		// ���� Ÿ�� ���� : ROI �ٱ��� ������ ���� // Set selection type: Select only vertices outside the ROI
		roiUtil3D.SetSelectionType(CROIUtilities3D::EResultSelectionType_Exclude);

		// CROIUtilities3D ���� // Execute CROIUtilities3D
		if((res = roiUtil3D.Execute()).IsFail())
			break;

		// CROIUtilities3D ���� ��� ��� ���� // Retrieve results from CROIUtilities3D
		CFLArray<CFLArray<int32_t>> flfaResultROIIndexExclude;
		if((res = roiUtil3D.GetResult(flfaResultROIIndexExclude)).IsFail())
			break;

		// EResultSelectionType.Remove ������ ���� CROIUtilities3D ��ü ���� �� roiUtil3D �� ���� ����. Exclude �������� ���� ��������� ����� // Declare a CROIUtilities3D object for EResultSelectionType.Remove operation and copy roiUtil3D. Results from the Exclude operation are also copied.
		CROIUtilities3D roiUtil3DRemove = roiUtil3D;
		// EResultSelectionType.XOR ������ ���� CROIUtilities3D ��ü ���� �� roiUtil3D �� ���� ����. Exclude �������� ���� ��������� ����� // Declare a CROIUtilities3D object for EResultSelectionType.XOR operation and copy roiUtil3D. Results from the Exclude operation are also copied.
		CROIUtilities3D roiUtil3DXOR = roiUtil3D;

		// ������ ��ü���� ROI�� ��� Ŭ���� // Clear all ROIs from the copied objects
		roiUtil3DRemove.ClearROI();
		roiUtil3DXOR.ClearROI();

		if(flfaResultROIIndexExclude.GetCount())
		{
			int32_t i32ObjectIdx = 0;

			// 3D �� �߰��� 3D ��ü�� ���� // Number of 3D objects added to the 3D viewer
			int32_t i32ObjectCount = view3DExclude.GetObjectCount();

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				// 3D �� �߰��� i��° 3D ��ü // The i-th 3D object added to the 3D viewer
				CGUIView3DObject* pObj = (CGUIView3DObject*)view3DExclude.GetView3DObject(i);

				// �ش� ��ü�� ���ų�, �ش� ��ü�� ���� ������ ��Ȱ��ȭ �Ǿ� �ִٸ� continue // Skip if the object is null or selection is disabled
				if(!pObj || !pObj->IsSelectionEnabled())
					continue;

				// i��° 3D ��ü�� ������(CFL3DObject) // Data of the i-th 3D object (CFL3DObject)
				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				if(!pObjData)
					continue;

				// �ش� ��ü�� ���ٸ� continue // Skip if the object data is null
				CFLArray<int32_t>& flaCollisionIndex = flfaResultROIIndexExclude.GetAt(i32ObjectIdx);
				i32ObjectIdx++;

				// i��° 3D ��ü�� ���� ����� �迭. �� �迭�� i��° 3D ��ü�� ����, ROI �ܺο� ��ġ�� ������ �ε����� �̷���� // Result array for the i-th 3D object. Contains indices of vertices outside the ROI.
				if(flaCollisionIndex.IsEmpty())
					continue;

				int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
				CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

				// i��° 3D ��ü�� ����, ROI �ٱ��� ��ġ�� ������ �Ķ������� ǥ�� // Mark vertices outside the ROI of the i-th 3D object in blue
				for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
					pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(0, 0, 255)); // BLUE

				// 3D �� �߰��� i��° 3D ��ü�� ���� ������ ������Ʈ // Update rendering for the i-th 3D object added to the 3D viewer
				pObj->UpdateAll();
				view3DExclude.UpdateObject(i);
			}

			// 3D ��� ������Ʈ // Update the 3D viewer
			view3DExclude.UpdateScreen();
		}

		// ���� ���� ����(������ Include�� ������ ����)�� �߰��� ������ ������ ROI�� ���� // Set an additional ROI to be selected in the existing selection area (previously selected with Include)
		CFLFrustum<float> flfrAdd;
		flfrAdd.Load(L"../../ExampleImages/ROIUtilities3D/frustumROI_Add.fig");
		// CROIUtilities3D ��ü�� ����ü ROI �߰� // Add the frustum ROI to the CROIUtilities3D object
		roiUtil3DAdd.PushBackROI(&flfrAdd);
		// 3D �信 ROI �߰� // Add the frustum ROI to the 3D view
		view3DAdd.PushBackROI(&flfrAdd);

		// ���� Ÿ�� ���� : ���� ����� ROI �ȿ� ���ԵǴ� ������ �߰� // Set selection type: Add vertices within the ROI to the existing results
		roiUtil3DAdd.SetSelectionType(CROIUtilities3D::EResultSelectionType_Add);

		// CROIUtilities3D ���� // Execute CROIUtilities3D
		if((res = roiUtil3DAdd.Execute()).IsFail())
			break;

		// CROIUtilities3D ���� ��� ��� ���� // Retrieve results from CROIUtilities3D
		CFLArray<CFLArray<int32_t>> flfaResultROIIndexAdd;
		if((res = roiUtil3DAdd.GetResult(flfaResultROIIndexAdd)).IsFail())
			break;

		if(flfaResultROIIndexAdd.GetCount())
		{
			int32_t i32ObjectIdx = 0;
			// 3D �� �߰��� 3D ��ü�� ���� // Number of 3D objects added to the 3D viewer
			int32_t i32ObjectCount = view3DAdd.GetObjectCount();

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				// 3D �� �߰��� i��° 3D ��ü // The i-th 3D object added to the 3D viewer
				CGUIView3DObject* pObj = (CGUIView3DObject*)view3DAdd.GetView3DObject(i);

				// �ش� ��ü�� ���ų�, �ش� ��ü�� ���� ������ ��Ȱ��ȭ �Ǿ� �ִٸ� continue // Skip if the object is null or selection is disabled
				if(!pObj || !pObj->IsSelectionEnabled())
					continue;

				// i��° 3D ��ü�� ������(CFL3DObject) // Data of the i-th 3D object (CFL3DObject)
				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				// �ش� ��ü�� ���ٸ� continue // Skip if the object data is null
				if(!pObjData)
					continue;

				// i��° 3D ��ü�� ���� ����� �迭. // Result array for the i-th 3D object
				CFLArray<int32_t>& flaCollisionIndex = flfaResultROIIndexAdd.GetAt(i32ObjectIdx);
				i32ObjectIdx++;

				if(flaCollisionIndex.IsEmpty())
					continue;

				int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
				CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

				// i��° 3D ��ü�� ����, ROI ���ο� ��ġ�� ������ ���������� ǥ�� // Mark vertices within the ROI of the i-th 3D object in red
				for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
					pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(255, 0, 0)); // RED

				// 3D �� �߰��� i��° 3D ��ü�� ���� ������ ������Ʈ // Update rendering for the i-th 3D object added to the 3D viewer
				pObj->UpdateAll();
				view3DAdd.UpdateObject(i);
			}

			// 3D ��� ������Ʈ // Update the 3D viewer
			view3DAdd.UpdateScreen();
		}

		// ���� ���� ����(������ Exclude�� ������ ����)���� ������ ������ ROI�� ���� // Set ROIs to remove areas from the existing selection (previously selected with Exclude)
		CFLFrustum<float> flfrRemove1, flfrRemove2;
		flfrRemove1.Load(L"../../ExampleImages/ROIUtilities3D/frustumROI_Remove1.fig");
		flfrRemove2.Load(L"../../ExampleImages/ROIUtilities3D/frustumROI_Remove2.fig");
		// CROIUtilities3D ��ü�� ����ü ROI �߰� // Add the frustum ROIs to the CROIUtilities3D object
		roiUtil3DRemove.PushBackROI(flfrRemove1);
		roiUtil3DRemove.PushBackROI(flfrRemove2);
		// 3D �信 ROI �߰� // Add the frustum ROIs to the 3D view
		view3DRemove.PushBackROI(&flfrRemove1);
		view3DRemove.PushBackROI(&flfrRemove2);

		// ���� Ÿ�� ���� : ���� ������� ROI ���� ������ ���� // Set selection type: Remove vertices within the ROI from the existing results
		roiUtil3DRemove.SetSelectionType(CROIUtilities3D::EResultSelectionType_Remove);

		// CROIUtilities3D ���� // Execute CROIUtilities3D
		if((res = roiUtil3DRemove.Execute()).IsFail())
			break;

		// CROIUtilities3D ���� ��� ��� ���� // Retrieve results from CROIUtilities3D
		CFLArray<CFLArray<int32_t>> flfaResultROIIndexRemove;
		if((res = roiUtil3DRemove.GetResult(flfaResultROIIndexRemove)).IsFail())
			break;

		if(flfaResultROIIndexRemove.GetCount())
		{
			int32_t i32ObjectIdx = 0;

			// 3D �� �߰��� 3D ��ü�� ���� // Number of 3D objects added to the 3D viewer
			int32_t i32ObjectCount = view3DRemove.GetObjectCount();

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				// 3D �� �߰��� i��° 3D ��ü // The i-th 3D object added to the 3D viewer
				CGUIView3DObject* pObj = (CGUIView3DObject*)view3DRemove.GetView3DObject(i);

				// �ش� ��ü�� ���ų�, �ش� ��ü�� ���� ������ ��Ȱ��ȭ �Ǿ� �ִٸ� continue // Skip if the object is null or selection is disabled
				if(!pObj || !pObj->IsSelectionEnabled())
					continue;

				// i��° 3D ��ü�� ������(CFL3DObject) // Data of the i-th 3D object (CFL3DObject)
				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				// �ش� ��ü�� ���ٸ� continue // Skip if the object data is null
				if(!pObjData)
					continue;

				// i��° 3D ��ü�� ���� ����� �迭. // Result array for the i-th 3D object
				CFLArray<int32_t>& flaCollisionIndex = flfaResultROIIndexRemove.GetAt(i32ObjectIdx);
				i32ObjectIdx++;

				if(flaCollisionIndex.IsEmpty())
					continue;

				int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
				CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

				// i��° 3D ��ü�� ����, ���� ������� ROI ���� ������ ���� �� ���õ� ������ �Ķ������� ǥ�� // Mark selected vertices after removing vertices within the ROI of the i-th 3D object in blue
				for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
					pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(0, 0, 255)); // BLUE

				// 3D �� �߰��� i��° 3D ��ü�� ���� ������ ������Ʈ // Update rendering for the i-th 3D object added to the 3D viewer
				pObj->UpdateAll();
				view3DRemove.UpdateObject(i);
			}

			// 3D ��� ������Ʈ // Update the 3D viewer
			view3DRemove.UpdateScreen();
		}

		// ���� ���� ����(������ Exclude�� ������ ����)���� XOR ������ ������ ROI�� ���� // Set an ROI to perform XOR operation on the existing selection (previously selected with Exclude)
		CFLFrustum<float> flfrXOR;
		flfrXOR.Load(L"../../ExampleImages/ROIUtilities3D/frustumROI_XOR.fig");
		// CROIUtilities3D ��ü�� ����ü ROI �߰� // Add the frustum ROI to the CROIUtilities3D object
		roiUtil3DXOR.PushBackROI(&flfrXOR);
		// 3D �信 ROI �߰� // Add the frustum ROI to the 3D view
		view3DXOR.PushBackROI(&flfrXOR);

		// ���� Ÿ�� ���� : ���� ������� ROI ���� ������ XOR �����Ͽ� ���� // Set selection type: Perform XOR operation with vertices inside the ROI on the existing results
		roiUtil3DXOR.SetSelectionType(CROIUtilities3D::EResultSelectionType_XOR);

		// CROIUtilities3D ���� // Execute CROIUtilities3D
		if((res = roiUtil3DXOR.Execute()).IsFail())
			break;

		// CROIUtilities3D ���� ��� ��� ���� // Retrieve results from CROIUtilities3D
		CFLArray<CFLArray<int32_t>> flfaResultROIIndexXOR;
		if((res = roiUtil3DXOR.GetResult(flfaResultROIIndexXOR)).IsFail())
			break;

		if(flfaResultROIIndexXOR.GetCount())
		{
			int32_t i32ObjectIdx = 0;
			// 3D �� �߰��� 3D ��ü�� ���� // Number of 3D objects added to the 3D viewer
			int32_t i32ObjectCount = view3DXOR.GetObjectCount();

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				// 3D �� �߰��� i��° 3D ��ü // The i-th 3D object added to the 3D viewer
				CGUIView3DObject* pObj = (CGUIView3DObject*)view3DXOR.GetView3DObject(i);

				// �ش� ��ü�� ���ų�, �ش� ��ü�� ���� ������ ��Ȱ��ȭ �Ǿ� �ִٸ� continue // Skip if the object is null or selection is disabled
				if(!pObj || !pObj->IsSelectionEnabled())
					continue;

				// i��° 3D ��ü�� ������(CFL3DObject) // Data of the i-th 3D object (CFL3DObject)
				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				// �ش� ��ü�� ���ٸ� continue // Skip if the object data is null
				if(!pObjData)
					continue;

				// i��° 3D ��ü�� ���� ����� �迭. // Result array for the i-th 3D object
				CFLArray<int32_t>& flaCollisionIndex = flfaResultROIIndexXOR.GetAt(i32ObjectIdx);
				i32ObjectIdx++;

				if(flaCollisionIndex.IsEmpty())
					continue;

				int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
				CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

				// i��° 3D ��ü�� ����, ���� ������� ROI ���� ������ XOR ������ ��� �������� �Ķ������� ǥ�� // Mark the vertices resulting from XOR operation within the ROI of the i-th 3D object in blue
				for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
					pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(0, 0, 255)); // BLUE

				// 3D �� �߰��� i��° 3D ��ü�� ���� ������ ������Ʈ // Update rendering for the i-th 3D object added to the 3D viewer
				pObj->UpdateAll();
				view3DXOR.UpdateObject(i);
			}

			// 3D ��� ������Ʈ // Update the 3D viewer
			view3DXOR.UpdateScreen();
		}

		// �Ʒ� �Լ� DrawTextCanvas()�� screen��ǥ�� �������� �ϴ� string�� drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		CFLPoint<double> flp(0, 0);
		if(IsFail(res = view3DSrc.GetLayer(0).DrawTextCanvas(&flp, L"Source", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3DInclude.GetLayer(0).DrawTextCanvas(&flp, L"Include", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3DExclude.GetLayer(0).DrawTextCanvas(&flp, L"Exclude", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3DAdd.GetLayer(0).DrawTextCanvas(&flp, L"Add(Include Result+Add)", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3DRemove.GetLayer(0).DrawTextCanvas(&flp, L"Remove(Exclude Result-Remove)", YELLOW, BLACK, 18)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = view3DXOR.GetLayer(0).DrawTextCanvas(&flp, L"XOR(Exclude Result^XOR)", YELLOW, BLACK, 20)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// 3D �並 ���� // Update 3D view
		for(int32_t i = 0; i < 6; ++i)
			arrView3D[i]->Invalidate(true);

		// 3D �䰡 ����� �� ���� ��ٸ� // Wait for the 3D view 
		while(arrView3D[0]->IsAvailable() && arrView3D[1]->IsAvailable() && arrView3D[2]->IsAvailable() && arrView3D[3]->IsAvailable() && arrView3D[4]->IsAvailable() && arrView3D[5]->IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}