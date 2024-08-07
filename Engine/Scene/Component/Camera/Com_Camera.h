#pragma once
#include "Engine/Scene/Component/Component.h"

#include "Engine/define_Enum.h"

#include <bitset>

namespace  ehw
{
	class Renderer;
	class SceneRenderer;
	class Com_Camera 
		: public Component<Com_Camera, eComponentCategory::Camera>
	{
		class CullingAgent;
	public:
		Com_Camera();
		virtual ~Com_Camera();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		__forceinline static const MATRIX& GetGpuViewMatrix() { return s_viewMatrix; }
		__forceinline static const MATRIX& GetGpuViewInvMatrix() { return s_viewInverseMatrix; }
		__forceinline static const MATRIX& GetGpuProjectionMatrix() { return s_projectionMatrix; }
		__forceinline static void SetGpuViewMatrix(const MATRIX& _view) { s_viewMatrix = _view; }
		__forceinline static void SetGpuProjectionMatrix(const MATRIX& _projection) { s_projectionMatrix = _projection; }

		void Init() override;
		void FinalUpdate() override;
		void FrameEnd() override;
		void OnDisable() override;
		void OnDestroy() override;

		void RenderCamera(SceneRenderer* _sceneRenderer);

		void CreateViewMatrix();

		void TurnLayerMask(uint32 _layer, bool _enable = true);
		void EnableLayerMasks() { m_layerMasks.set(); }
		void DisableLayerMasks() { m_layerMasks.reset(); }

		void SetProjectionType(eProjectionType _type) { m_projectionType = _type; CreateProjectionMatrix(); }
		eProjectionType GetProjectionType() const { return m_projectionType; }

		inline void SetCullEnable(bool _bCullingEnable);
		bool IsCullEnabled() const { return m_isEnableCulling; }

		void CreateProjectionMatrix();
		void CreateProjectionMatrix(uint ResolutionX, uint ResolutionY);
		
		void SetScale(float _scale);
		float GetScale() const { return m_scale; }
		const MATRIX& GetViewMatrix() const { return m_viewMatrix; }
		const MATRIX& GetProjectionMatrix() const { return m_projectionMatrix; }

	private:
		void SortGameObjects();
		void RenderDeffered();
		void RenderForwardOpaque();
		void RenderCutout();
		void RenderTransparent();
		void RenderPostProcess();
		void PushGameObjectToRenderingModes(const std::shared_ptr<GameObject>& _gameObj);
		void SortRenderersByMode();

	private:
		static MATRIX s_viewMatrix;
		static MATRIX s_viewInverseMatrix;
		static MATRIX s_projectionMatrix;

		MATRIX m_viewMatrix;
		MATRIX m_viewInverse;
		MATRIX m_projectionMatrix;

		eProjectionType m_projectionType;
		std::unique_ptr<CullingAgent> m_cullingAgent;
		bool m_isEnableCulling;

		float m_aspectRation;

		float m_nearDistance;
		float m_farDistance;
		float m_scale;

		std::bitset<g_maxLayer> m_layerMasks;
		std::vector<Renderer*> m_defferedOpaque;
		std::vector<Renderer*> m_forwardOpaque;
		std::vector<Renderer*> m_alphaTest;		//CutOut
		std::vector<Renderer*> m_alphaBlend;	//Transparent
		std::vector<Renderer*> m_postProcess;


		class CullingAgent : public Entity
		{
		public:
			CullingAgent() {};
			virtual ~CullingAgent() {};

			virtual void FinalUpdate() = 0;

		};

		class CullingAgent_Orthographic : public CullingAgent
		{
		public:
			CullingAgent_Orthographic();
			virtual ~CullingAgent_Orthographic();

			virtual void FinalUpdate() { ASSERT(false, "미구현"); }
		};

		class CullingAgent_Perspective : public CullingAgent
		{
		public:
			CullingAgent_Perspective();
			virtual ~CullingAgent_Perspective();

			virtual void FinalUpdate() { ASSERT(false, "미구현"); }
		};
	};

}
