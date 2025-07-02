#pragma once
#include <Editor/Base/EditorWindow.h>

#include <Engine/Util/SimpleMath.h>

namespace core
{
    class Texture;
}

namespace core
{
    namespace editor
    {
        class EditorUVCalculator :
            public EditorWindow
        {
        public:
            EditorUVCalculator();
            virtual ~EditorUVCalculator();

            virtual void init() override;
            virtual void update_UI() override;

        private:
            void UpdateLoadButton();
            void UpdateTexture();
            void UpdateUVIndicator();

        private:
            s_ptr<Texture> mTexture;
            ImVec2                      mTexRenderSize;

            ImVec2 mCursorPos;
            ImVec2 mDragStartPos;
            ImVec2 mDragEndPos;
        };
    }

}


