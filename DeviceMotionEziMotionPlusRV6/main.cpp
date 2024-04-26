#include <cstdio>

#include <FLImaging.h>

#include "../CommomHeader/ErrorPrint.h" 

// ��� Ÿ�� enum // motion type enum
enum EMotion
{
	EMotion_SearchOriginPosition = 0,
	EMotion_MoveAbsolutePosition,
	EMotion_MoveIncrementalPosition,
};


int main()
{
	CResult res = EResult_UnknownError;

	// Ezi Motion Plus R V6 ���� // Declare Ezi Motion Plus R V6
	CDeviceMotionEziMotionPlusRV6 motionEziMotionPluse;

	do
	{
		wchar_t arrInput[4096] = { 0, };

		CFLString<wchar_t> flsCamfilePath = L"";
		int32_t i32PortNumber = 0;
		int32_t i32AxisNumber = 0;
		double f64Resolution = 0;
		double f64MoveSpeed = 0;
		double f64AccDecSpeed = 0;
		double f64Position = 0;
		EMotion eMotionType = EMotion_SearchOriginPosition;
		CFLString<wchar_t> flsInput = L"";

		// ��ġ�� ����� ��Ʈ ��ȣ�� �Է��մϴ�. // Connected devices port number.
		printf("Port Number : ");
		fgetws(arrInput, 4096, stdin);
		flsInput = arrInput;
		i32PortNumber = flsInput.ToInt32();

		// ��Ʈ ��ȣ�� �����մϴ�. // Set Port Number.
		motionEziMotionPluse.SetPortNo(i32PortNumber);

		// ������ �� ���� // connected axis Count
		motionEziMotionPluse.SetAxisCount(1);

		// ��ġ �ʱ�ȭ // devices initialize
		if(IsFail(res = motionEziMotionPluse.Initialize()))
		{
			ErrorPrint(res, "Failed to initialize the motion.\n");
			break;
		}

		// ��� �� ��ü // motion axis object
		CDeviceMotionAxisEziMotionPlusRV6* pDMAxis = (CDeviceMotionAxisEziMotionPlusRV6*)motionEziMotionPluse.GetMotionAxis(i32AxisNumber);

		if(!pDMAxis)
		{
			printf("Failed to get Axis.\n");
			break;
		}

		// ���� On // Servo On
		if(IsFail(res = pDMAxis->SetServoOn(true)))
		{
			ErrorPrint(res, "Failed to servo on.\n");
			break;
		}

		do
		{
			Sleep(100);
		}
		while(!pDMAxis->IsServoOn());

		// Axis resolution�� �Է��մϴ�. // Set axis resolution value.
		printf("Enter axis resolution(mm/pulse) : ");
		fgetws(arrInput, 4096, stdin);
		flsInput = arrInput;
		f64Resolution = flsInput.ToDouble();
		pDMAxis->SetResolution(f64Resolution);

		while(true)
		{
			bool bExit = false;

			while(true)
			{
				printf("\n");
				printf("1. Search Origin Position\n");
				printf("2. Move Absolute Position\n");
				printf("3. Move Incremental Position\n");
				printf("0. Exit\n");
				printf("Select Motion Type: ");
				fgetws(arrInput, 4096, stdin);
				printf("\n");

				flsInput = arrInput;
				int32_t i32Select = flsInput.ToInt32();
				bool bSelected = true;

				switch(i32Select)
				{
				case 0:
					bExit = true;
					break;
				case 1:
					eMotionType = EMotion_SearchOriginPosition;
					break;
				case 2:
					eMotionType = EMotion_MoveAbsolutePosition;
					break;
				case 3:
					eMotionType = EMotion_MoveIncrementalPosition;
					break;

				default:
					bSelected = false;
					break;
				}

				if(bSelected)
					break;

				printf("Incorrect input. Please select again.\n\n");
			}

			if(bExit)
				break;

			if(eMotionType == EMotion_SearchOriginPosition)
			{
				// ���� ���� ������ �����մϴ�. // 
				if(IsFail(res = pDMAxis->SearchOriginPosition()))
				{
					ErrorPrint(res, "Failed to search origin position.\n");
					break;
				}

				// ����� ���� �ɶ����� ��� // Wait until motion stops/
				do
				{
					Sleep(100);
				}
				while(!pDMAxis->IsSearchOrigin());

				if(!pDMAxis->IsSearchOriginEnd())
					printf("Failed to search origin position.\n");
				else
					printf("Successed to search origin position.\n");
			}
			else
			{
				// �̵� �ӵ��� �Է��մϴ�. // Set axis speed.
				printf("Enter Axis Speed(mm/s): ");
				fgetws(arrInput, 4096, stdin);
				flsInput = arrInput;
				f64MoveSpeed = flsInput.ToDouble();

				// �������� �Է��մϴ�. // Set axis acceleation.
				printf("Enter Axis Acceleation(mm/s2): ");
				fgetws(arrInput, 4096, stdin);
				flsInput = arrInput;
				f64AccDecSpeed = flsInput.ToDouble();

				// �̵��Ÿ��� ������ġ�� �Է��մϴ�. // Set incremental or absolute position.
				printf("Enter Axis Position(mm): ");
				fgetws(arrInput, 4096, stdin);
				flsInput = arrInput;
				f64Position = flsInput.ToDouble();

				if(eMotionType == EMotion_MoveAbsolutePosition)
				{
					// ���� ��ǥ�� �̵� // move absolute position
					if(IsFail(res = pDMAxis->MovePosition(f64Position, f64MoveSpeed, f64AccDecSpeed, f64AccDecSpeed, false)))
					{
						ErrorPrint(res, "Failed to initialize the camera.\n");
						break;
					}

					// ����� ���� �ɶ����� ��� // Wait until motion stops/
					do
					{
						Sleep(100);
					}
					while(pDMAxis->IsInMotion());

					if(!pDMAxis->GetInposition())
						printf("Failed to move absolute position.\n");
					else
						printf("Successed to move absolute position.\n");
				}
				else if(eMotionType == EMotion_MoveIncrementalPosition)
				{
					if(IsFail(res = pDMAxis->MoveDistance(f64Position, f64MoveSpeed, f64AccDecSpeed, f64AccDecSpeed, false)))
					{
						ErrorPrint(res, "Failed to initialize the camera.\n");
						break;
					}

					// ����� ���� �ɶ����� ��� // Wait until motion stops/
					do
					{
						Sleep(100);
					}
					while(pDMAxis->IsInMotion());

					if(!pDMAxis->GetInposition())
						printf("Failed to move incremental position.\n");
					else
						printf("Successed to move incremental position.\n");
				}
			}
		}

	}
	while(false);

	// Motion ��ü�� ������ ���� �մϴ�. // Terminate the connection to the motion object.
	if(IsFail(res = motionEziMotionPluse.Terminate()))
	{
		ErrorPrint(res, "Failed to terminate the motion.\n");
	}

	return 0;
}