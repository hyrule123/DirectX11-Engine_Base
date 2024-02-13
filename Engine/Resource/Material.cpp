#include "Material.h"



#include "../GPU/ConstBuffer.h"

#include "../Manager/ResourceManager.h"
#include "../Manager/PathManager.h"

#include "../Util/Serialize/JsonSerializer.h"

namespace ehw
{
    Material::Material()
        : iResource(typeid(Material))
        , m_constBufferData{}
        , m_renderingMode(eRenderingMode::Opaque)
        , m_shader{}
        , mTextures{}
    {
    }

    Material::Material(const Material& _other)
        : iResource(_other)
        , m_shader(_other.m_shader)
        , mTextures(_other.mTextures)
        , m_constBufferData(_other.m_constBufferData)
        , m_renderingMode(_other.m_renderingMode)
    {
    }

    Material::~Material()
    {
    }

    eResult Material::Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath)
    {
        return this->Serializable<JsonSerializer>::SaveFile(_baseDir / _strKeyPath);
    }

    eResult Material::Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath)
    {
        return this->Serializable<JsonSerializer>::LoadFile(_baseDir / _strKeyPath);
    }

    eResult Material::Serialize(JsonSerializer& _ser)
    {
        _ser[JSON_KEY(m_renderingMode)] << m_renderingMode;

        Json::Value& cbData = _ser[JSON_KEY(m_constBufferData)];
        Json::Value& bTexArr = cbData["bTexArr"];
        bTexArr.resize((int)eTextureSlot::END);
        for (int i = 0; i < (int)eTextureSlot::END; ++i)
        {
            bTexArr[i] = true;
        }



        Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(m_constBufferData));
        Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(m_renderingMode));


        //포인터의 경우에는 포인터 자체를 저장하는게 아니라 Key를 저장
        if (m_shader)
        {
            jVal[JSON_KEY(m_shader)] = m_shader->GetStrKey();
        }
        Json::SaveLoad::SavePtrStrKeyVector(_pJVal, JSON_KEY_PAIR(mTextures));

        return eResult::Success;
    }

    eResult Material::DeSerialize(const JsonSerializer& _ser)
    {
        return eResult();
    }

    eResult Material::SaveJson(Json::Value* _pJVal)
    {

    }

    eResult Material::LoadJson(const Json::Value* _pJVal)
    {
        //nullptr 확인
        if (nullptr == _pJVal)
        {
            return eResult::Fail_Nullptr;
        }

        //부모 클래스 저장
        eResult result = iResource::LoadJson(_pJVal);
        if (eResultFail(result))
        {
            return result;
        }
        const Json::Value& jVal = *_pJVal;

        //단순 Value의 경우에는 함수로 바로 불러오기 가능
        Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(m_renderingMode));
        Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(m_constBufferData));

        //쉐이더 데이터가 있는지 확인하고 가져온 키값으로 쉐이더를 로드
        std::string shaderStrKey = Json::SaveLoad::LoadPtrStrKey(_pJVal, JSON_KEY_PAIR(m_shader));
        if (false == shaderStrKey.empty())
        {
            //쉐이더는 Base Path를 사용하지 않는다
            SetShader(ResourceManager<GraphicsShader>::Load(shaderStrKey));
        }
        
        //포인터 배열은 MH::LoadPtrStrKeyVector 함수를 통해서 Key값을 싹 받아올 수 있음.
        const auto& vecLoad = Json::SaveLoad::LoadPtrStrKeyVector(_pJVal, JSON_KEY_PAIR(mTextures));
        for (size_t i = 0; i < vecLoad.size(); ++i)
        {
            if (false == vecLoad[i].empty())
            {
                SetTexture((eTextureSlot)i, ResourceManager<Texture>::Load(vecLoad[i]));
            }
        }
        return eResult::Success;
    }


    void Material::BindData()
    {
        for (size_t slotIndex = 0; slotIndex < (uint)eTextureSlot::END; slotIndex++)
        {
            if (mTextures[slotIndex])
            {
                mTextures[slotIndex]->BindDataSRV((uint)slotIndex, eShaderStageFlag::Pixel);
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
            if (mTextures[slotIndex])
                mTextures[slotIndex]->UnBindData();
        }
    }
}
