#include "Engine/Game/Entity.h"

#include "Engine/define_Macro.h"

#include "Engine/Util/AtExit.h"
namespace core
{
	UINT32 Entity::g_nextID{};

	Entity::Entity(const std::string_view _class_concrete_name)
		: m_ID(++g_nextID)
		, m_concrete_class_name(_class_concrete_name)
	{
	}
	Entity::Entity(const Entity& _other)
		: m_ID(++g_nextID)
		, m_concrete_class_name(_other.m_concrete_class_name)
	{
	}
	Entity::~Entity()
	{
	}

	EntityFactory::EntityFactory()
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
