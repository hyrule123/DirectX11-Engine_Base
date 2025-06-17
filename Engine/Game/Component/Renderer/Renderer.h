#pragma once
#include "Engine/Game/Component/Component.h"

namespace core
{
	class Mesh;
	class Material;
	enum class eMaterialMode
	{
		Shared,
		Dynamic
	};

	class Renderer 
		: public Component
	{
		CLASS_INFO(Renderer, Component);
		BASE_COMPONENT(eComponentOrder::Renderer);

	public:
		Renderer(const std::string_view key);

		Renderer(const Renderer& _other);

		virtual ~Renderer();

		virtual void final_update() override;

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		void set_mesh(const s_ptr<Mesh>& _mesh) { m_mesh = _mesh; }
		s_ptr<Mesh> GetMesh() { return m_mesh; }

		//인스턴싱 구현 전까지는 일단 Material을 복사해서 사용
		void set_material(const s_ptr<Material>& mtrl);
		s_ptr<Material> SetMaterialMode(eMaterialMode _mode);

		s_ptr<Material> GetSharedMaterial() {
			return m_shared_material;
		}
		s_ptr<Material> GetDynamicMaterial() {
			return m_dynamic_material;
		}
		s_ptr<Material> GetCurrentMaterial() {
			return m_current_material;
		}

		bool IsRenderReady() const { return (m_mesh && m_current_material); }

		void SetCullingEnable(bool _bFrustumCull) { m_bCullingEnable = _bFrustumCull; }
		bool IsCullingEnabled() const { return m_bCullingEnable; }

	private:
		s_ptr<Mesh> m_mesh;
		s_ptr<Material> m_shared_material;
		s_ptr<Material> m_dynamic_material;
		s_ptr<Material> m_current_material;

		eMaterialMode m_material_mode;

		bool m_bCullingEnable;
	};

}
