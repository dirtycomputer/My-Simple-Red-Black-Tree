#pragma once

#include <iostream>
#define DEBUG

enum RbColor
{
	RB_COLOR_RED,
	RB_COLOR_BLACK
};

#ifdef DEBUG

template<typename T>
T debug_max(const T& a, const T& b)
{
	return a > b ? a : b;
}

#endif

template<typename T>
void rb_swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}


template<typename T>
class RBtree
{
public:
	typedef T data_type;
	typedef data_type& reference;
	typedef data_type& pointer;
	typedef const data_type& const_reference;
	typedef size_t size_type;

	class RBtreeNode
	{
	public:
		typedef RBtreeNode* RBtreeNodePointer;

		RBtreeNode()
		{
			value = data_type();
			parent_node = nullptr;
			left_node = nullptr;
			right_node = nullptr;
			size = 1;
			rb_color = RB_COLOR_BLACK;
		}

		explicit RBtreeNode(const_reference value_) : value(value_)
		{
			parent_node = nullptr;
			left_node = nullptr;
			right_node = nullptr;
			size = 1;
			rb_color = RB_COLOR_BLACK;
		}

		void switch_color()
		{
			if (rb_color == RB_COLOR_BLACK)
				rb_color = RB_COLOR_RED;
			else 
				rb_color = RB_COLOR_BLACK;
		}

		void size_update()
		{
			size = left_node->size + right_node->size + 1;
		}

		void set_nil(RBtreeNodePointer nil)
		{
			rb_color = RB_COLOR_BLACK;
			size = 0;
			parent_node = nil;
			left_node = nil;
			right_node = nil;
		}

		size_type size;
		data_type value;
		enum RbColor rb_color;

		RBtreeNodePointer parent_node;
		RBtreeNodePointer left_node;
		RBtreeNodePointer right_node;
	};
	typedef  RBtreeNode* RBtreeNodePointer;
	class Iterator
	{
	public:
		Iterator()
		{
			pointer = nullptr;
		}
		Iterator(const Iterator& other)
		{
			pointer = other.pointer;
		}
		// ReSharper disable CppNonExplicitConvertingConstructor
		Iterator(RBtreeNodePointer pointer_) :pointer(pointer_) {}
		// ReSharper restore CppNonExplicitConvertingConstructor

		RBtreeNodePointer pointer;

		data_type operator * ()
		{
			return pointer == nullptr ? data_type() : pointer->value;
		}

		RBtreeNodePointer operator -> ()
		{
			return pointer;
		}

		RBtreeNodePointer ptr() const
		{
			return pointer;
		}

		bool check_nil(RBtreeNodePointer check_node = pointer)
		{
			return check_node->size == 0;
		}

		Iterator& operator ++ () // log(n)
		{
			if (check_nil(pointer))
			{
				pointer = nullptr;
				return *this;
			}
			RBtreeNodePointer succeed_node = pointer;
			RBtreeNodePointer parent_node = pointer->parent_node;
			RBtreeNodePointer right_node = pointer->right_node;
			bool flag = false;
			
			if(!check_nil(right_node))
			{
				RBtreeNodePointer temp = right_node;
				while (!check_nil(temp->left_node))
				{
					temp = temp->left_node;
				}
				flag = true;
				succeed_node = temp;
			}
			else
			{
				RBtreeNodePointer temp = pointer;
				while ((!check_nil(parent_node)) && (parent_node->right_node == temp))
				{
					temp = parent_node;
					parent_node = parent_node->parent_node;
				}
				if (!check_nil(parent_node) && (parent_node->left_node == temp))
				{
					if (!flag || flag && temp->value < succeed_node->value)
					{
						succeed_node = parent_node;
						flag = true;
					}
				}
			}
			if (!flag)
			{
				succeed_node = nullptr;
			}
			pointer = succeed_node;
			return *this;
		}

		Iterator& operator -- () 
		{
			if (check_nil(pointer))
			{
				pointer = nullptr;
				return *this;
			}
			RBtreeNodePointer succeed_node = pointer;
			RBtreeNodePointer parent_node = pointer->parent_node;
			RBtreeNodePointer left_node = pointer->left_node;
			bool flag = false;

			if (!check_nil(left_node))
			{
				RBtreeNodePointer temp = left_node;
				while (!check_nil(temp->right_node))
				{
					temp = temp->right_node;
				}
				flag = true;
				succeed_node = temp;
			}
			else
			{
				RBtreeNodePointer temp = pointer;
				while ((!check_nil(parent_node)) && (parent_node->left_node == temp))
				{
					temp = parent_node;
					parent_node = parent_node->parent_node;
				}
				if (!check_nil(parent_node) && (parent_node->right_node == temp))
				{
					if (!flag || flag && temp->value < succeed_node->value)
					{
						succeed_node = parent_node;
						flag = true;
					}
				}
			}
			if (!flag)
			{
				succeed_node = nullptr;
			}
			pointer = succeed_node;
			return *this;
		}

		Iterator operator ++ (int)
		{
			auto copy = pointer;
			this->operator++();
			return Iterator(copy);
		}

		Iterator operator -- (int)
		{
			auto copy = pointer;
			this->operator--();
			return Iterator(copy);
		}

		bool operator == (const Iterator& another_iterator) const
		{
			return pointer == another_iterator.ptr();
		}

		bool operator != (const Iterator& another_iterator) const
		{
			return !(pointer == another_iterator.ptr());
		}

	};

	RBtree()
	{
		nil = new RBtreeNode;
		nil->set_nil(nil);
		root_node = nil;
	}

	size_type size() const
	{
		return root_node->size;
	}
	
	Iterator insert(const_reference insert_value)
	{
		RBtreeNodePointer insert_node = create_new_node(insert_value);
		//insert_rbtree_node(insert_node);
		if(this->size() == 0)
		{
			root_node = insert_node;
			root_node->rb_color = RB_COLOR_BLACK;
			root_node->parent_node = nil;
			root_node->left_node = nil;
			root_node->right_node = nil;
			return Iterator(insert_node);
		}
		insert_rbtree_node(root_node, insert_node, nil);
		rb_insert_fixup(insert_node);
		return Iterator(insert_node);
	}
	Iterator find(const_reference search_value) const
	{
		RBtreeNodePointer result = search_for(root_node, search_value);
		if (result == nil)
		{
			return nullptr;
		}
		return result;
	}

	bool remove(const_reference delete_value)
	{
		RBtreeNodePointer delete_node = find(delete_value).ptr();
		if (delete_node == nullptr)
		{
			return false;
		}
		if (size() == 1)
		{
			delete delete_node;
			root_node = nil;
			return true;
		}
		delete_node->size -= 1;
		rb_size_fixup(delete_node);
		delete_rbtree_node(delete_node);
		return true;
	}

	void clear()
	{
		rb_delete_all(root_node);
		root_node = nil;
	}

	bool erase(Iterator erase_pos)
	{
		RBtreeNodePointer delete_node = erase_pos.ptr();
		if (delete_node == nullptr)
		{
			return false;
		}
		if (size() == 1)
		{
			delete delete_node;
			root_node = nil;
			return true;
		}
		delete_node->size -= 1;
		rb_size_fixup(delete_node);
		delete_rbtree_node(delete_node);
		return true;
	}
	bool empty() const
	{
		return root_node->size == 0;
	}
	bool has(const_reference check_value) const
	{
		Iterator temp = find(check_value);
		if (temp.ptr() == nullptr)
		{
			return false;
		}
		return true;
	}
	Iterator begin() const
	{
		RBtreeNodePointer current_node = root_node;
		while (current_node->left_node != nil)
		{
			current_node = current_node->left_node;
		}
		return Iterator(current_node);
	}
	Iterator end() const
	{
		return Iterator(nullptr);
	}

#ifdef DEBUG
#include <cstdio>

	void debug_dot_generate_node(RBtreeNodePointer current_node, FILE* fp) const
	{
		if (current_node == nil)
		{
			//fprintf(fp, "node[shape=point,style=filled,color=black];\n");
			return;
		}
		if (current_node->rb_color == RB_COLOR_BLACK)
		{
			fprintf(fp, "node[shape=circle,style=filled,color=black,fontcolor=white];\n");
		} 
		else
		{
			fprintf(fp, "node[shape=circle,style=filled,color=red,fontcolor=white];\n");
		}
		fprintf(fp, "%d[label=\" %d \"];\n", current_node->value, current_node->value);		
	}

	void debug_dot_generate_tree(RBtreeNodePointer current_node, FILE* fp) const
	{
		if (current_node == nil)
		{
			return;
		}
		if (current_node->parent_node == nil)
		{
			debug_dot_generate_node(current_node, fp);
		}
		if (current_node->left_node != nil)
		{
			debug_dot_generate_node(current_node->left_node, fp);
			fprintf(fp, "%d->%d;\n", current_node->value, current_node->left_node->value);
		} 
		if (current_node->right_node != nil)
		{
			debug_dot_generate_node(current_node->right_node, fp);
			fprintf(fp, "%d->%d;\n", current_node->value, current_node->right_node->value);
		}
		debug_dot_generate_tree(current_node->left_node, fp);
		debug_dot_generate_tree(current_node->right_node, fp);

	}

	void debug_dot(FILE* fp) const
	{
		fprintf(fp, "digraph G{\n");
		debug_dot_generate_tree(root_node, fp);
		fprintf(fp, "}");
	}

	void debug_run() const
	{
		if (size() == 0)
		{
			std::cout << "It is empty!" << std::endl;
			return;
		}
		debug_visit(root_node);
		std::cout << std::endl;
	}

	void debug_visit(RBtreeNodePointer current_node) const
	{
		if (current_node == nil || current_node == nullptr)
		{
			return;
		}
		 std::cout << '(';
		debug_visit(current_node->left_node);		
		std::cout << current_node->value << ' ' << current_node->rb_color;
		debug_visit(current_node->right_node);
		 std::cout << ')';
	}

	int debug_get_depth()
	{
		return debug_search(root_node);
	}

	int debug_search(RBtreeNodePointer current_node)
	{
		if (current_node == nil)
			return 0;
		return debug_max(debug_search(current_node->left_node), debug_search(current_node->right_node)) + 1;
	}

	int debug_get_black_depth()
	{
		return debug_search_black(root_node);
	}

	int debug_search_black(RBtreeNodePointer current_node)
	{
		if (current_node == nil)
		{
			return 0;
		}
		int left_black_depth = debug_search_black(current_node->left_node);
		int right_black_depth = debug_search_black(current_node->right_node);
		if (left_black_depth != right_black_depth)
			return -100000000;
		return left_black_depth + int(current_node->rb_color);
	}



#endif // DEBUG

private:
	
	RBtreeNodePointer root_node;
	RBtreeNodePointer nil;

	RBtreeNodePointer search_for(RBtreeNodePointer current_node, const_reference value) const
	{
		if (current_node == nil)
		{
			return nil;
		}
		if (value < current_node->value)
			return search_for(current_node->left_node, value);
		if (current_node->value < value)
			return search_for(current_node->right_node, value);
		return current_node;
	}
	RBtreeNodePointer create_new_node(const_reference data_value) const
	{
		RBtreeNodePointer temp_pointer = new RBtreeNode(data_value);
		temp_pointer->left_node = nil;
		temp_pointer->right_node = nil;
		temp_pointer->parent_node =	nil;
		temp_pointer->rb_color = RB_COLOR_RED;
		return temp_pointer;
	}
	void rotate(RBtreeNodePointer current_node)
	{
		if (current_node == nil || current_node == root_node)
		{
			return;
		}

		RBtreeNodePointer parent_node = current_node->parent_node;
		RBtreeNodePointer grandparent_node = parent_node->parent_node;
		if(parent_node->left_node == current_node)
		{
			parent_node->left_node = current_node->right_node;
			current_node->right_node->parent_node = parent_node;
			current_node->right_node = parent_node;
		}
		else if (parent_node->right_node == current_node)
		{
			parent_node->right_node = current_node->left_node;
			current_node->left_node->parent_node = parent_node;
			current_node->left_node = parent_node;
		}
		parent_node->parent_node = current_node;

		if (grandparent_node->left_node == parent_node)
			grandparent_node->left_node = current_node;
		else if (grandparent_node->right_node == parent_node)
			grandparent_node->right_node = current_node;
		
		current_node->parent_node = grandparent_node;
		
		if (parent_node == root_node)
		{
			root_node = current_node; 
		}

		parent_node->size_update();
		current_node->size_update();
	}
	void insert_rbtree_node(RBtreeNodePointer& current_node, 
		RBtreeNodePointer insert_node,
		RBtreeNodePointer parent_node)
	{
		if(current_node == nil)
		{
			current_node = insert_node;
			insert_node->parent_node = parent_node;
			return;
		}
		if (current_node->value < insert_node->value)
		{
			insert_rbtree_node(current_node->right_node, insert_node, current_node);
		}
		else if (insert_node->value < current_node->value)
		{
			insert_rbtree_node(current_node->left_node, insert_node, current_node);
		}
		current_node->size_update();
	}
	void delete_rbtree_node(RBtreeNodePointer delete_node)
	{
		RBtreeNodePointer left_child = delete_node->left_node;
		RBtreeNodePointer right_child = delete_node->right_node;
		RBtreeNodePointer parent_node = delete_node->parent_node;

		if ((left_child == nil) && (right_child == nil))
		{
			if (parent_node->left_node == delete_node)
				parent_node->left_node = nil;
			else
				parent_node->right_node = nil;
			if (delete_node->rb_color == RB_COLOR_RED)
			{
				delete delete_node;
				return;
			}
			delete delete_node;
			rb_delete_fixup(nil, parent_node);
			if (root_node == delete_node)
			{
				root_node = nil;
			}
			return;
		}
		if (left_child == nil)
		{
			right_child->parent_node = parent_node;
			if (parent_node->left_node == delete_node)
				parent_node->left_node = right_child;
			else
				parent_node->right_node = right_child;
			delete delete_node;
			right_child->switch_color();
			if (delete_node == root_node)
			{
				root_node = right_child;
				right_child->rb_color = RB_COLOR_BLACK;
			}
			return;
		} 
		if (right_child == nil)
		{
			left_child->parent_node = parent_node;
			if (parent_node->left_node == delete_node)
				parent_node->left_node = left_child;
			else
				parent_node->right_node = left_child;
			delete delete_node;
			left_child->switch_color();
			if (delete_node == root_node)
			{
				root_node = left_child;
				left_child->rb_color = RB_COLOR_BLACK;
			}
		}
		else
		{
			Iterator succeed_node = Iterator(delete_node);
			++succeed_node;
			rb_swap(delete_node->value, succeed_node->value);			
			delete_rbtree_node(succeed_node.ptr());
		}
	}
	void rb_size_fixup(RBtreeNodePointer current_node)
	{
		if (current_node == nil)
		{
			return;
		}
		while (current_node->parent_node != nil)
		{
			current_node->parent_node->size_update();
			current_node = current_node->parent_node;
		}
	}
	void rb_delete_fixup(RBtreeNodePointer current_node, RBtreeNodePointer parent_node)
	{
		if (current_node == root_node)
		{
			return;
		}
		RBtreeNodePointer brother_node =
			parent_node->left_node == current_node ? 
			parent_node->right_node : parent_node->left_node;
		if (brother_node->rb_color == RB_COLOR_RED)
		{
			rotate(brother_node);
			brother_node->switch_color();
			parent_node->switch_color();
			rb_delete_fixup(current_node, parent_node);
		}
		else if (brother_node->left_node->rb_color == RB_COLOR_BLACK &&
			brother_node->right_node->rb_color == RB_COLOR_BLACK)
		{
			if (parent_node->rb_color == RB_COLOR_RED)
			{
				parent_node->switch_color();
				brother_node->switch_color();
			}
			else
			{
				brother_node->switch_color();
				rb_delete_fixup(parent_node, parent_node->parent_node);
			}
		}
		else if (parent_node->left_node == current_node)
		{
			if (brother_node->left_node->rb_color == RB_COLOR_RED &&
				brother_node->right_node->rb_color == RB_COLOR_BLACK)
			{
				brother_node->switch_color();
				brother_node->left_node->switch_color();
				rotate(brother_node->left_node);
				rb_delete_fixup(current_node, parent_node);
			}
			else
			{
				RbColor parent_node_color = parent_node->rb_color;
				brother_node->rb_color = parent_node_color;
				parent_node->rb_color = RB_COLOR_BLACK;
				brother_node->right_node->switch_color();
				rotate(brother_node);
			}
		}
		else
		{
			if (brother_node->right_node->rb_color == RB_COLOR_RED &&
				brother_node->left_node->rb_color == RB_COLOR_BLACK)
			{
				brother_node->switch_color();
				brother_node->right_node->switch_color();
				rotate(brother_node->right_node);
				rb_delete_fixup(current_node, parent_node);
			}
			else
			{
				RbColor parent_node_color = parent_node->rb_color;
				brother_node->rb_color = parent_node_color;
				parent_node->rb_color = RB_COLOR_BLACK;
				brother_node->left_node->switch_color();
				rotate(brother_node);
			}
		}
	}
	void rb_insert_fixup(RBtreeNodePointer current_node)
	{
		if (current_node == nil)
		{
			return;
		}
		RBtreeNodePointer parent_node = current_node->parent_node;
		RBtreeNodePointer grandparent_node = parent_node->parent_node;
		RBtreeNodePointer uncle_node =
			(grandparent_node->left_node == parent_node) ?
			grandparent_node->right_node : grandparent_node->left_node;
		if (parent_node->rb_color == RB_COLOR_BLACK)
		{
			return;
		}

		bool current_flag = parent_node->left_node == current_node;
		bool parent_flag = grandparent_node->left_node == parent_node;

		if (uncle_node->rb_color == RB_COLOR_BLACK)
		{
			if (parent_flag == current_flag)
			{
				parent_node->switch_color();
				grandparent_node->switch_color();
				rotate(parent_node);
			}
			else
			{
				rotate(current_node);
				rb_insert_fixup(current_node);
			}
		}
		else
		{
			parent_node->switch_color();
			uncle_node->switch_color();
			if (root_node != grandparent_node)
			{
				grandparent_node->switch_color();
			}
			rb_insert_fixup(grandparent_node);
		}
	}
	
	void rb_delete_all(RBtreeNodePointer current_node)
	{
		if (current_node == nil)
		{
			return;
		}
		rb_delete_all(current_node->left_node);
		rb_delete_all(current_node->right_node);
		delete current_node;
	}

};