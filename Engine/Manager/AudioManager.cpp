
#include "AudioManager.h"
#include "Engine/Util/AtExit.h"


#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>



namespace ehw
{
	

	FMOD::Studio::System* AudioManager::mSystem = nullptr;
	FMOD::System* AudioManager::mCoreSystem = nullptr;

	void AudioManager::Init()
	{
		AtExit::AddFunc(Release);

		void* extraDriverData = NULL;

		FMOD::Studio::System::create(&mSystem);

		// The example Studio project is authored for 5.1 sound, so set up the system output mode to match
		mSystem->getCoreSystem(&mCoreSystem);
		mCoreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);
		
		mSystem->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);
	}

	void AudioManager::Release()
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
