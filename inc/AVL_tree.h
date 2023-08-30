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
    AVL_tree_node(const K& key,const V& value)
        :m_key(key)
        ,m_value(value){}

public:
    K m_key;
    V m_value;
    int m_bf{0};
    int height{1};
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

    void insert(const Key& key,const Value& value);
    AVL_tree_node<Key,Value> * insert_recursive(AVL_tree_node<Key,Value> * node,const Key& key,const Value& value);
    int erase(Key key);
    AVL_tree_node<Key,Value>* Rotate_R(AVL_tree_node<Key,Value>* node);
    AVL_tree_node<Key,Value>* Rotate_L(AVL_tree_node<Key,Value>* node);
    void print_node();
    void print_node_(AVL_tree_node<Key,Value>* node);
    void level_print();
    void level_print_(std::queue<AVL_tree_node<Key,Value>*>& que);

    int get_height(AVL_tree_node<Key,Value> * node);
    void update_height(AVL_tree_node<Key,Value> * node);
    void update_balance_factor(AVL_tree_node<Key,Value> * node);

private:
    AVL_tree_node<Key,Value>* m_root_node{nullptr};
};

template<class Key,class Value>
void AVL_tree<Key,Value>::insert(const Key& key,const Value& value)
{
    m_root_node = insert_recursive(m_root_node,key,value);
}

template<class Key,class Value>
AVL_tree_node<Key,Value> * AVL_tree<Key,Value>::insert_recursive(AVL_tree_node<Key,Value> * node,const Key& key,const Value& value)
{
    //如果节点为nullptr，创建并返回
    if(node == nullptr)
    {
        return new AVL_tree_node<Key,Value>(key,value);
    }
    
    if(node->m_key > key)
    {
        node->m_left_node = insert_recursive(node->m_left_node,key,value);
        node->m_left_node->m_parent_node = node;
    }
    else if(node->m_key < key)
    {
        node->m_right_node = insert_recursive(node->m_right_node,key,value);
        node->m_right_node->m_parent_node = node;
    }
    else
    {
        node->m_value = value;
        return node;
    }

    update_height(node);
    update_balance_factor(node);

    if(node->m_bf < -1)
    {
        if(key < node->m_left_node->m_key)
        {
            node = Rotate_R(node);
        }
        else
        {
            node->m_left_node = Rotate_L(node->m_left_node);
            node = Rotate_R(node);
        }
    }
    
    if(node->m_bf > 1)
    {
        if(key > node->m_right_node->m_key)
        {
            node = Rotate_L(node);
        }
        else
        {
            node->m_right_node = Rotate_R(node->m_right_node);
            node = Rotate_L(node);
        }
    }

    return node;
}

template<class Key,class Value>
int AVL_tree<Key,Value>::erase(Key key)
{
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
        return 0;
    }

    AVL_tree_node<Key,Value> * sub_L = node->m_left_node;
    AVL_tree_node<Key,Value> * sub_LP = nullptr;

    while(sub_L)
    {
        if(sub_L->m_right_node)
        {
            sub_LP = sub_L;
            sub_L = sub_L->m_right_node;
        }
        else
        {
            break;
        }
    }

    //节点没有左子树，右节点代替节点。包括只有root节点的情况
    if(sub_L == nullptr)
    {
        AVL_tree_node<Key,Value> * sub_R = node->m_right_node;
        if(parent == nullptr)
        {
            m_root_node = sub_R;
        }
        else if(parent->m_left_node == node)
        {
            parent->m_left_node = sub_R;
        }
        else
        {
            parent->m_right_node = sub_R;
        }
        if(sub_R)
        {
            sub_R->m_parent_node = parent;
        }
        delete node;
        return 1;
    }

    //节点的左子树没有右节点，左节点代替节点
    if(sub_LP == nullptr)
    {
        AVL_tree_node<Key,Value> * sub_R = node->m_right_node;
        if(parent == nullptr)
        {
            m_root_node = sub_L;
        }
        else if(parent->m_left_node == node)
        {
            parent->m_left_node = sub_L;
        }
        else
        {
            parent->m_right_node = sub_L;
        }

        sub_L->m_parent_node = parent;
        sub_L->m_right_node = sub_R;

        if(sub_R)
        {
            sub_R->m_parent_node = sub_L;
        }

        sub_L->m_bf = 0;
        if(sub_L->m_left_node)
        {
            sub_L->m_bf -= 1;
        }
        if(sub_R)
        {
            sub_L->m_bf += 1;
        }

        delete node;
        return 1;
    }

    if(parent == nullptr)
    {
        m_root_node = sub_L;
    }
    else if(parent->m_left_node == node)
    {
        parent->m_left_node = sub_L;
    }
    else
    {
        parent->m_right_node = sub_L;
    }

    sub_LP->m_right_node = sub_L->m_left_node;
    
    if(sub_LP->m_right_node == nullptr)
    {
        sub_LP->m_bf -= 1;
    }
    
    sub_L->m_parent_node = parent;
    sub_L->m_right_node = node->m_right_node;
    sub_L->m_left_node = node->m_left_node;
    
    sub_L->m_bf = node->m_bf;
    if(sub_LP->m_bf == -2)
    {
        if(sub_LP->m_left_node->m_bf == 1)
        {
            Rotate_LR(sub_LP);
        }
        else
        {
            Rotate_R(sub_LP);
        }
    }

    delete node;
    return 1;
}

template<class Key,class Value>
AVL_tree_node<Key,Value>* AVL_tree<Key,Value>::Rotate_R(AVL_tree_node<Key,Value>* node)
{
    AVL_tree_node<Key,Value> * parent = node->m_parent_node;
    AVL_tree_node<Key,Value> * sub_L = node->m_left_node;
    AVL_tree_node<Key,Value> * sub_LR = sub_L->m_right_node;

    sub_L->m_parent_node = parent;
    sub_L->m_right_node = node;

    if(sub_LR)
    {
        sub_LR->m_parent_node = node;
    }

    node->m_parent_node = sub_L;
    node->m_left_node = sub_LR;

    if(parent)
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

    update_height(node);
    update_height(sub_L);

    update_balance_factor(sub_L);
    update_balance_factor(node);

    return sub_L;
}

template<class Key,class Value>
AVL_tree_node<Key,Value>* AVL_tree<Key,Value>::Rotate_L(AVL_tree_node<Key,Value>* node)
{
    AVL_tree_node<Key,Value> * parent = node->m_parent_node;
    AVL_tree_node<Key,Value> * sub_R = node->m_right_node;
    AVL_tree_node<Key,Value> * sub_RL = sub_R->m_left_node;
    
    sub_R->m_parent_node = parent;
    sub_R->m_left_node = node;

    if(sub_RL)
    {
        sub_RL->m_parent_node = node;
    }

    node->m_parent_node = sub_R;
    node->m_right_node = sub_RL;

    if(parent)
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

    update_height(node);
    update_height(sub_R);

    update_balance_factor(sub_R);
    update_balance_factor(node);

    return sub_R;
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
    std::cout<< node->m_key << " height = " << node->height  <<std::endl;
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

template<class Key,class Value>
int AVL_tree<Key,Value>::get_height(AVL_tree_node<Key,Value> * node)
{
    if(node == nullptr)
    {
        return 0;
    }

    return node->height;
}

template<class Key,class Value>
void AVL_tree<Key,Value>::update_height(AVL_tree_node<Key,Value> * node)
{
    if(node == nullptr)
    {
        return;
    }
    node->height = 1 + std::max(get_height(node->m_left_node),get_height(node->m_right_node));
}

template<class Key,class Value>
void AVL_tree<Key,Value>::update_balance_factor(AVL_tree_node<Key,Value> * node)
{
    if(node == nullptr)
    {
        return;
    }
    node->m_bf = get_height(node->m_right_node) - get_height(node->m_left_node);
}

#endif