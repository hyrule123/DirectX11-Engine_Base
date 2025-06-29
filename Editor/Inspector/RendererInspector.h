#pragma once
#include "Editor/Inspector/ComponentInspector.h"

namespace core
{
	class Mesh;
	class Material;
}

namespace core::editor
{
	class RendererInspcetor : public ComponentInspector
	{
	public:
		RendererInspcetor();
		virtual ~RendererInspcetor();

		virtual void update() override;

		virtual void update_UI() override;
	
		void set_mesh(const std::string& _key_path);
		void set_material(const std::string& _key_path);

	private:
		w_ptr<Mesh> m_mesh;
		w_ptr<Material> m_material;
	};
}
