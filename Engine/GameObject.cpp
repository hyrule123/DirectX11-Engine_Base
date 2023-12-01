#include "PCH_Engine.h"
#include "GameObject.h"

#include "SceneMgr.h"
#include "Layer.h"

#include "json-cpp/json.h"
#include "Prefab.h"
#include "EventMgr.h"

namespace ehw
{
	namespace strKey::Json
	{
		namespace GameObject
		{
			STRKEY_DECLARE(m_Components);
			STRKEY_DECLARE(m_Childs);
			//STRKEY_DECLARE(mScripts);
		}
	}


	GameObject::GameObject()
		: m_Components()
		, m_OwnerScene()
		, m_LayerType(eLayerType::None)
		, m_Name()
		, m_Parent()
		, m_Childs()
		, m_State(eState::Active)
		, m_bAwake()
		, m_bStart()
		, m_bDontDestroyOnSceneChange()
	{
		m_Components.reserve((int)eComponentType::Scripts + 10);
		m_Components.resize((int)eComponentType::Scripts);
	}


	GameObject::GameObject(const GameObject& _other)
		: Entity(_other)
		, m_Components()
		, m_OwnerScene(_other.m_OwnerScene)
		, m_LayerType(_other.m_LayerType)
		, m_Name(_other.m_Name)
		, m_Parent()
		, m_State(_other.m_State)
		, m_bAwake(_other.m_bAwake)
		, m_bStart(_other.m_bStart)
		, m_bDontDestroyOnSceneChange(_other.m_bDontDestroyOnSceneChange)
	{
		m_Components.reserve((int)eComponentType::Scripts + 10);
		m_Components.resize((int)eComponentType::Scripts);
		//AddComponent(&mTransform);

		//TODO: Clone
		//1. 컴포넌트 목록 복사
		for (size_t i = 0; i < _other.m_Components.size(); ++i)
		{
			if (_other.m_Components[i])
			{
				//AddComponent(_other.mFixedComponents[i]->Clone());
			}
		}

		//2. 자녀 오브젝트 복사
		for (size_t i = 0; i < _other.m_Childs.size(); ++i)
		{
			//AddChildGameObj(_other.mFixedComponents[i]->Clone());
		}
	}

	GameObject::~GameObject()
	{
		for (size_t i = 0; i < m_Components.size(); ++i)
		{
			//컴포넌트의 주인이 자신일 경우 제거
			if (m_Components[i] && this == m_Components[i]->GetOwner())
				delete m_Components[i];
		}

		for (size_t i = 0; i < m_Childs.size(); ++i)
		{
			if (m_Childs[i])
				delete m_Childs[i];
		}
	}

	eResult GameObject::SaveJson(Json::Value* _pJson)
	{
		eResult Result = Entity::SaveJson(_pJson);
		if (eResultFail(Result))
		{
			return Result;
		}

		Json::SaveLoad::SaveValue(_pJson, JSON_KEY_PAIR(m_Name));
		Json::SaveLoad::SaveValue(_pJson, JSON_KEY_PAIR(m_LayerType));
		Json::SaveLoad::SaveValue(_pJson, JSON_KEY_PAIR(m_bDontDestroyOnSceneChange));
		

		{
			(*_pJson)[strKey::Json::GameObject::m_Components] = Json::Value(Json::arrayValue);
			Json::Value& arrComponent = (*_pJson)[strKey::Json::GameObject::m_Components];
			
			//트랜스폼은 저장하지 않음
			for (size_t i = (size_t)eComponentType::Transform + (size_t)1; i < m_Components.size(); ++i)
			{
				if (m_Components[i])
				{
					Json::Value ComJson = Json::Value(Json::objectValue);
					
					Result = m_Components[i]->SaveJson(&(ComJson));
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


		//GameObject* m_Parent;
		//부모 오브젝트가 있을 경우 재귀 구조로 부모 쪽에서 생성한 뒤 자식으로 등록할 것임

		//child의 경우 별도의 프리팹으로 취급해서 새로운 파일을 생성
		//std::vector<GameObject*> m_Childs;
		{
			(*_pJson)[strKey::Json::GameObject::m_Childs] = Json::Value(Json::arrayValue);
			Json::Value& arrChilds = (*_pJson)[strKey::Json::GameObject::m_Childs];
			for (size_t i = 0; i < m_Childs.size(); ++i)
			{
				if (m_Childs[i])
				{
					//자식의 Key가 존재하지 않을 경우 자신의 Key에 숫자를 붙여서 생성
					std::string childStrKey = m_Childs[i]->GetKey();
					if (childStrKey.empty())
					{
						childStrKey = GetKey() + "_";
						childStrKey += std::to_string((int)i);
						m_Childs[i]->SetKey(childStrKey);
					}

					Prefab SavePrefab{};
					SavePrefab.RegisterPrefab(m_Childs[i], true);
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

		Json::SaveLoad::LoadValue(_pJson, JSON_KEY_PAIR(m_Name));
		Json::SaveLoad::LoadValue(_pJson, JSON_KEY_PAIR(m_LayerType));
		Json::SaveLoad::LoadValue(_pJson, JSON_KEY_PAIR(m_bDontDestroyOnSceneChange));

		//컴포넌트 추가
		if (_pJson->isMember(strKey::Json::GameObject::m_Components))
		{
			const Json::Value& jValCom = (*_pJson)[strKey::Json::GameObject::m_Components];
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

					IComponent* pCom = ComMgr::GetNewCom((*iter)[strKey::Json::Entity::mStrKey].asString());
					if (pCom)
					{
						AddComponent(pCom);
					}
				}
			}
		}

		if (_pJson->isMember(strKey::Json::GameObject::m_Childs))
		{
			const Json::Value& jValChilds = (*_pJson)[strKey::Json::GameObject::m_Childs];
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

					GameObject* child = new GameObject;
					const Json::Value& childJson = *iter;
					Result = child->LoadJson(&childJson);
					if (eResultFail(Result))
					{
						ERROR_MESSAGE_W(L"Child 오브젝트 로드 실패.");
						SAFE_DELETE(child);
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
		m_bAwake = true;
		for (size_t i = 0; i < m_Components.size(); ++i)
		{
			if (m_Components[i])
				m_Components[i]->Awake();
		}

		for (size_t i = 0; i < m_Childs.size(); ++i)
		{
			if (m_Childs[i])
				m_Childs[i]->Awake();
		}
	}


	void GameObject::Start()
	{
		m_bStart = true;

		for (size_t i = 0; i < m_Components.size(); ++i)
		{
			if (m_Components[i])
				m_Components[i]->Start();
		}

		for (size_t i = 0; i < m_Childs.size(); ++i)
		{
			if (m_Childs[i])
				m_Childs[i]->Start();
		}
	}

	void GameObject::Update()
	{
		if (false == m_bStart)
		{
			Start();
		}
			

		for (size_t i = 0; i < m_Components.size(); ++i)
		{
			if (m_Components[i])
				m_Components[i]->Update();
		}

		for (size_t i = 0; i < m_Childs.size(); ++i)
		{
			if (m_Childs[i])
				m_Childs[i]->Update();
		}
	}

	void GameObject::FixedUpdate()
	{
		for (size_t i = 0; i < m_Components.size(); ++i)
		{
			if (nullptr == m_Components[i])
				continue;
			m_Components[i]->FixedUpdate();
		}

		for (size_t i = 0; i < m_Childs.size(); ++i)
		{
			if (m_Childs[i])
				m_Childs[i]->FixedUpdate();
		}
	}

	//이 함수는 다른 카메라가 호출함
	//
	void GameObject::Render()
	{
		if (m_Components[(int)eComponentType::Renderer])
		{
			static_cast<IRenderer*>(m_Components[(int)eComponentType::Renderer])->Render();
		}
	}



	IComponent* GameObject::AddComponent(IComponent* _pCom)
	{
		if (nullptr == _pCom)
			return nullptr;

		//에러 발생시 leak 발생하지 않도록
		std::unique_ptr<IComponent> uniqPtr(_pCom);

		eComponentType ComType = _pCom->GetComType();

		ASSERT(false == _pCom->GetKey().empty(), R"(
컴포넌트에 String Key가 없습니다.
AddComponent<T> 또는 ComMgr::GetNewComponent()를 통해서 생성하세요.
)");

		if (eComponentType::Scripts == ComType)
		{
			m_Components.push_back(_pCom);
		}
		else
		{
			ASSERT(nullptr == m_Components[(int)ComType], "이미 중복된 타입의 컴포넌트가 들어가 있습니다.");

			m_Components[(int)ComType] = _pCom;
		}

		_pCom->SetOwner(this);
		_pCom->Init();

		//이미 Awake된 상태라면 바로 awake 진행
		if (IsActive() && m_bAwake)
		{
			_pCom->Awake();
		}

		return uniqPtr.release();
	}



	inline void GameObject::SetActive(bool _bActive)
	{
		if (_bActive)
		{
			m_State = eState::Active;

			if (m_OwnerScene->IsAwaken() && false == m_bAwake)
			{
				Awake();
			}
		}

		//한번 Destroy 되었으면 되돌릴 수 없음
		else if (false == IsDestroyed())
		{
			m_State = eState::InActive;
		}
	}

	void GameObject::Destroy()
	{
		EventMgr::DestroyGameObj(this);
	}
}
