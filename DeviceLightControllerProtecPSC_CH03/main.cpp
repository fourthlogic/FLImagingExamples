#include <cstdio>

#include <FLImaging.h>

#include "../CommomHeader/ErrorPrint.h" 

int main()
{
	CResult res = EResult_UnknownError;

	// ���� ��Ʈ�ѷ� ProtecPSC_CH03 ���� // Declare the ProtecPSC_CH03 Light Controller
	CDeviceLightControllerProtecPSC_CH03 lightController;

	do
	{
		wchar_t arrInput[4096] = { 0, };

		CFLString<wchar_t> flsInput = L"";
		bool bExit = false;

		{
			// ����Ʈ ��ȣ�� �Է��մϴ�. // Enter the COM port number.
			int32_t i32ComPortNumber = 0;
			printf("Port Number: ");
			fgetws(arrInput, 4096, stdin);
			flsInput = arrInput;
			i32ComPortNumber = flsInput.ToInt32();
			system("cls");
			// ����Ʈ ��ȣ ���� // Set the COM port number.
			lightController.SetComPortNumber(i32ComPortNumber);
		}

		if(IsFail(res = lightController.Initialize()))
		{
			ErrorPrint(res, "Failed to initialize the light controller.\n");
			break;
		}

		while(true)
		{
			int32_t i32SelectMode = 0;

			// �۾� ��带 �����մϴ�. // Select the operation mode.
			printf("1. Live Mode\n");
			printf("2. Strobe Mode\n");
			printf("0. Exit\n");
			printf("\n");
			printf("Select Number: ");
			fgetws(arrInput, 4096, stdin);
			printf("\n");
			system("cls");
			flsInput = arrInput;
			i32SelectMode = flsInput.ToInt32();

			if(i32SelectMode == 0)
			{
				bExit = true;
				break;
			}

			int32_t i32TriggerIndex = 0;

			if(i32SelectMode == 1)
			{
				// On/Off ���¸� �����մϴ�. // Set the On/Off state.
				printf("1. Live On\n");
				printf("2. Live Off\n");
				printf("\n");
				printf("Select Number: ");
				fgetws(arrInput, 4096, stdin);
				printf("\n");
				system("cls");
				flsInput = arrInput;

				int32_t i32OnOff = flsInput.ToInt32();

				lightController.SetOperationMode(CDeviceLightControllerProtecPSC_CH03::EOperationMode_Live);

				if(i32OnOff == 1)
				{
					lightController.EnableLiveTurnOn(true);

					// ������ ä�� ���� �Է��մϴ�. // Enter the channel value to configure.
					printf("Select Channel Index: ");
					fgetws(arrInput, 4096, stdin);
					flsInput = arrInput;

					int32_t i32Channel = flsInput.ToInt32();

					// ���� ���� �����մϴ�. // Set the light value.
					printf("Input Light Value (0 ~ 255): ");
					fgetws(arrInput, 4096, stdin);
					flsInput = arrInput;
					system("cls");
					uint8_t u8Value = (uint8_t)flsInput.ToInt32();

					lightController.SetLightValue(i32Channel, u8Value);
				}
				else if(i32OnOff == 2)
					lightController.EnableLiveTurnOn(false);
			}
			else if(i32SelectMode == 2)
			{
				lightController.SetOperationMode(CDeviceLightControllerProtecPSC_CH03::EOperationMode_Strobe);

				// ������ Ʈ���� Index�� �Է��մϴ�. // Enter the trigger index value to configure.
				printf("Select Trigger Index: ");
				fgetws(arrInput, 4096, stdin);
				flsInput = arrInput;

				i32TriggerIndex = flsInput.ToInt32();

				// ������ ä�� Index �� �Է��մϴ�. // Enter the channel index to configure.
				printf("Select Channel Index: ");
				fgetws(arrInput, 4096, stdin);
				flsInput = arrInput;

				int32_t i32Channel = flsInput.ToInt32();

				// ����κ� ���� �����մϴ�. // Set the strobe value.
				printf("Input Strobe Value (0 ~ 4000 us): ");
				fgetws(arrInput, 4096, stdin);
				flsInput = arrInput;
				system("cls");
				uint16_t u16StrobeTime = (uint16_t)flsInput.ToInt32();

				lightController.SetStrobe(i32TriggerIndex, i32Channel, u16StrobeTime);
			}

			// �Էµ� �Ķ���͸� �����մϴ�. // Apply the configured parameters.
			if(IsFail(res = lightController.Apply(i32TriggerIndex)))
			{
				ErrorPrint(res, "Failed to apply the light controller.\n");
				break;
			}
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