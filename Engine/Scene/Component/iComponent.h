#pragma once
#include "Engine/Entity.h"

#include "Engine/Util/SimpleMath.h"
#include "Engine/Util/Serialize/Serializable.h"

#include "Engine/Scene/Component/define_Component.h"

#include <type_traits>

namespace ehw
{
	class GameObject;
	class iScene;
	class iComponent 
		: public Entity
		, public Serializable_Json
	{
		friend class ComponentManager;
		friend class GameObject;
	public:
		enum class eState
		{
			NotInitialzed,
			NotAwaken,
			Active,
			DestroyReserved,
			Destroy
		};

		iComponent(eComponentCategory _type);

		iComponent(const iComponent& _other);
		CLONE_DISABLE(iComponent);

		virtual ~iComponent();

		virtual void Init() {}
		virtual void Awake() {}
		virtual void OnEnable() {}
		virtual void Start() {}
		virtual void FixedUpdate() {}
		virtual void Update() {}
		virtual void FinalUpdate() {}

		virtual void FrameEnd() {}

		virtual void OnDisable() {}
		virtual void OnDestroy() {}

		//레이어 변경시 호출됨. GameObject::Awake() 때 한번 호출됨.
		virtual void OnLayerChange(uint32 _layer) {}

		inline GameObject* GetOwner() { return m_ownerGameObject; }
		inline GameObject* GetOwner() const { return m_ownerGameObject; }
		inline void SetOwner(GameObject* _owner) { m_ownerGameObject = _owner; }

		eComponentCategory GetComponentCategory() const { return m_ComCategory; };

		inline void SetComponentTypeID(UINT32 _comTypeID) { m_ComTypeID = _comTypeID; }
		UINT32 GetComponentTypeID() const { return m_ComTypeID; };

		inline void CallStart() { if (false == m_isStarted) { Start(); m_isStarted = true; } }

		inline bool IsInitialized() const { return eState::NotInitialzed < m_state; }

		inline bool IsEnabled() const { return m_isEnabled; }
		void SetEnable(bool _bEnable);

		bool IsDestroyed() const { return eState::DestroyReserved <= m_state; }
		inline void Destroy();

		inline eState GetState() const { return m_state; }

		inline bool UpdateDestroyState();

		template <typename T>
		static inline UINT32 GetComponentTypeID();
	private:
		static inline UINT32 GetNextComponentTypeID();
		inline void SetState(eState _state) { m_state = _state; }

		const eComponentCategory m_ComCategory;
		UINT32 m_ComTypeID;
		GameObject* m_ownerGameObject;

		eState m_state;
		bool m_isStarted;
		bool m_isEnabled;
	};


	template<typename T>
	inline UINT32 iComponent::GetComponentTypeID()
	{
		static_assert(std::is_base_of_v<iComponent, T>);
		static UINT32 typeId = GetNextComponentTypeID();
		return typeId;
	}

	inline UINT32 iComponent::GetNextComponentTypeID()
	{
		static UINT32 lastID = UINT32_MAX;
		return ++lastID;
	}

	//제한적인 업캐스팅 지원을 위한 클래스 - 상속받는 클래스들은 이 클래스를 상속
	//(iComponent를 직접 상속하지 말것)
	template <class BaseComponentType, eComponentCategory _category>
	class Component : public iComponent
	{
	public:
		Component() : iComponent(_category) {};
		virtual ~Component() {};

		template <class CompareType>
		static constexpr bool IsBaseComponentType()
		{
			return std::is_same_v<iComponent, CompareType> || std::is_same_v<BaseComponentType, CompareType>;
		}

		static constexpr eComponentCategory GetComponentCategoryStatic() { return category; }
	private:
		static constexpr eComponentCategory category = _category;
	};


	inline void iComponent::Destroy()
	{
		if (IsDestroyed())
		{
			return;
		}

		m_state = eState::DestroyReserved;
	}

	inline bool iComponent::UpdateDestroyState()
	{
		if (eState::Destroy == m_state)
		{
			return true;
		}
		else if (eState::DestroyReserved == m_state)
		{
			m_state = eState::Destroy;
		}

		return false;
	}
}
