#pragma once
#include "../../../Resource/Shader/iComputeShader.h"



#include "../../../DefaultShader/CommonStruct.hlsli"

namespace ehw
{
	class StructBuffer;
    class Animation3DShader :
        public iComputeShader
    {
	public:


		Animation3DShader();
		virtual ~Animation3DShader();

		virtual bool BindData();
		virtual void UnBindData();

		struct Desc
		{
			tCB_Animation3D* Anim3DData{};
			StructBuffer* CurrentAnimKeyFrameBuffer{};
			StructBuffer* NextAnimKeyFrameBuffer{};
			StructBuffer* BoneOffsetMatrixBuffer{};
			StructBuffer* FinalBoneTranslationMatrixBuffer{};
		};
		void SetDesc(const Animation3DShader::Desc& _desc) { mDesc = _desc; }
	private:
		Desc mDesc;
    };
}


