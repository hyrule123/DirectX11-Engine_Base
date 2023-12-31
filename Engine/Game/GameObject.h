#pragma once
#include "../Entity.h"



#include "../Manager/ComponentManager.h"

#include <span>


namespace ehw
{
	class iScene;
	class Layer;
	class iScript;
	class GameObject : public Entity
	{
		friend class GameObject;
	public:
		using BaseComponents = std::array<std::shared_ptr<iComponent>, (size_t)eComponentCategory::BaseComponentEnd>;
		using Scripts = std::vector<std::shared_ptr<iScript>>;

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
		inline std::shared_ptr<T> AddComponent();
		
		std::shared_ptr<iComponent> AddComponent(const std::shared_ptr<iComponent>& _pCom);
		inline std::shared_ptr<iComponent> AddComponent(const std::string_view _strKey);

		template <typename T>
		inline std::shared_ptr<T> GetComponent();

		template <typename T>
		inline std::shared_ptr<T> GetScript();

		std::shared_ptr<iScript> GetScript(const std::string_view _strKey);

		inline const std::shared_ptr<iComponent>& GetComponent(eComponentCategory _type) { return m_baseComponents[(int)_type]; }



		const BaseComponents& GetComponents() { return m_baseComponents; }
		const Scripts& GetScripts() { return m_scripts; }

		void SetName(const std::string_view _Name) { m_name = _Name; }
		const std::string& GetName() const { return m_name; }
		
		void SetActive(bool _bActive);
		bool IsActive() const { return eState::Active == m_State; }

		void Destroy();
		bool IsDestroyed() const { return m_State == eState::Destroy; }
				
		bool IsDontDestroyOnLoad() const { return m_bDontDestroyOnLoad; }
		void DontDestroyOnLoad(bool _enable) { m_bDontDestroyOnLoad = _enable; }
		
		iScene* GetOwnerScene() const { return m_ownerScene; }
		void SetOwnerScene(iScene* _scene) { m_ownerScene = _scene; }
		bool IsInScene() const { return (nullptr != m_ownerScene); }

		eLayerType GetLayerType() const { return m_layerType; }

		//임의 호출하지 말것(특정 Layer에 실제로 들어가는 시점에 지정됨)
		void SetLayerType(eLayerType _type) { m_layerType = _type; }

		//특정 게임오브젝트를 자녀로 추가. Scene에 등록해주지는 않으므로 유의
		const std::shared_ptr<GameObject>& AddChild(const std::shared_ptr<GameObject>& _pObj);


		std::vector<std::shared_ptr<GameObject>> GetGameObjectsInHierarchy();

		bool IsMaster() const { return m_parent.expired(); }
		std::shared_ptr<GameObject> GetParent() { return m_parent.lock(); }
		const std::vector<std::shared_ptr<GameObject>>& GetChilds() const { return m_childs; }

		void SetParent(const std::shared_ptr<GameObject>& _pObj) { m_parent = _pObj; }
		void RemoveChild(GameObject* _pObj);

		bool IsAwaken() const { return m_bAwake; }

	protected:
		void GetGameObjectsRecursive(std::vector<std::shared_ptr<GameObject>>& _gameObjects);
		void SetActiveRecursive(bool _bActive);
		void DestroyRecursive();
		

	private:
		std::string m_name;

		iScene* m_ownerScene;
		eLayerType m_layerType;

		BaseComponents	m_baseComponents;
		std::vector<std::shared_ptr<iScript>> m_scripts;

		std::weak_ptr<GameObject> m_parent;
		std::vector<std::shared_ptr<GameObject>> m_childs;
		
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
	std::shared_ptr<T> GameObject::AddComponent()
	{
		eComponentCategory order = T::GetComponentCategoryStatic();

		if (eComponentCategory::UNKNOWN == order)
			return nullptr;

		std::shared_ptr<iComponent> pCom = std::make_shared<T>();
		pCom->SetComponentTypeID(iComponent::GetComponentTypeID<T>());
		pCom->SetStrKey(ComponentManager::GetComponentName<T>());

		//iComponent로 캐스팅해서 AddComponent 함수 호출 후 다시 T타입으로 바꿔서 반환
		return std::static_pointer_cast<T>(AddComponent(pCom));
	}

	inline std::shared_ptr<iComponent> GameObject::AddComponent(const std::string_view _strKey)
	{
		std::shared_ptr<iComponent> pCom = ComponentManager::GetNewComponent(_strKey);

		if (nullptr == pCom)
		{
			return nullptr;
		}

		return AddComponent(pCom);
	}




	template <typename T>
	std::shared_ptr<T> GameObject::GetComponent()
	{
		std::shared_ptr<T> pCom = nullptr;

		if constexpr (std::is_base_of_v<iScript, T>)
		{
			UINT32 comTypeID = iComponent::GetComponentTypeID<T>;
			for (size_t i = 0; i < m_scripts.size(); ++i)
			{
				if (comTypeID == m_scripts[i]->GetComponentTypeID())
				{
					pCom = static_pointer_cast<T>(m_scripts[i]);
					break;
				}
			}
		}

		//Script 아니고 Base Component 타입으로 반환을 요청한 경우
		else if constexpr (T::template IsBaseComponentType<T>())
		{
			eComponentCategory comCategory = T::GetComponentCategoryStatic();

			//Base Component 타입으로 요청했을 경우에는 static cast 후 반환
			pCom = static_pointer_cast<T>(m_baseComponents[(int)comCategory]);
		}

		//Base Component 타입으로 반환이 아닐 경우 타입 검증 후 반환
		else //constexpr
		{
			eComponentCategory comCategory = T::GetComponentCategoryStatic();
			if (
				m_baseComponents[(int)comCategory] &&
				m_baseComponents[(int)comCategory]->GetComponentTypeID() == iComponent::GetComponentTypeID<T>()
				)
			{
				//일단 ID값으로 비교 후 일치 시 static_cast
				pCom = std::static_pointer_cast<T>(m_baseComponents[(int)comCategory]);
			}
		}

		return pCom;
	}
	template<typename T>
	inline std::shared_ptr<T> GameObject::GetScript()
	{
		std::shared_ptr<T> ret = nullptr;
		UINT32 comTypeID = iComponent::GetComponentTypeID<T>;
		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (comTypeID == m_scripts[i]->GetComponentTypeID())
			{
				ret = std::static_pointer_cast<T>(m_scripts[i]);
				break;
			}
		}

		return ret;
	}
}

