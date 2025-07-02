#pragma once
#include <Engine/Common.h>
#include <Engine/define_enum.h>

#include <Engine/Game/Collision/ContactPair.h>
#include <Engine/DefaultShader/Debug/Debug.hlsli>

#include <array>
#include <vector>
#include <memory>
#include <unordered_map>

//설명: 2D 충돌체의 충돌 검사를 수행
//CollisionManager에서만 생성 가능.
namespace core
{
	class CollisionSystem;
	class Collider2D;
	class StructBuffer;
	class GameObject;
	class Mesh;
	class StructBuffer;

	class Collision2D
	{
		friend class Collider2D;
		friend class CollisionSystem;
		//리턴값: bool, 인자: 왼쪽 충돌체, 오른쪽 충돌체, Contact Point(부딫힌 지점)
		using CheckIntersectFunction = 
			std::function<bool(const s_ptr<Collider2D>&, const s_ptr<Collider2D>&, float2& _contactPoint)>;

	public:
		Collision2D(CollisionSystem* const _owner);
		~Collision2D();

		void fixed_update();
		void render();
		void frame_end();

	private:
		void Enqueue(const s_ptr<Collider2D>& _obj);

		static bool CheckIntersect_AABB_AABB(
			const s_ptr<Collider2D>& _AABB1, const s_ptr<Collider2D>& _AABB2, float2& _contactPoint);
		static bool CheckIntersect_AABB_OBB(
			const s_ptr<Collider2D>& _AABB, const s_ptr<Collider2D>& _OBB, float2& _contactPoint);
		static bool CheckIntersect_AABB_Circle(
			const s_ptr<Collider2D>& _AABB, const s_ptr<Collider2D>& _circle, float2& _contactPoint);
		static bool CheckIntersect_OBB_OBB(
			const s_ptr<Collider2D>& _OBB1, const s_ptr<Collider2D>& _OBB2, float2& _contactPoint);
		static bool CheckIntersect_OBB_Circle(
			const s_ptr<Collider2D>& _OBB, const s_ptr<Collider2D>& _circle, float2& _contactPoint);
		static bool CheckIntersect_Circle_Circle(
			const s_ptr<Collider2D>& _circle1, const s_ptr<Collider2D>& _circle2, float2& _contactPoint);


	private:
		CollisionSystem* const m_owner;

		std::array<std::vector<s_ptr<Collider2D>>, g_maxLayer> m_collidersInLayer;

		//지역변수와 swap해서 소멸자에서 CollisionExit()를 호출하게 만드는게 좋은 방법일듯.
		//->해 보니까 소멸자에서 무조건 호출돼서, 게임 강제 종료할때도 호출이 됨... 좋은방법은 아닌듯
		std::unordered_map<tColliderID, ContactPair, tColliderID_Hasher> m_collisions;

		//각 Collider2D 함수 주소를 담고있는 이중 배열
		//각 Collider2D가 가지고 있는 eCollider2D_Type를 index 번호로 해서 함수를 호출하기 위함
		static std::array<std::array<CheckIntersectFunction,
			(int)eCollider2D_Shape::END>, (int)eCollider2D_Shape::END>
			s_checkIntersectFunctions;
		
		std::array<std::vector<tDebugDrawData>, (int)eCollider2D_Shape::END> m_debugInstancingData;
	};
}

