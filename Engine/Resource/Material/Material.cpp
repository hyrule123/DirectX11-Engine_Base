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
    Material::Material(std::string_view key)
        : Resource(key)
        , m_const_buffer_data{}
        , m_renderingMode(eRenderingMode::forward_opaque)
        , m_shader{}
        , m_textures{}
    {
    }

    Material::Material(const Material& _other)
        : Resource(_other)
        , m_shader(_other.m_shader)
        , m_textures(_other.m_textures)
        , m_const_buffer_data(_other.m_const_buffer_data)
        , m_renderingMode(_other.m_renderingMode)
    {
    }

    Material::~Material()
    {
    }

    eResult Material::save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const
    {
        return SaveFile_Json(_baseDir / _key_path);
    }

    eResult Material::load(const std::fs::path& _baseDir, const std::fs::path& _key_path)
    {
        return LoadFile_Json(_baseDir / _key_path);
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
                ser[JSON_KEY(m_shader)] << m_shader->get_strkey();
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
                    textures[i] << m_textures[i]->get_strkey();
                }
                else
                {
                    textures[i] << Json::nullValue;
                }
            }

            //renderingMode
            ser[JSON_KEY(m_renderingMode)] << m_renderingMode;

            //const buffer
            Json::Value& cbData = ser[JSON_KEY(m_const_buffer_data)];
            cbData[JSON_KEY(m_const_buffer_data.Amb)] << m_const_buffer_data.Amb;
            cbData[JSON_KEY(m_const_buffer_data.bAnim)] << m_const_buffer_data.bAnim;
            cbData[JSON_KEY(m_const_buffer_data.BoneCount)] << m_const_buffer_data.BoneCount;
            cbData[JSON_KEY(m_const_buffer_data.Diff)] << m_const_buffer_data.Diff;
            cbData[JSON_KEY(m_const_buffer_data.Emv)] << m_const_buffer_data.Emv;
            cbData[JSON_KEY(m_const_buffer_data.Spec)] << m_const_buffer_data.Spec;
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
                std::string strKey{};
                ser[JSON_KEY(m_shader)] >> strKey;
                m_shader = ResourceManager<GraphicsShader>::GetInst().load(strKey);
            }


            //textures
            const Json::Value& textures = ser[JSON_KEY(m_textures)];
            for (size_t i = 0; i < m_textures.size(); ++i)
            {
                std::string strKey{};
                textures[i] >> strKey;
                const std::shared_ptr<Texture>& tex = ResourceManager<Texture>::GetInst().load(strKey);
                if (tex)
                {
                    set_texture((eTextureSlot)i, tex);
                }
            }

            //const buffer
            const Json::Value& cbData = ser[JSON_KEY(m_const_buffer_data)];
            cbData[JSON_KEY(m_const_buffer_data.Amb)] >> m_const_buffer_data.Amb;
            cbData[JSON_KEY(m_const_buffer_data.bAnim)] >> m_const_buffer_data.bAnim;
            cbData[JSON_KEY(m_const_buffer_data.BoneCount)] >> m_const_buffer_data.BoneCount;
            cbData[JSON_KEY(m_const_buffer_data.Diff)] >> m_const_buffer_data.Diff;
            cbData[JSON_KEY(m_const_buffer_data.Emv)] >> m_const_buffer_data.Emv;
            cbData[JSON_KEY(m_const_buffer_data.Spec)] >> m_const_buffer_data.Spec;
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

    void Material::clear_buffer()
    {

    }

    void Material::bind_buffer_to_gpu_register()
    {
        for (size_t slotIndex = 0; slotIndex < (uint)eTextureSlot::END; slotIndex++)
        {
            if (m_textures[slotIndex])
            {
                m_textures[slotIndex]->BindDataSRV((uint)slotIndex, eShaderStageFlag::Pixel);
            }
        }

        ConstBuffer* CB = RenderManager::GetInst().GetConstBuffer(eCBType::Material);
        CB->SetData(&m_const_buffer_data);

        eShaderStageFlag_ flag = eShaderStageFlag::Vertex | eShaderStageFlag::Geometry | eShaderStageFlag::Pixel;
        CB->bind_data(flag);

        m_shader->bind_data();
    }

    void Material::unbind_buffer_from_gpu_register()
    {
        GraphicsShader::unbind_data();

        ConstBuffer* CB = RenderManager::GetInst().GetConstBuffer(eCBType::Material);
        CB->unbind_data();

        Texture::ClearAll();
    }

    void Material::set_texture(eTextureSlot _slot, const std::shared_ptr<Texture>& _texture)
    {
        m_textures[(UINT)_slot] = _texture;
        BOOL bTex = nullptr != _texture ? TRUE : FALSE;
        switch ((UINT)_slot)
        {
        case 0u:
            m_const_buffer_data.bTex_0 = bTex;
            break;
        case 1u:
            m_const_buffer_data.bTex_1 = bTex;
            break;
        case 2u:
            m_const_buffer_data.bTex_2 = bTex;
            break;
        case 3u:
            m_const_buffer_data.bTex_3 = bTex;
            break;
        case 4u:
            m_const_buffer_data.bTex_4 = bTex;
            break;
        case 5u:
            m_const_buffer_data.bTex_5 = bTex;
            break;
        case 6u:
            m_const_buffer_data.bTex_6 = bTex;
            break;
        case 7u:
            m_const_buffer_data.bTex_7 = bTex;
            break;
        default:
            ASSERT(false, "에러");
            break;
        }
    }
}
