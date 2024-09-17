#include "Engine/Resource/Material/Material.h"

#include "Engine/GPU/ConstBuffer.h"

#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/PathManager.h"

#include "Engine/Util/Serialize/JsonSerializer.h"

#include "Engine/Resource/Texture.h"
#include "Engine/Resource/Shader/GraphicsShader.h"
#include "Engine/GPU/StructBuffer.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Component/Animator/Com_Animator3D.h"


namespace ehw
{
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

    void Material::init_static()
    {
        s_individual_mtrl_buffer = new StructBuffer;
        StructBuffer::Desc desc{};
        desc.eSBufferType = eStructBufferType::READ_ONLY;
        desc.GPU_register_t_SRV = GPU::Register::t::g_individual_mtrl_data;
        desc.GPU_register_u_UAV = GPU::Register::u::NONE;
        s_individual_mtrl_buffer->init<tIndividual_Material_Data>(desc);
    }

    void Material::release_static()
    {
        SAFE_DELETE(s_individual_mtrl_buffer);
    }

    void Material::bind_shared_material_buffer()
    {
        for (size_t slotIndex = 0; slotIndex < (uint)eTextureSlot::END; slotIndex++)
        {
            if (m_textures[slotIndex])
            {
                m_textures[slotIndex]->BindDataSRV((uint)slotIndex, eShaderStageFlag::Pixel);
            }
        }

        ConstBuffer* CB = RenderManager::GetInst().GetConstBuffer(eCBType::Material);
        CB->SetData(&m_shared_material_data);

        eShaderStageFlag_ flag = eShaderStageFlag::Vertex | eShaderStageFlag::Geometry | eShaderStageFlag::Pixel;
        CB->bind_buffer_to_GPU_register(flag);

        m_shader->bind_buffer_to_GPU_register();
    }

    void Material::clear_buffers()
    {
        m_individual_mtrl_data.clear();
    }

    void Material::set_data_to_buffer(const std::vector<GameObject*>& _objs)
    {
        for (GameObject* obj : _objs) {
            add_data_to_buffer(obj);
        }
    }

    void Material::add_data_to_buffer(GameObject* _obj)
    {
        auto* animator = _obj->GetComponent<Com_Animator3D>();
        tIndividual_Material_Data data{};

        if (animator && animator->IsPlaying()) {
            data.bAnim = TRUE;
            data.BoneCount = animator->get_bone_count();
        }

        m_individual_mtrl_data.push_back(data);
    }

    void Material::bind_buffer_to_GPU()
    {
        s_individual_mtrl_buffer->SetData(m_individual_mtrl_data.data(), (UINT)m_individual_mtrl_data.size());
    }

    eResult Material::save(const std::fs::path& _base_directory, const std::fs::path& _key_path) const
    {
        return SaveFile_Json(_base_directory / _key_path);
    }

    eResult Material::load(const std::fs::path& _base_directory, const std::fs::path& _key_path)
    {
        return LoadFile_Json(_base_directory / _key_path);
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
                ser[JSON_KEY(m_shader)] << m_shader->get_concrete_class_name();
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
                    textures[i] << m_textures[i]->get_concrete_class_name();
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
