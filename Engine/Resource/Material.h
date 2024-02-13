#pragma once



#include "../define_Enum.h"

#include "../Manager/RenderManager.h"

#include "iResource.h"
#include "Shader/GraphicsShader.h"
#include "Texture.h"

#include <array>

//에디터 전방선언
namespace editor
{
	class EditorMaterial;
}

namespace ehw
{
	class Material final 
		: public iResource
		, public Serializable<JsonSerializer>
	{
		friend class editor::EditorMaterial;
	public:
		Material();

		Material(const Material& _other);
		CLONE_ABLE(Material);

		virtual ~Material();

		virtual eResult Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;
		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;

		virtual eResult Serialize(JsonSerializer& _ser) override;
		virtual eResult DeSerialize(const JsonSerializer& _ser) override;

		void BindData();
		void UnBindData();

		void SetShader(std::shared_ptr<GraphicsShader> _shader) { m_shader = _shader; }
		std::shared_ptr<GraphicsShader> GetShader() const { return m_shader; }


		inline void SetTexture(eTextureSlot slot, std::shared_ptr<Texture> _texture);
		std::shared_ptr<Texture> GetTexture(eTextureSlot _slot) const { return m_textures[(uint)_slot]; }

		eRenderingMode GetRenderingMode() const { return m_renderingMode; }
		void SetRenderingMode(eRenderingMode _mode) { m_renderingMode = _mode; }

		inline void SetMaterialCoefficient(const float4& _vDiff, const float4& _vSpec, const float4& _vAmb, const float4& _vEmis);

		void SetAnim3D(bool _bAnim3D) { m_constBufferData.bAnim = (BOOL)_bAnim3D; }
		void SetBoneCount(int _iBoneCount) { m_constBufferData.BoneCount = _iBoneCount; }

			
	private:
		eRenderingMode m_renderingMode;
		tCB_MaterialData m_constBufferData;
		
		std::shared_ptr<GraphicsShader> m_shader;
		std::array<std::shared_ptr<Texture>, (int)eTextureSlot::END> m_textures;
	};

	inline void Material::SetMaterialCoefficient(const float4& _vDiff, const float4& _vSpec, const float4& _vAmb, const float4& _vEmis)
	{
		m_constBufferData.Diff = _vDiff;
		m_constBufferData.Spec = _vSpec;
		m_constBufferData.Amb = _vAmb;
		m_constBufferData.Emv = _vEmis;
	}

	inline void Material::SetTexture(eTextureSlot slot, std::shared_ptr<Texture> _texture)
	{
		m_textures[(UINT)slot] = _texture;
		BOOL bTex = nullptr != _texture ? TRUE : FALSE;
		switch ((UINT)slot)
		{
		case 0u:
			m_constBufferData.bTex_0 = bTex;
			break;
		case 1u:
			m_constBufferData.bTex_1 = bTex;
			break;
		case 2u:
			m_constBufferData.bTex_2 = bTex;
			break;
		case 3u:
			m_constBufferData.bTex_3 = bTex;
			break;
		case 4u:
			m_constBufferData.bTex_4 = bTex;
			break;
		case 5u:
			m_constBufferData.bTex_5 = bTex;
			break;
		case 6u:
			m_constBufferData.bTex_6 = bTex;
			break;
		case 7u:
			m_constBufferData.bTex_7 = bTex;
			break;
		default:
			ASSERT(false, "에러");
			break;
		}
	}
}

