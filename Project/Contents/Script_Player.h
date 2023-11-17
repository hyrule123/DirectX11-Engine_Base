#pragma once
#include <Engine/IScript.h>

namespace ehw
{
    class Script_Player :
        public IScript
    {
    public:
        Script_Player() {};
        virtual ~Script_Player() {};

        virtual void RequireComponent() override;
        virtual void Init() override;
        virtual void Update() override;

    private:
        void TestCallback();
    };

}

