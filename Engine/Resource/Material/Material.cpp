#include "Engine/Resource/Material/Material.h"

#include "Engine/GPU/ConstBuffer.h"

#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/PathManager.h"

#include "Engine/Util/Serialize/JsonSerializer.h"

#include "Engine/Resource/Texture.h"
#include "Engine/Resource/Shader/GraphicsShader.h"

#include "Engine/Game/GameObject.h"

namespace ehw
{
    Material::Material()
        : Resource(Material::concrete_class_name)
        , m_shared_material_data{}
        , m_renderingMode(eRenderingMode::forward_opaque)
        , m_shader{}
        , m_textures{}
    {
    }
    Material::Material(std::string_view key)
        : Resource(key)
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

    eResult Material::save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const
    {
        return SaveFile_Json(_base_directory / _resource_name);
    }

    eResult Material::load(const std::fs::path& _base_directory, const std::fs::path& _resource_name)
    {
        return LoadFile_Json(_base_directory / _resource_name);
    }

    eResult Material::serialize_json(JsonSerializer* _ser) const
    {
        if (nullptr == _ser)
        {
            ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
            return eResult::Fail_Nullptr;
        }
        JsonSerializer& ser = *_ser;

        try
        {
            //shader
            if (m_shader)
            {
                ser[JSON_KEY(m_shader)] << m_shader->get_resource_name();
            }
            else
            {
                ser[JSON_KEY(m_shader)] << Json::nullValue;
            }

            //textures
            Json::Value& textures = ser[JSON_KEY(m_textures)];
            textures.resize((Json::ArrayIndex)m_textures.size());
            for (size_t i = 0; i < m_textures.size(); ++i)
            {
                if (m_textures[i])
                {
                    textures[i] << m_textures[i]->get_resource_name();
                }
                else
                {
                    textures[i] << Json::nullValue;
                }
            }

            //renderingMode
            ser[JSON_KEY(m_renderingMode)] << m_renderingMode;

            //const buffer
            Json::Value& cbData = ser[JSON_KEY(m_shared_material_data)];
            cbData[JSON_KEY(m_shared_material_data.Amb)] << m_shared_material_data.Amb;
            cbData[JSON_KEY(m_shared_material_data.Diff)] << m_shared_material_data.Diff;
            cbData[JSON_KEY(m_shared_material_data.Emv)] << m_shared_material_data.Emv;
            cbData[JSON_KEY(m_shared_material_data.Spec)] << m_shared_material_data.Spec;
            //m_constBuffer의 btex는 저장하지 않음(m_textures와 내용 중복)
        }
        catch (const Json::Exception& _err)
        {
            ERROR_MESSAGE_A(_err.what());
            return eResult::Fail_InValid;
        }

        return eResult::Success;
    }

    eResult Material::deserialize_json(const JsonSerializer* _ser)
    {
        if (nullptr == _ser)
        {
            ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
            return eResult::Fail_Nullptr;
        }

        const JsonSerializer& ser = *_ser;

        try
        {
            //shader
            {
                std::string name{};
                ser[JSON_KEY(m_shader)] >> name;
                m_shader = ResourceManager<GraphicsShader>::get_inst().load(name);
            }


            //textures
            const Json::Value& textures = ser[JSON_KEY(m_textures)];
            for (size_t i = 0; i < m_textures.size(); ++i)
            {
                std::string name{};
                textures[i] >> name;
                const std::shared_ptr<Texture>& tex = ResourceManager<Texture>::get_inst().load(name);
                if (tex)
                {
                    set_texture((eTextureSlot)i, tex);
                }
            }

            //const buffer
            const Json::Value& cbData = ser[JSON_KEY(m_shared_material_data)];
            cbData[JSON_KEY(m_shared_material_data.Amb)] >> m_shared_material_data.Amb;
            cbData[JSON_KEY(m_shared_material_data.Diff)] >> m_shared_material_data.Diff;
            cbData[JSON_KEY(m_shared_material_data.Emv)] >> m_shared_material_data.Emv;
            cbData[JSON_KEY(m_shared_material_data.Spec)] >> m_shared_material_data.Spec;
            //m_constBuffer의 btex는 저장하지 않음(m_textures와 내용 중복)

            //renderingMode
            ser[JSON_KEY(m_renderingMode)] >> m_renderingMode;
        }
        catch (const Json::Exception& _err)
        {
            ERROR_MESSAGE_A(_err.what());
            return eResult::Fail_InValid;
        }

        return eResult::Success;
    }

    void Material::set_texture(eTextureSlot _slot, const std::shared_ptr<Texture>& _texture)
    {
        m_textures[(UINT)_slot] = _texture;
        BOOL bTex = nullptr != _texture ? TRUE : FALSE;
        switch ((UINT)_slot)
        {
        case 0u:
            m_shared_material_data.bTex_0 = bTex;
            break;
        case 1u:
            m_shared_material_data.bTex_1 = bTex;
            break;
        case 2u:
            m_shared_material_data.bTex_2 = bTex;
            break;
        case 3u:
            m_shared_material_data.bTex_3 = bTex;
            break;
        case 4u:
            m_shared_material_data.bTex_4 = bTex;
            break;
        case 5u:
            m_shared_material_data.bTex_5 = bTex;
            break;
        case 6u:
            m_shared_material_data.bTex_6 = bTex;
            break;
        case 7u:
            m_shared_material_data.bTex_7 = bTex;
            break;
        default:
            ASSERT(false, "에러");
            break;
        }
    }


}
