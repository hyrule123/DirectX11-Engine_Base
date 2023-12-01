#pragma once
#include "Entity.h"
#include "define_Struct.h"
#include "Animation3D.h"

//MeshData에 종속된 클래스
namespace ehw
{
	class FBXLoader;
	class StructBuffer;
	class Skeleton :
		public Entity
	{
	public:
		Skeleton();
		virtual ~Skeleton();

		eResult Save(const std::fs::path& _filePath);
		eResult Load(const std::fs::path& _filePath);
		eResult CreateFromFBX(FBXLoader* _fbxLoader);

	public:
		//Animation 3D
		const std::vector<tMTBone>& GetBones() const { return m_vecBones; }
		UINT GetBoneCount() const { return (UINT)m_vecBones.size(); }
		StructBuffer* GetBoneOffsetBuffer() { return  m_pBoneOffset.get(); }	   // 각 뼈의 offset 행렬

		//반환타입 변수길이가 좀 김
		const std::unordered_map<std::string, std::shared_ptr<Animation3D>, tHashFunc_StringView, std::equal_to<>>& 
			GetAnimations() const { return mMapAnimations; }

		std::shared_ptr<Animation3D> FindAnimation(const std::string_view _strAnimName);


		//애니메이션을 이동가능한지 확인하고, 이동시켜주는 함수
		bool CopyAnimationFromOther(const Skeleton& _other, const std::fs::path& _saveDir);
		int FindSameBoneIndex(const tMTBone& _other) const;

	private:
		void CreateBoneOffsetSBuffer();

	private:
		std::vector<tMTBone>					m_vecBones;
		std::unique_ptr<StructBuffer>					m_pBoneOffset;	  // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)

		std::unordered_map<std::string, std::shared_ptr<Animation3D>, tHashFunc_StringView, std::equal_to<>>	mMapAnimations;
	};
}


