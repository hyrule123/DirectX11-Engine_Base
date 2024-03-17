#include "Editor/Base/EditorBase.h"
#include "Editor/EditorManager.h"

#include <Engine/Define_Macro.h>
#include <Engine/Util/Serialize/JsonSerializer.h>


namespace ehw::editor
{
	EditorBase::EditorBase(const std::string_view _strName)
		: EditorEntity(_strName)
		, m_parent()
		, m_childs{}
		, m_bNoChild()
		, m_bEnable(true)
	{
	}

	EditorBase::~EditorBase()
	{
	}
	eResult EditorBase::Serialize_Json(JsonSerializer* _ser) const
	{
		using namespace ehw;

		SERIALIZER_CHECK_PTR(_ser);

		JsonSerializer& ser = *_ser;

		try
		{
			ser[JSON_KEY(m_bEnable)] << m_bEnable;

			JsonSerializer& childs = ser[JSON_KEY(m_childs)];
			for (size_t i = 0; i < m_childs.size(); ++i)
			{
				//에디터 child의 경우 정적으로 생성되므로 StrKey를 별도로 저장할 필요가 없음.
				JsonSerializer& child = childs.append(Json::Value());
				
				eResult result = m_childs[i]->Serialize_Json(&child);
				if (eResultFail(result))
				{
					ERROR_MESSAGE("Child 저장 실패");
					return result;
				}
			}
		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail;
		}


		return eResult::Success;
	}
	eResult EditorBase::DeSerialize_Json(const JsonSerializer* _ser)
	{
		using namespace ehw;
		SERIALIZER_CHECK_PTR(_ser);
		
		const JsonSerializer& ser = *_ser;

		try
		{
			ser[JSON_KEY(m_bEnable)] >> m_bEnable;

			const JsonSerializer& childs = ser[JSON_KEY(m_childs)];
			for (size_t i = 0; i < m_childs.size(); ++i)
			{
				//에디터 child의 경우 정적으로 생성되므로 StrKey를 별도로 저장할 필요가 없음.
				if (childs.isValidIndex((Json::ArrayIndex)i))
				{
					const JsonSerializer& child = childs[i];

					eResult result = m_childs[i]->DeSerialize_Json(&child);
					if (eResultFail(result))
					{
						ERROR_MESSAGE("Child 저장 실패");
						return result;
					}
				}
			}
		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail;
		}

		return eResult::Success;
	}


	void EditorBase::InitRecursive()
	{
		Init();
		for (size_t i = 0; i < m_childs.size(); ++i)
		{
			m_childs[i]->InitRecursive();
		}
	}
	void EditorBase::FinalUpdate()
	{
		if (false == GetEnable())
			return;

		//자신의 고유 Update 항목을 호출
		Update();

		if (true == BeginUI())
		{
			UpdateUI();
			for (size_t i = 0; i < m_childs.size(); ++i)
			{
				m_childs[i]->FinalUpdate();
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
			m_childs.push_back(_pChild);

			ret = _pChild;
		}

		return ret;
	}

	void EditorBase::ClearChild()
	{
		m_childs.clear();
	}


	void EditorBase::RemoveChild(const std::shared_ptr<EditorBase>& _pChild)
	{
		for (auto iter = m_childs.begin(); iter != m_childs.end(); ++iter)
		{
			if ((*iter) == _pChild)
			{
				(*iter)->SetParent(nullptr);
				m_childs.erase(iter);
				break;
			}
		}
	}
}
