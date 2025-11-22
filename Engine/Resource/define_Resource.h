#pragma once
#include <Engine/define_Macro.h>
#include <Engine/define_Enum.h>



namespace core
{
	enum class eResourceType
	{
		UNKNOWN,
		VertexBuffer,
		Shader,
		Texture,
		AudioClip,
		GraphicsPipeline,
		ConstBuffer,
		StructBuffer,
		Mesh,
		Material,
		Animation2D,
		Model3D,
		Skeleton,
		//Animation3D,//Skeleton에서 관리됨
		Prefab,
		END
	};

	namespace path
	{
		namespace directory
		{
			namespace resource
			{
				STR_KEY Resource = "Res";

				STR_KEY Texture = "Texture";
				STR_KEY Mesh = "Mesh";

				STR_KEY Material = "Material";

				STR_KEY Animation2D = "Animation2D";

				//모델링 설정에 따라 애니메이션을 사용할수도 안할수도 있음
				STR_KEY Model3D = "Model3D";
				//STR_KEY Skeleton = <Engine/Resource/Model3D/Skeleton";

				STR_KEY AudioClip = "AudioClip";

				STR_KEY Shader = "Shader";
			}
		}

		namespace extension
		{
			STR_KEY CompiledShader = ".cso";
			STR_KEY VertexBuffer = ".vtx";
			STR_KEY Mesh = ".mesh";
			STR_KEY Model3D = ".json";
			STR_KEY Material = ".json";
			STR_KEY Skeleton = ".sklt";
			STR_KEY Anim3D = ".a3d";
			STR_KEY ShaderSetting = ".json";
			STR_KEY Texture[] =
			{
				".png",
				".dds",
				".tga",
				".bmp",
				".jpg",
			};
			constexpr size_t Texture_ArrSize = sizeof(Texture) / sizeof(std::string_view);
		}
	}

		


	namespace default_res
	{
		namespace mesh
		{
			STR_KEY(point_mesh) = "point_mesh";
			STR_KEY(rect_mesh) = "rect_mesh";
			STR_KEY(debug_rect_mesh) = "debug_rect_mesh";
			STR_KEY(debug_cube_mesh) = "debug_cube_mesh";
			STR_KEY(circle_mesh) = "circle_mesh";
			STR_KEY(cube_mesh) = "cube_mesh";

			STR_KEY(sphere_mesh) = "sphere_mesh";
		}

		namespace material
		{
			STR_KEY(rect_material) = "rect_material";
			STR_KEY(sprite_material) = "sprite_material";
			STR_KEY(ui_material) = "ui_material";
			STR_KEY(grid_material) = "grid_material";
			STR_KEY(debug_material) = "debug_material";
			STR_KEY(particle_material) = "particle_material";
			STR_KEY(basic_3D_material) = "basic_3D_material";
			STR_KEY(post_process_material) = "post_process_material";
			STR_KEY(merge_material) = "merge_material";
			STR_KEY(directional_light_material) = "directional_light_material";
			STR_KEY(point_light_material) = "point_light_material";
		}

		namespace texture
		{
			STR_KEY default_sprite = "Texture/DefaultSprite.png";
			STR_KEY cartoon_smoke = "Texture/particle/CartoonSmoke.png";
			STR_KEY noise_01 = "Texture/noise/noise_01.png";
			STR_KEY noise_02 = "Texture/noise/noise_02.png";
			STR_KEY noise_03 = "Texture/noise/noise_03.jpg";
			STR_KEY tile = "Texture/Cube/TILE_01.tga";
			STR_KEY tile_normal = "Texture/Cube/TILE_01_N.tga";
			STR_KEY brick = "Texture/Cube/Brick.jpg";
			STR_KEY brick_normal = "Texture/Cube/Brick_N.jpg";
			STR_KEY paint_texture = "Texture/PaintTexture";
			STR_KEY render_target = "RenderTarget";
		}

		namespace shader
		{
			namespace graphics
			{
				STR_KEY(rect_shader) = "rect_shader";
				STR_KEY(sprite_shader) = "sprite_shader";
				STR_KEY(UI_shader) = "UI_shader";
				STR_KEY(grid_shader) = "grid_shader";
				STR_KEY(debug_shader) = "debug_shader";
				STR_KEY(normal_convert_shader) = "normal_convert_shader";
				STR_KEY(particle_shader) = "particle_shader";
				STR_KEY(post_process_shader) = "post_process_shader";
				STR_KEY(forward_3D_shader) = "forward_3D_shader";
				STR_KEY(deffered_3D_shader) = "deffered_3D_shader";
				//STR_KEY(LightShader);
				STR_KEY(directional_light_shader) = "directional_light_shader";
				STR_KEY(point_light_shader) = "point_light_shader";
				STR_KEY(merge_shader) = "merge_shader";
			}
		}

		namespace buffer
		{
			STR_KEY(DebugRenderBuffer) = "DebugRenderBuffer";
		}
	}
}
