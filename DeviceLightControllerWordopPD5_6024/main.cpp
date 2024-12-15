#include <cstdio>

#include <FLImaging.h>

#include "../CommomHeader/ErrorPrint.h" 

int main()
{
	CResult res = EResult_UnknownError;

	// ���� ��Ʈ�ѷ� WordopPD5_6024 ���� // Declare the WordopPD5_6024 Light Controller
	CDeviceLightControllerWordopPD5_6024 lightController;

	do
	{
		wchar_t arrInput[4096] = { 0, };

		CFLString<wchar_t> flsInput = L"";
		int32_t i32ConnectionType = 0;
		bool bExit = false;

		do
		{
			// ���� ��Ʈ�ѷ� ���� ����� �����մϴ�. // Select the connection method for the light controller.
			printf("1. RS232C\n");
			printf("2. TCP Server\n");
			printf("3. TCP Client\n");
			printf("4. UDP\n");
			printf("0. Exit\n");
			printf("Connection Type: ");
			fgetws(arrInput, 4096, stdin);
			printf("\n");

			flsInput = arrInput;

			int32_t i32Select = flsInput.ToInt32();

			if(i32Select == 0)
				bExit = true;
			else if(i32Select == 1 || i32Select == 2 || i32Select == 3 || i32Select == 4)
			{
				i32ConnectionType = i32Select;
				break;
			}

			printf("Incorrect input. Please select again.\n\n");
		}
		while(true);

		if(bExit)
			break;

		CDeviceLightControllerWordopPD5_6024::EConnectionMethod eConnectionMethod = CDeviceLightControllerWordopPD5_6024::EConnectionMethod_RS232C;

		if(i32ConnectionType == 2)
			eConnectionMethod = CDeviceLightControllerWordopPD5_6024::EConnectionMethod_TCPServer;
		else if(i32ConnectionType == 3)
			eConnectionMethod = CDeviceLightControllerWordopPD5_6024::EConnectionMethod_TCPClient;
		else if(i32ConnectionType == 4)
			eConnectionMethod = CDeviceLightControllerWordopPD5_6024::EConnectionMethod_UDP;

		// ���� ����� �����մϴ�. // Set the connection method.
		lightController.SetConnectionMethod(eConnectionMethod);

		if(i32ConnectionType == 1) // RS232C
		{
			// ����Ʈ ��ȣ�� �Է��մϴ�. // Enter the COM port number.
			int32_t i32ComPortNumber = 0;
			printf("Port Number: ");
			fgetws(arrInput, 4096, stdin);
			flsInput = arrInput;
			i32ComPortNumber = flsInput.ToInt32();

			// ����Ʈ ��ȣ ���� // Set the COM port number.
			lightController.SetConnectionComPortNumber(i32ComPortNumber);
		}
		else
		{
			CFLString<wchar_t> flsIPAddress;
			uint16_t u16Port = 0;

			// IP ��ȣ�� �Է��մϴ�. // Enter the IP address.
			printf("Input IP Address: ");
			fgetws(arrInput, 4096, stdin);

			flsIPAddress = arrInput;

			// Port ��ȣ�� �Է��մϴ�. // Enter the port number.
			printf("Port Number: ");
			fgetws(arrInput, 4096, stdin);

			flsInput = arrInput;
			u16Port = flsInput.ToInt32();

			// IP �ּ�, Port ���� // Set the IP address and port.
			lightController.SetConnectionIPAddress(flsIPAddress);
			lightController.SetConnectionPort(u16Port);
		}

		if(IsFail(res = lightController.Initialize()))
		{
			ErrorPrint(res, "Failed to initialize the light controller.\n");
			break;
		}

		int32_t i32Channel = 0;

		do
		{
			// ä�� ������ �����մϴ�. // Select the number of channels.
			printf("\n");
			printf("1. Channel 4\n");
			printf("2. Channel 8\n");
			printf("0. Exit\n");
			printf("Input Channel Count: ");
			fgetws(arrInput, 4096, stdin);
			printf("\n");

			flsInput = arrInput;

			int32_t i32Select = flsInput.ToInt32();

			if(i32Select == 0)
				bExit = true;
			else if(i32Select == 1 || i32Select == 2)
			{
				i32Channel = i32Select;
				break;
			}

			printf("Incorrect input. Please select again.\n\n");
		}
		while(true);

		if(bExit)
			break;

		CDeviceLightControllerWordopPD5_6024::ELightChannel eLightChannel = CDeviceLightControllerWordopPD5_6024::ELightChannel_Port_4;

		if(i32Channel == 1)
			eLightChannel = CDeviceLightControllerWordopPD5_6024::ELightChannel_Port_8;

		// ä�� ���� ���� // Set the number of channels.
		lightController.SetLightChannel(eLightChannel);

		int32_t i32CommuticationType = 0;

		do
		{
			// ��� ����� �����մϴ�. // Select the communication method.
			printf("\n");
			printf("1. ASCII Code\n");
			printf("2. Hexadecimal\n");
			printf("0. Exit\n");
			printf("Input Communication Type: ");
			fgetws(arrInput, 4096, stdin);
			printf("\n");

			flsInput = arrInput;

			int32_t i32Select = flsInput.ToInt32();

			if(i32Select == 0)
				bExit = true;
			else if(i32Select == 1 || i32Select == 2)
			{
				i32CommuticationType = i32Select;
				break;
			}

			printf("Incorrect input. Please select again.\n\n");
		}
		while(true);

		if(bExit)
			break;

		CDeviceLightControllerWordopPD5_6024::ECommunicationType eCommunicationType = CDeviceLightControllerWordopPD5_6024::ECommunicationType_ASCIICode;

		if(i32CommuticationType == 2)
			eCommunicationType = CDeviceLightControllerWordopPD5_6024::ECommunicationType_Hexadecimal;

		// ��� ����� �����մϴ�. // Set the communication type.
		lightController.SetCommunicationType(eCommunicationType);

		int32_t i32TriggerMethod = 0;

		while(true)
		{
			int32_t i32SelectMode = 0;

			// �۾� ��带 �����մϴ�. // Select the operation mode.
			printf("\n");
			printf("1. Light On/Off\n");
			printf("2. Light Value\n");
			printf("3. Strobe Time\n");
			printf("4. Trigger Method\n");
			printf("0. Exit\n");
			printf("Select Number: ");
			fgetws(arrInput, 4096, stdin);
			printf("\n");

			flsInput = arrInput;
			i32SelectMode = flsInput.ToInt32();

			if(i32SelectMode == 0)
			{
				bExit = true;
				break;
			}

			if(i32SelectMode == 4)
			{
				do
				{
					// Ʈ���� ����� �����մϴ�. // Select the trigger method.
					printf("\n");
					printf("1. Low Level\n");
					printf("2. High Level\n");
					printf("3. Falling Edge\n");
					printf("4. Rising Edge\n");
					printf("0. Exit\n");
					printf("Input Trigger Method: ");
					fgetws(arrInput, 4096, stdin);
					printf("\n");

					flsInput = arrInput;

					int32_t i32Select = flsInput.ToInt32();

					if(i32Select == 0)
						bExit = true;
					else if(i32Select == 1 || i32Select == 2 || i32Select == 3 || i32Select == 4)
					{
						i32TriggerMethod = i32Select;
						break;
					}

					printf("Incorrect input. Please select again.\n\n");
				}
				while(true);

				CDeviceLightControllerWordopPD5_6024::ETriggerMethod eTriggerMethod = CDeviceLightControllerWordopPD5_6024::ETriggerMethod_LowLevel;

				if(i32TriggerMethod == 2)
					eTriggerMethod = CDeviceLightControllerWordopPD5_6024::ETriggerMethod_HighLevel;
				else if(i32TriggerMethod == 3)
					eTriggerMethod = CDeviceLightControllerWordopPD5_6024::ETriggerMethod_FallingEdge;
				else if(i32TriggerMethod == 4)
					eTriggerMethod = CDeviceLightControllerWordopPD5_6024::ETriggerMethod_RisingEdge;

				// Ʈ���� ����� �����մϴ�. // Set the trigger method.
				lightController.SetTriggerMethod(eTriggerMethod);
			}
			else
			{
				// ������ ä�� ���� �Է��մϴ�. // Enter the channel value to configure.
				printf("Select Channel: ");
				fgetws(arrInput, 4096, stdin);
				flsInput = arrInput;

				int32_t i32Channel = flsInput.ToInt32();

				if(i32SelectMode == 1)
				{
					printf("\n");
					printf("0. On\n");
					printf("1. Off\n");
					printf("Enter On/Off: ");
					fgetws(arrInput, 4096, stdin);
					printf("\n");

					flsInput = arrInput;

					int32_t i32OnOff = flsInput.ToInt32();

					// ä�κ� On/Off ���¸� �����մϴ�. // Set the On/Off state for the channel.
					if(i32OnOff == 0)
						lightController.SetChannelState(i32Channel, false);
					else if(i32OnOff == 1)
						lightController.SetChannelState(i32Channel, true);
				}
				else if(i32SelectMode == 2)
				{
					// ���� ���� �����մϴ�. // Set the light value.
					printf("Input Light Value (0 ~ 255): ");
					fgetws(arrInput, 4096, stdin);
					flsInput = arrInput;

					uint8_t u8Value = (uint8_t)flsInput.ToInt32();

					lightController.SetLightValue(i32Channel, u8Value);
				}
				else if(i32SelectMode == 3)
				{
					// ����κ� Ÿ���� �����մϴ�. // Set the strobe time.
					printf("Input Strobe Time (1 ~ 999 ms): ");
					fgetws(arrInput, 4096, stdin);
					flsInput = arrInput;

					uint16_t u16StrobeTime = (uint16_t)flsInput.ToInt32();

					lightController.SetStrobeTime(i32Channel, u16StrobeTime);
				}
			}

			// �Էµ� �Ķ���͸� �����մϴ�. // Apply the configured parameters.
			lightController.Apply();
		}

		if(bExit)
			break;
	}
	while(false);

	// ���� ��Ʈ�ѷ��� ������ �����մϴ�. // Terminate the connection to the light controller.
	if(IsFail(res = lightController.Terminate()))
	{
		ErrorPrint(res, "Failed to terminate the motion.\n");
	}

	return 0;
}