#include <cstdio>

#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h"


int main()
{
	// You must call the following function once
	// before using any features of the FLImaging(R) library
	CLibraryUtilities::Initialize();

	// 이미지 객체 선언 // Declare image object
	CFLImage fliImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage;

	do
	{
		CResult res = EResult_UnknownError;
		// 이미지 로드 // Load image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/Blob/AlignBall.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(IsFail(res = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		if(IsFail(res = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// BlobSubsampled 객체 생성 // Create BlobSubsampled object
		CBlobSubsampled blobSubsampled;

		// 처리할 이미지 설정
		blobSubsampled.SetSourceImage(fliImage);
		// 논리 조건 설정
		blobSubsampled.SetLogicalCondition(ELogicalCondition_Less);
		// 임계값 설정,  위의 조건과 아래의 조건이 합쳐지면 127보다 작은 객체를 검출
		blobSubsampled.SetThreshold(127);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(res = blobSubsampled.Execute()))
		{
			ErrorPrint(res, "Failed to execute BlobSubsampled.");
			break;
		}

		// 결과 객체들 중 해당되는 조건을 가진 객체를 제거
		
		// 면적이 500보다 작은 객체들을 제거
		if(IsFail(res = blobSubsampled.Filter(CBlobSubsampled::EFilterItem_Area, 500, ELogicalCondition_LessEqual)))
		{
			ErrorPrint(res, "BlobSubsampled filtering algorithm error occurred.");
			break;
		}

		// Blob 결과를 얻어오기 위해 FigureArray 선언
		CFLFigureArray flfaBoundaryRects;

		// Blob 결과들 중 Boundary Rectangle 을 얻어옴
		if(IsFail(res = blobSubsampled.GetResultBoundaryRects(flfaBoundaryRects)))
		{
			ErrorPrint(res, "Failed to get boundary rects from the BlobSubsampled object.");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layer.Clear();

		// flfaBoundaryRects 는 Figure들의 배열이기 때문에 Layer에 넣기만 해도 모두 드로윙이 가능하다.
		// 아래 함수 DrawFigureImage는 Image좌표를 기준으로 하는 Figure를 Drawing 한다는 것을 의미하며 // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// 맨 마지막 두개의 파라미터는 불투명도 값이고 1일경우 불투명, 0일경우 완전 투명을 의미한다. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// 여기서 0.25이므로 옅은 반투명 상태라고 볼 수 있다.
		// 파라미터 순서 : 레이어 -> Figure 객체 -> 선 색 -> 선 두께 -> 면 색 -> 펜 스타일 -> 선 알파값(불투명도) -> 면 알파값 (불투명도) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(res = layer.DrawFigureImage(&flfaBoundaryRects, RED, 1, RED, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
			break;
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImage.Invalidate(true);

		// Rect 정보값을 각각 확인하는 코드
		for(int64_t i = 0; i < flfaBoundaryRects.GetCount(); ++i)
		{
			CFLRect<double>* pFlrRect = (CFLRect<double>*)flfaBoundaryRects.GetAt(i);

			if(pFlrRect)
			{
				printf("No. [%lld]\n", i);
				printf("LeftTop     : (%lf,%lf)\n", pFlrRect->left, pFlrRect->top);
				printf("RightBottom : (%lf,%lf)\n", pFlrRect->right, pFlrRect->bottom);
				printf("Width  : %lf\n", pFlrRect->GetWidth());
				printf("Height : %lf\n", pFlrRect->GetHeight());
				printf("Center : (%lf, %lf)\n\n", pFlrRect->GetCenter().x, pFlrRect->GetCenter().y);

				CFLString<wchar_t> strNumber;
				strNumber.Format(L"[%d]", i);

				CFLString<wchar_t> strLeftTop, strRightBottom, strInfo;
				strLeftTop.Format(L"(%lf, %lf)", pFlrRect->left, pFlrRect->top);
				strRightBottom.Format(L"(%lf, %lf)", pFlrRect->right, pFlrRect->bottom);
				strInfo.Format(L"Width : %lf\nHeight : %lf\nCenter : (%lf, %lf)", pFlrRect->GetWidth(), pFlrRect->GetHeight(), pFlrRect->GetCenter().x, pFlrRect->GetCenter().y);

		        // 아래 함수 DrawTextImage는 Image좌표를 기준으로 하는 Text를 Drawing 한다는 것을 의미한다.
				// 파라미터 순서 : 레이어 -> 문자열 좌표 -> 문자열 지정 -> 문자열 색 -> 면 색 -> 폰트 크기 -> 실제 크기로 그릴지의 여부 -> 각도 -> 문자열의 위치 기준
				layer.DrawTextImage(&CFLPoint<double>(pFlrRect->GetCenter()), strNumber, CYAN, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
				layer.DrawTextImage(&CFLPoint<double>(pFlrRect->left, pFlrRect->top), strLeftTop, YELLOW, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_RIGHT_BOTTOM);
				layer.DrawTextImage(&CFLPoint<double>(pFlrRect->right, pFlrRect->bottom), strRightBottom, YELLOW, BLACK);
				layer.DrawTextImage(&CFLPoint<double>(pFlrRect->right, pFlrRect->top), strInfo, LIME, BLACK, 12, false, 0., FLImaging::GUI::EGUIViewImageTextAlignment_LEFT_BOTTOM);
			}
		}

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}