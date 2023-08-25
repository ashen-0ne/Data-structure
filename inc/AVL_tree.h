#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <queue>

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

    bool insert(Key key,Value value);
    int erase(Key key);
    void Rotate_R(AVL_tree_node<Key,Value>*& node);
    void Rotate_L(AVL_tree_node<Key,Value>*& node);
    void Rotate_RL(AVL_tree_node<Key,Value>*& node);
    void Rotate_LR(AVL_tree_node<Key,Value>*& node);
    void print_node();
    void print_node_(AVL_tree_node<Key,Value>* node);
    void level_print();
    void level_print_(std::queue<AVL_tree_node<Key,Value>*>& que);

private:
    AVL_tree_node<Key,Value>* m_root_node{nullptr};
};

template<class Key,class Value>
bool AVL_tree<Key,Value>::insert(Key key,Value value)
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
                    // std::cout<<"key:"<<key<<" L"<<std::endl;
                    Rotate_L(parent);
                }
                else
                {
                    // std::cout<<"key:"<<key<<" RL"<<std::endl;
                    Rotate_RL(parent);
                }
            }
            else if(parent->m_bf == -2)
            {
                if(cur->m_bf == -1)
                {
                    // std::cout<<"key:"<<key<<" R"<<std::endl;
                    Rotate_R(parent);
                }
                else
                {
                    // std::cout<<"key:"<<key<<" LR"<<std::endl;
                    Rotate_LR(parent);
                }
            }
            break;
        }
    }
}

template<class Key,class Value>
int AVL_tree<Key,Value>::erase(Key key)
{
    int erase_count = 0;

    AVL_tree_node<Key,Value> * node = m_root_node;
    AVL_tree_node<Key,Value> * parent = nullptr;

    while(node)
    {
        if(node->m_key == key)
        {
            break;
        }
        else if(node->m_key > key)
        {
            parent = node;
            node = node->m_left_node;
        }
        else
        {
            parent = node;
            node = node->m_right_node;
        }
    }

    if(node == nullptr)
    {
        return erase_count;
    }

    AVL_tree_node<Key,Value> * sub_LP = sub_L->m_parent;
    AVL_tree_node<Key,Value> * sub_L = node->m_left_node;

    while(sub_L)
    {
        if(sub_L->m_right_node)
        {
            sub_L = sub_L->m_right_node;
        }
    }

    if(sub_L == nullptr)
    {
        sub_L = node->m_right_node;
    }

    if(parent == nullptr)
    {
        m_root_node = sub_L;
    }
    else
    {
        if(parent->m_left_node == node)
        {
            parent->m_left_node = sub_L;
        }
        else
        {
            parent->m_right_node = sub_L;
        }
    }

    sub_LP->m_right_node == sub_L->m_left_node;
    if(sub_LP->m_right_node == nullptr)
    {
        sub_LP->m_bf -= 1;
    }
    
    sub_L->m_parent = parent;
    sub_L->m_right_node = node->m_right_node;
    sub_L->m_left_node = node->m_left_node;

}

template<class Key,class Value>
void AVL_tree<Key,Value>::Rotate_R(AVL_tree_node<Key,Value>*& node)
{
    AVL_tree_node<Key,Value> * parent = node->m_parent_node;
    AVL_tree_node<Key,Value> * sub_L = node->m_left_node;
    AVL_tree_node<Key,Value> * sub_LR = sub_L->m_right_node;

    node->m_bf = 0;
    sub_L->m_bf = 0;

    sub_L->m_parent_node = parent;
    sub_L->m_right_node = node;

    if(sub_LR)
    {
        sub_LR->m_parent_node = node;
    }

    node->m_parent_node = sub_L;
    node->m_left_node = sub_LR;

    if(parent == nullptr)
    {
        m_root_node = sub_L;
    }
    else
    {
        if(parent->m_left_node == node)
        {
            parent->m_left_node = sub_L;
        }
        else
        {
            parent->m_right_node = sub_L;
        }
    }

    // node = sub_L;
}

template<class Key,class Value>
void AVL_tree<Key,Value>::Rotate_L(AVL_tree_node<Key,Value>*& node)
{
    AVL_tree_node<Key,Value> * parent = node->m_parent_node;
    AVL_tree_node<Key,Value> * sub_R = node->m_right_node;
    AVL_tree_node<Key,Value> * sub_RL = sub_R->m_left_node;

    node->m_bf = 0;
    sub_R->m_bf = 0;
    
    sub_R->m_parent_node = parent;
    sub_R->m_left_node = node;

    if(sub_RL)
    {
        sub_RL->m_parent_node = node;
    }

    node->m_parent_node = sub_R;
    node->m_right_node = sub_RL;

    if(parent == nullptr)
    {
        m_root_node = sub_R;
    }
    else
    {
        if(parent->m_left_node == node)
        {
            parent->m_left_node = sub_R;
        }
        else
        {
            parent->m_right_node = sub_R;
        }
    }

    // node = sub_R;
}

template<class Key,class Value>
void AVL_tree<Key,Value>::Rotate_RL(AVL_tree_node<Key,Value>*& node)
{
    AVL_tree_node<Key,Value> * parent = node;
    AVL_tree_node<Key,Value> * node_right = node->m_right_node;
    AVL_tree_node<Key,Value> * node_right_left = node_right->m_left_node;
    int bf = node_right_left->m_bf;

    Rotate_R(node->m_right_node);
    Rotate_L(node);

    if(bf == -1)
    {
        parent->m_bf = 0;
        node_right->m_bf = 1;
    }
    else if(bf == 1)
    {
        parent->m_bf = -1;
        node_right->m_bf = 0;
    }
    else
    {
        parent->m_bf = 0;
        node_right->m_bf = 0;
    }
}

template<class Key,class Value>
void AVL_tree<Key,Value>::Rotate_LR(AVL_tree_node<Key,Value>*& node)
{
    AVL_tree_node<Key,Value> * parent = node;
    AVL_tree_node<Key,Value> * node_left = node->m_left_node;
    AVL_tree_node<Key,Value> * node_left_right = node_left->m_right_node;
    int bf = node_left_right->m_bf;

    Rotate_L(node->m_left_node);
    Rotate_R(node);

    if(bf == -1)
    {
        parent->m_bf = 1;
        node_left->m_bf = 0;
    }
    else if(bf == 1)
    {
        parent->m_bf = 0;
        node_left->m_bf = -1;
    }
    else
    {
        parent->m_bf = 0;
        node_left->m_bf = 0;
    }
}

template<class Key,class Value>
void AVL_tree<Key,Value>::print_node()
{
    print_node_(m_root_node);
    std::cout<<std::endl;
}

template<class Key,class Value>
void AVL_tree<Key,Value>::print_node_(AVL_tree_node<Key,Value>* node)
{
    if(node == nullptr)
    {
        return;
    }
    print_node_(node->m_left_node);
    std::cout<< node->m_key << " bf = " << node->m_bf  <<std::endl;
    print_node_(node->m_right_node);
}

template<class Key,class Value>
void AVL_tree<Key,Value>::level_print()
{
    std::queue<AVL_tree_node<Key,Value>*> que;
    que.push(m_root_node);
    while(que.size() > 0)
    {
        level_print_(que);
    }
}

template<class Key,class Value>
void AVL_tree<Key,Value>::level_print_(std::queue<AVL_tree_node<Key,Value>*>& que)
{
    if(que.size() == 0)
    {
        return;
    }
    int size = que.size();
    for(int i = 0;i < size;++i)
    {
        AVL_tree_node<Key,Value>* temp = que.front();
        que.pop();
        std::cout<<temp->m_key<< " ";
        if(temp->m_left_node != nullptr)
        {
            que.push(temp->m_left_node);
        }
        if(temp->m_right_node != nullptr)
        {
            que.push(temp->m_right_node);
        }
    }
    std::cout<<std::endl;
    return;
}

#endif