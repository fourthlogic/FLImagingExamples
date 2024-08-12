#include <cstdio>

#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h" 


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
			m_pViewImage = pViewImage;
	}

	// ī�޶󿡼� �̹��� ��� �� ȣ�� �Ǵ� �Լ�
	virtual void OnAcquisition(const CDeviceImageBase* pDeviceImage)
	{
		// �̹��� ���� ��ȿ���� Ȯ���Ѵ�.
		if(m_pViewImage && m_pViewImage->IsAvailable())
		{
			// ī�޶󿡼� ��� �� �̹����� ���´�.
			pDeviceImage->GetAcquiredImage(m_pImage);

			if(m_pViewImage->GetImagePtr() != m_pImage)
			{
				// �̹��� �信 �̹��� ������ ����
				m_pViewImage->SetImagePtr(m_pImage);
				m_pViewImage->ZoomFit();
			}

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
	CResult res = EResult_UnknownError;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	// Matrox ī�޶� ����
	CDeviceCameraMatrox camMatrox;

	do
	{
		wchar_t arrInput[4096] = { 0, };

		CFLString<wchar_t> flsCamfilePath = L"";
		CDeviceCameraMatrox::EDeviceType eDeviceType = CDeviceCameraMatrox::EDeviceType_Unknown;
		int32_t i32DeviceIndex = 0;
		int32_t i32ModuleIndex = 0;

		CFLString<wchar_t> flsInput = L"";

		// Cam file �� ��ü ��θ� �Է��մϴ�.
		printf("Enter camfile full path (e.g. C:/Camfile/AnyCamfile.cam): ");
		fgetws(arrInput, 4096, stdin);
		flsCamfilePath = arrInput;
		flsCamfilePath.Replace(L"\r", L"");
		flsCamfilePath.Replace(L"\n", L"");

		// ��ġ Ÿ���� �Է��մϴ�.
		printf("Device type\n");
		printf("1.Clarity UHD\t\t2.Concord POE\t\t3.GenTL\n");
		printf("4.GevIQ\t\t\t5.GigE\t\t\t6.Host\n");
		printf("7.Indio\t\t\t8.Iris GTX\t\t9.Morphis\n");
		printf("10.Radient eV-CXP\t11.Radient eV-CL\t12.Rapixo Pro CL\n");
		printf("13.Rapixo CXP\t\t14.Solios\t\t15.USB3\n");
		printf("Enter device type: ");
		fgetws(arrInput, 4096, stdin);
		flsInput = arrInput;
		eDeviceType = (CDeviceCameraMatrox::EDeviceType)flsInput.ToInt32();

		// ��ġ�� �ε����� �Է��մϴ�.
		printf("Enter device index: ");
		fgetws(arrInput, 4096, stdin);
		flsInput = arrInput;
		i32DeviceIndex = flsInput.ToInt32();

		// ����� �ε����� �Է��մϴ�.
		printf("Enter module index: ");
		fgetws(arrInput, 4096, stdin);
		flsInput = arrInput;
		i32ModuleIndex = flsInput.ToInt32();

		printf("\n");

		// �̺�Ʈ�� ���� ��ü ����
		CDeviceEventImageEx eventImage;

		// ī�޶� �̺�Ʈ ��ü ����
		camMatrox.RegisterDeviceEvent(&eventImage);

		// ī�޶� ��ġ ����
		camMatrox.SetCamFilePath(flsCamfilePath);
		camMatrox.SetDeviceType(eDeviceType);
		camMatrox.SetDeviceIndex(i32DeviceIndex);
		camMatrox.SetModuleIndex(i32ModuleIndex);

		// ī�޶� �ʱ�ȭ �մϴ�.
		if(IsFail(res = camMatrox.Initialize()))
		{
			ErrorPrint(res, "Failed to initialize the camera.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(res = viewImage.Create(0, 0, 1000, 1000)))
		{
			ErrorPrint(res, "Failed to create the image view.\n");
			break;
		}

		eventImage.SetViewImage(&viewImage);

		// ī�޶� Live �մϴ�.
		if(IsFail(res = camMatrox.Live()))
		{
			ErrorPrint(res, "Failed to live the camera.\n");
			break;
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	// ī�޶��� �ʱ�ȭ�� �����մϴ�.
	camMatrox.Terminate();

	return 0;
}