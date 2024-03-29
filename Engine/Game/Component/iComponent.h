#pragma once
#include "Engine/Entity.h"

#include "Engine/Util/SimpleMath.h"
#include "Engine/Util/Serialize/Serializable.h"

#include "Engine/Game/Component/define_Component.h"

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
	public:
		iComponent(eComponentCategory _type);

		iComponent(const iComponent& _other);
		CLONE_DISABLE(iComponent);

		virtual ~iComponent();

		virtual void Init() {};
		virtual void Awake() {};
		virtual void OnEnable() {};
		virtual void OnDisable() {};
		virtual void Start() {};
		virtual void Update() {};
		virtual void InternalUpdate() = 0;

		virtual void FrameEnd() {};

		//Raw Pointer 사용하는 이유: 생성자에서 weak_ptr 할당시 에러가 발생함.
		//또한, Component는 GameObject에 달아서 사용하는 것을 전제로 만든 클래스임.
		inline GameObject* GetOwner() { return m_Owner; }
		inline void SetOwner(GameObject* _owner) { m_Owner = _owner; }

		inline iScene* GetOwnerScene() const { return m_OwnerScene; }
		inline void SetOwnerScene(iScene* _scene) { m_OwnerScene = _scene; }

		eComponentCategory GetComponentCategory() const { return m_ComCategory; };

		void SetComponentTypeID(UINT32 _comTypeID) { m_ComTypeID = _comTypeID; }
		UINT32 GetComponentTypeID() const { return m_ComTypeID; };

		void CallStart() { if (false == m_bStart) { Start(); m_bStart = true; } }

		bool IsEnabled() const { return eState::Enabled == m_State; }
		void SetEnable(bool _bEnable);

		bool IsDestroyed() const { return eState::Destroy == m_State; }
		void Destroy();
		void ForceDestroy() { m_State = eState::Destroy; }

		template <typename T>
		static inline UINT32 GetComponentTypeID();
	private:
		static inline UINT32 GetNextComponentTypeID();

		const eComponentCategory m_ComCategory;
		UINT32 m_ComTypeID;
		iScene* m_OwnerScene;
		GameObject* m_Owner;

		bool m_bStart;

		enum class eState
		{
			Disabled,
			Enabled,
			Destroy
		} m_State;

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


	//template<class BaseComponentType, eComponentCategory _category>
	//template<class CompareType>
	//inline constexpr bool Component<BaseComponentType, _category>::IsBaseComponentType()
	//{
	//	
	//}
}
