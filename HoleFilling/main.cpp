#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"


enum EType
{
	EType_Source = 0,
	EType_Destination,
	ETypeCount,
};

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	do
	{
		// Source 이미지 로드 // Load the source image
		if(IsFail(res = arrFliImage[EType_Source].Load(L"../../ExampleImages/HoleFilling/TodoList.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination 이미지를 Source 이미지와 동일한 이미지로 생성 // Create destination image as same as source image
		if(IsFail(res = arrFliImage[EType_Destination].Assign(arrFliImage[EType_Source])))
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Source 이미지 뷰 생성 // Create Source image view
		if(IsFail(res = arrViewImage[EType_Source].Create(100, 0, 612, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination 이미지 뷰 생성 // Create destination image view
		if(IsFail(res = arrViewImage[EType_Destination].Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if(IsFail(res = arrViewImage[i].SetImagePtr(&arrFliImage[i])))
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if(IsFail(res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the positions of the two image view windows
		if(IsFail(res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// 알고리즘 객체 생성 // Create Algorithm object
		CHoleFilling alg;
		// Source 이미지 설정 // Set the source image
		if((res = alg.SetSourceImage(arrFliImage[EType_Source])).IsFail())
			break;
		// Destination 이미지 설정 // Set the destination image
		if((res = alg.SetDestinationImage(arrFliImage[EType_Destination])).IsFail())
			break;
		// 처리할 Hole Area 넓이 범위 설정 // Set hole area range to process
		if((res = alg.SetMinimumHoleArea(10)).IsFail())
			break;
		// 처리할 Hole Area 넓이 범위 설정 // Set hole area range to process
		if((res = alg.SetMaximumHoleArea(99999999999)).IsFail())
			break;
		// 이미지 경계와 맞닿은 hole 의 처리 여부 설정 // Set whether to process holes that touch the image boundary
		if((res = alg.EnableIgnoreBoundaryHole(true)).IsFail())
			break;
		// Threshold 를 통과한 픽셀이 hole 인지 object 인지 설정 // Set whether the pixel that passed the threshold is a hole or an object
		if((res = alg.SetThresholdPassTarget(CHoleFilling::EThresholdPassTarget_Object)).IsFail())
			break;
		// Threshold 수와 결합 방식을 의미하는 Threshold 모드 설정 // Threshold mode setting, which refers to the number of threshold and combination method
		if((res = alg.SetThresholdMode(EThresholdMode_Dual_And)).IsFail())
			break;
		// 각 Threshold 내에서 채널 별 논리 결과 간의 결합 방식을 의미하는 Logical Condition Of Channels 설정 // Set the Logical Condition Of Channels, which refers to the combination method between logical results for each channel within each Threshold
		if((res = alg.SetLogicalConditionOfChannels(ELogicalConditionOfChannels_And)).IsFail())
			break;
		// Hole 영역을 채우는 방식을 설정 // Set the method of filling the hole area
		if((res = alg.SetFillingMethod((CHoleFilling::EFillingMethod)0)).IsFail())
			break;
		// Harmonic Interpolation 의 Precision 값 설정 // Set precision value for Harmonic Interpolation
		if((res = alg.SetPrecision(0.1)).IsFail())
			break;
		// Harmonic Interpolation 의 Max Iteration 값 설정 // Set max iteration value for Harmonic Interpolation
		if((res = alg.SetMaxIteration(100)).IsFail())
			break;
		// 첫 번째 Threshold 의 채널 별 논리 연산자와 값 설정 // Set the logical operator and value for each channel of the first Threshold
		CMultiVar<uint64_t> mvThresholdCondition1 = CMultiVar<uint64_t>((uint64_t)ELogicalCondition_GreaterEqual, (uint64_t)ELogicalCondition_GreaterEqual, (uint64_t)ELogicalCondition_GreaterEqual);
		if((res = alg.SetThresholdCondition(EThresholdIndex_First, mvThresholdCondition1)).IsFail())
			break;
		CMultiVar<uint64_t> mvThresholdValue1U64 = CMultiVar<uint64_t>(175, 230, 240);
		if((res = alg.SetThresholdValue(EThresholdIndex_First, mvThresholdValue1U64)).IsFail())
			break;
		// 두 번째 Threshold 의 채널 별 논리 연산자와 값 설정 // Set the logical operator and value for each channel of the second Threshold
		CMultiVar<uint64_t> mvThresholdCondition2 = CMultiVar<uint64_t>((uint64_t)ELogicalCondition_Less, (uint64_t)ELogicalCondition_Less, (uint64_t)ELogicalCondition_Less);
		if((res = alg.SetThresholdCondition(EThresholdIndex_Second, mvThresholdCondition2)).IsFail())
			break;
		CMultiVar<uint64_t> mvThresholdValue2U64 = CMultiVar<uint64_t>(200, 240, 255);
		if((res = alg.SetThresholdValue(EThresholdIndex_Second, mvThresholdValue2U64)).IsFail())
			break;

		// 알고리즘 실행 // Execute Algorithm
		if(IsFail(res = alg.Execute()))
		{
			ErrorPrint(res, "Failed to execute HoleFilling.");
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
			// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
			arrLayer[i].Clear();
		}

		const CFLFigure* pFlfHoleContours = alg.GetSelectedPageFigureObject();

		if(IsFail(res = arrLayer[EType_Source].DrawFigureImage(pFlfHoleContours, CYAN)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		// 이미지 뷰 정보 표시 // Display image view information
		// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if(IsFail(res = arrLayer[EType_Source].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = arrLayer[EType_Destination].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Image", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 // Update image view
		arrViewImage[EType_Source].Invalidate(true);
		arrViewImage[EType_Destination].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(arrViewImage[EType_Source].IsAvailable()
			&& arrViewImage[EType_Destination].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}