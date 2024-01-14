#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewSourceImage;

	do
	{
		CResult eResult = EResult_UnknownError;
		// 이미지 로드 // Load image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/Blob/AlignBall.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(eResult = viewSourceImage.Create(200, 0, 812, 512)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(eResult = viewSourceImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// Image 크기에 맞게 view의 크기를 조정 // Zoom the view to fit the image size
		if(IsFail(eResult = viewSourceImage.ZoomFit()))
		{
			ErrorPrint(eResult, "Failed to zoom fit\n");
			break;
		}

		// Blob 객체 생성 // Create Blob object
		CBlob blob;

		// 처리할 이미지 설정
		blob.SetSourceImage(fliImage);

		// 논리 조건 설정
		CMultiVarLL mvLogical((int64_t)ELogicalCondition_Less);
		blob.SetLogicalCondition(mvLogical);
		// 임계값 설정,  위의 조건과 아래의 조건이 합쳐지면 127보다 작은 객체를 검출
		CMultiVar<double> mvThreshold(127.0);
		blob.SetThreshold(mvThreshold);
		blob.SetThresholdMode(EThresholdMode_Single);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(eResult = blob.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute Blob.");
			break;
		}

		// 결과 객체들 중 해당되는 조건을 가진 객체를 제거

		// 20보다 작거나 큰 장변 길이를 가진 객체들을 제거
		if(IsFail(eResult = blob.Filter(CBlob::EFilterItem_MinimumEnclosingRectangleLongSideLength, 20, ELogicalCondition_LessEqual)))
		{
			ErrorPrint(eResult, "Blob filtering algorithm error occurs.");
			break;
		}

		// circularity 가 0.9보다 작은 객체들을 제거
		if(IsFail(eResult = blob.Filter(CBlob::EFilterItem_Circularity, 0.9, ELogicalCondition_Less)))
		{
			ErrorPrint(eResult, "Blob filtering algorithm error occurred.");
			break;
		}

		CFLArray<int32_t> flaItem;
		CFLArray<int32_t> flaOrder;

		// Blob 결과의 Mean, Variance, StandardDeviation 을 얻어오기 위한 Statistics 객체 선언
		CImageStatistics imgStatistics;
		int64_t i64ResultCount = blob.GetResultCount();

		// Statistics 소스 이미지 설정
		imgStatistics.SetSourceImage(fliImage);

		// Blob 결과를 얻어오기 위해 FigureArray 선언
		CFLFigureArray flfaContour;

		// Blob 결과들 중 Contour 를 얻어옴
		if(IsFail(eResult = blob.GetResultContours(flfaContour)))
		{
			ErrorPrint(eResult, "Failed to get boundary rects from the Blob object.");
			break;
		}

		CFLString<wchar_t> strMean, strVariance, strStandardDeviation;

		// 각 Coutour에 맞는 Mean, Variance, StandardDeviation 을 얻어오는 코드
		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			imgStatistics.SetSourceROI(flfaContour.GetAt(i));

			CMultiVar<double> mvMean;
			if(IsFail(eResult = imgStatistics.GetMean(mvMean)))
			{
				ErrorPrint(eResult, "Failed to get Mean Value from the Blob object.");
				break;
			}

			strMean.Format(L"No. %d Mean : ", i);
			strMean.AppendFormat(L"%lf", mvMean.GetAt(0));
			wprintf_s(L"%s\n", strMean.GetString());

			CMultiVar<double> mvVariance;
			if(IsFail(eResult = imgStatistics.GetVariance(mvVariance)))
			{
				ErrorPrint(eResult, "Failed to get Variance Value from the Blob object.");
				break;
			}

			strVariance.Format(L"No. %d Variance : ", i);
			strVariance.AppendFormat(L"%lf", mvVariance.GetAt(0));
			wprintf_s(L"%s\n", strVariance.GetString());

			CMultiVar<double> mvStandardDeviation;
			if(IsFail(eResult = imgStatistics.GetStandardDeviation(mvStandardDeviation)))
			{
				ErrorPrint(eResult, "Failed to get StandardDeviation Value from the Blob object.");
				break;
			}

			strStandardDeviation.Format(L"No. %d StandardDeviation : ", i);
			strStandardDeviation.AppendFormat(L"%lf", mvStandardDeviation.GetAt(0));
			wprintf_s(L"%s\n", strStandardDeviation.GetString());
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately

		CGUIViewImageLayerWrap layerSource = viewSourceImage.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layerSource.Clear();

		if(IsFail(eResult = layerSource.DrawTextCanvas(&CFLPointD(), L"Source", YELLOW, BLACK, 30)))
		{
			ErrorPrint(eResult, "Failed to draw text on the image view.\n");
			break;
		}

		// flfaOriginBoundaryRects 는 Figure들의 배열이기 때문에 Layer에 넣기만 해도 모두 드로윙이 가능하다.
		// 아래 함수 DrawFigureImage는 Image좌표를 기준으로 하는 Figure를 Drawing 한다는 것을 의미하며 // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// 맨 마지막 두개의 파라미터는 불투명도 값이고 1일경우 불투명, 0일경우 완전 투명을 의미한다. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// 여기서 0.25이므로 옅은 반투명 상태라고 볼 수 있다.
		// 파라미터 순서 : 레이어 -> Figure 객체 -> 선 색 -> 선 두께 -> 면 색 -> 펜 스타일 -> 선 알파값(불투명도) -> 면 알파값 (불투명도) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(eResult = layerSource.DrawFigureImage(&flfaContour, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(eResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// Rect 정보값을 각각 확인하는 코드
		for(int64_t i = 0; i < flfaContour.GetCount(); ++i)
		{
			CFLRectL* pFlrRect = (CFLRectL*)flfaContour.GetAt(i);

			CFLString<wchar_t> strNumber;
			strNumber.Format(L"%d", i);

			layerSource.DrawTextImage(&CFLPointD(pFlrRect->GetCenter()), (wchar_t*)strNumber.GetString(), CYAN);
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewSourceImage.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewSourceImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}