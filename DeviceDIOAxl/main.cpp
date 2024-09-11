#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	CResult res;

	// Axl DIO ��ġ�� ���� // Declare Axl DIO device
	CDeviceDIOAxl devDIO;

	do
	{
		wchar_t arrInput[4096] = { 0, };
		CFLString<wchar_t> flsInput = L"";

		// DIO ��ġ�� �ʱ�ȭ �մϴ�. // Initialize the DIO device.
		if((res = devDIO.Initialize()).IsFail())
		{
 			ErrorPrint(res, L"Failed to initialize the device.");
 			break;
		}

		while(true)
		{
			// ����� ����� �����մϴ�. // Select the features you want to use.
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
					// Bit �� �Է� �޽��ϴ�. // Enter Bit.
					printf("Bit input: ");
					fgetws(arrInput, 4096, stdin);

					flsInput = arrInput;
					int32_t i32Bit = flsInput.ToInt32();

					bool bReadStatus = false;

					// Bit �� ���¸� �н��ϴ�. // Read Bit status.
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
					// Bit �� �Է� �޽��ϴ�. // Enter Bit.
					printf("Bit input: ");
					fgetws(arrInput, 4096, stdin);

					flsInput = arrInput;
					int32_t i32Bit = flsInput.ToInt32();

					// ���¸� �Է� �޽��ϴ�. // Enter status.
					printf("Status input: ");
					fgetws(arrInput, 4096, stdin);
					
					bool bWriteStatus = false;

					flsInput = arrInput;
						
					if(flsInput.ToInt32())
						bWriteStatus = true;

					// Bit �� ���¸� ����մϴ�. // Write the status in Bit.
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

	// DIO ��ġ�� �ʱ�ȭ�� �����մϴ�. // Terminate the DIO device.
	devDIO.Terminate();

	if(res.IsFail())
		getchar();

	return 0;
}