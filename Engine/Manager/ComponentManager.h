#pragma once



#include "../CommonStruct.h"

#include "../Game/Component/BaseComponents.h"


#include <unordered_map>
#include <typeindex>
#include <string>
#include <functional>

namespace ehw
{
    class ComponentManager
    {

        friend class Application;
    public:
        template <typename T>
        static inline void AddComponentConstructor(const std::string_view _strKey);

        static std::shared_ptr<iComponent> GetNewComponent(const std::string_view _strKey);

        template <typename T>
        static const std::string_view GetComponentName();
        inline static const std::string_view GetComponentName(UINT32 _ComID);

        template <typename T>
        static inline UINT32 GetComponentTypeID();

        template <typename T>
        static inline constexpr eComponentCategory GetComponentCategory();

        template <typename T>
        static inline constexpr bool IsBaseComponent();

    private:
        static void Init();
        static void Release();

        static inline UINT32 GetNextComponentTypeID();

    private:
        static std::unordered_map<std::string_view, std::function<std::shared_ptr<iComponent>()>> mUmapComConstructor;
        static std::vector<std::string_view> mComNamesByID;
    };

    template <typename T>
    static inline void ComponentManager::AddComponentConstructor(const std::string_view _strKey)
    {
        static_assert(std::is_base_of_v<iComponent, T>);
        UINT32 ComIDIndex = ComponentManager::GetComponentTypeID<T>();
        if ((UINT32)mComNamesByID.size() <= ComIDIndex)
        {
            mComNamesByID.resize(mComNamesByID.size() * 2 + 1);
        }

        mComNamesByID[ComIDIndex] = _strKey;
        mUmapComConstructor.insert(std::make_pair(_strKey,
            []()->std::shared_ptr<iComponent>
            {
                std::shared_ptr<iComponent> com = std::make_shared<T>();
                com->SetComponentTypeID(ComponentManager::GetComponentTypeID<T>());
                return com;
            }
        ));
    }

    inline const std::string_view ComponentManager::GetComponentName(UINT32 _ComID)
    {
        std::string_view retStr{};
        if (_ComID < (UINT32)mComNamesByID.size())
        {
            retStr = mComNamesByID[_ComID];
        }
        return retStr;
    }

    template<typename T>
    inline UINT32 ComponentManager::GetComponentTypeID()
    {
        static_assert(std::is_base_of_v<iComponent, T>);
        static UINT32 typeId = GetNextComponentTypeID();
        return typeId;
    }

    inline UINT32 ComponentManager::GetNextComponentTypeID()
    {
        static UINT32 lastID = UINT32_MAX;
        return ++lastID;
    }
    template<typename T>
    static inline const std::string_view ComponentManager::GetComponentName()
    {
        return GetComponentName(GetComponentTypeID<T>());
    }


    template<typename T>
    inline constexpr eComponentCategory ComponentManager::GetComponentCategory()
    {
        if constexpr (std::is_base_of_v<iTransform, T>)
        {
            return eComponentCategory::Transform;
        }
        else if constexpr (std::is_base_of_v<iCollider, T>)
        {
            return eComponentCategory::Collider;
        }
        else if constexpr (std::is_base_of_v<iAnimator, T>)
        {
            return eComponentCategory::Animator;
        }
        else if constexpr (std::is_base_of_v<iLight, T>)
        {
            return eComponentCategory::Light;
        }
        else if constexpr (std::is_base_of_v<Com_Camera, T>)
        {
            return eComponentCategory::Camera;
        }
        else if constexpr (std::is_base_of_v<Com_AudioSource, T>)
        {
            return eComponentCategory::AudioSource;
        }
        else if constexpr (std::is_base_of_v<Com_AudioListener, T>)
        {
            return eComponentCategory::AudioListener;
        }
        else if constexpr (std::is_base_of_v<iRenderer, T>)
        {
            return eComponentCategory::Renderer;
        }
        else if constexpr (std::is_base_of_v<Com_BehaviorTree, T>)
        {
            return eComponentCategory::BehaviorTree;
        }
        else if constexpr (std::is_base_of_v<iScript, T>)
        {
            return eComponentCategory::Scripts;
        }

        return eComponentCategory::UNKNOWN;
    }

    template<typename T>
    inline constexpr bool ComponentManager::IsBaseComponent()
    {
        if constexpr (std::is_same_v<iComponent, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<iTransform, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<iCollider, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<iAnimator, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<iLight, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<Com_Camera, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<Com_AudioSource, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<Com_AudioListener, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<iRenderer, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<Com_BehaviorTree, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<iScript, T>)
        {
            return true;
        }

        return false;
    }
}



