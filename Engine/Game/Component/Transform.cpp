#include "Engine/Game/Component/Transform.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Game/Component/Camera/Com_Camera.h"

#include "Engine/Manager/RenderManager.h"

#include "Engine/GPU/StructBuffer.h"

#include "Engine/GameEngine.h"

#include <PhysX/PxPhysicsAPI.h>

namespace ehw
{
	Transform::Transform()
		: Component(Transform::concrete_class_name)
		, m_localScale(float3::One)
		, m_localRotation(Quaternion::Identity)
		, m_localPosition(float3::Zero)
		, m_localDirection{ float3::Right, float3::Up, float3::Forward }
		, m_localMatrix(MATRIX::Identity)
		, m_worldScale(float3::One)
		, m_worldRotation(math::Quaternion::Identity)
		, m_worldDirection{ float3::Right, float3::Up, float3::Forward }
		, m_worldMatrix(MATRIX::Identity)
		, m_parent_updated(false)
		, m_local_updated(true)
		, m_transform_updated(true)
		, m_parent()
		, m_childs()
	{
	}


	Transform::~Transform()
	{
	}

	void Transform::init_static()
	{
		s_buffer = new StructBuffer;
		StructBuffer::Desc desc{};
		desc.GPU_register_t_SRV = GPU::Register::t::g_transforms;
		desc.GPU_register_u_UAV = GPU::Register::u::NONE;
		desc.eSBufferType = eStructBufferType::READ_ONLY;
		desc.TargetStageSRV = eShaderStageFlag::ALL;
		s_buffer->init<tTransform>(desc);
	}

	void Transform::release_static()
	{
		SAFE_DELETE(s_buffer);
	}

	void Transform::final_update()
	{
		//아래 함수에 update_local_matrix()가 포함되어 있음.
		update_world_matrix_recursive();
	}

	void Transform::frame_end()
	{
		m_local_updated = false;
		m_parent_updated = false;
		m_transform_updated = false;
	}

	void Transform::set_data_to_buffer(const std::vector<tTransform>& _data)
	{
		s_buffer->set_data(_data.data(), (UINT)_data.size());
	}

	void Transform::bind_buffer_to_GPU_register()
	{
		s_buffer->bind_buffer_as_SRV();
	}

	void Transform::update_local_matrix()
	{
		//1. 크기행렬
		m_localMatrix = MATRIX::CreateScale(m_localScale);

		//2. 회전행렬
		MATRIX matRot = MATRIX::CreateFromQuaternion(m_localRotation);
		m_localMatrix *= matRot;

		//2-1. 회전행렬으로부터 직관적 방향을 계산한다.
		m_localDirection[(int)eDirection::Forward] = matRot.Forward();
		m_localDirection[(int)eDirection::Right] = matRot.Right();
		m_localDirection[(int)eDirection::Up] = matRot.Up();

		//3. 이동행렬
		m_localMatrix *= MATRIX::CreateTranslation(m_localPosition);
	}

	void Transform::unlink_parent()
	{
		if (m_parent)
		{
			m_parent->remove_child_ptr(this);
			m_parent = nullptr;
		}
	}

	void Transform::add_child(Transform* _transform)
	{
		if (_transform)
		{
			m_childs.push_back(_transform);

			_transform->set_parent(this);
			_transform->reserve_update_recursive();
		}
	}

	std::vector<GameObject*> Transform::get_gameobject_hierarchy()
	{
		std::vector<GameObject*> ret{};
		get_gameobject_hierarchy_recursive(ret);
		return ret;
	}

	void Transform::get_gameobject_hierarchy_recursive(std::vector<GameObject*>& _retObjects)
	{
		_retObjects.push_back(gameObject());
		for (size_t i = 0; i < m_childs.size(); ++i)
		{
			m_childs[i]->get_gameobject_hierarchy_recursive(_retObjects);
		}
	}

	void Transform::update_world_matrix_recursive()
	{
		//월드 행렬을 업데이트 해야 하는 경우: 로컬 또는 부모 트랜스폼 중 하나라도 업데이트 된 경우
		bool is_world_need_update = m_local_updated || m_parent_updated;

		//로컬행렬 업데이트
		if (m_local_updated) {
			update_local_matrix();
			m_local_updated = false;
		}

		//부모행렬 업데이트(재귀)
		if (m_parent_updated) {
			if (m_parent) {
				m_parent->update_world_matrix_recursive();
			}
			m_parent_updated = false;
		}

		//로컬, 월드 둘중 하나라도 업데이트 되었으면 자신의 월드행렬을 업데이트 한다.
		if (is_world_need_update) {
			m_worldMatrix = m_localMatrix;
			m_worldRotation = m_localRotation;
			m_worldScale = m_localScale;
			if (m_parent)
			{
				//부모 매트릭스를 받아온다.
				MATRIX parentMat = m_parent->get_world_matix_internal();
				constexpr size_t rowErasebytes = sizeof(float) * 3;

				//크기 무시
				if (m_is_ignore_parent_scale)
				{
					//크기 무시 + 회전 무시: 0, 0부터 3, 3까지 밀어버리고 단위행렬로 변경
					if (m_is_ignore_parent_rotation)
					{
						memset(parentMat.m[0], 0, rowErasebytes);
						memset(parentMat.m[1], 0, rowErasebytes);
						memset(parentMat.m[2], 0, rowErasebytes);
						parentMat._11 = 1.f;
						parentMat._22 = 1.f;
						parentMat._33 = 1.f;
					}

					//크기 무시 + 회전 반영
					else
					{
						//정규화하면 크기정보가 제거된다.
						parentMat.Right(parentMat.Right().Normalize());
						parentMat.Up(parentMat.Up().Normalize());
						parentMat.Forward(parentMat.Forward().Normalize());

						m_worldRotation *= m_parent->get_world_rotation_internal();
					}
				}

				//크기 반영
				else
				{
					//크기 반영 + 회전 무시
					if (m_is_ignore_parent_rotation)
					{
						//우선 크기 + 회전 정보가 있는 부분을 초기화 한다.
						memset(parentMat.m[0], 0, rowErasebytes);
						memset(parentMat.m[1], 0, rowErasebytes);
						memset(parentMat.m[2], 0, rowErasebytes);

						//WorldScale을 집어넣어준다.
						const float3& worldScale = m_parent->get_world_scale();
						parentMat._11 = worldScale.x;
						parentMat._22 = worldScale.y;
						parentMat._33 = worldScale.z;

						m_worldScale *= m_parent->get_world_scale();
					}

					//크기 반영 + 회전 반영 -> 그냥 바로 적용
				}

				m_worldMatrix *= parentMat;
			}

			//월드 방향 계산
			for (int i = 0; i < (int)eDirection::END; ++i)
			{
				m_worldDirection[i] = float3(m_worldMatrix.m[i]).Normalize();
			}
		}
	}

	void Transform::reserve_update_recursive()
	{
		m_parent_updated = true;
		m_local_updated = true;
		for (Transform* t : m_childs) {
			t->reserve_update_recursive();
		}
	}

	void Transform::set_world_scale(const float3& _worldScale)
	{
		//부모 스케일 무시 or 부모 트랜스폼 없을 경우
		if (m_is_ignore_parent_scale || nullptr == m_parent)
		{
			set_local_scale(_worldScale);
		}

		//else: false == m_is_ignore_parent_scale && m_parent
		else 
		{
			//자신의 월드행렬은 업데이트 할 필요 없음
			m_parent->update_world_matrix_recursive();

			//설정하려는 World Scale을 부모의 World Scale로 나눠 주면 Local Scale을 얼마나 지정해야하는지 알 수 있다.
			set_local_scale(_worldScale / m_parent->get_world_scale_internal());
		}
	}
	void Transform::set_world_rotation(const math::Quaternion& _worldRotation)
	{
		//부모 스케일 무시 or 부모 트랜스폼 없을 경우
		if (m_is_ignore_parent_rotation || nullptr == m_parent)
		{
			set_local_rotation(_worldRotation);
		}
		else
		{
			//부모의 월드행렬까지만 업데이트(자신은 로컬만 업데이트하면 됨)
			m_parent->update_world_matrix_recursive();

			//설정하려는 World Rotation을 부모의 World Rotation로 나눠 주면 Local Rotation을 얼마나 지정해야하는지 알 수 있다.
			set_local_rotation(_worldRotation / m_parent->get_world_rotation_internal());
		}
	}

	void Transform::set_world_position(const float3& _worldPosition)
	{
		//부모 트랜스폼 없을 경우
		if (nullptr == m_parent)
		{
			set_local_position(_worldPosition);
		}
		else
		{
			//World Position을 구하기 위해서는 자신 포함 World Matrix 업데이트를 해줘야함(Parent 까지만)
			update_world_matrix_recursive();

			//World Matrix의 위치 정보를 worldPosition으로 설정
			m_worldMatrix.Translation(_worldPosition);

			//update_world_matrix_recursive() 호출 시 Local Matrix, World Matrix 전부 새걸로 갱신되어 있는 상태이므로(flag들이 모두 false 가 되어있는 상태)
			//추가적으로 작업을 안해줘도 됨. -> Set함수를 통하지 않고 변경.
			MATRIX local_diff = m_worldMatrix * m_parent->get_world_matix_internal().Invert();
			m_localPosition = local_diff.Translation();

			//오차를 최대한 줄이기 위해 position만 가져온다.
			m_localMatrix.Translation(m_localPosition);
		}
	}

	void Transform::remove_child_ptr(Transform* _pTransform)
	{
		for (size_t i = 0; i < m_childs.size(); ++i)
		{
			if (_pTransform == m_childs[i])
			{
				m_childs.erase((m_childs.begin() + i));
				break;
			}
		}
	}

	void Transform::destroy_childs_recursive()
	{
		//swap 하는 이유: 지워질때마다 UnlinkParent가 호출되어 m_childs에서 자신을 지워달라고 요청함.
		//미리 m_childs를 비워놔야 에러가 안 발생함
		std::vector<Transform*> childs{};
		childs.swap(m_childs);
		for (size_t i = 0; i < m_childs.size(); ++i)
		{
			childs[i]->set_parent(nullptr);
			childs[i]->gameObject()->Destroy();
		}
	}
}
