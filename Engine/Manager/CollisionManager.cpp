#include "Engine/Manager/CollisionManager.h"

#include "Engine/Game/iScene.h"
#include "Engine/Game/Component/Transform/Com_Transform.h"
#include "Engine/Game/Component/Collider/iCollider2D.h"

#include "Engine/Util/AtExit.h"

#include "Engine/Manager/SceneManager.h"

namespace ehw
{
	std::bitset<(uint)eLayer::END> CollisionManager::mLayerCollisionMatrix[(uint)eLayer::END] = {};
	std::map<UINT64, bool> CollisionManager::mCollisionMap;

	void CollisionManager::Init()
	{
		AtExit::AddFunc(Release);
	}
	void CollisionManager::Release()
	{
		for (int i = 0; i < (int)eLayer::END; ++i)
		{
			mLayerCollisionMatrix[i].reset();
		}

		mCollisionMap.clear();
	}
	void CollisionManager::Update()
	{
		iScene* scene = SceneManager::GetActiveScene();
		for (uint row = 0; row < (uint)eLayer::END; row++)
		{
			for (uint column = 0; column < (uint)eLayer::END; column++)
			{
				if (mLayerCollisionMatrix[row][column])
				{
					LayerCollision(scene, (eLayer)row, (eLayer)column);
				}
			}
		}
	}
	void CollisionManager::InternalUpdate()
	{
	}
	void CollisionManager::Render()
	{
	}
	void CollisionManager::CollisionLayerCheck(eLayer _left, eLayer _right, bool _enable)
	{
		int row = 0;
		int column = 0;

		if ((uint)_left <= (uint)_right)
		{
			row = (uint)_left;
			column = (uint)_right;
		}
		else
		{
			row = (uint)_right;
			column = (uint)_left;
		}

		mLayerCollisionMatrix[row][column] = _enable;
	}
	void CollisionManager::LayerCollision(iScene* _scene, eLayer _left, eLayer _right)
	{
		const std::vector<std::shared_ptr<GameObject>>& lefts = _scene->GetGameObjects(_left);
		const std::vector<std::shared_ptr<GameObject>>& rights = _scene->GetGameObjects(_right);

		for (const std::shared_ptr<GameObject>& left : lefts)
		{
			if (false == left->IsActive())
				continue;
			if (left->GetComponent<iCollider2D>() == nullptr)
				continue;

			for (const std::shared_ptr<GameObject>& right : rights)
			{
				if (false == left->IsActive())
					continue;
				if (right->GetComponent<iCollider2D>() == nullptr)
					continue;
				/*if (left == right) //지워도 상관없어서 주석처리 (오류나면 수정)
					continue;*/

				ColliderCollision(left->GetComponent<iCollider2D>(), right->GetComponent<iCollider2D>());
			}

			/*if ((uint)left == (uint)right)  
				break;*/
		}

	}

	void CollisionManager::ColliderCollision(const std::shared_ptr<iCollider2D>& _left, const std::shared_ptr<iCollider2D>& _right)
	{
		// 두 충돌체 레이어로 구성된 ID 확인
		union_ColliderID colliderID;
		colliderID.Left = (uint)_left->GetID();
		colliderID.Right = (uint)_right->GetID();

		// 이전 충돌 정보를 검색한다.
		// 만약에 충돌정보가 없는 상태라면
		// 충돌정보를 생성해준다.
		std::map<UINT64, bool>::iterator iter = mCollisionMap.find(colliderID.Id);
		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(colliderID.Id, false));
			iter = mCollisionMap.find(colliderID.Id);
		}

		// 충돌체크를 해준다.
		if (Intersect(_left, _right)) // 충돌을 한 상태
		{
			// 최초 충돌중 Enter
			if (iter->second == false)
			{
				if (_left->IsTriiger())
					_left->OnTriggerEnter(_right);
				else
					_left->OnCollisionEnter(_right);

				if (_right->IsTriiger())
					_right->OnTriggerEnter(_left);
				else
					_right->OnCollisionEnter(_left);

				iter->second = true;
			}
			else // 충돌 중이지 않은 상태 Enter
			{
				if (_left->IsTriiger())
					_left->OnTriggerStay(_right);
				else
					_left->OnCollisionStay(_right);

				if (_right->IsTriiger())
					_right->OnTriggerStay(_left);
				else
					_right->OnCollisionStay(_left);
			}
		}
		else // 충돌하지 않은상태
		{
			// 충돌 중인상태 Exit
			if (iter->second == true)
			{
				if (_left->IsTriiger())
					_left->OnTriggerExit(_right);
				else
					_left->OnCollisionExit(_right);

				if (_right->IsTriiger())
					_right->OnTriggerExit(_left);
				else
					_right->OnCollisionExit(_left);

				iter->second = false;
			}
		}
	}

	bool CollisionManager::Intersect(const std::shared_ptr<iCollider2D>& _left, const std::shared_ptr<iCollider2D>& _right)
	{
		// Rect vs Rect 
		// 0 --- 1
		// |     |
		// 3 --- 2
		float3 arrLocalPos[4] =
		{
			float3{-0.5f, 0.5f, 0.0f}
			,float3{0.5f, 0.5f, 0.0f}
			,float3{0.5f, -0.5f, 0.0f}
			,float3{-0.5f, -0.5f, 0.0f}
		};

		const auto& leftTr = _left->GetOwner()->GetComponent<Com_Transform>();
		const auto& rightTr = _right->GetOwner()->GetComponent<Com_Transform>();
		if (false == (leftTr && rightTr))
			return false;


		MATRIX leftMat = leftTr->GetWorldMatrix();
		MATRIX rightMat = rightTr->GetWorldMatrix();



		// 분리축 벡터 4개 구하기
		float3 Axis[4] = {};

		float3 leftScale = float3(_left->GetSize().x, _left->GetSize().y, 1.0f);

		MATRIX finalLeft = MATRIX::CreateScale(leftScale);
		finalLeft *= leftMat;

		float3 rightScale = float3(_right->GetSize().x, _right->GetSize().y, 1.0f);
		MATRIX finalRight = MATRIX::CreateScale(rightScale);
		finalRight *= rightMat;

		Axis[0] = float3::Transform(arrLocalPos[1], finalLeft);
		Axis[1] = float3::Transform(arrLocalPos[3], finalLeft);
		Axis[2] = float3::Transform(arrLocalPos[1], finalRight);
		Axis[3] = float3::Transform(arrLocalPos[3], finalRight);

		Axis[0] -= float3::Transform(arrLocalPos[0], finalLeft);
		Axis[1] -= float3::Transform(arrLocalPos[0], finalLeft);
		Axis[2] -= float3::Transform(arrLocalPos[0], finalRight);
		Axis[3] -= float3::Transform(arrLocalPos[0], finalRight);

		for (int index = 0; index < 4; index++)
			Axis[index].z = 0.0f;

		float3 vc = leftTr->GetWorldPosition() - rightTr->GetWorldPosition();
		vc.z = 0.0f;

		float3 centerDir = vc;
		for (int index = 0; index < 4; index++)
		{
			float3 vA = Axis[index];
			//vA.Normalize();

			float projDist = 0.0f;
			for (int employee = 0; employee < 4; employee++)
			{
				projDist += fabsf(Axis[employee].Dot(vA) / 2.0f);
			}

			if (projDist < fabsf(centerDir.Dot(vA)))
			{
				return false;
			}
		}


		return true;
	}
}
