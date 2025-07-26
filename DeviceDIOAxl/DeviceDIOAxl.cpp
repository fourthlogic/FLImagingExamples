#include <cstdio>

#include <FLImaging.h>
#include "../CommonHeader/ErrorPrint.h"

int main()
{
	// You must call the following function once
	// before using any features of the FLImaging(R) library
	CLibraryUtilities::Initialize();

	CResult res;

	// Axl DIO 장치를 선언 // Declare Axl DIO device
	CDeviceDIOAxl devDIO;

	do
	{
		wchar_t arrInput[4096] = { 0, };
		CFLString<wchar_t> flsInput = L"";

		// DIO 장치를 초기화 합니다. // Initialize the DIO device.
		if((res = devDIO.Initialize()).IsFail())
		{
 			ErrorPrint(res, L"Failed to initialize the device.");
 			break;
		}

		while(true)
		{
			// 사용할 기능을 선택합니다. // Select the features you want to use.
			printf("1. Read input\n");
			printf("2. Read output\n");
			printf("3. Write input\n");
			printf("4. Write output\n");
			printf("Select: ");
			fgetws(arrInput, 4096, stdin);

			flsInput = arrInput;
			int32_t i32Select = flsInput.ToInt32();

			switch(i32Select)
			{
			case 1:
			case 2:
				{
					// Bit 를 입력 받습니다. // Enter Bit.
					printf("Bit input: ");
					fgetws(arrInput, 4096, stdin);

					flsInput = arrInput;
					int32_t i32Bit = flsInput.ToInt32();

					bool bReadStatus = false;

					// Bit 의 상태를 읽습니다. // Read Bit status.
					if(i32Select == 1)
						bReadStatus = devDIO.ReadInBit(i32Bit);
					else 
						bReadStatus = devDIO.ReadOutBit(i32Bit);

					printf("Read status: %d\n\n", bReadStatus);
				}
				break;

			case 3:
			case 4:
				{
					// Bit 를 입력 받습니다. // Enter Bit.
					printf("Bit input: ");
					fgetws(arrInput, 4096, stdin);

					flsInput = arrInput;
					int32_t i32Bit = flsInput.ToInt32();

					// 상태를 입력 받습니다. // Enter status.
					printf("Status input: ");
					fgetws(arrInput, 4096, stdin);
					
					bool bWriteStatus = false;

					flsInput = arrInput;
						
					if(flsInput.ToInt32())
						bWriteStatus = true;

					// Bit 에 상태를 기록합니다. // Write the status in Bit.
					if(i32Select == 3)
						res = devDIO.WriteInBit(i32Bit, bWriteStatus);
					else
						res = devDIO.WriteOutBit(i32Bit, bWriteStatus);

					if(res.IsOK())
						printf("Succeeded to write.\n\n");
					else
						printf("Failed to write.\n\n");
				}
				break;

			default:
				printf("Incorrect input. Please select again.\n\n");
				break;
			}
		}
	}
	while(false);

	// DIO 장치의 초기화를 해제합니다. // Terminate the DIO device.
	devDIO.Terminate();

	if(res.IsFail())
		getchar();

	return 0;
}