#pragma once
#include <Engine/Game/Component/Animator/Animator.h>

#include <Engine/Common.h>

#include <Engine/Resource/Animation2D.h>

//구현 필요
//std::vector에 다음 정보를 저장
//std::function vector를 프레임 수만큼 미리 만들어 두는것은 사이즈 낭비가 크므로(std::function은 64byte)
//프레임 번호, std::function, event 추가시마다 push_back 후 프레임 번호 순으로 정렬
//현재 이벤트 인덱스 cursor를 두고 현재 재생중인 프레임 번호와 cursor에 있는 event의 프레임 번호가 일치하면 재생
//이후 cursor + 1 -> 애니메이션 종료 시 다시 0으로 되돌림
namespace core
{
	class Animator2D 
		: public Animator
	{
		CLASS_INFO(Animator2D, Animator);
		CLONE_ABLE(Animator2D);

		NO_SERIALIZE_BINARY;
		NO_SERIALIZE_JSON;

	public:
		Animator2D();
		Animator2D(const Animator2D& _other);

		virtual ~Animator2D();

		virtual void update() override; 
		virtual void final_update() override;

		bool is_playing() const { ASSERT(false, "미구현"); return false; }

		s_ptr<Animation2D> create(const std::string_view _name
			, s_ptr<Texture> _atlas
			, float2 _leftTop, float2 _size
			, float2 _offset
			, uint _spriteLength
			, float _duration);

		bool CreateXY(const std::string_view _name, s_ptr<Texture> _atlas, UINT _uColTotal, UINT _uRowTotal, float _duration);

		s_ptr<Animation2D> find_animation(const std::string_view _name) const;
		void play(const std::string_view _name, bool _loop = true);

		std::function<void()>& get_start_event(const std::string_view _name) const;
		std::function<void()>& get_complete_event(const std::string_view _name) const;
		std::function<void()>& get_end_event(const std::string_view _name) const;
		std::function<void()>& get_event(const std::string_view _name, uint _index) const;

	private:
		std::unordered_map<std::string, s_ptr<Animation2D>, Hasher_StringView, std::equal_to<>>	m_animations;
		s_ptr<Animation2D> m_active_animation;
		bool m_b_loop;
	};
}
