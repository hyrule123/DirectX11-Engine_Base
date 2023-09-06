#pragma once
#include "guiWindow.h"
#include "guiComboBox.h"
#include "DefaultShader/SH_CommonStruct.hlsli"

namespace mh
{
    class Texture;
}

namespace gui
{
    class guiNormalConverter :
        public guiWindow
    {
    public:
        guiNormalConverter();
        virtual ~guiNormalConverter();

        virtual void Init() override;
        virtual void UpdateUI() override;

    private:
        void SrcTextureUpdate();
        void DestTextureUpdate();
        void CopyTextureUpdate();

        void Reset();

        enum class eXYZSign
        {
            X,
            Y,
            Z,
            MinusX,
            MinusY,
            MinusZ,
            XYZSignEnd
        };
        struct AxisAndSign
        {
            int Axis;
            int Sign;
        };

        inline AxisAndSign GetXYZSignToHLSLFormat(eXYZSign _sign);

        
    private:
        std::shared_ptr<mh::Texture>        mTextureSrc;
        guiComboBox mComboSrcR;
        guiComboBox mComboSrcG;
        guiComboBox mComboSrcB;

        std::filesystem::path               mTextureDestPath;

        tCB_UniformData mCB;
    };

    inline guiNormalConverter::AxisAndSign guiNormalConverter::GetXYZSignToHLSLFormat(eXYZSign _sign)
    {
        AxisAndSign ans{};
        switch (_sign)
        {
        case gui::guiNormalConverter::eXYZSign::X:
            ans.Axis = (int)eXYZSign::X;
            ans.Sign = 1;
            break;

        case gui::guiNormalConverter::eXYZSign::Y:
            ans.Axis = (int)eXYZSign::Y;
            ans.Sign = 1;
            break;

        case gui::guiNormalConverter::eXYZSign::Z:
            ans.Axis = (int)eXYZSign::Z;
            ans.Sign = 1;
            break;

        case gui::guiNormalConverter::eXYZSign::MinusX:
            ans.Axis = (int)eXYZSign::X;
            ans.Sign = -1;
            break;

        case gui::guiNormalConverter::eXYZSign::MinusY:
            ans.Axis = (int)eXYZSign::Y;
            ans.Sign = -1;
            break;

        case gui::guiNormalConverter::eXYZSign::MinusZ:
            ans.Axis = (int)eXYZSign::Z;
            ans.Sign = -1;
            break;

        default:
            ans.Axis = -1;
            ans.Sign = -1;
            break;
        }

        return ans;
    }

}


