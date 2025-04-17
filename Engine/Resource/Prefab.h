#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/Util/Serialize/JsonSerializer.h"

namespace core
{
    class GameObject;

    BASE_RESOURCE(Prefab);
	class Prefab final
		: public Resource
	{
        CLASS_NAME(Prefab);
    public:
        Prefab();
        virtual ~Prefab();

        virtual eResult save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const override;
        virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

        //virtual eResult SaveJson(Json::Value* _pJVal) override;
        //virtual eResult LoadJson(const Json::Value* _pJVal) override;

        //DontDelete를 true로 설정할 시에는 prefab이 제거될 때 관리하는 게임오브젝트를 삭제하지 않음.
        inline void     RegisterPrefab(std::shared_ptr<GameObject>&& _pPrefab) noexcept { m_prefab = std::move(_pPrefab); }
        bool            IsAvailable() const { return nullptr != m_prefab; }
        std::shared_ptr<GameObject>     instantiate();

    private:
        std::shared_ptr<GameObject>  m_prefab;
	};
}


