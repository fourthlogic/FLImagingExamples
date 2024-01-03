#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� ��ü ���� // Declare image object
	CFLImage fliImage;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	// ���� ��� ��ü ���� // Declare the execution result object
	CResult eResult;

	do
	{
		// Image View ���� // Create image view
		if(IsFail(eResult = viewImage.Create(200, 0, 800, 500)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIViewImageLayerWrap layer = viewImage.GetLayer(0);

		// ���ٽ� Ŭ���� ���� // Declare CStencil class instance. 
		CStencil stencil;

		// �� ���� ���� // Set the line spacing.
		if(IsFail(eResult = stencil.SetLineSpacing(0.2f)))
		{
			ErrorPrint(eResult, "Failed to set line spacing.\n");
			break;
		}

		// �ڰ� ���� // Set the letter spacing.
		if(IsFail(eResult = stencil.SetLetterSpacing(0.3f)))
		{
			ErrorPrint(eResult, "Failed to set letter spacing.\n");
			break;
		}

		// ��Ʈ ũ�� ���� // Set the font size.
		if(IsFail(eResult = stencil.SetFontSize(24)))
		{
			ErrorPrint(eResult, "Failed to set font size.\n");
			break;
		}

		// Arial ��Ʈ�� �̸� �ε� // Load font "Arial"
		if(IsFail(eResult = stencil.LoadFont("Arial")))
		{
			ErrorPrint(eResult, "Failed to load font : Arial.\n");
			break;
		}
		
		// Cambria ��Ʈ�� �̸� �ε� // Load font "Cambria"
		if(IsFail(eResult = stencil.LoadFont("Cambria")))
		{
			ErrorPrint(eResult, "Failed to load font : Cambria.\n");
			break;
 		}
		
		// �̸� �ε��� Arial ��Ʈ ���� // Select preloaded font "Arial"
		if(IsFail(eResult = stencil.SelectFont("Arial")))
		{
			ErrorPrint(eResult, "Failed to select font : Arial.\n");
			break;
 		}

		// ��Ʈ �̸� ���ڿ� ���� // Declare string to get font name.
		CFLString<wchar_t> strFontName = "";

		// ������ ��Ʈ�� �̸� ��� ���� // Get selected font name.
		if(IsFail(eResult = stencil.GetSelectedFontName(strFontName)))
		{
			ErrorPrint(eResult, "Failed to get selected font name.\n");
			break;
 		}

		// ���� ���� // Declare CFLFigureArray instance.
		CFLFigureArray flfaRes;

		// ���ڿ� ���� // Declare the string to convert to figure.
		CFLString<wchar_t> strText = "[Arial]\nFourthLogic CStencil class...";

		// ������ ��Ʈ�� ���ڿ��� �������� ��ȯ // Convert the text to figure
		if(IsFail(eResult = stencil.ConvertStringToFigure(strText, &flfaRes)))
		{
			ErrorPrint(eResult, "Failed to convert string to figure.\n");
			break;
		}

		// ���̾ ������ �׸��� // Draw the figure on a layer.
		layer.DrawFigureImage(flfaRes, BLACK, 1, YELLOW);
		
		// �̸� �ε��� Cambria ��Ʈ ����
		if(IsFail(eResult = stencil.SelectFont("Cambria")))
		{
			ErrorPrint(eResult, "Failed to select font : Cambria.\n");
			break;
 		}

		// �� ���� ���� // Set the line spacing.
		if(IsFail(eResult = stencil.SetLineSpacing(1.0)))
		{
			ErrorPrint(eResult, "Failed to set line spacing.\n");
			break;
		}

		// �ڰ� ���� // Set the letter spacing.
		if(IsFail(eResult = stencil.SetLetterSpacing(0)))
		{
			ErrorPrint(eResult, "Failed to set letter spacing.\n");
			break;
		}

		// ���ڿ� ���� ���� // Set the text to convert to figure.
		strText = "[Cambria]\nFourthLogic CStencil class...";
		
		// ������ ��Ʈ�� ���ڿ��� �������� ��ȯ // Convert the string to figure.
		if(IsFail(eResult = stencil.ConvertStringToFigure(strText, &flfaRes)))
		{
			ErrorPrint(eResult, "Failed to convert string to figure.\n");
			break;
		}

		// ��� �� ������ ��ġ�� ���� // Offset the position of the converted figure.
		flfaRes.Offset((double)0, (double)stencil.GetFontSize() * 5);

		// ���̾ ������ �׸��� // Draw the figure on a layer.
		layer.DrawFigureImage(flfaRes, BLACK, 1, CYAN);

		// �̹��� �並 ���� �մϴ�.
		viewImage.Invalidate(true);

		// �̹��� �䰡 ������ ����� ����
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}