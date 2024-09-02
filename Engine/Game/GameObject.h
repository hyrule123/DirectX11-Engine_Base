#pragma once
#include "Engine/Instance.h"

#include "Engine/Game/Component/define_Component.h"

#include "Engine/Util/Serialize/Serializable.h"
#include "Engine/Manager/InstanceManager.h"

namespace ehw
{
	class Scene;
	class Script;
	class Com_Transform;
	class iComponent;

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
		iComponent* AddComponent(iComponent* _pCom);
		template <typename T> requires std::is_base_of_v<iComponent, T>
		inline T* AddComponent();
		inline iComponent* AddComponent(const std::string_view _strKey);

		template <typename T>
		inline T* GetComponent();

		template <typename T> requires std::is_base_of_v<Script, T>
		inline T* AddScript();
		Script* AddScript(const std::string_view _strKey);

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
		
		Scene* scene() { return m_scene; }
		Scene* scene() const { return m_scene; }
		void SetScene(Scene* _scene) { m_scene = _scene; }
		bool IsInScene() const { return (nullptr != m_scene); }

		void SetLayer(uint32 _layer);
		uint32 GetLayer() const { return m_layer; }

		bool IsAwaken() const { return m_isAwaken; }

	protected:
		void SetActiveInternal(bool _bActive);

	private:
		
		void SetComponentData(iComponent* _pCom);


	private:
		std::string m_name;

		Scene* m_scene;
		uint32 m_layer;

		BaseComponents	m_baseComponents;
		Scripts m_scripts;
		
		eState m_state;

		bool m_isAwaken;
		bool m_bDontDestroyOnLoad;
	};


	template <typename T> requires std::is_base_of_v<iComponent, T>
	T* GameObject::AddComponent()
	{
		T* ret = new T;

		eComponentCategory order = T::GetComponentCategoryStatic();

		if (false == IsComponentCategoryValid(order))
		{
			delete ret;
			ret = nullptr;
			return ret;
		}

		//iComponent로 캐스팅해서 AddComponent 함수 호출 후 다시 T타입으로 바꿔서 반환
		return static_cast<T*>(AddComponent(ret));
	}

	template <typename T>
	T* GameObject::GetComponent()
	{
		T* pCom = nullptr;

		//Script는 이름을 통한 비교
		if constexpr (std::is_base_of_v<Script, T>)
		{
			pCom = GetScript<T>();
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
			pCom = dynamic_cast<T*>(m_baseComponents[(int)comCategory]);
		}

		return pCom;
	}

	template<typename T> requires std::is_base_of_v<Script, T>
	inline T* GameObject::AddScript()
	{
		return AddComponent<T>();
	};


	template<typename T>
	inline T* GameObject::GetScript()
	{
		return static_cast<T*>(GetScript(ClassInfo<T>::get_name()));
	};

	inline Com_Transform* GameObject::Transform()
	{
		return (Com_Transform*)m_baseComponents[(int)eComponentCategory::Transform];
	};
}
