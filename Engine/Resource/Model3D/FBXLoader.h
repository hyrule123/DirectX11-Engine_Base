#pragma once
#include <Engine/Game/Entity.h>

#include <Engine/Common.h>

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
		std::string		name{};
		std::string		diffuse_tex_name{};
		std::string		normal_tex_name{};
		std::string		specular_tex_name{};
		std::string		emissive_tex_name{};

		float4 diffuse_color{};
		float4 specular_color{};
		float4 ambient_color{};
		float4 emissive_color{};

		float	specular_power{};
		float	transparency_factor{};
		float	shiness{};
	};

	struct tFBXWeight
	{
		int		bone_idx{};
		double	weight{};
	};

	struct tFBXContainer
	{
		std::string								name{};
		std::vector<float4>						positions{};
		std::vector<float2>						UVs{};

		//index buffer 수 == material 수
		std::vector<std::vector<UINT>>			index_buffers{};
		std::vector<tFBXMaterial>				materials{};

		//이 정점이 특정 Bone Index로부터 얼마만큼의 추가 가중치를 받을것인지(배율)
		std::unordered_map<int, std::vector<tFBXWeight>>	weights_table{};

		//bump
		bool b_bump{};
		std::vector<float3>						normals{};
		std::vector<float3>						binormals{};
		std::vector<float3>						tangents{};

		//Animation
		bool b_animation{};
		std::vector<uint4>						blend_indices{};
		std::vector<float4>						blend_weights{};

		void resize_by_vertices_count(int _iSize)
		{
			positions.resize(_iSize);
			UVs.resize(_iSize);
			tangents.resize(_iSize);
			binormals.resize(_iSize);
			normals.resize(_iSize);
		}
	};

	struct tFBXBone
	{
		std::string					bone_name{};
		int							depth_level{};			// 계층구조 깊이
		int							parent_bone_idx{};	// 부모 Bone 의 인덱스
		fbxsdk::FbxAMatrix			offset_matrix{};		// Offset 행렬( -> 뿌리 -> Local)
		fbxsdk::FbxAMatrix			bone_matrix{};
	};

	struct tFBXKeyFrame
	{
		fbxsdk::FbxAMatrix  transform_matrix{};
		double				time{};
	};

	struct tFBXKeyFramesPerBone
	{
		int							bone_idx{};
		std::vector<tFBXKeyFrame>	key_frames{};
	};


	struct tFBXAnimClip
	{
		std::string		name{};
		fbxsdk::FbxTime		start_time{};
		fbxsdk::FbxTime		end_time{};
		
		fbxsdk::FbxLongLong start_frame_idx{};
		fbxsdk::FbxLongLong end_frame_idx{};
		fbxsdk::FbxLongLong frame_length{};//전체 길이(EndFrameIdx + 1)

		fbxsdk::FbxTime::EMode time_mode{};

		//각 본 별 키프레임 데이터
		std::vector<tFBXKeyFramesPerBone> key_frames_per_bone{};
	};

	class FBXLoader :
		public Entity
	{
		CLASS_INFO(FBXLoader, Entity);
		REGISTER_FACTORY(FBXLoader);
	public:
		FBXLoader();
		virtual ~FBXLoader();

	public:
		void Reset();

		eResult load_FBX(const std::filesystem::path& _strPath, bool _bStatic = true);

		// FbxMatrix -> Matrix(FBX(double) 포맷에서 프로젝트 포맷(float)으로 변환해서 반환해준다
		static MATRIX get_matrix_from_FbxAMatrix(const fbxsdk::FbxAMatrix& _mat);

		const std::vector<tFBXContainer>& get_containers() const { return m_fbx_containers; }
		const std::vector<tFBXBone>& get_bones() const { return m_fbx_bones; }
		const std::vector<tFBXAnimClip>& get_animations() const { return m_animation_clips; }

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
		void check_weight_and_indices(tFBXContainer& _container, fbxsdk::FbxMesh* _pMesh);

		int find_bone_index(const std::string& _strBoneName);
		fbxsdk::FbxAMatrix get_transform_matrix(fbxsdk::FbxNode* _pNode);


		int get_all_nodes_count_recursive(fbxsdk::FbxNode* _pNode);

	private:
		fbxsdk::FbxManager*						m_fbx_manager;
		fbxsdk::FbxScene*						m_fbx_scene;

		std::vector<tFBXContainer>				m_fbx_containers;

		// Animation
		std::vector<tFBXBone>					m_fbx_bones;
		fbxsdk::FbxArray<fbxsdk::FbxString*>	m_animation_names;
		std::vector<tFBXAnimClip>				m_animation_clips;
		bool									m_b_mixamo;

		std::string								m_file_name;
	};
}


