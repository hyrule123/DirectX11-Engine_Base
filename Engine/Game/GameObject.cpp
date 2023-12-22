#include "GameObject.h"



#include "../Manager/SceneManager.h"

#include "../json-cpp/json.h"

#include "../Resource/Prefab.h"

#include "Component/Renderer/iRenderer.h"


#include "Layer.h"

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
		, m_layerType(eLayerType::None)
		, m_name()
		, m_parent()
		, m_childs()
		, m_State(eState::Active)
		, m_bAwake()
		, m_bDontDestroyOnLoad()
	{
	}


	GameObject::GameObject(const GameObject& _other)
		: Entity(_other)
		, m_baseComponents()
		, m_ownerScene(_other.m_ownerScene)
		, m_layerType(_other.m_layerType)
		, m_name(_other.m_name)
		, m_parent()
		, m_State(_other.m_State)
		, m_bAwake(_other.m_bAwake)
		, m_bDontDestroyOnLoad(_other.m_bDontDestroyOnLoad)
	{
		//AddComponent(&mTransform);

		//TODO: Clone
		//1. 컴포넌트 목록 복사
		for (size_t i = 0; i < _other.m_baseComponents.size(); ++i)
		{
			if (_other.m_baseComponents[i])
			{
				//AddComponent(_other.mFixedComponents[i]->Clone());
			}
		}

		//2. 자녀 오브젝트 복사
		for (size_t i = 0; i < _other.m_childs.size(); ++i)
		{
			//AddChildGameObj(_other.mFixedComponents[i]->Clone());
		}
	}

	GameObject::~GameObject()
	{
		//for (size_t i = 0; i < m_baseComponents.size(); ++i)
		//{
		//	if (m_baseComponents[i])
		//	{
		//		delete m_baseComponents[i];
		//	}
		//}
	}

	eResult GameObject::SaveJson(Json::Value* _pJson)
	{
		eResult Result = Entity::SaveJson(_pJson);
		if (eResultFail(Result))
		{
			return Result;
		}

		Json::SaveLoad::SaveValue(_pJson, JSON_KEY_PAIR(m_name));
		Json::SaveLoad::SaveValue(_pJson, JSON_KEY_PAIR(m_layerType));
		Json::SaveLoad::SaveValue(_pJson, JSON_KEY_PAIR(m_bDontDestroyOnLoad));
		

		{
			(*_pJson)[strKey::Json::GameObject::m_baseComponents] = Json::Value(Json::arrayValue);
			Json::Value& arrComponent = (*_pJson)[strKey::Json::GameObject::m_baseComponents];
			
			//트랜스폼은 저장하지 않음
			for (size_t i = (size_t)eComponentCategory::Transform + (size_t)1; i < m_baseComponents.size(); ++i)
			{
				if (m_baseComponents[i])
				{
					Json::Value ComJson = Json::Value(Json::objectValue);
					
					Result = m_baseComponents[i]->SaveJson(&(ComJson));
					if (eResultFail(Result))
					{
						return Result;
					}
					arrComponent.append(ComJson);
				}
				else
				{
					arrComponent.append(Json::Value(Json::nullValue));
				}
			}
		}


		//GameObject* m_parent;
		//부모 오브젝트가 있을 경우 재귀 구조로 부모 쪽에서 생성한 뒤 자식으로 등록할 것임

		//child의 경우 별도의 프리팹으로 취급해서 새로운 파일을 생성
		//std::vector<GameObject*> m_childs;
		{
			(*_pJson)[strKey::Json::GameObject::m_childs] = Json::Value(Json::arrayValue);
			Json::Value& arrChilds = (*_pJson)[strKey::Json::GameObject::m_childs];
			for (size_t i = 0; i < m_childs.size(); ++i)
			{
				if (m_childs[i])
				{
					//자식의 Key가 존재하지 않을 경우 자신의 Key에 숫자를 붙여서 생성
					std::string childStrKey = m_childs[i]->GetStrKey();
					if (childStrKey.empty())
					{
						childStrKey = GetStrKey() + "_";
						childStrKey += std::to_string((int)i);
						m_childs[i]->SetStrKey(childStrKey);
					}

					Prefab SavePrefab{};
					SavePrefab.RegisterPrefab(m_childs[i].get(), true);
					eResult Result = SavePrefab.Save(childStrKey);
					if (eResultFail(Result))
					{
						return Result;
					}

					//자식 프리팹 이름을 등록
					arrChilds.append(childStrKey);
				}
			}
		}

		return eResult::Success;
	}

	eResult GameObject::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail_Nullptr;
		}

		eResult Result = Entity::LoadJson(_pJson);
		if (eResultFail(Result))
		{
			return Result;
		}

		Json::SaveLoad::LoadValue(_pJson, JSON_KEY_PAIR(m_name));
		Json::SaveLoad::LoadValue(_pJson, JSON_KEY_PAIR(m_layerType));
		Json::SaveLoad::LoadValue(_pJson, JSON_KEY_PAIR(m_bDontDestroyOnLoad));

		//컴포넌트 추가
		if (_pJson->isMember(strKey::Json::GameObject::m_baseComponents))
		{
			const Json::Value& jValCom = (*_pJson)[strKey::Json::GameObject::m_baseComponents];
			if (jValCom.isArray())
			{
				for (Json::ValueConstIterator iter = jValCom.begin(); iter != jValCom.end(); ++iter)
				{
					if (iter->isNull())
					{
						continue;
					}

					if (false == iter->isMember(strKey::Json::Entity::mStrKey))
					{
						continue;
					}

					std::shared_ptr<iComponent> pCom = ComponentManager::GetNewComponent((*iter)[strKey::Json::Entity::mStrKey].asString());
					if (pCom)
					{
						AddComponent(pCom);
					}
				}
			}
		}

		if (_pJson->isMember(strKey::Json::GameObject::m_childs))
		{
			const Json::Value& jValChilds = (*_pJson)[strKey::Json::GameObject::m_childs];
			if (jValChilds.isArray())
			{
				for (Json::ValueConstIterator iter = jValChilds.begin(); iter != jValChilds.end(); ++iter)
				{
					if (iter->isNull())
					{
						continue;
					}

					if (false == iter->isMember(strKey::Json::Entity::mStrKey))
					{
						continue;
					}

					std::shared_ptr<GameObject> child = std::make_shared<GameObject>();
					const Json::Value& childJson = *iter;
					Result = child->LoadJson(&childJson);
					if (eResultFail(Result))
					{
						ERROR_MESSAGE_W(L"Child 오브젝트 로드 실패.");
						child = nullptr;
						return Result;
					}

					AddChild(child);
				}
			}
		}

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


		for (size_t i = 0; i < m_childs.size(); ++i)
		{
			if (m_childs[i])
				m_childs[i]->Awake();
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

		for (size_t i = 0; i < m_childs.size(); ++i)
		{
			if (m_childs[i])
				m_childs[i]->Update();
		}
	}

	void GameObject::InternalUpdate()
	{
		if (false == IsActive())
		{
			return;
		}

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
			{
				m_baseComponents[i]->InternalUpdate();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->IsEnabled())
			{
				m_scripts[i]->InternalUpdate();
			}
		}

		for (size_t i = 0; i < m_childs.size(); ++i)
		{
			if (m_childs[i])
				m_childs[i]->InternalUpdate();
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



	iComponent* GameObject::AddComponent(const std::shared_ptr<iComponent>& _pCom)
	{
		if (nullptr == _pCom)
			return nullptr;

		eComponentCategory ComType = _pCom->GetComponentCategory();

		ASSERT(false == _pCom->GetStrKey().empty(),
			"컴포넌트에 String Key가 없습니다.\nAddComponent<T> 또는 ComponentManager::GetNewComponent()를 통해서 생성하세요.");

		if (eComponentCategory::Scripts == ComType)
		{
			m_scripts.emplace_back(std::static_pointer_cast<iScript>(_pCom));
		}
		else
		{
			ASSERT(nullptr == m_baseComponents[(int)ComType], "이미 중복된 타입의 컴포넌트가 들어가 있습니다.");

			m_baseComponents[(int)ComType] = _pCom;
		}

		_pCom->SetOwner(this);
		_pCom->SetOwnerScene(m_ownerScene);
		_pCom->Init();

		//Active 상태이고, Awake 이미 호출되었을 경우 Awake 함수 호출
		if (IsActive() && m_bAwake)
		{
			_pCom->Awake();
		}

		return _pCom.get();
	}

	GameObject* GameObject::AddChild(const std::shared_ptr<GameObject>& _pChild)
	{
		//nullptr이나 자기 자신을 인자로 호출했을 경우 오류 발생			
		ASSERT_DEBUG(_pChild, "child 포인터가 nullptr 입니다.");
		ASSERT_DEBUG(this != _pChild.get(), "자기 자신을 child로 추가했습니다.");
		ASSERT_DEBUG(eLayerType::None == _pChild->GetLayerType(), "Scene에 생성되지 않은 GameObject 입니다.");

		//부모 오브젝트가 있을 경우 기존의 부모 오브젝트에서 자신을 제거한 후 여기에 추가해야함
		GameObject* parent = _pChild->GetParent();
		if (parent)
		{
			parent->RemoveChild(_pChild.get());
		}
		_pChild->SetParent(shared_from_this_T<GameObject>());
		m_childs.push_back(_pChild);

		//나는 씬에 들어갔는데 자식은 들어가지 않은 경우 자식을 넣어줌
		if (IsInScene() && false == _pChild->IsInScene())
		{
			m_ownerScene->AddNewGameObjectHierarchy(m_layerType, _pChild);
		}

		if (m_bAwake && false == _pChild->IsAwaken())
		{
			_pChild->Awake();
		}

		return _pChild.get();
	}

	std::vector<std::shared_ptr<GameObject>> GameObject::GetGameObjectsInHierarchy()
	{
		std::vector<std::shared_ptr<GameObject>> retVec {};
		
		GetGameObjectsRecursive(retVec);

		return retVec;
	}

	void GameObject::RemoveChild(GameObject* _pObj)
	{
		for (auto iter = m_childs.begin(); iter != m_childs.end(); ++iter)
		{
			if ((*iter).get() == _pObj)
			{
				(*iter)->SetParent(nullptr);
				m_childs.erase(iter);
				break;
			}
		}
	}


	void GameObject::GetGameObjectsRecursive(std::vector<std::shared_ptr<GameObject>>& _gameObjects)
	{
		_gameObjects.push_back(shared_from_this_T<GameObject>());
		for (size_t i = 0; i < m_childs.size(); ++i)
		{
			m_childs[i]->GetGameObjectsRecursive(_gameObjects);
		}
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
		m_State = eState::Destroy;

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i])
			{
				//게임오브젝트에서 제거하는 경우에는 강제 제거
				m_baseComponents[i]->ForceDestroy();
			}
		}
		
		for (size_t i = 0; i < m_childs.size(); ++i)
		{
			m_childs[i]->DestroyRecursive();
		}
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

			for (size_t i = 0; i < m_childs.size(); ++i)
			{
				m_childs[i]->SetActiveRecursive(_bActive);
			}
		}
	}
}
