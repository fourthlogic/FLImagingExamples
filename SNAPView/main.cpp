#include <cstdio>

#include <FLImaging.h>
#include <Libraries/Includes/FLImagingGUI/GUIViewSNAPWrap.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// ���� ��� ��ü ���� // Declare the execution result object
	CResult res;

	do 
	{
		// ���� ���� ��ü ���� // Declare SNAP Build
		CGUIViewSNAPWrap viewSNAP;

		// ���� �� ���� // Create SNAP view
		if((res = viewSNAP.Create(0, 0, 600, 600)).IsFail())
		{
			ErrorPrint(res, "Failed to create the SNAP view.\n");
			break;
		}

		// ���� ���� �ε� // Load SNAP file
		if((res = viewSNAP.Load(L"C:\\Users\\Public\\Documents\\FLImaging\\ExamplesSNAP\\Advanced Functions\\Object\\Blob.flsf")).IsFail())
		{
			ErrorPrint(res, "Failed to load the SNAP file.\n");
			break;
		}

		// ���� ���� // Run SNAP
		res = viewSNAP.Run();

		// ������ ����� �� ���� ��ٸ� // Wait for the SNAP to close
		while(viewSNAP.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}