#include "PCH_Engine.h"
#include "guiTreeWidget.h"

namespace editor
{
	TreeWidget::tNode::tNode()
		: EditorEntity("")
		, mTreeWidget(nullptr)
		, mData{}
		, m_Parent(nullptr)
		, mbSelected(false)
		, mbStem(false)
	{
	}
	TreeWidget::tNode::~tNode()
	{
		for (tNode* child : m_Childs)
		{
			delete child;
			child = nullptr;
		}
	}
	// Node
	void TreeWidget::tNode::UpdateUI()
	{
		//ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed;

		int flag = 0;

		if (mbStem)
		{
			flag |= ImGuiTreeNodeFlags_Framed;
		}
		if (mbSelected)
		{
			flag |= ImGuiTreeNodeFlags_Selected;
		}
		if (m_Childs.empty())
		{
			flag |= ImGuiTreeNodeFlags_Leaf;
		}
		if (mbStem && m_Childs.empty())
		{
			SetStrKey("\t" + GetStrKey());
		}
		if (ImGui::TreeNodeEx(GetStrKey().c_str(), flag))
		{
			if (!mbStem && ImGui::IsItemHovered(0) && ImGui::IsMouseClicked(0))
			{
				mTreeWidget->SetSelectedNode(this);
			}

			for (tNode* node : m_Childs)
			{
				node->UpdateUI();
			}

			ImGui::TreePop();
		}
	}

	void TreeWidget::tNode::AddNode(tNode* node)
	{
		node->m_Parent = this;
		m_Childs.push_back(node);
	}

	// Tree
	TreeWidget::TreeWidget()
		: guiChild(strKey::TreeWidget)
		, mRoot(nullptr)
		, mbDummyRootUse{}
		, mSelectedNode{}
		, mEventGUI{}
		, mEvent{}
	{
	}

	TreeWidget::~TreeWidget()
	{
	}


	void TreeWidget::UpdateUI()
	{
		if (mRoot == nullptr)
		{
			return;
		}

		if (!mbDummyRootUse)
		{
			mRoot->UpdateUI();
		}
		else
		{
			const std::vector<tNode*>& childs = mRoot->GetChilds();
			for (tNode* child : childs)
			{
				child->UpdateUI();
			}
		}
	}

	TreeWidget::tNode* TreeWidget::AddNode(tNode* parent, const std::string& name, ehw::tDataPtr data, bool isFrame)
	{
		tNode* node = new tNode;
		node->SetStrKey(name);
		node->SetData(data);
		node->SetStem(isFrame);
		node->mTreeWidget = this;

		if (nullptr == parent)
		{
			mRoot = std::unique_ptr<tNode>(node);
		}
		else
		{
			parent->AddNode(node);
		}

		return node;
	}

	void TreeWidget::Clear()
	{
		mRoot.reset();
	}
	void TreeWidget::SetSelectedNode(tNode* node)
	{
		if (nullptr != mSelectedNode)
		{
			mSelectedNode->mbSelected = false;
		}

		mSelectedNode = node;
		mSelectedNode->mbSelected = true;

		if (mSelectedNode && mEvent)
		{
			mEvent(mSelectedNode->mData);
		}
	}
}
