#pragma once
#include "Engine/Entity.h"

#include "Engine/Util/Serialize/Serializable.h"

#include "Engine/Manager/ComponentManager.h"

namespace ehw
{
	class iScene;
	class Layer;
	class iScript;
	class GameObject 
		: public Entity
		, public Serializable_Json
	{
		friend class GameObject;
	public:

		GameObject();
		GameObject(const GameObject& _other);
		CLONE_ABLE(GameObject);

		virtual ~GameObject();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;
		
		void Awake();
		void Update();
		void InternalUpdate();

		void Render();
		void FrameEnd();

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
		inline std::shared_ptr<Com_Transform> Transform();



		using BaseComponents = std::array<std::shared_ptr<iComponent>, (size_t)eComponentCategory::BaseComponentEnd>;
		const BaseComponents& 
			GetComponents() const { return m_baseComponents; }

		using Scripts = std::vector<std::shared_ptr<iScript>>;
		const Scripts& GetScripts() const { return m_scripts; }

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

		eLayer GetLayerType() const { return m_layerType; }

		//임의 호출하지 말것(특정 Layer에 실제로 들어가는 시점에 지정됨)
		void SetLayerType(eLayer _type) { m_layerType = _type; }

		bool IsAwaken() const { return m_bAwake; }

	protected:
		void SetActiveRecursive(bool _bActive);
		void DestroyRecursive();
		

	private:
		std::string m_name;

		iScene* m_ownerScene;
		eLayer m_layerType;

		std::array<std::shared_ptr<iComponent>, (size_t)eComponentCategory::BaseComponentEnd>	m_baseComponents;
		std::vector<std::shared_ptr<iScript>> m_scripts;
		
		enum class eState
		{
			InActive,
			Active,
			Destroy
		} m_State;
		void SetState(eState _state) { m_State = _state; }

		bool m_bAwake;
		bool m_bDontDestroyOnLoad;
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

	inline std::shared_ptr<Com_Transform> GameObject::Transform()
	{
		return std::static_pointer_cast<Com_Transform>(m_baseComponents[(int)eComponentCategory::Transform]);
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

