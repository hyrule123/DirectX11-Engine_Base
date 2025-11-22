#include <Engine/Manager/ResourceManager.h>

#include <Engine/Resource/Texture.h>

namespace core {
	void ResourceManager::load_default_textures()
	{
#pragma region STATIC TEXTURE

		s_ptr<Texture> pTex = nullptr;

		pTex = ResourceManager::get_inst().load_default_resource<Texture>(default_res::texture::noise_01);
		ASSERT_MESSAGE(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager::get_inst().load_default_resource<Texture>(default_res::texture::noise_02);
		ASSERT_MESSAGE(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager::get_inst().load_default_resource<Texture>(default_res::texture::noise_03);
		ASSERT_MESSAGE(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager::get_inst().load_default_resource<Texture>(default_res::texture::tile);
		ASSERT_MESSAGE(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager::get_inst().load_default_resource<Texture>(default_res::texture::tile_normal);
		ASSERT_MESSAGE(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager::get_inst().load_default_resource<Texture>(default_res::texture::brick);
		ASSERT_MESSAGE(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager::get_inst().load_default_resource<Texture>(default_res::texture::brick_normal);
		ASSERT_MESSAGE(nullptr != pTex, "텍스처 로드 실패");

#pragma endregion

		//noise
		//s_ptr<Texture> m_noise_texture = std::make_shared<Texture>();
		//m_noise_texture->CreateBuffer(RenderManager::get_inst().GetResolutionX(), RenderManager::get_inst().GetResolutionY(), DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		//m_noise_texture->bind_buffer_as_SRV(GPU::Register::t::NoiseTexture, eShaderStageFlag::Pixel);
	}
}
