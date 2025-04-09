﻿#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 이미지 객체 선언 // Declare image object
	CFLImage fliImage;

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage[3];

	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	do
	{
		// Source Coordinate View 생성 // Create Source Coordinate View
		if(IsFail(res = viewImage[0].Create(200, 0, 700, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination Coordinate View 생성 // Create Destination Coordinate View
		if(IsFail(res = viewImage[1].Create(700, 0, 1200, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Restore Coordinate View 생성 // Create Restore Coordinate View
		if(IsFail(res = viewImage[2].Create(1200, 0, 1700, 500)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// 각 이미지 뷰의 시점을 동기화 한다. // Synchronize the viewpoint of each image view.
		if(IsFail(res = viewImage[0].SynchronizePointOfView(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}
		if(IsFail(res = viewImage[1].SynchronizePointOfView(&viewImage[2])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// 각 이미지 뷰 윈도우의 위치를 동기화 한다 // Synchronize the position of each image view window
		if(IsFail(res = viewImage[0].SynchronizeWindow(&viewImage[1])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}
		if(IsFail(res = viewImage[1].SynchronizeWindow(&viewImage[2])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// 화면상에 잘 보이도록 좌표 95배율을 적용 // Apply 95 magnification to the coordinates so that they can be seen clearly on the screen
		double f64Scale = 95;
		// 화면상에 잘 보이도록 시점 Offset 조정 // Adjust the viewpoint offset so that it can be seen clearly on the screen
		double f64CenterCoordX = 2.007;
		double f64CenterCoordY = 1.865;
		viewImage[0].SetViewCenterAndScale(CFLPoint<double>(f64CenterCoordX, f64CenterCoordY), f64Scale);

		// 화면에 출력하기 위해 Image View에서 레이어 0번을 얻어옴 // Obtain layer 0 number from image view for display
		// 이 객체는 이미지 뷰에 속해있기 때문에 따로 해제할 필요가 없음 // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer[3];

		for(int32_t i = 0; i < 3; ++i)
			layer[i] = viewImage[i].GetLayer(0);

		// 화면상 좌표(고정 좌표)에 Source 좌표 View 임을 표시
		// Indicate that the source coordinates are View at the coordinates (fixed coordinates) on the screen
		layer[0].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Source Coordinate", YELLOW, BLACK, 30);
		// 화면상 좌표(고정 좌표)에 Target 좌표 View 임을 표시
		// Indicate that it is the target coordinate view on the screen coordinates (fixed coordinates)
		layer[1].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Target Coordinate", YELLOW, BLACK, 30);
		// 화면상 좌표(고정 좌표)에 Restore 좌표 View 임을 표시
		// Indicate Restore Coordinates View on the screen coordinates (fixed coordinates)
		layer[2].DrawTextCanvas(&CFLPoint<int32_t>(0, 0), L"Restore Coordinate (from Target)", YELLOW, BLACK, 30);

		// 좌표 매핑용 클래스 선언 // Class declaration for coordinate mapping
		CThinPlateSplineMapping tps;

		// 그리드를 (5,5)로 초기화 // initialize the grid to (5,5)
		CFLPoint<int32_t> flpGridSize(5, 5);

		// 만약 기존 저장된 매핑 데이터가 있다면 해당 데이터를 로드합니다. // If there is previously saved mapping data, load the data.
		// 두번째 실행부터는 파일이 생성될 것이기 때문에 아래 세팅과정을 수행하지 않고 지나가게 됩니다. // Since the file will be created from the second execution, the setting process below will be skipped.
		// 계속 새로 데이터를 생성하는것을 테스트 하려 한다면 아래 Load함수와 관련된 if문 1줄을 삭제하면 됩니다. // If you want to test continuously creating new data, you can delete one line of the if statement related to the Load function below.
		if(IsFail(res = tps.Load(L"MappingData.fltps")))
		{
			CFLPoint<int32_t> flpGridIndex;
			for(int y = 0; y < flpGridSize.y; ++y)
			{
				flpGridIndex.y = y;

				for(int x = 0; x < flpGridSize.x; ++x)
				{
					flpGridIndex.x = x;

					// Grid Index와 같은 좌표로 Source 좌표를 설정 // Set the source coordinates to the same coordinates as the Grid Index
					CFLPoint<double> flpSource(flpGridIndex.x, flpGridIndex.y);
					// Grid Index와 같은 좌표에서 미세한 랜덤 값을 부여해서 좌표를 왜곡 // Distort the coordinates by giving fine random values at the same coordinates as the Grid Index
					CFLPoint<double> flpDistortion(flpGridIndex.x + CRandomGenerator::Double(-.15, .15), flpGridIndex.y + CRandomGenerator::Double(-.15, .15));

					// 위에서 설정한 좌표들을 바탕으로 ThinPlateSplineMapping 클래스에 하나의 Control Point를 설정
					// Set one Control Point in the ThinPlateSplineMapping class based on the coordinates set above
					tps.AddControlPoint(flpSource, flpDistortion);
				}
			}

			// 설정한 데이터를 매핑 가능하도록 마무리 작업을 진행합니다.
			// 반드시 이 함수를 호출해서 결과가 OK가 나와야 매핑 사용이 가능합니다.
			// We proceed with the finishing work so that the set data can be mapped.
			// You must call this function and the result must be OK to use the mapping.
			if(IsFail(res = tps.Finish()))
			{
				ErrorPrint(res, "Failed to finalize\n");
				break;
			}

			// Finalize 까지 완료된 상태라면 Save를 통해 파일에 저장이 가능하며,
			// 추후 Load함수를 통해 로드 시 위의 Initialize -> Set -> Finalize 과정을 생략할 수 있습니다.
			// If Finalize is completed, it can be saved to a file through Save.
			// When loading through the Load function later, the above Initialize -> Set -> Finalize process can be omitted.
			if(IsFail(res = tps.Save(L"MappingData.fltps")))
			{
				ErrorPrint(res, "Failed to save mapping data\n");
				break;
			}
		}

		// 세팅이 완료된 ThinPlateSplineMapping 클래스를 이용해 변환을 하는 단계입니다.
		// This step is to convert using the ThinPlateSplineMapping class that has been set.

		// ThinPlateSplineMapping 클래스에 설정된 Vertex 정보를 화면에 Display
		// Display the vertex information set in the ThinPlateSplineMapping class on the screen
		for(int64_t i = 0; i < tps.GetControlPointCount(); ++i)
		{
			CFLPoint<double> flpSource;
			CFLPoint<double> flpTarget;

			tps.GetControlPoint(i, &flpSource, &flpTarget);

			for(int32_t i = 0; i < 3; ++i)
			{
				// Target Vertex를 각 View Layer에 Drawing // Drawing the target vertex to each view layer
				if(IsFail(res = layer[i].DrawFigureImage(&flpTarget, BLUE, 3)))
				{
					ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
					break;
				}

				// Source Vertex를 각 View Layer에 Drawing // Drawing source vertex to each view layer
				if(IsFail(res = layer[i].DrawFigureImage(&flpSource, RED, 3)))
				{
					ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
					break;
				}
			}

			printf("Source Vertex : (%.03lf,%.03lf)\n", flpSource.x, flpSource.y);
			printf("Target Vertex : (%.03lf,%.03lf)\n", flpTarget.x, flpTarget.y);
		}


		// 정점 사이를 대략 10등분, 즉 하나의 영역에 100개의 보간영역을 테스트
		// Divide the vertices into approximately 10 equal parts, that is, test 100 interpolation areas in one area
		double f64Slice = 10;

		CFLPoint<double> flpSource; // Source 좌표 // Source coordinates
		CFLPoint<double> flpTarget; // Target 좌표 // Target coordinates
		CFLPoint<double> flpConvertedSource; // Target 좌표를 다시 Source로 변환, 검증 용도의 좌표 // Convert target coordinates back to source, coordinates for verification purposes

		for(int y = 0; y <= (flpGridSize.y - 1) * f64Slice; ++y)
		{
			flpSource.y = (y / f64Slice);

			for(int x = 0; x <= (flpGridSize.x - 1) * f64Slice; ++x)
			{
				flpSource.x = (x / f64Slice);

				// 연산을 수행할 Source 좌표를 View에 Display // Display the source coordinates to perform the operation on the View
				if(IsFail(res = layer[0].DrawFigureImage(&flpSource, YELLOW)))
				{
					ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
					break;
				}

				// Source 좌표의 공간을 Target 좌표 공간으로 변환 // Convert the space of source coordinates to target coordinate space
				if(IsOK(tps.ConvertSourceToTarget(flpSource, flpTarget)))
				{
					// Source 좌표에서 Target 좌표로 변환된 좌표를 View에 Display // Display coordinates converted from source coordinates to target coordinates on the View
					if(IsFail(res = layer[1].DrawFigureImage(&flpTarget, LIME)))
					{
						ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
						break;
					}

					// 변환된 Target 좌표를 그대로 Source 좌표로 변환해서 자신의 위치로 제대로 돌아오는지 검증
					// Verify that the converted target coordinates are converted to source coordinates as they are and return to their own position properly
					if(IsOK(tps.ConvertTargetToSource(flpTarget, flpConvertedSource)))
					{
						printf("Source (%.03lf,%.03lf) -> Target (%.03lf,%.03lf) -> Source (%.03lf,%.03lf)\n", flpSource.x, flpSource.y, flpTarget.x, flpTarget.y, flpConvertedSource.x, flpConvertedSource.y);

						// 변환된 좌표를 View에 Display // Display the converted coordinates in the View
						if(IsFail(res = layer[2].DrawFigureImage(&flpConvertedSource, CYAN)))
						{
							ErrorPrint(res, "Failed to draw figure objects on the image view.\n");
							break;
						}
					}
				}
			}
		}

		// 이미지 뷰들을 갱신 합니다. // Update the image views.
		for(int32_t i = 0; i < 3; ++i)
			viewImage[i].Invalidate(true);


		// 이미지 뷰가 셋중에 하나라도 꺼지면 종료로 간주 // Consider closed when any of the three image views are turned off
		while(viewImage[0].IsAvailable() && viewImage[1].IsAvailable() && viewImage[2].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}