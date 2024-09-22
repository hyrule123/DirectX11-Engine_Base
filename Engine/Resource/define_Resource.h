#pragma once
#include "Engine/define_Macro.h"
#include "Engine/define_Enum.h"

namespace ehw
{
	namespace name
	{
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
					STR_KEY Skeleton = "Engine/Resource/Model3D/Skeleton";

					STR_KEY AudioClip = "AudioClip";

					STR_KEY GraphicsShader = "Shader/Graphics";
					STR_KEY ComputeShader = "Shader/Compute";
				}


				STR_KEY CompiledShader = "Shader";
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
				constexpr size_t Texture_ArrSize = sizeof(Texture) / sizeof(const char*);
			}
		}

		


		namespace defaultRes
		{
			namespace mesh
			{
				STR_KEY_DECLARE(PointMesh);
				STR_KEY_DECLARE(RectMesh);
				STR_KEY_DECLARE(DebugRectMesh);
				STR_KEY_DECLARE(DebugCubeMesh);
				STR_KEY_DECLARE(CircleMesh);
				STR_KEY_DECLARE(CubeMesh);

				STR_KEY_DECLARE(SphereMesh);
			}

			namespace material
			{
				STR_KEY_DECLARE(RectMaterial);
				STR_KEY_DECLARE(SpriteMaterial);
				STR_KEY_DECLARE(UIMaterial);
				STR_KEY_DECLARE(GridMaterial);
				STR_KEY_DECLARE(DebugMaterial);
				STR_KEY_DECLARE(ParticleMaterial);
				STR_KEY_DECLARE(Basic3DMaterial);
				STR_KEY_DECLARE(PostProcessMaterial);
				STR_KEY_DECLARE(Deffered3DMaterial);
				STR_KEY_DECLARE(MergeMaterial);
				STR_KEY_DECLARE(LightDirMaterial);
				STR_KEY_DECLARE(LightPointMaterial);
			}

			namespace texture
			{
				STR_KEY DefaultSprite = "DefaultSprite.png";
				STR_KEY CartoonSmoke = "particle\\CartoonSmoke.png";
				STR_KEY noise_01 = "noise\\noise_01.png";
				STR_KEY noise_02 = "noise\\noise_02.png";
				STR_KEY noise_03 = "noise\\noise_03.jpg";
				STR_KEY BasicCube = "Cube\\TILE_01.tga";
				STR_KEY BasicCubeNormal = "Cube\\TILE_01_N.tga";
				STR_KEY Brick = "Cube\\Brick.jpg";
				STR_KEY Brick_N = "Cube\\Brick_N.jpg";
				STR_KEY PaintTexture = "PaintTexture";
				STR_KEY RenderTarget = "RenderTarget";
			}

			namespace shader
			{
				namespace graphics
				{
					STR_KEY_DECLARE(RectShader);
					STR_KEY_DECLARE(SpriteShader);
					STR_KEY_DECLARE(UIShader);
					STR_KEY_DECLARE(GridShader);
					STR_KEY_DECLARE(DebugShader);
					STR_KEY_DECLARE(NormalConvertShader);
					STR_KEY_DECLARE(ParticleShader);
					STR_KEY_DECLARE(PostProcessShader);
					STR_KEY_DECLARE(Forward3DShader);
					STR_KEY_DECLARE(Deffered3DShader);
					//STR_KEY_DECLARE(LightShader);
					STR_KEY_DECLARE(LightDirShader);
					STR_KEY_DECLARE(LightPointShader);
					STR_KEY_DECLARE(MergeShader);
				}
				namespace compute
				{
					STR_KEY_DECLARE(ParticleCS);
					STR_KEY_DECLARE(PostProcessShader);
					STR_KEY_DECLARE(BasicShader);
					STR_KEY_DECLARE(Animation3D);
					STR_KEY_DECLARE(NormalConvert);
					STR_KEY_DECLARE(GPUInitSetting);
				}
			}
		}

	}



}
