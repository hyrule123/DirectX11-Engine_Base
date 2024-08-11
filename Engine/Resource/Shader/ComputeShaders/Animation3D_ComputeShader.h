#pragma once
#include "Engine/Resource/Shader/ComputeShader.h"

#include "Engine/DefaultShader/Common_struct.hlsli"

//설명: 3D 애니메이션을 처리하기 위한 컴퓨트쉐이더
//사용법
//* Animation3D_ComputeShader::Desc 구조체에 필요한 내용을 넣어서 전달.

namespace ehw
{
	class StructBuffer;
    class Animation3D_ComputeShader :
        public ComputeShader
    {
	public:
		Animation3D_ComputeShader();
		virtual ~Animation3D_ComputeShader();

		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;

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
		void SetDesc(const Animation3D_ComputeShader::Desc& _desc) { m_desc = _desc; }
	private:
		Desc m_desc;
    };
}


