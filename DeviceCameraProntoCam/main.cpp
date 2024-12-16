#include <cstdio>

#include <FLImaging.h>


// ī�޶󿡼� �̹��� ��� �̺�Ʈ�� �ޱ� ���� CDeviceEventImageBase �� ��� �޾Ƽ� ����
class CDeviceEventImageEx : public CDeviceEventImageBase
{
public:
	// CDeviceEventImageEx ������
	CDeviceEventImageEx()
	{
		m_pViewImage = nullptr;
		// �̹����� ���� ��ü ���� // Create �̹����� ���� object
		m_pImage = new CFLImage;
	}

	// CDeviceEventImageEx �Ҹ���
	virtual ~CDeviceEventImageEx()
	{
		// ������ �̹��� ��ü ����
		if(m_pImage)
		{
			delete m_pImage;
			m_pImage = nullptr;
		}
	}

	// ����� �̹����� ǥ���� �̹��� �並 �����ϴ� �Լ�
	void SetViewImage(CGUIViewImageWrap* pViewImage)
	{
		if(pViewImage)
		{
			m_pViewImage = pViewImage;

			// �̹��� �信 �̹��� ������ ����
			m_pViewImage->SetImagePtr(m_pImage);
		}
	}

	// ī�޶󿡼� �̹��� ��� �� ȣ�� �Ǵ� �Լ�
	virtual void OnAcquisition(const CDeviceImageBase* pDeviceImage)
	{
		// �̹��� ���� ��ȿ���� Ȯ���Ѵ�.
		if(m_pViewImage && m_pViewImage->IsAvailable())
		{
			// ī�޶󿡼� ��� �� �̹����� ���´�.
			pDeviceImage->GetAcquiredImage(m_pImage);

			// �̹��� �並 �簻�� �Ѵ�.
			m_pViewImage->Invalidate();
		}
	}

protected:
	CGUIViewImageWrap* m_pViewImage;
	CFLImage* m_pImage;
};


int main()
{
	CResult drReturn = EResult_UnknownError;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	// ProntoCam ī�޶� ����
	CDeviceCameraProntoCam camProntoCam;

	// �̺�Ʈ�� ���� ��ü ����
	CDeviceEventImageEx eventImage;

	// ī�޶� �̺�Ʈ ��ü ����
	camProntoCam.RegisterDeviceEvent(&eventImage);

	do
	{
		wchar_t arrInput[4096] = { 0, };

		bool bAutoDetect = false;
		int32_t i32SelectDevice = -1;
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
			drReturn = camProntoCam.GetAutoDetectCamera(flarrSerialNumbers);
			
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
			// �ø��� ��ȣ�� �Է� �޽��ϴ�.
			printf("Input Serial Number: ");

			fgetws(arrInput, 4096, stdin);

			flsConnection = arrInput;
			flsConnection.Replace(L"\n", L"");
		}

		if(bAutoDetect)
		{
			// �ε����� �ش��ϴ� ī�޶�� ������ �����մϴ�.
			drReturn = camProntoCam.AutoDetectCamera(i32SelectDevice);
		}
		else
		{
			// �ø��� ��ȣ�� �����մϴ�.
			camProntoCam.SetSerialNumber(flsConnection);
		}

		// ī�޶� �ʱ�ȭ �մϴ�.
		drReturn = camProntoCam.Initialize();

		if(drReturn.IsFail())
		{
			printf("Failed to initialize the camera.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(viewImage.Create(0, 0, 1000, 1000).IsFail())
		{
			drReturn = EResult_FailedToCreateObject;

			printf("Failed to create the image view.\n");
			break;
		}

		eventImage.SetViewImage(&viewImage);

		// ī�޶� Live �մϴ�.
		drReturn = camProntoCam.Live();

		if(drReturn.IsFail())
		{
			printf("Failed to live the camera.\n");
			break;
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	// ī�޶��� �ʱ�ȭ�� �����մϴ�.
	camProntoCam.Terminate();
	camProntoCam.ClearDeviceEvents();

	if(drReturn.IsFail())
		getchar();

	return 0;
}