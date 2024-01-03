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
	CResult eResult = EResult_UnknownError;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	// Multicam ī�޶� ����
	CDeviceCameraMulticam camMulticam;

	do
	{
		wchar_t arrInput[4096] = { 0, };

		CFLString<wchar_t> flsCamfilePath = L"";
		int32_t i32BoardIndex = 0;
		CDeviceCameraMulticam::EBoardTopology eBoardTopology = CDeviceCameraMulticam::EBoardTopology_Mono;

		CFLString<wchar_t> flsInput = L"";

		// Cam file �� ��ü ��θ� �Է��մϴ�.
		printf("Enter camfile full path (e.g. C:/Camfile/AnyCamfile.cam): ");
		fgetws(arrInput, 4096, stdin);
		flsCamfilePath = arrInput;
		flsCamfilePath.Replace(L"\r", L"");
		flsCamfilePath.Replace(L"\n", L"");

		// ������ �ε����� �Է��մϴ�.
		printf("Enter board index: ");
		fgetws(arrInput, 4096, stdin);
		flsInput = arrInput;
		i32BoardIndex = flsInput.ToInt32();

		// ������ Topology�� �����մϴ�.
		while(true)
		{
			printf("1. Mono\n");
			printf("2. Mono deca\n");
			printf("3. Mono slow\n");
			printf("Select board topology: ");
			fgetws(arrInput, 4096, stdin);
			
			flsInput = arrInput;
			int32_t i32Select = flsInput.ToInt32();
			bool bSelected = true;

			switch(i32Select)
			{
			case 1:
				eBoardTopology = CDeviceCameraMulticam::EBoardTopology_Mono;
				break;

			case 2:
				eBoardTopology = CDeviceCameraMulticam::EBoardTopology_MonoDeca;
				break;

			case 3:
				eBoardTopology = CDeviceCameraMulticam::EBoardTopology_MonoSlow;
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

		// �̺�Ʈ�� ���� ��ü ����
		CDeviceEventImageEx eventImage;

		// ī�޶� �̺�Ʈ ��ü ����
		camMulticam.RegisterDeviceEvent(&eventImage);

		// ī�޶� ��ġ ����
		camMulticam.SetCamFilePath(flsCamfilePath);
		camMulticam.SetBoardIndex(i32BoardIndex);
		camMulticam.SetBoardTopology(eBoardTopology);

		// ī�޶� �ʱ�ȭ �մϴ�.
		if(IsFail(eResult = camMulticam.Initialize()))
		{
			ErrorPrint(eResult, "Failed to initialize the camera.\n");
			break;
		}

		// �̹��� �� ���� // Create image view
		if(IsFail(eResult = viewImage.Create(0, 0, 1000, 1000)))
		{
			ErrorPrint(eResult, "Failed to create the image view.\n");
			break;
		}

		eventImage.SetViewImage(&viewImage);

		// ī�޶� Live �մϴ�.
		if(IsFail(eResult = camMulticam.Live()))
		{
			ErrorPrint(eResult, "Failed to live the camera.\n");
			break;
		}

		// �̹��� �䰡 ����� �� ���� ��ٸ� // Wait for the image view to close
		while(viewImage.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	// ī�޶��� �ʱ�ȭ�� �����մϴ�.
	camMulticam.Terminate();

	return 0;
}