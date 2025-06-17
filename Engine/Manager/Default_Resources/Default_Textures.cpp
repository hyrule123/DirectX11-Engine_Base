#include "Engine/Manager/Resourcemanagers.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Texture.h"

namespace core {
	void ResourceManagers::load_default_textures()
	{
#pragma region STATIC TEXTURE
		using namespace name::defaultRes;

		s_ptr<Texture> pTex = nullptr;

		pTex = ResourceManager<Texture>::get_inst().load(texture::noise_01);
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager<Texture>::get_inst().load(texture::noise_02);
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager<Texture>::get_inst().load(texture::noise_03);
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager<Texture>::get_inst().load(texture::BasicCube);
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager<Texture>::get_inst().load(texture::BasicCubeNormal);
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager<Texture>::get_inst().load(texture::Brick);
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

#pragma endregion

		//noise
		//s_ptr<Texture> m_noiseTexture = std::make_shared<Texture>();
		//m_noiseTexture->CreateBuffer(RenderManager::get_inst().GetResolutionX(), RenderManager::get_inst().GetResolutionY(), DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		//m_noiseTexture->bind_buffer_as_SRV(GPU::Register::t::NoiseTexture, eShaderStageFlag::Pixel);
	}
}
