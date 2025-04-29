#include "AudioManager.h"

#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>



namespace core
{
	AudioManager::AudioManager()
		: mSystem{}
		, mCoreSystem{}
	{
		AtExit::add_func(AudioManager::destroy_inst);

		void* extraDriverData = NULL;

		FMOD_RESULT result = FMOD::Studio::System::create(&mSystem);
		ASSERT(FMOD_RESULT::FMOD_OK == result, "FMOD 초기화 실패");

		// The example Studio project is authored for 5.1 sound, so set up the system output mode to match
		result = mSystem->getCoreSystem(&mCoreSystem);
		ASSERT(FMOD_RESULT::FMOD_OK == result, "FMOD 초기화 실패");

		result = mCoreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);
		ASSERT(FMOD_RESULT::FMOD_OK == result, "FMOD 초기화 실패");

		result = mSystem->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);
		ASSERT(FMOD_RESULT::FMOD_OK == result, "FMOD 초기화 실패");
	}

	AudioManager::~AudioManager()
	{
		mSystem->release();
		mSystem = nullptr;
	}

	bool AudioManager::CreateSound(const std::filesystem::path& _fullPath,  FMOD::Sound** sound)
	{
		bool bResult = false;
		if (FMOD_OK == mCoreSystem->createSound(_fullPath.string().c_str(), FMOD_3D, 0, sound))
		{
			bResult = true;
		}

		return bResult;
	}

	void AudioManager::SoundPlay(FMOD::Sound* sound, FMOD::Channel** channel)
	{
		mCoreSystem->playSound(sound, 0, false, channel);
	}

	void AudioManager::Set3DListenerAttributes(const float3* _position, const float3* _velocity,
									   const float3* _forward, const float3* _up)
	{
		FMOD_VECTOR fmodPosition(_position->x, _position->y, _position->z);
		FMOD_VECTOR fmodVelocity(_velocity->x, _velocity->y, _velocity->z);
		FMOD_VECTOR fmodForward(_forward->x, _forward->y, _forward->z);
		FMOD_VECTOR fmodUp(_up->x, _up->y, _up->z);

		mCoreSystem->set3DListenerAttributes(0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp);
	}
}
