#pragma once
#include "iTransform.h"

#include "../../../DefaultShader/CommonStruct.hlsli"

//Transform 업데이트 로직
//1. 모든 변경사항은 InternalUpdate 로직 때 반영됨(지연 적용).
//2. 그러나 스크립트에서 호출할 가능성이 있는 함수 중, Get/Set World 함수의 경우에는 업데이트된 내용을 바로 갱신해야할 필요성이 있음.
//	이 경우에는 Impl 함수를 직접 호출해서, 값이 반영되었다는 사실을 숨기고(silence) 한번 갱신을 해준다.
//3. 

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
//Local Matrix: 요청 시 or InternalUpdate 호출 시 반영
//World Value: 요청 시 or InternalUpdate 호출 시 반영
//World Matrix: 요청 시 or InternalUpdate 호출 시 반영

namespace ehw
{
	class GameObject;
	class Com_Transform 
		: public iTransform
	{
		friend class Com_Transform;
	public:
		Com_Transform();

		//단순 Value만 저장 중이므로 기본 복사 생성자로도 충분함.
		Com_Transform(const Com_Transform& _other) = default;
		CLONE_ABLE(Com_Transform);

		virtual ~Com_Transform();

	
		virtual void InternalUpdate() override;
		virtual void BindData() override; 

		inline std::shared_ptr<Com_Transform> GetParent() const { return m_parent.lock(); }
		inline void SetParent(const std::shared_ptr<Com_Transform>& _transform) { m_parent = _transform; }

		inline void AddChild(const std::shared_ptr<Com_Transform>& _transform);
		inline const std::vector<std::shared_ptr<Com_Transform>>& GetChilds() const { return m_childs; }

		void GetGameObjectHierarchy_Recursive(std::vector<std::shared_ptr<GameObject>>& _retObjects);
		

#pragma region //LOCAL
		inline const float3& GetLocalScale() const { return m_localScale; }
		inline const math::Quaternion& GetLocalRotation() const { return m_localRotation; }
		inline const float3& GetLocalPosition() const { return m_localPosition; }
		inline const MATRIX& GetLocalMatrix() const { return m_localMatrix; }
		inline const float3& GetLocalDirection(eDirection _dirType);
		inline const std::array<float3, (int)eDirection::END>& GetLocalDirection();

		
		
		inline void SetLocalScale(const float3& _localScale);
		inline void SetLocalRotation(const math::Quaternion& _localRotation);
		inline void SetLocalPosition(const float3& _localPosition);
#pragma endregion //LOCAL

#pragma region //WORLD
		inline const float3& GetWorldScale();
		inline const math::Quaternion& GetWorldRotation();
		inline float3 GetWorldPosition();
		inline const MATRIX& GetWorldMatrix();
		inline const float3& GetWorldDirection(eDirection _dirType);
		inline const std::array<float3, (int)eDirection::END>& GetWorldDirection();

		void SetWorldScale(const float3& _worldScale);
		void SetWorldRotation(const math::Quaternion& _worldRotation);
		void SetWorldPosition(const float3& _worldPosition);
#pragma endregion //WORLD

		void SetIgnoreParentScale(bool _b) { m_bIgnoreParentScale = _b; }
		void SetIgnoreParentRotation(bool _b) { m_bIgnoreParentRotation = _b; }

	private:
		inline void SetAllFlagsOn();

		inline const float3& GetWorldScale_Internal() const { return m_worldScale; }
		inline const math::Quaternion& GetWorldRotation_Internal() const { return m_worldRotation; }
		inline const MATRIX& GetWorldMatrix_Internal() const { return m_worldMatrix; }

		bool UpdateLocalMatrix();

		//갱신사항이 있을 경우 true를 반환한다.
		bool UpdateWorldValue();

		bool UpdateWorldMatrix();

		//_bSilencedUpdate가 true일 경우: m_bInternalUpdated 값을 변경하지 않고 업데이트 함.
		//-> InternalUpdate 단계에서 다시 상태 업데이트가 진행된다

	private:
#pragma region LOCAL
		//크기
		float3 m_localScale;
		float3 m_localSize;
		
		//회전
		math::Quaternion m_localRotation;

		//이동
		float3 m_localPosition;

		MATRIX m_localMatrix;

		//방향
		std::array<float3, (int)eDirection::END> m_localDirection;
#pragma region //LOCAL


#pragma region //WORLD
		//월드 크기
		float3 m_worldScale;

		//월드 회전(참고 - 월드 회전은 부모 트랜스폼으로부터 누적시켜서 '비교를 위해서만' 사용한다.
		math::Quaternion m_worldRotation;

		//월드 위치는 구할 수 없음.

		MATRIX m_worldMatrix;

		std::array<float3, (int)eDirection::END> m_worldDirection;
#pragma endregion //WORLD

		//로컬정보 변경되었을 시 true
		bool m_bNeedUpdateLocalMatrix;

		//World Scale / Rotation 변경 시 true	
		bool m_bNeedUpdateWorldValue;

		//World Matrix가 변경되어야 할 경우 true
		bool m_bNeedUpdateWorldMatrix;


		//부모의 크기 무시할 시 true
		bool m_bIgnoreParentScale;

		//부모의 회전 무시할 시 true
		bool m_bIgnoreParentRotation;
		
// **************************** 부모자식관계가 transform 안에 있는 이유 *************************************
//I think 2 style reasons.
// One is that 3D modeling programs use a Transform for parent / child.Unity tries to be friendly to experienced game designers.

//The other, related, is that parent / child is mostly about the things a Transform holds(this may be what dan_wipf is getting at).
// When you change position / scale / rotation in a Transform, you run through it’s child Transforms and update their stats.
// It’s all Transforms – you never touch a gameObject.And it’s just as easy to walk through a tree of Transforms as one of gameObjects.
// It feels a little weird to walk from sub - object to sub - object, but it works fine.Esp. for people who prefer using: public Transform cow; as the primary way to link to gameObjects.
		std::weak_ptr<Com_Transform> m_parent;
		std::vector<std::shared_ptr<Com_Transform>> m_childs;
	};

	inline void Com_Transform::SetLocalScale(const float3& _localScale)
	{
		m_localScale = _localScale; 
		SetAllFlagsOn();
	}

	inline void Com_Transform::SetLocalRotation(const math::Quaternion& _localRotation)
	{
		m_localRotation = _localRotation;
		SetAllFlagsOn();
	}

	inline void Com_Transform::SetLocalPosition(const float3& _localPosition)
	{
		m_localPosition = _localPosition; 

		SetAllFlagsOn();
	}

	//Decompose 이론 - 참고만
//https://math.stackexchange.com/questions/237369/given-this-transformation-matrix-how-do-i-decompose-it-into-translation-rotati

	const float3& Com_Transform::GetWorldScale()
	{
		//InternalUpdate 단계가 아직 진행되지 않았을 경우 업데이트 함수 호출.
		//이때 bool 플래그는 바꾸지 않는다.(다른 스크립트에서 아직 위치 업데이트가 끝나지 않았을 수 있음)
		UpdateWorldValue();
		
		return m_worldScale;
	}

	const math::Quaternion& Com_Transform::GetWorldRotation()
	{
		//InternalUpdate 단계가 아직 진행되지 않았을 경우 업데이트 함수 호출.
		//이때 bool 플래그는 바꾸지 않는다.(다른 스크립트에서 아직 위치 업데이트가 끝나지 않았을 수 있음)
		UpdateWorldValue();

		return m_worldRotation;
	}
	float3 Com_Transform::GetWorldPosition()
	{
		//InternalUpdate 단계가 아직 진행되지 않았을 경우 업데이트 함수 호출.
		//이때 bool 플래그는 바꾸지 않는다.(다른 스크립트에서 아직 위치 업데이트가 끝나지 않았을 수 있음)
		UpdateWorldMatrix();

		return m_worldMatrix.Translation();
	}

	const MATRIX& Com_Transform::GetWorldMatrix()
	{
		UpdateWorldMatrix();
		return m_worldMatrix;
	}

	inline const float3& Com_Transform::GetWorldDirection(eDirection _dirType)
	{
		return GetWorldDirection()[(int)_dirType];
	}

	inline const std::array<float3, (int)eDirection::END>& Com_Transform::GetWorldDirection()
	{
		UpdateWorldMatrix();
		return m_worldDirection;
	}


	inline void Com_Transform::AddChild(const std::shared_ptr<Com_Transform>& _transform)
	{
		if (_transform)
		{
			m_childs.push_back(_transform);
			_transform->SetParent(shared_from_this_T<Com_Transform>());
			_transform->SetAllFlagsOn();
		}
	}


	inline const float3& ehw::Com_Transform::GetLocalDirection(eDirection _dirType)
	{
		return GetLocalDirection()[(int)_dirType];
	}
	

	inline const std::array<float3, (int)eDirection::END>& ehw::Com_Transform::GetLocalDirection()
	{
		UpdateLocalMatrix();
		return m_localDirection;
	}

	inline void Com_Transform::SetAllFlagsOn()
	{
		m_bNeedUpdateLocalMatrix = true;
		m_bNeedUpdateWorldValue = true;
		m_bNeedUpdateWorldMatrix = true;
	}
}
