#pragma once
#include "iResource.h"

#include "../Util/Serialize/JsonSerializer.h"

namespace ehw
{
    class GameObject;
	class Prefab final
		: public iResource
        , public Serializable<JsonSerializer>
	{
    public:
        Prefab();
        virtual ~Prefab();
        
        virtual eResult Serialize(JsonSerializer& _ser) override;
        virtual eResult DeSerialize(const JsonSerializer& _ser) override;

        //virtual eResult SaveJson(Json::Value* _pJVal) override;
        //virtual eResult LoadJson(const Json::Value* _pJVal) override;

        //DontDelete를 true로 설정할 시에는 prefab이 제거될 때 관리하는 게임오브젝트를 삭제하지 않음.
        inline void     RegisterPrefab(GameObject* _pPrefab, bool _bDontDelete = false);
        bool            IsAvailable() const { return nullptr != mPrefab; }
        GameObject*     Instantiate();


    private:
        GameObject* mPrefab;
        bool         mbDontDelete;
	};


    inline void Prefab::RegisterPrefab(GameObject* _pPrefab, bool _bDontDelete)
    {
        mPrefab = _pPrefab;
        mbDontDelete = _bDontDelete;
        
    }
}


