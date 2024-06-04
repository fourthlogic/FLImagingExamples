#include "stdafx.h"
#include "PropertyView3DExamples.h"

using namespace FLImaging;
using namespace FLImaging::Base;
using namespace FLImaging::GUI;
using namespace FLImaging::Foundation;

CPropertyView3DExamples::CPropertyView3DExamples()
{
}

CPropertyView3DExamples::~CPropertyView3DExamples()
{
}

const CResult CPropertyView3DExamples::ConfigureMenu()
{
	CResult cReturn = EResult_UnknownError;

	do
	{
		EnableStatusMessageBox(true);
		SetMessageDisplayMode(EMessageDisplayMode_AutoScroll);

		CGUIPropertyItemCategory* pCat = new CGUIPropertyItemCategory;
		pCat->SetName(L"PushObject");
		AddItem(pCat);
		{
			CGUIPropertyItemView3DList* pIVL = new CGUIPropertyItemView3DList;
			pIVL->SetPath(L"PushObject");
			pIVL->SetName(L"3D View");
			AddItem(pIVL);

			CGUIPropertyItemButton* pPIButton = nullptr;
			CGUIPropertyItemDropdownList* pPIDD = nullptr;

			pPIDD = new CGUIPropertyItemDropdownList;
			pPIDD->SetPath(L"PushObject");
			pPIDD->SetName(L"Object Type");
			pPIDD->SetDescription(L"Select the type of object to be displayed in the 3D view.\n\
								  3D �信 ���÷��� �� ��ü Ÿ���� �����մϴ�.");

			pPIDD->AddListItem(L"PLY File");
			pPIDD->AddListItem(L"Image(Height Map)");
			pPIDD->AddListItem(L"Image(Height Map & Texture)");
			pPIDD->AddListItem(L"3D Line");
			pPIDD->AddListItem(L"Sphere");
			pPIDD->AddListItem(L"Unselectable Axis", L"3D Axis(x,y,z) which is unselectable");

			pPIDD->SetDefaultValue(L"PLY File");
			AddItem(pPIDD);

			pPIButton = new CGUIPropertyItemButton;
			pPIButton->SetPath(L"PushObject");
			pPIButton->SetName(L"PushObject");
			pPIButton->SetButtonName(L"PushObject");
			pPIButton->SetPropertyButtonClickProcedure(PushObject());
			pPIButton->SetDescription(L"An example of adding an object to be displayed in a 3D view.\n\
									  3D �信 ���÷��� �� ��ü�� �߰��ϴ� �����Դϴ�.");
			AddItem(pPIButton);
		}

		pCat = new CGUIPropertyItemCategory;
		pCat->SetName(L"ROI");
		AddItem(pCat);
		{
			CGUIPropertyItemView3DList* pIVL = new CGUIPropertyItemView3DList;
			pIVL->SetPath(L"ROI");
			pIVL->SetName(L"3D View");
			AddItem(pIVL);

			CGUIPropertyItemButton* pPIButton = nullptr;

			pPIButton = new CGUIPropertyItemButton;
			pPIButton->SetPath(L"ROI");
			pPIButton->SetName(L"ROIUtilities3D");
			pPIButton->SetButtonName(L"Execute");
			pPIButton->SetDescription(L"Example of CROIUtilities3D");
			pPIButton->SetPropertyButtonClickProcedure(ROIUtilities3DTest());
			pPIButton->SetDescription(L"An example of using CROIUtilities3D.\n\
									  CROIUtilities3D ��뿡 ���� �����Դϴ�.");
			AddItem(pPIButton);

			pPIButton = new CGUIPropertyItemButton;
			pPIButton->SetPath(L"ROI");
			pPIButton->SetName(L"Frustum Collision Test");
			pPIButton->SetButtonName(L"Execute");
			pPIButton->SetPropertyButtonClickProcedure(FrustumCollisionTest());
			pPIButton->SetDescription(L"An example of testing for collisions between a frustum and 3D objects.\n\
									  ����ü�� 3D ��ü ���� �浹 ���� �׽�Ʈ �����Դϴ�.");
			AddItem(pPIButton);
		}

		pCat = new CGUIPropertyItemCategory;
		pCat->SetName(L"Transform");
		AddItem(pCat);
		{
			CGUIPropertyItemView3DList* pIVL = new CGUIPropertyItemView3DList;
			pIVL->SetPath(L"Transform");
			pIVL->SetName(L"Source View");
			AddItem(pIVL);

			pIVL = new CGUIPropertyItemView3DList;
			pIVL->SetPath(L"Transform");
			pIVL->SetName(L"Target View");
			AddItem(pIVL);

			CGUIPropertyItemDropdownList* pPIDD = new CGUIPropertyItemDropdownList;
			pPIDD->SetPath(L"Transform");
			pPIDD->SetName(L"Transform Type");

			pPIDD->AddListItem(L"Rotate");
			pPIDD->AddListItem(L"Rotate with Pivot");
			pPIDD->AddListItem(L"Translate");
			pPIDD->AddListItem(L"Scale");

			pPIDD->SetDefaultValue(L"Rotate");
			AddItem(pPIDD);

			CGUIPropertyItemButton* pPIButton = nullptr;

			pPIButton = new CGUIPropertyItemButton;
			pPIButton->SetPath(L"Transform");
			pPIButton->SetName(L"FL3DObjectUtilities");
			pPIButton->SetButtonName(L"Execute");
			pPIButton->SetDescription(L"Example of CFL3DObjectUtilities");
			pPIButton->SetPropertyButtonClickProcedure(FL3DObjectUtilitiesTest());
			pPIButton->SetDescription(L"An example of using CFL3DObjectUtilities, which supports transformation functions such as object rotation, scaling, and translation.\n\
									  ��ü ȸ��, ������, �̵� �� ��ȯ ����� �����ϴ� CFL3DObjectUtilities ��뿡 ���� �����Դϴ�.");
			AddItem(pPIButton);
		}

		pCat = new CGUIPropertyItemCategory;
		pCat->SetName(L"Camera");
		AddItem(pCat);
		{
			CGUIPropertyItemView3DList* pIVL = new CGUIPropertyItemView3DList;
			pIVL->SetPath(L"Camera");
			pIVL->SetName(L"3D View");
			AddItem(pIVL);

			CGUIPropertyItemButton* pPIButton = nullptr;

			CGUIPropertyItemCategory* pCatValue = new CGUIPropertyItemCategory;
			pCatValue->SetName(L"Position");
			pCatValue->SetPath(L"Camera");
			pCatValue->SetValueList(true);
			pCatValue->SetExpand(false);
			AddItem(pCatValue);
			{
				CGUIPropertyItemText* pText = new CGUIPropertyItemText;
				pText->SetName(L"x");
				pText->SetInputType(CGUIEditBase::EInputType_Double);
				pText->SetPath(L"Camera@Position");
				pText->SetDefaultValue(L"0");
				AddItem(pText);

				pText = new CGUIPropertyItemText;
				pText->SetName(L"y");
				pText->SetInputType(CGUIEditBase::EInputType_Double);
				pText->SetPath(L"Camera@Position");
				pText->SetDefaultValue(L"0");
				AddItem(pText);

				pText = new CGUIPropertyItemText;
				pText->SetName(L"z");
				pText->SetInputType(CGUIEditBase::EInputType_Double);
				pText->SetPath(L"Camera@Position");
				pText->SetDefaultValue(L"0");
				AddItem(pText);
			}

			pCatValue = new CGUIPropertyItemCategory;
			pCatValue->SetName(L"Direction");
			pCatValue->SetPath(L"Camera");
			pCatValue->SetValueList(true);
			pCatValue->SetExpand(false);
			AddItem(pCatValue);
			{
				CGUIPropertyItemText* pText = new CGUIPropertyItemText;
				pText->SetName(L"x");
				pText->SetInputType(CGUIEditBase::EInputType_Double);
				pText->SetPath(L"Camera@Direction");
				pText->SetDefaultValue(L"0");
				AddItem(pText);

				pText = new CGUIPropertyItemText;
				pText->SetName(L"y");
				pText->SetInputType(CGUIEditBase::EInputType_Double);
				pText->SetPath(L"Camera@Direction");
				pText->SetDefaultValue(L"0");
				AddItem(pText);

				pText = new CGUIPropertyItemText;
				pText->SetName(L"z");
				pText->SetInputType(CGUIEditBase::EInputType_Double);
				pText->SetPath(L"Camera@Direction");
				pText->SetDefaultValue(L"0");
				AddItem(pText);
			}

			pCatValue = new CGUIPropertyItemCategory;
			pCatValue->SetName(L"Up Direction");
			pCatValue->SetPath(L"Camera");
			pCatValue->SetValueList(true);
			pCatValue->SetExpand(false);
			AddItem(pCatValue);
			{
				CGUIPropertyItemText* pText = new CGUIPropertyItemText;
				pText->SetName(L"x");
				pText->SetInputType(CGUIEditBase::EInputType_Double);
				pText->SetPath(L"Camera@Up Direction");
				pText->SetDefaultValue(L"0");
				AddItem(pText);

				pText = new CGUIPropertyItemText;
				pText->SetName(L"y");
				pText->SetInputType(CGUIEditBase::EInputType_Double);
				pText->SetPath(L"Camera@Up Direction");
				pText->SetDefaultValue(L"0");
				AddItem(pText);

				pText = new CGUIPropertyItemText;
				pText->SetName(L"z");
				pText->SetInputType(CGUIEditBase::EInputType_Double);
				pText->SetPath(L"Camera@Up Direction");
				pText->SetDefaultValue(L"0");
				AddItem(pText);
			}

			pCatValue = new CGUIPropertyItemCategory;
			pCatValue->SetName(L"Target");
			pCatValue->SetPath(L"Camera");
			pCatValue->SetValueList(true);
			pCatValue->SetExpand(false);
			AddItem(pCatValue);
			{
				CGUIPropertyItemText* pText = new CGUIPropertyItemText;
				pText->SetName(L"x");
				pText->SetInputType(CGUIEditBase::EInputType_Double);
				pText->SetPath(L"Camera@Target");
				pText->SetDefaultValue(L"0");
				AddItem(pText);

				pText = new CGUIPropertyItemText;
				pText->SetName(L"y");
				pText->SetInputType(CGUIEditBase::EInputType_Double);
				pText->SetPath(L"Camera@Target");
				pText->SetDefaultValue(L"0");
				AddItem(pText);

				pText = new CGUIPropertyItemText;
				pText->SetName(L"z");
				pText->SetInputType(CGUIEditBase::EInputType_Double);
				pText->SetPath(L"Camera@Target");
				pText->SetDefaultValue(L"0");
				AddItem(pText);
			}

			CGUIPropertyItemText* pText = new CGUIPropertyItemText;
			pText->SetName(L"Angle of View(Degree)");
			pText->SetDescription(L"Input the angle of view in degree(not radian)");
			pText->SetInputType(CGUIEditBase::EInputType_Double);
			pText->SetPath(L"Camera");
			pText->SetDefaultValue(L"45");
			AddItem(pText);

			pPIButton = new CGUIPropertyItemButton;
			pPIButton->SetPath(L"Camera");
			pPIButton->SetName(L"Set Parameters");
			pPIButton->SetButtonName(L"Apply");
			pPIButton->SetPropertyButtonClickProcedure(SetCameraParameters());
			pPIButton->SetDescription(L"An example of using the camera object (CGUIView3DCamera).\n\
									  ī�޶� ��ü(CGUIView3DCamera) ��뿡 ���� �����Դϴ�.");
			AddItem(pPIButton);
		}

		cReturn = EResult_OK;
	}
	while(false);

	return cReturn;
}

CPropertyItemButtonClickProcedure* FLImaging::GUI::CPropertyView3DExamples::PushObject()
{
	CPropertyItemButtonClickProcedure* pProcedure = new CPropertyItemButtonClickProcedure;

	*pProcedure = MakePropertyItemButtonClickProcedure
	{
		do
		{
			CGUIPropertyItemView3DList* pViewList = dynamic_cast<CGUIPropertyItemView3DList*>(FindItemByFullPath(L"PushObject@3D View"));

			if(!pViewList)
				break;

			CGUIView3D* pView3D = pViewList->GetSelectedView3D();

			if(!pView3D)
			{
				SetStatusMessage(L"[Error] Select 3D view.");
				break;
			}

			CGUIPropertyItemDropdownList* pPIDD = (CGUIPropertyItemDropdownList*)FindItemByFullPath(L"PushObject@Object Type");

			if(!pPIDD)
				break;

			CFLString<wchar_t> str = pPIDD->GetValue();

			CResult res;

			if(str == L"PLY File")
				res = PushObjectPLY(pView3D);
			else if(str == L"Image(Height Map)")
				res = PushObjectHeightMap(pView3D);
			else if(str == L"Image(Height Map & Texture)")
				res = PushObjectHeightMapAndTexture(pView3D);
			else if(str == L"3D Line")
				res = PushObjectLine(pView3D);
			else if(str == L"Sphere")
				res = PushObjectSphere(pView3D);
			else if(str == L"Unselectable Axis")
				res = PushObjectUnselectableAxis(pView3D);
		}
		while(false);
	};

	return pProcedure;
}

CPropertyItemButtonClickProcedure* FLImaging::GUI::CPropertyView3DExamples::ROIUtilities3DTest()
{
	CPropertyItemButtonClickProcedure* pProcedure = new CPropertyItemButtonClickProcedure;

	*pProcedure = MakePropertyItemButtonClickProcedure
	{
		CResult res;

		do
		{
			CGUIPropertyItemView3DList* pViewList = dynamic_cast<CGUIPropertyItemView3DList*>(FindItemByFullPath(L"ROI@3D View"));

			if(!pViewList)
				break;

			CGUIView3D* pView3D = pViewList->GetSelectedView3D();

			if(!pView3D)
			{
				SetStatusMessage(L"[Error] Select 3D view.");
				break;
			}

			int32_t i32ObjectCount = pView3D->GetObjectCount();

			if(!i32ObjectCount)
			{
				SetStatusMessage(L"[Error] " + pView3D->GetTitle() + L" doesn't have any 3D objects.");
				break;
			}

			std::vector<CFLFrustum<float>> vctFrustum;
			pView3D->GetAllROI(vctFrustum);

			if(vctFrustum.empty())
			{
				SetStatusMessage(L"[Error] There is no ROI in " + pView3D->GetTitle() + L". Press Ctrl+3 to teach ROI.");
				break;
			}

			CROIUtilities3D roiUtil3D;

			for(auto& frustum : vctFrustum)
				roiUtil3D.PushBackROI(frustum);

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				CGUIView3DObject* pObj = (CGUIView3DObject*)pView3D->GetView3DObject(i);

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
					CGUIView3DObject* pObj = (CGUIView3DObject*)pView3D->GetView3DObject(i);

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
					pView3D->UpdateObject(i);
				}
			}

			if(flfaResultROIIndexExclude.GetCount())
			{
				int32_t i32IndexRes = 0;

				for(int32_t i = 0; i < i32ObjectCount; ++i)
				{
					CGUIView3DObject* pObj = (CGUIView3DObject*)pView3D->GetView3DObject(i);

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
					pView3D->UpdateObject(i);
				}
			}

			pView3D->UpdateScreen();

			SetStatusMessage(GetResultString(EResult_OK));
		}
		while(false);
	};

	return pProcedure;
}

CPropertyItemButtonClickProcedure* FLImaging::GUI::CPropertyView3DExamples::FrustumCollisionTest()
{
	CPropertyItemButtonClickProcedure* pProcedure = new CPropertyItemButtonClickProcedure;

	*pProcedure = MakePropertyItemButtonClickProcedure
	{
		do
		{
			CGUIPropertyItemView3DList* pViewList = dynamic_cast<CGUIPropertyItemView3DList*>(FindItemByFullPath(L"ROI@3D View"));

			if(!pViewList)
				break;

			CGUIView3D* pView3D = pViewList->GetSelectedView3D();

			if(!pView3D)
			{
				SetStatusMessage(L"[Error] Select 3D view.");
				break;
			}

			int32_t i32ObjectCount = pView3D->GetObjectCount();

			if(!i32ObjectCount)
			{
				SetStatusMessage(L"[Error] " + pView3D->GetTitle() + L" doesn't have any 3D objects.");
				break;
			}

			std::vector<CFLFrustum<float>> vctFrustum;
			pView3D->GetAllROI(vctFrustum);

			if(vctFrustum.empty())
			{
				SetStatusMessage(L"[Error] There is no ROI in " + pView3D->GetTitle() + L". Press Ctrl+3 to teach ROI.");
				break;
			}

			for(int32_t i = 0; i < i32ObjectCount; ++i)
			{
				CGUIView3DObject* pObj = (CGUIView3DObject*)pView3D->GetView3DObject(i);

				if(!pObj)
					continue;

				CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

				if(!pObjData)
					continue;

				bool bUpdate = false;

				for(auto& frustumROI : vctFrustum)
				{
					CFLArray<int32_t> flaCollisionIndex;

					if(frustumROI.Contains(pObjData->GetVertices(), flaCollisionIndex).IsFail())
						continue;

					int32_t i32CollisionIndexCount = (int32_t)flaCollisionIndex.GetCount();
					bUpdate |= i32CollisionIndexCount > 0 ? true : false;

					CFLArray<Base::TPoint3<uint8_t>>* pFlaVertexColor = (CFLArray<Base::TPoint3<uint8_t>>*)pObjData->GetVertexColors();

					for(int32_t j = 0; j < i32CollisionIndexCount; ++j)
						pFlaVertexColor->SetAt(flaCollisionIndex.GetAt(j), TPoint3<uint8_t>(0, 255, 0)); // GREEN
				}

				if(bUpdate)
				{
					pObj->UpdateAll();
					pView3D->UpdateObject(i);
				}
			}

			pView3D->UpdateScreen();

			SetStatusMessage(GetResultString(EResult_OK));
		}
		while(false);
	};

	return pProcedure;
}

CPropertyItemButtonClickProcedure* FLImaging::GUI::CPropertyView3DExamples::SetCameraParameters()
{
	CPropertyItemButtonClickProcedure* pProcedure = new CPropertyItemButtonClickProcedure;

	*pProcedure = MakePropertyItemButtonClickProcedure
	{
		do
		{
			CGUIPropertyItemView3DList* pViewList = dynamic_cast<CGUIPropertyItemView3DList*>(FindItemByFullPath(L"Camera@3D View"));

			if(!pViewList)
				break;

			CGUIView3D* pView3D = pViewList->GetSelectedView3D();

			if(!pView3D)
			{
				SetStatusMessage(L"[Error] Select 3D view.");
				break;
			}

			CGUIPropertyItemText* pTextPX = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Position@x"));
			CGUIPropertyItemText* pTextPY = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Position@y"));
			CGUIPropertyItemText* pTextPZ = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Position@z"));

			CGUIPropertyItemText* pTextDX = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Direction@x"));
			CGUIPropertyItemText* pTextDY = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Direction@y"));
			CGUIPropertyItemText* pTextDZ = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Direction@z"));

			CGUIPropertyItemText* pTextUDX = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Up Direction@x"));
			CGUIPropertyItemText* pTextUDY = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Up Direction@y"));
			CGUIPropertyItemText* pTextUDZ = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Up Direction@z"));

			CGUIPropertyItemText* pTextTX = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Target@x"));
			CGUIPropertyItemText* pTextTY = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Target@y"));
			CGUIPropertyItemText* pTextTZ = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Target@z"));

			CGUIPropertyItemText* pTextAoV = dynamic_cast<CGUIPropertyItemText*>(FindItemByFullPath(L"Camera@Angle of View(Degree)"));

			// 3D View ���� ī�޶� ��ü ��� ����
			// Gets the camera object from the 3D View.
			CGUIView3DCamera* pCamera = (CGUIView3DCamera*)pView3D->GetCamera();

			if(pCamera)
			{
				{
					float f32DX = (float)_wtof(pTextDX->GetValue());
					float f32DY = (float)_wtof(pTextDY->GetValue());
					float f32DZ = (float)_wtof(pTextDZ->GetValue());

					// ī�޶� �ٶ󺸴� ������ ����
					// Sets the direction that the camera is looking.
					pCamera->SetDirection(CFLPoint3<float>(f32DX, f32DY, f32DZ));
				}

				{
					float f32UDX = (float)_wtof(pTextUDX->GetValue());
					float f32UDY = (float)_wtof(pTextUDY->GetValue());
					float f32UDZ = (float)_wtof(pTextUDZ->GetValue());

					// ī�޶��� ���� ���� ���͸� ����
					// Sets the up direction vector of the camera.
					pCamera->SetDirectionUp(CFLPoint3<float>(f32UDX, f32UDY, f32UDZ));
				}

				{
					float f32PX = (float)_wtof(pTextPX->GetValue());
					float f32PY = (float)_wtof(pTextPY->GetValue());
					float f32PZ = (float)_wtof(pTextPZ->GetValue());

					// ī�޶��� �������� ����
					// Sets the position of the camera.
					pCamera->SetPosition(CFLPoint3<float>(f32PX, f32PY, f32PZ));
				}

				{
					float f32TX = (float)_wtof(pTextTX->GetValue());
					float f32TY = (float)_wtof(pTextTY->GetValue());
					float f32TZ = (float)_wtof(pTextTZ->GetValue());

					// ī�޶� �ٶ󺸴� ��ü�� ��ǥ�� ����
					// Sets the coordinates of the object that the camera is looking at.
					pCamera->SetTarget(CFLPoint3<float>(f32TX, f32TY, f32TZ));
				}

				{
					float f32AoV = (float)_wtof(pTextAoV->GetValue());

					// ī�޶��� ȭ�� ����
					// Sets the angle of view
					pCamera->SetAngleOfViewY(f32AoV, false);
				}

				// �� ������Ʈ // Update the view	
				pView3D->UpdateScreen();

				CFLPoint3<float> flp3D = pCamera->GetDirection();
				CFLPoint3<float> flp3UD = pCamera->GetDirectionUp();
				CFLPoint3<float> flp3P = pCamera->GetPosition();
				CFLPoint3<float> flp3T = pCamera->GetTargetPosition();
				float f32Aov = pCamera->GetAngleOfViewY();

				pTextPX->SetValue(CFLString<wchar_t>().Format(L"%f", flp3P.x));
				pTextPY->SetValue(CFLString<wchar_t>().Format(L"%f", flp3P.y));
				pTextPZ->SetValue(CFLString<wchar_t>().Format(L"%f", flp3P.z));

				pTextDX->SetValue(CFLString<wchar_t>().Format(L"%f", flp3D.x));
				pTextDY->SetValue(CFLString<wchar_t>().Format(L"%f", flp3D.y));
				pTextDZ->SetValue(CFLString<wchar_t>().Format(L"%f", flp3D.z));

				pTextUDX->SetValue(CFLString<wchar_t>().Format(L"%f", flp3UD.x));
				pTextUDY->SetValue(CFLString<wchar_t>().Format(L"%f", flp3UD.y));
				pTextUDZ->SetValue(CFLString<wchar_t>().Format(L"%f", flp3UD.z));

				pTextTX->SetValue(CFLString<wchar_t>().Format(L"%f", flp3T.x));
				pTextTY->SetValue(CFLString<wchar_t>().Format(L"%f", flp3T.y));
				pTextTZ->SetValue(CFLString<wchar_t>().Format(L"%f", flp3T.z));

				pTextAoV->SetValue(CFLString<wchar_t>().Format(L"%f", f32Aov));
			}

			SetStatusMessage(GetResultString(EResult_OK));
		}
		while(false);
	};

	return pProcedure;
}
CPropertyItemButtonClickProcedure* FLImaging::GUI::CPropertyView3DExamples::FL3DObjectUtilitiesTest()
{
	CPropertyItemButtonClickProcedure* pProcedure = new CPropertyItemButtonClickProcedure;

	*pProcedure = MakePropertyItemButtonClickProcedure
	{
		do
		{
			CGUIPropertyItemView3DList* pViewListSource = dynamic_cast<CGUIPropertyItemView3DList*>(FindItemByFullPath(L"Transform@Source View"));
			CGUIPropertyItemView3DList* pViewListTarget = dynamic_cast<CGUIPropertyItemView3DList*>(FindItemByFullPath(L"Transform@Target View"));

			if(!pViewListSource || !pViewListTarget)
				break;

			CGUIView3D* pView3DSource = pViewListSource->GetSelectedView3D();
			CGUIView3D* pView3DTarget = pViewListTarget->GetSelectedView3D();

			if(!pView3DSource)
			{
				SetStatusMessage(L"[Error] Select source view.");
				break;
			}

			if(!pView3DTarget)
			{
				SetStatusMessage(L"[Error] Select target view.");
				break;
			}

			CGUIPropertyItemDropdownList* pPIDD = (CGUIPropertyItemDropdownList*)FindItemByFullPath(L"Transform@Transform Type");

			if(!pPIDD)
				break;

			// ȭ��ǥ�� ���� // Height of the arrow.
			float f32Height = 30.f;
			// ȭ��ǥ���� ������ �κ�(����)�� �����ϴ� ����
			// Ratio of the arrowhead (cone) to the total arrow length.
			float f32ArrowRatio = 0.15f;
			// ȭ��ǥ�� ������ ������ // Radius of the arrow's pipe.
			float f32PipeRadius = f32Height / 40.f;
			// ȭ��ǥ�� ������ �κ�(����)�� ������ // Radius of the arrowhead (cone).
			float f32ArrowBottomRadius = f32Height / 20.f;

			// CGUIView3DObject ��ü ����(�ӽ� ����)
			// Declare CGUIView3DObject (temporary variable)
			CGUIView3DObject mesh1, mesh2;

			// CGUIView3DObject ��ü�� �������� Ÿ���� �ָ���� ����
			// Set the topology type of the CGUIView3DObject to solid
			mesh1.SetTopologyType(ETopologyType3D_Solid);
			mesh2.SetTopologyType(ETopologyType3D_Solid);

			// ȭ��ǥ ��ü�� ����(�Ķ���)
			// Create the arrow object(Blue color).
			GetArrowObject(mesh1, mesh2, TPoint3<float>(0, 0, 0), f32Height, f32ArrowRatio, f32PipeRadius, f32ArrowBottomRadius, BLUE);

			// CGUIView3DObject ��ü�� �信 ���÷���
			// Display the CGUIView3DObject in the view
			pView3DSource->Clear();
			pView3DTarget->Clear();
			pView3DSource->PushObject(mesh1);
			pView3DSource->PushObject(mesh2);
			pView3DTarget->PushObject(mesh1);
			pView3DTarget->PushObject(mesh2);
			pView3DSource->ZoomFit();
			pView3DTarget->ZoomFit();

			pView3DSource->GetLayer(0)->DrawTextCanvas(&CFLPoint<double>(0, 0), L"Original Object", YELLOW, BLACK, 30);

			CFLString<wchar_t> str = pPIDD->GetValue();
			CResult res;

			if(str == L"Rotate")
			{
				res = RotateObject(pView3DTarget);
				pView3DTarget->GetLayer(0)->DrawTextCanvas(&CFLPoint<double>(0, 0), L"Rotated Object", YELLOW, BLACK, 30);
			}
			else if(str == L"Rotate with Pivot")
			{
				res = RotateObjectWithPivot(pView3DTarget);
				pView3DTarget->GetLayer(0)->DrawTextCanvas(&CFLPoint<double>(0, 0), L"Rotated Object", YELLOW, BLACK, 30);
			}
			else if(str == L"Translate")
			{
				res = TranslateObject(pView3DTarget);
				pView3DTarget->GetLayer(0)->DrawTextCanvas(&CFLPoint<double>(0, 0), L"Translated Object", YELLOW, BLACK, 30);
			}
			else if(str == L"Scale")
			{
				res = ScaleObject(pView3DTarget);
				pView3DTarget->GetLayer(0)->DrawTextCanvas(&CFLPoint<double>(0, 0), L"Scaled Object", YELLOW, BLACK, 30);
			}
			}
		while(false);
	};

	return pProcedure;

	return nullptr;
}

const CResult FLImaging::GUI::CPropertyView3DExamples::PushObjectPLY(CGUIView3D* pView3D)
{
	CResult res;

	do
	{
		if(!pView3D)
		{
			res = EResult_NullPointer;
			break;
		}

		// CGUIView3DObject ��ü ����(�ӽ� ����)
		// Declare CGUIView3DObject (temporary variable)
		CGUIView3DObject objTemp;

		// CGUIView3DObject ��ü�� �������� Ÿ���� �ָ���� ����
		// Set the topology type of the CGUIView3DObject to solid
		objTemp.SetTopologyType(ETopologyType3D_Solid);

		// CGUIView3DObject ��ü�� �信 ���÷��� ���� ��, ��ü�� �ε����� ��� ���� ���� ���� ����
		// Declare a variable to obtain the index of the object when the CGUIView3DObject is displayed in the view
		int32_t i32Index = -1;

		// CGUIView3DObject ��ü�� �信 ���÷���
		// Display the CGUIView3DObject in the view
		if((res = pView3D->PushObject(objTemp, &i32Index)).IsFail())
			break;

		// �信 ���÷��̵� CGUIView3DObject ��ü�� �ε����� ���� ��� ����
		// Retrieve the displayed CGUIView3DObject using the index
		CGUIView3DObject* pView3DObject = (CGUIView3DObject*)pView3D->GetView3DObject(i32Index);

		if(!pView3DObject)
		{
			res = EResult_NullPointer;
			break;
		}

		// �信 ���÷��̵� CGUIView3DObject ��ü�� ���� ������ ������ ��� ����
		// ���ؽ�, �÷� ���� ������ ���� ������ ��ü�� CFL3DObject �ȿ� ����
		// Retrieve the internal data pointer of the displayed CGUIView3DObject
		// Information such as vertices, colors, etc., are inside the internal data object CFL3DObject
		CFL3DObject* pObjData = (CFL3DObject*)pView3DObject->GetData();

		// CFL3DObject ��ü�� Load() �Լ� ȣ���� ���� PLY ������ �ε��Ͽ� 
		// ��ü�� ���ؽ�, �÷�, ���, ���̽� ���� ������ ����
		// Load the PLY file through the Load() function of the CFL3DObject object
		// and store information such as vertices, colors, normals, faces, etc., in the object
		if((res = pObjData->Load(L"../../ExampleImages/View3DExamples/Car wheel example.ply")).IsFail())
			break;

		// CGUIView3DObject ��ü�� ���� ������ ��ü�� CFL3DObject �� ����Ǿ����Ƿ� 
		// �信���� ���� ������ �ݿ��ؼ� ���÷��� �ϱ� ���� ������Ʈ �÷��� ����
		// Since the internal data object of CGUIView3DObject, CFL3DObject, has changed
		// set the update flag to reflect the changes in the view and display
		pView3DObject->UpdateAll();

		// �信�� �ش� ��ü�� ������Ʈ
		// Update the object in the view
		pView3D->UpdateObject(i32Index);

		// ������Ʈ�� ��ü�� ũ�⸦ �ݿ��Ͽ� �並 ZoomFit
		// ZoomFit the view to reflect the size of the updated object
		pView3D->ZoomFit();

		res = EResult_OK;
		SetStatusMessage(GetResultString(res));
	}
	while(false);

	return res;
}

const CResult FLImaging::GUI::CPropertyView3DExamples::PushObjectSphere(CGUIView3D* pView3D)
{
	CResult res;

	do
	{
		if(!pView3D)
		{
			res = EResult_NullPointer;
			break;
		}

		// CGUIView3DObject ��ü ����(�ӽ� ����)
		// Declare CGUIView3DObject (temporary variable)
		CGUIView3DObject objTemp;

		// CGUIView3DObject ��ü�� �������� Ÿ���� �ָ���� ����
		// Set the topology type of the CGUIView3DObject to solid
		objTemp.SetTopologyType(ETopologyType3D_Solid);

		// CGUIView3DObject ��ü�� �信 ���÷��� ���� ��, ��ü�� �ε����� ��� ���� ���� ���� ����
		// Declare a variable to obtain the index of the object when the CGUIView3DObject is displayed in the view
		int32_t i32Index = -1;

		// CGUIView3DObject ��ü�� �信 ���÷���
		// Display the CGUIView3DObject in the view
		if((res = pView3D->PushObject(objTemp, &i32Index)).IsFail())
			break;

		// �信 ���÷��̵� CGUIView3DObject ��ü�� �ε����� ���� ��� ����
		// Retrieve the displayed CGUIView3DObject using the index
		CGUIView3DObject* pView3DObject = (CGUIView3DObject*)pView3D->GetView3DObject(i32Index);

		if(!pView3DObject)
		{
			res = EResult_NullPointer;
			break;
		}

		// ���� ������ 
		// Sets the radius of the sphere.
		float f32Radius = 30.f;

		// ���� �����̽� ����. ���� Ŭ���� �ε巯�� ���� ��������ϴ�.
		// Sets the number of slices for the sphere. Higher values create a smoother sphere.
		uint32_t u32SliceCount = 30;

		// ���� ���� ����. ���� Ŭ���� �ε巯�� ���� ��������ϴ�.
		// Sets the number of stacks for the sphere. Higher values create a smoother sphere.
		uint32_t u32StackCount = 30;

		// CGUIView3DObject ��ü�� ���ؽ�, �÷�, �븻, ���̽� ������ ����
		// Sets the vertex, color, normal, and face information to the CGUIView3DObject object.
		if((res = GetSphereObject(*pView3DObject, f32Radius, u32SliceCount, u32StackCount)).IsFail())
			break;

		// CGUIView3DObject ��ü�� ���̵� Ÿ���� Phong shading ���� ����
		// Sets the shading type of the CGUIView3DObject object to Phong shading.
		pView3DObject->SetShadingType(EView3DObjectShadingType_Phong);

		// CGUIView3DObject ��ü�� ���� ������ ��ü�� CFL3DObject �� ����Ǿ����Ƿ� 
		// �信���� ���� ������ �ݿ��ؼ� ���÷��� �ϱ� ���� ������Ʈ �÷��� ����
		// Since the internal data object of CGUIView3DObject, CFL3DObject, has changed
		// set the update flag to reflect the changes in the view and display
		pView3DObject->UpdateVertex();
		pView3DObject->UpdateNormal();
		pView3DObject->UpdateColor();

		// �信�� �ش� ��ü�� ������Ʈ
		// Update the object in the view
		pView3D->UpdateObject(i32Index);
		pView3D->UpdateScreen();

		res = EResult_OK;
		SetStatusMessage(GetResultString(res));
	}
	while(false);

	return res;
}

const CResult FLImaging::GUI::CPropertyView3DExamples::PushObjectHeightMap(CGUIView3D* pView3D)
{
	CResult res;

	do
	{
		if(!pView3D)
		{
			res = EResult_NullPointer;
			break;
		}

		// CGUIView3DObjectHeightMap ��ü ����(�ӽ� ����)
		// Declare CGUIView3DObjectHeightMap (temporary variable)
		CGUIView3DObjectHeightMap objTemp;

		// CGUIView3DObjectHeightMap ��ü�� �������� Ÿ���� �ָ���� ����
		// Set the topology type of the CGUIView3DObjectHeightMap to solid
		objTemp.SetTopologyType(ETopologyType3D_Solid);

		// CGUIView3DObjectHeightMap ��ü�� �信 ���÷��� ���� ��, ��ü�� �ε����� ��� ���� ���� ���� ����
		// Declare a variable to obtain the index of the object when the CGUIView3DObjectHeightMap is displayed in the view
		int32_t i32Index = -1;

		// CGUIView3DObjectHeightMap ��ü�� �信 ���÷���
		// Display the CGUIView3DObjectHeightMap in the view
		if((res = pView3D->PushObject(objTemp, &i32Index)).IsFail())
			break;

		// �信 ���÷��̵� CGUIView3DObjectHeightMap ��ü�� �ε����� ���� ��� ����
		// Retrieve the displayed CGUIView3DObjectHeightMap using the index
		CGUIView3DObjectHeightMap* pObjHeightMap = (CGUIView3DObjectHeightMap*)pView3D->GetView3DObject(i32Index);

		if(!pObjHeightMap)
			break;

		// �信 ���÷��̵� CGUIView3DObjectHeightMap ��ü�� ���� ������ ������ ��� ����
		// ���ؽ�, �÷� ���� ������ ���� ������ ��ü�� CFL3DObjectHeightMap �ȿ� ����
		// Retrieve the internal data pointer of the displayed CGUIView3DObjectHeightMap
		// Information such as vertices, colors, etc., are inside the internal data object CFL3DObjectHeightMap
		CFL3DObjectHeightMap* pObjData = (CFL3DObjectHeightMap*)pObjHeightMap->GetData();

		// ���� ������ ����� �̹��� ��ü ����
		// Declares an image object to be used as a height map.
		CFLImage fliHeightMap;

		// ���� �� ���� �ε�
		// Loads the height map file.
		fliHeightMap.Load(L"../../ExampleImages/View3DExamples/mountain.flif");

		// �̹��� ��ü�� CFL3DObjectHeightMap ��ü�� Assign
		// Assigns the image object to a CFL3DObjectHeightMap object.
		pObjData->Assign(fliHeightMap);

		// CGUIView3DObjectHeightMap ��ü�� ���� ������ ��ü�� CFL3DObjectHeightMap �� ����Ǿ����Ƿ� 
		// �信���� ���� ������ �ݿ��ؼ� ���÷��� �ϱ� ���� ������Ʈ �÷��� ����
		// Since the internal data object of CGUIView3DObjectHeightMap, CFL3DObjectHeightMap, has changed
		// set the update flag to reflect the changes in the view and display
		pObjHeightMap->UpdateVertex();
		pObjHeightMap->UpdateColor();

		// �信�� �ش� ��ü�� ������Ʈ
		// Update the object in the view
		pView3D->UpdateObject(i32Index);

		// ������Ʈ�� ��ü�� ũ�⸦ �ݿ��Ͽ� �並 ZoomFit
		// ZoomFit the view to reflect the size of the updated object
		pView3D->ZoomFit();

		res = EResult_OK;
		SetStatusMessage(GetResultString(res));
	}
	while(false);

	return res;
}

const CResult FLImaging::GUI::CPropertyView3DExamples::PushObjectHeightMapAndTexture(CGUIView3D* pView3D)
{
	CResult res;

	do
	{
		if(!pView3D)
		{
			res = EResult_NullPointer;
			break;
		}

		// CGUIView3DObjectHeightMap ��ü ����(�ӽ� ����)
		// Declare CGUIView3DObjectHeightMap (temporary variable)
		CGUIView3DObjectHeightMap objTemp;

		// CGUIView3DObjectHeightMap ��ü�� �������� Ÿ���� �ָ���� ����
		// Set the topology type of the CGUIView3DObjectHeightMap to solid
		objTemp.SetTopologyType(ETopologyType3D_Solid);

		// CGUIView3DObjectHeightMap ��ü�� �信 ���÷��� ���� ��, ��ü�� �ε����� ��� ���� ���� ���� ����
		// Declare a variable to obtain the index of the object when the CGUIView3DObjectHeightMap is displayed in the view
		int32_t i32Index = -1;

		// CGUIView3DObjectHeightMap ��ü�� �信 ���÷���
		// Display the CGUIView3DObjectHeightMap in the view
		if((res = pView3D->PushObject(objTemp, &i32Index)).IsFail())
			break;

		// �信 ���÷��̵� CGUIView3DObjectHeightMap ��ü�� �ε����� ���� ��� ����
		// Retrieve the displayed CGUIView3DObjectHeightMap using the index
		CGUIView3DObjectHeightMap* pObjHeightMap = (CGUIView3DObjectHeightMap*)pView3D->GetView3DObject(i32Index);

		if(!pObjHeightMap)
		{
			res = EResult_NullPointer;
			break;
		}

		// �信 ���÷��̵� CGUIView3DObjectHeightMap ��ü�� ���� ������ ������ ��� ����
		// ���ؽ�, �÷� ���� ������ ���� ������ ��ü�� CFL3DObjectHeightMap �ȿ� ����
		// Retrieve the internal data pointer of the displayed CGUIView3DObjectHeightMap
		// Information such as vertices, colors, etc., are inside the internal data object CFL3DObjectHeightMap
		CFL3DObjectHeightMap* pObjData = (CFL3DObjectHeightMap*)pObjHeightMap->GetData();

		// ���� ������ ����� �̹��� ��ü ����
		// Declares an image object to be used as a height map.
		CFLImage fliHeightMap;

		// ���� �� ���� �ε�
		// Loads the height map file.
		fliHeightMap.Load(L"../../ExampleImages/View3DExamples/mountain.flif");

		// �ؽ��ķ� ����� �̹��� ��ü ����
		// Declares an image object to be used as a texture
		CFLImage fliTexture;

		// �ؽ��� ���� �ε�
		// Loads the texture file.
		fliTexture.Load(L"../../ExampleImages/View3DExamples/mountain_texture.flif");

		// ���� �� �̹��� ��ü�� CFL3DObjectHeightMap ��ü�� Assign
		// Assigns the height map image object to a CFL3DObjectHeightMap object.
		pObjData->Assign(fliHeightMap);

		// �ؽ��� �̹��� ��ü�� CFL3DObjectHeightMap ��ü�� Assign
		// Assigns the texture image object to a CFL3DObjectHeightMap object.
		pObjData->SetTextureImage(fliTexture);
		pObjData->ActivateVertexColorTexture(true);

		// CGUIView3DObjectHeightMap ��ü�� ���� ������ ��ü�� CFL3DObjectHeightMap �� ����Ǿ����Ƿ� 
		// �信���� ���� ������ �ݿ��ؼ� ���÷��� �ϱ� ���� ������Ʈ �÷��� ����
		// Since the internal data object of CGUIView3DObjectHeightMap, CFL3DObjectHeightMap, has changed
		// set the update flag to reflect the changes in the view and display
		pObjHeightMap->UpdateVertex();
		pObjHeightMap->UpdateColor();

		// �信�� �ش� ��ü�� ������Ʈ
		// Update the object in the view
		pView3D->UpdateObject(i32Index);

		// ������Ʈ�� ��ü�� ũ�⸦ �ݿ��Ͽ� �並 ZoomFit
		// ZoomFit the view to reflect the size of the updated object
		pView3D->ZoomFit();

		res = EResult_OK;
		SetStatusMessage(GetResultString(res));
	}
	while(false);

	return res;
}

const CResult FLImaging::GUI::CPropertyView3DExamples::PushObjectLine(CGUIView3D* pView3D)
{
	CResult res;

	do
	{
		if(!pView3D)
		{
			res = EResult_NullPointer;
			break;
		}

		// �Ķ���� ���� : ������ ������ -> ������ ���� -> ���� ���� -> ���� �β� -> �� ��Ÿ��
		// Parameter Order: Start Point of Line -> End Point of Line -> Line Color -> Line Thickness -> Pen Style
		pView3D->PushObject(CGUIView3DObjectLine(TPoint3<float>(0, 10, 0), TPoint3<float>(40, 10, 0), WHITE, 6.f, EGUIViewImagePenStyle_Solid));
		pView3D->PushObject(CGUIView3DObjectLine(TPoint3<float>(0, 20, 0), TPoint3<float>(40, 20, 0), YELLOW, 2.f, EGUIViewImagePenStyle_Dash));
		pView3D->PushObject(CGUIView3DObjectLine(TPoint3<float>(0, 30, 0), TPoint3<float>(40, 30, 0), VIOLET, 3.f, EGUIViewImagePenStyle_Dot));

		// ������Ʈ�� ��ü�� ũ�⸦ �ݿ��Ͽ� �並 ZoomFit
		// ZoomFit the view to reflect the size of the updated object
		pView3D->ZoomFit();

		// 3D View ���� ī�޶� ��ü ��� ����
		// Gets the camera object from the 3D View.
		CGUIView3DCamera* pCamera = (CGUIView3DCamera*)pView3D->GetCamera();

		if(pCamera)
		{
			// �߰��� ������ �� �� �� �ִ� �������� �̵�
			// Moves the viewpoint to a good position to see the added line.

			// ī�޶� �ٶ󺸴� ������ ����
			// Sets the direction that the camera is looking.
			pCamera->SetDirection(CFLPoint3<float>(0, 0, -1));

			// ī�޶��� ���� ���� ���͸� ����
			// Sets the up direction vector of the camera.
			pCamera->SetDirectionUp(CFLPoint3<float>(0, 1, 0));

			// ī�޶��� �������� ����
			// Sets the position of the camera.
			pCamera->SetPosition(CFLPoint3<float>(20, 20, 100));

			// ī�޶� �ٶ󺸴� ��ü�� ��ǥ�� ����
			// Sets the coordinates of the object that the camera is looking at.
			pCamera->SetTarget(CFLPoint3<float>(20, 20, 0));
		}

		res = EResult_OK;
		SetStatusMessage(GetResultString(res));
	}
	while(false);

	return res;
}

const CResult FLImaging::GUI::CPropertyView3DExamples::PushObjectUnselectableAxis(CGUIView3D* pView3D)
{
	CResult res;

	do
	{
		if(!pView3D)
		{
			res = EResult_NullPointer;
			break;
		}

		// ���� �߰��� 3D Axis �� ���� ���� ������ ����Ʈ Axis �� ����� ó��
		// Hides the default axis to display the custom 3D axis.
		pView3D->ShowAxis(false);

		// ȭ��ǥ�� ���� // Height of the arrow.
		float f32Height = 30.f;
		// ȭ��ǥ���� ������ �κ�(����)�� �����ϴ� ����
		// Ratio of the arrowhead (cone) to the total arrow length.
		float f32ArrowRatio = 0.15f;
		// ȭ��ǥ�� ������ ������ // Radius of the arrow's pipe.
		float f32PipeRadius = f32Height / 40.f;
		// ȭ��ǥ�� ������ �κ�(����)�� ������ // Radius of the arrowhead (cone).
		float f32ArrowBottomRadius = f32Height / 20.f;

		// CGUIView3DObject ��ü ����(�ӽ� ����)
		// Declare CGUIView3DObject (temporary variable)
		CGUIView3DObject mesh1, mesh2;

		// CGUIView3DObject ��ü�� �������� Ÿ���� �ָ���� ����
		// Set the topology type of the CGUIView3DObject to solid
		mesh1.SetTopologyType(ETopologyType3D_Solid);
		mesh2.SetTopologyType(ETopologyType3D_Solid);

		// �� CGUIView3DObject ��ü�� ROI �Ǵ� Crop Object ���� �� ���õǵ��� ������ �Ұ��ϰ� ó��
		// Make these CGUIView3DObjects unselectable for ROI or Crop Object operations.
		mesh1.EnableSelection(false);
		mesh2.EnableSelection(false);

		// X �� // Axis x
		{
			// ȭ��ǥ ��ü�� ����(������)
			// Create the arrow object(Red color).
			GetArrowObject(mesh1, mesh2, TPoint3<float>(0, 0, 0), f32Height, f32ArrowRatio, f32PipeRadius, f32ArrowBottomRadius, RED);

			// CFL3DObjectUtilities �� ����Ͽ� �����ϰ� ȸ��
			// �Ķ���� ���� : ���� ���� ���� -> ���� ���� ���� -> ȸ���� ��ü
			// ���� ���Ϳ��� ���� ���ͻ����� ������ŭ ȸ���˴ϴ�.
			// Rotate the arrow using CFL3DObjectUtilities.
			// Parameter order: start point vector -> end point vector -> object to rotate
			// Rotates by the angle between the start and end vectors.
			CFL3DObjectUtilities::Rotate(CFLGeometry3DVector<float>(0, 0, 1), CFLGeometry3DVector<float>(1, 0, 0), *(Foundation::CFL3DObject*)mesh1.GetData());
			CFL3DObjectUtilities::Rotate(CFLGeometry3DVector<float>(0, 0, 1), CFLGeometry3DVector<float>(1, 0, 0), *(Foundation::CFL3DObject*)mesh2.GetData());

			// CGUIView3DObject ��ü�� �信 ���÷���
			// Display the CGUIView3DObject in the view
			pView3D->PushObject(mesh1);
			pView3D->PushObject(mesh2);

			// ���� ��(Y��)�� �׸��� ���� Ŭ����
			// Clear the objects to prepare for drawing the next axis (Y axis).
			((Foundation::CFL3DObject*)mesh1.GetData())->Clear();
			((Foundation::CFL3DObject*)mesh2.GetData())->Clear();
		}

		// Y �� // Axis y
		{
			// ȭ��ǥ ��ü�� ����(�ʷϻ�)
			// Create the arrow object(Green color).
			GetArrowObject(mesh1, mesh2, TPoint3<float>(0, 0, 0), f32Height, f32ArrowRatio, f32PipeRadius, f32ArrowBottomRadius, GREEN);

			// CFL3DObjectUtilities �� ����Ͽ� �����ϰ� ȸ��
			// �Ķ���� ���� : ���� ���� ���� -> ���� ���� ���� -> ȸ���� ��ü
			// ���� ���Ϳ��� ���� ���ͻ����� ������ŭ ȸ���˴ϴ�.
			// Rotate the arrow using CFL3DObjectUtilities.
			// Parameter order: start point vector -> end point vector -> object to rotate
			// Rotates by the angle between the start and end vectors.
			CFL3DObjectUtilities::Rotate(CFLGeometry3DVector<float>(0, 0, 1), CFLGeometry3DVector<float>(0, 1, 0), *(Foundation::CFL3DObject*)mesh1.GetData());
			CFL3DObjectUtilities::Rotate(CFLGeometry3DVector<float>(0, 0, 1), CFLGeometry3DVector<float>(0, 1, 0), *(Foundation::CFL3DObject*)mesh2.GetData());

			// CGUIView3DObject ��ü�� �信 ���÷���
			// Display the CGUIView3DObject in the view
			pView3D->PushObject(mesh1);
			pView3D->PushObject(mesh2);

			// ���� ��(Z��)�� �׸��� ���� Ŭ����
			// Clear the objects to prepare for drawing the next axis (Z axis).
			((Foundation::CFL3DObject*)mesh1.GetData())->Clear();
			((Foundation::CFL3DObject*)mesh2.GetData())->Clear();
		}

		// Z �� // Axis z
		{
			// ȭ��ǥ ��ü�� ����(�Ķ���)
			// Create the arrow object(Blue color).
			GetArrowObject(mesh1, mesh2, TPoint3<float>(0, 0, 0), f32Height, f32ArrowRatio, f32PipeRadius, f32ArrowBottomRadius, BLUE);

			// CGUIView3DObject ��ü�� �信 ���÷���
			// Display the CGUIView3DObject in the view
			pView3D->PushObject(mesh1);
			pView3D->PushObject(mesh2);
		}

		pView3D->ZoomFit();

		res = EResult_OK;
		SetStatusMessage(GetResultString(res));
	}
	while(false);

	return res;
}

const CResult FLImaging::GUI::CPropertyView3DExamples::RotateObject(CGUIView3D* pView3D)
{
	CResult res;

	do
	{
		if(!pView3D)
		{
			res = EResult_NullPointer;
			break;
		}

		int32_t i32ObjectCount = pView3D->GetObjectCount();

		if(!i32ObjectCount)
		{
			SetStatusMessage(L"[Error] " + pView3D->GetTitle() + L" doesn't have any 3D objects.");
			break;
		}

		for(int32_t i = 0; i < i32ObjectCount; ++i)
		{
			CGUIView3DObject* pObj = (CGUIView3DObject*)pView3D->GetView3DObject(i);

			if(!pObj || !pObj->IsSelectionEnabled())
				continue;

			CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

			if(!pObjData)
				continue;

			// CFL3DObjectUtilities �� ����Ͽ� �����ϰ� ȸ��
			// �Ķ���� ���� : ���� ���� ���� -> ���� ���� ���� -> ȸ���� ��ü
			// ���� ���Ϳ��� ���� ���ͻ����� ������ŭ ȸ���˴ϴ�.
			// Rotate the object using CFL3DObjectUtilities.
			// Parameter order: start point vector -> end point vector -> object to rotate
			// Rotates by the angle between the start and end vectors.
			CFL3DObjectUtilities::Rotate(CFLGeometry3DVector<float>(0, 0, 1), CFLGeometry3DVector<float>(1, 0, 0), *pObjData);

			pObj->UpdateVertex();
			pView3D->UpdateObject(i);
		}

		res = EResult_OK;
		SetStatusMessage(GetResultString(res));
	}
	while(false);

	return res;
}

const CResult FLImaging::GUI::CPropertyView3DExamples::RotateObjectWithPivot(CGUIView3D* pView3D)
{
	CResult res;

	do
	{
		if(!pView3D)
		{
			res = EResult_NullPointer;
			break;
		}

		int32_t i32ObjectCount = pView3D->GetObjectCount();

		if(!i32ObjectCount)
		{
			SetStatusMessage(L"[Error] " + pView3D->GetTitle() + L" doesn't have any 3D objects.");
			break;
		}

		for(int32_t i = 0; i < i32ObjectCount; ++i)
		{
			CGUIView3DObject* pObj = (CGUIView3DObject*)pView3D->GetView3DObject(i);

			if(!pObj || !pObj->IsSelectionEnabled())
				continue;

			CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

			if(!pObjData)
				continue;

			// �Ǻ��� �߽����� ȸ��
			// An example of rotating around a pivot 
			CFL3DObjectUtilities::Rotate(CFLPoint3<float>(0, 0, 30), // Pivot
										 CFLGeometry3DQuaternion<float>(CFLGeometry3DVector<float>(0.f, 1.f, 0.f), 
																		CFLGeometry3DVector<float>(0.f, 0.5f, 0.5f)), 
										 *pObjData);

			pObj->UpdateVertex();
			pView3D->UpdateObject(i);
		}

		res = EResult_OK;
		SetStatusMessage(GetResultString(res));
	}
	while(false);

	return res;
}

const CResult FLImaging::GUI::CPropertyView3DExamples::TranslateObject(CGUIView3D* pView3D)
{
	CResult res;

	do
	{
		if(!pView3D)
		{
			res = EResult_NullPointer;
			break;
		}

		int32_t i32ObjectCount = pView3D->GetObjectCount();

		if(!i32ObjectCount)
		{
			SetStatusMessage(L"[Error] " + pView3D->GetTitle() + L" doesn't have any 3D objects.");
			break;
		}

		for(int32_t i = 0; i < i32ObjectCount; ++i)
		{
			CGUIView3DObject* pObj = (CGUIView3DObject*)pView3D->GetView3DObject(i);

			if(!pObj || !pObj->IsSelectionEnabled())
				continue;

			CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

			if(!pObjData)
				continue;

			// CFL3DObjectUtilities �� ����Ͽ� �����ϰ� �̵�
			// Translate the object using CFL3DObjectUtilities.
			CFL3DObjectUtilities::Translate(CFLGeometry3DVector<float>(4, 0, 0), *pObjData);

			pObj->UpdateVertex();
			pView3D->UpdateObject(i);
		}

		res = EResult_OK;
		SetStatusMessage(GetResultString(res));
	}
	while(false);

	return res;
}

const CResult FLImaging::GUI::CPropertyView3DExamples::ScaleObject(CGUIView3D* pView3D)
{
	CResult res;

	do
	{
		if(!pView3D)
		{
			res = EResult_NullPointer;
			break;
		}

		int32_t i32ObjectCount = pView3D->GetObjectCount();

		if(!i32ObjectCount)
		{
			SetStatusMessage(L"[Error] " + pView3D->GetTitle() + L" doesn't have any 3D objects.");
			break;
		}

		for(int32_t i = 0; i < i32ObjectCount; ++i)
		{
			CGUIView3DObject* pObj = (CGUIView3DObject*)pView3D->GetView3DObject(i);

			if(!pObj || !pObj->IsSelectionEnabled())
				continue;

			CFL3DObject* pObjData = (CFL3DObject*)pObj->GetData();

			if(!pObjData)
				continue;

			// CFL3DObjectUtilities �� ����Ͽ� �����ϰ� ������
			// Scales the object using CFL3DObjectUtilities.
			CFL3DObjectUtilities::Scale(CFLGeometry3DVector<float>(0, 0, 0), 2.f, *pObjData);

			pObj->UpdateVertex();
			pView3D->UpdateObject(i);
		}

		res = EResult_OK;
		SetStatusMessage(GetResultString(res));
	}
	while(false);

	return res;
}

const CResult FLImaging::GUI::CPropertyView3DExamples::GetSphereObject(CGUIView3DObject& meshSphere, float f32Radius, uint32_t u32SliceCount, uint32_t u32StackCount)
{
	CResult cReturn;

	do
	{
		SFaceDataTangent sFaceData;

		if((cReturn = CGUIView3DUtils::CreateSphere(f32Radius, u32SliceCount, u32StackCount, sFaceData)).IsFail())
			break;

		CFL3DObject* pObjectData = (CFL3DObject*)meshSphere.GetData();
		int32_t i32VertexCount = (int32_t)sFaceData.m_vctVertex.size();
		int32_t i32IndexCount = (int32_t)sFaceData.m_vctIndices32.size();

		CFLArray<TPoint3<float>>* pFlaVertices = (CFLArray<TPoint3<float>>*)pObjectData->GetVertices();

		if(!pFlaVertices->Resize(i32VertexCount))
		{
			cReturn = EResult_NotEnoughMemory;
			break;
		}

		CFLArray<TPoint3<uint8_t>>* pFlaVertexColors = (CFLArray<TPoint3<uint8_t>>*)pObjectData->GetVertexColors();

		if(!pFlaVertexColors->Resize(i32VertexCount))
		{
			cReturn = EResult_NotEnoughMemory;
			break;
		}

		CFLArray<TPoint3<float>>* pFlaVertexNormal = (CFLArray<TPoint3<float>>*)pObjectData->GetNormalVectors();

		if(!pFlaVertexNormal->Resize(i32VertexCount))
		{
			cReturn = EResult_NotEnoughMemory;
			break;
		}

		CFLArray<int32_t>* pFlaFaces = (CFLArray<int32_t>*)pObjectData->GetFaces();
		pFlaFaces->Reserve(i32IndexCount);

		for(int32_t i = 0; i < (int32_t)sFaceData.m_vctVertex.size(); ++i)
		{
			auto& pt3 = sFaceData.m_vctVertex[i];
			TPoint3<float>& tpVertex = pFlaVertices->GetAt(i);
			TPoint3<uint8_t>& tpColor = pFlaVertexColors->GetAt(i);
			TPoint3<float>& tpNormal = pFlaVertexNormal->GetAt(i);
			tpVertex = pt3.position;
			tpNormal = pt3.normal;
			tpColor.x = 155;
			tpColor.y = 230;
			tpColor.z = 255;
		}

		for(int32_t i = 0; i < (int32_t)sFaceData.m_vctIndices32.size(); ++i)
			pFlaFaces->PushBack((int32_t)sFaceData.m_vctIndices32[i]);

		cReturn = EResult_OK;
	}
	while(false);

	return cReturn;
}

const CResult FLImaging::GUI::CPropertyView3DExamples::GetArrowObject(CGUIView3DObject& mesh1, CGUIView3DObject& mesh2, const TPoint3<float>& tp3Origin, float f32Length, float f32ArrowRatio, float f32PipeRadius, float f32ArrowBottomRadius, COLORREF clr)
{
	CResult cReturn;

	do
	{
		// color
		uint8_t r = GetRValue(clr);
		uint8_t g = GetGValue(clr);
		uint8_t b = GetBValue(clr);

		SFaceDataTangent sFaceData1;
		float ArrowHeight = f32Length * f32ArrowRatio;
		float PipeHeight = f32Length * (1.f - f32ArrowRatio);

		// origin
		float x = tp3Origin.x;
		float y = tp3Origin.y;
		float z = tp3Origin.z + (PipeHeight + ArrowHeight * 0.5f);

		uint32_t sliceCount = 20;
		uint32_t stackCount = 10;

		cReturn = CGUIView3DUtils::CreateCylinder(f32ArrowBottomRadius, 0.f, ArrowHeight, x, y, z, sliceCount, stackCount, sFaceData1);

		if(cReturn.IsFail())
			break;

		SFaceDataTangent sFaceData2;

		// origin
		x = tp3Origin.x;
		y = tp3Origin.y;
		z = tp3Origin.z + PipeHeight * 0.5f;

		cReturn = CGUIView3DUtils::CreateCylinder(f32PipeRadius, f32PipeRadius, PipeHeight, x, y, z, sliceCount, stackCount, sFaceData2);

		if(cReturn.IsFail())
			break;

		CFL3DObject* pObjectData1 = (CFL3DObject*)mesh1.GetData();
		CFL3DObject* pObjectData2 = (CFL3DObject*)mesh2.GetData();

		int32_t i32VertexCount1 = (int32_t)sFaceData1.m_vctVertex.size();
		int32_t i32VertexCount2 = (int32_t)sFaceData2.m_vctVertex.size();
		int32_t i32IndexCount1 = (int32_t)sFaceData1.m_vctIndices32.size() / 3;
		int32_t i32IndexCount2 = (int32_t)sFaceData2.m_vctIndices32.size() / 3;

		CFLArray<TPoint3<float>>* pFlaVertices1 = (CFLArray<TPoint3<float>>*)pObjectData1->GetVertices();

		if(!pFlaVertices1->Resize(i32VertexCount1))
		{
			cReturn = EResult_NotEnoughMemory;
			break;
		}

		CFLArray<TPoint3<uint8_t>>* pFlaVertexColors1 = (CFLArray<TPoint3<uint8_t>>*)pObjectData1->GetVertexColors();

		if(!pFlaVertexColors1->Resize(i32VertexCount1))
		{
			cReturn = EResult_NotEnoughMemory;
			break;
		}

		CFLArray<int32_t>* pFlaFaces1 = (CFLArray<int32_t>*)pObjectData1->GetFaces();

		pFlaFaces1->Reserve(i32IndexCount1);

		for(int32_t i = 0; i < (int32_t)sFaceData1.m_vctVertex.size(); ++i)
		{
			auto& pt3 = sFaceData1.m_vctVertex[i];
			TPoint3<float>& tpVertex = pFlaVertices1->GetAt(i);
			tpVertex = TPoint3<float>(pt3.position.x, pt3.position.y, pt3.position.z);

			TPoint3<uint8_t>& tpColor = pFlaVertexColors1->GetAt(i);
			tpColor = TPoint3<uint8_t>(r, g, b);
		}

		for(int32_t i = 0; i < (int32_t)sFaceData1.m_vctIndices32.size(); ++i)
			pFlaFaces1->PushBack((int32_t)sFaceData1.m_vctIndices32[i]);


		CFLArray<TPoint3<float>>* pFlaVertices2 = (CFLArray<TPoint3<float>>*)pObjectData2->GetVertices();

		if(!pFlaVertices2->Resize(i32VertexCount2))
		{
			cReturn = EResult_NotEnoughMemory;
			break;
		}

		CFLArray<TPoint3<uint8_t>>* pFlaVertexColors2 = (CFLArray<TPoint3<uint8_t>>*)pObjectData2->GetVertexColors();

		if(!pFlaVertexColors2->Resize(i32VertexCount2))
		{
			cReturn = EResult_NotEnoughMemory;
			break;
		}

		CFLArray<int32_t>* pFlaFaces2 = (CFLArray<int32_t>*)pObjectData2->GetFaces();

		pFlaFaces2->Reserve(i32IndexCount2);

		for(int32_t i = 0; i < (int32_t)sFaceData2.m_vctVertex.size(); ++i)
		{
			auto& pt3 = sFaceData2.m_vctVertex[i];
			TPoint3<float>& tpVertex = pFlaVertices2->GetAt(i);
			tpVertex = TPoint3<float>(pt3.position.x, pt3.position.y, pt3.position.z);

			TPoint3<uint8_t>& tpColor = pFlaVertexColors2->GetAt(i);
			tpColor = TPoint3<uint8_t>(r, g, b);
		}

		for(int32_t i = 0; i < (int32_t)sFaceData2.m_vctIndices32.size(); ++i)
			pFlaFaces2->PushBack((int32_t)sFaceData2.m_vctIndices32[i]);

		cReturn = EResult_OK;
	}
	while(false);

	return cReturn;
}
