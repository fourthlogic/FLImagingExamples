#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


int main()
{
	// �׷��� �� ���� // Declare the graph view
	CGUIViewGraphWrap viewGraphDark;
	CGUIViewGraphWrap viewGraphLight;

	do
	{
		// ���� ��� // operation result
		CResult res = EResult_UnknownError;

		// Graph �� ���� // Create graph view
		if(IsFail(res = viewGraphDark.Create(100, 0, 100 + 440, 340)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		// Graph �� ���� // Create graph view
		if(IsFail(res = viewGraphLight.Create(100 + 440 * 1, 0, 100 + 440 * 2, 340)))
		{
			ErrorPrint(res, " Failed to create the graph view. \n");
			break;
		}

		// Graph ���� ��ġ ����ȭ // / Synchronize the positions of windows
		if(IsFail(res = viewGraphDark.SynchronizeWindow(&viewGraphLight)))
		{
			ErrorPrint(res, "Failed to synchronize window\n");
			break;
		}

		// �������� 100���� �����͸� �����Ѵ�. // Create 100 random (x,y) coordinates.
		const size_t stDataCount = 100;
		double arrF64DataX[stDataCount] = { 0., };
		double arrF64DataY[stDataCount] = { 0., };

		double f64PrevX = 0.;
		double f64PrevY = 0.;

		for(size_t i = 0; i < stDataCount; ++i)
		{
			arrF64DataX[i] = f64PrevX + ((rand() % 100) / 10.);
			if(rand() % 2)
				arrF64DataY[i] = f64PrevY + ((rand() % 100) / 10.);
			else
				arrF64DataY[i] = f64PrevY - ((rand() % 100) / 10.);

			f64PrevX = arrF64DataX[i];
			f64PrevY = arrF64DataY[i];
		}

		uint32_t ui32Color = COLOR(rand() % 256, rand() % 256, rand() % 256);
		CFLString<wchar_t> flstrName(L"Chart 1");

		// �׷����� ������ �����͸� �߰��Ѵ�. // Plot chart on the graph view.
		viewGraphDark.Plot(arrF64DataX, arrF64DataY, stDataCount, EChartType_Scatter, ui32Color, flstrName, nullptr);
		viewGraphLight.Plot(arrF64DataX, arrF64DataY, stDataCount, EChartType_Scatter, ui32Color, flstrName, nullptr);

		// Graph �� �׸��� ��ũ���� ���� // Sets the theme of the graph view to dark mode.
		viewGraphDark.SetDarkMode();

		// Graph �� �׸��� ����Ʈ���� ���� // Sets the theme of the graph view to light mode.
		viewGraphLight.SetLightMode();

		// Graph ���� �������� ���� // Sets the scales of the graph view.
		viewGraphDark.ZoomFit();
		viewGraphLight.ZoomFit();

		// �׷��� �䰡 ����� �� ���� ��ٸ�
		while(viewGraphDark.IsAvailable() && viewGraphLight.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}