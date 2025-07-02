
#include "Model3D.h"

#include <Engine/Util/define_Util.h>
#include <Engine/Util/Serialize/JsonSerializer.h>

#include <Engine/Manager/ResourceManager.h>
#include <Engine/Manager/PathManager.h>

#include <Engine/Game/Scene.h>
#include <Engine/Game/GameObject.h>

#include <Engine/Game/Component/Transform.h>
#include <Engine/Game/Component/Renderer/Com_Renderer_3DAnimMesh.h>
#include <Engine/Game/Component/Animator/Com_Animator3D.h>
#include <Engine/Game/Component/Animator/Animation3D_PlayData.h>

#include <Engine/Resource/Shader/GraphicsShader.h>

#include <Engine/Resource/Texture.h>
#include <Engine/Resource/Model3D/Skeleton.h>
#include <Engine/Resource/Model3D/FBXLoader.h>
#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Default3DMtrl.h>

#include <regex>

namespace core
{
	Model3D::Model3D()
		: Super(Model3D::s_static_type_name)
	{
	}

	Model3D::~Model3D()
	{
	}


	eResult Model3D::load(const std::fs::path& _base_directory, const std::fs::path& _resource_name)
	{
		//Model3D는 다른 클래스와 저장 / 로드 방식이 약간 다름
		//예를 들어 Player를 저장한다고 하면
		//Player/Player.json 형태로 저장한다.
		std::fs::path fullPath = _base_directory / _resource_name;
		fullPath.replace_extension();
		fullPath /= _resource_name;
		fullPath.replace_extension(name::path::extension::Model3D);

		return LoadFile_Json(fullPath);
	}

	eResult Model3D::save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const
	{
		//Model3D는 다른 클래스와 저장 / 로드 방식이 약간 다름
		//예를 들어 Player를 저장한다고 하면
		//Player/Player.json 형태로 저장한다.
		std::fs::path name = _resource_name;
		name.replace_extension();
		name /= _resource_name;

		std::fs::path fullPath = _base_directory / name;
		fullPath.replace_extension(name::path::extension::Model3D);
		return SaveFile_Json(fullPath);
	}

	eResult Model3D::serialize_json(JsonSerializer* _ser) const
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
				if (m_skeleton) {
					eResult result = ResourceManager<Skeleton>::get_inst().save(m_skeleton);
					if (eResult_fail(result))
					{
						ERROR_MESSAGE("스켈레톤 정보 저장 실패!");
						return result;
					}
					ser[JSON_KEY(m_skeleton)] << m_skeleton->get_resource_name();
				}
				else {
					ser[JSON_KEY(m_skeleton)] << "";
				}
			}


			//전부 포인터 형태이므로 StrKey를 등록해 준다.
			Json::Value& arrMeshCont = (ser)[JSON_KEY(m_mesh_mtrl_pairs)];
			for (size_t i = 0; i < m_mesh_mtrl_pairs.size(); ++i)
			{
				//nullptr check
				if (nullptr == m_mesh_mtrl_pairs[i].mesh || nullptr == m_mesh_mtrl_pairs[i].material)
				{
					return eResult::Fail_InValid;
				}
				Json::Value& meshContainer = arrMeshCont[i];

				//Mesh
				eResult result = ResourceManager<Mesh>::get_inst().save(m_mesh_mtrl_pairs[i].mesh);
				if (eResult_fail(result))
				{
					ERROR_MESSAGE("메쉬 정보 저장 실패");
					return result;
				}
				meshContainer["mesh"]["s_static_type_name"] = m_mesh_mtrl_pairs[i].mesh->get_static_type_name();
				meshContainer["mesh"]["resource_name"] << m_mesh_mtrl_pairs[i].mesh->get_resource_name();

				//material
				result = ResourceManager<Material>::get_inst().save(m_mesh_mtrl_pairs[i].material);
				if (eResult_fail(result))
				{
					ERROR_MESSAGE("재질 정보 저장 실패");
					return result;
				}
				meshContainer["material"]["s_static_type_name"]
					<< m_mesh_mtrl_pairs[i].material->get_static_type_name();
				meshContainer["material"]["resource_name"]
					<< m_mesh_mtrl_pairs[i].material->get_resource_name();
			}
		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail;
		}

		return eResult::Success;
	}

	eResult Model3D::deserialize_json(const JsonSerializer* _ser)
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		m_skeleton = nullptr;
		m_mesh_mtrl_pairs.clear();

		const JsonSerializer& ser = *_ser;

		try {
			//Skeleton
			{
				std::string skeletonStrKey{};
				ser[JSON_KEY(m_skeleton)] >> skeletonStrKey;

				if (false == skeletonStrKey.empty())
				{
					m_skeleton = ResourceManager<Skeleton>::get_inst().load(skeletonStrKey);
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
				const Json::Value& arrMeshCont = (ser)[JSON_KEY(m_mesh_mtrl_pairs)];
				size_t meshContSize = (size_t)arrMeshCont.size();
				m_mesh_mtrl_pairs.resize(meshContSize);
				for (size_t i = 0; i < m_mesh_mtrl_pairs.size(); ++i)
				{
					const Json::Value& meshContainer = arrMeshCont[i];

					//Mesh
					std::string class_name;
					std::string res_name;
					meshContainer["mesh"]["resource_name"] >> res_name;
					meshContainer["mesh"]["s_static_type_name"] >> class_name;
					
					m_mesh_mtrl_pairs[i].mesh = ResourceManager<Mesh>::get_inst().load(res_name, class_name);
					if (nullptr == m_mesh_mtrl_pairs[i].mesh)
					{
						ERROR_MESSAGE("메쉬 로드 실패");
						return eResult::Fail_Open;
					}
					m_mesh_mtrl_pairs[i].mesh->set_skeleton(m_skeleton);

					//material
					class_name.clear(); res_name.clear();
					meshContainer["material"]["resource_name"] >> res_name;
					meshContainer["material"]["s_static_type_name"] >> class_name;
					m_mesh_mtrl_pairs[i].material = 
						ResourceManager<Material>::get_inst().load(res_name, class_name);

					if (nullptr == m_mesh_mtrl_pairs[i].material)
					{
						ERROR_MESSAGE("재질 로드 실패");
						return eResult::Fail_Open;
					}
				}
			}
		}
		catch (const std::exception& _err) {
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail_InValid;
		}

		return eResult::Success;
	}

	std::vector<s_ptr<GameObject>> Model3D::instantiate()
	{
		std::vector<s_ptr<GameObject>> newObjects{};

		s_ptr<GameObject> root = std::make_shared<GameObject>();
		newObjects.push_back(root);

		root->set_name(get_static_type_name());

		s_ptr<Transform> rootTransform = root->get_component<Transform>();

		//스켈레톤 있고 + 애니메이션 데이터가 있을 경우 Animator 생성
		s_ptr<Animation3D_PlayData> sharedAnimationData{};
		if (m_skeleton)
		{
			s_ptr<Com_Animator3D> rootAnimator = root->add_component<Com_Animator3D>();

			sharedAnimationData = rootAnimator->CreateSharedAnimationData();
			sharedAnimationData->set_skeleton(m_skeleton);
		}

		//사이즈가 딱 하나일 경우: GameObject 본체에 데이터를 생성
		if (1u == (UINT)m_mesh_mtrl_pairs.size())
		{
			s_ptr<Com_Renderer_Mesh> renderer = nullptr;
			if (sharedAnimationData)
			{
				//수동으로 애니메이터를 설정
				s_ptr<Com_Renderer_3DAnimMesh> renderer3D = root->add_component<Com_Renderer_3DAnimMesh>();
				renderer = static_pointer_cast<Com_Renderer_Mesh>(renderer3D);
			}
			else
			{
				renderer = root->add_component<Com_Renderer_Mesh>();
			}

			if (false == set_data_to_renderer(renderer, 0))
			{
				newObjects.clear();
				ASSERT(false, "Renderer 세팅 실패.");
				return newObjects;
			}
		}

		//여러 개의 container를 가지고 있을 경우: 하나의 부모 object에 여러개의 child를 생성해서 각각 Meshrenderer에 할당
		else
		{
			for (size_t i = 0; i < m_mesh_mtrl_pairs.size(); ++i)
			{
				newObjects.push_back(std::make_unique<GameObject>());
				GameObject* child = newObjects.back().get();
				rootTransform->add_child(child->get_component<Transform>());

				child->set_name(m_mesh_mtrl_pairs[i].mesh->get_resource_name());

				//ComponentManager로부터 Mesh 렌더러를 받아와서 MultiMesh에 넣어준다.
				s_ptr<Com_Renderer_Mesh> renderer = nullptr;
				if (sharedAnimationData)
				{
					s_ptr<Com_Animator3D> childAnimator = child->add_component<Com_Animator3D>();
					childAnimator->SetSharedAnimationData(sharedAnimationData);

					//수동으로 애니메이터를 설정
					auto renderer3D = child->add_component<Com_Renderer_3DAnimMesh>();
					renderer = std::static_pointer_cast<Com_Renderer_Mesh>(renderer3D);
				}
				else
				{
					renderer = child->add_component<Com_Renderer_Mesh>();
				}

				if (false == set_data_to_renderer(renderer, (UINT)i))
				{
					newObjects.clear();
					ASSERT(false, "Renderer 세팅 실패.");
					return newObjects;
				}
			}
		}

		return newObjects;
	}

	eResult Model3D::convert_fbx(
		const std::fs::path& _fbxPath, bool _bStatic,
		const std::fs::path& _dirAndFileName
	)
	{
		eResult result = load_from_fbx(_fbxPath, _bStatic, _dirAndFileName);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("FBX로부터 로드 실패.");
			return result;
		}


		//다른게 다 진행됐으면 저장 진행
		//키값 만들고 세팅하고
		result = ResourceManager<Model3D>::get_inst().save_as(
			std::static_pointer_cast<Model3D>(this->shared_from_this()), _dirAndFileName);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("Model3D 저장에 실패했습니다.");
			return result;
		}

		return eResult::Success;
	}

	eResult Model3D::load_from_fbx(const std::fs::path& _fbxPath, bool _bStatic, const std::fs::path& _dir_name)
	{
		if (false == std::fs::exists(_fbxPath))
		{
			ERROR_MESSAGE("파일을 찾지 못했습니다.");
			return eResult::Fail_Open;
		}
		else if (_dir_name.empty())
		{
			ERROR_MESSAGE("파일명을 설정하지 않았습니다.");
			return eResult::Fail_Open;
		}

		FBXLoader loader{};
		eResult result = loader.load_FBX(_fbxPath, _bStatic);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("FBX 불러오기 실패.");
			return result;
		}

		//Res/Model3D/Player
		std::fs::path dir_and_filename = _dir_name;
		//Res/Model3D/Player/Player
		dir_and_filename = dir_and_filename / _dir_name;
		dir_and_filename.replace_extension();

		//Bone 정보 로드
		m_skeleton = std::make_shared<Skeleton>();
		m_skeleton->set_resource_name(dir_and_filename.string());
		result = m_skeleton->create_from_FBX(&loader);
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

		const std::vector<tFBXContainer>& containers = loader.get_containers();
		for (size_t i = 0; i < containers.size(); ++i)
		{
			const tFBXContainer& cont = containers[i];

			//버텍스 버퍼 생성
			std::vector<Vertex3D> vertices_3d(cont.vecPosition.size());
			for (size_t v = 0; v < cont.vecPosition.size(); ++v)
			{
				vertices_3d[v].position = cont.vecPosition[v];

				vertices_3d[v].UV = cont.vecUV[v];

				vertices_3d[v].tangent = cont.vecTangent[v];
				vertices_3d[v].binormal = cont.vecBinormal[v];
				vertices_3d[v].Normal = cont.vecNormal[v];

				if (cont.bAnimation)
				{
					vertices_3d[v].Weights = cont.vecBlendWeight[v];
					vertices_3d[v].Indices = cont.vecBlendIndex[v];
				}
			}
			//vertex buffer은 container당 하나
			s_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>();
			vb->set_resource_name((_dir_name / cont.Name).string());
			vb->create_vertex_buffer(vertices_3d);
			//ResourceManager<VertexBuffer>::get_inst().save(vb);

			// 인덱스 정보
			for (size_t j = 0; j < cont.vecIndexBuffers.size(); ++j) {
				const auto& indices = cont.vecIndexBuffers[j];

				//메쉬
				tMeshMtrlPair mm_pair{};

				//가져올 메쉬를 생성
				mm_pair.mesh = std::make_shared<Mesh>();
				mm_pair.mesh->set_vertex_buffer(vb);
				mm_pair.mesh->create_index_buffer(indices, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//스켈레톤 주소를 지정
				mm_pair.mesh->set_skeleton(m_skeleton);

				//기본적으로는 컨테이너 이름을 사용
				//비어있을 경우 이름을 만들어준다
				std::fs::path name = _dir_name / cont.Name;
				name += "_";
				name += std::to_string(j);

				//.msh로 확장자를 변경
				//Key로 Mesh를 저장
				mm_pair.mesh->set_resource_name(
					name.string());
				//ResourceManager<Mesh>::get_inst().save(mm_pair.mesh);

				// 메테리얼 가져오기
				mm_pair.material =
					convert_material(&(cont.vecMtrl[j]), _dir_name);
				if (nullptr == mm_pair.material)
				{
					ERROR_MESSAGE("머티리얼 로드에 실패했습니다.");
					return eResult::Fail_InValid;
				}
				mm_pair.material->set_resource_name(name.string());
				//ResourceManager<Material>::get_inst().save(mm_pair.material);

				m_mesh_mtrl_pairs.push_back(mm_pair);
			}
		}
		return eResult::Success;
	}

	eResult Model3D::add_animation_from_FBX(const std::fs::path& _fbxPath, const std::fs::path& _meshDataName)
	{
		if (false == std::fs::exists(_fbxPath))
		{
			ERROR_MESSAGE("파일을 찾지 못했습니다.");
			return eResult::Fail_Open;
		}

		FBXLoader loader{};
		eResult result = loader.load_FBX(_fbxPath, false);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("FBX 불러오기 실패.");
			return result;
		}

		s_ptr<Skeleton> skeletonOfFBX = std::make_shared<Skeleton>();
		result = skeletonOfFBX->create_from_FBX(&loader);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("FBX로부터 스켈레톤 로딩 실패.");
			return result;
		}

		//지금 필요한건 FBX에 저장된 Skeleton과 Animation 정보 뿐임
		s_ptr<Skeleton> skeletonOfProj = ResourceManager<Skeleton>::get_inst().load((_meshDataName / _meshDataName).string());

		//Skeleton의 실제 경로 예시: Player/Player
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("프로젝트 스켈레톤 불러오기 실패.");
			return result;
		}

		if (false == skeletonOfProj->copy_animation_from_other((*skeletonOfFBX), _meshDataName))
		{
			ERROR_MESSAGE("스켈레톤 구조가 일치하지 않아 애니메이션을 추가할 수 없습니다.");
			return eResult::Fail;
		}

		return eResult::Success;
	}

	s_ptr<Material> Model3D::convert_material(const tFBXMaterial* _fbxMtrl, const std::fs::path& _texDestDir)
	{
		if (nullptr == _fbxMtrl || _texDestDir.empty())
		{
			return nullptr;
		}
		//material 하나 생성
		s_ptr<Material> mtrl = std::make_shared<Default3DMtrl>();

		std::fs::path name = _texDestDir.filename();
		name /= _fbxMtrl->strMtrlName;
		name.replace_extension(name::path::extension::Material);
		mtrl->set_resource_name(name.string());

		mtrl->SetDiffuseColor(_fbxMtrl->DiffuseColor);
		mtrl->SetSpecularColor(_fbxMtrl->SpecularColor);
		mtrl->SetAmbientColor(_fbxMtrl->AmbientColor);
		mtrl->SetEmissiveColor(_fbxMtrl->EmissiveColor);

		std::fs::path texDir = ResourceManager<Texture>::get_inst().GetBaseDir();
		texDir /= _texDestDir;

		//텍스처 옮기기 위한 람다 함수
		auto CopyAndLoadTex =
			[&](const std::string& _srcTexPath)->s_ptr<Texture>
			{
				//비어있을경우 return
				if (_srcTexPath.empty())
				{
					return nullptr;
				}

				std::fs::path srcTexPath = _srcTexPath;

				if (false == std::fs::exists(texDir))
				{
					std::fs::create_directories(texDir);
				}

				if (srcTexPath.has_extension() && std::fs::exists(srcTexPath))
				{
					const auto copyOption =
						std::fs::copy_options::overwrite_existing;

					std::fs::copy(srcTexPath, texDir, copyOption);
					std::fs::remove(srcTexPath);
				}

				std::fs::path texKey = _texDestDir.filename();
				texKey /= std::fs::path(_srcTexPath).filename();

				s_ptr<Texture> newTex = ResourceManager<Texture>::get_inst().load(texKey.string());
				//바로 Texture Load. 로드 실패 시 false 반환
				if (nullptr == newTex)
				{
					newTex = nullptr;
				}

				return newTex;
			};

		mtrl->set_texture(eTextureSlot::diffuse_texture, CopyAndLoadTex(_fbxMtrl->strDiffuseTex));
		mtrl->set_texture(eTextureSlot::normal_texture, CopyAndLoadTex(_fbxMtrl->strNormalTex));
		mtrl->set_texture(eTextureSlot::specular_texture, CopyAndLoadTex(_fbxMtrl->strSpecularTex));
		mtrl->set_texture(eTextureSlot::emissive_texture, CopyAndLoadTex(_fbxMtrl->strEmissiveTex));

		s_ptr<GraphicsShader> defferedShader = ResourceManager<GraphicsShader>::get_inst().find(name::defaultRes::shader::graphics::Deffered3DShader);
		mtrl->set_shader(defferedShader);

		mtrl->set_rendering_mode(eRenderingMode::deffered_opaque);

		check_MH_material(mtrl.get(), texDir);

		return mtrl;
	}

	void Model3D::check_MH_material(Material* _mtrl, const std::fs::path& _texDestDir)
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
			const s_ptr<Texture>& tex = _mtrl->get_texture((eTextureSlot)i);
			if (tex)
			{
				std::string texKey{ tex->get_resource_name() };
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
				s_ptr<Texture> tex = _mtrl->get_texture((eTextureSlot)i);
				if (tex)
				{
					for (size_t j = 0; j < texSuffixSize; ++j)
					{
						//j번째 텍스처는 없다면
						s_ptr<Texture> newTex = _mtrl->get_texture((eTextureSlot)j);
						if (nullptr == newTex)
						{
							//i번째 텍스처의 이름을 가져와서
							std::string texKey(tex->get_resource_name());

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

									newTex = ResourceManager<Texture>::get_inst().load(texKey.string());

									ASSERT(nullptr != newTex, "Texture 로드에 실패했습니다.");

									_mtrl->set_texture((eTextureSlot)j, newTex);
								}
							}
						}
					}
				}
			}
		}
	}

	bool Model3D::set_data_to_renderer(const s_ptr<Com_Renderer_Mesh>& _renderer, UINT _idx)
	{
		if (nullptr == _renderer)
			return false;

		else if (_idx >= (UINT)m_mesh_mtrl_pairs.size())
			return false;

		//Mesh 또는 Material은 없을 리가 없음(생성할 때 예외처리 함)
		_renderer->set_mesh(m_mesh_mtrl_pairs[_idx].mesh);
		_renderer->set_material(m_mesh_mtrl_pairs[_idx].material);

		return true;
	}
}

