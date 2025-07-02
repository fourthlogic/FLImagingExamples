﻿#include <cstdio>

#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h"

int main()
{
	// 수행 결과 객체 선언 // Declare the execution result object
	CResult res;

	do 
	{
		// 스냅 빌드 객체 선언 // Declare SNAP Build
		CSNAPBuild snapBuild;

		// 스냅 파일 로드 // Load SNAP file
		if((res = snapBuild.Load(L"C:\\Users\\Public\\Documents\\FLImaging\\ExamplesSNAP\\Advanced Functions\\Object\\Blob.flsf")).IsFail())
		{
			ErrorPrint(res, "Failed to load the SNAP file.\n");
			break;
		}

		// 스냅 실행 // Run SNAP
		res = snapBuild.Run();

		// 스냅이 종료될 때 까지 기다림 // Wait for the SNAP to close
		while(snapBuild.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}