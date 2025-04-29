#include <cstdio>

#include <FLImaging.h>


// 카메라에서 이미지 취득 이벤트를 받기 위해 CDeviceEventImageBase 를 상속 받아서 구현
class CDeviceEventImageEx : public CDeviceEventImageBase
{
public:
	// CDeviceEventImageEx 생성자
	CDeviceEventImageEx()
	{
		m_pViewImage = nullptr;
		// 이미지를 받을 객체 생성 // Create 이미지를 받을 object
		m_pImage = new CFLImage;
	}

	// CDeviceEventImageEx 소멸자
	virtual ~CDeviceEventImageEx()
	{
		// 생성한 이미지 객체 해제
		if(m_pImage)
		{
			delete m_pImage;
			m_pImage = nullptr;
		}
	}

	// 취득한 이미지를 표시할 이미지 뷰를 설정하는 함수
	void SetViewImage(CGUIViewImageWrap* pViewImage)
	{
		if(pViewImage)
		{
			m_pViewImage = pViewImage;

			// 이미지 뷰에 이미지 포인터 설정
			m_pViewImage->SetImagePtr(m_pImage);
		}
	}

	// 카메라에서 이미지 취득 시 호출 되는 함수
	virtual void OnAcquisition(const CDeviceImageBase* pDeviceImage)
	{
		// 이미지 뷰의 유효성을 확인한다.
		if(m_pViewImage && m_pViewImage->IsAvailable())
		{
			// 카메라에서 취득 한 이미지를 얻어온다.
			pDeviceImage->GetAcquiredImage(m_pImage);

			// 이미지 뷰를 재갱신 한다.
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

	// 이미지 뷰 선언 // Declare image view
	CGUIViewImageWrap viewImage;

	// iRAYPLE 카메라 선언
	CDeviceCameraiRAYPLE camiRAYPLE;

	do
	{
		wchar_t arrInput[4096] = { 0, };

		CDeviceGenICamBase::EDeviceType eDeviceType = CDeviceGenICamBase::EDeviceType_GigE;
		bool bAutoDetect = false;
		int32_t i32SelectDevice = -1;
		CDeviceGenICamBase::EConnectionMethod eConnectionMethod = CDeviceGenICamBase::EConnectionMethod_SerialNumber;
		CFLString<wchar_t> flsConnection = L"";

		// 장치 타입을 선택합니다.
		while(true)
		{
			printf("1. GigE\n");
			printf("2. IEEE 1394\n");
			printf("3. USB\n");
			printf("4. Camera Link\n");
			printf("Select Device Type: ");
			fgetws(arrInput, 4096, stdin);

			CFLString<wchar_t> flsInput(arrInput);
			int32_t i32Select = flsInput.ToInt32();
			bool bSelected = true;

			switch(i32Select)
			{
			case 1:
				eDeviceType = CDeviceGenICamBase::EDeviceType_GigE;
				break;

			case 2:
				eDeviceType = CDeviceGenICamBase::EDeviceType_IEEE1394;
				break;

			case 3:
				eDeviceType = CDeviceGenICamBase::EDeviceType_USB;
				break;

			case 4:
				eDeviceType = CDeviceGenICamBase::EDeviceType_CameraLink;
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

		// 장치 찾기 방법을 선택합니다.
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

			// 연결되어 있는 카메라의 시리얼 번호를 얻는다.
			switch(eDeviceType)
			{
			case CDeviceGenICamBase::EDeviceType_GigE:
				drReturn = camiRAYPLE.GetAutoDetectGigECameraSerialNumbers(flarrSerialNumbers);
				break;

			case CDeviceGenICamBase::EDeviceType_IEEE1394:
				drReturn = camiRAYPLE.GetAutoDetectIEEE1394CameraSerialNumbers(flarrSerialNumbers);
				break;

			case CDeviceGenICamBase::EDeviceType_USB:
				drReturn = camiRAYPLE.GetAutoDetectUSBCameraSerialNumbers(flarrSerialNumbers);
				break;

			case CDeviceGenICamBase::EDeviceType_CameraLink:
				drReturn = camiRAYPLE.GetAutoDetectCameraLinkCameraSerialNumbers(flarrSerialNumbers);
				break;

			default:
				break;
			}

			if(drReturn.IsFail() || !flarrSerialNumbers.GetCount())
			{
				drReturn = EResult_FailedToRead;
				printf("Not Found Device.\n");
				break;
			}

			// 연결 할 카메라를 선택합니다.
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
			if(eDeviceType == CDeviceGenICamBase::EDeviceType_GigE)
			{
				// 연결 방법을 선택합니다.
				while(true)
				{
					printf("1. Serial Number\n");
					printf("2. IP Address\n");
					printf("Select Connection Method: ");
					fgetws(arrInput, 4096, stdin);

					CFLString<wchar_t> flsInput(arrInput);
					int32_t i32Select = flsInput.ToInt32();
					bool bSelected = true;

					switch(i32Select)
					{
					case 1:
						eConnectionMethod = CDeviceGenICamBase::EConnectionMethod_SerialNumber;
						break;

					case 2:
						eConnectionMethod = CDeviceGenICamBase::EConnectionMethod_IPAddress;
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
			}
			else
				eConnectionMethod = CDeviceGenICamBase::EConnectionMethod_SerialNumber;

			// 시리얼 번호 혹은 IP 주소를 입력 받는다.
			if(eConnectionMethod == CDeviceGenICamBase::EConnectionMethod_SerialNumber)
				printf("Input Serial Number: ");
			else
				printf("Input IP Address: ");

			fgetws(arrInput, 4096, stdin);

			flsConnection = arrInput;
			flsConnection.Replace(L"\n", L"");
		}

		// 이벤트를 받을 객체 선언
		CDeviceEventImageEx eventImage;

		// 카메라에 이벤트 객체 설정
		camiRAYPLE.RegisterDeviceEvent(&eventImage);

		// 카메라에 장치 타입 설정
		camiRAYPLE.SetDeviceType(eDeviceType);

		// 인덱스에 해당하는 카메라로 연결을 설정한다.
		if(bAutoDetect)
		{
			switch(eDeviceType)
			{
			case CDeviceGenICamBase::EDeviceType_GigE:
				drReturn = camiRAYPLE.AutoDetectGigECamera(i32SelectDevice);
				break;

			case CDeviceGenICamBase::EDeviceType_IEEE1394:
				drReturn = camiRAYPLE.AutoDetectIEEE1394Camera(i32SelectDevice);
				break;

			case CDeviceGenICamBase::EDeviceType_USB:
				drReturn = camiRAYPLE.AutoDetectUSBCamera(i32SelectDevice);
				break;

			case CDeviceGenICamBase::EDeviceType_CameraLink:
				drReturn = camiRAYPLE.AutoDetectCameraLinkCamera(i32SelectDevice);
				break;

			default:
				break;
			}
		}
		else
		{
			// 장치 연결 방법을 설정합니다.
			camiRAYPLE.SetConnectionMethod(eConnectionMethod);

			if(eConnectionMethod == CDeviceGenICamBase::EConnectionMethod_SerialNumber)
				// 시리얼 번호를 설정합니다.
				camiRAYPLE.SetSerialNumber(flsConnection);
			else
				// IP 주소를 설정합니다.
				camiRAYPLE.SetIPAddress(flsConnection);
		}

		// 카메라를 초기화 합니다.
		drReturn = camiRAYPLE.Initialize();
		if(drReturn.IsFail())
		{
			printf("Failed to initialize the camera.\n");
			break;
		}

		// 이미지 뷰 생성 // Create image view
		if(viewImage.Create(0, 0, 1000, 1000).IsFail())
		{
			drReturn = EResult_FailedToCreateObject;

			printf("Failed to create the image view.\n");
			break;
		}

		eventImage.SetViewImage(&viewImage);

		// 카메라를 Live 합니다.
		drReturn = camiRAYPLE.Live();
		if(drReturn.IsFail())
		{
			printf("Failed to live the camera.\n");
			break;
		}

		// 이미지 뷰가 종료될 때 까지 기다림 // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	// 카메라의 초기화를 해제합니다.
	camiRAYPLE.Terminate();
	camiRAYPLE.ClearDeviceEvents();

	if(drReturn.IsFail())
		getchar();

	return 0;
}