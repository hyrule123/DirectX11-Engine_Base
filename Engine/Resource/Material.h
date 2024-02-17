#pragma once
#include "../define_Enum.h"

#include "../GPU/CommonGPU.h"

#include "../Util/Serialize/Serializable.h"

#include "iResource.h"

#include <array>

//에디터 전방선언
namespace editor
{
	class EditorMaterial;
}

namespace ehw
{
	class Texture;
	class GraphicsShader;
	class Material final 
		: public iResource
		, public Serializable_Json
	{
		friend class editor::EditorMaterial;
	public:
		Material();

		Material(const Material& _other);
		CLONE_ABLE(Material);

		virtual ~Material();

		virtual eResult Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;
		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		void BindData();
		void UnBindData();

		void SetShader(const std::shared_ptr<GraphicsShader>& _shader) { m_shader = _shader; }
		const std::shared_ptr<GraphicsShader>& GetShader() const { return m_shader; }


		void SetTexture(eTextureSlot _slot, std::shared_ptr<Texture> _texture);
		std::shared_ptr<Texture> GetTexture(eTextureSlot _slot) const { return m_textures[(uint)_slot]; }

		eRenderingMode GetRenderingMode() const { return m_renderingMode; }
		void SetRenderingMode(eRenderingMode _mode) { m_renderingMode = _mode; }

		inline void SetMaterialCoefficient(const float4& _vDiff, const float4& _vSpec, const float4& _vAmb, const float4& _vEmis);

		void SetAnim3D(bool _bAnim3D) { m_constBufferData.bAnim = (BOOL)_bAnim3D; }
		void SetBoneCount(int _iBoneCount) { m_constBufferData.BoneCount = _iBoneCount; }

			
	private:
		std::shared_ptr<GraphicsShader> m_shader;
		std::array<std::shared_ptr<Texture>, (int)eTextureSlot::END> m_textures;

		eRenderingMode m_renderingMode;
		tCB_MaterialData m_constBufferData;
	};

	inline void Material::SetMaterialCoefficient(const float4& _vDiff, const float4& _vSpec, const float4& _vAmb, const float4& _vEmis)
	{
		m_constBufferData.Diff = _vDiff;
		m_constBufferData.Spec = _vSpec;
		m_constBufferData.Amb = _vAmb;
		m_constBufferData.Emv = _vEmis;
	}
}

