#pragma once
#include "Engine/define_Macro.h"
#include "Engine/define_Enum.h"

namespace ehw
{
	//enum class eResourceType
	//{
	//	UNKNOWN = -1,
	//	Mesh,
	//	Model3D,

	//	Texture,
	//	Material,
	//	//Sound,
	//	/*Font,*/
	//	Animation,

	//	AudioClip,
	//	Prefab,
	//	
	//	GraphicsShader,
	//	ComputeShader,
	//	END,

	//};

	namespace strKey
	{
		namespace path
		{
			namespace directory
			{
				namespace resource
				{
					STRKEY Resource = "Res";

					STRKEY Texture = "Texture";
					STRKEY Mesh = "Mesh";

					STRKEY Material = "Material";

					STRKEY Animation2D = "Animation2D";

					//모델링 설정에 따라 애니메이션을 사용할수도 안할수도 있음
					STRKEY Model3D = "Model3D";
					STRKEY Skeleton = "Engine/Resource/Model3D/Skeleton";

					STRKEY AudioClip = "AudioClip";

					STRKEY GraphicsShader = "Shader/Graphics";
					STRKEY ComputeShader = "Shader/Compute";
				}


				STRKEY CompiledShader = "Shader";
			}

			namespace extension
			{
				STRKEY CompiledShader = ".cso";
				STRKEY Mesh = ".mesh";
				STRKEY Model3D = ".json";
				STRKEY Material = ".json";
				STRKEY Skeleton = ".sklt";
				STRKEY Anim3D = ".a3d";
				STRKEY ShaderSetting = ".json";
				STRKEY Texture[] =
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
				STRKEY_DECLARE(PointMesh);
				STRKEY_DECLARE(RectMesh);
				STRKEY_DECLARE(DebugRectMesh);
				STRKEY_DECLARE(CircleMesh);
				STRKEY_DECLARE(CubeMesh);
				STRKEY_DECLARE(SphereMesh);
			}

			namespace material
			{
				STRKEY_DECLARE(RectMaterial);
				STRKEY_DECLARE(SpriteMaterial);
				STRKEY_DECLARE(UIMaterial);
				STRKEY_DECLARE(GridMaterial);
				STRKEY_DECLARE(DebugMaterial);
				STRKEY_DECLARE(ParticleMaterial);
				STRKEY_DECLARE(Basic3DMaterial);
				STRKEY_DECLARE(PostProcessMaterial);
				STRKEY_DECLARE(DefferedMaterial);
				STRKEY_DECLARE(MergeMaterial);
				STRKEY_DECLARE(LightDirMaterial);
				STRKEY_DECLARE(LightPointMaterial);
			}

			namespace texture
			{
				STRKEY DefaultSprite = "DefaultSprite.png";
				STRKEY CartoonSmoke = "particle\\CartoonSmoke.png";
				STRKEY noise_01 = "noise\\noise_01.png";
				STRKEY noise_02 = "noise\\noise_02.png";
				STRKEY noise_03 = "noise\\noise_03.jpg";
				STRKEY BasicCube = "Cube\\TILE_01.tga";
				STRKEY BasicCubeNormal = "Cube\\TILE_01_N.tga";
				STRKEY Brick = "Cube\\Brick.jpg";
				STRKEY Brick_N = "Cube\\Brick_N.jpg";
				STRKEY PaintTexture = "PaintTexture";
				STRKEY RenderTarget = "RenderTarget";
			}

			namespace shader
			{
				namespace graphics
				{
					STRKEY_DECLARE(RectShader);
					STRKEY_DECLARE(SpriteShader);
					STRKEY_DECLARE(UIShader);
					STRKEY_DECLARE(GridShader);
					STRKEY_DECLARE(DebugShader);
					STRKEY_DECLARE(NormalConvertShader);
					STRKEY_DECLARE(ParticleShader);
					STRKEY_DECLARE(PostProcessShader);
					STRKEY_DECLARE(Basic3DShader);
					STRKEY_DECLARE(DefferedShader);
					//STRKEY_DECLARE(LightShader);
					STRKEY_DECLARE(LightDirShader);
					STRKEY_DECLARE(LightPointShader);
					STRKEY_DECLARE(MergeShader);
				}
				namespace compute
				{
					STRKEY_DECLARE(ParticleCS);
					STRKEY_DECLARE(PostProcessShader);
					STRKEY_DECLARE(BasicShader);
					STRKEY_DECLARE(Animation3D);
					STRKEY_DECLARE(NormalConvert);
					STRKEY_DECLARE(GPUInitSetting);
				}
			}
		}

	}



}
