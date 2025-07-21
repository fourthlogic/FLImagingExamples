﻿#include <cstdio>

#include "../CommonHeader/ErrorPrint.h"
#include <FLImaging.h>


enum EType
{
	EType_Source = 0,
	EType_Destination1,
	EType_Destination2,
	ETypeCount,
};

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	CResult res = EResult_UnknownError;

	do
	{
		// Source 이미지 로드 // Load the source image
		if((res = arrFliImage[EType_Source].Load(L"../../ExampleImages/OperationBlend/Sky.flif")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the image file.\n");
			break;
		}

		// Destination1 이미지를 Source 이미지와 동일한 이미지로 생성 // Create destination1 image as same as source image
		if((res = arrFliImage[EType_Destination1].Assign(arrFliImage[EType_Source])).IsFail())
		{
			ErrorPrint(res, L"Failed to assign the image file.\n");
			break;
		}

		// Destination1 이미지를 Source 이미지와 동일한 이미지로 생성 // Create destination1 image as same as source image
		if((res = arrFliImage[EType_Destination2].Assign(arrFliImage[EType_Source])).IsFail())
		{
			ErrorPrint(res, L"Failed to assign the image file.\n");
			break;
		}

		// Source 이미지 뷰 생성 // Create Source image view
		if((res = arrViewImage[EType_Source].Create(100, 0, 612, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination1 이미지 뷰 생성 // Create destination1 image view
		if((res = arrViewImage[EType_Destination1].Create(612, 0, 1124, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination2 이미지 뷰 생성 // Create destination2 image view
		if((res = arrViewImage[EType_Destination2].Create(1124, 0, 1636, 512)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// 이미지 뷰에 이미지를 디스플레이 // Display an image in an image view
		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			if((res = arrViewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(res, L"Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if((res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination1])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰의 시점을 동기화 한다 // Synchronize the viewpoints of the two image views
		if((res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination2])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize view\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if((res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination1])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// 두 이미지 뷰 윈도우의 위치를 맞춤 // Synchronize the positions of the two image view windows
		if((res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination2])).IsFail())
		{
			ErrorPrint(res, L"Failed to synchronize window.\n");
			break;
		}

		// 알고리즘 동작 결과 // Algorithm execution result
		res = EResult_UnknownError;

		// Operation Blend 객체 생성 // Create Operation Blend object
		COperationBlend blend;
		// Source 이미지 설정 // Set the source image
		blend.SetSourceImage(arrFliImage[EType_Source]);
		// Destination 이미지 설정 // Set the destination image
		blend.SetDestinationImage(arrFliImage[EType_Destination1]);
		// 연산 방식 설정 // Set operation source
		blend.SetOperationSource(EOperationSource_Scalar);
		// blend 값 설정 // Set Blend value
		blend.SetScalarValue(CMultiVar<double>(30, 0, 0));
		// Blend Ratio 설정 // Set Blend Ratio
		blend.SetSourceRatio(0.75f);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((res = blend.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute operation blend.");
			break;
		}

		// Destination 이미지 설정 // Set the destination image
		blend.SetDestinationImage(arrFliImage[EType_Destination2]);
		// 연산 방식 설정 // Set operation source
		blend.SetOperationSource(EOperationSource_Scalar);
		// blend 값 설정 // Set blend value
		blend.SetScalarValue(CMultiVar<double>(0, 30, 0));

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if((res = blend.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute operation blend.");
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// 출력을 위한 이미지 레이어를 얻어옵니다. //  Gets the image layer for output.
			// 따로 해제할 필요 없음 // No need to release separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
			arrLayer[i].Clear();
		}

		// View 정보를 디스플레이 합니다. // Display View information.
		// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
		//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((res = arrLayer[EType_Source].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination1 Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}
		if((res = arrLayer[EType_Destination1].DrawTextCanvas(&CFLPoint<double>(0, 32), L"Source Ratio 0.75, Scalar(30,0,0) Ratio 0.25", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		if((res = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination2 Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}
		if((res = arrLayer[EType_Destination2].DrawTextCanvas(&CFLPoint<double>(0, 32), L"Source Ratio 0.75, Scalar(0,30,0) Ratio 0.25", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text\n");
			break;
		}

		// 이미지 뷰를 갱신 // Update image view
		arrViewImage[EType_Source].Invalidate(true);
		arrViewImage[EType_Destination1].Invalidate(true);
		arrViewImage[EType_Destination2].Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(arrViewImage[EType_Source].IsAvailable()
			  && arrViewImage[EType_Destination1].IsAvailable()
			  && arrViewImage[EType_Destination2].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}