#pragma once
#include "Editor/Inspector/Inspector_Component.h"

namespace ehw
{
	class Mesh;
	class Material;
}

namespace ehw::editor
{
	class Inspector_Com_Renderer : public Inspector_Component
	{
	public:
		Inspector_Com_Renderer();
		virtual ~Inspector_Com_Renderer();

		virtual void Update() override;

		virtual void UpdateUI() override;
	
		void SetMesh(const std::string& _key_path);
		void SetMaterial(const std::string& _key_path);

	private:
		Mesh* mMesh;
		Material* mMaterial;
	};
}
