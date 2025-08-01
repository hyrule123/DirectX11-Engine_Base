#include "Material.h"

#include <Engine/GPU/ConstBuffer.h>

#include <Engine/Manager/ResourceManager.h>
#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/PathManager.h>

#include <Engine/Util/Serialize/JsonSerializer.h>

#include <Engine/Resource/Texture.h>
#include <Engine/Resource/Shader/GraphicsShader.h>

#include <Engine/Game/GameObject.h>

namespace core
{
    Material::Material()
        : Resource()
        , m_shared_material_data{}
        , m_renderingMode(eRenderingMode::forward_opaque)
        , m_shader{}
        , m_textures{}
    {
    }

    Material::Material(const Material& _other)
        : Resource(_other)
        , m_shader(_other.m_shader)
        , m_textures(_other.m_textures)
        , m_shared_material_data(_other.m_shared_material_data)
        , m_renderingMode(_other.m_renderingMode)
    {
    }

    Material::~Material()
    {
    }

    void Material::bind_shader()
    {
        for (size_t slotIndex = 0; slotIndex < (uint)eTextureSlot::END; slotIndex++)
        {
            if (m_textures[slotIndex])
            {
                m_textures[slotIndex]->bind_buffer_as_SRV((uint)slotIndex, eShaderStageFlag::Pixel);
            }
        }

        ConstBuffer* CB = RenderManager::get_inst().get_const_buffer(eCBType::Material);
        CB->set_data(&m_shared_material_data);

        eShaderStageFlag_ flag = eShaderStageFlag::Vertex | eShaderStageFlag::Geometry | eShaderStageFlag::Pixel;
        CB->bind_buffer_to_GPU_register(flag);

        m_shader->bind_shader();
    }

    eResult Material::save(const std::fs::path& _base_directory) const
    {
        return save_file_json(_base_directory);
    }

    eResult Material::load(const std::fs::path& _base_directory)
    {
        return load_file_json(_base_directory);
    }

    eResult Material::serialize_json(JsonSerializer& _ser) const
    {
        eResult result = Super::serialize_json(_ser);
        if (eResult_fail(result)) { return result; }

        try
        {
            //shader
            if (m_shader)
            {
                _ser[JSON_KEY(m_shader)] << m_shader->get_res_filename();
            }
            else
            {
                _ser[JSON_KEY(m_shader)] << Json::nullValue;
            }

            //textures
            Json::Value& textures = _ser[JSON_KEY(m_textures)];
            textures.resize((Json::ArrayIndex)m_textures.size());
            for (size_t i = 0; i < m_textures.size(); ++i)
            {
                if (m_textures[i])
                {
                    textures[i] << m_textures[i]->get_res_filename();
                }
                else
                {
                    textures[i] << Json::nullValue;
                }
            }

            //renderingMode
            _ser[JSON_KEY(m_renderingMode)] << m_renderingMode;

            //const buffer
            Json::Value& cbData = _ser[JSON_KEY(m_shared_material_data)];
            cbData[JSON_KEY(m_shared_material_data.ambient)] << m_shared_material_data.ambient;
            cbData[JSON_KEY(m_shared_material_data.diffuse)] << m_shared_material_data.diffuse;
            cbData[JSON_KEY(m_shared_material_data.emissive)] << m_shared_material_data.emissive;
            cbData[JSON_KEY(m_shared_material_data.specular)] << m_shared_material_data.specular;
            //m_constBuffer의 btex는 저장하지 않음(m_textures와 내용 중복)
        }
        catch (const Json::Exception& _err)
        {
            ERROR_MESSAGE_A(_err.what());
            return eResult::Fail_InValid;
        }

        return eResult::Success;
    }

    eResult Material::deserialize_json(const JsonSerializer& _ser)
    {
        eResult result = Super::deserialize_json(_ser);
        if (eResult_fail(result)) { return result; }

        try
        {
            //shader
            {
                std::string name{};
                _ser[JSON_KEY(m_shader)] >> name;
                m_shader = ResourceManager<GraphicsShader>::get_inst().load(name);
            }


            //textures
            const Json::Value& textures = _ser[JSON_KEY(m_textures)];
            for (size_t i = 0; i < m_textures.size(); ++i)
            {
                std::string name{};
                textures[i] >> name;
                const s_ptr<Texture>& tex = ResourceManager<Texture>::get_inst().load(name);
                if (tex)
                {
                    set_texture((eTextureSlot)i, tex);
                }
            }

            //const buffer
            const Json::Value& cbData = _ser[JSON_KEY(m_shared_material_data)];
            cbData[JSON_KEY(m_shared_material_data.ambient)] >> m_shared_material_data.ambient;
            cbData[JSON_KEY(m_shared_material_data.diffuse)] >> m_shared_material_data.diffuse;
            cbData[JSON_KEY(m_shared_material_data.emissive)] >> m_shared_material_data.emissive;
            cbData[JSON_KEY(m_shared_material_data.specular)] >> m_shared_material_data.specular;
            //m_constBuffer의 btex는 저장하지 않음(m_textures와 내용 중복)

            //renderingMode
            _ser[JSON_KEY(m_renderingMode)] >> m_renderingMode;
        }
        catch (const Json::Exception& _err)
        {
            ERROR_MESSAGE_A(_err.what());
            return eResult::Fail_InValid;
        }

        return eResult::Success;
    }

    void Material::set_texture(eTextureSlot _slot, const s_ptr<Texture>& _texture)
    {
        m_textures[(UINT)_slot] = _texture;
        BOOL bTex = nullptr != _texture ? TRUE : FALSE;
        switch ((UINT)_slot)
        {
        case 0u:
            m_shared_material_data.is_texture_0 = bTex;
            break;
        case 1u:
            m_shared_material_data.is_texture_1 = bTex;
            break;
        case 2u:
            m_shared_material_data.is_texture_2 = bTex;
            break;
        case 3u:
            m_shared_material_data.is_texture_3 = bTex;
            break;
        case 4u:
            m_shared_material_data.is_texture_4 = bTex;
            break;
        case 5u:
            m_shared_material_data.is_texture_5 = bTex;
            break;
        case 6u:
            m_shared_material_data.is_texture_6 = bTex;
            break;
        case 7u:
            m_shared_material_data.is_texture_7 = bTex;
            break;
        default:
            ASSERT(false, "에러");
            break;
        }
    }


}
