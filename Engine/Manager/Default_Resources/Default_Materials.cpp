#include <Engine/Manager/ResourceManager.h>

#include <Engine/Resource/Shader/GraphicsPipeline.h>
#include <Engine/Resource/Texture.h>
#include <Engine/Resource/Material/Material.h>

#include <Engine/Resource/Material/Light_3D_Material.h>
#include <Engine/Resource/Material/Default3DMtrl.h>
#include <Engine/Resource/Material/DebugMaterial.h>

namespace core {
	void ResourceManager::load_default_materials()
	{
		using namespace default_res;
#pragma region DEFAULT
		s_ptr<GraphicsPipeline> shader = ResourceManager::get_inst().find<GraphicsPipeline>(shader::graphics::rect_shader);
		s_ptr<Material> rect_material = new_entity<Material>();
		rect_material->set_shader(shader);

		ResourceManager::get_inst().insert_default_resource(rect_material, material::rect_material);
#pragma endregion

#pragma region SPRITE
		s_ptr <Texture> spriteTexture = ResourceManager::get_inst().find<Texture>(texture::default_sprite);
		s_ptr<GraphicsPipeline> spriteShader = ResourceManager::get_inst().find<GraphicsPipeline>(shader::graphics::sprite_shader);
		s_ptr<Material> sprite_material = new_entity<Material>();
		sprite_material->set_rendering_mode(eRenderingMode::forward_opaque);
		sprite_material->set_shader(spriteShader);
		sprite_material->set_texture(eTextureSlot::diffuse_texture, spriteTexture);

		ResourceManager::get_inst().insert_default_resource(sprite_material, material::sprite_material);
#pragma endregion

#pragma region UI
		s_ptr<GraphicsPipeline> uiShader = ResourceManager::get_inst().find<GraphicsPipeline>(shader::graphics::UI_shader);
		s_ptr<Material> ui_material = new_entity<Material>();
		ui_material->set_rendering_mode(eRenderingMode::forward_opaque);
		ui_material->set_shader(uiShader);

		ResourceManager::get_inst().insert_default_resource(ui_material, material::ui_material);
#pragma endregion

#pragma region GRID
		s_ptr<GraphicsPipeline> gridShader = ResourceManager::get_inst().find<GraphicsPipeline>(shader::graphics::grid_shader);
		s_ptr<Material> grid_material = new_entity<Material>();
		grid_material->set_shader(gridShader);

		ResourceManager::get_inst().insert_default_resource(grid_material, material::grid_material);
#pragma endregion


#pragma region PARTICLE
		s_ptr<GraphicsPipeline> particleShader = ResourceManager::get_inst().find<GraphicsPipeline>(shader::graphics::particle_shader);
		s_ptr<Material> particle_material = new_entity<Material>();
		particle_material->set_rendering_mode(eRenderingMode::forward_transparent);
		particle_material->set_shader(particleShader);

		ResourceManager::get_inst().insert_default_resource(particle_material, material::particle_material);
#pragma endregion

#pragma region POSTPROCESS
		s_ptr<GraphicsPipeline> postProcessShader = ResourceManager::get_inst().find<GraphicsPipeline>(shader::graphics::post_process_shader);
		s_ptr<Material> post_process_material = new_entity<Material>();
		post_process_material->set_rendering_mode(eRenderingMode::post_process);
		post_process_material->set_shader(postProcessShader);

		ResourceManager::get_inst().insert_default_resource(post_process_material, material::post_process_material);
#pragma endregion

#pragma region FORWARD_3D
		s_ptr<GraphicsPipeline> forward_3D_shader = ResourceManager::get_inst().find<GraphicsPipeline>(shader::graphics::forward_3D_shader);
		s_ptr<Material> basic_3D_material = new_entity<Default3DMtrl>();
		basic_3D_material->set_rendering_mode(eRenderingMode::forward_transparent);
		basic_3D_material->set_shader(forward_3D_shader);

		ResourceManager::get_inst().insert_default_resource(basic_3D_material, material::basic_3D_material);
#pragma endregion

#pragma region LIGHT
		{
			s_ptr<GraphicsPipeline> lightShader = ResourceManager::get_inst().find<GraphicsPipeline>(default_res::shader::graphics::directional_light_shader);
			s_ptr<Material> light_material = new_entity<Material>();
			light_material->set_rendering_mode(eRenderingMode::NONE);
			light_material->set_shader(lightShader);

			ResourceManager::get_inst().insert_default_resource(light_material, default_res::material::directional_light_material);
		}

		{
			s_ptr<GraphicsPipeline> point_light_shader = ResourceManager::get_inst().find<GraphicsPipeline>(default_res::shader::graphics::point_light_shader);
			s_ptr<Material> light_material = new_entity<Material>();
			light_material->set_rendering_mode(eRenderingMode::NONE);
			light_material->set_shader(point_light_shader);

			ResourceManager::get_inst().insert_default_resource(light_material, default_res::material::point_light_material);
		}

#pragma endregion

#pragma region MERGE
		s_ptr<GraphicsPipeline> mergeShader = ResourceManager::get_inst().find<GraphicsPipeline>(default_res::shader::graphics::merge_shader);
		s_ptr<Material> merge_material = new_entity<Material>();
		merge_material->set_rendering_mode(eRenderingMode::NONE);
		merge_material->set_shader(mergeShader);

		ResourceManager::get_inst().insert_default_resource(merge_material, default_res::material::merge_material);
#pragma endregion

#pragma region DEBUG
		s_ptr<GraphicsPipeline> debugShader = ResourceManager::get_inst().find<GraphicsPipeline>(shader::graphics::debug_shader);
		s_ptr<DebugMaterial> debug_material = new_entity<DebugMaterial>();
		debug_material->set_rendering_mode(eRenderingMode::forward_transparent);
		debug_material->set_shader(debugShader);

		ResourceManager::get_inst().insert_default_resource(debug_material, material::debug_material);
#pragma endregion
	}
}
