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
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/QRCode/Plate.flif")))
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

		// QR Code Decoder 객체 생성 // Create QR Code Decoder object
		CQRCodeDecoder qrCodeDecoder;

		// ROI 범위 설정
		CFLRectL flrROI(210, 60, 400, 250);

		// 처리할 이미지 설정
		qrCodeDecoder.SetSourceImage(fliImage);
		// 처리할 ROI 설정
		qrCodeDecoder.SetSourceROI(flrROI);
		// Decode 데이터 영역 색상 설정
		// EQRCodeColors_Auto 로 설정 시 자동으로 Decode 된다.
		qrCodeDecoder.SetColorMode(EDataCodeColor_WhiteOnBlack);

		// 앞서 설정된 파라미터 대로 알고리즘 수행 // Execute algorithm according to previously set parameters

		if(IsFail(eResult = qrCodeDecoder.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute QR code decoder.");

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
		if(IsFail(eResult = layer.DrawFigureImage(&flrROI, BLUE, 3)))
		{
			ErrorPrint(eResult, "Failed to draw figures objects on the image view.\n");
			break;
		}

		// QR Code Decoder 결과 개수를 얻는다.
		int64_t i64Results = qrCodeDecoder.GetResultCount();

		for(int64_t i = 0; i < i64Results; ++i)
		{

			// QR Code Decoder 결과를 얻어오기 위해 FLQuadD 선언
			CFLQuadD flqdRegion;

			// QR Code Decoder 결과들 중 Data Region 을 얻어옴
			if(IsFail(eResult = qrCodeDecoder.GetResultDataRegion(i, flqdRegion)))
			{
				ErrorPrint(eResult, "Failed to get data region from the QR code decoder object.");
				continue;
			}

			// QR Code 의 영역을 디스플레이 한다.
			if(IsFail(eResult = layer.DrawFigureImage(&flqdRegion, LIME, 2)))
			{
				ErrorPrint(eResult, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// QR Code Decoder 결과를 얻어오기 위해 FigureArray 선언
			CFLFigureArray flfaGridRegion;

			// QR Code Decoder 결과들 중 Grid Region 을 얻어옴
			if(IsFail(eResult = qrCodeDecoder.GetResultGridRegion(i, flfaGridRegion)))
			{
				ErrorPrint(eResult, "Failed to get grid region from the QR code decoder object.");
				continue;
			}

			// QR Code 의 Grid Region 을 디스플레이 한다.
			if(IsFail(eResult = layer.DrawFigureImage(&flfaGridRegion, LIME, 2)))
			{
				ErrorPrint(eResult, "Failed to draw figure objects on the image view.\n");
				continue;
			}

			// QR Code Decoder 결과를 얻어오기 위해 FLFigureArray 선언
			CFLFigureArray flfaFinderPattern;

			// QR Code Decoder 결과들 중 Finder Pattern 을 얻어옴
			qrCodeDecoder.GetResultFinderPattern(i, flfaFinderPattern);

			// QR Code 의 Finder Pattern 을 디스플레이 한다.
			if(IsFail(eResult = layer.DrawFigureImage(&flfaFinderPattern, CYAN, 5)))
			{
				ErrorPrint(eResult, "Failed to draw figure object on the image view.\n");
				continue;
			}

			// QR Code Decoder 결과를 얻어오기 위해 FLStringW 선언
			CFLString<wchar_t> flstrDecoded;

			// QR Code Decoder 결과들 중 Decoded String 을 얻어옴
			if(IsFail(eResult = qrCodeDecoder.GetResultDecodedString(i, flstrDecoded)))
			{
				ErrorPrint(eResult, "Failed to get decoded string from the QR code decoder object.");
				continue;
			}

			CQRCodeSpec codeSpec;
			qrCodeDecoder.GetResultQRCodeSpec(i, codeSpec);

			EQRCodeErrorCorrectionLevel eECLevel = codeSpec.GetQRCodeErrorCorrectionLevel();
			EQRCodeSymbolVersion1 eSymbol1 = EQRCodeSymbolVersion1_None;
			EQRCodeSymbolVersion2 eSymbol2 = EQRCodeSymbolVersion2_None;
			codeSpec.GetQRCodeSymbolVersion(eSymbol1, eSymbol2);

			CFLString<wchar_t> flsAdditionalData;

			wprintf(L"No. %lld Code : ", i);

			switch(eECLevel)
			{
			case FLImaging::AdvancedFunctions::EQRCodeErrorCorrectionLevel_Low:
				flsAdditionalData = L"[Low";
				break;
			case FLImaging::AdvancedFunctions::EQRCodeErrorCorrectionLevel_Medium:
				flsAdditionalData = L"[Medium";
				break;
			case FLImaging::AdvancedFunctions::EQRCodeErrorCorrectionLevel_Quartile:
				flsAdditionalData = L"[Quartile";
				break;
			case FLImaging::AdvancedFunctions::EQRCodeErrorCorrectionLevel_High:
				flsAdditionalData = L"[High";
				break;
			default:
				break;
			}

			if(eSymbol1 != EQRCodeSymbolVersion1_None)
			{
				int32_t i32SymbolValue = (int32_t)eSymbol1;
				int32_t i32Symbol = 0;

				for(int32_t i = 0; i < 20; ++i)
				{
					if((i32SymbolValue >> i) & 1)
					{
						i32Symbol = i + 1;
						break;
					}
				}

				flsAdditionalData.AppendFormat(L"-%d]", i32Symbol);
			}

			if(eSymbol2 != EQRCodeSymbolVersion2_None)
			{
				int32_t i32SymbolValue = (int32_t)eSymbol2;
				int32_t i32Symbol = 0;

				for(int32_t i = 0; i < 20; ++i)
				{
					if((i32SymbolValue >> i) & 1)
					{
						i32Symbol = i + 21;
						break;
					}
				}

				flsAdditionalData.AppendFormat(L"-%d]", i32Symbol);
			}

			wprintf(L"%s\n", flstrDecoded.GetString());

			// Decoded String 을 디스플레이 한다.
			// 아래 함수 DrawTextCanvas은 Screen좌표를 기준으로 하는 String을 Drawing 한다. // The function DrawTextCanvas below draws a String based on the screen coordinates.
			// 색상 파라미터를 EGUIViewImageLayerTransparencyColor 으로 넣어주게되면 배경색으로 처리함으로 불투명도를 0으로 한것과 같은 효과가 있다. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
			// 파라미터 순서 : 레이어 -> 기준 좌표 Figure 객체 -> 문자열 -> 폰트 색 -> 면 색 -> 폰트 크기 -> 실제 크기 유무 -> 각도 ->
			//                 얼라인 -> 폰트 이름 -> 폰트 알파값(불투명도) -> 면 알파값 (불투명도) -> 폰트 두께 -> 폰트 이텔릭
			// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
			//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
			if(IsFail(eResult = layer.DrawTextImage(&flqdRegion.flpPoints[0], flsAdditionalData, YELLOW, BLACK, 20, false, 0., EGUIViewImageTextAlignment_LEFT_BOTTOM)))
			{
				ErrorPrint(eResult, "Failed to draw string object on the image view.\n");
				continue;
			}

			if(IsFail(eResult = layer.DrawTextImage(&flqdRegion.flpPoints[3], flstrDecoded, CYAN, BLACK, 20, false, 0., EGUIViewImageTextAlignment_LEFT_TOP)))
			{
				ErrorPrint(eResult, "Failed to draw string object on the image view.\n");
				continue;
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