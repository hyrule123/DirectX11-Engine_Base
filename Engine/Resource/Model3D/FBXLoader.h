#pragma once
#include "Engine/Game/Entity.h"

#include "Engine/Common.h"

#include <windef.h>

#include <fbxsdk/fbxsdk.h>


namespace core
{
//===============
// Struct of FBX 
//===============
// Material 계수

	struct tFBXMaterial
	{
		std::string		strMtrlName{};
		std::string		strDiffuseTex{};
		std::string		strNormalTex{};
		std::string		strSpecularTex{};
		std::string		strEmissiveTex{};

		float4 DiffuseColor{};
		float4 SpecularColor{};
		float4 AmbientColor{};
		float4 EmissiveColor{};

		float	SpecularPower{};
		float	TransparencyFactor{};
		float	Shininess{};
	};

	struct tFBXWeight
	{
		int		BoneIdx{};
		double	Weight{};
	};

	struct tFBXContainer
	{
		std::string								Name{};
		std::vector<float4>						vecPosition{};
		std::vector<float2>						vecUV{};

		//index buffer 수 == material 수
		std::vector<std::vector<UINT>>			vecIndexBuffers{};
		std::vector<tFBXMaterial>				vecMtrl{};

		//이 정점이 특정 Bone Index로부터 얼마만큼의 추가 가중치를 받을것인지(배율)
		std::unordered_map<int, std::vector<tFBXWeight>>	mapWeights{};

		//bump
		bool bBump{};
		std::vector<float3>						vecNormal{};
		std::vector<float3>						vecBinormal{};
		std::vector<float3>						vecTangent{};

		//Animation
		bool bAnimation{};
		std::vector<uint4>						vecBlendIndex{};
		std::vector<float4>						vecBlendWeight{};

		void resize_by_vertices_count(int _iSize)
		{
			vecPosition.resize(_iSize);
			vecUV.resize(_iSize);
			vecTangent.resize(_iSize);
			vecBinormal.resize(_iSize);
			vecNormal.resize(_iSize);
		}
	};

	struct tFBXBone
	{
		std::string					strBoneName{};
		int							Depth{};			// 계층구조 깊이
		int							ParentIndx{};	// 부모 Bone 의 인덱스
		fbxsdk::FbxAMatrix			matOffset{};		// Offset 행렬( -> 뿌리 -> Local)
		fbxsdk::FbxAMatrix			matBone{};
	};

	struct tFBXKeyFrame
	{
		fbxsdk::FbxAMatrix  matTransform{};
		double				Time{};
	};

	struct tFBXKeyFramesPerBone
	{
		int							BoneIndex{};
		std::vector<tFBXKeyFrame>	vecKeyFrame{};
	};


	struct tFBXAnimClip
	{
		std::string		strName{};
		fbxsdk::FbxTime		StartTime{};
		fbxsdk::FbxTime		EndTime{};
		
		fbxsdk::FbxLongLong StartFrameIdx{};
		fbxsdk::FbxLongLong EndFrameIdx{};
		fbxsdk::FbxLongLong FrameLength{};//전체 길이(EndFrameIdx + 1)

		fbxsdk::FbxTime::EMode TimeMode{};

		//각 본 별 키프레임 데이터
		std::vector<tFBXKeyFramesPerBone> KeyFramesPerBone{};
	};

	class FBXLoader :
		public Entity
	{
		CLASS_NAME(FBXLoader);
		REGISTER_INSTANCE_DEFAULT(FBXLoader);
	public:
		FBXLoader();
		virtual ~FBXLoader();

	public:
		void Reset();

		eResult load_FBX(const std::filesystem::path& _strPath, bool _bStatic = true);

		// FbxMatrix -> Matrix(FBX(double) 포맷에서 프로젝트 포맷(float)으로 변환해서 반환해준다
		static MATRIX get_matrix_from_FbxAMatrix(const fbxsdk::FbxAMatrix& _mat);

		const std::vector<tFBXContainer>& get_containers() const { return mContainers; }
		const std::vector<tFBXBone>& get_bones() const { return mBones; }
		const std::vector<tFBXAnimClip>& get_animations() const { return mAnimClips; }

	private:
		void convert_from_FBX_tree(fbxsdk::FbxNode* _pRoot, bool _bStatic);
		tFBXContainer load_container(fbxsdk::FbxMesh* _pFBXMesh, bool _bStatic);
		tFBXMaterial load_material(fbxsdk::FbxSurfaceMaterial* _pMtrlSur);

		float3 get_tangent(fbxsdk::FbxMesh* _pMesh, int _vertexIdx, int _iVtxOrder);
		float3 get_binormal(fbxsdk::FbxMesh* _pMesh, int _vertexIdx, int _iVtxOrder);
		float3 get_normal(fbxsdk::FbxMesh* _pMesh, int _vertexIdx, int _iVtxOrder);
		float2 get_UV(fbxsdk::FbxMesh* _pMesh, int _vertexIdx, int _iVtxOrder);

		float4 get_material_data(fbxsdk::FbxSurfaceMaterial* _pSurface, const char* _pMtrlName, const char* _pMtrlFactorName);
		std::string get_material_texture_name(fbxsdk::FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty);

		void load_textures();

		//혹시라도 Material 이름이 안지어져있을 때 사용하기위해 Path를 받아옴(이름이 있을경우 사용 X)
		//void CreateMaterial(const std::filesystem::path& _strPath);

		// Animation
		void load_skeleton(fbxsdk::FbxNode* _pNode);
		void load_bone_recursive(fbxsdk::FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx);
		void load_animation_clip();
		void triangulate(fbxsdk::FbxNode* _pNode);

		void load_animation_data(tFBXContainer& _container, fbxsdk::FbxMesh* _pMesh);
		void load_index_weights(tFBXContainer& _container, fbxsdk::FbxCluster* _pCluster, int _bondIndex);
		void load_offset_matrices(tFBXContainer& _container, fbxsdk::FbxCluster* _pCluster, const fbxsdk::FbxAMatrix& _matNodeTransform, int _bondIndex);
		void load_keyframe_transform(tFBXContainer& _container, fbxsdk::FbxNode* _pNode, fbxsdk::FbxCluster* _pCluster, const fbxsdk::FbxAMatrix& _matNodeTransform
			, int _bondIndex);
		void CheckWeightAndIndices(tFBXContainer& _container, fbxsdk::FbxMesh* _pMesh);

		int FindBoneIndex(const std::string& _strBoneName);
		fbxsdk::FbxAMatrix GetTransform(fbxsdk::FbxNode* _pNode);


		int GetAllNodesCountRecursive(fbxsdk::FbxNode* _pNode);

	private:
		fbxsdk::FbxManager*						mManager;
		fbxsdk::FbxScene*						mScene;

		std::vector<tFBXContainer>				mContainers;

		// Animation
		std::vector<tFBXBone>					mBones;
		fbxsdk::FbxArray<fbxsdk::FbxString*>	mAnimNames;
		std::vector<tFBXAnimClip>				mAnimClips;
		bool									mbMixamo;

		std::string								mFileName;
	};
}


