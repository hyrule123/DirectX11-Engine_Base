#pragma once
#include "Engine/define_enum.h"
#include "Engine/Util/type_traits_Ex.h"
#include "Engine/CommonType.h"

#include "Engine/Manager/Collision/Collision2D.h"
#include "Engine/Manager/Collision/Collision3D.h"

#include <unordered_map>
#include <array>
#include <bitset>

//업데이트 순서
//1. SceneManager::Update: 각 Collider들이 자신의 정보를 등록한다.
//2. CollisionManager::FinalUpdate: 등록된 Collider에 대한 충돌검사를 시행한다.(여기서 충돌 관련 연산이 완료 되어야함)
//3. SceneManager::FinalUpdate: 최종적으로 Trasnfrom 위치를 갱신하고, 각 컴포넌트의 렌더링 준비를 마친다.

//아이디어 1
//Collider의 ID(UINT32) 두개를 붙여서(union) uint 형태를 만들고, 이 값을 이용해 Collision Pair unordered_map을 생성

//아이디어 2(현재 채택)
//개별 Collider 컴포넌트가 현재 충돌중인 상대방 Collider 주소를 관리

namespace ehw
{
	class iCollider2D;

	//	class Scene;
	class CollisionManager
	{
		friend class GameEngine;
	public:

		template <type_traits_Ex::is_enum_class_v T>
		inline void SetCollisionInteraction(T _left, T _right, bool _enable = true);

		static inline void RegisterCollider2D(const std::shared_ptr<iCollider2D>& _col2D);

		static void LayerCollision(class iScene* _scene, uint32 _left, uint32 _right);
		static void ColliderCollision(const std::shared_ptr<iCollider2D>& _left, const std::shared_ptr<iCollider2D>& _right);
		static bool Intersect(const std::shared_ptr<iCollider2D>& _left, const std::shared_ptr<iCollider2D>& _right);

		static const void SetCollisionMask(uint _layerA, uint _layerB, bool _isEnable);
		static const std::array<std::bitset<g_maxLayer>, g_maxLayer>& 
			GetCollisionMasks() { return m_collisionMask; }

	private:
		static void Init();
		static void Release();

		static void Update();
		
		static void Render();

	private:
		//레이어끼리의 상호작용 여부를 담아놓은 이차원 배열
		static std::array<std::bitset<g_maxLayer>, g_maxLayer> m_collisionMask;

		static Collision2D m_col2DManager;
		static bool m_bEnableCollision2D;

		static Collision3D m_col3DManager;
		static bool m_bEnableCollision3D;
	};

	template<type_traits_Ex::is_enum_class_v T>
	inline void CollisionManager::SetCollisionInteraction(T _left, T _right, bool _enable)
	{
		uint left = static_cast<uint>(_left);
		uint right = static_cast<uint>(_right);

		m_collisionMask[left][right] = _enable;
		m_collisionMask[right][left] = _enable;
	}

	inline void CollisionManager::RegisterCollider2D(const std::shared_ptr<iCollider2D>& _col2D)
	{
		m_col2DManager.Register(_col2D);
	}
}
