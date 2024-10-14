#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	CResult res;

	// Keyence ������ ���� ���� ��ġ�� ���� // Declare keyence laser displacement sensor device
	CDeviceLaserDisplacementSensorKeyence devLaserDisplacement;

	do
	{
		wchar_t arrInput[4096] = { 0, };
		CFLString<wchar_t> flsInput = L"";

		// ����Ʈ ��ȣ�� �Է��մϴ�. // Enter the com port number.
		printf("Enter com port number: ");
		fgetws(arrInput, 4096, stdin);
		flsInput = arrInput;

		int32_t i32ComPortNumber = flsInput.ToInt32();

		devLaserDisplacement.SetComPortNumber(i32ComPortNumber);

		while(true)
		{
			// ���� ����Ʈ�� �����մϴ�. // Select the baud rate.
			printf("\n");
			printf("1. 9600\n");
			printf("2. 19200\n");
			printf("3. 38400\n");
			printf("4. 57600\n");
			printf("5. 115200\n");
			printf("Select baud rate: ");

			fgetws(arrInput, 4096, stdin);
			flsInput = arrInput;

			int32_t i32Select = flsInput.ToInt32();
			int32_t i32BaudRate = -1;

			switch(i32Select)
			{
			case 1:
				i32BaudRate = 9600;
				break;
			case 2:
				i32BaudRate = 19200;
				break;
			case 3:
				i32BaudRate = 38400;
				break;
			case 4:
				i32BaudRate = 57600;
				break;
			case 5:
				i32BaudRate = 115200;
				break;
			}

			if(i32BaudRate != -1)
			{
				devLaserDisplacement.SetBaudRate(i32BaudRate);
				break;
			}

			printf("Incorrect input. Please select again.\n\n");
		}

		while(true)
		{
			// �и�Ƽ�� �����մϴ�. // Select the parity.
			printf("\n");
			printf("1. None\n");
			printf("2. Even\n");
			printf("3. Odd\n");
			printf("Select parity: ");

			fgetws(arrInput, 4096, stdin);
			flsInput = arrInput;

			int32_t i32Select = flsInput.ToInt32();
			int32_t i32Parity = -1;

			switch(i32Select)
			{
			case 1:
				i32Parity = 0;
				break;
			case 2:
				i32Parity = 1;
				break;
			case 3:
				i32Parity = 2;
				break;
			}

			if(i32Parity != -1)
			{
				devLaserDisplacement.SetParity(i32Parity);
				break;
			}

			printf("Incorrect input. Please select again.\n\n");
		}
		
		// ������ ���� ���� ��ġ�� �ʱ�ȭ �մϴ�. // Initialize the laser displacement sensor device.
		if((res = devLaserDisplacement.Initialize()).IsFail())
		{
 			ErrorPrint(res, L"Failed to initialize the device.");
 			break;
		}

		while(true)
		{
			// ��� ä���� �����մϴ�. // Select the output channel.
			printf("\n");
			printf("1. Output channel 1\n");
			printf("2. Output channel 2\n");
			printf("Select output channel: ");

			fgetws(arrInput, 4096, stdin);
			flsInput = arrInput;

			int32_t i32Select = flsInput.ToInt32();

			// �������� ���ɴϴ�. // Retrieve the measured value
			CFLArray<double> flaMeasured;

			switch(i32Select)
			{
			case 1:
				res = devLaserDisplacement.GetMeasuredValue(CDeviceLaserDisplacementSensorKeyence::EOutputChannel_1, flaMeasured);
				break;

			case 2:
				res = devLaserDisplacement.GetMeasuredValue(CDeviceLaserDisplacementSensorKeyence::EOutputChannel_2, flaMeasured);
				break;

			default:
				printf("Incorrect input. Please select again.\n\n");
			}

			if(!flaMeasured.GetCount())
				continue;

			if(res.IsFail())
			{
				ErrorPrint(res, L"Failed to get measured value.");
				continue;
			}

			printf("Output channel %d measured: %0.5lf\n\n", i32Select, flaMeasured.GetAt(0));
		}
	}
	while(false);

	// ������ ���� ���� ��ġ�� �ʱ�ȭ�� �����մϴ�. // Terminate the laser displacement sensor device.
	devLaserDisplacement.Terminate();

	if(res.IsFail())
		getchar();

	return 0;
}