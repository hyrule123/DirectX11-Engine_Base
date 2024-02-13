#pragma once
#include "iResource.h"



#include "../Manager/AudioManager.h"

namespace ehw
{
	

	class AudioClip final : public iResource
	{
	public:
		AudioClip();
		virtual ~AudioClip();

		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;
		
		void Play();
		void Stop();
		void Set3DAttributes(const float3 _pos, const float3 _vel);
		void SetLoop(bool _loop) { mbLoop = _loop; }

	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;
		float mMinDistance;
		float mMaxDistance;
		bool mbLoop;
	};
}

