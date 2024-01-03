#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage;

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
		if(IsFail(eResult = viewImage.Create(400, 0, 1424, 768)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		if(IsFail(eResult = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		// BlobSubsampled 객체 생성 // Create BlobSubsampled object
		CBlobSubsampled blob;

		// 처리할 이미지 설정
		blob.SetSourceImage(fliImage);

		// 논리 조건 설정
		blob.SetLogicalCondition(ELogicalCondition_Less);

		// 임계값 설정,  위의 조건과 아래의 조건이 합쳐지면 50보다 작은 객체를 검출
		blob.SetThreshold(50);

		// BlobSubsampled Result Type mask 생성 (Contour, GravityCenter)
		int32_t i32ResultTypeMask = (int32_t)CBlobSubsampled::EBlobResultType_Contour | (int32_t)CBlobSubsampled::EBlobResultType_GravityCenter;

		// Result Type 설정
		blob.SetResultType((CBlobSubsampled::EBlobResultType)i32ResultTypeMask);

		// Subsampling Level 설정
		blob.SetSubsamplingLevel(3);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(eResult = blob.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute BlobSubsampled.");
			break;
		}

		 // BoundaryRect의 20보다 작은 너비를 가진 객체들을 제거
		if(IsFail(eResult = blob.Filter(CBlobSubsampled::EFilterItem_BoundaryRectWidth, 20, ELogicalCondition_Less)))
		{
			ErrorPrint(eResult, "BlobSubsampled filtering algorithm error occurred.");
			break;
		}

		// BoundaryRect의  20보다 작은 높이를 가진 객체들을 제거
		if(IsFail(eResult = blob.Filter(CBlobSubsampled::EFilterItem_BoundaryRectHeight, 20, ELogicalCondition_Less)))
		{
			ErrorPrint(eResult, "BlobSubsampled filtering algorithm error occurred.");
			break;
		}

		// BlobSubsampled 결과를 얻어오기 위해 CFLFigureArray 선언
		CFLFigureArray flfaContours;
		CFLFigureArray flfGravityCenter;

		// BlobSubsampled 결과들 중 Contour 을 얻어옴
		if(IsFail(eResult = blob.GetResultContours(flfaContours)))
		{
			ErrorPrint(eResult, "Failed to get contours from the BlobSubsampled object.");
			break;
		}

		// BlobSubsampled 결과들 중 Gravity Center 을 얻어옴
		if(IsFail(eResult = blob.GetResultGravityCenters(flfGravityCenter)))
		{
			ErrorPrint(eResult, "Failed to get gravity center from the BlobSubsampled object.");
			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layer.Clear();

		// flfaContours 는 Figure들의 배열이기 때문에 Layer에 넣기만 해도 모두 드로윙이 가능하다.
		// 아래 함수 DrawFigureImage는 Image좌표를 기준으로 하는 Figure를 Drawing 한다는 것을 의미하며 // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// 맨 마지막 두개의 파라미터는 불투명도 값이고 1일경우 불투명, 0일경우 완전 투명을 의미한다. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// 여기서 0.25이므로 옅은 반투명 상태라고 볼 수 있다.
		// 파라미터 순서 : 레이어 -> Figure 객체 -> 선 색 -> 선 두께 -> 면 색 -> 펜 스타일 -> 선 알파값(불투명도) -> 면 알파값 (불투명도) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(eResult = layer.DrawFigureImage(&flfaContours, BLUE, 1, BLUE, EGUIViewImagePenStyle_Solid, 1, 0.25)))
		{
			ErrorPrint(eResult, "Failed to draw figure objects on the image view.\n");
			break;
		}

		CFLString<wchar_t> str;
		CFLString<wchar_t> flsTextResult;

		// Image View 객체에 Index, Gravity Center 순서대로 출력
		if(flfaContours.GetCount())
		{
			for(int64_t i = 0; i < flfaContours.GetCount(); ++i)
			{
				CFLPointD* flpCenter = dynamic_cast<CFLPointD*>(flfGravityCenter.GetAt(i));

				if(!flpCenter)
					continue;

				str.Format(L"[%lld]\n\n\n\n", i);
				flsTextResult.Format(L"\n\n\n\n\n\nGravity Center\nX : %.2lf Y : %.2lf", flpCenter->x, flpCenter->y);

				// Image View 출력
				layer.DrawTextImage(CFLPointD(flfaContours[i]), str, LIME, BLACK, 10, false, 0., GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
				layer.DrawTextImage(CFLPointD(flfaContours[i]), flsTextResult, YELLOW, BLACK, 10, false, 0., GUI::EGUIViewImageTextAlignment_CENTER_CENTER);
				layer.DrawFigureImage(flpCenter->MakeCrossHair(10, true), ORANGERED, 1, ORANGERED, EGUIViewImagePenStyle_Solid, 1, 0.25);

				// 콘솔에 출력
				printf("[%lld] Gravity Center x : %.2lf\ty : %.2lf \n", i, flpCenter->x, flpCenter->y);
			}
		}

		// 이미지 뷰를 갱신 합니다. // Update image view
		viewImage.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}
