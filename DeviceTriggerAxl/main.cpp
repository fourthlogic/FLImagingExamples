#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	CResult res;

	// Axl Trigger 장치를 선언 // Declare Axl Trigger device
	CDeviceTriggerAxl devTrigger;

	do
	{
		wchar_t arrInput[4096] = { 0, };
		CFLString<wchar_t> flsInput = L"";

		// 장치의 모듈 인덱스를 입력합니다. // Enter the module index of the device.
		printf("Enter Module index: ");
		fgetws(arrInput, 4096, stdin);
		flsInput = arrInput;

		int32_t i32ModuleIndex = flsInput.ToInt32();

		// 장치의 모듈 인덱스를 설정합니다. // Sets the module index for the device.
		if((res = devTrigger.SetModuleIndex(i32ModuleIndex)).IsFail())
		{
 			ErrorPrint(res, L"Failed to set module index.");
 			break;
		}

		// Trigger 장치를 초기화 합니다. // Initialize the Trigger device.
		if((res = devTrigger.Initialize()).IsFail())
		{
			ErrorPrint(res, L"Failed to initialize the device.");
			break;
		}

		// 트리거 채널을 입력합니다. // Enter the trigger channel.
		int32_t i32Channel = 0;

		while(true)
		{
			printf("\n");
			printf("Enter trigger channel(0 ~ %d): ", devTrigger.GetTriggerChannelCount() - 1);
			fgetws(arrInput, 4096, stdin);
			flsInput = arrInput;

			i32Channel = flsInput.ToInt32();

			if(i32Channel < 0 || i32Channel >= devTrigger.GetTriggerChannelCount())
 				printf("Incorrect input. Please enter again.\n");
 			else
				break;
		}


		// 엔코더 소스를 입력합니다. // Enter the encoder source.
		CDeviceTriggerAxl::EEncoderSource eEncoderSource = CDeviceTriggerAxl::EEncoderSource_ABPhase;

		while(true)
		{
			printf("\n");
			printf("Encoder Source\n");
			printf("1. AB Phase\n");
			printf("2. Z Phase\n");
			printf("Select: ");
			fgetws(arrInput, 4096, stdin);
			flsInput = arrInput;

			int32_t i32Select = flsInput.ToInt32();

			bool bSelected = true;

			switch(i32Select)
			{
			case 1:
				eEncoderSource = CDeviceTriggerAxl::EEncoderSource_ABPhase;
				break;

			case 2:
				eEncoderSource = CDeviceTriggerAxl::EEncoderSource_ZPhase;
				break;

			default:
				bSelected = false;
				break;
			}

			if(bSelected)
				break;

			printf("Incorrect input. Please select again.\n");
		}

		// 엔코더 소스를 설정합니다. // Sets the encoder source.
		if((res = devTrigger.SetEncoderSource(i32Channel, eEncoderSource)).IsFail())
		{
 			ErrorPrint(res, L"Failed to set encoder source.");
			break;
		}


		// 엔코더 방식을 입력합니다. // Enter the encoder method.
		CDeviceTriggerAxl::EEncoderMethod eEncoderMethod = CDeviceTriggerAxl::EEncoderMethod_UpDownSqr1;

		while(true)
		{
			printf("\n");
			printf("Encoder Method\n");
			printf("1. Up/Down Square 1\n");
			printf("2. Up/Down Square 2\n");
			printf("3. AB Phase Square 1\n");
			printf("4. AB Phase Square 2\n");
			printf("5. AB Phase Square 4\n");
			printf("6. Pulse/Direction Square 1\n");
			printf("7. Pulse/Direction Square 2\n");
			printf("Select: ");
			fgetws(arrInput, 4096, stdin);
			flsInput = arrInput;

			int32_t i32Select = flsInput.ToInt32();

			bool bSelected = true;

			switch(i32Select)
			{
			case 1:
				eEncoderMethod = CDeviceTriggerAxl::EEncoderMethod_UpDownSqr1;
				break;

			case 2:
				eEncoderMethod = CDeviceTriggerAxl::EEncoderMethod_UpDownSqr2;
				break;

			case 3:
				eEncoderMethod = CDeviceTriggerAxl::EEncoderMethod_ABPhaseSqr1;
				break;

			case 4:
				eEncoderMethod = CDeviceTriggerAxl::EEncoderMethod_ABPhaseSqr2;
				break;

			case 5:
				eEncoderMethod = CDeviceTriggerAxl::EEncoderMethod_ABPhaseSqr4;
				break;

			case 6:
				eEncoderMethod = CDeviceTriggerAxl::EEncoderMethod_PulseDirSqr1;
				break;

			case 7:
				eEncoderMethod = CDeviceTriggerAxl::EEncoderMethod_PulseDirSqr2;
				break;

			default:
				bSelected = false;
				break;
			}

			if(bSelected)
				break;

			printf("Incorrect input. Please select again.\n");
		}

		// 엔코더 방식을 설정합니다. // Sets the encoder method.
		if((res = devTrigger.SetEncoderMethod(i32Channel, eEncoderMethod)).IsFail())
		{
 			ErrorPrint(res, L"Failed to set encoder method.");
 			break;
		}

		// 트리거 모드를 설정합니다. // Sets the trigger mode.
		if((res = devTrigger.SetTriggerMode(i32Channel, CDeviceTriggerAxl::ETriggerMode_Position)).IsFail())
		{
 			ErrorPrint(res, L"Failed to set trigger mode.");
 			break;
		}
		
		while(true)
		{
			// 트리거를 비활성화 합니다. // Disable the trigger.
			if((res = devTrigger.SetTriggerEnable(i32Channel, false)).IsFail())
			{
	 			ErrorPrint(res, L"Failed to set trigger enable.");
	 			break;
			}

			// 엔코더 포지션을 0 으로 설정합니다. // Set the encoder position to 0.
			if((res = devTrigger.SetEncoderPosition(i32Channel, 0.)).IsFail())
			{
	 			ErrorPrint(res, L"Failed to set encoder position.");
	 			break;
			}

			// 포지션 값을 입력합니다. // Enter a position value.
			printf("\n");
			printf("Enter trigger position(10, 20, 30, ...): ");
			fgetws(arrInput, 4096, stdin);
			flsInput = arrInput;

			// 포지션 값을 담기위해 CFLArray<double> 생성 // Create CFLArray<double>> to hold position values
			CFLArray<double> flaPosition;

			// 입력 받은 문자열을 ',' 으로 구분하여 double 값으로 변환합니다. // Separate the input string with ',' and convert it to a double value.
			int64_t i64TokenIdx = 0;

			while(true)
			{
				CFLString<char> flstrToken = flsInput.Tokenize(",", i64TokenIdx);

				if(flstrToken.IsEmpty())
					break;

				if(flstrToken == "\n")
					break;

				flaPosition.PushBack(flstrToken.ToDouble());
			}

			// 트리거 포지션을 설정합니다. // Sets the trigger position.
			if((res = devTrigger.SetTriggerPosition(i32Channel, flaPosition)).IsFail())
			{
	 			ErrorPrint(res, L"Failed to set trigger position.");
	 			break;
			}

			// 트리거를 활성화 합니다. // Enables the trigger.
			if((res = devTrigger.SetTriggerEnable(i32Channel, true)).IsFail())
			{
	 			ErrorPrint(res, L"Failed to set trigger enable.");
	 			break;
			}

			printf("\n");
			printf("0. Reset the trigger position\n");
			printf("Other. Exit\n");
			printf("Enter: ");
			fgetws(arrInput, 4096, stdin);
			flsInput = arrInput;

			if(flsInput != L"0\n")
				break;
		}
	}
	while(false);

	// Trigger 장치의 초기화를 해제합니다. // Terminate the Trigger device.
	devTrigger.Terminate();

	if(res.IsFail())
		getchar();

	return 0;
}