#pragma once

namespace FLImaging
{
	namespace GUI
	{
		class CPropertyView3DExamples : public CGUIPropertyMenuBase
		{
			SupportToDuplicateObject(CPropertyView3DExamples);

		public:
			CPropertyView3DExamples();
			virtual ~CPropertyView3DExamples();

			virtual const CResult ConfigureMenu() override;

			// PushObject 버튼 클릭 프로시저
			CPropertyItemButtonClickProcedure* PushObject();
			CPropertyItemButtonClickProcedure* FrustumCollisionTest();
			CPropertyItemButtonClickProcedure* ROIUtilities3DTest();
			CPropertyItemButtonClickProcedure* SetCameraParameters();

			const CResult PushObjectPLY(CGUIView3D* pView3D);
			const CResult PushObjectSphere(CGUIView3D* pView3D);
			const CResult PushObjectHeightMap(CGUIView3D* pView3D);
			const CResult PushObjectHeightMapAndTexture(CGUIView3D* pView3D);
			const CResult PushObjectLine(CGUIView3D* pView3D);
			const CResult PushObjectUnselectableAxis(CGUIView3D* pView3D);

			// Sphere 만드는 예제 // Example of creating a sphere
			const CResult GetSphereObject(CGUIView3DObject& meshSphere, float f32Radius, uint32_t sliceCount, uint32_t stackCount);

			// Arrow 만드는 예제 // Example of creating an Arrow
			const CResult GetArrowObject(CGUIView3DObject& mesh1, CGUIView3DObject& mesh2, const TPoint3<float>& tp3Origin, float f32Length, float f32ArrowRatio, float f32PipeRadius, float f32ArrowBottomRadius, COLORREF clr);

			DeclareGetClassType();
		};
	}
}