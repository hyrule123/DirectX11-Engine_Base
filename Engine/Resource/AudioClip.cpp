
#include "AudioClip.h"

#include <Engine/Game/Component/Transform.h>

#include <Engine/Manager/ResourceManager.h>

#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

namespace core
{
	AudioClip::AudioClip()
		: Resource(AudioClip::s_static_type_name)
		, m_sound(nullptr)
		, m_channel(nullptr)
		, m_min_distance(1.0f)
		, m_max_distance(1000.0f)
		, m_b_loop(false)
	{

	}

	AudioClip::~AudioClip()
	{
		m_sound->release();
		m_sound = nullptr;
	}


	eResult AudioClip::load(const std::fs::path& _base_directory, const std::fs::path& _resource_name)
	{
		std::fs::path fullPath = _base_directory / _resource_name;
		if (false == std::fs::exists(fullPath))
		{
			ERROR_MESSAGE("파일이 없습니다.");
			return eResult::Fail_Open;
		}

		if (false == AudioManager::get_inst().create_sound(fullPath, &m_sound))
			return eResult::Fail_Open;

		m_sound->set3DMinMaxDistance(m_min_distance, m_max_distance);

		return eResult::Success;
	}

	void AudioClip::play()
	{
		if (m_b_loop)
			m_sound->setMode(FMOD_LOOP_NORMAL);
		else
			m_sound->setMode(FMOD_LOOP_OFF);

		AudioManager::get_inst().sound_play(m_sound, &m_channel);
	} 

	void AudioClip::stop()
	{
		m_channel->stop();
	}

	void AudioClip::set_3D_attributes(const float3 _pos, const float3 _vel)
	{
		FMOD_VECTOR fmodPos(_pos.x, _pos.y, _pos.z);
		FMOD_VECTOR fmodVel(_vel.x, _vel.y, _vel.z);

		m_channel->set3DAttributes(&fmodPos, &fmodVel);
	}
}
