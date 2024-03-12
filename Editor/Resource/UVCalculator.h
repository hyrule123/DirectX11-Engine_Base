#pragma once
#include "Editor/Base/EditorWindow.h"

#include <Engine/Util/SimpleMath.h>

namespace ehw
{
    class Texture;
}

namespace ehw
{
    namespace editor
    {
        class EditorUVCalculator :
            public EditorWindow
        {
        public:
            EditorUVCalculator();
            virtual ~EditorUVCalculator();

            virtual void Init() override;
            virtual void UpdateUI() override;

        private:
            void UpdateLoadButton();
            void UpdateTexture();
            void UpdateUVIndicator();

        private:
            std::shared_ptr<Texture> mTexture;
            ImVec2                      mTexRenderSize;

            ImVec2 mCursorPos;
            ImVec2 mDragStartPos;
            ImVec2 mDragEndPos;
        };
    }

}


