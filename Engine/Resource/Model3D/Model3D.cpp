#include "Model3D.h"


#include "../../Util/define_Util.h"
#include "../../Util/define_Util.h"

#include "../../Manager/ResourceManager.h"
#include "../../Manager/PathManager.h"

#include "../../Game/GameObject.h"
#include "../../Game/Component/Animator/Com_DummyAnimator.h"
#include "../../Game/Component/Renderer/Com_Renderer_3DAnimMesh.h"
#include "../../Game/Component/Animator/Com_Animator3D.h"

#include "../../json-cpp/json.h"


#include "Skeleton.h"
#include "FBXLoader.h"


#include <regex>

namespace ehw
{
	Model3D::Model3D()
		: iResource(typeid(Model3D))
	{
	}

	Model3D::~Model3D()
	{
	}

	eResult Model3D::Save(const std::fs::path& _pathFromBaseDir)
	{
		eResult result = eResult::Fail;

		//Model3D는 다른 클래스와 저장 / 로드 방식이 약간 다름
		//예를 들어 Player를 저장한다고 하면
		//Player/Player.json 형태로 저장한다.
		std::fs::path fileName = _pathFromBaseDir;
		fileName.replace_extension();
		fileName = fileName / fileName;		
		fileName.replace_extension(strKey::path::extension::Model3D);
		

		const std::fs::path& basePath = ResourceManager<Model3D>::GetBaseDir();
		iResource::Save(fileName);

		std::fs::path fullPath = basePath / fileName;

		std::ofstream ofs(fullPath);
		if (false == ofs.is_open())
			return eResult::Fail_Open;

		Json::Value jVal{};
		result = SaveJson(&jVal);
		if (eResultFail(result))
			return result;
		
		ofs << jVal;
		ofs.close();

		return eResult::Success;
	}

	eResult Model3D::Load(const std::fs::path& _pathFromBaseDir)
	{
		eResult result = eResult::Fail;

		//Model3D는 다른 클래스와 저장 / 로드 방식이 약간 다름
		//예를 들어 Player를 저장한다고 하면
		//Player/Player.json 형태로 저장한다.
		std::fs::path fileName = _pathFromBaseDir;
		fileName.replace_extension();
		fileName /= fileName;
		fileName.replace_extension(strKey::path::extension::Model3D);

		const std::fs::path& basePath = ResourceManager<Model3D>::GetBaseDir();
		iResource::Load(fileName);

		std::fs::path fullPath = basePath / fileName;

		Json::Value jVal;
		fullPath.replace_extension(strKey::path::extension::Model3D);
		std::ifstream ifs(fullPath);
		if (false == ifs.is_open())
		{
			return eResult::Fail_Open;
		}

		ifs >> jVal;
		ifs.close();
		result = LoadJson(&jVal);
		if (eResultFail(result))
			return result;

		return eResult::Success;
	}


	eResult Model3D::SaveJson(Json::Value* _pJson)
	{
		if (nullptr == _pJson)
			return eResult::Fail_Nullptr;
		eResult result = iResource::SaveJson(_pJson);
		if (eResultFail(result))
			return result;

		//비어있을 경우 저장 불가
		if (m_meshContainers.empty())
		{
			return eResult::Fail_InValid;
		}


		//Skeleton
		if (m_skeleton)
		{
			result = m_skeleton->Save(m_skeleton->GetStrKey().String());
			if (eResultFail(result))
				return result;
		}
		Json::SaveLoad::SavePtrStrKey(_pJson, JSON_KEY_PAIR(m_skeleton));



		//순회 돌아주면서 array 형태의 json에 append 해준다
		Json::Value& arrMeshCont = (*_pJson)[JSON_KEY(m_meshContainers)];
		for (size_t i = 0; i < m_meshContainers.size(); ++i)
		{
			Json::Value meshCont{};
			if (nullptr == m_meshContainers[i].mesh || m_meshContainers[i].materials.empty())
			{
				ASSERT_DEBUG(nullptr == m_meshContainers[i].mesh, "메쉬 없음.");
				ASSERT_DEBUG(false == m_meshContainers[i].materials.empty(), "머티리얼 없음.");
				return eResult::Fail_InValid;
			}
			//Mesh
			result = m_meshContainers[i].mesh->Save(m_meshContainers[i].mesh->GetStrKey().String());
			if (eResultFail(result))
			{
				return result;
			}
			Json::SaveLoad::SavePtrStrKey(&meshCont, JSON_KEY(mesh), m_meshContainers[i].mesh);

			Json::Value& arrMtrl = meshCont[JSON_KEY(materials)];
			//Material 저장
			for (size_t j = 0; j < m_meshContainers[i].materials.size(); ++j)
			{
				result = m_meshContainers[i].materials[j]->Save(m_meshContainers[i].materials[j]->GetStrKey().String());

				if (eResultFail(result))
				{
					return result;
				}

				if (nullptr == m_meshContainers[i].materials[j])
				{
					return eResult::Fail_Nullptr;
				}
				else
				{
					arrMtrl.append(m_meshContainers[i].materials[j]->GetStrKey());
				}

			}

			arrMeshCont.append(meshCont);
		}

		return eResult::Success;
	}


	eResult Model3D::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
			return eResult::Fail_Nullptr;
		eResult result = iResource::LoadJson(_pJson);
		if (eResultFail(result))
			return result;
		
		m_meshContainers.clear();

		//mesh container 순회 돌아주면서 하나씩 Load
		const Json::Value& jsonMeshCont = (*_pJson)[JSON_KEY(m_meshContainers)];
		for (Json::ValueConstIterator iter = jsonMeshCont.begin();
			iter != jsonMeshCont.end();
			++iter)
		{
			tMeshContainer cont{};

			//Mesh
			std::string meshStrKey = Json::SaveLoad::LoadPtrStrKey(&(*iter), JSON_KEY(mesh), cont.mesh);
			cont.mesh = std::make_shared<Mesh>();
			result = cont.mesh->Load(meshStrKey);
			if (eResultFail(result))
			{
				return eResult::Fail_Empty;
			}
				

			//Materials
			const auto& materialsStrKey = Json::SaveLoad::LoadPtrStrKeyVector(&(*iter), JSON_KEY(materials), cont.materials);
			cont.materials.resize(materialsStrKey.size());
			for (size_t i = 0; i < materialsStrKey.size(); ++i)
			{
				cont.materials[i] = std::make_shared<Material>();
				cont.materials[i]->Load(materialsStrKey[i]);
			}


			m_meshContainers.push_back(cont);
		}

		const std::string& skeletonKey = Json::SaveLoad::LoadPtrStrKey(_pJson, JSON_KEY_PAIR(m_skeleton));
		if (false == skeletonKey.empty())
		{
			m_skeleton = std::make_shared<Skeleton>();
			result = m_skeleton->Load(skeletonKey);
			if (eResultFail(result))
				return result;
		}

		if (m_skeleton)
		{
			for (size_t i = 0; i < m_meshContainers.size(); ++i)
			{
				if (m_meshContainers[i].mesh)
				{
					m_meshContainers[i].mesh->SetSkeleton(m_skeleton);
				}
			}
		}

		return eResult::Success;
	}


	std::shared_ptr<GameObject> Model3D::Instantiate()
	{
		std::shared_ptr<GameObject> rootObj{}; 
		if (m_meshContainers.empty())
		{
			return rootObj;
		}
		rootObj = std::make_shared<GameObject>();

		if (eResultFail(Instantiate(rootObj.get())))
		{
			rootObj = nullptr;
		}

		//만들어진 GameObject 주소를 반환
		return rootObj;
	}

	eResult Model3D::Instantiate(GameObject* _emptyRootObj)
	{
		if (nullptr == _emptyRootObj)
		{
			ERROR_MESSAGE("GameObject가 nullptr 입니다.");
			return eResult::Fail_Nullptr;
		}

		std::shared_ptr<Com_Transform> tr = _emptyRootObj->AddComponent<Com_Transform>();
		ASSERT(tr, "트랜스폼 컴포넌트 생성 실패");

		//스켈레톤 있고 + 애니메이션 데이터가 있을 경우 Animator 생성
		std::shared_ptr<Com_Animator3D> animator = nullptr;
		if (m_skeleton)
		{
			animator = _emptyRootObj->AddComponent<Com_Animator3D>();
			ASSERT(animator, "애니메이터 컴포넌트 생성 실패");
			animator->SetSkeleton(m_skeleton);
		}

		//사이즈가 딱 하나일 경우: GameObject 본체에 데이터를 생성
		if (1u == (UINT)m_meshContainers.size())
		{
			std::shared_ptr<Com_Renderer_Mesh> renderer = nullptr;
			if (animator)
			{
				//수동으로 애니메이터를 설정
				std::shared_ptr<Com_Renderer_3DAnimMesh> renderer3D = _emptyRootObj->AddComponent<Com_Renderer_3DAnimMesh>();
				renderer = std::static_pointer_cast<Com_Renderer_Mesh>(renderer3D);
			}
			else
			{
				renderer = _emptyRootObj->AddComponent<Com_Renderer_Mesh>();
			}

			SetRenderer(renderer, 0);
		}

		//여러 개의 container를 가지고 있을 경우: 하나의 부모 object에 여러개의 child를 생성해서 각각 Meshrenderer에 할당
		else
		{
			for (size_t i = 0; i < m_meshContainers.size(); ++i)
			{
				std::shared_ptr<GameObject> child = std::make_shared<GameObject>();
				_emptyRootObj->AddChild(child);

				child->AddComponent<Com_Transform>();
				child->AddComponent<Com_DummyAnimator>();

				//ComponentManager로부터 Mesh 렌더러를 받아와서 MultiMesh에 넣어준다.
				std::shared_ptr<Com_Renderer_Mesh> renderer = nullptr;
				if (animator)
				{
					//수동으로 애니메이터를 설정
					auto renderer3D = child->AddComponent<Com_Renderer_3DAnimMesh>();
					renderer = std::static_pointer_cast<Com_Renderer_Mesh>(renderer3D);
				}
				else
				{
					renderer = child->AddComponent<Com_Renderer_Mesh>();
				}

				ASSERT(renderer, "renderer가 생성되지 않았습니다.");
				SetRenderer(renderer, (UINT)i);
			}
		}


		return eResult::Success;
	}

	eResult Model3D::ConvertFBX(
		const std::fs::path& _fbxPath, bool _bStatic,
		const std::fs::path& _dirAndFileName
	)
	{
		eResult result = LoadFromFBX(_fbxPath, _bStatic, _dirAndFileName);
		if (eResultFail(result))
		{
			ERROR_MESSAGE("FBX로부터 로드 실패.");
			return result;
		}



		//다른게 다 진행됐으면 저장 진행
		//키값 만들고 세팅하고
		result = Save(_dirAndFileName);
		if (eResultFail(result))
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
		if (eResultFail(result))
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
			m_skeleton->SetStrKey(skltStrKey.string());
		}
		result = m_skeleton->CreateFromFBX(&loader);

		//애니메이션 정보가 없을 경우에는 도로 제거
		if (eResult::Fail_Empty == result)
		{
			m_skeleton = nullptr;
		}
		//그 외의 이유로 실패했을 경우에는 에러
		else if (eResultFail(result))
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

			result = meshCont.mesh->CreateFromContainer(&(containers[i]));
			if (eResultFail(result))
			{
				ERROR_MESSAGE("FBX로부터 메쉬 정보를 읽어오는 데 실패했습니다.");
				return eResult::Fail;
			}

			//스켈레톤 주소를 지정
			meshCont.mesh->SetSkeleton(m_skeleton);

			if (nullptr != meshCont.mesh)
			{
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
				meshCont.mesh->SetStrKey(strKey.string());
			}

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
			m_meshContainers.push_back(meshCont);
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
		if (eResultFail(result))
		{
			ERROR_MESSAGE("FBX 불러오기 실패.");
			return result;
		}

		std::shared_ptr<Skeleton> skeletonOfFBX = std::make_shared<Skeleton>();
		result = skeletonOfFBX->CreateFromFBX(&loader);
		if (eResultFail(result))
		{
			ERROR_MESSAGE("FBX로부터 스켈레톤 로딩 실패.");
			return result;
		}

		//지금 필요한건 FBX에 저장된 Skeleton과 Animation 정보 뿐임
		std::shared_ptr<Skeleton> skeletonOfProj = std::make_shared<Skeleton>();

		//Skeleton의 실제 경로: Player/Player
		result = skeletonOfProj->Load(_meshDataName / _meshDataName);
		if (eResultFail(result))
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
		mtrl->SetStrKey(strKey.string());

		mtrl->SetMaterialCoefficient(_fbxMtrl->DiffuseColor, _fbxMtrl->SpecularColor, _fbxMtrl->AmbientColor, _fbxMtrl->EmissiveColor);

		std::fs::path texDir = ResourceManager<Texture>::GetBaseDir();
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

				std::shared_ptr<Texture> newTex = std::make_shared<Texture>();

				std::fs::path texKey = _texDestDir.filename();
				texKey /= std::fs::path(_srcTexPath).filename();

				//바로 Texture Load. 로드 실패 시 false 반환
				if (eResultFail(newTex->Load(texKey)))
				{
					newTex = nullptr;
				}

				return newTex;
			};

		mtrl->SetTexture(eTextureSlot::Albedo, CopyAndLoadTex(_fbxMtrl->strDiffuseTex));
		mtrl->SetTexture(eTextureSlot::Normal, CopyAndLoadTex(_fbxMtrl->strNormalTex));
		mtrl->SetTexture(eTextureSlot::Specular, CopyAndLoadTex(_fbxMtrl->strSpecularTex));
		mtrl->SetTexture(eTextureSlot::Emissive, CopyAndLoadTex(_fbxMtrl->strEmissiveTex));

		std::shared_ptr<GraphicsShader> defferedShader = ResourceManager<GraphicsShader>::Find(strKey::defaultRes::shader::graphics::DefferedShader);
		mtrl->SetShader(defferedShader);

		mtrl->SetRenderingMode(eRenderingMode::DefferdOpaque);

		CheckMHMaterial(mtrl, texDir);

		return mtrl;
	}

	void Model3D::CheckMHMaterial(std::shared_ptr<Material> _mtrl, const std::fs::path& _texDestDir)
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
			std::shared_ptr<Texture> tex = _mtrl->GetTexture((eTextureSlot)i);
			if (tex)
			{
				std::string texKey = tex->GetStrKey();
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
							std::string texKey = tex->GetStrKey();

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
									newTex = std::make_shared<Texture>();

									std::fs::path texKey = _texDestDir.filename();
									texKey /= dirIter.path().filename();
									newTex->SetStrKey(texKey.string());

									newTex->Load(texKey);

									_mtrl->SetTexture((eTextureSlot)j, newTex);
								}
							}
						}
					}
				}
			}
		}
	}

	bool Model3D::SetRenderer(const std::shared_ptr<Com_Renderer_Mesh>& _renderer, UINT _idx)
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

