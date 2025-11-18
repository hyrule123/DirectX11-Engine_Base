#include "ResourceManager.h"

#include <Engine/Util/AtExit.h>

#include <Engine/Resource/Texture.h>
#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>
#include <Engine/Resource/Model3D/Model3D.h>
#include <Engine/Resource/Model3D/Skeleton.h>
#include <Engine/Resource/AudioClip.h>
#include <Engine/Resource/Animation2D.h>
#include <Engine/Resource/Shader/GraphicsShader.h>
#include <Engine/Resource/Shader/ComputeShader.h>
#include <Engine/Resource/Mesh/VertexBuffer.h>
#include <Engine/Resource/Shader/ComputeShaders/GPUInitSetting.h>

#include <Engine/GPU/ConstBuffer.h>
#include <Engine/GPU/StructBuffer.h>

#include <Engine/Game/Component/Transform.h>
#include <Engine/Game/Component/Light/Light_3D.h>

#include <fstream>


namespace core
{
	ResourceManager::ResourceManager() 
		: m_resources{}
	{}
	void ResourceManager::init()
	{
		AtExit::add_func(ResourceManager::destroy_inst);

		//에러가 발생하지 않게 디렉토리가 없을 경우 생성해주는 작업까지 진행
		m_resource_directory = std::filesystem::current_path().parent_path().parent_path().parent_path();
		m_resource_directory /= path::directory::resource::Resource;
		if (false == std::fs::exists(m_resource_directory))
		{
			std::fs::create_directories(m_resource_directory);
		}

		m_shader_directory = std::filesystem::current_path();
		m_shader_directory /= core::path::directory::CompiledShader;
		if (false == std::fs::exists(m_shader_directory))
		{
			std::fs::create_directories(m_shader_directory);
		}
	}

	ResourceManager::~ResourceManager()
	{
		Transform::release_static();
	}

	void ResourceManager::clean_unused()
	{
		for (auto& umap : m_resources)
		{
			for (auto iter = umap.begin(); iter != umap.end();)
			{
				//기본 리소스가 아니면서 카운트가 1만 남은 경우 제거
				if (iter->second.use_count() == 1)
				{
					iter = umap.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}
	}

	s_ptr<Resource> ResourceManager::find(eResourceType _res_type, const std::string_view _resource_name)
	{
		s_ptr<Resource> ret = nullptr;
		auto iter = m_resources[(int)_res_type].find(_resource_name);
		if (iter != m_resources[(int)_res_type].end())
		{
			ret = iter->second;
		}
		return ret;
	}

	s_ptr<Resource> ResourceManager::load(const std::string_view _class_name, const std::string_view _resource_name)
	{
		//클래스 생성 후 다운캐스팅
		s_ptr<Entity> ent = new_entity(_class_name);
		s_ptr<Resource> res = std::dynamic_pointer_cast<Resource>(ent);
		if (nullptr == res)
		{
			ERROR_MESSAGE("클래스 생성 실패");
			return nullptr;
		}

		//중복 리소스 검사
		eResourceType type = res->get_resource_type();
		s_ptr<Resource> found = find(type, _resource_name);
		if (found) { return found; }

		//로드 후 성공 시 반환
		res->set_res_pathkey(_resource_name);
		eResult result = load_internal(res);
		if (eResult_fail(result)) { return nullptr; }

		return res;
	}

	s_ptr<Entity> ResourceManager::load_from_json(const std::fs::path& _json_path)
	{
		std::ifstream ifs;

		std::fs::path full_path = m_resource_directory / _json_path;
		ifs.open(full_path, std::ios::binary);

		if (false == ifs.is_open())
		{
			return nullptr;
		}

		Json::Value parse;
		s_ptr<Entity> ret = nullptr;

		try
		{
			ifs >> parse;
			
			std::string concrete_class_name = parse["m_concrete_class_name"].asString();
			ret = new_entity(concrete_class_name);
		}
		catch (const Json::Exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
		}

		ifs.close();
		
		if (ret)
		{
			eResult result = ret->deserialize_json(parse);
			if (eResult_fail(result))
			{
				ERROR_MESSAGE("Serialize 실패.");
				return nullptr;
			}
		}

		return ret;
	}

	eResult ResourceManager::save(const s_ptr<Resource>& _res)
	{
		if (nullptr == _res) 
		{ 
			ERROR_MESSAGE("리소스 주소가 nullptr 입니다.");
			return eResult::Fail; 
		}
		if (_res->get_res_pathkey().empty())
		{
			ERROR_MESSAGE("리소스 주소가 지정되지 않았습니다.");
			return eResult::Fail;
		}

		eResult result = _res->save(m_resource_directory / _res->get_res_pathkey());
		return result;
	}

	eResult ResourceManager::save_as(const s_ptr<Resource>& _res, const std::string_view _res_path)
	{
		if (nullptr == _res) 
		{ 
			ERROR_MESSAGE("리소스가 nullptr 입니다.");
			return eResult::Fail_Nullptr; 
		}
		if (_res_path.empty())
		{
			ERROR_MESSAGE("경로가 없습니다.");
			return eResult::Fail_Path;
		}

		std::string before = _res->get_res_pathkey();
		_res->set_res_pathkey(_res_path);
		eResult result = _res->save(m_resource_directory / _res->get_res_pathkey());
		_res->set_res_pathkey(before); //결과와 상관없이 원래 주소로 돌려놓기
		if (eResult_fail(result)) { 
			ERROR_MESSAGE("저장 실패");
			return result; 
		}

		return eResult::Success;
	}

	eResult ResourceManager::insert(const s_ptr<Resource>& _res, const std::string_view _res_name)
	{
		if(nullptr == _res) 
		{ 
			ERROR_MESSAGE("리소스가 nullptr 입니다.");
			return eResult::Fail_Nullptr;
		}
		if (_res_name.empty())
		{
			ERROR_MESSAGE("Resource Key가 비어있습니다.");
			return eResult::Fail_Path;
		}

		if (find(_res->get_resource_type(), _res_name))
		{
			ERROR_MESSAGE("이미 같은 이름의 리소스가 있습니다.");
			return eResult::Fail_InValid;
		}
		_res->set_res_pathkey(_res_name);
		m_resources[(int32)_res->get_resource_type()][_res->get_res_pathkey()] = _res;

		return eResult::Success;
	}

	eResult ResourceManager::load_internal(const s_ptr<Resource>& _res)
	{
		if (nullptr == _res)
		{
			ERROR_MESSAGE("리소스의 포인터 주소가 nullptr 입니다.");
			return eResult::Fail;
		}
		if (_res->get_res_pathkey().empty())
		{
			ERROR_MESSAGE("Resource Key가 지정되지 않았습니다.");
			return eResult::Fail;
		}

		eResult result = _res->load(m_resource_directory / _res->get_res_pathkey());
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("리소스 로드 실패");
			return result;
		}

		m_resources[(uint32)_res->get_resource_type()][_res->get_res_pathkey()] = _res;

		return result;
	}

	void ResourceManager::load_default_resources()
	{
		create_default_buffers();
		load_default_shaders();
		load_default_textures();
		load_default_meshes();
		load_default_materials();

		Transform::init_static();
	}

	std::fs::path ResourceManager::create_resource_name_from_full_path(const std::fs::path& _fullPath)
	{
		std::fs::path strKeyPath{};
		std::fs::path newPath = _fullPath.lexically_relative(m_resource_directory);

		//비어있거나, Res 바깥의 폴더(../)이면 안됨
		if (newPath.empty() || (*newPath.begin()) == "..")
		{
			NOTIFICATION("리소스 파일은 반드시 Res 폴더의 안쪽에 있어야 합니다.");
		}
		else
		{
			for (auto iter = ++newPath.begin(); iter != newPath.end(); ++iter)
			{
				strKeyPath /= *iter;
			}
		}
		return strKeyPath;
	}
}


