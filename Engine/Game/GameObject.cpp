#include "Engine/Game/GameObject.h"

#include "Engine/Manager/SceneManager.h"

#include "Engine/Util/Serialize/json.h"

#include "Engine/Resource/Prefab.h"

#include "Engine/Game/Component/Renderer/iRenderer.h"

#include "Engine/Game/Layer.h"

#include "Engine/Util/StringConverter.h"

namespace ehw
{
	namespace strKey::Json
	{
		namespace GameObject
		{
			STRKEY_DECLARE(m_baseComponents);
			STRKEY_DECLARE(m_childs);
		}
	}

	GameObject::GameObject()
		: m_baseComponents()
		, m_ownerScene()
		, m_layer()
		, m_name()
		, m_state(eState::Active)
		, m_isAwakeCalled(false)
		, m_bDontDestroyOnLoad(false)
	{
		AddComponent<Com_Transform>();
	}

	GameObject::GameObject(const std::string_view _name)
		: GameObject()
	{
		m_name = _name;
	}

	GameObject::GameObject(const GameObject& _other)
		: Entity(_other)
		, m_baseComponents()
		, m_ownerScene(_other.m_ownerScene)
		, m_layer(_other.m_layer)
		, m_name(_other.m_name)
		, m_state(_other.m_state)
		, m_isAwakeCalled(_other.m_isAwakeCalled)
		, m_bDontDestroyOnLoad(_other.m_bDontDestroyOnLoad)
	{
		//1. 컴포넌트 목록 복사
		for (size_t i = 0; i < _other.m_baseComponents.size(); ++i)
		{
			if (_other.m_baseComponents[i])
			{
				//상대방의 m_baseComponents에 들어왔다는건 AddComponent가 호출되었다는뜻.
				//그냥 복사한뒤 주인만 바꾸면 됨.
				m_baseComponents[i] = static_cast<iComponent*>(_other.m_baseComponents[i]->Clone());

				if (m_baseComponents[i])
				{
					m_baseComponents[i]->SetOwner(this);
				}
			}
		}


		//복사 시 Clone 불가능한 Script는 복사하지 않는다.
		for (size_t i = 0; i < _other.m_scripts.size(); ++i)
		{
			iScript* cloned = static_cast<iScript*>(_other.m_scripts[i]->Clone());
			if (cloned)
			{
				m_scripts.push_back(cloned);
				m_scripts.back()->SetOwner(this);
			}
#ifdef _DEBUG
			else
			{
				std::wstringstream stream{};
				stream << StringConverter::UTF8_to_Unicode(_other.m_scripts[i]->GetStrKey());
				stream << L"스크립트 복사 실패.\n";
				DEBUG_LOG_W(stream.str().c_str());
			}
#endif
		}
	}


	GameObject::~GameObject()
	{
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			iComponent* com = m_baseComponents[i];
			if (com)
			{
				delete com;
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			iScript* script = m_scripts[i];
			if (script)
			{
				delete script;
			}
		}
	}



	eResult GameObject::Serialize_Json(JsonSerializer* _ser) const
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

	eResult GameObject::DeSerialize_Json(const JsonSerializer* _ser)
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

	void GameObject::Awake()
	{
		if (m_isAwakeCalled || false == IsActive())
		{
			return;
		}

		m_isAwakeCalled = true;
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i])
			{
				m_baseComponents[i]->Awake();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			m_scripts[i]->Awake();
		}

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
			{
				m_baseComponents[i]->OnEnable();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i]->IsEnabled())
			{
				m_scripts[i]->OnEnable();
			}
		}
		SetLayer(m_layer);

		//Awake의 경우 재귀적으로 호출
		const std::vector<Com_Transform*>& childs = GetComponent<Com_Transform>()->GetChilds();
		for (size_t i = 0; i < childs.size(); ++i)
		{
			childs[i]->GetOwner()->Awake();
		}
	}

	void GameObject::Update()
	{
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{

			if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
			{
				m_baseComponents[i]->CallStart();
				m_baseComponents[i]->Update();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->IsEnabled())
			{
				m_scripts[i]->CallStart();
				m_scripts[i]->Update();
			}
		}
	}

	void GameObject::CollisionUpdate()
	{
		if (m_baseComponents[(int)eComponentCategory::Collider])
		{
			//std::static_pointer_cast<iCollider>(m_baseComponents[(int)eComponentCategory::Collider])
				//->CollisionUpdate();
		}
	}

	void GameObject::FinalUpdate()
	{
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
			{
				m_baseComponents[i]->FinalUpdate();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->IsEnabled())
			{
				m_scripts[i]->FinalUpdate();
			}
		}
	}


	void GameObject::RemoveDestroyed()
	{
		auto needDestroyPred =
			[](iComponent* const _com)->bool
			{
				iComponent::eState state = _com->GetState();

				if (iComponent::eState::DestroyReserved == state)
				{
					_com->SetEnable(false);
					_com->SetState(iComponent::eState::Destroy);
					return false;
				}
				else if (iComponent::eState::Destroy == state)
				{
					_com->OnDestroy();
					delete _com;
					return true;
				}

				return false;
			};

		//개별 Component Destroy 여부 확인 후 true 반환될 시 제거
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && needDestroyPred(m_baseComponents[i]))
			{
				m_baseComponents[i] = nullptr;
			}
		}

		std::erase_if(m_scripts, needDestroyPred);
	}


	//이 함수는 다른 카메라가 호출함
	void GameObject::Render()
	{
		if (false == IsActive())
		{
			return;
		}

		if (m_baseComponents[(int)eComponentCategory::Renderer] && 
			m_baseComponents[(int)eComponentCategory::Renderer]->IsEnabled())
		{
			static_cast<iRenderer*>(m_baseComponents[(int)eComponentCategory::Renderer])->Render();
		}
	}

	void GameObject::FrameEnd()
	{
		if (false == IsActive())
		{
			return;
		}

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
			{
				m_baseComponents[i]->FrameEnd();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->IsEnabled())
			{
				m_scripts[i]->FrameEnd();
			}
		}
	}


	iComponent* GameObject::AddComponent(iComponent* _pCom)
	{
		iComponent* ret = nullptr;
		if (nullptr == _pCom)
		{
			return ret;
		}

		ret = _pCom;
		eComponentCategory ComType = _pCom->GetComponentCategory();

		ASSERT_DEBUG(false == _pCom->GetStrKey().empty(),
			"컴포넌트에 String Key가 없습니다.\nAddComponent<T> 또는 ComponentManager::GetNewComponent()를 통해서 생성하세요.");

		
		if (eComponentCategory::Scripts == ComType)
		{
			m_scripts.push_back(static_cast<iScript*>(_pCom));
		}
		else
		{
			//해당 컴포넌트 카테고리가 비어있을 경우 컴포넌트를 집어넣는다.
			if (nullptr == m_baseComponents[(int)ComType])
			{
				m_baseComponents[(int)ComType] = _pCom;
			}

			//동일한 ID의 컴포넌트가 컴포넌트 카테고리 안에 들어가 있을경우 해당 컴포넌트를 반환한다.
			else if (_pCom->GetComponentTypeID() == m_baseComponents[(int)ComType]->GetComponentTypeID())
			{
				_pCom = nullptr;
				return m_baseComponents[(int)ComType];
			}

			//컴포넌트가 들어가 있는데, 동일한 컴포넌트 ID가 아닐 경우 에러
			else
			{
				ASSERT_DEBUG(nullptr == m_baseComponents[(int)ComType], "이미 중복된 타입의 컴포넌트가 들어가 있습니다.");
			}
		}

		ret->SetOwner(this);

		if (false == ret->IsInitialized())
		{
			ret->Init();
			ret->SetState(iComponent::eState::NotAwaken);
		}

		//Active 상태이고, Awake 이미 호출되었을 경우 Awake 함수 호출
		if (IsActive() && m_isAwakeCalled)
		{
			ret->Awake();
		}

		return ret;
	}



	void GameObject::SetActive(bool _bActive)
	{
		if (IsDestroyed() || IsActive() == _bActive)
		{
			return;
		}

		//씬이 작동 중일 경우 람다함수를 통해 지연 실행
		if (m_ownerScene->IsAwaken())
		{
			m_ownerScene->AddFrameEndJob(&GameObject::SetActiveInternal, this, _bActive);
		}

		//씬이 작동중이지 않을 경우 바로 호출
		else
		{
			SetActiveInternal(_bActive);
		}

		//자식이 있을경우 전부 InActive
		const auto& childs = Transform()->GetGameObjectHierarchy();
		for (size_t i = 0; i < childs.size(); ++i)
		{
			childs[i]->SetActive(false);
		}
	}

	void GameObject::Destroy()
	{
		if (IsDestroyed())
		{
			return;
		}

		m_state = eState::DestroyReserved;

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i])
			{
				m_baseComponents[i]->Destroy();
			}
		}
		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i])
			{
				m_scripts[i]->Destroy();
			}
		}

		Com_Transform* const tr = Transform();
		tr->UnlinkParent();
		tr->DestroyChildsRecursive();
	}

	void GameObject::SwapBaseComponents(GameObject& _other)
	{
		Entity::Swap(_other);

		m_baseComponents.swap(_other.m_baseComponents);
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			m_baseComponents[i]->SetOwner(this);
			_other.m_baseComponents[i]->SetOwner(&_other);
		}
	}

	void GameObject::SetActiveInternal(bool _bActive)
	{
		//제거 대기 상태가 아님 && 바꾸고자 하는 상태가 현재 상태와 다른 경우에만
		if (false == IsDestroyed() && IsActive() != _bActive)
		{
			if (_bActive)
			{
				m_state = eState::Active;

				//Scene이 작동중인 상태인데 아직 Awake 함수가 호출되지 않았을 경우 Awake 함수 호출
				if (m_ownerScene->IsAwaken() && false == m_isAwakeCalled)
				{
					m_isAwakeCalled = true;

					for (size_t i = 0; i < m_baseComponents.size(); ++i)
					{
						m_baseComponents[i]->Awake();
					}
				}

				for (size_t i = 0; i < m_baseComponents.size(); ++i)
				{
					if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
					{
						m_baseComponents[i]->OnEnable();
					}
						
				}
			}
			else
			{
				m_state = eState::InActive;

				//InActive 상태가 되면 OnDisable은 호출되지만, 각 컴포넌트의 Enabled 상태는 변하지 않음.
				for (size_t i = 0; i < m_baseComponents.size(); ++i)
				{
					if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
					{
						m_baseComponents[i]->OnDisable();
					}
				}
			}
		}
	}

	iScript* GameObject::GetScript(const std::string_view _strKey)
	{
		iScript* retScript = nullptr;

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (_strKey == m_scripts[i]->GetStrKey())
			{
				retScript = m_scripts[i];
				break;
			}
		}

		return retScript;
	}

}
