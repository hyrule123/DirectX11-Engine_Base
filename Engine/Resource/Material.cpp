#include "Material.h"

#include "GPU/ConstBuffer.h"

#include "Manager/ResourceManager.h"
#include "Manager/RenderManager.h"
#include "Manager/PathManager.h"

#include "Util/Serialize/JsonSerializer.h"

#include "Resource/Texture.h"
#include "Shader/GraphicsShader.h"

namespace ehw
{
    Material::Material()
        : iResource(typeid(Material))
        , m_constBufferData{}
        , m_renderingMode(eRenderingMode::Opaque)
        , m_shader{}
        , m_textures{}
    {
    }

    Material::Material(const Material& _other)
        : iResource(_other)
        , m_shader(_other.m_shader)
        , m_textures(_other.m_textures)
        , m_constBufferData(_other.m_constBufferData)
        , m_renderingMode(_other.m_renderingMode)
    {
    }

    Material::~Material()
    {
    }

    eResult Material::Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) const
    {
        return SaveFile_Json(_baseDir / _strKeyPath);
    }

    eResult Material::Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath)
    {
        return LoadFile_Json(_baseDir / _strKeyPath);
    }

    eResult Material::Serialize_Json(JsonSerializer* _ser) const
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
                ser[JSON_KEY(m_shader)] << m_shader->GetStrKey();
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
                    textures[i] << m_textures[i]->GetStrKey();
                }
                else
                {
                    textures[i] << Json::nullValue;
                }
            }

            //renderingMode
            ser[JSON_KEY(m_renderingMode)] << m_renderingMode;

            //const buffer
            Json::Value& cbData = ser[JSON_KEY(m_constBufferData)];
            cbData[JSON_KEY(m_constBufferData.Amb)] << m_constBufferData.Amb;
            cbData[JSON_KEY(m_constBufferData.bAnim)] << m_constBufferData.bAnim;
            cbData[JSON_KEY(m_constBufferData.BoneCount)] << m_constBufferData.BoneCount;
            cbData[JSON_KEY(m_constBufferData.Diff)] << m_constBufferData.Diff;
            cbData[JSON_KEY(m_constBufferData.Emv)] << m_constBufferData.Emv;
            cbData[JSON_KEY(m_constBufferData.Spec)] << m_constBufferData.Spec;
            //m_constBuffer의 btex는 저장하지 않음(m_textures와 내용 중복)
        }
        catch (const Json::Exception& _err)
        {
            ERROR_MESSAGE_A(_err.what());
            return eResult::Fail_InValid;
        }

        return eResult::Success;
    }

    eResult Material::DeSerialize_Json(const JsonSerializer* _ser)
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
                m_shader = ResourceManager<GraphicsShader>::Load(strKey);
            }


            //textures
            const Json::Value& textures = ser[JSON_KEY(m_textures)];
            for (size_t i = 0; i < m_textures.size(); ++i)
            {
                std::string strKey{};
                textures[i] >> strKey;
                const std::shared_ptr<Texture>& tex = ResourceManager<Texture>::Load(strKey);
                if (tex)
                {
                    SetTexture((eTextureSlot)i, tex);
                }
            }

            //const buffer
            const Json::Value& cbData = ser[JSON_KEY(m_constBufferData)];
            cbData[JSON_KEY(m_constBufferData.Amb)] >> m_constBufferData.Amb;
            cbData[JSON_KEY(m_constBufferData.bAnim)] >> m_constBufferData.bAnim;
            cbData[JSON_KEY(m_constBufferData.BoneCount)] >> m_constBufferData.BoneCount;
            cbData[JSON_KEY(m_constBufferData.Diff)] >> m_constBufferData.Diff;
            cbData[JSON_KEY(m_constBufferData.Emv)] >> m_constBufferData.Emv;
            cbData[JSON_KEY(m_constBufferData.Spec)] >> m_constBufferData.Spec;
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


    void Material::BindData()
    {
        for (size_t slotIndex = 0; slotIndex < (uint)eTextureSlot::END; slotIndex++)
        {
            if (m_textures[slotIndex])
            {
                m_textures[slotIndex]->BindDataSRV((uint)slotIndex, eShaderStageFlag::Pixel);
            }
            else
            {
                Texture::ClearSRV((UINT)slotIndex);
            }
        }

        static ConstBuffer* CB = RenderManager::GetConstBuffer(eCBType::Material);
        CB->SetData(&m_constBufferData);

        eShaderStageFlag_ flag = eShaderStageFlag::Vertex | eShaderStageFlag::Geometry | eShaderStageFlag::Pixel;
        CB->BindData(flag);

        m_shader->BindData();
    }

    void Material::UnBindData()
    {
        for (size_t slotIndex = 0; slotIndex < (uint)eTextureSlot::END; slotIndex++)
        {
            if (m_textures[slotIndex])
                m_textures[slotIndex]->UnBindData();
        }
    }

    void Material::SetTexture(eTextureSlot _slot, const std::shared_ptr<Texture>& _texture)
    {
        m_textures[(UINT)_slot] = _texture;
        BOOL bTex = nullptr != _texture ? TRUE : FALSE;
        switch ((UINT)_slot)
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
