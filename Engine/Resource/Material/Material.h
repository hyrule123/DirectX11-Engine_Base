#pragma once
#include "Engine/define_Macro.h"
#include "Engine/define_Enum.h"

#include "Engine/GPU/CommonGPU.h"

#include "Engine/Util/Serialize/Serializable.h"

#include "Engine/Resource/Resource.h"

#include <array>


namespace ehw
{
	class Texture;
	class GraphicsShader;
	class Material 
		: public Resource
		, public Serializable_Json
	{
		CLASS_NAME(Material);
	public:
		Material(std::string_view key);

		Material(const Material& _other);

		virtual ~Material();

		// 필수 재정의 함수들 //
		virtual void clear_buffer() = 0; //버퍼 클리어
		virtual void upload_buffer_to_gpu(const std::vector<GameObject*>& _objs) = 0;	//버퍼 업로드
		virtual void bind_buffer_to_gpu_register();	//이 함수는 재정의했어도 부모함수를 호출해주어야 한다.
		virtual void unbind_buffer_from_gpu_register();
		///////////////////////////////

		virtual eResult save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const override;
		virtual eResult load(const std::fs::path& _baseDir, const std::fs::path& _key_path) override;

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		void set_shader(const std::shared_ptr<GraphicsShader>& _shader) { m_shader = _shader; }
		const std::shared_ptr<GraphicsShader>& get_shader() const { return m_shader; }

		void set_texture(eTextureSlot _slot, const std::shared_ptr<Texture>& _texture);
		const std::shared_ptr<Texture>& get_texture(eTextureSlot _slot) const { return m_textures[(uint)_slot]; }

		eRenderingMode get_rendering_mode() const { return m_renderingMode; }
		void set_rendering_mode(eRenderingMode _mode) { m_renderingMode = _mode; }

		void set_diffuse(const float4& _diff) { m_const_buffer_data.Diff = _diff; }
		void set_specular(const float4& _spec) { m_const_buffer_data.Spec = _spec; }
		void set_amibence(const float4& _amb) { m_const_buffer_data.Amb = _amb; }
		void set_emissvie(const float4& _emsv) { m_const_buffer_data.Emv = _emsv; }

		void SetAnim3D(bool _bAnim3D) { m_const_buffer_data.bAnim = (BOOL)_bAnim3D; }
		void SetBoneCount(int _iBoneCount) { m_const_buffer_data.BoneCount = _iBoneCount; }

		float4 GetDiffuseColor() const { return m_const_buffer_data.Diff; }
		float4 GetSpecularColor() const { return m_const_buffer_data.Spec; }
		float4 GetAmbientColor() const { return m_const_buffer_data.Amb; }
		float4 GetEmissiveColor() const { return m_const_buffer_data.Emv; }

		const void SetDiffuseColor(const float4& _diff)		{ m_const_buffer_data.Diff = _diff; }
		const void SetSpecularColor(const float4& _spec)	{ m_const_buffer_data.Spec = _spec; }
		const void SetAmbientColor(const float4& _amb)		{ m_const_buffer_data.Amb = _amb; }
		const void SetEmissiveColor(const float4& _emsv)	{ m_const_buffer_data.Emv = _emsv; }
			
	private:
		std::shared_ptr<GraphicsShader> m_shader;
		std::array<std::shared_ptr<Texture>, (int)eTextureSlot::END> m_textures;

		eRenderingMode m_renderingMode;
		tCB_MaterialData m_const_buffer_data;
	};
}

