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
        inline const std::string_view GetComponentName(uint32 _ComID);

    protected:
        ComponentManager();
        ~ComponentManager();

    private:
        std::unordered_map<std::string_view, std::function<std::unique_ptr<iComponent>()>> mUmapComConstructor;
        std::vector<std::string_view> mComNamesByID;
    };

    template <typename T>
    inline void ComponentManager::AddComponentConstructor(const std::string_view _strKey)
    {
        static_assert(std::is_base_of_v<iComponent, T>);
        uint32 ComIDIndex = iComponent::GetComponentTypeID<T>();
        if ((uint32)mComNamesByID.size() <= ComIDIndex)
        {
            mComNamesByID.resize(ComIDIndex + 1);
        }

        mComNamesByID[ComIDIndex] = _strKey;
        mUmapComConstructor.insert(std::make_pair(_strKey,
            []()->std::unique_ptr<iComponent>
            {
                std::unique_ptr<iComponent> com = std::make_unique<T>();
                com->SetComponentTypeID(iComponent::GetComponentTypeID<T>());
                return com;
            }
        ));
    }

    inline const std::string_view ComponentManager::GetComponentName(uint32 _ComID)
    {
        std::string_view retStr{};
        if (_ComID < (uint32)mComNamesByID.size())
        {
            retStr = mComNamesByID[_ComID];
        }
        return retStr;
    }




    template<typename T>
    inline const std::string_view ComponentManager::GetComponentName()
    {
        return GetComponentName(iComponent::GetComponentTypeID<T>());
    }
}



