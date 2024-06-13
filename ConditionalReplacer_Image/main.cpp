#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage arrFliImage[4];

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[4];

    // 수행 결과 객체 선언 // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		// Source 이미지 로드 // Load the source image
		if((res = arrFliImage[0].Load(L"../../ExampleImages/ConditionalReplacer/CatSource.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Operand 이미지 로드 // Loads the operand image
		if((res = arrFliImage[1].Load(L"../../ExampleImages/ConditionalReplacer/CatOperand.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination1 이미지를 Source 이미지와 동일한 이미지로 생성 // Create destination1 image as same as source image
		if((res = arrFliImage[2].Assign(arrFliImage[0])).IsFail())
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Destination2 이미지를 Source 이미지와 동일한 이미지로 생성 // Create destination2 image as same as source image
		if((res = arrFliImage[3].Assign(arrFliImage[0])).IsFail())
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Source 이미지 뷰 생성 // Create Source image view
		if((res = viewImage[0].Create(100, 0, 548, 448)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Operand 이미지 뷰 생성 // Creates operand image view
		if((res = viewImage[1].Create(548, 0, 996, 448)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination1 이미지 뷰 생성 // Create destination1 image view
		if((res = viewImage[2].Create(996, 0, 1444, 448)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination2 이미지 뷰 생성 // Create destination2 image view
		if((res = viewImage[3].Create(1444, 0, 1892, 448)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		for(int32_t i = 0; i < 4; ++i)
		{
			if((res = viewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(res, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if((res = viewImage[0].SynchronizePointOfView(&viewImage[1])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if((res = viewImage[0].SynchronizePointOfView(&viewImage[2])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if((res = viewImage[0].SynchronizePointOfView(&viewImage[3])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if((res = viewImage[0].SynchronizeWindow(&viewImage[1])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if((res = viewImage[0].SynchronizeWindow(&viewImage[2])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if((res = viewImage[0].SynchronizeWindow(&viewImage[3])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Conditional Replacer 객체 생성 // Create Conditional Replacer object
		CConditionalReplacer conditionalReplacer;
		// Source 이미지 설정 // Set the source image
		conditionalReplacer.SetSourceImage(arrFliImage[0]);
		// Operand 이미지 설정 // Set the operand image
		conditionalReplacer.SetOperandImage(arrFliImage[1]);
		// Destination 이미지 설정 // Set the destination image
		conditionalReplacer.SetDestinationImage(arrFliImage[2]);

		// Operation Source 설정 // Set the Operation Source
		conditionalReplacer.SetOperationSource(EOperationSource_Image);

		// Threshold Mode 설정 // Set the Threshold Mode
		conditionalReplacer.SetThresholdMode(EThresholdMode_Single);

		// 논리 조건이 입력될 MultiVar 객체 생성 // Create the MultiVar object that push the logical condition
		CMultiVarL mvCondition1;

		// 논리 조건 입력 // Set the logical condition
		mvCondition1.PushBack(ELogicalCondition_GreaterEqual);

		// 논리 조건 설정 // Set the logical condition
		conditionalReplacer.SetLogicalCondition(mvCondition1);

		// 채널 논리 조건 설정 // Set the logical condition of channels
		conditionalReplacer.SetLogicalConditionOfChannels(ELogicalConditionOfChannels_And);

		// 조건이 참일 경우 Replacement 값 설정하기 위한 MultiVar 객체 생성 // Create the MultiVar object that sets the Replacement value if the condition is true
		CMultiVar<double> mvReplacement;

		// 1채널 Replacement 값 입력 // Push the 1Channel Replacement value
		mvReplacement.PushBack(0.0);

		// Replacement 값 설정 // Set Replacement value
		conditionalReplacer.SetReplacementValue(mvReplacement);

		// 앞에서 설정된 파라미터 대로 Conditional Replacer 수행 // Execute Conditional Replacer algorithm according to previously set parameters
		if((res = conditionalReplacer.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Conditional Replacer.");
			wprintf(res.GetString());
			break;
		}

		// Destination 이미지 설정 // Set the destination image
		conditionalReplacer.SetDestinationImage(arrFliImage[3]);

		// 채널 논리 조건 설정 // Set the logical condition of channels
		conditionalReplacer.SetLogicalConditionOfChannels(ELogicalConditionOfChannels_Or);

		// 앞에서 설정된 파라미터 대로 Conditional Replacer 수행 // Execute Conditional Replacer algorithm according to previously set parameters
		if((res = conditionalReplacer.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Conditional Replacer.");
			wprintf(res.GetString());
			break;
		}

		CGUIViewImageLayerWrap arrLayer[4];

		for(int32_t i = 0; i < 4; ++i)
		{
			// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
			// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = viewImage[i].GetLayer(0);

			// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
			arrLayer[i].Clear();
		}

		// 이미지 뷰 정보 표시 // Display image view information
		// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = arrLayer[0].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = arrLayer[1].DrawTextCanvas(&CFLPointD(0, 0), L"Operand Image", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = arrLayer[2].DrawTextCanvas(&CFLPointD(0, 0), L"Destination1 Image\nThreshold Mode: Single(Greater Equal)\nChannel Condition: And", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if((res = arrLayer[3].DrawTextCanvas(&CFLPointD(0, 0), L"Destination2 Image\nThreshold Mode: Single(Greater Equal)\nChannel Condition: Or", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 // Update image view
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);
		viewImage[2].Invalidate(true);
		viewImage[3].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage[0].IsAvailable()
			  && viewImage[1].IsAvailable()
			  && viewImage[2].IsAvailable()
			  && viewImage[3].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}