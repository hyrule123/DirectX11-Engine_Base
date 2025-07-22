#pragma once
#include <Editor/Base/EditorUIWindow.h>

#include <Engine/Common.h>

namespace core::editor
{
	class HierarchyTreeWidget;
	class TreeNode : public EditorEntity
	{
		CLASS_INFO(TreeNode, EditorEntity);
		friend class HierarchyTreeWidget;

	public:
		TreeNode();

		virtual ~TreeNode();

		void update_UI();

		//내부에서 w_ptr로 저장됨 - 데이터 관리가 필요하면 별도의 공간에 s_ptr로 저장할것
		void set_data_ptr(const s_ptr<void>& _ptr) { m_data_ptr = _ptr; }
		s_ptr<void> get_data_ptr() const { return m_data_ptr.lock(); }

		s_ptr<TreeNode> get_parent() const { return m_parent.lock(); }
		void set_parent(const s_ptr<TreeNode>& _parent) { m_parent = _parent; }

		void add_child(const s_ptr<TreeNode>& _node);
		const std::vector<s_ptr<TreeNode>>& get_childs() { return m_childs; }

		void remove_child(const s_ptr<TreeNode>& _node);

		void set_root(bool _is_root) { m_b_root = _is_root; }
		bool is_root() const { return m_b_root; }

		ImGuiTreeNodeFlags get_flag() const { return m_flag; }
		void set_flag(ImGuiTreeNodeFlags _flag) { m_flag = _flag; }
		void add_flag(ImGuiTreeNodeFlags _flag) { m_flag |= _flag; }
		void sub_flag(ImGuiTreeNodeFlags _flag) { m_flag &= ~_flag; }

		s_ptr<HierarchyTreeWidget> get_owner() const { return m_owner.lock(); }
		void set_owner(const s_ptr<HierarchyTreeWidget>& _owner) { m_owner = _owner; }

	private:
		w_ptr<HierarchyTreeWidget> m_owner;

		//얘 때문에 데이터가 남아있는 것을 방지하기 위해 일단은 w_ptr 사용 중
		w_ptr<void> m_data_ptr;

		w_ptr<TreeNode> m_parent;
		std::vector<s_ptr<TreeNode>> m_childs;

		ImGuiTreeNodeFlags m_flag;

		bool m_b_root;
	};

	class HierarchyTreeWidget : public EditorUIWindow
	{
		CLASS_INFO(HierarchyTreeWidget, EditorUIWindow);
	public:
		HierarchyTreeWidget();
		void init() override;

		virtual ~HierarchyTreeWidget();

		virtual void update_UI() override;

		TreeNode& get_root() const { return *m_root; }

		void clear();
		void set_selected_node(const s_ptr<TreeNode>& node);

		//함수 caller를 w_ptr로 별도 저장 
		void set_selected_callaback_func(const std::function<void(w_ptr<void>)>& func) {
			m_selected_callback = func;
		}

	private:
		s_ptr<TreeNode> m_root;

		w_ptr<TreeNode> m_selected_node;

		std::function<void(w_ptr<void>)> m_selected_callback;
	};
}
