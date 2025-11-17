#include <Engine/Manager/ResourceManager.h>

#include <Engine/Resource/Texture.h>

namespace core {
	void ResourceManager::load_default_textures()
	{
#pragma region STATIC TEXTURE
		using namespace name::defaultRes;

		s_ptr<Texture> pTex = nullptr;

		pTex = ResourceManager::get_inst().load_default_resource<Texture>("Texture/noise/noise_01.png");
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager::get_inst().load_default_resource<Texture>("Texture/noise/noise_02.png");
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager::get_inst().load_default_resource<Texture>("Texture/noise/noise_03.jpg");
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager::get_inst().load_default_resource<Texture>("Texture/Cube/TILE_01_N.tga");
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager::get_inst().load_default_resource<Texture>("Texture/Cube/TILE_01_N.tga");
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager::get_inst().load_default_resource<Texture>("Texture/Cube/Brick.jpg");
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

#pragma endregion

		//noise
		//s_ptr<Texture> m_noise_texture = std::make_shared<Texture>();
		//m_noise_texture->CreateBuffer(RenderManager::get_inst().GetResolutionX(), RenderManager::get_inst().GetResolutionY(), DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		//m_noise_texture->bind_buffer_as_SRV(GPU::Register::t::NoiseTexture, eShaderStageFlag::Pixel);
	}
}
