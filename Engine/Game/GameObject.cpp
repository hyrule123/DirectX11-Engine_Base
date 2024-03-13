#include "Engine/Game/GameObject.h"

#include "Engine/Manager/SceneManager.h"

#include "Engine/Util/Serialize/json.h"

#include "Engine/Resource/Prefab.h"

#include "Engine/Game/Component/Renderer/iRenderer.h"

#include "Engine/Game/Layer.h"

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
		, m_State(eState::Active)
		, m_bAwake(false)
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
		, m_layer()
		, m_name(_other.m_name)
		, m_State(_other.m_State)
		, m_bAwake(_other.m_bAwake)
		, m_bDontDestroyOnLoad(_other.m_bDontDestroyOnLoad)
	{
		//1. 컴포넌트 목록 복사
		for (size_t i = 0; i < _other.m_baseComponents.size(); ++i)
		{
			if (_other.m_baseComponents[i])
			{
				std::shared_ptr<iComponent> cloned = std::shared_ptr<iComponent>(_other.m_baseComponents[i]->Clone());
				AddComponent(cloned);
			}
		}
	}

	GameObject::~GameObject()
	{
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
		if (false == IsActive() || m_bAwake)
		{
			return;
		}

		m_bAwake = true;
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

		//Awake의 경우 재귀적으로 호출
		const std::vector<std::shared_ptr<Com_Transform>>& childs = GetComponent<Com_Transform>()->GetChilds();
		for (size_t i = 0; i < childs.size(); ++i)
		{
			childs[i]->GetOwner()->Awake();
		}
	}

	void GameObject::Update()
	{
		if (false == IsActive())
		{
			return;
		}
			
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

	void GameObject::LateUpdate()
	{
		if (false == IsActive())
		{
			return;
		}

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
			{
				m_baseComponents[i]->LateUpdate();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->IsEnabled())
			{
				m_scripts[i]->LateUpdate();
			}
		}
	}

	//이 함수는 다른 카메라가 호출함
	//
	void GameObject::Render()
	{
		if (false == IsActive())
		{
			return;
		}

		if (m_baseComponents[(int)eComponentCategory::Renderer] && 
			m_baseComponents[(int)eComponentCategory::Renderer]->IsEnabled())
		{
			static_cast<iRenderer*>(m_baseComponents[(int)eComponentCategory::Renderer].get())->Render();
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


	std::shared_ptr<iComponent> GameObject::AddComponent(const std::shared_ptr<iComponent>& _pCom)
	{
		std::shared_ptr<iComponent> retRVO{};

		if (nullptr == _pCom)
		{
			return retRVO;
		}

		retRVO = _pCom;
		eComponentCategory ComType = _pCom->GetComponentCategory();

		ASSERT(false == _pCom->GetStrKey().empty(),
			"컴포넌트에 String Key가 없습니다.\nAddComponent<T> 또는 ComponentManager::GetNewComponent()를 통해서 생성하세요.");

		if (eComponentCategory::Scripts == ComType)
		{
			m_scripts.emplace_back(std::static_pointer_cast<iScript>(_pCom));
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
				retRVO = m_baseComponents[(int)ComType];
			}

			//컴포넌트가 들어가 있는데, 동일한 컴포넌트 ID가 아닐 경우 에러
			else
			{
				ASSERT(nullptr == m_baseComponents[(int)ComType], "이미 중복된 타입의 컴포넌트가 들어가 있습니다.");
			}
		}

		retRVO->SetOwner(this);
		retRVO->SetOwnerScene(m_ownerScene);
		retRVO->Init();

		//Active 상태이고, Awake 이미 호출되었을 경우 Awake 함수 호출
		if (IsActive() && m_bAwake)
		{
			retRVO->Awake();
		}

		return retRVO;
	}


	void GameObject::SetActive(bool _bActive)
	{
		if (IsDestroyed() || IsActive() == _bActive)
			return;

		//씬이 작동 중일 경우 람다함수를 통해 지연 실행
		if (m_ownerScene->IsAwaken())
		{
			m_ownerScene->AddFrameEndJob(&GameObject::SetActiveRecursive, this, _bActive);
		}

		//씬이 작동중이지 않을 경우 바로 호출
		else
		{
			SetActiveRecursive(_bActive);
		}

	}

	void GameObject::Destroy()
	{
		if (IsDestroyed())
			return;

		m_State = eState::Destroy;

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i])
			{
				//게임오브젝트에서 제거하는 경우에는 강제 제거
				m_baseComponents[i]->ForceDestroy();
			}
		}

		//씬이 작동 중일 경우 지연 실행
		if (m_ownerScene->IsAwaken())
		{
			m_ownerScene->AddFrameEndJob(&GameObject::DestroyRecursive, this);
		}

		//씬이 작동중이지 않을 경우 바로 호출
		else
		{
			DestroyRecursive();
		}

	}



	void GameObject::DestroyRecursive()
	{

	}

	void GameObject::SetActiveRecursive(bool _bActive)
	{
		//제거 대기 상태가 아님 && 바꾸고자 하는 상태가 현재 상태와 다른 경우에만
		if (false == IsDestroyed() && IsActive() != _bActive)
		{
			if (_bActive)
			{
				m_State = eState::Active;

				//Scene이 작동중인 상태인데 아직 Awake 함수가 호출되지 않았을 경우 Awake 함수 호출
				if (m_ownerScene->IsAwaken() && false == m_bAwake)
				{
					m_bAwake = true;

					for (size_t i = 0; i < m_baseComponents.size(); ++i)
					{
						m_baseComponents[i]->Awake();
					}
				}

				for (size_t i = 0; i < m_baseComponents.size(); ++i)
				{
					if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
						m_baseComponents[i]->OnEnable();
				}
			}
			else
			{
				m_State = eState::InActive;

				for (size_t i = 0; i < m_baseComponents.size(); ++i)
				{
					if(m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
						m_baseComponents[i]->OnDisable();
				}
			}
		}
	}

	std::shared_ptr<iScript> GameObject::GetScript(const std::string_view _strKey)
	{
		std::shared_ptr<iScript> retScript = nullptr;

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
