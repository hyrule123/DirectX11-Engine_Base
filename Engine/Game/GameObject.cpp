
#include "GameObject.h"

#include <Engine/Manager/SceneManager.h>

#include <Engine/Util/Serialize/json.h>

#include <Engine/Resource/Prefab.h>

#include <Engine/Game/Component/Renderer/Renderer.h>

#include <Engine/Util/StringConverter.h>

#include <Engine/Game/Component/Component.h>
#include <Engine/Game/Component/Script/Script.h>
#include <Engine/Game/Component/Transform.h>

namespace core
{
	GameObject::GameObject()
		: Entity(GameObject::s_static_type_name)
	{
	}

	GameObject::GameObject(const std::string_view _name)
		: GameObject()
	{
		m_name = _name;
	}

	GameObject::GameObject(const GameObject& _other)
		: Entity(_other)
		, m_baseComponents()
		, m_scene(_other.m_scene)
		, m_layer(_other.m_layer)
		, m_name(_other.m_name)
		, m_state(_other.m_state)
		, m_isAwaken(_other.m_isAwaken)
		, m_bDontDestroyOnLoad(_other.m_bDontDestroyOnLoad)
	{
		//1. 컴포넌트 목록 복사
		for (size_t i = 0; i < _other.m_baseComponents.size(); ++i)
		{
			if (_other.m_baseComponents[i])
			{
				//상대방의 m_baseComponents에 들어왔다는건 AddComponent가 호출되었다는뜻.
				//그냥 복사한뒤 주인만 바꾸면 됨.
				m_baseComponents[i] = std::static_pointer_cast<Component>(_other.m_baseComponents[i]->clone());

				if (m_baseComponents[i])
				{
					s_ptr<GameObject> my = std::static_pointer_cast<GameObject>(this->shared_from_this());
					m_baseComponents[i]->set_game_object(my);
				}
			}
		}


		//복사 시 clone 불가능한 Script는 복사하지 않는다.
		for (size_t i = 0; i < _other.m_scripts.size(); ++i)
		{
			s_ptr<Script> cloned = std::static_pointer_cast<Script>(_other.m_scripts[i]->clone());
			if (cloned)
			{
				s_ptr<GameObject> my = std::static_pointer_cast<GameObject>(this->shared_from_this());
				cloned->set_game_object(my);
				m_scripts.push_back(cloned);
			}
#ifdef _DEBUG
			else
			{
				std::wstringstream stream{};
				stream << StringConverter::UTF8_to_UTF16(_other.m_scripts[i]->get_static_type_name());
				stream << L"스크립트 복사 실패.\n";
				DEBUG_LOG_W(stream.str().c_str());
			}
#endif
		}
	}

	GameObject::~GameObject()
	{
	}

	eResult GameObject::serialize_json(JsonSerializer* _ser) const
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		JsonSerializer& ser = *_ser;

		ser[JSON_KEY(m_name)] << m_name;
		ser[JSON_KEY(m_layer)] << m_layer;
		ser[JSON_KEY(m_bDontDestroyOnLoad)] << m_bDontDestroyOnLoad;

		ASSERT(false, "미구현");

		return eResult::Success;
	}

	eResult GameObject::deserialize_json(const JsonSerializer* _ser)
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		const JsonSerializer& ser = *_ser;

		ASSERT(false, "미구현");

		return eResult::Success;
	}

	void GameObject::init()
	{
		Super::init();

		m_baseComponents;
		m_scene;
		m_layer = UINT_MAX;
		m_name;
		m_state = eState::Active;
		m_isAwaken = false;
		m_bDontDestroyOnLoad = false;

		add_component<Transform>();
	}

	void GameObject::awake()
	{
		if (m_isAwaken || false == is_active())
		{
			return;
		}

		m_isAwaken = true;
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i])
			{
				m_baseComponents[i]->awake();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			m_scripts[i]->awake();
		}

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->is_enabled())
			{
				m_baseComponents[i]->on_enable();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i]->is_enabled())
			{
				m_scripts[i]->on_enable();
			}
		}

		//on_layer_change 호출
		set_layer(m_layer);

		//Awake의 경우 재귀적으로 호출
		const std::vector<w_ptr<Transform>>& childs = get_component<Transform>()->get_childs();
		for (size_t i = 0; i < childs.size(); ++i)
		{
			auto child = childs[i].lock();
			if (childs[i].lock())
			{
				auto owner = child->get_owner();
				if (owner)
				{
					owner->awake();
				}
			}
		}
	}

	void GameObject::update()
	{
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->is_enabled())
			{
				if (false == m_baseComponents[i]->is_started()) {
					m_baseComponents[i]->start();
				}
				m_baseComponents[i]->update();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->is_enabled())
			{
				if (false == m_scripts[i]->is_started()) {
					m_scripts[i]->start();
				}
				m_scripts[i]->update();
			}
		}
	}

	void GameObject::fixed_update()
	{
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->is_enabled())
			{
				m_baseComponents[i]->fixed_update();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->is_enabled())
			{
				m_scripts[i]->fixed_update();
			}
		}
	}



	void GameObject::final_update()
	{
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->is_enabled())
			{
				m_baseComponents[i]->final_update();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->is_enabled())
			{
				m_scripts[i]->final_update();
			}
		}
	}


	void GameObject::remove_destroyed()
	{
		//람다 쓰는 이유: BaseComponents와 Scripts 두군데에 적용해야함
		auto needDestroyPred =
			[](const s_ptr<Component>& _com)->bool
			{
				Component::eState state = _com->get_state();

				if (Component::eState::DestroyReserved == state)
				{
					_com->set_enable(false);
					_com->set_state(Component::eState::destroy);
					return false;
				}
				else if (Component::eState::destroy == state)
				{
					_com->on_destroy();
					return true;
				}

				return false;
			};

		//BaseComponents
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i])
			{
				if (needDestroyPred(m_baseComponents[i]))
				{
					m_baseComponents[i] = nullptr;
				}
			}
		}

		//Scripts
		std::erase_if(m_scripts, needDestroyPred);
	}


	//이 함수는 다른 카메라가 호출함
	void GameObject::render()
	{
		if (false == is_active())
		{
			return;
		}

		if (m_baseComponents[(int)eComponentOrder::Renderer] && 
			m_baseComponents[(int)eComponentOrder::Renderer]->is_enabled())
		{
			//static_cast<Renderer*>(m_baseComponents[(int)eComponentOrder::Renderer])->render();
		}
	}

	void GameObject::frame_end()
	{
		if (false == is_active())
		{
			return;
		}

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->is_enabled())
			{
				m_baseComponents[i]->frame_end();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->is_enabled())
			{
				m_scripts[i]->frame_end();
			}
		}
	}

	s_ptr<Transform> GameObject::get_transform() const
	{
		return std::static_pointer_cast<Transform>(get_component(eComponentOrder::Transform));
	}

	s_ptr<Component> GameObject::add_component(const s_ptr<Component>& _pCom)
	{
		if (nullptr == _pCom) { return nullptr; }

		eComponentOrder ComType = _pCom->get_component_order();
		
		if (eComponentOrder::Script == ComType)
		{
			m_scripts.push_back(static_pointer_cast<Script>(_pCom));
		}
		else
		{
#ifdef _DEBUG
			if (m_baseComponents[(int)ComType])
			{
				ERROR_MESSAGE("동일한 Component가 들어가 있습니다.");
			}
#endif
			m_baseComponents[(int)ComType] = _pCom;
		}

		s_ptr<GameObject> my = std::static_pointer_cast<GameObject>(this->shared_from_this());
		_pCom->set_game_object(my);

		if (false == _pCom->is_initialized())
		{
			_pCom->init();
			_pCom->set_state(Component::eState::NotAwaken);
		}

		//Active 상태이고, awake 이미 호출되었을 경우 Awake 함수 호출
		if (is_active() && m_isAwaken)
		{
			_pCom->awake();
		}

		return _pCom;
	}

	s_ptr<Component> GameObject::add_component(const std::string_view _resource_name)
	{
		s_ptr<Component> ret = 
			std::dynamic_pointer_cast<Component>(EntityFactory::get_inst().instantiate(_resource_name));
		return ret;
	}

	s_ptr<Script> GameObject::add_script(const std::string_view _resource_name)
	{
		s_ptr<Script> ret =
			std::dynamic_pointer_cast<Script>(EntityFactory::get_inst().instantiate(_resource_name));
		return ret;
	}

	void GameObject::set_active(bool _bActive)
	{
		if (is_destroyed() || is_active() == _bActive)
		{
			return;
		}

		//씬이 작동 중일 경우 람다함수를 통해 지연 실행
		s_ptr<Scene> scene = m_scene.lock();
		ASSERT_DEBUG(scene, "속해있는 scene이 없습니다.");
		if (scene->is_awaken())
		{
			scene->add_frame_end_job(&GameObject::set_active_internal, this, _bActive);
		}

		//씬이 작동중이지 않을 경우 바로 호출
		else
		{
			set_active_internal(_bActive);
		}

		//자식이 있을경우 전부 InActive
		const auto& childs = get_component<Transform>()->get_gameobject_hierarchy();
		for (size_t i = 0; i < childs.size(); ++i)
		{
			childs[i]->set_active(false);
		}
	}

	void GameObject::destroy()
	{
		if (is_destroyed())
		{
			return;
		}

		m_state = eState::DestroyReserved;

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i])
			{
				m_baseComponents[i]->destroy();
			}
		}
		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i])
			{
				m_scripts[i]->destroy();
			}
		}

		s_ptr<Transform> tr = get_component<Transform>();
		tr->unlink_parent();
		tr->destroy_childs_recursive();
	}

	void GameObject::set_layer(uint32 _layer)
	{

		m_layer = _layer;

		if (m_isAwaken)
		{
			for (size_t i = 0; i < m_baseComponents.size(); ++i)
			{
				if (m_baseComponents[i])
				{
					m_baseComponents[i]->on_layer_change(_layer);
				}
			}
			for (size_t i = 0; i < m_scripts.size(); ++i)
			{
				m_scripts[i]->on_layer_change(_layer);
			}
		}
	}

	void GameObject::set_active_internal(bool _bActive)
	{
		//제거 대기 상태가 아님 && 바꾸고자 하는 상태가 현재 상태와 다른 경우에만
		if (false == is_destroyed() && is_active() != _bActive)
		{
			if (_bActive)
			{
				m_state = eState::Active;

				s_ptr<Scene> scene = m_scene.lock();
				ASSERT_DEBUG(scene, "속해있는 scene이 없습니다.");
				//Scene이 작동중인 상태인데 아직 awake 함수가 호출되지 않았을 경우 Awake 함수 호출
				if (scene->is_awaken() && false == m_isAwaken)
				{
					m_isAwaken = true;

					for (size_t i = 0; i < m_baseComponents.size(); ++i)
					{
						m_baseComponents[i]->awake();
					}
				}

				for (size_t i = 0; i < m_baseComponents.size(); ++i)
				{
					if (m_baseComponents[i] && m_baseComponents[i]->is_enabled())
					{
						m_baseComponents[i]->on_enable();
					}
						
				}
			}
			else
			{
				m_state = eState::InActive;

				//InActive 상태가 되면 OnDisable은 호출되지만, 각 컴포넌트의 Enabled 상태는 변하지 않음.
				for (size_t i = 0; i < m_baseComponents.size(); ++i)
				{
					if (m_baseComponents[i] && m_baseComponents[i]->is_enabled())
					{
						m_baseComponents[i]->on_disable();
					}
				}
			}
		}
	}

	s_ptr<Script> GameObject::get_script(const std::string_view _resource_name)
	{
		s_ptr<Script> retScript = nullptr;

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (_resource_name == m_scripts[i]->get_static_type_name())
			{
				retScript = m_scripts[i];
				break;
			}
		}

		return retScript;
	}

}
