#pragma once
#include "../iComponent.h"



#include "../../../Resource/Mesh.h"
#include "../../../Resource/Material.h"

namespace ehw
{
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
		virtual void InternalUpdate() override {};
		virtual void Render() = 0;

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;

		void SetMesh(const std::shared_ptr<Mesh> _mesh);

		//인스턴싱 구현 전까지는 일단 Material을 복사해서 사용
		inline void SetMaterial(const std::shared_ptr <Material> _Mtrl, UINT _idx);
		Material* SetMaterialMode(UINT _idx, eMaterialMode _mode);

		std::shared_ptr<Mesh> GetMesh() { return mMesh; }

		inline std::shared_ptr<Material> GetSharedMaterial(UINT _idx);
		inline Material* GetDynamicMaterial(UINT _idx);
		inline Material* GetCurrentMaterial(UINT _idx);

		UINT GetMaterialCount() { return (UINT)mMaterials.size(); }
		bool IsRenderReady() const { return (mMesh && false == mMaterials.empty()); }

		void SetCullingEnable(bool _bFrustumCull) { mbCullingEnable = _bFrustumCull; }
		bool IsCullingEnabled() const { return mbCullingEnable; }

	private:
		inline tMaterialSet* GetMaterialSet(UINT _idx);

	private:
		std::shared_ptr<Mesh> mMesh;
		std::vector<tMaterialSet> mMaterials;

		bool mbCullingEnable;
	};

	inline void iRenderer::SetMaterial(const std::shared_ptr<Material> _Mtrl, UINT _idx)
	{
		if ((UINT)mMaterials.size() <= _idx)
			mMaterials.resize(_idx + 1u);

		mMaterials[_idx] = {};
		mMaterials[_idx].SharedMaterial = _Mtrl;
		mMaterials[_idx].CurrentMaterial = mMaterials[_idx].SharedMaterial.get();
	}



	inline Material* iRenderer::GetCurrentMaterial(UINT _idx)
	{
		Material* retMtrl = nullptr;
		if ((UINT)mMaterials.size() > _idx)
		{
			retMtrl = mMaterials[_idx].CurrentMaterial;
		}

		return retMtrl;
	}

	inline std::shared_ptr<Material> iRenderer::GetSharedMaterial(UINT _idx)
	{
		std::shared_ptr<Material> retMtrl = nullptr;
		if ((UINT)mMaterials.size() > _idx)
		{
			retMtrl = mMaterials[_idx].SharedMaterial;
		}
		return retMtrl;
	}

	inline Material* iRenderer::GetDynamicMaterial(UINT _idx)
	{
		Material* retMtrl = nullptr;
		if ((UINT)mMaterials.size() > _idx)
		{
			retMtrl = mMaterials[_idx].DynamicMaterial.get();
		}
		return retMtrl;
	}


	inline tMaterialSet* iRenderer::GetMaterialSet(UINT _idx)
	{
		tMaterialSet* mtrlSet = nullptr;
		if ((UINT)mMaterials.size() > _idx)
		{
			mtrlSet = &mMaterials[_idx];
		}
		return mtrlSet;
	}

}
