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
	CResult eResult = EResult_UnknownError;

	do
	{
		// Source �̹��� �ε� // Load the source image
		if((eResult = arrFliImage[0].Load(L"../../ExampleImages/ConditionalExtractor/CatSource.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Operand1 �̹��� �ε� // Loads the operand1 image
		if((eResult = arrFliImage[1].Load(L"../../ExampleImages/ConditionalExtractor/CatOperandDual1.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Operand2 �̹��� �ε� // Loads the operand2 image
		if((eResult = arrFliImage[2].Load(L"../../ExampleImages/ConditionalExtractor/CatOperandDual2.flif")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the image file.\n");
			break;
		}

		// Destination �̹����� Source �̹����� ������ �̹����� ���� // Create destination image as same as source image
		if((eResult = arrFliImage[3].Assign(arrFliImage[0])).IsFail())
		{
			ErrorPrint(eResult, "Failed to assign the image file.\n");
			break;
		}

		// Source �̹��� �� ���� // Create Source image view
		if((eResult = viewImage[0].Create(100, 0, 500, 400)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Operand1 �̹��� �� ���� // Creates operand1 image view
		if((eResult = viewImage[1].Create(500, 0, 900, 400)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Operand2 �̹��� �� ���� // Creates operand2 image view
		if((eResult = viewImage[2].Create(900, 0, 1300, 400)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// Destination �̹��� �� ���� // Create destination image view
		if((eResult = viewImage[3].Create(1300, 0, 1700, 400)).IsFail())
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		bool bError = false;

		// �̹��� �信 �̹����� ���÷��� // Display an image in an image view
		for(int32_t i = 0; i < 4; ++i)
		{
			if((eResult = viewImage[i].SetImagePtr(&arrFliImage[i])).IsFail())
			{
				ErrorPrint(eResult, "Failed to set image object on the image view.\n");
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		// Source �̹��� ��� Operand1 �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the source view and the operand1 view
		if((eResult = viewImage[0].SynchronizePointOfView(&viewImage[1])).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� ��� Operand2 �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the source view and the operand2 view
		if((eResult = viewImage[0].SynchronizePointOfView(&viewImage[2])).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// Source �̹��� ��� Destination �̹��� ���� ������ ����ȭ �Ѵ� // Synchronize the viewpoints of the source view and the destination view
		if((eResult = viewImage[0].SynchronizePointOfView(&viewImage[3])).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize view\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((eResult = viewImage[0].SynchronizeWindow(&viewImage[1])).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((eResult = viewImage[0].SynchronizeWindow(&viewImage[2])).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// �� �̹��� �� �������� ��ġ�� ���� // Synchronize the positions of the two image view windows
		if((eResult = viewImage[0].SynchronizeWindow(&viewImage[3])).IsFail())
		{
			ErrorPrint(eResult, "Failed to synchronize window.\n");
			break;
		}

		// Conditional Extractor ��ü ���� // Create Conditional Extractor object
		CConditionalExtractor conditionalExtractor;
		// Source �̹��� ���� // Set the source image
		conditionalExtractor.SetSourceImage(arrFliImage[0]);
		// Operand1 �̹��� ���� // Set the operand1 image
		conditionalExtractor.SetOperandImage(arrFliImage[1]);
		// Operand2 �̹��� ���� // Set the operand2 image
		conditionalExtractor.SetOperandImage2(arrFliImage[2]);
		// Destination �̹��� ���� // Set the destination image
		conditionalExtractor.SetDestinationImage(arrFliImage[3]);

		// Operation Source ���� // Set the Operation Source
		conditionalExtractor.SetOperationSource(EOperationSource_Image);

		// Threshold Mode ���� // Set the Threshold Mode
		conditionalExtractor.SetThresholdMode(EThresholdMode_Dual_Or);

		// �� ������ �Էµ� MultiVar ��ü ���� // Create the MultiVar object that push the logical condition
		CMultiVarL mvCondition1, mvCondition2;

		// �� ���� �Է� // Push the logical condition
		mvCondition1.PushBack(ELogicalCondition_Greater);
		mvCondition2.PushBack(ELogicalCondition_Equal);

		// �� ���� ���� // Set the logical condition
		conditionalExtractor.SetLogicalCondition(mvCondition1);
		conditionalExtractor.SetLogicalCondition(mvCondition2, EThresholdIndex_Second);

		// ������ ������ ��� Out of Range �� ���� ���� ���� // Determine the Out of Range value if the condition is false
		conditionalExtractor.EnableOutOfRange(true);

		// ������ ������ ��� Out of range �� �����ϱ� ���� MultiVar ��ü ���� // Create the MultiVar object that sets the Out of Range value if the condition is false
		CMultiVar<double> mvOutOfRange;

		// Out of Range �� �Է� // Push the out of range value
		mvOutOfRange = CMultiVar<double>(0., 0., 0.);

		// Out of Range �� ���� // Set Out of Range value
		conditionalExtractor.SetOutOfRangeValue(mvOutOfRange);

		// �տ��� ������ �Ķ���� ��� Conditional Extractor ���� // Execute Conditional Extractor algorithm according to previously set parameters
		if((eResult = conditionalExtractor.Execute()).IsFail())
		{
			ErrorPrint(eResult, "Failed to execute Conditional Extractor.");
			wprintf(eResult.GetString());
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
		// �Ʒ� �Լ� DrawTextCanvas�� Screen��ǥ�� �������� �ϴ� String�� Drawing �Ѵ�. // The function DrawTextCanvas below draws a String based on the screen coordinates.
		// ���� �Ķ���͸� EGUIViewImageLayerTransparencyColor ���� �־��ְԵǸ� �������� ó�������� �������� 0���� �ѰͰ� ���� ȿ���� �ִ�. // If the color parameter is added as EGUIViewImageLayerTransparencyColor, it has the same effect as setting the opacity to 0 by processing it as a background color.
		// �Ķ���� ���� : ���̾� -> ���� ��ǥ Figure ��ü -> ���ڿ� -> ��Ʈ �� -> �� �� -> ��Ʈ ũ�� -> ���� ũ�� ���� -> ���� ->
		//                 ����� -> ��Ʈ �̸� -> ��Ʈ ���İ�(������) -> �� ���İ� (������) -> ��Ʈ �β� -> ��Ʈ ���ڸ�
		// Parameter order: layer -> reference coordinate Figure object -> string -> font color -> Area color -> font size -> actual size -> angle ->
		//                  Align -> Font Name -> Font Alpha Value (Opaqueness) -> Cotton Alpha Value (Opaqueness) -> Font Thickness -> Font Italic
		if((eResult = arrLayer[0].DrawTextCanvas(&CFLPointD(0, 0), L"Source Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if((eResult = arrLayer[1].DrawTextCanvas(&CFLPointD(0, 0), L"Operand1 Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if((eResult = arrLayer[2].DrawTextCanvas(&CFLPointD(0, 0), L"Operand2 Image", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
			break;
		}

		if((eResult = arrLayer[3].DrawTextCanvas(&CFLPointD(0, 0), L"Destination Image\nDual Or(Greater, Equal)", YELLOW, BLACK, 30)).IsFail())
		{
			ErrorPrint(eResult, "Failed to draw text\n");
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