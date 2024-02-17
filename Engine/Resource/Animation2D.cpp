#include "Animation2D.h"



#include "../Manager/TimeManager.h"
#include "../Manager/RenderManager.h"

#include "../GPU/ConstBuffer.h"

#include "../DefaultShader/Animation/Animation2D.hlsli"

namespace ehw
{
	Animation2D::Animation2D()
		: iAnimation(typeid(iAnimation))
		, mAnimator(nullptr)
		, mAtlas(nullptr)
		, mSpriteSheet{}
		, mIndex(-1)
		, mTime(0.0f)
		, mbComplete(false)
	{
	}

	Animation2D::Animation2D(const Animation2D& _other)
		: iAnimation(_other)
		, mAnimator() //이건 이 애니메이션을 복사해서 가져가는 주인이 새로 설정해줘야함
		, mAnimationName(_other.mAnimationName)
		, mAtlas(_other.mAtlas) //Atlas == Texture -> 공유하는 리소스
		, mSpriteSheet(_other.mSpriteSheet) //vector도 value를 담고 있는 경우에는 그냥 복사하면 됨
		, mIndex(_other.mIndex)
		, mTime(_other.mTime)
		, mbComplete(_other.mbComplete)
	{
	}

	Animation2D::~Animation2D()
	{
	}


	eResult Animation2D::Serialize_Json(JsonSerializer* _ser) const
	{
		return eResult::Fail_NotImplemented;
	}

	eResult Animation2D::DeSerialize_Json(const JsonSerializer* _ser)
	{
		return eResult::Fail_NotImplemented;
	}

	uint Animation2D::Update()
	{
		if (mbComplete)
			return -1;

		// 시간 체크
		mTime += TimeManager::DeltaTime();

		// 누적 시간이 해당 프레임의 유지시간을 넘어서면 다음프레임으로 이동
		if (mSpriteSheet[mIndex].Duration < mTime)
		{
			mTime = 0.0f;
			++mIndex;
			if (mSpriteSheet.size() <= mIndex)
			{
				mbComplete = true;
				mIndex = static_cast<int>(mSpriteSheet.size()) - 1;
			}

			return mIndex;
		}

		return -1;
	}

	void Animation2D::InternalUpdate()
	{

	}

	void Animation2D::Render()
	{
	}

	void Animation2D::Create(const std::string_view _name
		, std::shared_ptr<Texture> _atlas
		, float2 _leftTop, float2 _size, float2 _offset
		, uint _spriteLegth, float _duration)
	{
		mAnimationName = _name;

		mAtlas = _atlas;
		
	

		float width = static_cast<float>(_atlas->GetWidth());
		float height = static_cast<float>(_atlas->GetHeight());

		for (int index = 0; index < static_cast<int>(_spriteLegth); index++)
		{
			// API 와는 다르게 0~1 사이의 비율좌표로 위치를 표현해야한다.
			tSprite sprite = {};
			sprite.LeftTop = float2((_leftTop.x + (_size.x * static_cast<float>(index))) / width
				, (_leftTop.y) / height);
			sprite.Size = float2(_size.x / width, _size.y / height);
			sprite.Offset = _offset;
			sprite.Duration = _duration;
			sprite.AtlasSize = float2(width, height);

			mSpriteSheet.push_back(sprite);
		}

	}

	void Animation2D::CreateXY(const std::string_view _name, std::shared_ptr<Texture> _atlas, UINT _uColTotal, UINT _uRowTotal, float _duration)
	{
		mAnimationName = _name;

		mAtlas = _atlas;

		float RowSliceUV = 1.f / (float)_uRowTotal;
		float ColSliceUV = 1.f / (float)_uColTotal;

		float2 FrameSize = mAtlas->GetSizeFloat() / float2(_uColTotal, _uRowTotal);

		//Left Top부터 열 순서대로 저장
		for (UINT Row = 0; Row < _uRowTotal; ++Row)
		{
			for (UINT Col = 0; Col < _uColTotal; ++Col)
			{
				// API 와는 다르게 0~1 사이의 비율좌표로 위치를 표현해야한다.
				tSprite sprite = {};
				sprite.LeftTop.x = ColSliceUV * Col;
				sprite.LeftTop.y = RowSliceUV * Row;
				sprite.Size = float2{ColSliceUV, RowSliceUV};
				sprite.Offset = float2{};
				sprite.Duration = _duration;
				sprite.AtlasSize = FrameSize;

				mSpriteSheet.push_back(sprite);
			}
		}

	}

	void Animation2D::BindData()
	{
		mAtlas->BindDataSRV(Register_t_AtlasTexture, eShaderStageFlag::Pixel);

		ConstBuffer* cb = RenderManager::GetConstBuffer(eCBType::Animation2D);

		tCB_Animation2D info = {};
		info.animationType = (uint)eDimensionType::_2D;
		info.leftTop = mSpriteSheet[mIndex].LeftTop;
		info.offset = mSpriteSheet[mIndex].Offset;
		info.spriteSize = mSpriteSheet[mIndex].Size;
		info.atlasSize = mSpriteSheet[mIndex].AtlasSize;

		cb->SetData(&info);
		cb->BindData(eShaderStageFlag::Pixel);
	}

	void Animation2D::Reset()
	{
		mTime = 0.0f;
		mIndex = 0;
		mbComplete = false;
	}

	void Animation2D::UnBindData()
	{
		//Texture clear
		Texture::ClearSRV(Register_t_AtlasTexture);

		ConstBuffer* cb = RenderManager::GetConstBuffer(eCBType::Animation2D);

		tCB_Animation2D info = {};
		info.animationType = (uint)eDimensionType::NOT_SET;

		cb->SetData(&info);
		cb->BindData(eShaderStageFlag::Pixel);
	}



}
