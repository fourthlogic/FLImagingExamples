#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage arrFliImage[4];

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage[4];

    // ���� ��� ��ü ���� // Declare the execution result object
	CResult res = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((res = arrFliImage[0].Load(L"../../ExampleImages/ConditionalReplacer/CatSource.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Operand �̹��� �ε� // Loads the operand image
		if((res = arrFliImage[1].Load(L"../../ExampleImages/ConditionalReplacer/CatOperand.flif")).IsFail())
		{
			ErrorPrint(res, "Failed to load the image file.\n");
			break;
		}

		// Destination1 �̹����� Source �̹����� ������ �̹����� ���� // Create destination1 image as same as source image
		if((res = arrFliImage[2].Assign(arrFliImage[0])).IsFail())
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Destination2 �̹����� Source �̹����� ������ �̹����� ���� // Create destination2 image as same as source image
		if((res = arrFliImage[3].Assign(arrFliImage[0])).IsFail())
		{
			ErrorPrint(res, "Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((res = viewImage[0].Create(100, 0, 548, 448)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Operand �̹��� �� ���� // Creates operand image view
		if((res = viewImage[1].Create(548, 0, 996, 448)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination1 �̹��� �� ���� // Create destination1 image view
		if((res = viewImage[2].Create(996, 0, 1444, 448)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Destination2 �̹��� �� ���� // Create destination2 image view
		if((res = viewImage[3].Create(1444, 0, 1892, 448)).IsFail())
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
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

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImage[0].SynchronizePointOfView(&viewImage[1])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImage[0].SynchronizePointOfView(&viewImage[2])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the two image views
		if((res = viewImage[0].SynchronizePointOfView(&viewImage[3])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = viewImage[0].SynchronizeWindow(&viewImage[1])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = viewImage[0].SynchronizeWindow(&viewImage[2])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((res = viewImage[0].SynchronizeWindow(&viewImage[3])).IsFail())
		{
			ErrorPrint(res, "Failed to synchronize window.\n");
			break;
		}

		// Conditional Replacer ��ü ���� // Create Conditional Replacer object
		CConditionalReplacer conditionalReplacer;
		// Source �̹��� ���� // Set the source image
		conditionalReplacer.SetSourceImage(arrFliImage[0]);
		// Operand �̹��� ���� // Set the operand image
		conditionalReplacer.SetOperandImage(arrFliImage[1]);
		// Destination �̹��� ���� // Set the destination image
		conditionalReplacer.SetDestinationImage(arrFliImage[2]);

		// Operation Source ���� // Set the Operation Source
		conditionalReplacer.SetOperationSource(EOperationSource_Image);

		// Threshold Mode ���� // Set the Threshold Mode
		conditionalReplacer.SetThresholdMode(EThresholdMode_Single);

		// �� ������ �Էµ� MultiVar ��ü ���� // Create the MultiVar object that push the logical condition
		CMultiVarL mvCondition1;

		// �� ���� �Է� // Set the logical condition
		mvCondition1.PushBack(ELogicalCondition_GreaterEqual);

		// �� ���� ���� // Set the logical condition
		conditionalReplacer.SetLogicalCondition(mvCondition1);

		// ä�� �� ���� ���� // Set the logical condition of channels
		conditionalReplacer.SetLogicalConditionOfChannels(ELogicalConditionOfChannels_And);

		// ������ ���� ��� Replacement �� �����ϱ� ���� MultiVar ��ü ���� // Create the MultiVar object that sets the Replacement value if the condition is true
		CMultiVar<double> mvReplacement;

		// 1ä�� Replacement �� �Է� // Push the 1Channel Replacement value
		mvReplacement.PushBack(0.0);

		// Replacement �� ���� // Set Replacement value
		conditionalReplacer.SetReplacementValue(mvReplacement);

		// �տ��� ������ �Ķ���� ��� Conditional Replacer ���� // Execute Conditional Replacer algorithm according to previously set parameters
		if((res = conditionalReplacer.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Conditional Replacer.");
			wprintf(res.GetString());
			break;
		}

		// Destination �̹��� ���� // Set the destination image
		conditionalReplacer.SetDestinationImage(arrFliImage[3]);

		// ä�� �� ���� ���� // Set the logical condition of channels
		conditionalReplacer.SetLogicalConditionOfChannels(ELogicalConditionOfChannels_Or);

		// �տ��� ������ �Ķ���� ��� Conditional Replacer ���� // Execute Conditional Replacer algorithm according to previously set parameters
		if((res = conditionalReplacer.Execute()).IsFail())
		{
			ErrorPrint(res, "Failed to execute Conditional Replacer.");
			wprintf(res.GetString());
			break;
		}

		CGUIViewImageLayerWrap arrLayer[4];

		for(int32_t i = 0; i < 4; ++i)
		{
			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
			arrLayer[i] = viewImage[i].GetLayer(0);

			// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
			arrLayer[i].Clear();
		}

		// �̹��� �� ���� ǥ�� // Display image view information
		// �Ʒ� �Լ� DrawTextCanvas �� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
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

		// �̹��� �並 ���� // Update image view
		viewImage[0].Invalidate(true);
		viewImage[1].Invalidate(true);
		viewImage[2].Invalidate(true);
		viewImage[3].Invalidate(true);

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage[0].IsAvailable()
			  && viewImage[1].IsAvailable()
			  && viewImage[2].IsAvailable()
			  && viewImage[3].IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}