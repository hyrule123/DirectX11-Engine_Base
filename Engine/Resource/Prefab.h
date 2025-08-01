#pragma once
#include <Engine/Resource/Resource.h>

#include <Engine/Util/Serialize/JsonSerializer.h>

namespace core
{
    class GameObject;

    BASE_RESOURCE(Prefab);
	class Prefab final
		: public Resource
	{
        CLASS_INFO(Prefab, Resource);
    public:
        Prefab();
        virtual ~Prefab();

        virtual eResult save(const std::fs::path& _base_directory) const override;
        virtual eResult load(const std::fs::path& _base_directoryb) override;

        //virtual eResult SaveJson(Json::Value* _pJVal) override;
        //virtual eResult LoadJson(const Json::Value* _pJVal) override;

        //DontDelete를 true로 설정할 시에는 prefab이 제거될 때 관리하는 게임오브젝트를 삭제하지 않음.
        inline void     register_prefab(s_ptr<GameObject>&& _pPrefab) noexcept { m_prefab = std::move(_pPrefab); }
        bool            is_available() const { return nullptr != m_prefab; }
        s_ptr<GameObject>     instantiate();

    private:
        s_ptr<GameObject>  m_prefab;
	};
}


