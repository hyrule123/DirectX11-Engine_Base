
#include "Entity.h"

#include <Engine/define_Macro.h>
#include <Engine/Util/AtExit.h>

#include <Engine/Util/Serialize/BinarySerializer.h>
#include <Engine/Util/Serialize/JsonSerializer.h>

namespace core
{
	uint32 Entity::s_next_instance_ID = 0;
	uint32 Entity::s_next_class_ID = 0;

	Entity::Entity()
		: m_instance_ID{ ++s_next_instance_ID }
		, m_concrete_class_ID{}
		, m_concrete_class_name{}
	{
	}

	void Entity::init()
	{
		m_concrete_class_ID = get_concrete_class_ID_internal();
		m_concrete_class_name = get_concrete_class_name_internal();
	}

	Entity::~Entity()
	{
	}

	Entity::Entity(const Entity& _other)
		: m_instance_ID{ ++s_next_instance_ID }
		, m_concrete_class_ID{ _other.m_concrete_class_ID }
		, m_concrete_class_name{ _other.m_concrete_class_name }
	{
	}

	eResult Entity::serialize_binary(BinarySerializer& _ser) const
	{
		_ser << m_concrete_class_name;
		return eResult::Success;
	}

	eResult Entity::deserialize_binary(const BinarySerializer& _ser)
	{
		std::string compare;
		_ser >> compare;
		if (compare != m_concrete_class_name)
		{
			ERROR_MESSAGE("Serialize 대상 클래스 미일치");
			return eResult::Fail;
		}
		return eResult::Success;
	}

	eResult Entity::serialize_json(JsonSerializer& _ser) const
	{
		_ser[JSON_KEY(m_concrete_class_name)] << m_concrete_class_name;
		return eResult::Success;
	}

	eResult Entity::deserialize_json(const JsonSerializer& _ser)
	{
		std::string compare;
		_ser[JSON_KEY(m_concrete_class_name)] >> compare;
		if (compare != m_concrete_class_name)
		{
			ERROR_MESSAGE("Serialize 대상 클래스 미일치");
			return eResult::Fail;
		}
		return eResult::Success;
	}


	EntityFactory::EntityFactory()
		: m_ctors{}
		, m_next_class_ID{}
	{}
	void EntityFactory::init()
	{
		AtExit::add_func(EntityFactory::destroy_inst);
	}

	EntityFactory::~EntityFactory()	
	{
	}
	s_ptr<Entity> EntityFactory::instantiate(const std::string_view key)
	{
		s_ptr<Entity> ret = nullptr;
		auto iter = m_ctors.find(key);
		if (iter != m_ctors.end()) {
			ret = iter->second();
		}
		return ret;
	}
}
