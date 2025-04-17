#pragma once
#include "Engine/Game/Component/Animator/Animator.h"

#include "Engine/Common.h"

#include "Engine/Resource/Animation2D.h"

namespace core
{
	class Com_Animator2D 
		: public Animator
	{
		CLASS_NAME(Com_Animator2D);
		CLONE_ABLE(Com_Animator2D);
	public:
		struct tEvents
		{
			struct tEvent
			{
				void operator=(std::function<void()> _func)
				{
					Event = std::move(_func);
				}
				void operator()()
				{
					if (Event)
						Event();
				}

				std::function<void()> Event;
			};

			tEvent StartEvent;
			tEvent CompleteEvent;
			tEvent EndEvent;

			std::vector<tEvent> Events;
		};

		Com_Animator2D();
		Com_Animator2D(const Com_Animator2D& _other);

		virtual ~Com_Animator2D();

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		virtual void update() override; 
		virtual void final_update() override;

		bool is_playing() const { ASSERT(false, "미구현"); return false; }

		bool create(const std::string_view _name
			, std::shared_ptr<Texture> _atlas
			, float2 _leftTop, float2 _size
			, float2 _offset
			, uint _spriteLength
			, float _duration);

		bool CreateXY(const std::string_view _name, std::shared_ptr<Texture> _atlas, UINT _uColTotal, UINT _uRowTotal, float _duration);

		Animation2D* find_animation(const std::string_view _name) const;
		tEvents* FindEvents(const std::string_view _name) const; 
		void Play(const std::string_view _name, bool _loop = true);


		std::function<void()>& GetStartEvent(const std::string_view _name) const;
		std::function<void()>& GetCompleteEvent(const std::string_view _name) const;
		std::function<void()>& GetEndEvent(const std::string_view _name) const;
		std::function<void()>& GetEvent(const std::string_view _name, uint _index) const;

	private:
		std::unordered_map<std::string, Animation2D*, Hasher_StringView, std::equal_to<>>	mAnimations;
		std::unordered_map<std::string, tEvents*, Hasher_StringView, std::equal_to<>>		mEvents;
		Animation2D* mActiveAnimation;
		bool mbLoop;
	};
}
