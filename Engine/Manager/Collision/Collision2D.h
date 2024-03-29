#pragma once
#include "Engine/CommonType.h"
#include "Engine/CommonStruct.h"
#include "Engine/define_enum.h"

#include "Engine/Manager/Collision/CollisionInfo.h"

#include "Engine/DefaultShader/Debug/Debug.hlsli"

#include <array>
#include <vector>
#include <memory>
#include <unordered_map>

//설명: 2D 충돌체의 충돌 검사를 수행
//CollisionManager에서만 생성 가능.
namespace ehw
{
	class iCollider2D;
	class StructBuffer;
	class GameObject;
	class Mesh;
	class StructBuffer;

	class Collision2D
	{
		friend class iCollider2D;
		friend class CollisionManager;
		//리턴값: bool, 인자: 왼쪽 충돌체, 오른쪽 충돌체, Contact Point(부딫힌 지점)
		using Collision2DFunction = std::function<
			bool(iCollider2D* const, iCollider2D* const, Vector2&)
		>;

	private:
		Collision2D();
		~Collision2D();
		void Reset();

		void Init();
		void Update();
		void Render();
		void FrameEnd();

	private:
		void Enqueue(iCollider2D* const _obj);

		bool CheckIntersect_AABB_AABB(
			iCollider2D* const _AABB1, iCollider2D* const _AABB2, 
			Vector2& _hitPoint);

		bool CheckIntersect_AABB_OBB(
			iCollider2D* const _AABB, iCollider2D* const _OBB, 
			Vector2& _hitPoint);

		bool CheckIntersect_AABB_Circle(
			iCollider2D* const _AABB, iCollider2D* const _circle, 
			Vector2& _hitPoint);

		bool CheckIntersect_OBB_OBB(
			iCollider2D* const _OBB1, iCollider2D* const _OBB2, 
			Vector2& _hitPoint);

		bool CheckIntersect_OBB_Circle(
			iCollider2D* const _OBB, iCollider2D* const _circle, 
			Vector2& _hitPoint);

		bool CheckIntersect_OBB_AABB(
			iCollider2D* const _OBB, iCollider2D* const _AABB,
			Vector2& _hitPoint);

		bool CheckIntersect_Circle_Circle(
			iCollider2D* const _circle1, iCollider2D* const _circle2,
			Vector2& _hitPoint);

	private:
		void RenderDebugMesh(const std::shared_ptr<Mesh>& _mesh, const std::vector<tDebug>& _debugData, StructBuffer* const _sbuffer);

	private:
		std::array<std::vector<iCollider2D*>, g_maxLayer> m_collidersInLayer;

		//지역변수와 swap해서 소멸자에서 CollisionExit()를 호출하게 만드는게 좋은 방법일듯.
		//->해 보니까 소멸자에서 무조건 호출돼서, 게임 강제 종료할때도 호출이 됨... 좋은방법은 아닌듯
		std::unordered_map<tColliderID, CollisionInfo, tColliderID_Hasher> m_collisions;

		//각 Collider2D 함수 주소를 담고있는 이중 배열
		//사용하는 이유: 
		//각 Collider2D가 가지고 있는 eCollider2D_Type를 index 번호로 해서 함수를 호출하기 위함
		std::array<std::array<Collision2DFunction, (int)eCollider2D_Shape::END>, (int)eCollider2D_Shape::END>
			m_collision2DFunctions;

		std::array<std::vector<tDebug>, (int)eCollider2D_Shape::END> m_debugInstancingData;
		std::unique_ptr<StructBuffer> m_debugInfoSBuffer;
	};
}

