
#include "AudioManager.h"

#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>



namespace core
{
	AudioManager::AudioManager() 
		: m_FMOD_core_system{ nullptr }
		, m_FMOD_system{ nullptr }
	{}

	void AudioManager::init()
	{
		void* extraDriverData = NULL;

		FMOD_RESULT result = FMOD::Studio::System::create(&m_FMOD_system);
		ASSERT(FMOD_RESULT::FMOD_OK == result, "FMOD 초기화 실패");

		// The example Studio project is authored for 5.1 sound, so set up the system output mode to match
		result = m_FMOD_system->getCoreSystem(&m_FMOD_core_system);
		ASSERT(FMOD_RESULT::FMOD_OK == result, "FMOD 초기화 실패");

		result = m_FMOD_core_system->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);
		ASSERT(FMOD_RESULT::FMOD_OK == result, "FMOD 초기화 실패");

		result = m_FMOD_system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);
		ASSERT(FMOD_RESULT::FMOD_OK == result, "FMOD 초기화 실패");

		AtExit::add_func(AudioManager::destroy_inst);
	}

	AudioManager::~AudioManager()
	{
		m_FMOD_system->release();
		m_FMOD_system = nullptr;
	}

	bool AudioManager::create_sound(const std::filesystem::path& _fullPath,  FMOD::Sound** sound)
	{
		bool bResult = false;
		if (FMOD_OK == m_FMOD_core_system->createSound(_fullPath.string().c_str(), FMOD_3D, 0, sound))
		{
			bResult = true;
		}

		return bResult;
	}

	void AudioManager::sound_play(FMOD::Sound* sound, FMOD::Channel** channel)
	{
		m_FMOD_core_system->playSound(sound, 0, false, channel);
	}

	void AudioManager::set_3D_listener_attrib(const float3* _position, const float3* _velocity,
									   const float3* _forward, const float3* _up)
	{
		FMOD_VECTOR fmodPosition(_position->x, _position->y, _position->z);
		FMOD_VECTOR fmodVelocity(_velocity->x, _velocity->y, _velocity->z);
		FMOD_VECTOR fmodForward(_forward->x, _forward->y, _forward->z);
		FMOD_VECTOR fmodUp(_up->x, _up->y, _up->z);

		m_FMOD_core_system->set3DListenerAttributes(0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp);
	}
}
