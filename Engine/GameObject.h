		#pragma once
#include "Entity.h"
#include "ComMgr.h"

namespace ehw
{
	class iScene;
	class Layer;
	class GameObject : public Entity
	{
		friend class GameObject;
		friend class EventMgr;

	public:
		GameObject();
		GameObject(const GameObject& _other);
		CLONE(GameObject);

		virtual ~GameObject();

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;
		
		void Awake();
		void Update();
		void InternalUpdate();

		void Render();

	public:

		template <typename T>
		inline T* AddComponent();
		
		iComponent* AddComponent(iComponent* _pCom);
		inline iComponent* AddComponent(const std::string_view _strKey);

		template <typename T>
		inline T* GetComponent();

		inline iComponent* GetComponent(eComponentCategory _type) { return m_Components[(int)_type]; }



		const std::vector<iComponent*>& GetComponents() { return m_Components; }
		inline const std::span<iScript*> GetScripts();

		void SetName(const std::string_view _Name) { m_Name = _Name; }
		const std::string& GetName() const { return m_Name; }
		
		void SetActive(bool _bActive);
		bool IsActive() const { return eState::Active == m_CurState; }

		void Destroy();
		bool IsDestroyed() const { return m_CurState == eState::Destroy; }
				
		bool IsDontDestroyOnSceneChange() const { return m_bDontDestroyOnLoad; }
		void DontDestroyOnSceneChange(bool _enable) { m_bDontDestroyOnLoad = _enable; }
		
		iScene* GetOwnerScene() const { return m_OwnerScene; }
		void SetOwnerScene(iScene* _scene) { m_OwnerScene = _scene; }

		eLayerType GetLayerType() const { return m_LayerType; }
		void SetLayerType(eLayerType _type) { m_LayerType = _type; }

		GameObject* AddChild(GameObject* _pObj);

		void GetGameObjectHierarchy(std::vector<GameObject*>& _gameObjects);

		bool IsMaster() const { return (nullptr == m_Parent); }
		GameObject* GetParent() { return m_Parent; }
		const std::vector<GameObject*>& GetChilds() const { return m_Childs; }

		void SetParent(GameObject* _pObj) { m_Parent = _pObj; }
		void RemoveChild(GameObject* _pObj);

		bool IsAwake() const { return m_bAwake; }

		//편의성을 위한 컴포넌트 받아오기 함수
		//iTransform* Transform() { return static_cast<iTransform*>(m_Components[(int)eComponentCategory::Transform]); }
		//iCollider* Collider() { return static_cast<iCollider*>(m_Components[(int)eComponentCategory::Collider]); }
		//iAnimator* Animator() { return static_cast<iAnimator*>(m_Components[(int)eComponentCategory::Animator]); }
		//iLight* Light() { return static_cast<iLight*>(m_Components[(int)eComponentCategory::Light]); }
		//
		//Com_Camera* Camera() { return static_cast<Com_Camera*>(m_Components[(int)eComponentCategory::Camera]); }
		//iRenderer* Renderer() { return static_cast<iRenderer*>(m_Components[(int)eComponentCategory::Renderer]); }
		//Com_AudioSource* AudioSource() { return static_cast<Com_AudioSource*>(m_Components[(int)eComponentCategory::AudioSource]); }
		//Com_AudioListener* AudioListener() { return static_cast<Com_AudioListener*>(m_Components[(int)eComponentCategory::AudioListener]); }
		//Com_BehaviorTree* BehaviorTree() { return static_cast<Com_BehaviorTree*>(m_Components[(int)eComponentCategory::BehaviorTree]); }

	protected:
		void SetActiveRecursive(bool _bActive);
		void DestroyRecursive();
		

	private:
		std::string m_Name;

		iScene* m_OwnerScene;
		eLayerType m_LayerType;

		std::vector<iComponent*>	m_Components;

		GameObject* m_Parent;
		std::vector<GameObject*> m_Childs;

		enum class eState
		{
			InActive,
			Active,
			Destroy
		} m_CurState;
		eState m_PrevState;
		bool m_bAwake;

		bool m_bDontDestroyOnLoad;

		void SetState(eState _state) { m_CurState = _state; }
	};


	template <typename T>
	T* GameObject::AddComponent()
	{
		eComponentCategory order = ComMgr::GetComponentCategory<T>();

		if (eComponentCategory::UNKNOWN == order)
			return nullptr;

		T* pCom = new T;
		pCom->SetComTypeID(ComMgr::GetComTypeID<T>());
		pCom->SetKey(ComMgr::GetComName<T>());

		//iComponent로 캐스팅해서 AddComponent 함수 호출 후 다시 T타입으로 바꿔서 반환
		return static_cast<T*>(AddComponent(static_cast<iComponent*>(pCom)));
	}

	inline iComponent* GameObject::AddComponent(const std::string_view _strKey)
	{
		iComponent* pCom = ComMgr::GetNewCom(_strKey);

		if (nullptr == pCom)
		{
			return nullptr;
		}

		return AddComponent(pCom);
	}





	inline GameObject* GameObject::AddChild(GameObject* _pChild)
	{
		//nullptr이나 자기 자신을 인자로 호출했을 경우 오류 발생			
		ASSERT(_pChild, "child 포인터가 nullptr 입니다.");
		ASSERT(this != _pChild, "자기 자신을 child로 추가했습니다.");

		//부모 오브젝트가 있을 경우 기존의 부모 오브젝트에서 자신을 제거한 후 여기에 추가해야함
		GameObject* parent = _pChild->GetParent();
		if (parent)
		{
			parent->RemoveChild(_pChild);
		}
		_pChild->SetParent(this);
		m_Childs.push_back(_pChild);

		if (m_bAwake && false == _pChild->IsAwake())
		{
			_pChild->Awake();
		}

		return _pChild;
	}

	inline void GameObject::GetGameObjectHierarchy(std::vector<GameObject*>& _gameObjects)
	{
		_gameObjects.push_back(this);
		for (size_t i = 0; i < m_Childs.size(); ++i)
		{
			m_Childs[i]->GetGameObjectHierarchy(_gameObjects);
		}
	}

	inline void GameObject::RemoveChild(GameObject* _pObj)
	{
		for (auto iter = m_Childs.begin(); iter != m_Childs.end(); ++iter)
		{
			if ((*iter) == _pObj)
			{
				(*iter)->SetParent(nullptr);
				m_Childs.erase(iter);
				break;
			}
		}
	}


	template <typename T>
	T* GameObject::GetComponent()
	{
		T* pCom{};

		if constexpr (std::is_base_of_v<iScript, T>)
		{
			UINT32 comTypeID = ComMgr::GetComTypeID<T>();
			for (size_t i = (size_t)eComponentCategory::Scripts; i < m_Components.size(); ++i)
			{
				if (comTypeID == m_Components[i]->GetComTypeID())
				{
					pCom = static_cast<T*>(m_Components[i]);
					break;
				}
			}
		}
		else
		{
			eComponentCategory ComType = ComMgr::GetComponentCategory<T>();
			if (m_Components[(int)ComType])
			{
				//Base Component 타입으로 요청했을 경우에는 static cast 후 반환
				if constexpr (ComMgr::IsBaseComponent<T>())
				{
					pCom = static_cast<T*>(m_Components[(int)ComType]);
				}
				//일단 ID값으로 비교 후 일치 시 static_cast해도 안전
				else if (ComMgr::GetComTypeID<T>() == m_Components[(int)ComType]->GetComTypeID())
				{
					pCom = static_cast<T*>(m_Components[(int)ComType]);
				}
			}
		}

		return pCom;
	}





	inline const std::span<iScript*> GameObject::GetScripts()
	{
		std::span<iScript*> scriptSpan{};

		int ScriptSize = (int)m_Components.size() - (int)eComponentCategory::Scripts;
		if (0 < ScriptSize)
		{
			scriptSpan =
				std::span<iScript*>((iScript**)m_Components.data() + (size_t)eComponentCategory::Scripts, (size_t)ScriptSize);
		}

		return scriptSpan;
	}
}

