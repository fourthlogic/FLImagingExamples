#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[4];

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	do
	{
		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage[0].Create(400, 0, 812, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[1].Create(812, 0, 1224, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[2].Create(400, 384, 812, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImage[3].Create(812, 384, 1224, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// SourceView, DstView 의 0번 레이어 가져오기 // Get Layer 0 of SourceView, DstView
		CGUIViewImageLayerWrap Src1Layer0 = viewImage[0].GetLayer(0);
		CGUIViewImageLayerWrap Dst1Layer0 = viewImage[1].GetLayer(0);
		CGUIViewImageLayerWrap Src2Layer0 = viewImage[2].GetLayer(0);
		CGUIViewImageLayerWrap Dst2Layer0 = viewImage[3].GetLayer(0);

		Src1Layer0.DrawTextCanvas(TPoint<double>(0, 0), L"Source Figure 1", YELLOW, BLACK, 15);
		Src2Layer0.DrawTextCanvas(TPoint<double>(0, 0), L"Source Figure 2", YELLOW, BLACK, 15);
		Dst1Layer0.DrawTextCanvas(TPoint<double>(0, 0), L"Result Figure 1", YELLOW, BLACK, 15);
		Dst2Layer0.DrawTextCanvas(TPoint<double>(0, 0), L"Result Figure 2", YELLOW, BLACK, 15);

		Dst1Layer0.DrawTextCanvas(TPoint<double>(0, 20), L"Index of Minimum Distance", CYAN, BLACK);
		Dst2Layer0.DrawTextCanvas(TPoint<double>(0, 20), L"Index of Minimum Distance", CYAN, BLACK);

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(res = viewImage[2].SynchronizePointOfView(&viewImage[3])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		for(int32_t i = 1; i < 4; ++i)
		{
			if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[i])))
			{
				ErrorPrint(res, "Failed to synchronize window.\n");
				break;
			}
		}

		// Figure 선언 // Figure declaration
		CFLPointArray flpaSource1;
		CFLCircle<double> flcDestination1;
		CFLFigureArray flfaSource2;
		CFLFigureArray flfaDestination2;

		// Source Figure 불러오기 // Load Source figure
		if((res = flpaSource1.Load(L"../../ExampleImages/Figure/PointArray1.fig")).IsFail())
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}

		if((res = flfaSource2.Load(L"../../ExampleImages/Figure/various_arrays.fig")).IsFail())
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}

		// Destination Figure 불러오기 // Load Destination Figure
		if((res = flcDestination1.Load(L"../../ExampleImages/Figure/Circle2.fig")).IsFail())
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}

		if((res = flfaDestination2.Load(L"../../ExampleImages/Figure/Circles2.fig")).IsFail())
		{
			ErrorPrint(res, "Failed to load the figure file.\n");
			break;
		}

		// Figure 사이의 최소 거리를 나타내는 인덱스를 추출 // Get the index of representing the minimum distance between figures
		CFLFigureArray flfaResultSrc1;

		if((res = flpaSource1.GetIndexOfMinimumDistance(flcDestination1, &flfaResultSrc1)).IsFail())
		{
			ErrorPrint(res, "Failed to process.\n");
			break;
		}

		CFLFigureArray flfaResultSrc2;
		CFLFigureArray flfaResultDst2;

		if((res = flfaSource2.GetIndexOfMinimumDistance(flfaDestination2, &flfaResultSrc2, true, true, &flfaResultDst2)).IsFail())
		{
			ErrorPrint(res, "Failed to process.\n");
			break;
		}

		// SourceView1의 0번 레이어에 Source, Destination Figure 그리기 // Draw Source and Destination Figure on Layer 0 of SourceView1
		Src1Layer0.DrawFigureImage(flpaSource1, BLACK, 3);
		Src1Layer0.DrawFigureImage(flpaSource1, LIME);
		Src1Layer0.DrawFigureImage(flcDestination1, BLACK, 3);
		Src1Layer0.DrawFigureImage(flcDestination1, KHAKI);

		for(int64_t i = 0; i < flpaSource1.GetCount(); ++i)
			Src1Layer0.DrawTextImage(flpaSource1.GetAt(i)->GetCenter(), CFLString<wchar_t>().Format(L"%lld", i), CYAN, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_BOTTOM);

		// DstView1의 0번 레이어에 결과 그리기 // Draw the result on layer 0 of DstView1
		CFLScalar<int64_t> flvSrc = (CFLScalar<int64_t>)flfaResultSrc1.Front();
		Dst1Layer0.DrawFigureImage(flpaSource1, BLACK, 3);
		Dst1Layer0.DrawFigureImage(flpaSource1, LIME);
		Dst1Layer0.DrawFigureImage(flcDestination1, BLACK, 3);
		Dst1Layer0.DrawFigureImage(flcDestination1, KHAKI);
		Dst1Layer0.DrawFigureImage(flpaSource1.GetAt(flvSrc.v), CYAN, 3);
		Dst1Layer0.DrawTextImage(flpaSource1.GetAt(flvSrc.v)->GetCenter(), CFLString<wchar_t>().Format(L"%lld", flvSrc.v), CYAN, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_BOTTOM);
		Dst1Layer0.DrawTextImage(flcDestination1.GetCenter(), CFLString<wchar_t>().Format(L"%lld", flvSrc.v), CYAN, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_CENTER);

		// SourceView2의 0번 레이어에 Source, Destination Figure 그리기 // Draw Source and Destination Figure on Layer 0 of SourceView2
		Src2Layer0.DrawFigureImage(flfaSource2, BLACK, 3);
		Src2Layer0.DrawFigureImage(flfaSource2, LIME);
		Src2Layer0.DrawFigureImage(flfaDestination2, BLACK, 3);
		Src2Layer0.DrawFigureImage(flfaDestination2, KHAKI);

		for(int64_t i = 0; i < flfaSource2.GetCount(); ++i)
		{
			CFLFigureArray flfaArrayDepth1 = (CFLFigureArray)flfaSource2.GetAt(i);
			CFLRect<double> flrBoundary;
			flfaArrayDepth1.GetBoundaryRect(&flrBoundary);

			Src2Layer0.DrawFigureImage(flrBoundary, LIGHTBLUE, 1);
			Src2Layer0.DrawTextImage(flfaArrayDepth1.GetCenter(), CFLString<wchar_t>().Format(L"%lld", i), LIGHTBLUE, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_CENTER);

			for(int64_t j = 0; j < flfaArrayDepth1.GetCount(); ++j)
				Src2Layer0.DrawTextImage(flfaArrayDepth1.GetAt(j)->GetCenter(), CFLString<wchar_t>().Format(L"%lld", j), CYAN, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_CENTER);
		}

		for(int64_t i = 0; i < flfaDestination2.GetCount(); ++i)
			Src2Layer0.DrawTextImage(flfaDestination2.GetAt(i)->GetCenter(), CFLString<wchar_t>().Format(L"%lld", i), CYAN, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_CENTER);

		// DstView2의 0번 레이어에 결과 그리기 // Draw the result on layer 0 of DstView2
		Dst2Layer0.DrawFigureImage(flfaSource2, BLACK, 3);
		Dst2Layer0.DrawFigureImage(flfaSource2, LIME);
		Dst2Layer0.DrawFigureImage(flfaDestination2, BLACK, 3);
		Dst2Layer0.DrawFigureImage(flfaDestination2, KHAKI);


		CFLScalar<int64_t>* pFlvSrcDepth1 = (CFLScalar<int64_t>*)flfaResultSrc2.GetAt(0);
		CFLScalar<int64_t>* pFlvSrcDepth2 = (CFLScalar<int64_t>*)flfaResultSrc2.GetAt(1);

		CFLFigureArray* pFlfaSrcDepth1 = (CFLFigureArray*)flfaSource2.GetAt(pFlvSrcDepth1->v);
		CFLFigure* pFlfSrcDepth1 = flfaSource2.GetAt(pFlvSrcDepth1->v);
		CFLFigure* pFlfSrcDepth2 = (CFLFigureArray*)((CFLFigureArray*)pFlfSrcDepth1)->GetAt(pFlvSrcDepth2->v);

		CFLFigureArray* pFlfaArraySrcDepth1 = (CFLFigureArray*)flfaSource2.GetAt(pFlvSrcDepth1->v);
		CFLRect<double> flrBoundary2;
		pFlfaArraySrcDepth1->GetBoundaryRect(&flrBoundary2);

		Dst2Layer0.DrawFigureImage(flrBoundary2, LIGHTORANGE, 1);
		Dst2Layer0.DrawTextImage(pFlfaArraySrcDepth1->GetCenter(), CFLString<wchar_t>().Format(L"%lld", pFlvSrcDepth1->v), LIGHTORANGE, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_CENTER);

		Dst2Layer0.DrawTextImage(pFlfaArraySrcDepth1->GetAt(pFlvSrcDepth2->v)->GetCenter(), CFLString<wchar_t>().Format(L"%lld", pFlvSrcDepth2->v), CYAN, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_CENTER);
		Dst2Layer0.DrawFigureImage(pFlfSrcDepth2, CYAN, 1);

		CFLScalar<int64_t> flvDstDepth1 = (CFLScalar<int64_t>)flfaResultDst2.GetAt(0);

		CFLFigure* pFlfDstDepth1 = flfaDestination2.GetAt(flvDstDepth1.v);

		Dst2Layer0.DrawTextImage(pFlfDstDepth1->GetCenter(), CFLString<wchar_t>().Format(L"%lld", flvDstDepth1.v), MAGENTA, BLACK, 12, false, 0, EGUIViewImageTextAlignment_CENTER_CENTER);
		Dst2Layer0.DrawFigureImage(pFlfDstDepth1, MAGENTA, 1);


		// Console 출력 // Console output
		wprintf(L"Source1 CFLPointArray\n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(flpaSource1).GetString());

		wprintf(L"Destination1 CFLCircle<double>\n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(flcDestination1).GetString());

		wprintf(L"Result1 Index of Minimum distance\n");
		wprintf(L"%lld\n\n", flvSrc.v);

		wprintf(L"\n\n");

		wprintf(L"Source2 CFLFigureArray\n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(flfaSource2).GetString());

		wprintf(L"Destination2 CFLFigureArray\n");
		wprintf(L"%s\n\n", CFigureUtilities::ConvertFigureObjectToString(flfaDestination2).GetString());

		wprintf(L"Src Result2 Index of Minimum distance\n");
		wprintf(L"Depth1 : %lld\nDepth2 : %lld\n\n", pFlvSrcDepth1->v, pFlvSrcDepth2->v);

		wprintf(L"Dst Result2 Index of Minimum distance\n");
		wprintf(L"Depth1 : %lld\n\n", flvDstDepth1.v);

		// 이미지 뷰를 갱신 합니다. // Update image view
		for(int32_t i = 0; i < 4; ++i)
			viewImage[i].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable() && viewImage[2].IsAvailable() && viewImage[3].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}