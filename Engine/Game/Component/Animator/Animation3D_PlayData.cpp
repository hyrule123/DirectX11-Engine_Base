#include "Engine/Game/Component/Animator/Animation3D_PlayData.h"

#include "Engine/Game/Component/Animator/Animator.h"

#include "Engine/GPU/StructBuffer.h"

#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/TimeManager.h"

#include "Engine/Resource/Model3D/Animation3D.h"
#include "Engine/Resource/Model3D/Skeleton.h"

#include "Engine/Resource/Shader/ComputeShaders/Animation3D_ComputeShader.h"

#include <fbxsdk/fbxsdk.h>

namespace ehw
{
	Animation3D_PlayData::Animation3D_PlayData()
		: Entity(Animation3D_PlayData::concrete_name)
		, m_animationComputeShader()
		, m_skeleton()
		//, m_iFramePerSecond(30)
		, m_pBoneFinalMatBuffer(nullptr)
		, m_PrevFrame(-1)
		, m_Anim3DCBuffer()

		, m_fChangeTimeLength()
		, m_fChangeTimeAccumulate()

		, m_currentAnimation()
		, m_fClipUpdateTime()

		, m_bInternalUpdated(false)
		, m_bFinalMatrixUpdated(false)
	{
		m_animationComputeShader = LOAD_COMPUTESHADER(Animation3D_ComputeShader);

		StructBuffer::Desc desc{};
		desc.GPU_register_t_SRV = GPU::Register::t::g_FinalBoneMatrixArray;
		desc.TargetStageSRV = eShaderStageFlag::Vertex;
		desc.GPU_register_u_UAV = GPU::Register::u::g_FinalBoneMatrixArrayRW;
		desc.eSBufferType = eStructBufferType::READ_WRITE;
		m_pBoneFinalMatBuffer = std::make_unique<StructBuffer>();
		m_pBoneFinalMatBuffer->Init<MATRIX>(desc);
	}

	Animation3D_PlayData::Animation3D_PlayData(const Animation3D_PlayData& _other)
		: Entity(_other)
		, m_skeleton()
		//, m_iFramePerSecond(_other.m_iFramePerSecond)
		, m_pBoneFinalMatBuffer(nullptr)
		, m_bFinalMatrixUpdated(false)
		, m_PrevFrame(_other.m_PrevFrame)
		, m_Anim3DCBuffer(_other.m_Anim3DCBuffer)

		, m_fChangeTimeLength(_other.m_fChangeTimeLength)
		, m_fChangeTimeAccumulate(_other.m_fChangeTimeAccumulate)

		, m_currentAnimation(_other.m_currentAnimation)
		, m_fClipUpdateTime()
	{
		if (_other.m_pBoneFinalMatBuffer)
		{
			m_pBoneFinalMatBuffer = std::unique_ptr<StructBuffer>(_other.m_pBoneFinalMatBuffer->Clone());
		}
	}

	Animation3D_PlayData::~Animation3D_PlayData()
	{
	}

	bool Animation3D_PlayData::final_update()
	{
		

		if (nullptr == m_skeleton || nullptr == m_currentAnimation)
		{
			return false;
		}
		else if (m_bInternalUpdated)
		{
			return false;
		}
		m_bInternalUpdated = true;

		bool bChangeEnd = false;
		if (m_Anim3DCBuffer.bChangingAnim)
		{
			m_fChangeTimeAccumulate += TimeManager::GetInst().DeltaTime();

			if (m_fChangeTimeLength < m_fChangeTimeAccumulate)
			{
				m_fChangeTimeAccumulate = m_fChangeTimeLength;
				bChangeEnd = true;
			}

			m_Anim3DCBuffer.ChangeRatio = m_fChangeTimeAccumulate / m_fChangeTimeLength;
		}
		else
		{
			double curTime = 0.f;
			// 현재 재생중인 Clip 의 시간을 진행한다.
			m_fClipUpdateTime += TimeManager::GetInst().DeltaTime();

			//애니메이션 재생이 끝났으면 -> 첫 프레임으로
			//m_fClipUpdateTime = 애니메이션 시작 이후 누적 시간
			float frameTime = (float)m_currentAnimation->GetTimeLength();
			if (m_fClipUpdateTime >= frameTime)
			{
				m_fClipUpdateTime = 0.f;
			}

			//애니메이션의 Start Time에 애니메이션 재생 시작 후 지나간 시간을 더해줌
			curTime = m_currentAnimation->GetStartTime() + (double)m_fClipUpdateTime;

			// 현재 프레임 인덱스 구하기
			// 현재 애니메이션 시간 * 초당 프레임 수
			double dFrameIdx = curTime * (double)m_currentAnimation->GetFPS();

			m_Anim3DCBuffer.CurrentFrame = (int)dFrameIdx;

			//만약 이미 마지막 프레임에 도달했을 경우 현재 프레임 유지
			int maxFrameCount = m_currentAnimation->GetStartFrame() + m_currentAnimation->GetFrameLength() - 1;

			if (m_Anim3DCBuffer.CurrentFrame >= maxFrameCount)
				m_Anim3DCBuffer.NextFrame = maxFrameCount;	// 끝이면 현재 인덱스를 유지
			else
				m_Anim3DCBuffer.NextFrame = m_Anim3DCBuffer.CurrentFrame + 1;

			// 프레임간의 시간에 따른 비율을 구해준다.
			m_Anim3DCBuffer.FrameRatio = (float)(dFrameIdx - (double)m_Anim3DCBuffer.CurrentFrame);
		}

		if (bChangeEnd)
		{
			Play(m_nextAnimation, false);
			m_nextAnimation = nullptr;
			m_Anim3DCBuffer.bChangingAnim = FALSE;
			m_Anim3DCBuffer.ChangeFrameLength = 0;
			m_Anim3DCBuffer.ChangeFrameIdx = 0;
			m_Anim3DCBuffer.ChangeRatio = 0.f;
			m_fChangeTimeLength = 0.f;
			m_fChangeTimeAccumulate = 0.f;
		}


		// 컴퓨트 쉐이더 연산여부
		m_bFinalMatrixUpdated = false;
		return true;
	}
	void Animation3D_PlayData::frame_end()
	{
		m_bInternalUpdated = false;
		m_bFinalMatrixUpdated = false;
	}
	bool Animation3D_PlayData::bind_data()
	{
		if (nullptr == m_currentAnimation)
		{
			return false;
		}

		if (false == m_bFinalMatrixUpdated)
		{
			m_bFinalMatrixUpdated = true;

			//구조화 버퍼가 정상적으로 생성되었는지 확인한다.
			if (false == CheckMesh())
			{
				return false;
			}
				

			Animation3D_ComputeShader::Desc desc{};
			desc.CurrentAnimKeyFrameBuffer = m_currentAnimation->GetKeyFrameSBuffer().get();

			if (m_nextAnimation)
			{
				desc.NextAnimKeyFrameBuffer = m_nextAnimation->GetKeyFrameSBuffer().get();
			}

			desc.BoneOffsetMatrixBuffer = m_skeleton->GetBoneOffsetBuffer().get();

			desc.FinalBoneTranslationMatrixBuffer = m_pBoneFinalMatBuffer.get();

			desc.Anim3DData = &m_Anim3DCBuffer;

			m_animationComputeShader->SetDesc(desc);

			// 업데이트 쉐이더 실행
			m_animationComputeShader->OnExcute();

			m_bFinalMatrixUpdated = true;
		}

		// t19 레지스터에 최종행렬 데이터(구조버퍼) 바인딩	
		m_pBoneFinalMatBuffer->BindDataSRV();// Register_t_g_arrBoneMat, eShaderStageFlag::Vertex);

		return true;
	}

	void Animation3D_PlayData::UnBindData()
	{
		m_pBoneFinalMatBuffer->UnbindData();
	}

	int Animation3D_PlayData::GetStartFrame() const
	{
		if (m_currentAnimation)
		{
			return m_currentAnimation->GetStartFrame();
		}

		return -1;
	}

	void Animation3D_PlayData::SetSkeleton(const std::shared_ptr<Skeleton> _skeleton)
	{
		//스켈레톤 주소를 받아서
		m_skeleton = _skeleton;
		if (m_skeleton)
		{
			//최종 Bone별 행렬이 저장될 Vector 크기를 재조정
			//m_vecFinalBoneMat.resize(m_skeleton->GetBoneCount());
			m_Anim3DCBuffer.BoneCount = m_skeleton->GetBoneCount();
		}
	}

	bool Animation3D_PlayData::Play(const std::string_view _animationName, float _blendTime)
	{
		if (m_skeleton)
		{
			std::shared_ptr<Animation3D> anim = m_skeleton->FindAnimation(_animationName);
			return Play(anim, _blendTime);
		}

		return false;
	}

	void Animation3D_PlayData::PlayNext()
	{
		if (nullptr == m_skeleton)
		{
			return;
		}
		const auto& anims = m_skeleton->GetAnimations();

		if (anims.empty())
		{
			return;
		}
		else if (nullptr == m_currentAnimation || (size_t)1 == anims.size())
		{
			Play(anims.begin()->second, true);
		}
		else
		{
			for (auto iter = anims.begin();
				iter != anims.end();
				)
			{
				if (iter->second == m_currentAnimation)
				{
					++iter;
					if (iter == anims.end())
					{
						Play(anims.begin()->second, 1.f);
					}
					else
					{
						Play(iter->second, 1.f);
					}
					break;
				}
				++iter;
			}
		}
		
	}


	bool Animation3D_PlayData::Play(const std::shared_ptr<Animation3D>& _anim, float _blendTime)
	{
		if (nullptr == _anim)
			return false;

		bool bPlayed = false;

		//기존의 애니메이션이 있고 Blend 켰을 경우 Animation Blend 모드 On
		if (m_currentAnimation && (0.f < _blendTime))
		{
			m_nextAnimation = _anim;
			if (m_nextAnimation)
			{
				m_Anim3DCBuffer.bChangingAnim = TRUE;

				double dFrameIdx = m_nextAnimation->GetStartTime() * (double)m_nextAnimation->GetFPS();
				m_Anim3DCBuffer.ChangeFrameIdx = (int)dFrameIdx;
				m_Anim3DCBuffer.ChangeFrameLength = m_nextAnimation->GetFrameLength();

				m_Anim3DCBuffer.ChangeRatio = 0.f;
				m_fChangeTimeLength = 1.f;
				m_fChangeTimeAccumulate = 0.f;

				bPlayed = true;
			}
		}

		//재생중이 아닐경우, 또는 Blend를 활성화하지 않았을 경우
		else
		{
			m_currentAnimation = _anim;
			if (m_currentAnimation)
			{

				m_fClipUpdateTime = 0.f;
				//m_iFramePerSecond = m_currentAnimation->GetFPS();

				m_PrevFrame = -1;
				m_Anim3DCBuffer.CurrentFrame = 0;
				m_Anim3DCBuffer.NextFrame = 1;
				m_Anim3DCBuffer.FrameRatio = 0.f;
				m_Anim3DCBuffer.FrameLength = m_currentAnimation->GetFrameLength();
				m_bFinalMatrixUpdated = false;
				bPlayed = true;
			}

		}
		return bPlayed;
	}

	bool Animation3D_PlayData::CheckMesh()
	{
		if (m_skeleton)
		{
			UINT iBoneCount = m_skeleton->GetBoneCount();
			if (m_pBoneFinalMatBuffer->GetElemCount() != iBoneCount)
			{
				if (eResult_fail(m_pBoneFinalMatBuffer->Resize((size_t)iBoneCount, nullptr, 0))) {
					return false;
				}
			}
		}
		return true;
	}

}
