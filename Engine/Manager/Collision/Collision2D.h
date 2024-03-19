#pragma once
#include "Engine/CommonType.h"
#include "Engine/CommonStruct.h"
#include "Engine/define_enum.h"

#include "Engine/Manager/Collision/CollisionInfo.h"

#include <array>
#include <vector>
#include <memory>
#include <unordered_map>

//설명: 2D 충돌체의 충돌 검사를 수행
namespace ehw
{
	class iCollider2D;
	class Collision2D
	{
		friend class CollisionManager;
		//리턴값: bool, 인자: 왼쪽 충돌체, 오른쪽 충돌체, Hit Point(부딫힌 지점)
		using Collision2DFunction = std::function<
			bool(const std::shared_ptr<iCollider2D>&, const std::shared_ptr<iCollider2D>&, Vector2&)
		>;
	public:
		void Register(const std::shared_ptr<iCollider2D>& _obj);

	private:
		Collision2D();
		~Collision2D();

		void Init();
		void Update();
		void Reset();

	private:
		void Enqueue(const std::shared_ptr<iCollider2D>& _obj);

		bool CheckIntersect_AABB_AABB(
			const std::shared_ptr<iCollider2D>& _AABB1, const std::shared_ptr<iCollider2D>& _AABB2, 
			Vector2& _hitPoint);

		bool CheckIntersect_AABB_OBB(
			const std::shared_ptr<iCollider2D>& _AABB, const std::shared_ptr<iCollider2D>& _OBB, 
			Vector2& _hitPoint);

		bool CheckIntersect_AABB_Circle(
			const std::shared_ptr<iCollider2D>& _AABB, const std::shared_ptr<iCollider2D>& _circle, 
			Vector2& _hitPoint);

		bool CheckIntersect_OBB_OBB(
			const std::shared_ptr<iCollider2D>& _OBB1, const std::shared_ptr<iCollider2D>& _OBB2, 
			Vector2& _hitPoint);

		bool CheckIntersect_OBB_Circle(
			const std::shared_ptr<iCollider2D>& _OBB, const std::shared_ptr<iCollider2D>& _circle, 
			Vector2& _hitPoint);

		bool CheckIntersect_OBB_AABB(
			const std::shared_ptr<iCollider2D>& _OBB, const std::shared_ptr<iCollider2D>& _AABB,
			Vector2& _hitPoint);

		bool CheckIntersect_Circle_Circle(
			const std::shared_ptr<iCollider2D>& _circle1, const std::shared_ptr<iCollider2D>& _circle2,
			Vector2& _hitPoint);


	private:
		std::vector<std::shared_ptr<iCollider2D>> m_colliders;
		std::array<std::vector<std::shared_ptr<iCollider2D>>, g_maxLayer> m_objectsInLayer;

		//지역변수와 swap해서 소멸자에서 CollisionExit()를 호출하게 만드는게 좋은 방법일듯.
		//->해 보니까 소멸자에서 무조건 호출돼서, 게임 강제 종료할때도 호출이 됨... 좋은방법은 아닌듯
		std::unordered_map<tColliderID, CollisionInfo, tColliderID_Hasher> m_collisions;

		//각 Collider2D 함수 주소를 담고있는 이중 배열
		//사용하는 이유: 
		//각 Collider2D가 가지고 있는 eCollider2D_Type를 index 번호로 해서 함수를 호출하기 위함
		std::array<std::array<Collision2DFunction, (int)eCollider2D_Shape::END>, (int)eCollider2D_Shape::END>
			m_collision2DFunctions;
	};
}

