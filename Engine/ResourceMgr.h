#pragma once
#include "define_Res.h"
#include "define_Struct.h"
#include "iResource.h"

#include "Mesh.h"
#include "MeshData.h"

#include "Texture.h"
#include "Material.h"

#include "AudioClip.h"
#include "Prefab.h"

#include "GraphicsShader.h"
#include "iComputeShader.h"

#include "iScript.h"


namespace ehw
{
	class ResourceMgr
	{
		friend class Application;
	public:
		template <typename T>
		static eResourceType GetResType();

		template <typename T>
		static std::shared_ptr<T> Load(const std::filesystem::path& _filePath);

		template <typename T>
		static std::shared_ptr<T> Find(const std::string_view _strKey);

		static std::shared_ptr<iResource> Find(eResourceType _ResType, const std::string_view _strKey);

		template <typename T>
		static const std::unordered_map<std::string, std::shared_ptr<iResource>, tHashFunc_StringView, std::equal_to<>>&
			GetResources();

		static void Insert(const std::string_view _strKey, std::shared_ptr<iResource> _Res);
		
	private:
		static void Init();
		static void Release();
		
	
	private:
		static std::unordered_map<std::string, std::shared_ptr<iResource>, tHashFunc_StringView, std::equal_to<>> mArrRes[(int)eResourceType::END];
	};


	template<typename T>
	inline eResourceType ResourceMgr::GetResType()
	{
		static_assert(std::is_base_of_v<iResource, T>);

		if constexpr (std::is_same_v<Mesh, T>)
		{
			return eResourceType::Mesh;
		}
		else if constexpr (std::is_same_v<MeshData, T>)
		{
			return eResourceType::MeshData;
		}
		else if constexpr (std::is_same_v<Texture, T>)
		{
			return eResourceType::Texture;
		}
		else if constexpr (std::is_same_v<Material, T>)
		{
			return eResourceType::Material;
		}
		else if constexpr (std::is_same_v<AudioClip, T>)
		{
			return eResourceType::AudioClip;
		}
		else if constexpr (std::is_same_v<Prefab, T>)
		{
			return eResourceType::Prefab;
		}
		else if constexpr (std::is_same_v<GraphicsShader, T>)
		{
			return eResourceType::GraphicsShader;
		}

		//컴퓨트쉐이더는 상속관계를 가질수 있으므로 is_base_of_v를 사용.
		else if constexpr (std::is_base_of_v<iComputeShader, T>)
		{
			return eResourceType::ComputeShader;
		}
		
		return eResourceType::END;
	}

	template<typename T>
	inline std::shared_ptr<T> ResourceMgr::Load(const std::filesystem::path& _filePath)
	{
		//IRes를 상속받는 클래스가 아닐 경우 컴파일 중지
		static_assert(std::is_base_of<iResource, T>::value);

		if (_filePath.empty())
		{
			return nullptr;
		}

		std::string strKey = _filePath.string();

		std::shared_ptr<iResource> FindRes = Find(GetResType<T>(), strKey);

		// 이미 해당 키로 리소스가 있다면, 캐스팅 해서 반환
		if (FindRes)
			return std::static_pointer_cast<T>(FindRes);

		std::shared_ptr<T> NewRes = std::make_shared<T>();

		//Load 실패 시 nullptr 반환
		if (eResultFail(NewRes->Load(_filePath)))
			return nullptr;


		Insert(NewRes->GetStrKey(), NewRes);

		return NewRes;
	}

	template<typename T>
	inline std::shared_ptr<T> ResourceMgr::Find(const std::string_view _strKey)
	{
		eResourceType ResType = GetResType<T>();

		if (eResourceType::UNKNOWN == ResType)
			return nullptr;

		const auto& iter = mArrRes[(int)ResType].find(_strKey);
		
		if (iter == mArrRes[(int)ResType].end())
			return nullptr;

		//컴퓨트쉐이더는 상속구조를 가질수 있으므로 실제 타입의 ID를 비교한 후, 다르면 nullptr 반환
		if constexpr (std::is_base_of_v<iComputeShader, T>)
		{
			if (typeid(T) != std::static_pointer_cast<iComputeShader>(iter->second)->GetLeafTypeID())
			{
				return nullptr;
			}
		}

		return std::static_pointer_cast<T, iResource>(iter->second);
	}

	template<typename T>
	inline const std::unordered_map<std::string, std::shared_ptr<iResource>, tHashFunc_StringView, std::equal_to<>>& ResourceMgr::GetResources()
	{
		eResourceType Type = GetResType<T>();
		assert(eResourceType::UNKNOWN != Type);

		return mArrRes[(int)Type];
	}



	inline std::shared_ptr<iResource> ResourceMgr::Find(eResourceType _ResType, const std::string_view _strKey)
	{
		if (eResourceType::UNKNOWN == _ResType)
			return nullptr;

		const auto& iter = mArrRes[(int)_ResType].find(_strKey);

		if (iter == mArrRes[(int)_ResType].end())
			return nullptr;

		return iter->second;
	}

	inline void ResourceMgr::Insert(const std::string_view _strKey, std::shared_ptr<iResource> _Res)
	{
		eResourceType ResType = _Res->GetResType();

		assert(nullptr == Find(ResType, _strKey));

		_Res->SetStrKey(_strKey);

		mArrRes[(int)ResType].insert(std::make_pair(std::string(_strKey), _Res));
	}



}
