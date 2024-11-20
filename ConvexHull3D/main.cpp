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

			// ó���� 3D ��ü ���� // Set 3D object to process
			convexhull3D.SetSourceObject(floSrc);
			convexhull3D.SetDestinationObject(floDst);
			
			// ������ ���ϲ��� ��ü�� �÷��� ���� // Set color of new 3D object
			convexhull3D.EnableVertexRecoloring(true);
			convexhull3D.SetTargetVertexColor(TPoint3<uint8_t>(255, 125, 0));

			// 3D �信 ǥ���� 3D ��ü �߰� // Add 3D object to the 3D view
			view3DSrc.PushObject(floSrc);
			view3DSrc.SetPointSize(3);

			// �ռ� ������ �Ķ���� ��� �˰��� ���� // Execute algorithm according to previously set parameters
			if((res = convexhull3D.Execute()).IsFail())
			{
				ErrorPrint(res, L"Failed to execute Convex Hull 3D.");
				break;
			}

			// 3D �信 ǥ���� 3D ��ü �߰� // Add 3D object to the 3D view
			view3DDst.PushObject(floDst);

			// ȭ�鿡 ����ϱ� ���� View���� ���̾� 0���� ���� // Obtain layer number 0 from view for display
			// �� ��ü�� �信 �����ֱ� ������ ���� ������ �ʿ䰡 ���� // This object belongs to a view and does not need to be released separately		
			CGUIView3DLayerWrap layerViewSrc = view3DSrc.GetLayer(0);
			CGUIView3DLayerWrap layerViewDst = view3DDst.GetLayer(0);

			// View ������ ���÷��� �Ѵ�. // Display view information
			CFLPoint<double> flpTopLeft(0, 0);
			if((res = layerViewSrc.DrawTextCanvas(&flpTopLeft, L"Source Object(Point Cloud)", YELLOW, BLACK, 20)).IsFail() ||
			   (res = layerViewDst.DrawTextCanvas(&flpTopLeft, L"Destination Object(Convex Hull)", YELLOW, BLACK, 20)).IsFail())
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