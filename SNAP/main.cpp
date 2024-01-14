#include <cstdio>

#include <FLImaging.h>
#include <Libraries/Includes/FLImagingGUI/SNAPBuild.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// ���� ��� ��ü ���� // Declare the execution result object
	CResult eResult;

	do 
	{
		// ���� ���� ��ü ���� // Declare SNAP Build
		CSNAPBuild snapBuild;

		// ���� ���� �ε� // Load SNAP file
		if((eResult = snapBuild.Load(L"C:\\Users\\Public\\Documents\\FLImaging\\ExamplesSNAP\\Advanced Functions\\Object\\Blob.flsf")).IsFail())
		{
			ErrorPrint(eResult, "Failed to load the SNAP file.\n");
			break;
		}

		// ���� ���� // Run SNAP
		eResult = snapBuild.Run();

		// ������ ����� �� ���� ��ٸ� // Wait for the SNAP to close
		while(snapBuild.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}