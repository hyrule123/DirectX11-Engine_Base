#pragma once
#include "Engine/define_Enum.h"

#include "Engine/GPU/CommonGPU.h"

#include "Engine/Util/Serialize/Serializable.h"

#include "Engine/Resource/Resource.h"

#include <array>


namespace ehw
{
	class Texture;
	class GraphicsShader;
	class Material final 
		: public Resource
		, public Serializable_Json
	{
		REGISTER_CLASS_INSTANCE_ABLE(Material);
	public:
		Material();
		Material(std::string_view key);

		Material(const Material& _other);
		CLONE_ABLE(Material);

		virtual ~Material();

		virtual eResult Save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const override;
		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _key_path) override;

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		void BindData();
		static void UnbindData();

		void SetShader(const std::shared_ptr<GraphicsShader>& _shader) { m_shader = _shader; }
		const std::shared_ptr<GraphicsShader>& GetShader() const { return m_shader; }


		void SetTexture(eTextureSlot _slot, const std::shared_ptr<Texture>& _texture);
		const std::shared_ptr<Texture>& GetTexture(eTextureSlot _slot) const { return m_textures[(uint)_slot]; }

		eRenderingMode GetRenderingMode() const { return m_renderingMode; }
		void SetRenderingMode(eRenderingMode _mode) { m_renderingMode = _mode; }

		inline void SetMaterialCoefficient(const float4& _vDiff, const float4& _vSpec, const float4& _vAmb, const float4& _vEmis);

		void SetAnim3D(bool _bAnim3D) { m_constBufferData.bAnim = (BOOL)_bAnim3D; }
		void SetBoneCount(int _iBoneCount) { m_constBufferData.BoneCount = _iBoneCount; }

		float4 GetDiffuseColor() const { return m_constBufferData.Diff; }
		float4 GetSpecularColor() const { return m_constBufferData.Spec; }
		float4 GetAmbientColor() const { return m_constBufferData.Amb; }
		float4 GetEmissiveColor() const { return m_constBufferData.Emv; }

		const void SetDiffuseColor(const float4& _diff)		{ m_constBufferData.Diff = _diff; }
		const void SetSpecularColor(const float4& _spec)	{ m_constBufferData.Spec = _spec; }
		const void SetAmbientColor(const float4& _amb)		{ m_constBufferData.Amb = _amb; }
		const void SetEmissiveColor(const float4& _emsv)	{ m_constBufferData.Emv = _emsv; }
			
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

