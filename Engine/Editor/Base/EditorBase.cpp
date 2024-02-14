
#include "EditorBase.h"

#include "../../Define_Macro.h"

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

	std::shared_ptr<EditorBase> EditorBase::AddChild(const std::shared_ptr<EditorBase>& _pChild)
	{
		std::shared_ptr<EditorBase> ret = nullptr;

		if (_pChild)
		{
			_pChild->MakeUniqueKeyByName();

			auto parent = shared_from_this();
			_pChild->SetParent(std::static_pointer_cast<EditorBase>(parent));
			m_Childs.push_back(_pChild);

			ret = _pChild;
		}

		return ret;
	}

	void EditorBase::ClearChild()
	{
		m_Childs.clear();
	}


	void EditorBase::RemoveChild(const std::shared_ptr<EditorBase>& _pChild)
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
