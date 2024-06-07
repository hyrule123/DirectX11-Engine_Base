#pragma once
#include "Engine/Common.h"

#include "Engine/Scene/Component/BaseComponents.h"
#include "Engine/Util/StaticSingleton.h"

#include <unordered_map>
#include <typeindex>
#include <string>
#include <functional>

namespace ehw
{
    class ComponentManager : public StaticSingleton<ComponentManager>
    {
        friend class StaticSingleton<ComponentManager>;
        friend class GameEngine;
    public:
        void Init();
        void Release();

        template <typename T>
        inline void AddComponentConstructor(const std::string_view _strKey);

        std::unique_ptr<iComponent> GetNewComponent(const std::string_view _strKey);

        template <typename T>
        const std::string_view GetComponentName();
        inline const std::string_view GetComponentName(size_t _ComID);

    protected:
        ComponentManager();
        ~ComponentManager();

    private:
        size_t m_curComponentID;

        struct ClassInfo {
            std::string_view name;
            std::function<std::unique_ptr<iComponent>()> ctor;
        };
        //Idx: ID, data: 이름과 생성자 호출 함수
        std::vector<ClassInfo> m_comIDtoInfo;
        std::unordered_map<std::string_view, size_t, tHasher_StringView, std::equal_to<>> m_comNamesToID;
    };

    template <typename T>
    inline void ComponentManager::AddComponentConstructor(const std::string_view _strKey)
    {
        static_assert(std::is_base_of_v<iComponent, T>);
        size_t id = iComponent::GetComponentTypeID<T>();
        ASSERT(id == m_comIDtoInfo.size(), "id와 vector index 불일치");

        //name, ctor
        m_comIDtoInfo.push_back({ 
            _strKey,
            []()->std::unique_ptr<iComponent>
            {
                std::unique_ptr<iComponent> com = std::make_unique<T>();
                com->SetComponentTypeID(iComponent::GetComponentTypeID<T>());
                return com;
            }
            });

        m_comNamesToID.insert({ _strKey, id });
    }

    inline const std::string_view ComponentManager::GetComponentName(size_t _ComID)
    {
        std::string_view retStr{};
        if (_ComID < m_comIDtoInfo.size())
        {
            retStr = m_comIDtoInfo[_ComID].name;
        }
        return retStr;
    }

    template<typename T>
    inline const std::string_view ComponentManager::GetComponentName()
    {
        return GetComponentName(iComponent::GetComponentTypeID<T>());
    }
}



