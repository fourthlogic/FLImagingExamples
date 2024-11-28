#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"



int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	bool bError = false;

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult res;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/ShapeMatch/Circle Match.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage.Create(400, 0, 912, 384)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			bError = true;
			break;
		}

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		if(IsFail(res = viewImage.SetImagePtr(&fliImage)))
		{
			ErrorPrint(res, "Failed to set image object on the image view.\n");
			bError = true;
			break;
		}

		if(bError)
			break;

		// Shape Match ��ü ���� // Create Shape Match object
		CShapeMatchCircle shapeMatch;

		// �н��� ���� ������ ���� // Set the radius of the circle to learn
		shapeMatch.SetRadius(30);

		// ���� �н� // Learn shape
		if(IsFail(res = shapeMatch.Learn()))
		{
			ErrorPrint(res, "Failed to Learn.\n");
			break;
		}

		// ������ �̹��� ���� // Set image to detect
		shapeMatch.SetSourceImage(fliImage);
		// ���� �� ���� ��ȿ ���� ũ������� �����մϴ�. // Set the effective change size range to be used for detection.
		shapeMatch.SetScaleRatio(0.9, 1.1);
		// ������ ��ü�� ������ �����մϴ�. // Sets the color of the object to be detected.
		shapeMatch.SetObjectColor(EShapeMatchObjectColor_Bright);

		// �˰����� ���� // Execute the algorithm
		if(IsFail(res = shapeMatch.Execute()))
		{
			ErrorPrint(res, "Failed to execute.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		int64_t i64ResultCount = shapeMatch.GetResultCount();

		for(int64_t i = 0; i < i64ResultCount; ++i)
		{
			CShapeMatchCircleResult matchResult;

			shapeMatch.GetResult(i, matchResult);

			// ���� ���� ����� Consoleâ�� ����մϴ�. // Output the shape detection result to the console window.
			printf(" < Instance : %lld >\n", i);
			printf("  1. Shape Type : Circle\n");
			printf("    Center X: %.3lf\n", matchResult.flcResultObject.flpCenter.x);
			printf("    Center Y: %.3lf\n", matchResult.flcResultObject.flpCenter.y);
			printf("    Radius    : %.3lf\n", matchResult.flcResultObject.radius);
			printf("  2. Score : %.3lf\n  3. Scale : %.3lf\n\n", matchResult.f32Score, matchResult.f32Scale);

			if(IsFail(res = layer.DrawFigureImage(matchResult.flcResultObject, CYAN, 3)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsFail(res = layer.DrawTextImage(matchResult.flcResultObject.GetCenter(), CFLString<wchar_t>().Format(L"Score : %.3lf\nScale : %.3lf", matchResult.f32Score, matchResult.f32Scale), YELLOW, BLACK, 15)))
			{
				ErrorPrint(res, "Failed to draw text\n");
				break;
			}
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		viewImage.Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);		
	}
	while(false);

	return 0;
}