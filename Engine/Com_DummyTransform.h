#pragma once
#include "iTransform.h"

namespace ehw
{
    class Com_Transform;

    //아무 작업도 안하는 Transform.
    //부모쪽 트랜스폼의 정보를 그대로 사용하고자 할 경우 이걸 쓰면 됨
    class Com_DummyTransform :
        public iTransform
    {
    public:
        Com_DummyTransform();
        virtual ~Com_DummyTransform();

        virtual void Init() override;
        virtual void InternalUpdate() override {};
        virtual void BindData() override;

    private:
        Com_Transform* mParentTransform;
    };
}


