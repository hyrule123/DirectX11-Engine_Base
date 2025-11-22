#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/GPU/CommonGPU.h"

namespace core
{
    class Shader :
        public Resource
    {
        CLASS_INFO(Shader, Resource);
		BASE_RESOURCE(Shader, eResourceType::Shader);

        NO_SERIALIZE_JSON;

        public:
        Shader();
        virtual ~Shader();
        virtual eResult save(const std::fs::path& _full_path) const override;
        virtual eResult load(const std::fs::path& _full_path) override;

        virtual eResult serialize_binary(BinarySerializer& _ser) const override;
		virtual eResult deserialize_binary(const BinarySerializer& _ser) override;

		virtual void bind_shader(const ComPtr<ID3D11DeviceContext>& _context) = 0;

    protected:
        virtual bool load_shader(const std::vector<unsigned char>& _byte_code) = 0;
    };
}

