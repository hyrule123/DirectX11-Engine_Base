#pragma once
#include "ComputeShader.h"
#include "DefaultShader/SH_CommonStruct.hlsli"

namespace mh
{
	class StructBuffer;
    class Animation3DShader :
        public ComputeShader
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


