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
		if(IsFail(res = fliImage.Load(L"../../ExampleImages/ShapeMatch/Ellipse Match.flif")))
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
		CShapeMatchEllipse shapeMatch;

		// �н��� Ÿ���� ������ ���� // Set the radius of the ellipse to learn
		shapeMatch.SetRadius(15, 30);
		// �н��� Ÿ���� ���� ���� // Set the angle of the ellipse to learn
		shapeMatch.SetAngle(15);

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

		// �˰��� ���� // Execute the algorithm
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
			CShapeMatchEllipseResult matchResult;

			shapeMatch.GetResult(i, matchResult);

			// ���� ���� ����� Consoleâ�� ����մϴ�. // Output the shape detection result to the console window.
			printf(" < Instance : %lld >\n", i);
			printf("  1. Shape Type : Ellipse\n");
			printf("    Pivot X: %.3lf\n", matchResult.flpPivot.x);
			printf("    Pivot Y: %.3lf\n", matchResult.flpPivot.y);
			printf("    Radius 1    : %.3lf\n", matchResult.fleResultObject.radius1);
			printf("    Radius 2    : %.3lf\n", matchResult.fleResultObject.radius2);
			printf("    Angle    : %.3lf\n", matchResult.fleResultObject.angle);
			printf("  2. Score : %.3lf\n  3. Angle : %.3lf\n  4. Scale : %.3lf\n\n", matchResult.f32Score, matchResult.f32Angle, matchResult.f32Scale);

			if(IsFail(res = layer.DrawFigureImage(matchResult.fleResultObject, CYAN, 3)))
			{
				ErrorPrint(res, "Failed to draw figure\n");
				break;
			}

			if(IsFail(res = layer.DrawTextImage(matchResult.flpPivot, CFLString<wchar_t>().Format(L"Score : %.3lf\nAngle : %.3lf\nScale : %.3lf\nPivot : (%.2lf, %.2lf)", matchResult.f32Score, matchResult.f32Angle, matchResult.f32Scale, matchResult.flpPivot.x, matchResult.flpPivot.y), YELLOW, BLACK, 15)))
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