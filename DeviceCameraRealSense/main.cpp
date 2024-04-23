#include <cstdio>

#include <FLImaging.h>


// ī�޶󿡼� ������ ��� �̺�Ʈ�� �ޱ� ���� CDeviceEventImageBase �� ��� �޾Ƽ� ����
class CDeviceEventImageEx : public CDeviceEventImageBase
{
public:
	// CDeviceEventImageEx ������
	CDeviceEventImageEx()
	{
		m_pView3D = nullptr;
	}

	// CDeviceEventImageEx �Ҹ���
	virtual ~CDeviceEventImageEx()
	{
	}

	// ����� �����͸� ǥ���� 3D �並 �����ϴ� �Լ�
	void SetView3D(CGUIView3DWrap* pView3D)
	{
		if(pView3D)
			m_pView3D = pView3D;
	}

	// ī�޶󿡼� ������ ��� �� ȣ�� �Ǵ� �Լ�
	virtual void OnAcquisition(const CDeviceImageBase* pDeviceImage)
	{
		do
		{
			if(!m_pView3D)
				break;

			// 3D ���� ��ȿ���� Ȯ���Ѵ�.
			if(!m_pView3D->IsAvailable())
				break;

			const CDeviceCameraRealSense* pCamera = dynamic_cast<const CDeviceCameraRealSense*>(pDeviceImage);

			if(!pCamera)
				break;

			// ������ ��ü ����
			CFL3DObject floData;

			// ī�޶󿡼� ��� �� �����͸� ���´�.
			pCamera->GetAcquired3DData(floData);

			// 3D ���� ������Ʈ�� �����ϴ�.
			m_pView3D->LockUpdate();

			// 3D ���� ��ȿ���� Ȯ���Ѵ�.
			if(!m_pView3D->IsAvailable())
				break;

			// 3D ���� ��ü ������ ���ɴϴ�.
			int32_t i32ObjectCount = m_pView3D->GetObjectCount();

			// 3D ���� ��ü���� ��� Ŭ�����մϴ�.
			m_pView3D->ClearObjects();

			// 3D ���� ��ȿ���� Ȯ���Ѵ�.
			if(!m_pView3D->IsAvailable())
				break;

			// 3D �信 ��ü�� �߰��մϴ�.
			m_pView3D->PushObject(floData);

			// 3D ���� ��ȿ���� Ȯ���Ѵ�.
			if(!m_pView3D->IsAvailable())
				break;

			// 3D ���� ������Ʈ ���� ���� �����մϴ�.
			m_pView3D->UnlockUpdate();

			// 3D ���� �������� �����մϴ�.
			if(!i32ObjectCount)
				m_pView3D->ZoomFit();
		}
		while(false);
	}

protected:
	CGUIView3DWrap* m_pView3D;
};


int main()
{
	CResult drReturn = EResult_UnknownError;

	// 3D �� ����
	CGUIView3DWrap View3D;

	// RealSense ī�޶� ����
	CDeviceCameraRealSense camRealSense;

	do
	{
		wchar_t arrInput[4096] = { 0, };
		bool bAutoDetect = false;
		int32_t i32SelectDevice = 0;
		CFLString<wchar_t> flsConnection = L"";

		// ��ġ ã�� ����� �����մϴ�.
		while(true)
		{
			printf("1. Auto Detect\n");
			printf("2. Manual\n");
			printf("Select Detection Method: ");
			fgetws(arrInput, 4096, stdin);

			CFLString<wchar_t> flsInput(arrInput);
			int32_t i32Select = flsInput.ToInt32();
			bool bSelected = true;

			switch(i32Select)
			{
			case 1:
				bAutoDetect = true;
				break;

			case 2:
				bAutoDetect = false;
				break;

			default:
				bSelected = false;
				break;
			}

			if(bSelected)
				break;

			printf("Incorrect input. Please select again.\n\n");
		}

		printf("\n");

		if(bAutoDetect)
		{
			CFLArray<CFLString<wchar_t>> flarrSerialNumbers;

			// ����Ǿ� �ִ� ī�޶��� �ø��� ��ȣ�� ��´�.
			drReturn = camRealSense.GetAutoDetectCameraSerialNumbers(flarrSerialNumbers);

			if(drReturn.IsFail() || !flarrSerialNumbers.GetCount())
			{
				drReturn = EResult_FailedToRead;
				printf("Not Found Device.\n");
				break;
			}

			// ���� �� ī�޶� �����մϴ�.
			while(true)
			{
				for(int64_t i = 0; i < flarrSerialNumbers.GetCount(); ++i)
				{
					CFLString<wchar_t> flsElement;
					flsElement.Format(L"%d. ", i + 1);

					flsElement += flarrSerialNumbers.GetAt(i);
					flsElement += L"\n";

					wprintf_s(flsElement);
				}

				printf("Select Device: ");
				fgetws(arrInput, 4096, stdin);

				CFLString<wchar_t> flsInput(arrInput);
				int32_t i32Select = flsInput.ToInt32() - 1;

				if(i32Select < 0 || i32Select >= flarrSerialNumbers.GetCount())
				{
					printf("Incorrect input. Please select again.\n\n");
					continue;
				}

				i32SelectDevice = i32Select;

				break;
			}
		}
		else
		{
			// �ø��� ��ȣ�� �Է� �޴´�.
			printf("Input Serial Number: ");
			fgetws(arrInput, 4096, stdin);

			flsConnection = arrInput;
			flsConnection.Replace(L"\n", L"");
		}

		// �̺�Ʈ�� ���� ��ü ����
		CDeviceEventImageEx eventImage;

		// ī�޶� �̺�Ʈ ��ü ����
		camRealSense.RegisterDeviceEvent(&eventImage);

		if(bAutoDetect)
			// �ε����� �ش��ϴ� ī�޶�� ������ �����մϴ�.
			drReturn = camRealSense.AutoDetectCamera(i32SelectDevice);
		else
			// �ø��� ��ȣ�� �����մϴ�.
			camRealSense.SetSerialNumber(flsConnection);

		// ī�޶� �ʱ�ȭ �մϴ�.
		drReturn = camRealSense.Initialize();

		if(drReturn.IsFail())
		{
			printf("Failed to initialize the camera.\n");
			break;
		}

		// 3D �� ����
		if(View3D.Create(0, 0, 1000, 1000).IsFail())
		{
			drReturn = EResult_FailedToCreateObject;

			printf("Failed to create the image view.\n");
			break;
		}

		eventImage.SetView3D(&View3D);

		// ī�޶� Live �մϴ�.
		drReturn = camRealSense.Live();

		if(drReturn.IsFail())
		{
			printf("Failed to live the camera.\n");
			break;
		}

		// 3D �䰡 ����� �� ���� ��ٸ�
		while(View3D.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	// ī�޶��� �ʱ�ȭ�� �����մϴ�.
	camRealSense.Terminate();

	if(drReturn.IsFail())
		getchar();

	return 0;
}