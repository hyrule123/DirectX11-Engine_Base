#pragma once
#include <unordered_map>
#include <typeindex>
#include <string>
#include <functional>
#include "define_Struct.h"
#include "define_Component.h"
#include "Components.h"

namespace ehw
{
    class ComMgr
    {

        friend class Application;
    public:
        template <typename T>
        static inline void AddComConstructor(const std::string_view _strKey);

        static IComponent* GetNewCom(const std::string_view _strKey);

        template <typename T>
        static const std::string_view GetComName();
        inline static const std::string_view GetComName(UINT32 _ComID);

        template <typename T>
        static inline UINT32 GetComTypeID();

        template <typename T>
        static inline constexpr eComponentCategory GetComponentCategory();

        template <typename T>
        static inline constexpr bool IsBaseComponent();

    private:
        static void Init();
        static void Release();

        static inline UINT32 GetNextComTypeID();

    private:
        static std::unordered_map<std::string_view, std::function<IComponent* ()>> mUmapComConstructor;
        static std::vector<std::string_view> mComNamesByID;
    };

    template <typename T>
    static inline void ComMgr::AddComConstructor(const std::string_view _strKey)
    {
        static_assert(std::is_base_of_v<IComponent, T>);
        UINT32 ComIDIndex = ComMgr::GetComTypeID<T>();
        if ((UINT32)mComNamesByID.size() <= ComIDIndex)
        {
            mComNamesByID.resize(mComNamesByID.size() * 2 + 1);
        }

        mComNamesByID[ComIDIndex] = _strKey;
        mUmapComConstructor.insert(std::make_pair(_strKey,
            []()->T*
            {
                T* com = new T;
                com->SetComTypeID(ComMgr::GetComTypeID<T>());
                return com;
            }
        ));
    }

    inline const std::string_view ComMgr::GetComName(UINT32 _ComID)
    {
        std::string_view retStr{};
        if (_ComID < (UINT32)mComNamesByID.size())
        {
            retStr = mComNamesByID[_ComID];
        }
        return retStr;
    }

    template<typename T>
    inline UINT32 ComMgr::GetComTypeID()
    {
        static_assert(std::is_base_of_v<IComponent, T>);
        static UINT32 typeId = GetNextComTypeID();
        return typeId;
    }

    inline UINT32 ComMgr::GetNextComTypeID()
    {
        static UINT32 lastID = UINT32_MAX;
        return ++lastID;
    }
    template<typename T>
    static inline const std::string_view ComMgr::GetComName()
    {
        return GetComName(GetComTypeID<T>());
    }


    template<typename T>
    inline constexpr eComponentCategory ComMgr::GetComponentCategory()
    {
        if constexpr (std::is_base_of_v<ITransform, T>)
        {
            return eComponentCategory::Transform;
        }
        else if constexpr (std::is_base_of_v<ICollider, T>)
        {
            return eComponentCategory::Collider;
        }
        else if constexpr (std::is_base_of_v<IAnimator, T>)
        {
            return eComponentCategory::Animator;
        }
        else if constexpr (std::is_base_of_v<ILight, T>)
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
        else if constexpr (std::is_base_of_v<IRenderer, T>)
        {
            return eComponentCategory::Renderer;
        }
        else if constexpr (std::is_base_of_v<Com_BehaviorTree, T>)
        {
            return eComponentCategory::BehaviorTree;
        }
        else if constexpr (std::is_base_of_v<IScript, T>)
        {
            return eComponentCategory::Scripts;
        }

        return eComponentCategory::UNKNOWN;
    }

    template<typename T>
    inline constexpr bool ComMgr::IsBaseComponent()
    {
        if constexpr (std::is_same_v<IComponent, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<ITransform, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<ICollider, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<IAnimator, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<ILight, T>)
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
        else if constexpr (std::is_same_v<IRenderer, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<Com_BehaviorTree, T>)
        {
            return true;
        }
        else if constexpr (std::is_same_v<IScript, T>)
        {
            return true;
        }

        return false;
    }
}



