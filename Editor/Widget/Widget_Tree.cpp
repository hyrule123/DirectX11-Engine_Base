
#include "Widget_Tree.h"

namespace core::editor
{
	TreeNode::TreeNode()
		: EditorEntity(TreeNode::s_static_type_name)
	{
	}

	void TreeNode::init()
	{
		Super::init();

		m_owner;
		m_data_ptr;
		m_parent;
		m_childs;
	}

	TreeNode::~TreeNode()
	{
	}
	// Node
	void TreeNode::update_UI()
	{
		ImGuiTreeNodeFlags flag = m_flag;

		if (m_childs.empty())
		{
			flag |= ImGuiTreeNodeFlags_Leaf;
		}

		if (ImGui::TreeNodeEx(get_unique_name().c_str(), flag))
		{
			if (ImGui::IsItemHovered(0) && ImGui::IsMouseClicked(0))
			{
				if (m_owner.lock())
				{
					m_owner.lock()->set_selected_node(std::static_pointer_cast<TreeNode>(shared_from_this()));
				}
			}

			for (const s_ptr<TreeNode>& node : m_childs)
			{
				node->update_UI();
			}

			ImGui::TreePop();
		}
	}

	void TreeNode::add_child(const s_ptr<TreeNode>& _node)
	{
		if (nullptr == _node) { return; }

		if (_node->get_parent())
		{
			_node->get_parent()->remove_child(_node);
		}

		_node->set_parent(std::static_pointer_cast<TreeNode>(shared_from_this()));
		_node->set_owner(m_owner.lock());
		m_childs.push_back(_node);
	}

	void TreeNode::remove_child(const s_ptr<TreeNode>& _node)
	{
		auto iter = m_childs.begin();
		auto iter_end = m_childs.end();
		for (iter; iter != iter_end; ++iter)
		{
			if (_node == (*iter))
			{
				m_childs.erase(iter);
				break;
			}
		}
	}


	// Tree
	EditorWidget_Tree::EditorWidget_Tree()
		: EditorWindow(name::EditorWidget_Tree)
	{
	}

	void EditorWidget_Tree::init()
	{
		Super::init();

		m_root = new_entity<TreeNode>();
		m_selected_node;
		m_selected_callback = nullptr;
	}

	EditorWidget_Tree::~EditorWidget_Tree()
	{
	}

	void EditorWidget_Tree::update_UI()
	{
		Super::update_UI();

		m_root->update_UI();
	}

	void EditorWidget_Tree::clear()
	{
		m_root = nullptr;
		m_root = new_entity<TreeNode>();

		m_root->set_owner(std::static_pointer_cast<EditorWidget_Tree>(shared_from_this()));
		m_root->m_b_root = true;
	}
	void EditorWidget_Tree::set_selected_node(const s_ptr<TreeNode>& node)
	{
		s_ptr<TreeNode> selected = m_selected_node.lock();

		if (selected)
		{
			selected->sub_flag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected);
		}

		m_selected_node = node;
		if (node)
		{
			node->add_flag(ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected);

			//caller과 콜백함수 모두 살아있을 경우 함수를 호출한다
			if (m_selected_callback)
			{
				m_selected_callback(node->m_data_ptr);
			}
		}
	}
}
