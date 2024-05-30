#pragma once
#include "Engine/Scene/Component/iComponent.h"

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

	class iRenderer 
		: public Component<iRenderer, eComponentCategory::Renderer>
	{
	public:
		iRenderer();

		iRenderer(const iRenderer& _other);

		virtual ~iRenderer();

		//virtual void Init() override;
		//virtual void Update() override;
		virtual void FinalUpdate() override;
		virtual void Render() = 0;

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

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


	inline Material* iRenderer::GetCurrentMaterial(UINT _idx)
	{
		Material* retMtrl = nullptr;
		if ((UINT)m_materials.size() > _idx)
		{
			retMtrl = m_materials[_idx].CurrentMaterial;
		}
		return retMtrl;
	}

	inline std::shared_ptr<Material> iRenderer::GetSharedMaterial(UINT _idx)
	{
		std::shared_ptr<Material> retMtrl = nullptr;
		if ((UINT)m_materials.size() > _idx)
		{
			retMtrl = m_materials[_idx].SharedMaterial;
		}
		return retMtrl;
	}

	inline Material* iRenderer::GetDynamicMaterial(UINT _idx)
	{
		Material* retMtrl = nullptr;
		if ((UINT)m_materials.size() > _idx)
		{
			retMtrl = m_materials[_idx].DynamicMaterial.get();
		}
		return retMtrl;
	}


	inline tMaterialSet* iRenderer::GetMaterialSet(UINT _idx)
	{
		tMaterialSet* mtrlSet = nullptr;
		if ((UINT)m_materials.size() > _idx)
		{
			mtrlSet = &m_materials[_idx];
		}
		return mtrlSet;
	}

}
