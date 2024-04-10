#include "Collision3D.h"


#include "Engine/Game/iScene.h"
#include "Engine/Game/GameObject.h"

#include "Engine/Game/Component/Transform/Com_Transform.h"
#include "Engine/Game/Component/Collider/iCollider3D.h"
#include "Engine/Game/Component/Collider/Com_Collider3D_Rigid.h"

#include "Engine/Game/Collision/CollisionSystem.h"
#include "Engine/Manager/TimeManager.h"

#include "Engine/Game/Collision/PhysXInstance.h"
#include "Engine/Game/Collision/PhysXConverter.h"


namespace ehw
{
	using namespace ::physx;

	Collision3D::Collision3D(CollisionSystem* _owner)
		: m_collisionSystem(_owner)
		, m_pxScene{ nullptr }
		, m_defaultPxMaterial{}
		, m_curUpdateInterval{ UpdateInterval::Frame_60 }
		, m_accumulatedDeltaTime(0.f)
	{
	}

	Collision3D::~Collision3D()
	{
		//const PxU32 actorCount = m_pxScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		//if (actorCount > 0)
		//{
		//	std::vector<PxRigidActor*> actors(actorCount);
		//	m_pxScene->getActors(
		//		PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, 
		//		reinterpret_cast<PxActor**>(actors.data()), actorCount);

		//	for (const auto& actor : actors)
		//	{
		//		iCollider3D* collider = static_cast<iCollider3D*>(actor->userData);
		//		if (collider)
		//		{
		//			collider->DestroyShape();
		//		}
		//	}
		//}

		PX_RELEASE(m_defaultPxMaterial);

		PX_RELEASE(m_pxScene);
	}

	void Collision3D::Init()
	{
		CreatePxScene();

		physx::PxMaterial* m_defaultPxMaterial = PhysXInstance::GetInst().createMaterial(0.5f, 0.5f, 0.6f);
		ASSERT(m_defaultPxMaterial, "pxMaterial 생성 실패");
	}

	void Collision3D::CreatePxScene()
	{
		iScene* gameScene = m_collisionSystem->GetOwnerScene();

		PxSceneDesc sceneDescription = PxSceneDesc{ PhysXInstance::GetInst().getTolerancesScale() };
		sceneDescription.gravity = PxVec3{ 0.f, -9.8f, 0.f };
		sceneDescription.cpuDispatcher = PhysXInstance::GetCPUDispatcher();
		sceneDescription.filterShader = &Collision3D::FilterShader;
		sceneDescription.simulationEventCallback = this;

		m_pxScene = PhysXInstance::GetInst().createScene(sceneDescription);
		ASSERT(m_pxScene, "pxScene 생성 실패.");

		m_pxScene->setName(gameScene->GetStrKey().c_str());
	}

	void Collision3D::FixedUpdate()
	{
		m_accumulatedDeltaTime += TimeManager::DeltaTime();

		int simulateCount = (int)(m_accumulatedDeltaTime / m_physxUpdateIntervals[(int)m_curUpdateInterval]);

		//for문을 돌면서 여러번 충돌검사를 진행하면 절망의 늪 현상이 일어날 수 있다
		//https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/BestPractices.html
		if (0 < simulateCount)
		{
			float simulateTime = (float)simulateCount * m_physxUpdateIntervals[(int)m_curUpdateInterval];
			m_pxScene->simulate(simulateTime);

			m_accumulatedDeltaTime -= simulateTime;
			if (0.f > m_accumulatedDeltaTime)
			{
				m_accumulatedDeltaTime = 0.f;
			}
		}

		PxSceneToGameScene();
		
		ASSERT(false, "여기서 충돌결과 전달하는게아니라 충돌결과를 CollisionSystem에 받아놓은뒤 별도처리를 해야됨");
		//충돌 결과 전달
		if (m_pxScene->checkResults(true))
		{
			//위치 갱신
			

			//충돌 콜백함수 호출
			m_pxScene->fetchResults(false);
		}
	}

	void Collision3D::GameSceneToPxScene()
	{
		const PxU32 actorCount = m_pxScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
		if (actorCount == 0)
		{
			return;
		}

		std::vector<PxRigidActor*> actors(actorCount);

		//Scene -> PxScene은 Static도 포함시킨다
		m_pxScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC,
			reinterpret_cast<PxActor**>(actors.data()), actorCount);

		for (size_t i = 0; i < actors.size(); ++i)
		{
			const PxTransform worldTransform = actors[i]->getGlobalPose();

			Com_Collider3D_Rigid* const collider = static_cast<Com_Collider3D_Rigid*>(actors[i]->userData);
			if (collider == nullptr || collider->IsDestroyed())
			{
				continue;
			}

			Com_Transform* tr = collider->GetTransform();
			if (false == tr->IsTransformUpdated())
			{
				continue;
			}
			physx::PxRigidActor* rigidActor = static_cast<PxRigidActor*>(actors[i]);

			physx::PxTransformT<float> transform{};
			transform.p = tr->GetWorldPosition();
			transform.q = tr->GetLocalRotation();

			rigidActor->setGlobalPose(transform);
		}
	}

	void Collision3D::PxSceneToGameScene()
	{
		const PxU32 actorCount = m_pxScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
		if (actorCount == 0)
		{
			return;
		}

		std::vector<PxRigidActor*> actors(actorCount);

		//PxScene->Scene의 경우 Dynamic만
		m_pxScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, reinterpret_cast<PxActor**>(actors.data()), actorCount);

		for (size_t i = 0; i < actors.size(); ++i)
		{
			const PxTransform worldTransform = actors[i]->getGlobalPose();

			const Com_Collider3D_Rigid* collider = static_cast<Com_Collider3D_Rigid*>(actors[i]->userData);
			if (collider == nullptr || collider->IsDestroyed())
			{
				continue;
			}
			// 충돌체에 대해서 일어난 변화이므로, 이를 Transform에도 반영해야 한다.
			else if (collider->IsTrigger())
			{
				continue;
			}

			Com_Transform* tr = collider->GetOwner()->GetComponent<Com_Transform>();
			tr->SetWorldPosition(worldTransform.p);
			tr->SetLocalRotation(worldTransform.q);
		}
	}


	//void Collision3D::SyncGameScene() const
	//{
	//	const PxU32 actorCount = m_pxScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);

	//	if (actorCount == 0)
	//	{
	//		return;
	//	}

	//	std::vector<PxRigidActor*> actors(actorCount);
	//	m_pxScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(actors.data()), actorCount);

	//	for (const auto& actor : actors)
	//	{
	//		const PxTransform worldTransform = actor->getGlobalPose();

	//		iCollider3D* collider = static_cast<iCollider3D*>(actor->userData);

	//		if (collider == nullptr || collider->IsDestroyed())
	//		{
	//			continue;
	//		}
	//			
	//		// 충돌체에 대해서 일어난 변화이므로, 이를 Com_Transform에도 반영해야 한다.
	//		if (collider->IsTrigger())
	//		{
	//			continue;
	//		}
	//			
	//		Com_Transform* tr = collider->GetTransform();

	//		//원래 위치 정보에서 크기정보 제거
	//		const MATRIX from = MATRIX::CreateScale(tr->GetWorldScale()).Invert() * tr->GetWorldMatrix();

	//		//피직스에서 받아온 트랜스폼 정보를 가져온다
	//		const MATRIX to = PhysXConverter::ToMATRIX(PxMat44{ worldTransform });

	//		//차이를 구해준다
	//		const MATRIX diff = from.Invert() * to;

	//		// 이동은 회전에 영향을 받으므로
	//		const Quaternion rotation = Quaternion::CreateFromRotationMatrix(diff);
	//		
	//		const float3	 position = to.Translation() - float3::Transform(from.Translation(), rotation);

	//		tr->SetWorldPosition(float3::Transform(tr->GetWorldPosition(), rotation) + position);
	//		tr->SetWorldRotation(tr->GetWorldRotation() * rotation);
	//	}
	//}

	void Collision3D::onTrigger(PxTriggerPair* pairs, PxU32 count)
	{
		ASSERT(false, "리팩토링 미진행");
		for (PxU32 i = 0; i < count; ++i)
		{
			const PxTriggerPair& contactpair = pairs[i];

			if (contactpair.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
				continue;

			// if these actors do not exist in the scene anymore due to deallocation, do not process
			if (contactpair.triggerActor->userData == nullptr || contactpair.otherActor->userData == nullptr)
			{
				continue;
			}

			GameObject* triggerObject = static_cast<GameObject*>(contactpair.triggerActor->userData);
			assert(triggerObject);
			GameObject* otherObject = static_cast<GameObject*>(contactpair.otherActor->userData);
			assert(otherObject);
			if (triggerObject->IsDestroyed() || otherObject->IsDestroyed())
			{
				continue;
			}
				

			iCollider3D* triggerCollider = triggerObject->GetComponent<iCollider3D>();
			iCollider3D* otherCollider = otherObject->GetComponent<iCollider3D>();

			// process events
			if (contactpair.status & (PxPairFlag::eNOTIFY_TOUCH_FOUND))
			{
				triggerCollider->OnTriggerEnter(otherCollider);
				otherCollider->OnTriggerEnter(triggerCollider);
			}
			else if (contactpair.status & (PxPairFlag::eNOTIFY_TOUCH_LOST))
			{
				triggerCollider->OnTriggerExit(otherCollider);
				otherCollider->OnTriggerExit(triggerCollider);
			}
		}
	}

	void Collision3D::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
	{
		for (PxU32 i = 0; i < nbPairs; ++i)
		{
			const PxContactPair& contactpair = pairs[i];

			// if these actors do not exist in the scene anymore due to deallocation, do not process
			if (pairHeader.actors[0]->userData == nullptr || pairHeader.actors[1]->userData == nullptr)
			{
				continue;
			}

			iCollider3D* leftCollider = static_cast<iCollider3D*>(pairHeader.actors[0]->userData);
			iCollider3D* rightCollider = static_cast<iCollider3D*>(pairHeader.actors[1]->userData);
			if (leftCollider->IsDestroyed() || rightCollider->IsDestroyed())
			{
				continue;
			}
			else if (leftCollider == rightCollider)
			{
				continue;
			}
			

			PxContactPairPoint collisionPoint{};
			const PxU32		   count = contactpair.extractContacts(&collisionPoint, 1);
			const float3	   collisionPosition = collisionPoint.position;

			// invoke events
			if (contactpair.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				leftCollider->OnCollisionEnter(rightCollider, collisionPosition);
				rightCollider->OnCollisionEnter(leftCollider, collisionPosition);
			}
			else if (contactpair.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				leftCollider->OnCollisionStay(rightCollider, collisionPosition);
				rightCollider->OnCollisionStay(leftCollider, collisionPosition);
			}
			else if (contactpair.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				leftCollider->OnCollisionExit(rightCollider);
				rightCollider->OnCollisionExit(leftCollider);
			}
		}
	}

	void Collision3D::EnableGravity(bool _enable, const float3& _gravity) const
	{
		if (_enable)
		{
			m_pxScene->setGravity(_gravity);
		}
		else
		{
			m_pxScene->setGravity(physx::PxVec3(0.f));
		}
	}

	//void Collision3D::changeGeometry(iCollider3D* collider, physx::PxShape* shape, eCollider3D_Shape type)
	//{
	//	const float3 scale = collider->GetWorldScale();
	//	switch (type)
	//	{
	//	case eCollider3D_Shape::Cube:
	//		shape->setGeometry(PxSphereGeometry{ scale.x });
	//		break;
	//	case eCollider3D_Shape::Sphere:
	//		shape->setGeometry(PxBoxGeometry{ scale.x, scale.y, scale.z });
	//		break;
	//	}
	//}

	bool Collision3D::Raycast(uint32 srcLayerIndex, const float3& origin, const float3& direction, float maxDistance, RaycastHit* outHit) const
	{

		std::bitset<32> layer{};
		layer[srcLayerIndex] = true;

		const PxHitFlags  hitFlag = PxHitFlag::eDEFAULT;
		PxQueryFilterData filter{};

		filter.data.word0 = m_collisionSystem->GetRaycastMask()[srcLayerIndex].to_ulong();

		PxRaycastBuffer hit{};
		const bool		result = m_pxScene->raycast(origin, direction, maxDistance, hit, hitFlag, filter);

		outHit->hitCollider = (hit.hasBlock) ? static_cast<iCollider3D*>(hit.block.actor->userData) : nullptr;
		outHit->hasBlocking = hit.hasBlock;
		outHit->hitDistance = hit.block.distance;
		outHit->hitPosition = hit.block.position;
		outHit->hitNormal = hit.block.normal;

		return result;
	}

	physx::PxFilterData Collision3D::GetCollisionFilterData(uint32 _layer)
	{
		physx::PxFilterData ret{};

		if (_layer < g_maxLayer)
		{
			ret.word0 = _layer;
			ret.word1 = m_collisionSystem->GetLayerCollisionMask(_layer).to_ulong();
		}


		return ret;
	}

	physx::PxFilterData Collision3D::GetRaycastFilterData(uint32 _layer)
	{
		physx::PxFilterData ret{};

		if (_layer < g_maxLayer)
		{
			ret.word0 = _layer;
			ret.word1 = m_collisionSystem->GetLayerRaycastMask(_layer).to_ulong();
		}

		return ret;
	}



	//void Collision3D::changeScene(iScene* scene)
	//{
	//	const auto iter = m_physxScenes.find(scene);
	//		//std::find_if(m_physxScenes.begin(), m_physxScenes.end(), [scene](const PxScene* pxScene) {
	//		//return pxScene->getName() == scene->GetNameChar();
	//		//});
	//	//assert(iter != m_physxScenes.end());

	//	physx::PxScene* pxScene = nullptr;

	//	if (iter != m_physxScenes.end())
	//	{
	//		pxScene = iter->second;
	//	}
	//	
	//	m_pxScene = pxScene;
	//}


	PxFilterFlags Collision3D::FilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags,
		const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		// let triggers through
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
			{
				pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			}
			else
			{
				return PxFilterFlag::eKILL;
			}
				

			return PxFilterFlag::eDEFAULT;
		}

		// generate contacts for all that were not filtered above
		// trigger the contact callback for pairs (A,B) where
		// the filtermask of A contains the ID of B or vice versa.
		if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
		{
			pairFlags = PxPairFlag::eCONTACT_DEFAULT;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eNOTIFY_CONTACT_POINTS;
			return PxFilterFlag::eDEFAULT;
		}

		return PxFilterFlag::eKILL;
	}

#pragma region NOT IMPLEMENTED
	void Collision3D::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
	{
	}

	void Collision3D::onWake(PxActor** actors, PxU32 count)
	{
	}

	void Collision3D::onSleep(PxActor** actors, PxU32 count)
	{
	}

	void Collision3D::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
	{
	}
#pragma endregion
} // namespace pa 