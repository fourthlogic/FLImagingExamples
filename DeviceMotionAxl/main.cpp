#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 

// ��� ��� enum // motion feature enum
enum EMotionFeature
{
	EMotionFeature_SearchOriginPosition = 0,
	EMotionFeature_MoveAbsolutePosition,
	EMotionFeature_MoveIncrementalPosition,
};

int main()
{
	CResult res = EResult_UnknownError;

	// Axl Motion ��ġ�� ���� // Declare Axl Motion device
	CDeviceMotionAxl devMotion;

	do
	{
		wchar_t arrInput[4096] = { 0, };
		CFLString<wchar_t> flsInput = L"";

		// ��� ������ ��ü ��θ� �Է��մϴ�. // Enter the full path of the motion file.
		printf("Enter motion file full path (e.g. C:/Motion/Any.mot): ");
		fgetws(arrInput, 4096, stdin);

		CFLString<wchar_t> flsMotionPath = arrInput;
		flsMotionPath.Replace(L"\r", L"");
		flsMotionPath.Replace(L"\n", L"");

		// ��� ������ ��θ� �����մϴ�. // Sets the path to the motion file.
		if((res = devMotion.SetMotionFilePath(flsMotionPath)).IsFail())
		{
			ErrorPrint(res, L"Failed to set motion file path.");
			break;
		}

		// ������ �� ������ �����մϴ�. // Sets the number of axes to connect to.
		if((res = devMotion.SetAxisCount(1)).IsFail())
		{
			ErrorPrint(res, L"Failed to set axis count.");
			break;
		}

		// ��� ��ġ�� �ʱ�ȭ �մϴ�. // Initialize the motion device.
		if((res = devMotion.Initialize()).IsFail())
		{
			ErrorPrint(res, L"Failed to initialize the device.");
			break;
		}

		// ��� �� ��ü�� ���ɴϴ�. // Obtain motion axis objects.
		const CDeviceMotionAxlAxis* pMotionAxis = dynamic_cast<const CDeviceMotionAxlAxis*>(devMotion.GetMotionAxis(0));

		if(!pMotionAxis)
		{
			printf("Failed to get motion axis.\n");
			break;
		}

		// ������ �մϴ�. // Turn on the servo.
		if((res = pMotionAxis->SetServoOn(true)).IsFail())
		{
			ErrorPrint(res, "Failed to servo on.\n");
			break;
		}

		do
		{
			CThreadUtilities::Sleep(100);
		}
		while(!pMotionAxis->IsServoOn());

		while(true)
		{
			EMotionFeature eMotionFeature = EMotionFeature_MoveAbsolutePosition;
			bool bExit = false;

			while(true)
			{
				// ����� ��� ����� �����մϴ�. // Select the motion feature you want to use.
				printf("\n");
				printf("1. Search Origin Position\n");
				printf("2. Move Absolute Position\n");
				printf("3. Move Incremental Position\n");
				printf("0. Exit\n");
				printf("Select: ");
				fgetws(arrInput, 4096, stdin);
				flsInput = arrInput;

				int32_t i32Select = flsInput.ToInt32();

				bool bSelected = true;

				switch(i32Select)
				{
				case 0:
					bExit = true;
					break;
				case 1:
					eMotionFeature = EMotionFeature_SearchOriginPosition;
					break;
				case 2:
					eMotionFeature = EMotionFeature_MoveAbsolutePosition;
					break;
				case 3:
					eMotionFeature = EMotionFeature_MoveIncrementalPosition;
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

			if(eMotionFeature == EMotionFeature_SearchOriginPosition)
			{
				// ���� ���� ������ �����մϴ�. // Proceed with the return-to-origin action.
				if((res = pMotionAxis->SearchOriginPosition()).IsFail())
				{
					ErrorPrint(res, "Failed to search origin position.\n");
					break;
				}

				// ���� ���� ������ �Ϸ� �ɶ����� ����մϴ�. // Wait until the return to origin action is complete.
				do
				{
					CThreadUtilities::Sleep(100);
				}
				while(pMotionAxis->GetSearchOriginStatus() == CDeviceMotionAxlAxis::ESearchOriginStatus_Searching);

				if(pMotionAxis->GetSearchOriginStatus() == CDeviceMotionAxlAxis::ESearchOriginStatus_Error)
					printf("Failed to search origin position.\n");
				else
					printf("Successed to search origin position.\n");
			}
			else
			{
				double f64MoveVelocity = 0.;
				double f64MoveAccelAndDecel = 0.;
				double f64MovePosition = 0.;

				// �̵� �ӵ��� �Է��մϴ�. // Enter the velocity of movement.
				printf("Enter Axis Velocity(mm/s): ");
				fgetws(arrInput, 4096, stdin);
				flsInput = arrInput;
				f64MoveVelocity = flsInput.ToDouble();

				// �������� �Է��մϴ�. // Enter acceleration and deceleration.
				printf("Enter Axis Acceleration(mm/s^2): ");
				fgetws(arrInput, 4096, stdin);
				flsInput = arrInput;
				f64MoveAccelAndDecel = flsInput.ToDouble();

				// �̵��Ÿ��� ������ġ�� �Է��մϴ�. // Enter the distance or absolute position.
				printf("Enter Axis Position(mm): ");
				fgetws(arrInput, 4096, stdin);
				flsInput = arrInput;
				f64MovePosition = flsInput.ToDouble();

				if(eMotionFeature == EMotionFeature_MoveAbsolutePosition)
				{
					// ���� ��ǥ�� �̵��մϴ�. // Move to absolute coordinates.
					if((res = pMotionAxis->MovePosition(f64MovePosition, f64MoveVelocity, f64MoveAccelAndDecel, f64MoveAccelAndDecel, false)).IsFail())
					{
						ErrorPrint(res, "Failed to move position.\n");
						break;
					}

					// ����� ���� �ɶ����� ����մϴ�. // Wait until motion stops.
					do
					{
						CThreadUtilities::Sleep(100);
					}
					while(!pMotionAxis->IsMotionDone());
				}
				else if(eMotionFeature == EMotionFeature_MoveIncrementalPosition)
				{
					// ��� ��ǥ�� �̵��մϴ�. // Move to relative coordinates.
					if((res = pMotionAxis->MoveDistance(f64MovePosition, f64MoveVelocity, f64MoveAccelAndDecel, f64MoveAccelAndDecel, false)).IsFail())
					{
						ErrorPrint(res, "Failed to move distance.\n");
						break;
					}

					// ����� ���� �ɶ����� ����մϴ�. // Wait until motion stops.
					do
					{
						CThreadUtilities::Sleep(100);
					}
					while(!pMotionAxis->IsMotionDone());
				}
			}
		}
	}
	while(false);

	// Motion ��ġ�� �ʱ�ȭ�� �����մϴ�. // Terminate the Motion device.
	devMotion.Terminate();

	if(res.IsFail())
		getchar();

	return 0;
}