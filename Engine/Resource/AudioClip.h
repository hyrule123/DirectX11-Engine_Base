#pragma once
#include <Engine/Resource/Resource.h>

#include <Engine/Manager/AudioManager.h>

namespace core
{
	BASE_RESOURCE(AudioClip);
	class AudioClip final : public Resource
	{
		CLASS_INFO(AudioClip, Resource);
	public:
		AudioClip();
		virtual ~AudioClip();

		virtual eResult load(const std::fs::path& _base_directory) override;
		
		void play();
		void stop();
		void set_3D_attributes(const float3 _pos, const float3 _vel);
		void set_loop(bool _loop) { m_b_loop = _loop; }

	private:
		FMOD::Sound* m_sound;
		FMOD::Channel* m_channel;
		float m_min_distance;
		float m_max_distance;
		bool m_b_loop;
	};
}

