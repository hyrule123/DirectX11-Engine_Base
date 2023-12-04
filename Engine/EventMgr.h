#pragma once
#include "define_Enum.h"

namespace ehw
{
    class GameObject;
    class IScene;
    class EventMgr
    {
        friend class Application;
    public:
        enum class eEventType
        {
            SpawnGameObj,
            DeleteGameObj,
            AddChild,
            MoveGameObjLayer,
        };
        struct tEvent
        {
            eEventType Type;
            DWORD_PTR	wParam;
            DWORD_PTR	lParam;
        };
    public:
        //새 게임오브젝트를 동적할당해서 반환
        static GameObject* SpawnGameObject(eLayerType _layer);

        //기본적으로 자신을 포함한 Child 게임오브젝트를 동일한 레이어에 넣도록 설정하였음.
        //만약 특정 자식 오브젝트만 다른 레이어에 넣고 싶을 경우 ChangeGameObjectLayer 함수를 사용할 것
        static GameObject* SpawnGameObject(eLayerType _layer, GameObject* _gameObj);

        static void        ChangeGameObjectLayer(eLayerType _layer, GameObject* _gameObj);

        template <class F, class... Args>
        static inline void AddFrameEndEvent(F&& _func, Args&&... _args);

        //리턴값이 필요한 경우(future 변수 사용)
        template <class F, class... Args>
        static inline std::future<typename std::invoke_result<F, Args...>::type> AddFrameEndEventReturn(F&& _func, Args&&... _args);



    private:
        static bool Init();
        static void Release();

        static void FrameEnd();


    private:
        static std::vector<std::function<void()>> m_FrameEndEvents;

    private:
        EventMgr() = delete;
        ~EventMgr() = delete;
    };

    template<class F, class ...Args>
    inline void EventMgr::AddFrameEndEvent(F&& _func, Args && ..._args)
    {
        auto func = std::bind(std::forward<F>(_func), std::forward<Args>(_args)...);
        m_FrameEndEvents.push_back([func]() { func(); });
    }

    template<class F, class ...Args>
    inline std::future<typename std::invoke_result<F, Args...>::type> EventMgr::AddFrameEndEventReturn(F&& _func, Args && ..._args)
    {
        using return_type = std::invoke_result<F, Args...>::type;

        auto pFunc = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(_func), std::forward<Args>(_args)...)
        );
            
        std::future<return_type> pFunc_result_future = pFunc->get_future();
        m_FrameEndEvents.push_back([pFunc]() { (*pFunc)(); });

        return pFunc_result_future;
    }

}


