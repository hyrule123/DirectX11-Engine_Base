#pragma once
#include "Engine/Game/Component/Component.h"

namespace ehw
{
	class Mesh;
	class Material;
	enum class eMaterialMode
	{
		Shared,
		Dynamic
	};
	struct tMaterialSet
	{
		std::shared_ptr<Material> SharedMaterial;
		std::unique_ptr<Material> DynamicMaterial;
		Material* CurrentMaterial;

		eMaterialMode MaterialMode;
	};

	class Renderer 
		: public Component<Renderer, eComponentCategory::Renderer>
	{
	public:
		Renderer(const std::string_view key);

		Renderer(const Renderer& _other);

		virtual ~Renderer();

		virtual void final_update() override;

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		void SetMesh(const std::shared_ptr<Mesh> _mesh);

		//인스턴싱 구현 전까지는 일단 Material을 복사해서 사용
		void SetMaterial(const std::shared_ptr<Material>& _Mtrl, UINT _idx);
		Material* SetMaterialMode(UINT _idx, eMaterialMode _mode);

		inline std::shared_ptr<Mesh> GetMesh() { return m_mesh; }

		std::shared_ptr<Material> GetSharedMaterial(UINT _idx);
		inline Material* GetDynamicMaterial(UINT _idx);
		inline Material* GetCurrentMaterial(UINT _idx);

		UINT GetMaterialCount() { return (UINT)m_materials.size(); }
		bool IsRenderReady() const { return (m_mesh && false == m_materials.empty()); }

		void SetCullingEnable(bool _bFrustumCull) { m_bCullingEnable = _bFrustumCull; }
		bool IsCullingEnabled() const { return m_bCullingEnable; }

	private:
		inline tMaterialSet* GetMaterialSet(UINT _idx);

	private:
		std::shared_ptr<Mesh> m_mesh;
		std::vector<tMaterialSet> m_materials;

		bool m_bCullingEnable;
	};


	inline Material* Renderer::GetCurrentMaterial(UINT _idx)
	{
		Material* retMtrl = nullptr;
		if ((UINT)m_materials.size() > _idx)
		{
			retMtrl = m_materials[_idx].CurrentMaterial;
		}
		return retMtrl;
	}

	inline std::shared_ptr<Material> Renderer::GetSharedMaterial(UINT _idx)
	{
		std::shared_ptr<Material> retMtrl = nullptr;
		if ((UINT)m_materials.size() > _idx)
		{
			retMtrl = m_materials[_idx].SharedMaterial;
		}
		return retMtrl;
	}

	inline Material* Renderer::GetDynamicMaterial(UINT _idx)
	{
		Material* retMtrl = nullptr;
		if ((UINT)m_materials.size() > _idx)
		{
			retMtrl = m_materials[_idx].DynamicMaterial.get();
		}
		return retMtrl;
	}


	inline tMaterialSet* Renderer::GetMaterialSet(UINT _idx)
	{
		tMaterialSet* mtrlSet = nullptr;
		if ((UINT)m_materials.size() > _idx)
		{
			mtrlSet = &m_materials[_idx];
		}
		return mtrlSet;
	}

}
