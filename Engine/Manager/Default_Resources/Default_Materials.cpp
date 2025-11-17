#include <Engine/Manager/ResourceManager.h>

#include <Engine/Resource/Shader/GraphicsShader.h>
#include <Engine/Resource/Texture.h>
#include <Engine/Resource/Material/Material.h>

#include <Engine/Resource/Material/Light_3D_Material.h>
#include <Engine/Resource/Material/Default3DMtrl.h>
#include <Engine/Resource/Material/DebugMaterial.h>

namespace core {
	void ResourceManager::load_default_materials()
	{
		using namespace name::defaultRes;
#pragma region DEFAULT
		s_ptr<GraphicsShader> shader = ResourceManager::get_inst().find<GraphicsShader>(shader::graphics::RectShader);
		s_ptr<Material> rect_material = new_entity<Material>();
		rect_material->set_shader(shader);

		ResourceManager::get_inst().insert_default_resource(rect_material, material::RectMaterial);
#pragma endregion

#pragma region SPRITE
		s_ptr <Texture> spriteTexture = ResourceManager::get_inst().find<Texture>(texture::DefaultSprite);
		s_ptr<GraphicsShader> spriteShader = ResourceManager::get_inst().find<GraphicsShader>(shader::graphics::SpriteShader);
		s_ptr<Material> sprite_material = new_entity<Material>();
		sprite_material->set_rendering_mode(eRenderingMode::forward_opaque);
		sprite_material->set_shader(spriteShader);
		sprite_material->set_texture(eTextureSlot::diffuse_texture, spriteTexture);

		ResourceManager::get_inst().insert_default_resource(sprite_material, material::SpriteMaterial);
#pragma endregion

#pragma region UI
		s_ptr<GraphicsShader> uiShader = ResourceManager::get_inst().find<GraphicsShader>(shader::graphics::UIShader);
		s_ptr<Material> ui_material = new_entity<Material>();
		ui_material->set_rendering_mode(eRenderingMode::forward_opaque);
		ui_material->set_shader(uiShader);

		ResourceManager::get_inst().insert_default_resource(ui_material, material::UIMaterial);
#pragma endregion

#pragma region GRID
		s_ptr<GraphicsShader> gridShader = ResourceManager::get_inst().find<GraphicsShader>(shader::graphics::GridShader);
		s_ptr<Material> grid_material = new_entity<Material>();
		grid_material->set_shader(gridShader);

		ResourceManager::get_inst().insert_default_resource(grid_material, material::GridMaterial);
#pragma endregion


#pragma region PARTICLE
		s_ptr<GraphicsShader> particleShader = ResourceManager::get_inst().find<GraphicsShader>(shader::graphics::ParticleShader);
		s_ptr<Material> particle_material = new_entity<Material>();
		particle_material->set_rendering_mode(eRenderingMode::forward_transparent);
		particle_material->set_shader(particleShader);

		ResourceManager::get_inst().insert_default_resource(particle_material, material::ParticleMaterial);
#pragma endregion

#pragma region POSTPROCESS
		s_ptr<GraphicsShader> postProcessShader = ResourceManager::get_inst().find<GraphicsShader>(shader::graphics::PostProcessShader);
		s_ptr<Material> post_process_material = new_entity<Material>();
		post_process_material->set_rendering_mode(eRenderingMode::post_process);
		post_process_material->set_shader(postProcessShader);

		ResourceManager::get_inst().insert_default_resource(post_process_material, material::PostProcessMaterial);
#pragma endregion

#pragma region FORWARD_3D
		s_ptr<GraphicsShader> forward_3D_shader = ResourceManager::get_inst().find<GraphicsShader>(shader::graphics::Forward3DShader);
		s_ptr<Material> basic_3D_material = new_entity<Default3DMtrl>();
		basic_3D_material->set_rendering_mode(eRenderingMode::forward_transparent);
		basic_3D_material->set_shader(forward_3D_shader);

		ResourceManager::get_inst().insert_default_resource(basic_3D_material, material::Basic3DMaterial);
#pragma endregion

#pragma region LIGHT
		{
			s_ptr<GraphicsShader> lightShader = ResourceManager::get_inst().find<GraphicsShader>(name::defaultRes::shader::graphics::LightDirShader);
			s_ptr<Material> light_material = new_entity<Material>();
			light_material->set_rendering_mode(eRenderingMode::NONE);
			light_material->set_shader(lightShader);

			ResourceManager::get_inst().insert_default_resource(light_material, name::defaultRes::material::LightDirMaterial);
		}

		{
			s_ptr<GraphicsShader> LightPointShader = ResourceManager::get_inst().find<GraphicsShader>(name::defaultRes::shader::graphics::LightPointShader);
			s_ptr<Material> light_material = new_entity<Material>();
			light_material->set_rendering_mode(eRenderingMode::NONE);
			light_material->set_shader(LightPointShader);

			ResourceManager::get_inst().insert_default_resource(light_material, name::defaultRes::material::LightPointMaterial);
		}

#pragma endregion

#pragma region MERGE
		s_ptr<GraphicsShader> mergeShader = ResourceManager::get_inst().find<GraphicsShader>(name::defaultRes::shader::graphics::MergeShader);
		s_ptr<Material> merge_material = new_entity<Material>();
		merge_material->set_rendering_mode(eRenderingMode::NONE);
		merge_material->set_shader(mergeShader);

		ResourceManager::get_inst().insert_default_resource(merge_material, name::defaultRes::material::MergeMaterial);
#pragma endregion

#pragma region DEBUG
		s_ptr<GraphicsShader> debugShader = ResourceManager::get_inst().find<GraphicsShader>(shader::graphics::DebugShader);
		s_ptr<DebugMaterial> debug_material = new_entity<DebugMaterial>();
		debug_material->set_rendering_mode(eRenderingMode::forward_transparent);
		debug_material->set_shader(debugShader);

		ResourceManager::get_inst().insert_default_resource(debug_material, material::DebugMaterial);
#pragma endregion
	}
}
