#include "Engine/Game/Component/Animator/Animation3D_PlayData.h"

#include "Engine/Game/Component/Animator/Animator.h"

#include "Engine/GPU/StructBuffer.h"

#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/TimeManager.h"

#include "Engine/Resource/Model3D/Animation3D.h"
#include "Engine/Resource/Model3D/Skeleton.h"

#include <fbxsdk/fbxsdk.h>

namespace ehw
{
	Animation3D_PlayData::Animation3D_PlayData()
		: Entity(Animation3D_PlayData::concrete_class_name)
		, m_skeleton()
		//, m_iFramePerSecond(30)
		, m_PrevFrame(-1)
		, m_animation3D_data()

		, m_fChangeTimeLength()
		, m_fChangeTimeAccumulate()

		, m_currentAnimation()
		, m_fClipUpdateTime()

		, m_is_pre_updated(false)
	{
		
	}

	Animation3D_PlayData::Animation3D_PlayData(const Animation3D_PlayData& _other)
		: Entity(_other)
		, m_skeleton()
		//, m_iFramePerSecond(_other.m_iFramePerSecond)
		, m_PrevFrame(_other.m_PrevFrame)
		, m_animation3D_data(_other.m_animation3D_data)

		, m_fChangeTimeLength(_other.m_fChangeTimeLength)
		, m_fChangeTimeAccumulate(_other.m_fChangeTimeAccumulate)

		, m_currentAnimation(_other.m_currentAnimation)
		, m_fClipUpdateTime()
	{

	}

	Animation3D_PlayData::~Animation3D_PlayData()
	{
	}

	bool Animation3D_PlayData::pre_update()
	{
		m_is_pre_updated = true;

		//필요한 요소가 없을 시 return
		if (nullptr == m_skeleton || nullptr == m_currentAnimation)
		{
			return false;
		}

		bool bChangeEnd = false;
		if (m_animation3D_data.bChangingAnim)
		{
			m_fChangeTimeAccumulate += TimeManager::get_inst().DeltaTime();

			if (m_fChangeTimeLength < m_fChangeTimeAccumulate)
			{
				m_fChangeTimeAccumulate = m_fChangeTimeLength;
				bChangeEnd = true;
			}

			m_animation3D_data.ChangeRatio = (float)(m_fChangeTimeAccumulate / m_fChangeTimeLength);
		}
		else
		{
			double curTime = 0.f;
			// 현재 재생중인 Clip 의 시간을 진행한다.
			m_fClipUpdateTime += TimeManager::get_inst().DeltaTime();

			//애니메이션 재생이 끝났으면 -> 첫 프레임으로
			//m_fClipUpdateTime = 애니메이션 시작 이후 누적 시간
			const double frameTime = m_currentAnimation->get_time_length();
			if (frameTime <= m_fClipUpdateTime)
			{
				m_fClipUpdateTime = 0.f;
			}

			//애니메이션의 Start Time에 애니메이션 재생 시작 후 지나간 시간을 더해줌
			curTime = m_currentAnimation->get_start_time() + m_fClipUpdateTime;

			// 현재 프레임 인덱스 구하기
			// 현재 애니메이션 시간 * 초당 프레임 수
			const double dFrameIdx = curTime * (double)m_currentAnimation->get_FPS();
			
			m_animation3D_data.CurrentFrame = (uint)dFrameIdx;
			m_animation3D_data.NextFrame = m_animation3D_data.CurrentFrame + 1;

			// 프레임간의 시간에 따른 비율을 구해준다.(보간)
			m_animation3D_data.FrameRatio =
				std::clamp((float)(dFrameIdx - (double)m_animation3D_data.CurrentFrame), 0.f, 1.f);

			//인덱스 넘어가면 Start로 돌아가준다(일단 루프 기본적으로 한다고 가정)
			const uint start_frame_idx = m_currentAnimation->get_start_frame();
			const uint end_frame_idx = m_currentAnimation->get_end_frame();
			
			//범위 내로 제한
			if (m_animation3D_data.CurrentFrame < start_frame_idx) {
				m_animation3D_data.CurrentFrame = start_frame_idx;
			}
			else if (end_frame_idx < m_animation3D_data.CurrentFrame) {
				m_animation3D_data.CurrentFrame = start_frame_idx;
			}

			if (m_animation3D_data.NextFrame < start_frame_idx) {
				m_animation3D_data.NextFrame = start_frame_idx;
			}
			else if (end_frame_idx < m_animation3D_data.NextFrame) {
				m_animation3D_data.NextFrame = start_frame_idx;
			}
		}

		if (bChangeEnd)
		{
			play_internal(m_nextAnimation, false);
			m_nextAnimation = nullptr;
			m_animation3D_data.bChangingAnim = FALSE;
			m_animation3D_data.ChangeFrameLength = 0;
			m_animation3D_data.ChangeFrameIdx = 0;
			m_animation3D_data.ChangeRatio = 0.f;
			m_fChangeTimeLength = 0.f;
			m_fChangeTimeAccumulate = 0.f;
		}

		//계산 대기열에 넣는다. 받아오는 변수는 자신의 인덱스 번호(차후에 사용됨)
		m_animation3D_data.model_inst_ID = m_skeleton->add_compute_queue(this);

		return true;
	}

	void Animation3D_PlayData::bind_computed_final_bone_matrix()
	{
		if (m_skeleton) 
		{
			m_skeleton->compute_bone_final_matrix();
			m_skeleton->bind_bone_final_matrix_SRV();
		}
	}

	void Animation3D_PlayData::frame_end()
	{
		m_is_pre_updated = false;
	}

	int Animation3D_PlayData::get_start_frame() const
	{
		if (m_currentAnimation)
		{
			return m_currentAnimation->get_start_frame();
		}

		return -1;
	}

	void Animation3D_PlayData::set_skeleton(const std::shared_ptr<Skeleton> _skeleton)
	{
		//스켈레톤 주소를 받아서
		m_skeleton = _skeleton;
		if (m_skeleton)
		{
			//최종 Bone별 행렬이 저장될 Vector 크기를 재조정
			m_animation3D_data.BoneCount = m_skeleton->get_bone_count();
		}
	}

	bool Animation3D_PlayData::play(const std::string_view _animationName, float _blendTime)
	{
		if (m_skeleton)
		{
			std::shared_ptr<Animation3D> anim = m_skeleton->find_animation(_animationName);
			return play_internal(anim, _blendTime);
		}

		return false;
	}

	void Animation3D_PlayData::play_next()
	{
		if (nullptr == m_skeleton)
		{
			return;
		}
		const auto& anims = m_skeleton->get_animations();

		if (anims.empty())
		{
			return;
		}
		else if (nullptr == m_currentAnimation || (size_t)1 == anims.size())
		{
			play_internal(anims.begin()->second, true);
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
						play_internal(anims.begin()->second, 1.f);
					}
					else
					{
						play_internal(iter->second, 1.f);
					}
					break;
				}
				++iter;
			}
		}
		
	}


	bool Animation3D_PlayData::play_internal(const std::shared_ptr<Animation3D>& _anim, float _blendTime)
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
				m_animation3D_data.bChangingAnim = TRUE;

				double dFrameIdx = m_nextAnimation->get_start_time() * (double)m_nextAnimation->get_FPS();
				m_animation3D_data.ChangeFrameIdx = (int)dFrameIdx;
				m_animation3D_data.ChangeFrameLength = m_nextAnimation->get_frame_length();

				m_animation3D_data.ChangeRatio = 0.f;
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
				//m_iFramePerSecond = m_currentAnimation->get_FPS();

				m_PrevFrame = 0;
				m_animation3D_data.CurrentFrame = 0;
				m_animation3D_data.NextFrame = 1;
				m_animation3D_data.FrameRatio = 0.f;
				m_animation3D_data.FrameLength = m_currentAnimation->get_frame_length();
				m_is_pre_updated = false;
				bPlayed = true;
			}

		}
		return bPlayed;
	}
}
