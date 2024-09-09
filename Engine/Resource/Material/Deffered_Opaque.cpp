#include "Engine/Resource/Material/Deffered_Opaque.h"

#include "Engine/Manager/ResourceManager.h"
#include "Engine/Resource/Shader/GraphicsShader.h"

#ifdef _DEBUG
#include "Engine/CompiledShaderHeader/VS_Deffered_x64_Debug.h"
#include "Engine/CompiledShaderHeader/PS_Deffered_x64_Debug.h"
#else
#include "Engine/CompiledShaderHeader/VS_Deffered_x64_Release.h"
#include "Engine/CompiledShaderHeader/PS_Deffered_x64_Release.h"
#endif

namespace ehw {
    Deffered_Opaque::Deffered_Opaque()
        : Material(Deffered_Opaque::concrete_name)
    {
        std::shared_ptr<GraphicsShader> shader = ResourceManager<GraphicsShader>::GetInst().load(strKey::defaultRes::shader::graphics::DefferedShader);

        ASSERT_DEBUG(shader);

        set_shader(shader);

        set_rendering_mode(eRenderingMode::DefferdOpaque);
        set_engine_default_res(true);
    }
    Deffered_Opaque::~Deffered_Opaque()
    {
    }
    void Deffered_Opaque::clear_buffer()
    {
    }
    void Deffered_Opaque::upload_buffer_to_gpu(const std::vector<GameObject*>& _objs)
    {
    }
    void Deffered_Opaque::bind_buffer_to_gpu_register()
    {
    }
    void Deffered_Opaque::unbind_buffer_from_gpu_register()
    {
    }
    void Deffered_Opaque::load_dedicated_graphics_shader()
    {
        std::shared_ptr<GraphicsShader> deffered_opaque = std::make_unique<GraphicsShader>();

        eResult result = deffered_opaque->CreateByHeader(eGSStage::Vertex, VS_Deffered, sizeof(VS_Deffered));
        ASSERT(eResult_success(result), "쉐이더 컴파일 실패");

        result = deffered_opaque->CreateByHeader(eGSStage::Pixel, PS_Deffered, sizeof(PS_Deffered));
        ASSERT(eResult_success(result), "쉐이더 컴파일 실패");

        ResourceManager<GraphicsShader>::GetInst().Insert(g_strKey_deffered_opaque_shader, deffered_opaque);
    }
}
