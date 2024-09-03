#include "Engine/Resource/Model3D/Model3D.h"

#include "Engine/Util/define_Util.h"
#include "Engine/Util/Serialize/JsonSerializer.h"

#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/PathManager.h"

#include "Engine/Game/Scene.h"
#include "Engine/Game/GameObject.h"

#include "Engine/Game/Component/Com_Transform.h"
#include "Engine/Game/Component/Renderer/Com_Renderer_3DAnimMesh.h"
#include "Engine/Game/Component/Animator/Com_Animator3D.h"
#include "Engine/Game/Component/Animator/Animation3D_PlayData.h"

#include "Engine/Resource/Shader/GraphicsShader.h"

#include "Engine/Resource/Texture.h"
#include "Engine/Resource/Model3D/Skeleton.h"
#include "Engine/Resource/Model3D/FBXLoader.h"
#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Material.h"

#include <regex>

namespace ehw
{
	Model3D::Model3D()
		: Resource(ClassInfo<Model3D>::name())
	{
	}

	Model3D::~Model3D()
	{
	}


	eResult Model3D::Load(const std::fs::path& _baseDir, const std::fs::path& _key_path)
	{
		//Model3D는 다른 클래스와 저장 / 로드 방식이 약간 다름
		//예를 들어 Player를 저장한다고 하면
		//Player/Player.json 형태로 저장한다.
		std::fs::path strKey = _key_path;
		strKey.replace_extension();

		std::fs::path fullPath = _baseDir / strKey / strKey;
		fullPath.replace_extension(strKey::path::extension::Model3D);

		return LoadFile_Json(fullPath);
	}

	eResult Model3D::Save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const
	{
		//Model3D는 다른 클래스와 저장 / 로드 방식이 약간 다름
		//예를 들어 Player를 저장한다고 하면
		//Player/Player.json 형태로 저장한다.
		std::fs::path strKey = _key_path;
		strKey.replace_extension();

		std::fs::path fullPath = _baseDir / strKey / strKey;
		fullPath.replace_extension(strKey::path::extension::Model3D);
		return SaveFile_Json(fullPath);
	}

	eResult Model3D::Serialize_Json(JsonSerializer* _ser) const
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Json::Value가 nullptr 입니다.");
			return eResult::Fail_Nullptr;
		}

		Json::Value& ser = *_ser;

		try
		{
			//Skeleton
			{
				std::string skeletonStrKey{};
				if (m_skeleton)
				{
					eResult result = ResourceManager<Skeleton>::GetInst().Save(m_skeleton.get());
					if (eResult_fail(result))
					{
						ERROR_MESSAGE("스켈레톤 정보 저장 실패!");
						return result;
					}
					skeletonStrKey = m_skeleton->get_strkey();
				}

				ser[JSON_KEY(m_skeleton)] << skeletonStrKey;
			}


			//전부 포인터 형태이므로 StrKey를 등록해 준다.
			Json::Value& arrMeshCont = (ser)[JSON_KEY(m_meshContainers)];
			for (size_t i = 0; i < m_meshContainers.size(); ++i)
			{
				//nullptr check
				if (nullptr == m_meshContainers[i].mesh || m_meshContainers[i].materials.empty())
				{
					return eResult::Fail_InValid;
				}
				Json::Value& meshContainer = arrMeshCont[i];

				//Mesh
				eResult result = ResourceManager<Mesh>::GetInst().Save(m_meshContainers[i].mesh.get());
				if (eResult_fail(result))
				{
					ERROR_MESSAGE("메쉬 정보 저장 실패");
					return result;
				}
				meshContainer["mesh"] << m_meshContainers[i].mesh->get_strkey();

				//Materials
				Json::Value& materials = meshContainer["materials"];
				for (size_t j = 0; j < m_meshContainers[i].materials.size(); ++j)
				{
					if (nullptr == m_meshContainers[i].materials[j])
					{
						return eResult::Fail_Nullptr;
					}

					eResult result = ResourceManager<Material>::GetInst().Save(m_meshContainers[i].materials[j].get());
					if (eResult_fail(result))
					{
						ERROR_MESSAGE("재질 정보 저장 실패");
						return result;
					}

					materials[j] << m_meshContainers[i].materials[j]->get_strkey();
				}
			}
		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail;
		}



		return eResult::Success;
	}

	eResult Model3D::DeSerialize_Json(const JsonSerializer* _ser)
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}
		const JsonSerializer& ser = *_ser;

		m_skeleton = nullptr;
		m_meshContainers.clear();

		//Skeleton
		{
			std::string skeletonStrKey{};
			ser[JSON_KEY(m_skeleton)] >> skeletonStrKey;

			if (false == skeletonStrKey.empty())
			{
				m_skeleton = ResourceManager<Skeleton>::GetInst().Load(skeletonStrKey);
				if (nullptr == m_skeleton)
				{
					ERROR_MESSAGE("스켈레톤 정보 로드에 실패했습니다.");
					return eResult::Fail_Nullptr;
				}
			}
		}

		//MeshContainers
		{
			//전부 포인터 형태이므로 StrKey를 등록해 준다.
			const Json::Value& arrMeshCont = (ser)[JSON_KEY(m_meshContainers)];
			size_t meshContSize = (size_t)arrMeshCont.size();
			m_meshContainers.resize(meshContSize);
			for (size_t i = 0; i < m_meshContainers.size(); ++i)
			{
				const Json::Value& meshContainer = arrMeshCont[i];
				
				//nullptr check
				if (false == meshContainer.isMember("mesh"))
				{
					ERROR_MESSAGE("메쉬 정보가 없습니다.");
					return eResult::Fail_InValid;
				}
				else if (false == meshContainer.isMember("materials"))
				{
					ERROR_MESSAGE("재질 정보가 없습니다.");
					return eResult::Fail_InValid;
				}
				
				
				//Mesh
				std::string meshStrkey{};
				meshContainer["mesh"] >> meshStrkey;
				m_meshContainers[i].mesh = ResourceManager<Mesh>::GetInst().Load(meshStrkey);
				if (nullptr == m_meshContainers[i].mesh)
				{
					ERROR_MESSAGE("메쉬 로드 실패");
					return eResult::Fail_Open;
				}
				m_meshContainers[i].mesh->SetSkeleton(m_skeleton);

				//Materials
				const Json::Value& materials = meshContainer["materials"];
				size_t materialSize = materials.size();
				m_meshContainers[i].materials.resize(materialSize);
				for (size_t j = 0; j < m_meshContainers[i].materials.size(); ++j)
				{
					std::string materialStrKey{};
					materials[j] >> materialStrKey;
					m_meshContainers[i].materials[j] = ResourceManager<Material>::GetInst().Load(materialStrKey);

					if (nullptr == m_meshContainers[i].materials[j])
					{
						ERROR_MESSAGE("재질 로드 실패");
						return eResult::Fail_Open;
					}
				}
			}
		}

		return eResult::Success;
	}

	std::vector<std::unique_ptr<GameObject>> Model3D::Instantiate()
	{
		std::vector<std::unique_ptr<GameObject>> newObjects{};
		newObjects.push_back(std::make_unique<GameObject>());
		GameObject* root = newObjects.back().get();

		root->SetName(get_strkey());

		Com_Transform* rootTransform = root->GetComponent<Com_Transform>();
		

		//스켈레톤 있고 + 애니메이션 데이터가 있을 경우 Animator 생성
		std::shared_ptr<Animation3D_PlayData> sharedAnimationData{};
		if (m_skeleton)
		{
			Com_Animator3D* rootAnimator = root->AddComponent<Com_Animator3D>();

			sharedAnimationData = rootAnimator->CreateSharedAnimationData();
			sharedAnimationData->SetSkeleton(m_skeleton);
		}

		//사이즈가 딱 하나일 경우: GameObject 본체에 데이터를 생성
		if (1u == (UINT)m_meshContainers.size())
		{
			Com_Renderer_Mesh* renderer = nullptr;
			if (sharedAnimationData)
			{
				//수동으로 애니메이터를 설정
				Com_Renderer_3DAnimMesh* renderer3D = root->AddComponent<Com_Renderer_3DAnimMesh>();
				renderer = static_cast<Com_Renderer_Mesh*>(renderer3D);
			}
			else
			{
				renderer = root->AddComponent<Com_Renderer_Mesh>();
			}

			if (false == SetDataToRenderer(renderer, 0))
			{
				newObjects.clear();
				ASSERT(false, "Renderer 세팅 실패.");
				return newObjects;
			}
		}

		//여러 개의 container를 가지고 있을 경우: 하나의 부모 object에 여러개의 child를 생성해서 각각 Meshrenderer에 할당
		else
		{
			for (size_t i = 0; i < m_meshContainers.size(); ++i)
			{
				newObjects.push_back(std::make_unique<GameObject>());
				GameObject* child = newObjects.back().get();
				rootTransform->AddChild(child->Transform());

				//ComponentManager로부터 Mesh 렌더러를 받아와서 MultiMesh에 넣어준다.
				Com_Renderer_Mesh* renderer = nullptr;
				if (sharedAnimationData)
				{
					Com_Animator3D* childAnimator = child->AddComponent<Com_Animator3D>();
					childAnimator->SetSharedAnimationData(sharedAnimationData);

					//수동으로 애니메이터를 설정
					auto renderer3D = child->AddComponent<Com_Renderer_3DAnimMesh>();
					renderer = static_cast<Com_Renderer_Mesh*>(renderer3D);
				}
				else
				{
					renderer = child->AddComponent<Com_Renderer_Mesh>();
				}

				if (false == SetDataToRenderer(renderer, (UINT)i))
				{
					newObjects.clear();
					ASSERT(false, "Renderer 세팅 실패.");
					return newObjects;
				}
			}
		}

		return newObjects;
	}

	eResult Model3D::ConvertFBX(
		const std::fs::path& _fbxPath, bool _bStatic,
		const std::fs::path& _dirAndFileName
	)
	{
		eResult result = LoadFromFBX(_fbxPath, _bStatic, _dirAndFileName);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("FBX로부터 로드 실패.");
			return result;
		}


		//다른게 다 진행됐으면 저장 진행
		//키값 만들고 세팅하고
		result = ResourceManager<Model3D>::GetInst().Save(this, _dirAndFileName);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("Model3D 저장에 실패했습니다.");
			return result;
		}

		return eResult::Success;
	}

	eResult Model3D::LoadFromFBX(const std::fs::path& _fbxPath, bool _bStatic, const std::fs::path& _dirAndFileName)
	{
		if (false == std::fs::exists(_fbxPath))
		{
			ERROR_MESSAGE("파일을 찾지 못했습니다.");
			return eResult::Fail_Open;
		}
		else if (_dirAndFileName.empty())
		{
			ERROR_MESSAGE("파일명을 설정하지 않았습니다.");
			return eResult::Fail_Open;
		}

		FBXLoader loader{};
		eResult result = loader.LoadFbx(_fbxPath, _bStatic);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("FBX 불러오기 실패.");
			return result;
		}

		//Res/Model3D/Player
		std::fs::path filePath = _dirAndFileName;
		filePath.replace_extension();
		//Res/Model3D/Player/Player
		filePath = filePath / filePath;

		//Bone 정보 로드
		m_skeleton = std::make_shared<Skeleton>();

		//Key 설정
		{
			std::fs::path skltStrKey = filePath;
			skltStrKey.replace_extension(strKey::path::extension::Skeleton);
			m_skeleton->set_keypath(skltStrKey.string());
		}
		result = m_skeleton->CreateFromFBX(&loader);

		//애니메이션 정보가 없을 경우에는 도로 제거
		if (eResult::Fail_Empty == result)
		{
			m_skeleton = nullptr;
		}
		//그 외의 이유로 실패했을 경우에는 에러
		else if (eResult_fail(result))
		{
			ERROR_MESSAGE("Skeleton 로드 실패.");
			return result;
		}

		const std::vector<tFBXContainer>& containers = loader.GetContainers();
		for (size_t i = 0; i < containers.size(); ++i)
		{
			tMeshContainer meshCont{};

			//가져올 메쉬를 생성
			meshCont.mesh = std::make_shared<Mesh>();
			meshCont.mesh->SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			result = meshCont.mesh->CreateFromContainer(&(containers[i]));
			if (eResult_fail(result))
			{
				ERROR_MESSAGE("FBX로부터 메쉬 정보를 읽어오는 데 실패했습니다.");
				return eResult::Fail;
			}

			//스켈레톤 주소를 지정
			meshCont.mesh->SetSkeleton(m_skeleton);


			//기본적으로는 컨테이너 이름을 사용
			//비어있을 경우 이름을 만들어준다
			std::fs::path strKey{}; 
			if (containers[i].Name.empty())
			{
				strKey = filePath;
				strKey.replace_extension();
				strKey += "_";
				strKey += std::to_string(i);
			}
			else
			{
				strKey = _dirAndFileName;
				strKey /= containers[i].Name;
			}

			//.msh로 확장자를 변경
			strKey.replace_extension(strKey::path::extension::Mesh);
			//Key로 Mesh를 저장
			meshCont.mesh->set_keypath(strKey.string());


			// 메테리얼 가져오기
			for (UINT j = 0; j < containers[i].vecMtrl.size(); ++j)
			{

				std::shared_ptr<Material> mtrl = 
					ConvertMaterial(&(containers[i].vecMtrl[j]), _dirAndFileName);
				if (nullptr == mtrl)
				{
					ERROR_MESSAGE("머티리얼 로드에 실패했습니다.");
					return eResult::Fail_InValid;
				}

				meshCont.materials.push_back(mtrl);
			}
			m_meshContainers.push_back(std::move(meshCont));
		}

		return eResult::Success;
	}

	eResult Model3D::AddAnimationFromFBX(const std::fs::path& _fbxPath, const std::fs::path& _meshDataName)
	{
		if (false == std::fs::exists(_fbxPath))
		{
			ERROR_MESSAGE("파일을 찾지 못했습니다.");
			return eResult::Fail_Open;
		}

		FBXLoader loader{};
		eResult result = loader.LoadFbx(_fbxPath, false);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("FBX 불러오기 실패.");
			return result;
		}

		std::shared_ptr<Skeleton> skeletonOfFBX = std::make_shared<Skeleton>();
		result = skeletonOfFBX->CreateFromFBX(&loader);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("FBX로부터 스켈레톤 로딩 실패.");
			return result;
		}

		//지금 필요한건 FBX에 저장된 Skeleton과 Animation 정보 뿐임
		std::shared_ptr<Skeleton> skeletonOfProj = ResourceManager<Skeleton>::GetInst().Load(_meshDataName / _meshDataName);

		//Skeleton의 실제 경로 예시: Player/Player
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("프로젝트 스켈레톤 불러오기 실패.");
			return result;
		}

		if (false == skeletonOfProj->CopyAnimationFromOther((*skeletonOfFBX), _meshDataName))
		{
			ERROR_MESSAGE("스켈레톤 구조가 일치하지 않아 애니메이션을 추가할 수 없습니다.");
			return eResult::Fail;
		}

		return eResult::Success;
	}

	std::shared_ptr<Material> Model3D::ConvertMaterial(const tFBXMaterial* _fbxMtrl, const std::fs::path& _texDestDir)
	{
		if (nullptr == _fbxMtrl || _texDestDir.empty())
		{
			return nullptr;
		}
		//material 하나 생성
		std::shared_ptr<Material> mtrl = std::make_shared<Material>();

		std::fs::path strKey = _texDestDir.filename();
		strKey /= _fbxMtrl->strMtrlName;
		strKey.replace_extension(strKey::path::extension::Material);
		mtrl->set_keypath(strKey.string());

		mtrl->SetMaterialCoefficient(_fbxMtrl->DiffuseColor, _fbxMtrl->SpecularColor, _fbxMtrl->AmbientColor, _fbxMtrl->EmissiveColor);

		std::fs::path texDir = ResourceManager<Texture>::GetInst().GetBaseDir();
		texDir /= _texDestDir;

		//media directory(텍스처같은 파일들) 옮겨졌는지 여부 저장 변수
		bool bMediaDirMoved = false;

		//텍스처 옮기기 위한 람다 함수
		auto CopyAndLoadTex =
			[&](const std::string& _srcTexPath)->std::shared_ptr<Texture>
			{
				//비어있을경우 return
				if (_srcTexPath.empty())
					return nullptr;

				if (false == bMediaDirMoved)
				{
					std::fs::path srcTexPath = _srcTexPath;
					srcTexPath = srcTexPath.parent_path();

					if (false == std::fs::exists(texDir))
					{
						std::fs::create_directories(texDir);
					}

					if (std::fs::exists(srcTexPath))
					{
						const auto copyOption =
							std::fs::copy_options::overwrite_existing
							| std::fs::copy_options::recursive;

						std::fs::copy(srcTexPath, texDir, copyOption);
						std::fs::remove_all(srcTexPath);

						bMediaDirMoved = true;
					}
				}

				std::fs::path texKey = _texDestDir.filename();
				texKey /= std::fs::path(_srcTexPath).filename();

				std::shared_ptr<Texture> newTex = ResourceManager<Texture>::GetInst().Load(texKey);
				//바로 Texture Load. 로드 실패 시 false 반환
				if (nullptr == newTex)
				{
					newTex = nullptr;
				}

				return newTex;
			};

		mtrl->SetTexture(eTextureSlot::Albedo, CopyAndLoadTex(_fbxMtrl->strDiffuseTex));
		mtrl->SetTexture(eTextureSlot::Normal, CopyAndLoadTex(_fbxMtrl->strNormalTex));
		mtrl->SetTexture(eTextureSlot::Specular, CopyAndLoadTex(_fbxMtrl->strSpecularTex));
		mtrl->SetTexture(eTextureSlot::Emissive, CopyAndLoadTex(_fbxMtrl->strEmissiveTex));

		std::shared_ptr<GraphicsShader> defferedShader = ResourceManager<GraphicsShader>::GetInst().Find(strKey::defaultRes::shader::graphics::DefferedShader);
		mtrl->SetShader(defferedShader);

		mtrl->SetRenderingMode(eRenderingMode::DefferdOpaque);

		CheckMHMaterial(mtrl.get(), texDir);

		return mtrl;
	}

	void Model3D::CheckMHMaterial(Material* _mtrl, const std::fs::path& _texDestDir)
	{
		if (nullptr == _mtrl)
			return;

		constexpr const char* texSuffix[] =
		{
			"_BML",
			"_NM",
			"_XM",
			"_EM",
			"_RMT"
		};
		constexpr size_t texSuffixSize = sizeof(texSuffix) / sizeof(const char*);

		std::string strSuffix = "(";
		for (size_t i = 0; i < texSuffixSize; ++i)
		{
			strSuffix += texSuffix[i];

			if (i + 1 < texSuffixSize)
			{
				strSuffix += "|";
			}
		}
		strSuffix += ")(\\..+)$";
		const std::regex regexPrefix("(.+)(DXT\\d_|BC\\d_)");
		const std::regex regexSuffix(strSuffix);


		//텍스처 기본 생성 파일명을 통해서 몬스터헌터 텍스처 파일인지 확인
		bool isMHTex = false;
		for (size_t i = 0; i < texSuffixSize; ++i)
		{
			const std::shared_ptr<Texture>& tex = _mtrl->GetTexture((eTextureSlot)i);
			if (tex)
			{
				std::string texKey{ tex->get_strkey() };
				size_t pos = texKey.find(texSuffix[i]);
				if (std::string::npos != pos)
				{
					isMHTex = true;
				}
				else
				{
					//하나라도 이름이 일치하지 않으면 몬스터헌터 포맷이 아닌것으로 간주
					isMHTex = false;
					break;
				}
			}
		}
		

		//몬스터헌터 재질이 맞는것이 확인될 경우 못 가져온 텍스처 가져오는 작업 진행
		if (isMHTex)
		{
			for (size_t i = 0; i < texSuffixSize; ++i)
			{
				//i번째 텍스처가 있고
				std::shared_ptr<Texture> tex = _mtrl->GetTexture((eTextureSlot)i);
				if (tex)
				{
					for (size_t j = 0; j < texSuffixSize; ++j)
					{
						//j번째 텍스처는 없다면
						std::shared_ptr<Texture> newTex = _mtrl->GetTexture((eTextureSlot)j);
						if (nullptr == newTex)
						{
							//i번째 텍스처의 이름을 가져와서
							std::string texKey(tex->get_strkey());

							//regex 돌려서 prefix suffix 제거하고
							texKey = std::regex_replace(texKey, regexPrefix, "");
							texKey = std::regex_replace(texKey, regexSuffix, "");

							//파일 순회를 돌아주면서
							for (const auto& dirIter : std::fs::directory_iterator(_texDestDir))
							{
								std::string fileName = dirIter.path().filename().string();
								
								//이름이 일치하는 파일명이 아니면 continue
								if (std::string::npos == fileName.find(texKey))
									continue;

								//이름이 일치할 경우 j번째 suffix와 일치하는지 확인
								if (std::string::npos != fileName.find(texSuffix[j]))
								{
									//일치할 경우 이 텍스처를 material에 추가
									std::fs::path texKey = _texDestDir.filename();
									texKey /= dirIter.path().filename();

									newTex = ResourceManager<Texture>::GetInst().Load(texKey);


									ASSERT(nullptr != newTex, "Texture 로드에 실패했습니다.");

									_mtrl->SetTexture((eTextureSlot)j, newTex);
								}
							}
						}
					}
				}
			}
		}
	}

	bool Model3D::SetDataToRenderer(Com_Renderer_Mesh* _renderer, UINT _idx)
	{
		if (nullptr == _renderer)
			return false;

		else if (_idx >= (UINT)m_meshContainers.size())
			return false;

		//Mesh 또는 Material은 없을 리가 없음(생성할 때 예외처리 함)
		_renderer->SetMesh(m_meshContainers[_idx].mesh);

		for (size_t i = 0; i < m_meshContainers[_idx].materials.size(); ++i)
		{
			_renderer->SetMaterial(m_meshContainers[_idx].materials[i], (UINT)i);
		}

		return true;
	}
}

