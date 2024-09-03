#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/Common.h"

#include "Engine/Util/Serialize/Serializable.h"

//MeshData에 종속된 클래스
namespace ehw
{
	class FBXLoader;
	class StructBuffer;
	class Animation3D;
	class Skeleton final
		: public Resource
		, public Serializable_Binary
	{
		REGISTER_CLASS_INFO(Skeleton);
	public:
		Skeleton();
		virtual ~Skeleton();

		virtual eResult Save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const override;
		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _key_path) override;

		virtual eResult Serialize_Binary(BinarySerializer* _ser) const override;
		virtual eResult DeSerialize_Binary(const BinarySerializer* _ser) override;
		

		eResult CreateFromFBX(FBXLoader* _fbxLoader);

	public:
		//Animation 3D
		const std::vector<tMTBone>& GetBones() const { return m_vecBones; }
		UINT GetBoneCount() const { return (UINT)m_vecBones.size(); }
		std::shared_ptr<StructBuffer> GetBoneOffsetBuffer() { return  m_pBoneOffset; }	   // 각 뼈의 offset 행렬

		//반환타입 변수길이가 좀 김
		const std::unordered_map<std::string, std::shared_ptr<Animation3D>, tHasher_StringView, std::equal_to<>>& 
			GetAnimations() const { return m_animations; }

		std::shared_ptr<Animation3D> FindAnimation(const std::string_view _strAnimName);


		//애니메이션을 이동가능한지 확인하고, 이동시켜주는 함수
		bool CopyAnimationFromOther(const Skeleton& _other, const std::fs::path& _saveDir);
		int FindSameBoneIndex(const tMTBone& _other) const;

	private:
		void CreateBoneOffsetSBuffer();

	private:
		std::vector<tMTBone>							m_vecBones;

		// 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)
		std::shared_ptr<StructBuffer>					m_pBoneOffset;	  

		std::unordered_map<std::string, std::shared_ptr<Animation3D>, tHasher_StringView, std::equal_to<>>	m_animations;
	};
}


