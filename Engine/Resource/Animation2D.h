#pragma once
#include <Engine/Resource/Animation.h>

#include <Engine/Resource/Texture.h>
#include <Engine/Util/Serialize/Serializable.h>

namespace core
{

	BASE_RESOURCE(Animation2D);
	class Animation2D final
		: public Animation
		, public Serializable_Json
	{
		CLASS_INFO(Animation2D, Animation);
		CLONE_ABLE(Animation2D);
	public:
		struct tSprite
		{
			float2 left_top;			// 좌측 상단 좌표
			float2 size;				// 좌측상단부터 잘라낼 가로 세로의 픽셀 길이
			float2 offset;			// 렌더링 위치 조정하기위한 좌표
			float2 atlas_size;		// 텍스처 이미지의 크기
			float duration;					// 프레임간에 시간 간격

			tSprite()
				: left_top(0.0f, 0.0f)
				, size(0.0f, 0.0f)
				, offset(0.0f, 0.0f)
				, atlas_size(0.0f)
				, duration(0.1f)
			{

			}
		};

		Animation2D();
		Animation2D(const Animation2D& _other);

		virtual ~Animation2D();

		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override
		{ 
			return eResult::Fail_NotImplemented; 
		}

		
		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		uint update();
		void final_update();
		void render();

		void create(const std::string_view _name, s_ptr<Texture> _atlas
			, float2 _leftTop, float2 _size, float2 _offset
			, uint _spriteLegth, float _duration);

		//아틀라스를 균일하게 조각조각 내줌
		void create_XY(const std::string_view _name, s_ptr<Texture> _atlas, UINT _uColTotal, UINT _uRowTotal, float _duration);

		void bind_buffer_to_GPU_register();
		void reset();
		void unbind_buffer_from_GPU_register();

		bool is_complete() const { return m_b_complete; }
		const std::string& get_animation_name() const { return m_animation_name; }
		inline float2 get_sprite_size(UINT _spriteIdx);

	private:
		std::string m_animation_name;
		s_ptr<Texture> m_atlas;
		std::vector<tSprite> m_sprite_sheet;
		int m_idx;
		float m_time;
		bool m_b_complete;
	};

	inline float2 Animation2D::get_sprite_size(UINT _spriteIdx)
	{
		if (_spriteIdx < (UINT)m_sprite_sheet.size())
		{
			return m_sprite_sheet[_spriteIdx].atlas_size;
		}
		return float2{};
	}
}
