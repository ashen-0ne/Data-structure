#ifndef AVL_TREE_
#define AVL_TREE_

#include <iostream>

template<class K,class V>
class AVL_tree_node
{
public:
    AVL_tree_node() = default;
    ~AVL_tree_node() = default;
    AVL_tree_node(AVL_tree_node&) = default;
    AVL_tree_node(AVL_tree_node&&) = default;
    AVL_tree_node(K key,V value)
        :m_key(key)
        ,m_value(value){}

public:
    K m_key;
    V m_value;
    int m_bf{0};
    AVL_tree_node<K,V>* m_parent_node{nullptr};
    AVL_tree_node<K,V>* m_left_node{nullptr};
    AVL_tree_node<K,V>* m_right_node{nullptr};
};

template<class Key,class Value>
class AVL_tree
{
public:
    AVL_tree() = default;
    AVL_tree(AVL_tree&) = default;
    AVL_tree(AVL_tree&&) = default;
    ~AVL_tree() = default;

    bool insert(Key key,Value value)
    {
        if(m_root_node == nullptr)
        {
            m_root_node = new AVL_tree_node<Key,Value>();
            m_root_node->m_key = key;
            m_root_node->m_value = value;
            return true;
        }

        if(m_root_node->m_key == key)
        {
            return false;
        }

        AVL_tree_node<Key,Value> * parent = nullptr;
        AVL_tree_node<Key,Value> * cur = m_root_node;

        while(cur)
        {
            parent = cur;
            if(cur->m_key == key)
            {
                return false;
            }
            else if(cur->m_key > key)
            {
                cur = cur->m_left_node;
                continue;
            }
            else
            {
                cur = cur->m_right_node;
                continue;
            }
        }

        cur = new AVL_tree_node<Key,Value>(key,value);
        cur->m_parent_node = parent;
        if(parent->m_key > key)
        {
            parent->m_left_node = cur;
        }
        else
        {
            parent->m_right_node = cur;
        }

        while(parent)
        {
            if(parent->m_left_node == cur)
            {
                parent->m_bf--;
            }
            else
            {
                parent->m_bf++;
            }

            if(parent->m_bf == 0)
            {
                break;
            }
            else if(parent->m_bf == -1 || parent->m_bf == 1)
            {
                cur = parent;
                parent = cur->m_parent_node;
            }
            else
            {
                if(parent->m_bf == 2)
                {
                    if(cur->m_bf == 1)
                    {
                        std::cout<<"key:"<<key<<" L"<<std::endl;
                        Rotate_L(parent);
                    }
                    else
                    {
                        std::cout<<"key:"<<key<<" RL"<<std::endl;
                        Rotate_RL(parent);
                    }
                }
                else if(parent->m_bf == -2)
                {
                    if(cur->m_bf == -1)
                    {
                        std::cout<<"key:"<<key<<" R"<<std::endl;
                        Rotate_R(parent);
                    }
                    else
                    {
                        std::cout<<"key:"<<key<<" LR"<<std::endl;
                        Rotate_LR(parent);
                    }
                }
                break;
            }
        }
    }

    void Rotate_R(AVL_tree_node<Key,Value>*& node)
    {
        AVL_tree_node<Key,Value> * parent = node->m_parent_node;
        AVL_tree_node<Key,Value> * temp_left_node = node->m_left_node;

        if(parent != nullptr)
        {
            if(parent->m_left_node == node)
            {
                parent->m_left_node = temp_left_node;
            }
            else
            {
                parent->m_right_node = temp_left_node;
            }
        }
        else
        {
            m_root_node = temp_left_node;
        }

        node->m_bf = 0;
        temp_left_node->m_bf = 0;

        node->m_parent_node = temp_left_node;
        node->m_left_node = temp_left_node->m_right_node;

        temp_left_node->m_parent_node = parent;
        temp_left_node->m_right_node = node;

        node = temp_left_node;
    }

    void Rotate_L(AVL_tree_node<Key,Value>*& node)
    {
        AVL_tree_node<Key,Value> * parent = node->m_parent_node;
        AVL_tree_node<Key,Value> * temp_right_node = node->m_right_node;

        if(parent != nullptr)
        {
            if(parent->m_left_node == node)
            {
                parent->m_left_node = temp_right_node;
            }
            else
            {
                parent->m_right_node = temp_right_node;
            }
        }
        else
        {
            m_root_node = temp_right_node;
        }
        
        
        node->m_bf = 0;
        temp_right_node->m_bf = 0;

        node->m_parent_node = temp_right_node;
        node->m_right_node = temp_right_node->m_left_node;

        temp_right_node->m_parent_node = parent;
        temp_right_node->m_left_node = node;

        node = temp_right_node;
    }

    void Rotate_RL(AVL_tree_node<Key,Value>*& node)
    {
        AVL_tree_node<Key,Value> * node_right = node->m_right_node;
        AVL_tree_node<Key,Value> * node_right_left = node_right->m_left_node;
        int bf = node_right_left->m_bf;

        Rotate_R(node->m_right_node);
        Rotate_L(node);

        if(bf == -1)
        {
            node->m_bf = 1;
            node_right->m_bf = 0;
        }
        else if(bf == 1)
        {
            node->m_bf = 0;
            node_right->m_bf = -1;
        }
        else
        {
            node->m_bf = 0;
            node_right->m_bf = 0;
        }
    }

    void Rotate_LR(AVL_tree_node<Key,Value>*& node)
    {
        AVL_tree_node<Key,Value> * node_left = node->m_left_node;
        AVL_tree_node<Key,Value> * node_left_right = node_left->m_right_node;
        int bf = node_left_right->m_bf;

        Rotate_L(node->m_left_node);
        Rotate_R(node);

        if(bf == -1)
        {
            node->m_bf = 1;
            node_left->m_bf = 0;
        }
        else if(bf == 1)
        {
            node->m_bf = 0;
            node_left->m_bf = -1;
        }
        else
        {
            node->m_bf = 0;
            node_left->m_bf = 0;
        }
    }

    void print_node()
    {
        print_node_(m_root_node);
    }

    void print_node_(AVL_tree_node<Key,Value>* node)
    {
        if(node == nullptr)
        {
            return;
        }
        print_node_(node->m_left_node);
        std::cout<< node->m_key << " bf = " << node->m_bf  <<std::endl;
        print_node_(node->m_right_node);
    }

private:
    AVL_tree_node<Key,Value>* m_root_node{nullptr};
};

#endif