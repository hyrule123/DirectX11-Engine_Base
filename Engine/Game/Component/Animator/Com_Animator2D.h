#pragma once
#include "iAnimator.h"



#include "../../../Resource/Animation/Animation2D.h"
#include "../../../CommonStruct.h"

namespace ehw
{
	class Com_Animator2D : public iAnimator
	{
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
		CLONE(Com_Animator2D);

		virtual ~Com_Animator2D();

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;

		virtual void Update() override; 
		virtual void InternalUpdate() override;

		virtual bool IsPlaying() const override { ASSERT(false, "미구현"); return false; }

		bool Create(const std::string_view _name
			, std::shared_ptr<Texture> _atlas
			, float2 _leftTop, float2 _size
			, float2 _offset
			, uint _spriteLength
			, float _duration);

		bool CreateXY(const std::string_view _name, std::shared_ptr<Texture> _atlas, UINT _uColTotal, UINT _uRowTotal, float _duration);

		Animation2D* FindAnimation(const std::string_view _name) const;
		tEvents* FindEvents(const std::string_view _name) const; 
		void Play(const std::string_view _name, bool _loop = true);

		virtual void BindData() override;
		virtual void UnBindData() override;

		std::function<void()>& GetStartEvent(const std::string_view _name) const;
		std::function<void()>& GetCompleteEvent(const std::string_view _name) const;
		std::function<void()>& GetEndEvent(const std::string_view _name) const;
		std::function<void()>& GetEvent(const std::string_view _name, uint _index) const;

	private:
		std::unordered_map<std::string, Animation2D*, tHashFunc_StringView, std::equal_to<>>	mAnimations;
		std::unordered_map<std::string, tEvents*, tHashFunc_StringView, std::equal_to<>>		mEvents;
		Animation2D* mActiveAnimation;
		bool mbLoop;
	};
}
