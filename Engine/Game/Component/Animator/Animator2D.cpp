
#include "Animator2D.h"

#include <Engine/Game/GameObject.h>

#include <Engine/Game/Component/Transform.h>

namespace core
{
	Animator2D::Animator2D()
		: Animator(eDimensionType::_2D)
		, m_animations{}
		, m_active_animation(nullptr)
		, m_b_loop(false)
	{
	}
	Animator2D::Animator2D(const Animator2D& _other)
		: Animator(_other)
		//복사작업을 따로 해줘야할 경우 여기서는 일단 초기화해줌
		, m_animations{}
		, m_active_animation{}

		//bool 변수 = value이브로 바로 복사
		, m_b_loop(_other.m_b_loop)
	{

		//std::unordered_map<std::string, Animation*, Hasher_StringView, std::equal_to<>>	m_animations;
		//이 부분 주의해야함 _other.mAnimation이랑 mAnimation이랑 헷갈릴때 많음
		for (const auto& iter : _other.m_animations)
		{
			if (nullptr == iter.second)
				continue;

			//애니메이션을 복사해서 새로 받아온다.
			s_ptr<Animation2D> AnimClone = std::static_pointer_cast<Animation2D>(iter.second->clone());

			//애니메이션 이름이 비어있으면 에러 발생시킴
			ASSERT(false == AnimClone->get_animation_name().empty(), "애니메이션 이름이 존재하지 않습니다.");

			//복사한 애니메이션을 내 애니메이션 목록에 집어넣음
			m_animations.insert(std::make_pair(AnimClone->get_animation_name(), AnimClone));


			//Animation* m_active_animation;
			//만약 재생중이던 애니메이션이 있는데
			if (_other.m_active_animation)
			{
				//그 애니메이션이랑 방금 복사한 애니메이션이랑 이름이 같을경우
				if (_other.m_active_animation->get_animation_name() == AnimClone->get_animation_name())
				{
					//여기도 동일하게 복사한 애니메이션 주소를 대입
					m_active_animation = AnimClone;
				}
			}
			//std::unordered_map<std::string, tEvents*, Hasher_StringView, std::equal_to<>> mEvents;
			//mEvents(노티파이 함수)의 경우 누가 호출했는지를 알수 없기때문에 그냥 복사하면 에러 날수도 있음
			//예를들어서 A 게임오브젝트에서 자신에게서 소리 나도록 설정했는데 B 오브젝트로 그냥 복사해서 가져오면
			//B오브젝트가 애니메이션 재생했는데 A 오브젝트에서 소리날수도 있다던가 그런거
			//이건 그래서 함수를 등록한 쪽에서 직접 다시 등록을 해줘야함

			//이런식으로 복사생성자를 구현해주면 됨
		}

	}

	Animator2D::~Animator2D()
	{
	}

	eResult Animator2D::serialize_json(JsonSerializer* _ser) const
	{
		return eResult();
	}
	eResult Animator2D::deserialize_json(const JsonSerializer* _ser)
	{
		return eResult();
	}

	void Animator2D::update()
	{
		if (m_active_animation == nullptr)
			return;
	}
	void Animator2D::final_update()
	{
	}


	s_ptr<Animation2D> Animator2D::create(const std::string_view _name, s_ptr<Texture> _atlas
		, float2 _leftTop, float2 _size, float2 _offset
		, uint _spriteLegth, float _duration)
	{
		if (_atlas == nullptr) { return nullptr; }

		s_ptr<Animation2D> animation = find_animation(_name);
		if (animation) { 
			DEBUG_MESSAGE("같은 이름의 Animation이 존재합니다.");
			return nullptr;
		}

		animation = new_entity<Animation2D>();
		animation->create(_name, _atlas, _leftTop, _size, _offset, _spriteLegth, _duration);

		m_animations.insert(std::make_pair(_name, animation));

		return animation;
	}

	bool Animator2D::CreateXY(const std::string_view _name, s_ptr<Texture> _atlas, UINT _uColTotal, UINT _uRowTotal, float _duration)
	{
		if (_atlas == nullptr)
			return false;

		s_ptr<Animation2D> animation = find_animation(_name);
		if (animation != nullptr)
			return false;

		animation = new_entity<Animation2D>();
		animation->create_XY(_name, _atlas, _uColTotal, _uRowTotal, _duration);

		m_animations.insert(std::make_pair(_name, animation));		

		//사이즈 수정
		const auto& tr = get_owner()->get_component<Transform>(); 
		if (tr)
		{
			float2 size = animation->get_sprite_size(0u);
			
			tr->set_ignore_parent_scale(true);
			tr->set_local_scale(float3(size, 1.f));
		}

		return true;
	}

	s_ptr<Animation2D> Animator2D::find_animation(const std::string_view _name) const
	{
		const auto& iter = m_animations.find(_name);

		if (iter == m_animations.end())
		{
			return nullptr;
		}

		return iter->second;
	}
	
	void Animator2D::play(const std::string_view _name, bool _loop)
	{
		m_active_animation = find_animation(_name);
		m_active_animation->reset();
		m_b_loop = _loop;
	}

	//void Animator2D::bind_buffer_to_GPU_register()
	//{
	//	if (m_active_animation == nullptr)
	//		return;

	//	m_active_animation->bind_buffer_to_GPU_register();
	//}

	//void Animator2D::unbind_buffer_from_GPU_register()
	//{
	//	if (m_active_animation == nullptr)
	//		return;

	//	m_active_animation->unbind_buffer_from_GPU_register();
	//}

}
