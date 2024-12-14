#include <cstdio>

#include <FLImaging.h>
#include <Libraries/Includes/FLImagingGUI/GUIViewSNAPWrap.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	do 
	{
		// 스냅 빌드 객체 선언 // Declare SNAP Build
		CGUIViewSNAPWrap viewSNAP;

		// 스냅 뷰 생성 // Create SNAP view
		if((res = viewSNAP.Create(0, 0, 600, 600)).IsFail())
		{
			ErrorPrint(res, "Failed to create the SNAP view.\n");
			break;
		}

		// 스냅 파일 로드 // Load SNAP file
		if((res = viewSNAP.Load(L"C:\\Users\\Public\\Documents\\FLImaging\\ExamplesSNAP\\Advanced Functions\\Object\\Blob.flsf")).IsFail())
		{
			ErrorPrint(res, "Failed to load the SNAP file.\n");
			break;
		}

		// 스냅 실행 // Run SNAP
		res = viewSNAP.Run();

		// 스냅이 종료될 때 까지 기다림 // Wait for the SNAP to close
		while(viewSNAP.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}