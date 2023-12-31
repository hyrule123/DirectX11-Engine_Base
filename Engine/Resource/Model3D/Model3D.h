#pragma once
#include "../../Resource/iResource.h"


#include "../../CommonStruct.h"
#include "../../GPU/CommonGPU.h"

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

    class Model3D :
        public iResource
    {
		friend class editor::EditorFBXConverter;
		
	public:
		Model3D();
		virtual ~Model3D();

		virtual eResult Save(const std::fs::path& _pathFromBaseDir) override;
		virtual eResult Load(const std::fs::path& _pathFromBaseDir) override;
		

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;

		//아예 새 게임오브젝트를 반환
		std::shared_ptr<GameObject> Instantiate();
		eResult Instantiate(GameObject* _emptyRootObj);

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
		void CheckMHMaterial(std::shared_ptr<Material> _mtrl, const std::fs::path& _texDestDir);

		bool SetRenderer(const std::shared_ptr<Com_Renderer_Mesh>& _renderer, UINT _idx);

		
		//리소스는 독자적으로 관리(ResourceManager에 등록하지 않음 - 이름이 겹칠 수 있기 떄문)
	private:
		std::vector<tMeshContainer> m_meshContainers;
		std::shared_ptr<Skeleton> m_skeleton;
    };

}


