
#include "Animation2D.h"

#include <Engine/Manager/TimeManager.h>
#include <Engine/Manager/RenderManager.h>

#include <Engine/GPU/ConstBuffer.h>

#include <Engine/DefaultShader/Animation/Animation2D.hlsli>

namespace core
{
	Animation2D::Animation2D()
		: Animation(Animation2D::s_static_type_name)
		, m_atlas(nullptr)
		, m_sprite_sheet{}
		, m_idx(-1)
		, m_time(0.0f)
		, m_b_complete(false)
	{
	}

	Animation2D::Animation2D(const Animation2D& _other)
		: Animation(_other)
		, m_animation_name(_other.m_animation_name)
		, m_atlas(_other.m_atlas) //Atlas == Texture -> 공유하는 리소스
		, m_sprite_sheet(_other.m_sprite_sheet) //vector도 value를 담고 있는 경우에는 그냥 복사하면 됨
		, m_idx(_other.m_idx)
		, m_time(_other.m_time)
		, m_b_complete(_other.m_b_complete)
	{
	}

	Animation2D::~Animation2D()
	{
	}


	eResult Animation2D::serialize_json(JsonSerializer* _ser) const
	{
		return eResult::Fail_NotImplemented;
	}

	eResult Animation2D::deserialize_json(const JsonSerializer* _ser)
	{
		return eResult::Fail_NotImplemented;
	}

	uint Animation2D::update()
	{
		if (m_b_complete)
			return -1;

		// 시간 체크
		m_time += TimeManager::get_inst().delta_time();

		// 누적 시간이 해당 프레임의 유지시간을 넘어서면 다음프레임으로 이동
		if (m_sprite_sheet[m_idx].duration < m_time)
		{
			m_time = 0.0f;
			++m_idx;
			if (m_sprite_sheet.size() <= m_idx)
			{
				m_b_complete = true;
				m_idx = static_cast<int>(m_sprite_sheet.size()) - 1;
			}

			return m_idx;
		}

		return -1;
	}

	void Animation2D::final_update()
	{

	}

	void Animation2D::render()
	{
	}

	void Animation2D::create(const std::string_view _name
		, s_ptr<Texture> _atlas
		, float2 _leftTop, float2 _size, float2 _offset
		, uint _spriteLegth, float _duration)
	{
		m_animation_name = _name;

		m_atlas = _atlas;
		
	

		float width = static_cast<float>(_atlas->get_texture_width());
		float height = static_cast<float>(_atlas->get_texture_height());

		for (int index = 0; index < static_cast<int>(_spriteLegth); index++)
		{
			// API 와는 다르게 0~1 사이의 비율좌표로 위치를 표현해야한다.
			tSprite sprite = {};
			sprite.left_top = float2((_leftTop.x + (_size.x * static_cast<float>(index))) / width
				, (_leftTop.y) / height);
			sprite.size = float2(_size.x / width, _size.y / height);
			sprite.offset = _offset;
			sprite.duration = _duration;
			sprite.atlas_size = float2(width, height);

			m_sprite_sheet.push_back(sprite);
		}

	}

	void Animation2D::create_XY(const std::string_view _name, s_ptr<Texture> _atlas, UINT _uColTotal, UINT _uRowTotal, float _duration)
	{
		m_animation_name = _name;

		m_atlas = _atlas;

		float RowSliceUV = 1.f / (float)_uRowTotal;
		float ColSliceUV = 1.f / (float)_uColTotal;

		float2 texture_size = { m_atlas->get_texture_width(), m_atlas->get_texture_height() };
		float2 FrameSize = texture_size / float2(_uColTotal, _uRowTotal);

		//Left Top부터 열 순서대로 저장
		for (UINT Row = 0; Row < _uRowTotal; ++Row)
		{
			for (UINT Col = 0; Col < _uColTotal; ++Col)
			{
				// API 와는 다르게 0~1 사이의 비율좌표로 위치를 표현해야한다.
				tSprite sprite = {};
				sprite.left_top.x = ColSliceUV * Col;
				sprite.left_top.y = RowSliceUV * Row;
				sprite.size = float2{ColSliceUV, RowSliceUV};
				sprite.offset = float2{};
				sprite.duration = _duration;
				sprite.atlas_size = FrameSize;

				m_sprite_sheet.push_back(sprite);
			}
		}

	}

	void Animation2D::bind_buffer_to_GPU_register()
	{
		m_atlas->bind_buffer_as_SRV(GPU::Register::t::AtlasTexture, eShaderStageFlag::Pixel);

		ConstBuffer* cb = RenderManager::get_inst().get_const_buffer(eCBType::Animation2D);

		tCB_Animation2D info = {};
		info.animationType = (uint)eDimensionType::_2D;
		info.leftTop = m_sprite_sheet[m_idx].left_top;
		info.offset = m_sprite_sheet[m_idx].offset;
		info.spriteSize = m_sprite_sheet[m_idx].size;
		info.atlasSize = m_sprite_sheet[m_idx].atlas_size;

		cb->set_data(&info);
		cb->bind_buffer_to_GPU_register(eShaderStageFlag::Pixel);
	}

	void Animation2D::reset()
	{
		m_time = 0.0f;
		m_idx = 0;
		m_b_complete = false;
	}

	void Animation2D::unbind_buffer_from_GPU_register()
	{
		//Texture clear
		Texture::clear_SRV(GPU::Register::t::AtlasTexture);

		ConstBuffer* cb = RenderManager::get_inst().get_const_buffer(eCBType::Animation2D);

		tCB_Animation2D info = {};
		info.animationType = (uint)eDimensionType::NOT_SET;

		cb->set_data(&info);
		cb->bind_buffer_to_GPU_register(eShaderStageFlag::Pixel);
	}



}
