#include <cstdio>
#include <FLImaging.h>
#include "../CommomHeader/ErrorPrint.h"
#include <map>

int main()
{
	CFL3DObject floSrc;
	CFL3DObject floDst;
	CGUIView3DWrap view3DSrc;
	CGUIView3DWrap view3DDst;

	do
	{
		// �˰��� ���� ��� // Algorithm execution result
		CResult res = EResult_UnknownError;

		// �̹��� �ε� // Load the image
		if((res = floSrc.Load(L"../../ExampleImages/ConvexHull3D/RandomPointsOnSphere.ply")).IsFail())
		{
			ErrorPrint(res, L"Failed to load the 3D Object.\n");
			break;
		}
		
		// CConvexHull3D ��ü ���� // Create CConvexHull3D object
		CConvexHull3D convexhull3D;


		{
			// �̹��� �� ���� // Create image view
			if((res = view3DSrc.Create(100, 0, 612, 512)).IsFail() ||
			   (res = view3DDst.Create(612, 0, 1124, 512)).IsFail())
			{
				ErrorPrint(res, "Failed to create 3D views.\n");
				break;
			}

			// ó���� 3D ��ü ����
			convexhull3D.SetSourceObject(floSrc);
			convexhull3D.SetDestinationObject(floDst);

			// �信 ǥ���� 3D ��ü �߰�
			view3DSrc.PushObject(floSrc);

			// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
			if((res = convexhull3D.Execute()).IsFail())
			{
				ErrorPrint(res, L"Failed to execute Convex Hull 3D.");
				break;
			}

			view3DDst.PushObject(floDst);

			// ȭ�鿡 ����ϱ� ���� Image View���� ���̾� 0���� ���� // Obtain layer 0 number from image view for display
			// �� ��ü�� �̹��� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to an image view and does not need to be released separately		
			CGUIView3DLayerWrap layerViewSrc = view3DSrc.GetLayer(0);
			CGUIView3DLayerWrap layerViewDst = view3DDst.GetLayer(0);

			// ������ Layer�� �׷��� �������� ���� // Clear the figures drawn on the existing layer
			layerViewSrc.Clear();

			// View ������ ���÷��� �Ѵ�. // Display view information
			if((res = layerViewSrc.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Source Object", YELLOW, BLACK, 20)).IsFail() ||
			   (res = layerViewDst.DrawTextCanvas(&CFLPoint<double>(0, 0), L"Destination Object", YELLOW, BLACK, 20)).IsFail())
			{
				ErrorPrint(res, L"Failed to draw text.\n");
				break;
			}

			// �Է� ���� ������ �̵�
			view3DSrc.ZoomFit();

			// �Է�, ��� ���� ������ ����
			view3DSrc.SynchronizePointOfView(&view3DDst);

			// �Է�, ��� �並 �ϳ��� â���� ���
			view3DSrc.SynchronizeWindow(&view3DDst);

			view3DSrc.Invalidate();
			view3DDst.Invalidate();

			// 3D �䰡 ����� ������ ��ٸ�
			while(view3DSrc.IsAvailable() && view3DDst.IsAvailable())
				CThreadUtilities::Sleep(1);
		}
	}
	while(false);

	return 0;
}