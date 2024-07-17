#pragma once
#include "Engine/Entity.h"

#include "Engine/Util/Serialize/Serializable.h"

#include "Engine/Manager/ComponentManager.h"

namespace ehw
{
	class Scene;
	class Script;
	class GameObject
		: public Entity
		, public Serializable_Json
	{
		friend class GameObject;
	public:
		using BaseComponents = std::array<iComponent*, (size_t)eComponentCategory::BaseComponentEnd>;
		using Scripts = std::vector<Script*>;

		enum class eState
		{
			InActive,
			Active,
			DestroyReserved,
			Destroy
		};

		GameObject();
		GameObject(const std::string_view _name);
		GameObject(const GameObject& _other);
		CLONE_ABLE(GameObject);

		virtual ~GameObject();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		void Awake();
		void FixedUpdate();
		void Update();
		void FinalUpdate();
		void Render();
		void FrameEnd();
		void RemoveDestroyed();

	public:
		template <typename T> requires std::is_base_of_v<iComponent, T>
		inline T* AddComponent();
		inline iComponent* AddComponent(std::unique_ptr<iComponent>&& _pCom);
		inline iComponent* AddComponent(const std::string_view _strKey);

		template <typename T>
		inline T* GetComponent();

		template <typename T> requires std::is_base_of_v<Script, T>
		inline T* AddScript();
		inline Script* AddScript(std::unique_ptr<Script>&& _pCom);
		inline Script* AddScript(const std::string_view _strKey);

		template <typename T>
		inline T* GetScript();

		Script* GetScript(const std::string_view _strKey);

		inline iComponent* GetComponent(eComponentCategory _type) { return m_baseComponents[(int)_type]; }
		inline Com_Transform* Transform();

		inline const BaseComponents& GetComponents() const { return m_baseComponents; }
		
		inline const Scripts& GetScripts() const { return m_scripts; }

		inline void SetName(const std::string_view _Name) { m_name = _Name; }
		inline const std::string& GetName() const { return m_name; }
		
		void SetActive(bool _bActive);
		bool IsActive() const { return eState::Active == m_state; }

		inline eState GetState() const { return m_state; }
		inline void SetState(eState _state) { m_state = _state; }

		void Destroy();
		inline bool IsDestroyed() const { return (eState::DestroyReserved <= m_state); }

		bool IsDontDestroyOnLoad() const { return m_bDontDestroyOnLoad; }
		void DontDestroyOnLoad(bool _enable) { m_bDontDestroyOnLoad = _enable; }
		
		Scene* GetScene() { return m_ownerScene; }
		Scene* GetScene() const { return m_ownerScene; }
		void SetScene(Scene* _scene) { m_ownerScene = _scene; }
		bool IsInScene() const { return (nullptr != m_ownerScene); }

		void SetLayer(uint32 _layer);
		uint32 GetLayer() const { return m_layer; }

		bool IsAwaken() const { return m_isAwakeCalled; }

		void SwapBaseComponents(GameObject& _other);

	protected:
		void SetActiveInternal(bool _bActive);

	private:
		iComponent* AddComponent(iComponent* _pCom);
		void SetComponentData(iComponent* _pCom);


	private:
		std::string m_name;

		Scene* m_ownerScene;
		uint32 m_layer;

		BaseComponents	m_baseComponents;
		Scripts m_scripts;
		
		eState m_state;

		bool m_isAwakeCalled;
		bool m_bDontDestroyOnLoad;
	};


	template <typename T> requires std::is_base_of_v<iComponent, T>
	T* GameObject::AddComponent()
	{
		eComponentCategory order = T::GetComponentCategoryStatic();

		if (false == IsComponentCategoryValid(order))
		{
			return nullptr;
		}

		std::unique_ptr<iComponent> pCom = std::make_unique<T>();
		pCom->SetComponentTypeID(iComponent::GetComponentTypeID<T>());
		pCom->SetStrKey(ComponentManager::GetInst().GetComponentName<T>());

		//iComponent로 캐스팅해서 AddComponent 함수 호출 후 다시 T타입으로 바꿔서 반환
		return static_cast<T*>(AddComponent(std::move(pCom)));
	}


	inline iComponent* GameObject::AddComponent(const std::string_view _strKey)
	{
		std::unique_ptr<iComponent> pCom = ComponentManager::GetInst().GetNewComponent(_strKey);

		if (nullptr == pCom)
		{
			return nullptr;
		}

		return AddComponent(std::move(pCom));
	}


	inline iComponent* GameObject::AddComponent(std::unique_ptr<iComponent>&& _pCom)
	{
		iComponent* ret = AddComponent(_pCom.get());

		if (_pCom.get() == ret)
		{
			_pCom.release();
		}
		else
		{
			_pCom.reset();
		}

		return ret;
	}


	template <typename T>
	T* GameObject::GetComponent()
	{
		T* pCom = nullptr;

		if constexpr (std::is_base_of_v<Script, T>)
		{
			UINT32 comTypeID = iComponent::GetComponentTypeID<T>;
			for (size_t i = 0; i < m_scripts.size(); ++i)
			{
				if (comTypeID == m_scripts[i]->GetComponentTypeID())
				{
					pCom = static_cast<T*>(m_scripts[i]);
					break;
				}
			}
		}

		//Script 아니고 Base Component 타입으로 반환을 요청한 경우
		else if constexpr (T::template IsBaseComponentType<T>())
		{
			eComponentCategory comCategory = T::GetComponentCategoryStatic();

			//Base Component 타입으로 요청했을 경우에는 static cast 후 반환
			pCom = static_cast<T*>(m_baseComponents[(int)comCategory]);
		}

		//Base Component 타입으로 반환이 아닐 경우 타입 검증 후 반환
		else //constexpr
		{
			eComponentCategory comCategory = T::GetComponentCategoryStatic();
			if (m_baseComponents[(int)comCategory])
			{
				if (iComponent::GetComponentTypeID<T>()
					== m_baseComponents[(int)comCategory]->GetComponentTypeID())
				{
					//일단 ID값으로 비교 후 일치 시 static_cast
					pCom = static_cast<T*>(m_baseComponents[(int)comCategory]);
				}
				else
				{
					//ID값으로 일치하지 않을 경우에는 dynamic_cast 수행
					pCom = dynamic_cast<T*>(m_baseComponents[(int)comCategory]);
				}

			}
		}

		return pCom;
	}

	template<typename T> requires std::is_base_of_v<Script, T>
	inline T* GameObject::AddScript()
	{
		return AddComponent<T>();
	};

	inline Script* GameObject::AddScript(std::unique_ptr<Script>&& _pCom)
	{
		return static_cast<Script*>(AddComponent(std::move(_pCom)));
	}

	inline Script* GameObject::AddScript(const std::string_view _strKey)
	{
		std::unique_ptr<iComponent> pCom = ComponentManager::GetInst().GetNewComponent(_strKey);

		if (nullptr == pCom)
		{
			return nullptr;
		}
		else if (pCom->GetComponentCategory() != eComponentCategory::Scripts) {
			return nullptr;
		}

		return static_cast<Script*>(AddComponent(std::move(pCom)));
	}

	template<typename T>
	inline T* GameObject::GetScript()
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

	inline Com_Transform* GameObject::Transform()
	{
		return static_cast<Com_Transform*>(m_baseComponents[(int)eComponentCategory::Transform]);
	};
}

