﻿#include <cstdio>

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
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/DataMatrix/Module.flif")))
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

		// Data Matrix Verifier 객체 생성 // Create Data Matrix Verifier object
		CDataMatrixVerifier dataMatrixVerifier;

		// ROI 범위 설정
		CFLRect<int32_t> flrROI(150, 25, 475, 300);

		// 처리할 이미지 설정
		dataMatrixVerifier.SetSourceImage(fliImage);
		// 처리할 ROI 설정
		dataMatrixVerifier.SetSourceROI(flrROI);
		// Decode 데이터 영역 색상 설정
		// EDataCodeColor_Auto 로 설정 시 자동으로 Decode 된다.
		dataMatrixVerifier.SetColorMode(EDataCodeColor_BlackOnWhite);

		// ISO/IEC 15415 양식 인쇄 품질 평가를 활성화합니다. 기본값은 true이며 처리하지 않아도 됩니다.
		// Enables ISO/IEC 15415 Form Print Quality Assessment. The default is true and does not require processing.
		dataMatrixVerifier.EnablePrintQuality_ISOIEC_15415(true);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters
		if(IsFail(res = dataMatrixVerifier.Execute()))
		{
			ErrorPrint(res, "Failed to execute data matrix decoder.");

			break;
		}

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// 기존에 Layer에 그려진 도형들을 삭제 // Clear the figures drawn on the existing layer
		layer.Clear();


		// ROI영역이 어디인지 알기 위해 디스플레이 한다 // Display to find out where ROI is
		// FLImaging의 Figure 객체들은 어떤 도형모양이든 상관없이 하나의 함수로 디스플레이가 가능 // FLimaging's Figure objects can be displayed as a function regardless of the shape
		// 아래 함수 DrawFigureImage는 Image좌표를 기준으로 하는 Figure를 Drawing 한다는 것을 의미하며 // The function DrawFigureImage below means drawing a picture based on the image coordinates
		// 맨 마지막 두개의 파라미터는 불투명도 값이고 1일경우 불투명, 0일경우 완전 투명을 의미한다. // The last two parameters are opacity values, which mean opacity for 1 day and complete transparency for 0 day.
		// 파라미터 순서 : 레이어 -> Figure 객체 -> 선 색 -> 선 두께 -> 면 색 -> 펜 스타일 -> 선 알파값(불투명도) -> 면 알파값 (불투명도) // Parameter order: Layer -> Figure object -> Line color -> Line thickness -> Face color -> Pen style -> Line alpha value (opacity) -> Area alpha value (opacity)
		if(IsFail(res = layer.DrawFigureImage(&flrROI, BLUE, 3, 0, EGUIViewImagePenStyle_Solid, 1, 0)))
		{
			ErrorPrint(res, "Failed to draw figures objects on the image view.\n");
			break;
		}

		// Data Matrix Verifier 결과 개수를 얻는다.
		int64_t i64Results = dataMatrixVerifier.GetResultCount();

		for(int64_t i = 0; i < i64Results; ++i)
		{
			// Data Matrix Verifier 결과를 얻어오기 위해 FLQuadD 선언
			CFLQuad<double> flqdRegion;

			// Data Matrix Verifier 결과들 중 Data Region 을 얻어옴
			if(IsFail(res = dataMatrixVerifier.GetResultDataRegion(i, flqdRegion)))
			{
				ErrorPrint(res, "Failed to get data region from the data matrix decoder object.");
				continue;
			}

			// Data Matrix 의 영역을 디스플레이 한다.
			if(IsFail(res = layer.DrawFigureImage(&flqdRegion, LIME, 2)))
			{
				ErrorPrint(res, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// Data Matrix Verifier 결과를 얻어오기 위해 FigureArray 선언
			CFLFigureArray flfaGridRegion;

			// Data Matrix Verifier 결과들 중 Grid Region 을 얻어옴
			if(IsFail(res = dataMatrixVerifier.GetResultGridRegion(i, flfaGridRegion)))
			{
				ErrorPrint(res, "Failed to get grid region from the data matrix decoder object.");
				continue;
			}

			// Data Matrix 의 Grid Region 을 디스플레이 한다.
			if(IsFail(res = layer.DrawFigureImage(&flfaGridRegion, LIME, 2)))
			{
				ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
				continue;
			}

			// Data Matrix Verifier 결과를 얻어오기 위해 FLStringW 선언
			CFLString<wchar_t> flstrDecoded;

			// Data Matrix Verifier 결과들 중 Decoded String 을 얻어옴
			if(IsFail(res = dataMatrixVerifier.GetResultDecodedString(i, flstrDecoded)))
			{
				ErrorPrint(res, "Failed to get decoded string from the data matrix decoder object.");
				continue;
			}

			wprintf(L"No. %lld Code : ", i);
			wprintf(L"%s\n", flstrDecoded.GetString());

			// String 을 디스플레이 하기 위한 기준 좌표 FLPointL 선언
			CFLPoint<int32_t> flplPos(flqdRegion.flpPoints[3]);

			// Decoded String 을 디스플레이 한다.
			// 아래 함수 DrawTextCanvas 는 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
			// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
			// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
			//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
			// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
			//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
			if(IsFail(res = layer.DrawTextImage(&flplPos, flstrDecoded, CYAN, BLACK, 20, false, flqdRegion.flpPoints[3].GetAngle(flqdRegion.flpPoints[2]))))
			{
				ErrorPrint(res, "Failed to draw string object on the image view.\n");
				continue;
			}

			// Data Matrix Verifier 결과들 중 인쇄 품질을 얻어옴 // Get print quality among Data Matrix Verifier results
			CPrintQuality_ISOIEC_15415 printQuality;

			if(IsFail(res = dataMatrixVerifier.GetResultPrintQuality_ISOIEC_15415(i, printQuality)))
			{
				ErrorPrint(res, "Failed to get print quality from the data matrix decoder object.");
				continue;
			}

			// 등급 계산이 처리되었는 지 확인
			if(printQuality.IsGraded())
			{
				CFLString<wchar_t> flsGrade;

				flsGrade.Format(L"[ISO/IEC 15415]\r\nDecoding Grade : %.1lf\r\nAxialNonuniformity Grade : %.1lf\r\nGridNonuniformity Grade : %.1lf\r\nSymbolContrast Grade : %.1lf\r\nUnusedErrorCorrection Grade : %.1lf\r\nModulation Grade : %.1lf\r\nFixedPatternDamage Grade : %.1lf\r\nHorizontalPrintGrowth Grade : %.1lf\r\nVerticalPrintGrowth Grade : %.1lf\r\nOverallSymbol Grade : %.1lf", printQuality.f64DecodingGrade, printQuality.f64AxialNonuniformityGrade, printQuality.f64GridNonuniformityGrade, printQuality.f64SymbolContrastGrade, printQuality.f64UnusedErrorCorrectionGrade, printQuality.f64ModulationGrade, printQuality.f64FixedPatternDamageGrade, printQuality.f64HorizontalPrintGrowthGrade, printQuality.f64VerticalPrintGrowthGrade, printQuality.f64OverallSymbolGrade);

				wprintf(L"%s", flsGrade.GetString());

				CFLRect<double> flrBoundary = flqdRegion.GetBoundaryRect();

				if(IsFail(res = layer.DrawTextImage(CFLPoint<double>(flrBoundary.left, flrBoundary.top), flsGrade, YELLOW, BLACK, 15, false, 0., EGUIViewImageTextAlignment_RIGHT_TOP)))
				{
					ErrorPrint(res, "Failed to draw string object on the image view.\n");
					continue;
				}
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