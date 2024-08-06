#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare the image object
	CFLImage fliLearnImage;
	CFLImage fliFindImage;

	// �̹��� �� ���� // Declare the image view
	CGUIViewImageWrap viewImageLearn;
	CGUIViewImageWrap viewImageFind;

	CResult res = EResult_UnknownError;

	do
	{
		// �̹��� �ε� // Loads image
		if(IsFail(res = fliLearnImage.Load(L"../../ExampleImages/Matching/Rectangle Array_0.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		if(IsFail(res = fliFindImage.Load(L"../../ExampleImages/Matching/Rectangle Array_1.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Creates imageview
		if(IsFail(res = viewImageLearn.Create(400, 0, 912, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		if(IsFail(res = viewImageFind.Create(912, 0, 1680, 576)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display the image in the imageview
		if(IsFail(res = viewImageLearn.SetImagePtr(&fliLearnImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		if(IsFail(res = viewImageFind.SetImagePtr(&fliFindImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = viewImageLearn.SynchronizeWindow(&viewImageFind)))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		CGUIViewImageLayerWrap layerLearn = viewImageLearn.GetLayer(0);
		CGUIViewImageLayerWrap layerFind = viewImageFind.GetLayer(1);

		layerLearn.Clear();
		layerFind.Clear();

		if(IsFail(res = layerLearn.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Measurement Array", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		if(IsFail(res = layerFind.DrawTextCanvas(&CFLPoint<double>(0, 0), L"FIND", YELLOW, BLACK, 30)))
		{
			ErrorPrint(res, "Failed to draw text\n");
			break;
		}

		// Rectangle Array Match ��ü ���� // Create a Rectangle Array Match object
		CRectangleArrayMatch arrayMatch;

		// �н��� ������ �����մϴ�. // Set the area to learn.
		CFLFigureArray flfaMeasurement;
		CFLRect<double> flrRect00(587.479194, 364.452004, 929.550836, 616.575019);
		CFLRect<double> flrRect01(583.464651, 1215.493013, 924.560595, 1467.566788);
		CFLRect<double> flrRect02(1531.503352, 655.504324, 1872.516362, 908.626989);
		CFLRect<double> flrRect03(1241.471070, 1222.460787, 1580.517129, 1474.488487);

		flfaMeasurement.PushBack(flrRect00);
		flfaMeasurement.PushBack(flrRect01);
		flfaMeasurement.PushBack(flrRect02);
		flfaMeasurement.PushBack(flrRect03);

		CFLPoint<double> flpCameraPivot(0., 0.);

		// ���� �� ���� �Ķ���͸� �����մϴ�. // Set the parameters to be used for detection.
		// Ž���� �̹����� �����մϴ�. // Set the image to browse.
		arrayMatch.SetSourceImage(fliFindImage);
		// ���� �迭�� �����մϴ�. // Set up the measurement array.
		arrayMatch.SetArray(flfaMeasurement); 
		// Ž�� ��, MeasurementArray�� �⺻ ������ �����մϴ�. // On navigation, set the default angle of the MeasurementArray.
		arrayMatch.SetBaseAngle(0.);
		// �߽��� �ʱⰪ�� �̹��� �߽����� ���� �����մϴ�. // Set whether the initial value of the center is the center of the image.
		arrayMatch.EnablePivotImageCenter(true);
		// �߽� �������� �����մϴ�. // Set the center offset.
		arrayMatch.SetPivotOffset(flpCameraPivot);
		// �ּ� ���ھ� ������ �����մϴ�. // Set the minimum score score.
		arrayMatch.SetMinScore(0.5);
		// Ž�� ��, ���� Ž�� ������ �����մϴ�. // When searching, set the angle search range.
		arrayMatch.SetObjectAngleTolerance(180);
		// Ž�� ��, Fitting Enable/Disable�� �����մϴ�. // When searching, set Fitting Enable/Disable.
		arrayMatch.SetFitting();
		// Ž�� ��, ��� �̵��� ������ �����մϴ�. // When searching, set the allowable movement range.
		arrayMatch.SetAllowingObjectDistanceError();
		


		// �迭 ���� ������ ������� �˱� ���� ���÷��� �Ѵ� // Display to see where the array measurement area is
		if((res = layerLearn.DrawFigureImage(flfaMeasurement, BLUE, 3, BLUE, GUI::EGUIViewImagePenStyle_Solid, 0.25, 0.25)).IsFail())
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		// �˰��� ���� // Execute the algorithm
		if(IsFail(res = arrayMatch.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}


		// ���� ��� �迭�� ������ �����ɴϴ�. // Get the number of detection result arrays.
		int64_t i64ResultCount = arrayMatch.GetResultCount();
		double f64Score, f64Angle;
		// ���� ��� �迭�� ������ �����ɴϴ�. // Get the score of the detection result array.
		arrayMatch.GetResultForArrayScore(f64Score);
		// ���� ��� �迭�� ������ �����ɴϴ�. // Get the angle of the detection result array.
		arrayMatch.GetResultForArrayAngle(f64Angle);

		for(int32_t i = 0; i < i64ResultCount; ++i)
		{
			CRectangleArrayMatch::SResult sResult;

			// ���� ��� �� �迭 �ϳ��� �����ɴϴ�. // Get an array of detection results.
			arrayMatch.GetResult(i, sResult);
			CFLPoint<double> flpRegionCenter = sResult.flrMeasuredRegion.GetCenter();
			CFLString<wchar_t> strDisplayResult;
			strDisplayResult.Format(L"Array Element ID : %lld\n Score : %.3lf\n Angle : %.3lf", sResult.i64Index, sResult.f64Score, sResult.f64Angle);

			// ���� ��� �� �迭 �ϳ��� ����� ���÷��� �մϴ�. // Display the result of one array among the detection results.
			if((res = layerFind.DrawFigureImage(sResult.flrMeasuredRegion, BLACK, 3)).IsFail())
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			// ���� ��� �� �迭 �ϳ��� ����� ���÷��� �մϴ�. // Display the result of one array among the detection results.
			if((res = layerFind.DrawFigureImage(sResult.flrMeasuredRegion, CYAN, 1)).IsFail())
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			// ���� ��� �� �迭 �ϳ��� ����� �߽����� ���÷��� �մϴ�. // Display the center point of the result of one of the detection results.
			if((res = layerFind.DrawTextImage(flpRegionCenter, strDisplayResult, YELLOW, BLACK, 11)).IsFail())
			{
				ErrorPrint(res, "Failed to draw text\n");
				break;
			}

			CFLString<wchar_t> strDisplayResultArray;

			if(i == 0)
			{
				strDisplayResultArray.Format(L"Array Score : % .3lf  Array Angle : % .3lf", f64Score, f64Angle);
				wprintf(L"%s\n", strDisplayResultArray.GetString());

				strDisplayResult.Format(L"Array Score : % .3lf\n Array Angle : % .3lf", f64Score, f64Angle);

				CFLQuad<double> flqRegion(sResult.flrMeasuredRegion);

				double f64MaxY = INT_MAX, f64MaxX = INT_MAX;

				for(int32_t i32Search = 0; i32Search < 4; ++i32Search)
				{
					if(f64MaxY > flqRegion.flpPoints[i32Search].y)
						f64MaxY = flqRegion.flpPoints[i32Search].y;
				}

				for(int32_t i32Search = 0; i32Search < 4; ++i32Search)
				{
					if(f64MaxY == flqRegion.flpPoints[i32Search].y)
					{
						if(f64MaxX > flqRegion.flpPoints[i32Search].x)
							f64MaxX = flqRegion.flpPoints[i32Search].x;
					}
				}

				CFLPoint<double> flpArrayResult(f64MaxX, f64MaxY - 10);

			    // ���� ��� �� �迭 �ϳ��� ����� ������ ���÷��� �մϴ�. // Display the information of the result of one of the detection results.
				if((res = layerFind.DrawTextImage(flpArrayResult, strDisplayResult, GOLD, BLACK, 14)).IsFail())
				{
					ErrorPrint(res, "Failed to draw text\n");
					break;
				}
			}

			CFLString<wchar_t> strDisplayResultElement;
			strDisplayResultElement.Format(L"Array Element ID : %lld Score : %.3lf Angle : %.3lf", sResult.i64Index, sResult.f64Score, sResult.f64Angle);
			wprintf(L" - %s\n", strDisplayResultElement.GetString());
		}
				
		// �̹��� �並 ���� �մϴ�. // Update the image view.
		viewImageLearn.Invalidate(true);
		viewImageFind.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImageLearn.IsAvailable() && viewImageFind.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}