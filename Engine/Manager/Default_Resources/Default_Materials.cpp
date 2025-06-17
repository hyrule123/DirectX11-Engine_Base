#include "Engine/Manager/Resourcemanagers.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Shader/GraphicsShader.h"
#include "Engine/Resource/Texture.h"
#include "Engine/Resource/Material/Material.h"

#include "Engine/Resource/Material/Light_3D_Material.h"
#include "Engine/Resource/Material/Default3DMtrl.h"
#include "Engine/Resource/Material/DebugMaterial.h"

namespace core {
	void ResourceManagers::load_default_materials()
	{
		using namespace name::defaultRes;
#pragma region DEFAULT
		s_ptr<GraphicsShader> shader = ResourceManager<GraphicsShader>::get_inst().find(shader::graphics::RectShader);
		s_ptr<Material> RectMaterial = std::make_shared<Material>();
		RectMaterial->set_shader(shader);
		RectMaterial->set_engine_default_res(true);
		ResourceManager<Material>::get_inst().insert(material::RectMaterial, RectMaterial);
#pragma endregion

#pragma region SPRITE
		s_ptr <Texture> spriteTexture = ResourceManager<Texture>::get_inst().find(texture::DefaultSprite);
		s_ptr<GraphicsShader> spriteShader = ResourceManager<GraphicsShader>::get_inst().find(shader::graphics::SpriteShader);
		s_ptr<Material> spriteMaterial = std::make_shared<Material>();
		spriteMaterial->set_rendering_mode(eRenderingMode::forward_opaque);
		spriteMaterial->set_shader(spriteShader);
		spriteMaterial->set_texture(eTextureSlot::diffuse_texture, spriteTexture);
		spriteMaterial->set_engine_default_res(true);
		ResourceManager<Material>::get_inst().insert(material::SpriteMaterial, spriteMaterial);
#pragma endregion

#pragma region UI
		s_ptr<GraphicsShader> uiShader = ResourceManager<GraphicsShader>::get_inst().find(shader::graphics::UIShader);
		s_ptr<Material> uiMaterial = std::make_shared<Material>();
		uiMaterial->set_rendering_mode(eRenderingMode::forward_opaque);
		uiMaterial->set_shader(uiShader);
		uiMaterial->set_engine_default_res(true);
		ResourceManager<Material>::get_inst().insert(material::UIMaterial, uiMaterial);
#pragma endregion

#pragma region GRID
		s_ptr<GraphicsShader> gridShader = ResourceManager<GraphicsShader>::get_inst().find(shader::graphics::GridShader);
		s_ptr<Material> gridMaterial = std::make_shared<Material>();
		gridMaterial->set_shader(gridShader);
		gridMaterial->set_engine_default_res(true);
		ResourceManager<Material>::get_inst().insert(material::GridMaterial, gridMaterial);
#pragma endregion


#pragma region PARTICLE
		s_ptr<GraphicsShader> particleShader = ResourceManager<GraphicsShader>::get_inst().find(shader::graphics::ParticleShader);
		s_ptr<Material> particleMaterial = std::make_shared<Material>();
		particleMaterial->set_rendering_mode(eRenderingMode::forward_transparent);
		particleMaterial->set_shader(particleShader);
		particleMaterial->set_engine_default_res(true);
		ResourceManager<Material>::get_inst().insert(material::ParticleMaterial, particleMaterial);
#pragma endregion

#pragma region POSTPROCESS
		s_ptr<GraphicsShader> postProcessShader = ResourceManager<GraphicsShader>::get_inst().find(shader::graphics::PostProcessShader);
		s_ptr<Material> postProcessMaterial = std::make_shared<Material>();
		postProcessMaterial->set_rendering_mode(eRenderingMode::post_process);
		postProcessMaterial->set_shader(postProcessShader);
		postProcessMaterial->set_engine_default_res(true);
		ResourceManager<Material>::get_inst().insert(material::PostProcessMaterial, postProcessMaterial);
#pragma endregion

#pragma region FORWARD_3D
		s_ptr<GraphicsShader> forward_3D_shader = ResourceManager<GraphicsShader>::get_inst().find(shader::graphics::Forward3DShader);
		s_ptr<Material> basic3DMaterial = std::make_shared<Default3DMtrl>();
		basic3DMaterial->set_rendering_mode(eRenderingMode::forward_transparent);
		basic3DMaterial->set_shader(forward_3D_shader);

		basic3DMaterial->set_engine_default_res(true);
		ResourceManager<Material>::get_inst().insert(material::Basic3DMaterial, basic3DMaterial);
#pragma endregion

#pragma region LIGHT
		{
			s_ptr<GraphicsShader> lightShader = ResourceManager<GraphicsShader>::get_inst().find(name::defaultRes::shader::graphics::LightDirShader);
			s_ptr<Material> lightMaterial = std::make_shared<Material>();
			lightMaterial->set_rendering_mode(eRenderingMode::NONE);
			lightMaterial->set_shader(lightShader);
			lightMaterial->set_engine_default_res(true);
			ResourceManager<Material>::get_inst().insert(name::defaultRes::material::LightDirMaterial, lightMaterial);
		}

		{
			s_ptr<GraphicsShader> LightPointShader = ResourceManager<GraphicsShader>::get_inst().find(name::defaultRes::shader::graphics::LightPointShader);
			s_ptr<Material> lightMaterial = std::make_shared<Material>();
			lightMaterial->set_rendering_mode(eRenderingMode::NONE);
			lightMaterial->set_shader(LightPointShader);
			lightMaterial->set_engine_default_res(true);

			ResourceManager<Material>::get_inst().insert(name::defaultRes::material::LightPointMaterial, lightMaterial);
		}

#pragma endregion

#pragma region MERGE
		s_ptr<GraphicsShader> mergeShader = ResourceManager<GraphicsShader>::get_inst().find(name::defaultRes::shader::graphics::MergeShader);
		s_ptr<Material> mergeMaterial = std::make_shared<Material>();
		mergeMaterial->set_rendering_mode(eRenderingMode::NONE);
		mergeMaterial->set_shader(mergeShader);
		mergeMaterial->set_engine_default_res(true);
		ResourceManager<Material>::get_inst().insert(name::defaultRes::material::MergeMaterial, mergeMaterial);
#pragma endregion

#pragma region DEBUG
		s_ptr<GraphicsShader> debugShader = ResourceManager<GraphicsShader>::get_inst().find(shader::graphics::DebugShader);
		s_ptr<DebugMaterial> debugMaterial = std::make_shared<DebugMaterial>();
		debugMaterial->set_rendering_mode(eRenderingMode::forward_transparent);
		debugMaterial->set_shader(debugShader);
		debugMaterial->set_engine_default_res(true);
		ResourceManager<Material>::get_inst().insert(material::DebugMaterial, debugMaterial);
#pragma endregion
	}
}
