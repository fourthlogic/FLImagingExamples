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
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[ETypeCount];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap arrViewImage[ETypeCount];

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult res;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if(IsFail(res = arrFliImage[EType_Source].Load(L"../../ExampleImages/HoleFilling/TodoList.flif")))
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹����� Source �̹����� ������ �̹����� ���� // Create destination image as same as source image
		if(IsFail(res = arrFliImage[EType_Destination].Assign(arrFliImage[EType_Source])))
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if(IsFail(res = arrViewImage[EType_Source].Create(100, 0, 612, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if(IsFail(res = arrViewImage[EType_Destination].Create(1124, 0, 1636, 512)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
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

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if(IsFail(res = arrViewImage[EType_Source].SynchronizePointOfView(&arrViewImage[EType_Destination])))
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ����ȭ �Ѵ� // Synchronize the positions of the two image view windows
		if(IsFail(res = arrViewImage[EType_Source].SynchronizeWindow(&arrViewImage[EType_Destination])))
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �˰��� ��ü ���� // Create Algorithm object
		CHoleFilling alg;
		// Source �̹��� ���� // Set the source image
		if((res = alg.SetSourceImage(arrFliImage[EType_Source])).IsFail())
			break;
		// Destination �̹��� ���� // Set the destination image
		if((res = alg.SetDestinationImage(arrFliImage[EType_Destination])).IsFail())
			break;
		// ó���� Hole Area ���� ���� ���� // Set hole area range to process
		if((res = alg.SetMinimumHoleArea(10)).IsFail())
			break;
		// ó���� Hole Area ���� ���� ���� // Set hole area range to process
		if((res = alg.SetMaximumHoleArea(99999999999)).IsFail())
			break;
		// �̹��� ���� �´��� hole �� ó�� ���� ���� // Set whether to process holes that touch the image boundary
		if((res = alg.EnableIgnoreBoundaryHole(true)).IsFail())
			break;
		// Threshold �� ����� �ȼ��� hole ���� object ���� ���� // Set whether the pixel that passed the threshold is a hole or an object
		if((res = alg.SetThresholdPassTarget(CHoleFilling::EThresholdPassTarget_Object)).IsFail())
			break;
		// Threshold ���� ���� ����� �ǹ��ϴ� Threshold ��� ���� // Threshold mode setting, which refers to the number of threshold and combination method
		if((res = alg.SetThresholdMode(EThresholdMode_Dual_And)).IsFail())
			break;
		// �� Threshold ������ ä�� �� �� ��� ���� ���� ����� �ǹ��ϴ� Logical Condition Of Channels ���� // Set the Logical Condition Of Channels, which refers to the combination method between logical results for each channel within each Threshold
		if((res = alg.SetLogicalConditionOfChannels(ELogicalConditionOfChannels_And)).IsFail())
			break;
		// Hole ������ ä��� ����� ���� // Set the method of filling the hole area
		if((res = alg.SetFillingMethod((CHoleFilling::EFillingMethod)0)).IsFail())
			break;
		// Harmonic Interpolation �� Precision �� ���� // Set precision value for Harmonic Interpolation
		if((res = alg.SetPrecision(0.1)).IsFail())
			break;
		// Harmonic Interpolation �� Max Iteration �� ���� // Set max iteration value for Harmonic Interpolation
		if((res = alg.SetMaxIteration(100)).IsFail())
			break;
		// ù ��° Threshold �� ä�� �� �� �����ڿ� �� ���� // Set the logical operator and value for each channel of the first Threshold
		CMultiVar<uint64_t> mvThresholdCondition1 = CMultiVar<uint64_t>((uint64_t)ELogicalCondition_GreaterEqual, (uint64_t)ELogicalCondition_GreaterEqual, (uint64_t)ELogicalCondition_GreaterEqual);
		if((res = alg.SetThresholdCondition(EThresholdIndex_First, mvThresholdCondition1)).IsFail())
			break;
		CMultiVar<uint64_t> mvThresholdValue1U64 = CMultiVar<uint64_t>(175, 230, 240);
		if((res = alg.SetThresholdValue(EThresholdIndex_First, mvThresholdValue1U64)).IsFail())
			break;
		// �� ��° Threshold �� ä�� �� �� �����ڿ� �� ���� // Set the logical operator and value for each channel of the second Threshold
		CMultiVar<uint64_t> mvThresholdCondition2 = CMultiVar<uint64_t>((uint64_t)ELogicalCondition_Less, (uint64_t)ELogicalCondition_Less, (uint64_t)ELogicalCondition_Less);
		if((res = alg.SetThresholdCondition(EThresholdIndex_Second, mvThresholdCondition2)).IsFail())
			break;
		CMultiVar<uint64_t> mvThresholdValue2U64 = CMultiVar<uint64_t>(200, 240, 255);
		if((res = alg.SetThresholdValue(EThresholdIndex_Second, mvThresholdValue2U64)).IsFail())
			break;

		// �˰��� ���� // Execute Algorithm
		if(IsFail(res = alg.Execute()))
		{
			ErrorPrint(res, "Failed to execute HoleFilling.");
			break;
		}

		CGUIViewImageLayerWrap arrLayer[ETypeCount];

		for(int32_t i = 0; i < ETypeCount; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = arrViewImage[i].GetLayer(0);

			// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
			arrLayer[i].Clear();
		}

		const CFLFigure* pFlfHoleContours = alg.GetSelectedPageFigureObject();

		if(IsFail(res = arrLayer[EType_Source].DrawFigureImage(pFlfHoleContours, CYAN)))
		{
			ErrorPrint(res, "Failed to draw figure\n");
			break;
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
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

		// �̹��� �並 ���� // Update image view
		arrViewImage[EType_Source].Invalidate(true);
		arrViewImage[EType_Destination].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(arrViewImage[EType_Source].IsAvailable()
			&& arrViewImage[EType_Destination].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}