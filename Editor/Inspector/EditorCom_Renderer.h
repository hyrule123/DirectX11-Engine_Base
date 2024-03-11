#pragma once
#include "Inspector/EditorComponent.h"

#include "Resource/Material.h"
#include "Resource/Mesh.h"

namespace editor
{
	class EditorCom_Renderer : public EditorComponent
	{
	public:
		EditorCom_Renderer();
		virtual ~EditorCom_Renderer();

		virtual void Update() override;

		virtual void UpdateUI() override;
	
		void SetMesh(const std::string& _strKey);
		void SetMaterial(const std::string& _strKey);

	private:
		ehw::Mesh* mMesh;
		ehw::Material* mMaterial;
	};
}
