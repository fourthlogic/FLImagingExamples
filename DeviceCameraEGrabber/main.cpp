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
	CResult res = EResult_UnknownError;

	// �̹��� �� ���� // Declare image view
	CGUIViewImageWrap viewImage;

	// EGrabber ī�޶� ����
	CDeviceCameraEGrabber camEGrabber;

	do
	{
		wchar_t arrInput[4096] = { 0, };

		CFLString<wchar_t> flsCamfilePath = L"";
		int32_t i32BoardIndex = 0;
		int32_t i32ModuleIndex = 0;

		CFLString<wchar_t> flsInput = L"";

		// ������ �ε����� �Է��մϴ�.
		printf("Enter board index: ");
		fgetws(arrInput, 4096, stdin);
		flsInput = arrInput;
		i32BoardIndex = flsInput.ToInt32();

		// ����� �ε����� �Է��մϴ�.
		printf("Enter module index: ");
		fgetws(arrInput, 4096, stdin);
		flsInput = arrInput;
		i32ModuleIndex = flsInput.ToInt32();

		printf("\n");

		// �̺�Ʈ�� ���� ��ü ����
		CDeviceEventImageEx eventImage;

		// ī�޶� �̺�Ʈ ��ü ����
		camEGrabber.RegisterDeviceEvent(&eventImage);

		// ī�޶� ��ġ ����
		camEGrabber.SetBoardIndex(i32BoardIndex);
		camEGrabber.SetModuleIndex(i32ModuleIndex);

		// ī�޶� �ʱ�ȭ �մϴ�.
		if(IsFail(res = camEGrabber.Initialize()))
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
		if(IsFail(res = camEGrabber.Live()))
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
	camEGrabber.Terminate();

	return 0;
}