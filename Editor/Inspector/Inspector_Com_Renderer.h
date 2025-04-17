#pragma once
#include "Editor/Inspector/Inspector_Component.h"

namespace core
{
	class Mesh;
	class Material;
}

namespace core::editor
{
	class Inspector_Com_Renderer : public Inspector_Component
	{
	public:
		Inspector_Com_Renderer();
		virtual ~Inspector_Com_Renderer();

		virtual void update() override;

		virtual void update_UI() override;
	
		void set_mesh(const std::string& _key_path);
		void set_material(const std::string& _key_path);

	private:
		std::weak_ptr<Mesh> m_mesh;
		std::weak_ptr<Material> m_material;
	};
}
