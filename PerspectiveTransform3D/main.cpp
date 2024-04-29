#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"

int main()
{
	// �̹��� �� ���� // Declare image view
	CGUIView3DWrap view3DSrc;
	CGUIView3DWrap view3DDst;
	CFL3DObject fl3DObjectSrc;
	CFL3DObject fl3DObjectDst;

	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult res = EResult_UnknownError;

		// Source 3D �̹��� �� ���� // Create the Source 3D image view
		if((res = view3DSrc.Create(100, 0, 600, 500)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		// Destination 3D �̹��� �� ���� // Create the destination 3D image view
		if((res = view3DDst.Create(600, 0, 1100, 500)).IsFail())
		{
			ErrorPrint(res, L"Failed to create the image view.\n");
			break;
		}

		fl3DObjectSrc.Load(L"../../ExampleImages/DistanceTransform3D/binary-vertex.ply");

		// Perspective Transform 3D ��ü ���� // Create Perspective Transform 3D object
		CPerspectiveTransform3D PerspectiveTransform3D;

		TPoint3<float> tpPosition = TPoint3<float>(2.0, 0.0f, 1.5f);
		TPoint3<float> tpDirection = TPoint3<float>(-1.0f, 0.0f, -1.0f);
		TPoint3<float> tpUpVector = TPoint3<float>(0.0f, 1.0f, 0.0f);

		// Source ��ü ���� // Set the source object
		PerspectiveTransform3D.SetSourceObject(&fl3DObjectSrc);
		// Destionation ��ü ���� // Set the Destionation object
		PerspectiveTransform3D.SetDestinationObject(&fl3DObjectDst);
		// ī�޶� ��ġ ���� // Set the camera position
		PerspectiveTransform3D.SetPosition(tpPosition);
		// ī�޶� ���� ���� // Set the camera direction
		PerspectiveTransform3D.SetDirection(tpDirection);
		// ī�޶� ���� Ÿ�� ���� // Set the camera direction type
		PerspectiveTransform3D.SetDirectionType(CPerspectiveTransform3D::EDirectionType_Decrement);
		// ī�޶� �� ���� ���� // Set the camera up vector
		PerspectiveTransform3D.SetUpVector(tpUpVector);

		// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
		if((res = PerspectiveTransform3D.Execute()).IsFail())
		{
			ErrorPrint(res, L"Failed to execute multiFocus.");
			break;
		}

		// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
		// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately
		CGUIView3DLayerWrap layer3DSrc = view3DSrc.GetLayer(0);
		CGUIView3DLayerWrap layer3DDst = view3DDst.GetLayer(0);

		// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
		layer3DSrc.Clear();
		layer3DDst.Clear();

		// Destination �̹����� ���� ���������� Zoom fit �� ���� ���÷��� �Ǵ� �̹��� ������ ȭ�鿡 �����ش�. // With the newly created Destination image, the image magnification displayed through Zoom fit is adjusted to the screen.
		view3DSrc.PushObject(fl3DObjectSrc);
		view3DSrc.ZoomFit();

		view3DDst.PushObject(fl3DObjectDst);
		view3DDst.ZoomFit();

		if((res = layer3DSrc.DrawTextCanvas(&CFLPointD(0, 0), L"Source Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		if((res = layer3DDst.DrawTextCanvas(&CFLPointD(0, 0), L"Destination Object", YELLOW, BLACK, 20)).IsFail())
		{
			ErrorPrint(res, L"Failed to draw text.\n");
			break;
		}

		// �̹��� �並 ���� �մϴ�. // Update image view
		view3DSrc.Invalidate(true);
		view3DDst.Invalidate(true);

		// �̹��� ��, 3D �䰡 ����� �� ���� ��ٸ�
		while(view3DSrc.IsAvailable() || view3DDst.IsAvailable())
			CThreadUtilities::Sleep(1);
	}
	while(false);

	return 0;
}