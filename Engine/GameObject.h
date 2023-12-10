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
		
		iComponent* AddComponent(const std::shared_ptr<iComponent>& _pCom);
		inline iComponent* AddComponent(const std::string_view _strKey);

		template <typename T>
		inline T* GetComponent();

		inline iComponent* GetComponent(eComponentCategory _type) { return m_Components[(int)_type].get(); }



		const std::vector<std::shared_ptr<iComponent>>& GetComponents() { return m_Components; }
		inline const std::span<std::shared_ptr<iScript>> GetScripts();

		void SetName(const std::string_view _Name) { m_Name = _Name; }
		const std::string& GetName() const { return m_Name; }
		
		void SetActive(bool _bActive);
		bool IsActive() const { return eState::Active == m_State; }

		void Destroy();
		bool IsDestroyed() const { return m_State == eState::Destroy; }
				
		bool IsDontDestroyOnLoad() const { return m_bDontDestroyOnLoad; }
		void DontDestroyOnLoad(bool _enable) { m_bDontDestroyOnLoad = _enable; }
		
		iScene* GetOwnerScene() const { return m_OwnerScene; }
		void SetOwnerScene(iScene* _scene) { m_OwnerScene = _scene; }
		bool IsInScene() const { return (nullptr != m_OwnerScene); }

		eLayerType GetLayerType() const { return m_LayerType; }

		//임의 호출하지 말것(특정 Layer에 실제로 들어가는 시점에 지정됨)
		void SetLayerType(eLayerType _type) { m_LayerType = _type; }

		//특정 게임오브젝트를 자녀로 추가. Scene에 등록해주지는 않으므로 유의
		GameObject* AddChild(const std::shared_ptr<GameObject>& _pObj);


		std::vector<std::shared_ptr<GameObject>> GetGameObjectsInHierarchy();

		bool IsMaster() const { return m_Parent.expired(); }
		GameObject* GetParent() { return m_Parent.lock().get(); }
		const std::vector<std::shared_ptr<GameObject>>& GetChilds() const { return m_Childs; }

		void SetParent(const std::shared_ptr<GameObject>& _pObj) { m_Parent = _pObj; }
		void RemoveChild(GameObject* _pObj);

		bool IsAwaken() const { return m_bAwake; }

	protected:
		void GetGameObjectsRecursive(std::vector<std::shared_ptr<GameObject>>& _gameObjects);
		void SetActiveRecursive(bool _bActive);
		void DestroyRecursive();
		

	private:
		std::string m_Name;

		iScene* m_OwnerScene;
		eLayerType m_LayerType;

		std::vector<std::shared_ptr<iComponent>>	m_Components;

		std::weak_ptr<GameObject> m_Parent;
		std::vector<std::shared_ptr<GameObject>> m_Childs;

		enum class eState
		{
			InActive,
			Active,
			Destroy
		} m_State;
		bool m_bAwake;

		bool m_bDontDestroyOnLoad;

		void SetState(eState _state) { m_State = _state; }
	};


	template <typename T>
	T* GameObject::AddComponent()
	{
		eComponentCategory order = ComMgr::GetComponentCategory<T>();

		if (eComponentCategory::UNKNOWN == order)
			return nullptr;

		std::shared_ptr<iComponent> pCom = std::make_shared<T>();
		pCom->SetComTypeID(ComMgr::GetComTypeID<T>());
		pCom->SetStrKey(ComMgr::GetComName<T>());

		//iComponent로 캐스팅해서 AddComponent 함수 호출 후 다시 T타입으로 바꿔서 반환
		return static_cast<T*>(AddComponent(pCom));
	}

	inline iComponent* GameObject::AddComponent(const std::string_view _strKey)
	{
		std::shared_ptr<iComponent> pCom = ComMgr::GetNewCom(_strKey);

		if (nullptr == pCom)
		{
			return nullptr;
		}

		return AddComponent(pCom);
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
					pCom = static_cast<T*>(m_Components[i].get());
					break;
				}
			}
		}

		//Script 아니고 Base Component 타입으로 반환을 요청한 경우
		else if constexpr (ComMgr::IsBaseComponent<T>())
		{
			eComponentCategory comCategory = ComMgr::GetComponentCategory<T>();

			//Base Component 타입으로 요청했을 경우에는 static cast 후 반환
			pCom = static_cast<T*>(m_Components[(int)comCategory].get());
		}

		//Base Component 타입으로 반환이 아닐 경우 타입 검증 후 반환
		else //constexpr
		{
			eComponentCategory comCategory = ComMgr::GetComponentCategory<T>();
			if (
				m_Components[(int)comCategory] &&
				m_Components[(int)comCategory]->GetComTypeID() == ComMgr::GetComTypeID<T>()
				)
			{
				//일단 ID값으로 비교 후 일치 시 static_cast
				pCom = static_cast<T*>(m_Components[(int)comCategory].get());
			}
		}

		return pCom;
	}





	inline const std::span<std::shared_ptr<iScript>> GameObject::GetScripts()
	{
		std::span<std::shared_ptr<iScript>> scriptSpan{};

		int ScriptSize = (int)m_Components.size() - (int)eComponentCategory::Scripts;
		if (0 < ScriptSize)
		{
			scriptSpan =
				std::span<std::shared_ptr<iScript>>((std::shared_ptr<iScript>*)m_Components.data() + (size_t)eComponentCategory::Scripts, (size_t)ScriptSize);
		}

		return scriptSpan;
	}
}

