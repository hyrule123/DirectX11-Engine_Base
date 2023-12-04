		#pragma once
#include "Entity.h"

#include "Components.h"
#include "ComMgr.h"
#include "Com_Transform.h"

namespace ehw
{
	class IScene;
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
		
		IComponent* AddComponent(IComponent* _pCom);
		inline IComponent* AddComponent(const std::string_view _strKey);

		template <typename T>
		inline T* GetComponent();

		inline IComponent* GetComponent(eComponentCategory _type) { return m_Components[(int)_type]; }

		template <typename T>
		inline eComponentCategory GetComponentType();

		const std::vector<IComponent*>& GetComponents() { return m_Components; }
		inline const std::span<IScript*> GetScripts();

		void SetName(const std::string_view _Name) { m_Name = _Name; }
		const std::string& GetName() const { return m_Name; }
		
		void SetActive(bool _bActive);
		bool IsActive() const { return eState::Active == m_CurState; }

		void Destroy();
		bool IsDestroyed() const { return m_CurState == eState::Destroy; }
				
		bool IsDontDestroyOnSceneChange() const { return m_bDontDestroyOnLoad; }
		void DontDestroyOnSceneChange(bool _enable) { m_bDontDestroyOnLoad = _enable; }
		
		IScene* GetOwnerScene() const { return m_OwnerScene; }
		void SetOwnerScene(IScene* _scene) { m_OwnerScene = _scene; }

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
		ITransform* Transform() { return static_cast<ITransform*>(m_Components[(int)eComponentCategory::Transform]); }
		ICollider* Collider() { return static_cast<ICollider*>(m_Components[(int)eComponentCategory::Collider]); }
		IAnimator* Animator() { return static_cast<IAnimator*>(m_Components[(int)eComponentCategory::Animator]); }
		ILight* Light() { return static_cast<ILight*>(m_Components[(int)eComponentCategory::Light]); }
		
		Com_Camera* Camera() { return static_cast<Com_Camera*>(m_Components[(int)eComponentCategory::Camera]); }
		IRenderer* Renderer() { return static_cast<IRenderer*>(m_Components[(int)eComponentCategory::Renderer]); }
		Com_AudioSource* AudioSource() { return static_cast<Com_AudioSource*>(m_Components[(int)eComponentCategory::AudioSource]); }
		Com_AudioListener* AudioListener() { return static_cast<Com_AudioListener*>(m_Components[(int)eComponentCategory::AudioListener]); }
		Com_BehaviorTree* BehaviorTree() { return static_cast<Com_BehaviorTree*>(m_Components[(int)eComponentCategory::BehaviorTree]); }

	protected:
		void SetActiveRecursive(bool _bActive);
		void DestroyRecursive();
		

	private:
		std::string m_Name;

		IScene* m_OwnerScene;
		eLayerType m_LayerType;

		std::vector<IComponent*>	m_Components;

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
		eComponentCategory order = GetComponentType<T>();

		if (eComponentCategory::UNKNOWN == order)
			return nullptr;

		T* pCom = new T;
		pCom->SetComTypeID(ComMgr::GetComTypeID<T>());
		pCom->SetKey(ComMgr::GetComName<T>());

		//IComponent로 캐스팅해서 AddComponent 함수 호출 후 다시 T타입으로 바꿔서 반환
		return static_cast<T*>(AddComponent(static_cast<IComponent*>(pCom)));
	}

	inline IComponent* GameObject::AddComponent(const std::string_view _strKey)
	{
		IComponent* pCom = ComMgr::GetNewCom(_strKey);

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

		if constexpr (std::is_base_of_v<IScript, T>)
		{
			const std::string_view name = ComMgr::GetComName<T>();
			for (size_t i = (size_t)eComponentCategory::Scripts; i < m_Components.size(); ++i)
			{
				if (name == m_Components[i]->GetKey())
				{
					pCom = static_cast<T*>(m_Components[i]);
					break;
				}
			}
		}
		else
		{
			eComponentCategory ComType = GetComponentType<T>();
			if (m_Components[(int)ComType])
			{
				//일단 ID값으로 비교 후 일치 시 static_cast해도 안전
				if (ComMgr::GetComTypeID<T>() == m_Components[(int)ComType]->GetComTypeID())
				{
					pCom = static_cast<T*>(m_Components[(int)ComType]);
				}
			}
		}

		return pCom;
	}


	template<typename T>
	inline eComponentCategory GameObject::GetComponentType()
	{
		if constexpr (std::is_base_of_v<ITransform, T>)
		{
			return eComponentCategory::Transform;
		}
		else if constexpr (std::is_base_of_v<ICollider, T>)
		{
			return eComponentCategory::Collider;
		}
		else if constexpr (std::is_base_of_v<IAnimator, T>)
		{
			return eComponentCategory::Animator;
		}
		else if constexpr (std::is_base_of_v<ILight, T>)
		{
			return eComponentCategory::Light;
		}
		else if constexpr (std::is_base_of_v<Com_Camera, T>)
		{
			return eComponentCategory::Camera;
		}
		else if constexpr (std::is_base_of_v<Com_AudioSource, T>)
		{
			return eComponentCategory::AudioSource;
		}
		else if constexpr (std::is_base_of_v<Com_AudioListener, T>)
		{
			return eComponentCategory::AudioListener;
		}
		else if constexpr (std::is_base_of_v<IRenderer, T>)
		{
			return eComponentCategory::Renderer;
		}
		else if constexpr (std::is_base_of_v<Com_BehaviorTree, T>)
		{
			return eComponentCategory::BehaviorTree;
		}
		else if constexpr (std::is_base_of_v<IScript, T>)
		{
			return eComponentCategory::Scripts;
		}

		return eComponentCategory::UNKNOWN;
	}


	inline const std::span<IScript*> GameObject::GetScripts()
	{
		std::span<IScript*> scriptSpan{};

		int ScriptSize = (int)m_Components.size() - (int)eComponentCategory::Scripts;
		if (0 < ScriptSize)
		{
			scriptSpan =
				std::span<IScript*>((IScript**)m_Components.data() + (size_t)eComponentCategory::Scripts, (size_t)ScriptSize);
		}

		return scriptSpan;
	}
}

