﻿#include <cstdio>

#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h"


int main()
{
	// You must call the following function once
	// before using any features of the FLImaging(R) library
	CLibraryUtilities::Initialize();

	// 이미지 객체 선언 // Declare the image object
	CFLImage fliSrcImage;
	CFLImage fliDstImage;

	// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImageSrc;
	CGUIViewImageWrap viewImageDst;

	CResult res = EResult_UnknownError;

	do
	{
		// Source 이미지 로드 // Load the source image
		if(IsFail(res = fliSrcImage.Load(L"../../ExampleImages/HoleFilling/TodoList.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image views
		if((res = viewImageSrc.Create(100, 0, 600, 500)).IsFail() ||
		   (res = viewImageDst.Create(600, 0, 1100, 500)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view. \n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(res = viewImageSrc.SynchronizePointOfView(&viewImageDst)))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageSrc.SynchronizeWindow(&viewImageDst)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the image view
		if((res = viewImageSrc.SetImagePtr(&fliSrcImage)).IsFail() ||
		   (res = viewImageDst.SetImagePtr(&fliDstImage)).IsFail())
		{
			ErrorPrint(res, "Failed to set image object on the image view. \n");
			break;
		}


		// 알고리즘 객체 생성 // Create algorithm object
		CHoleFilling algObject;

		if((res = algObject.SetSourceImage(fliSrcImage)).IsFail())
			break;
		if((res = algObject.SetDestinationImage(fliDstImage)).IsFail())
			break;
		if((res = algObject.SetMinimumHoleArea(10)).IsFail())
			break;
		if((res = algObject.SetMaximumHoleArea(99999999999)).IsFail())
			break;
		if((res = algObject.EnableIgnoreBoundaryHole(true)).IsFail())
			break;
		if((res = algObject.SetThresholdPassTarget(CHoleFilling::EThresholdPassTarget_Object)).IsFail())
			break;
		if((res = algObject.SetThresholdMode(EThresholdMode_Dual_And)).IsFail())
			break;
		if((res = algObject.SetLogicalConditionOfChannels(ELogicalConditionOfChannels_And)).IsFail())
			break;
		if((res = algObject.SetFillingMethod((CHoleFilling::EFillingMethod)0)).IsFail())
			break;
		if((res = algObject.SetPrecision(0.1)).IsFail())
			break;
		if((res = algObject.SetMaxIteration(100)).IsFail())
			break;
		CMultiVar<uint64_t> mvThresholdCondition1 = CMultiVar<uint64_t>((uint64_t)ELogicalCondition_GreaterEqual, (uint64_t)ELogicalCondition_GreaterEqual, (uint64_t)ELogicalCondition_GreaterEqual);
		if((res = algObject.SetThresholdCondition(EThresholdIndex_First, mvThresholdCondition1)).IsFail())
			break;
		CMultiVar<uint64_t> mvThresholdValue1U64 = CMultiVar<uint64_t>(175, 230, 240);
		if((res = algObject.SetThresholdValue(EThresholdIndex_First, mvThresholdValue1U64)).IsFail())
			break;
		CMultiVar<uint64_t> mvThresholdCondition2 = CMultiVar<uint64_t>((uint64_t)ELogicalCondition_Less, (uint64_t)ELogicalCondition_Less, (uint64_t)ELogicalCondition_Less);
		if((res = algObject.SetThresholdCondition(EThresholdIndex_Second, mvThresholdCondition2)).IsFail())
			break;
		CMultiVar<uint64_t> mvThresholdValue2U64 = CMultiVar<uint64_t>(200, 240, 255);
		if((res = algObject.SetThresholdValue(EThresholdIndex_Second, mvThresholdValue2U64)).IsFail())
			break;

		// 알고리즘 수행 // Execute the algorithm
		if((res = algObject.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute the algorithm.");
			break;
		}


		// 출력을 위한 이미지 레이어를 얻어옵니다. //  Gets the image layer for output.
		// 따로 해제할 필요 없음 // No need to release separately
		CGUIViewImageLayerWrap layerSrc = viewImageSrc.GetLayer(0);
		CGUIViewImageLayerWrap layerDst = viewImageDst.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Delete the shapes drawn on the existing layer
		layerSrc.Clear();
		layerDst.Clear();

		// View 정보를 디스플레이 합니다. // Display View information.
		CFLPoint<double> flpPoint = new CFLPoint<double>(0, 0);
		if((res = (layerSrc.DrawTextCanvas(flpPoint, L"Source Image", YELLOW, BLACK, 30))).IsFail() ||
		   (res = layerDst.DrawTextCanvas(flpPoint, L"Destination Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text. \n");
			break;
		}

		const CFLFigure* pFlfHoleContours = algObject.GetSelectedPageFigureObject();
		if(IsFail(res = layerSrc.DrawFigureImage(pFlfHoleContours, CYAN)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		// Zoom Fit
		viewImageSrc.ZoomFit();
		viewImageDst.ZoomFit();

		// 이미지 뷰를 갱신 합니다. // Update the image view.
		viewImageSrc.Invalidate(true);
		viewImageDst.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImageSrc.IsAvailable() && viewImageDst.IsAvailable())
			CThreadUtilities::Sleep(1);

	}
	while(false);

	return 0;
}