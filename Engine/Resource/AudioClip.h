#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/Manager/AudioManager.h"

namespace ehw
{
	class AudioClip final : public Resource
	{
	public:
		AudioClip();
		virtual ~AudioClip();

		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _key_path) override;
		
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

