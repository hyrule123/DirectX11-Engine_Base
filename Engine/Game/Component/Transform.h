#pragma once
#include <Engine/Game/Component/Component.h>

#include <Engine/DefaultShader/Common_struct.hlsli>

#include <Engine/Game/Collision/PxActorWrapper.h>

//Transform 업데이트 로직
//모든 작업은 더티 플래그를 통해 지연 계산된다.
//set함수가 호출될 때 당장 행렬계산을 하지 않고 플래그만 켜둔 뒤 행렬계산이 필요할 때 실행한다.


//SRT 각 행렬은 다음과 같은 특징을 가진다.
//S: 단위행렬 E의 배수 형태(11, 22, 33에 크기정보, 44는 1).
// 대각선 행렬(diagonal matrix)이므로 역행렬 존재. 교환법칙 무조건 성립 가능.
//R: 직교행렬(orthogonal matrix). 직교행렬은 전치행렬이 곧 역행렬이다.
//T: 판별식이 0. 교환법칙 성립하지 않음.

//child Transform의 world 행렬: ScRcTc*SpRpTp
//S: 언제나 교환법칙이 성립함. 앞으로 보낼 수 있음 -> ScSp * RcTc * RpTp
//R: 교환법칙은 성립하지 않으나, S행렬과 R행렬은 지들끼리만 놈.
//T: 계산 '결과'가 저장됨.
// SR 행렬에 영향을 받으나, SR 행렬에 영향을 미치지 않음. 교환법칙도 성립 안함.
//-> 그러나, T행렬이 영향을 미치지 않으므로 최종 T 결과(Tw)를 알고 있다면,
//ScSp * RcRp * Tw가 가능하다는 것.

//Local Value: 변경 즉시 반영
//Local Matrix: 요청 시 or final_update 호출 시 반영
//World Value: 요청 시 or final_update 호출 시 반영
//World Matrix: 요청 시 or final_update 호출 시 반영
namespace physx
{
	class PxActor;
}

namespace core
{
	class GameObject;
	class StructBuffer;

	class Transform
		: public Component
	{
		CLASS_INFO(Transform, Component);
		BASE_COMPONENT(eComponentOrder::Transform);
		REGISTER_FACTORY(Transform);
		CLONE_ABLE(Transform);

		NO_SERIALIZE_BINARY;
		NO_SERIALIZE_JSON;
		
		friend class Transtform;
		friend class GameObject;

	public:
		Transform();
		//단순 Value만 저장 중이므로 기본 복사 생성자로도 충분함.
		Transform(const Transform& _other) = default;
		
		Transform(Transform&& _move) noexcept = default;

		virtual ~Transform();

		static void init_static();
		static void release_static();
		
		virtual void final_update() override;
		virtual void frame_end() override;

		//Render 시점에 호출
		static void set_data_to_buffer(const std::vector<tTransform>& _data);
		static void bind_buffer_to_GPU_register();

#pragma region //Hierarchy
		bool is_root() const { return m_parent.expired(); }
		s_ptr<Transform> get_parent() const { return m_parent.lock(); }
		void set_parent(const s_ptr<Transform>& _transform) { m_parent = _transform; }
		void unlink_parent();

		void add_child(const s_ptr<Transform>& _transform);
		const std::vector<w_ptr<Transform>>& get_childs() const { return m_childs; }

		std::vector<s_ptr<GameObject>> get_gameobject_hierarchy();
		void get_gameobject_hierarchy_recursive(std::vector<s_ptr<GameObject>>& _ret);
		
#pragma region //Hierarchy
		bool is_transform_updated() const { return m_transform_updated; }

#pragma region //LOCAL
		const float3& get_local_scale() const { return m_local_scale; }

		const math::Quaternion& get_local_rotation() const { return m_local_rotation; }

		const float3& get_local_position() const { return m_local_position; }

		const MATRIX& get_local_matrix() {
			if (m_local_updated) {
				update_local_matrix();
				m_local_updated = false;
			}
			return m_local_matrix;
		}

		const std::array<float3, (int)eDirection::END>& get_local_direction() {
			if (m_local_updated) {
				update_local_matrix();
				m_local_updated = false;
			}
			return m_local_direction;
		}

		const float3& get_local_direction(eDirection _dirType) {
			if (m_local_updated) {
				update_local_matrix();
				m_local_updated = false;
			}
			return get_local_direction()[(int)_dirType];
		}

		
		void set_local_scale(const float3& _localScale) {
			m_local_scale = _localScale;
			m_local_updated = true;
			m_transform_updated = true;
		}
		void set_local_rotation(const math::Quaternion& _localRotation) {
			m_local_rotation = _localRotation;
			m_local_updated = true;
			m_transform_updated = true;
		}
		void set_local_rotation(const float3& _localRotationEuler) {
			m_local_rotation = Quaternion::CreateFromYawPitchRoll(_localRotationEuler);
			m_local_updated = true;
			m_transform_updated = true;
		}
		void set_local_position(const float3& _localPosition) {
			m_local_position = _localPosition;
			m_local_updated = true;
			m_transform_updated = true;
		}
#pragma endregion //LOCAL

#pragma region //WORLD
		const float3& get_world_scale() {
			update_world_matrix_recursive();
			return m_world_scale;
		}
		const math::Quaternion& get_world_rotation() {
			update_world_matrix_recursive();
			return m_world_rotation;
		}
		
		float3 get_world_position() {
			update_world_matrix_recursive();
			return { m_world_matrix._41, m_world_matrix._42, m_world_matrix._43 };
		}
		const MATRIX& get_world_matrix() {
			update_world_matrix_recursive();
			return m_world_matrix;
		}
		const float3& get_world_direction(eDirection _dirType) {
			update_world_matrix_recursive();
			return m_world_direction[(int)_dirType];
		}
		const std::array<float3, (int)eDirection::END>& get_world_direction() {
			update_world_matrix_recursive();
			return m_world_direction;
		}

		void set_world_scale(const float3& _worldScale);
		void set_world_rotation(const math::Quaternion& _worldRotation);
		void set_world_rotation(const float3& _worldRotationEuler) {
			set_world_rotation(Quaternion::CreateFromYawPitchRoll(_worldRotationEuler));
		}
		void set_world_position(const float3& _worldPosition);
#pragma endregion //WORLD

		void set_ignore_parent_scale(bool _b) { m_is_ignore_parent_scale = _b; }
		void set_ignore_parent_rotation(bool _b) { m_is_ignore_parent_rotation = _b; }

	protected:
		const float3& get_world_scale_internal() const { return m_world_scale; }
		const math::Quaternion& get_world_rotation_internal() const { return m_world_rotation; }
		const MATRIX& get_world_matix_internal() const { return m_world_matrix; }

		//아래의 2개 함수는 GameObject에서 호출됨.
		void remove_child_ptr(const s_ptr<Transform>& _pTransform);
		void destroy_childs_recursive();

		void update_local_matrix();

		void update_world_matrix_recursive();

		void reserve_update_recursive();

	private:
		static inline StructBuffer* s_buffer = nullptr;

#pragma region LOCAL
		//크기
		float3 m_local_scale;
		
		//회전
		math::Quaternion m_local_rotation;

		//이동
		float3 m_local_position;

		MATRIX m_local_matrix;

		//방향
		std::array<float3, (int)eDirection::END> m_local_direction;
#pragma region //LOCAL


#pragma region //WORLD
		//월드 크기
		float3 m_world_scale;

		//월드 회전(참고 - 월드 회전은 부모 트랜스폼으로부터 누적시켜서 '비교를 위해서만' 사용한다.
		math::Quaternion m_world_rotation;

		//월드 위치는 구할 수 없음.

		MATRIX m_world_matrix;

		std::array<float3, (int)eDirection::END> m_world_direction;
#pragma endregion //WORLD

		//아래 3개 boolean은 매 프레임마다 갱신된다.
		bool m_parent_updated;	//부모 트랜스폼에서 재귀 형태로 true
		bool m_local_updated;	//로컬 값 변경 시 true
		bool m_transform_updated;//위의 두 값중 하나라도 true였으면 true

		//부모의 크기 무시할 시 true
		bool m_is_ignore_parent_scale;

		//부모의 회전 무시할 시 true
		bool m_is_ignore_parent_rotation;
		
// **************************** 부모자식관계가 transform 안에 있는 이유 *************************************
//I think 2 style reasons.
// One is that 3D modeling programs use a Transform for parent / child.Unity tries to be friendly to experienced game designers.

//The other, related, is that parent / child is mostly about the things a Transform holds(this may be what dan_wipf is getting at).
// When you change position / scale / rotation in a Transform, you run through it’s child Transforms and update their stats.
// It’s all Transforms – you never touch a get_owner.And it’s just as easy to walk through a tree of Transforms as one of gameObjects.
// It feels a little weird to walk from sub - object to sub - object, but it works fine.Esp. for people who prefer using: public Transform cow; as the primary way to link to gameObjects.
		w_ptr<Transform> m_parent;
		std::vector<w_ptr<Transform>> m_childs;
	};

	//Decompose 이론 - 참고만
//https://math.stackexchange.com/questions/237369/given-this-transformation-matrix-how-do-i-decompose-it-into-translation-rotati
}
