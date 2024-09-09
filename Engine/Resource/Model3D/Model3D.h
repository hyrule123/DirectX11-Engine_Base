#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/Common.h"
#include "Engine/GPU/CommonGPU.h"

#include "Engine/Util/Serialize/Serializable.h"

namespace editor
{
	class EditorFBXConverter;
}



namespace ehw
{
	class Mesh;
	class Material;
	class GameObject;
	class Com_Renderer_Mesh;
	class Skeleton;
	struct tFBXMaterial;

	struct tMeshContainer
	{
		std::shared_ptr<Mesh>					mesh;
		std::vector<std::shared_ptr<Material>>	materials;
	};

    class Model3D final 
		: public Resource
		, public Serializable_Json
    {
		CLASS_NAME(Model3D);
		REGISTER_INSTANCE_DEFAULT(Model3D);

		friend class editor::EditorFBXConverter;
		
	public:
		Model3D();
		virtual ~Model3D();

		//Model3D의 경로명 = 폴더명
		virtual eResult save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const override;
		virtual eResult load(const std::fs::path& _baseDir, const std::fs::path& _key_path) override;
		
		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		std::vector<std::unique_ptr<GameObject>> Instantiate();

	private:
		eResult ConvertFBX(
			const std::fs::path& _fbxPath, bool _bStatic,
			const std::fs::path& _dirAndFileName);

		eResult LoadFromFBX(
			const std::fs::path& _fbxPath, bool _bStatic,
			const std::fs::path& _dirAndFileName);

		//기존에 존재하는 변환된 데이터에 애니메이션만 추가
		static eResult AddAnimationFromFBX(const std::fs::path& _fbxPath, const std::fs::path& _meshDataName);

		//_defaultMtrlStrKey: FBX 변환된 재질이 이름이 없을수도 있음 -> 만약 없을 경우 사용할 기본 키 이름
		std::shared_ptr<Material> ConvertMaterial(const tFBXMaterial* _material, const std::fs::path& _texDestDir);

		//몬스터헌터 텍스처 일부를 FBX에서 읽어주지 못해서 직접 등록해야함.
		//특히 노말맵이 등록이 안돼있어서 수동으로 등록해줘야됨.
		//다행히도 이름이 통일되어 있어서 이를 활용하면 가능할 듯
		void CheckMHMaterial(Material* _mtrl, const std::fs::path& _texDestDir);

		bool SetDataToRenderer(Com_Renderer_Mesh* _renderer, UINT _idx);

	private:
		std::shared_ptr<Skeleton> m_skeleton;
		std::vector<tMeshContainer> m_meshContainers;
    };
}


