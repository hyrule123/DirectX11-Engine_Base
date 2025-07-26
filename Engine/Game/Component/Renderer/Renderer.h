#pragma once
#include <Engine/Game/Component/Component.h>

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

		NO_SERIALIZE_BINARY;
		NO_SERIALIZE_JSON;

	public:
		Renderer();

		Renderer(const Renderer& _other);

		virtual ~Renderer();

		virtual void final_update() override;

		void set_mesh(const s_ptr<Mesh>& _mesh) { m_mesh = _mesh; }
		s_ptr<Mesh> get_mesh() { return m_mesh; }

		//인스턴싱 구현 전까지는 일단 Material을 복사해서 사용
		void set_material(const s_ptr<Material>& mtrl);
		s_ptr<Material> set_material_mode(eMaterialMode _mode);

		s_ptr<Material> get_shared_material() {
			return m_shared_material;
		}
		s_ptr<Material> get_dynamic_material() {
			return m_dynamic_material;
		}
		s_ptr<Material> get_current_material() {
			return m_current_material;
		}

		bool is_render_ready() const { return (m_mesh && m_current_material); }

		void set_frustum_cull(bool _bFrustumCull) { m_bCullingEnable = _bFrustumCull; }
		bool is_frustum_cull_enabled() const { return m_bCullingEnable; }

	private:
		s_ptr<Mesh> m_mesh;
		s_ptr<Material> m_shared_material;
		s_ptr<Material> m_dynamic_material;
		s_ptr<Material> m_current_material;

		eMaterialMode m_material_mode;

		bool m_bCullingEnable;
	};

}
