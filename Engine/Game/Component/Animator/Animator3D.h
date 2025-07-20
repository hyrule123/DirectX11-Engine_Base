#pragma once
#include <Engine/Game/Component/Animator/Animator.h>

namespace core
{
	class Animation3D;
	class Animation3D_PlayData;
	class Skeleton;
	class Animator3D :
		public Animator
	{
		CLASS_INFO(Animator3D, Animator);
		CLONE_ABLE(Animator3D);
	public:
		Animator3D();
		Animator3D(const Animator3D& _other);

		virtual ~Animator3D();

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		//virtual void init() override;
		virtual void final_update() override;
		virtual void frame_end() override;

		bool is_playing() const;

		virtual bool add_event(const std::string_view _animName, uint _frameIdx, const std::function<void()>& _func) override;

		s_ptr<Animation3D_PlayData> create_shared_animation_data();
		void set_shared_animation_data(const s_ptr<Animation3D_PlayData>& _sharedData) {
			m_shared_play_data = _sharedData;
		}
		s_ptr<Skeleton> get_skeleton();

		bool play(const std::string_view _strAnimName, float _blendTime = 0.f);

		//디버그용
		void play_next_animation();

		void bind_computed_final_bone_matrix();

		s_ptr<Animation3D_PlayData> get_shared_play_data() {
			return m_shared_play_data;
		}

	private:
		s_ptr<Animation3D_PlayData> m_shared_play_data;
	};
}


