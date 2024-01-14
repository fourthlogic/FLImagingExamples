#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare the image object
	CFLImage fliImage;

	// 이미지 뷰 선언 // Declare the image view
	CGUIViewImageWrap viewImage;
	CResult eResult;

	do
	{
		// 이미지 로드 // Loads image
		if(IsFail(eResult = fliImage.Load(L"../../ExampleImages/ChessboardDetector/ChessBoard.flif")))
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// 이미지 뷰 생성 // Creates imageview
		if(IsFail(eResult = viewImage.Create(400, 0, 1040, 480)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// 이미지 뷰에 이미지를 디스플레이 // Display the image in the imageview
		if(IsFail(eResult = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(eResult, "Failed to set image object on the image view.\n");
			break;
		}

		CResult eResult = EResult_UnknownError;

		// ChessBoard Detector 객체 생성 // Create a ChessBoard Detector object
		CChessBoardDetector chessBoard;

		// 처리할 이미지 설정 // set the image to process
		chessBoard.SetSourceImage(fliImage);

		// ChessBoard Detector 수행 // Execute ChessBoard Detector
		if(IsFail(eResult = chessBoard.Execute()))
		{
			ErrorPrint(eResult, "Failed to execute ChessBoard Detector.\n");
			break;
		}

		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		layer.Clear();

		// ROI영역이 어디인지 알기 위해 디스플레이 한다 // Display to know where the ROI area is
		CFLFigureArray flfaResultBoardRegions;

		for(int64_t i = 0; i < chessBoard.GetResultBoardCount(); ++i)
		{
			CFLQuadD flq;
			chessBoard.GetResultBoardRegion(i, flq);

			double f64Width = flq.flpPoints[0].GetDistance(flq.flpPoints[1]);
			double f64Pitch = chessBoard.GetResultBoardAverageCellPitch(i);
			double f64Angle = flq.flpPoints[0].GetAngle(flq.flpPoints[1]);

			if(IsFail(eResult = layer.DrawFigureImage(flq, BLACK, 3)))
			{
				ErrorPrint(eResult, L"Failed to draw figure\n");
				break;
			}

			if(IsFail(eResult = layer.DrawFigureImage(flq, YELLOW, 1)))
			{
				ErrorPrint(eResult, L"Failed to draw figure\n");
				break;
			}

			if(IsFail(eResult = layer.DrawTextImage(flq.flpPoints[0], CFLString<wchar_t>().Format(L"[%d] (%d X %d) Pitch [%.01lf]", i, chessBoard.GetResultBoardColumns(i), chessBoard.GetResultBoardRows(i), chessBoard.GetResultBoardAverageCellPitch(i)), YELLOW, BLACK, (int32_t)(f64Width / 16), true, f64Angle, EGUIViewImageTextAlignment_LEFT_BOTTOM, nullptr, 1.f, 1.f, EGUIViewImageFontWeight_EXTRABOLD)))
			{
				ErrorPrint(eResult, L"Failed to draw text\n");
				break;
			}
		}

		static int32_t i32LineTransition;
		static int32_t i32VertexNumber;
		uint32_t crTable[3] = { RED, LIME, CYAN };

		for(int64_t i = 0; i < chessBoard.GetResultBoardCount(); ++i)
		{
			double f64Pitch = chessBoard.GetResultBoardAverageCellPitch(i);

			i32LineTransition = 0;
			i32VertexNumber = 0;

			CFLArray<CFLArray<TPoint<double> > > flaPoints;
			chessBoard.GetResultCornerPoints(i, flaPoints);

			CFLPointD flpLastPoint;

			for(int64_t j = 0; j < flaPoints.GetCount(); ++j)
			{
				CFLArray<TPoint<double> >& fla2 = flaPoints.GetAt(j);

				if(j)
				{
					CFLLineD fll(flpLastPoint, CFLPointD(fla2[0]));

					if(IsFail(eResult = layer.DrawFigureImage(fll, BLACK, 5)))
					{
						ErrorPrint(eResult, L"Failed to draw figure\n");
						break;
					}

					if(IsFail(eResult = layer.DrawFigureImage(fll, YELLOW, 3)))
					{
						ErrorPrint(eResult, L"Failed to draw figure\n");
						break;
					}
				}

				for(int64_t k = 0; k < fla2.GetCount(); ++k)
				{
					if(k)
					{
						CFLLineD fll(flpLastPoint, CFLPointD(fla2[k]));

						if(IsFail(eResult = layer.DrawFigureImage(fll, BLACK, 5)))
						{
							ErrorPrint(eResult, L"Failed to draw figure\n");
							break;
						}

						if(IsFail(eResult = layer.DrawFigureImage(fll, crTable[i32LineTransition++ % 3], 3)))
						{
							ErrorPrint(eResult, L"Failed to draw figure\n");
							break;
						}
					}

					flpLastPoint = fla2[k];
				}
			}

			i32LineTransition = 0;

			for(int64_t j = 0; j < flaPoints.GetCount(); ++j)
			{
				CFLArray<TPoint<double> >& fla2 = flaPoints.GetAt(j);

				double f64Angle = CFLPointD(fla2[0]).GetAngle(CFLPointD(fla2[1]));

				for(int64_t k = 0; k < fla2.GetCount(); ++k)
				{
					uint32_t crTextColor = crTable[(i32LineTransition++) % 3];

					if(!((i32VertexNumber + 1) % fla2.GetCount()))
						crTextColor = YELLOW;
					else
					{
						double f64Dx = fla2[k + 1].x - fla2[k].x;
						double f64Dy = fla2[k + 1].y - fla2[k].y;

						f64Pitch = sqrt(f64Dx * f64Dx + f64Dy * f64Dy);
					}

					if(!j)
					{
						double f64Dx = flaPoints[1][k].x - flaPoints[0][k].x;
						double f64Dy = flaPoints[1][k].y - flaPoints[0][k].y;

						f64Pitch = std::min<double>(f64Pitch, sqrt(f64Dx * f64Dx + f64Dy * f64Dy));
					}
					else
					{
						double f64Dx = flaPoints[j][k].x - flaPoints[j - 1][k].x;
						double f64Dy = flaPoints[j][k].y - flaPoints[j - 1][k].y;

						f64Pitch = std::min<double>(f64Pitch, sqrt(f64Dx * f64Dx + f64Dy * f64Dy));
					}

					if(IsFail(eResult = layer.DrawTextImage(CFLPointD(fla2[k]), CFLString<wchar_t>().Format(L"%d", i32VertexNumber++), crTextColor, BLACK, (int32_t)(f64Pitch / 2), true, f64Angle)))
					{
						ErrorPrint(eResult, L"Failed to draw text\n");
						break;
					}

					if(k)
						f64Angle = CFLPointD(fla2[k - 1]).GetAngle(CFLPointD(fla2[k]));
				}

				--i32LineTransition;
			}
		}

		// 이미지 뷰를 갱신 합니다. // Update the image view.
		viewImage.Invalidate(true);

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}