#include "PCH_Engine.h"
#include "EditorBase.h"

#include "define_Macro.h"

#include "json-cpp/json.h"

namespace editor
{
	EditorBase::EditorBase(const std::string_view _strName)
		: EditorEntity(_strName)
		, m_Parent()
		, m_Childs{}
		, mbNoChild()
		, mbEnable(true)
	{
	}

	EditorBase::~EditorBase()
	{
		for (size_t i = 0; i < m_Childs.size(); ++i)
		{
			if (m_Childs[i])
				delete m_Childs[i];
		}
	}
	ehw::eResult EditorBase::SaveJson(Json::Value* _pJval)
	{
		if (nullptr == _pJval)
			return ehw::eResult::Fail_Nullptr;

		ehw::eResult result = EditorEntity::SaveJson(_pJval);
		if (eResultFail(result))
			return result;

		Json::SaveLoad::SaveValue(_pJval, JSON_KEY_PAIR(mbEnable));

		for (size_t i = 0; i < m_Childs.size(); ++i)
		{
			if (m_Childs[i])
				m_Childs[i]->SaveJson(_pJval);
		}

		return ehw::eResult::Success;
	}
	ehw::eResult EditorBase::LoadJson(const Json::Value* _pJval)
	{
		if (nullptr == _pJval)
			return ehw::eResult::Fail_Nullptr;

		ehw::eResult result = EditorEntity::LoadJson(_pJval);
		if (eResultFail(result))
			return result;

		Json::SaveLoad::LoadValue(_pJval, JSON_KEY_PAIR(mbEnable));

		for (size_t i = 0; i < m_Childs.size(); ++i)
		{
			if (m_Childs[i])
				m_Childs[i]->LoadJson(_pJval);
		}

		return ehw::eResult::Success;
	}
	void EditorBase::InitRecursive()
	{
		Init();
		for (size_t i = 0; i < m_Childs.size(); ++i)
		{
			m_Childs[i]->InitRecursive();
		}
	}
	void EditorBase::InternalUpdate()
	{
		if (false == GetEnable())
			return;

		//자신의 고유 Update 항목을 호출
		Update();

		if (true == BeginUI())
		{
			UpdateUI();
			for (size_t i = 0; i < m_Childs.size(); ++i)
			{
				m_Childs[i]->InternalUpdate();
			}

			EndUI();
		}
	}

	void EditorBase::AddChild(EditorBase* _pChild)
	{
		if (_pChild)
		{
			_pChild->MakeUniqueKeyByName();
			_pChild->SetParent(this);
			m_Childs.push_back(_pChild);
		}
	}


	void EditorBase::RemoveChild(EditorBase* _pChild)
	{
		for (auto iter = m_Childs.begin(); iter != m_Childs.end(); ++iter)
		{
			if ((*iter) == _pChild)
			{
				(*iter)->SetParent(nullptr);
				m_Childs.erase(iter);
				break;
			}
		}
	}
}
