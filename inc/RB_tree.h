#ifndef RB_TREE_H
#define RB_TREE_H

enum COLOR
{
    BLACK = 0,
    RED
};

template<class K,class V>
class RB_tree_node
{
public:
    RB_tree_node() = default;
    RB_tree_node(K key,V value)
        :m_key(key)
        ,m_value(value)
    {}
    ~RB_tree_node() = default;
    RB_tree_node(RB_tree_node &) = default;
    RB_tree_node(RB_tree_node &&) = default;

public:
    K m_key;
    V m_value;
    COLOR m_color{COLOR::RED};
    RB_tree_node<K,V> * m_parent{nullptr};
    RB_tree_node<K,V> * m_left_node{nullptr};
    RB_tree_node<K,V> * m_right_node{nullptr};
};

template<class Key,class Value>
class RB_tree
{
public:
    RB_tree() = default;
    ~RB_tree() = default;
    RB_tree(RB_tree &) = default;
    RB_tree(RB_tree &&) = default;

    bool insert(Key key,Value value);
    bool rotate_R(RB_tree_node<Key,Value> *& node);
    bool rotate_L(RB_tree_node<Key,Value> *& node);
    void print_node();
    void print_node_(RB_tree_node<Key,Value>* node);

private:
    RB_tree_node<Key,Value> * m_root_node{nullptr};
};

template<class Key,class Value>
bool RB_tree<Key,Value>::insert(Key key,Value value)
{
    if(m_root_node == nullptr)
    {
        m_root_node = new RB_tree_node<Key,Value>(key,value);
        m_root_node->m_color = COLOR::BLACK;
        return true;
    }

    RB_tree_node<Key,Value> * parent = nullptr;
    RB_tree_node<Key,Value> * node = m_root_node;
    while(node)
    {
        if(node->m_key == key)
        {
            return false;
        }

        if(node->m_key > key)
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

    node = new RB_tree_node<Key,Value>(key,value);
    node->m_parent = parent;
    if(parent->m_key > key)
    {
        parent->m_left_node = node;
    }
    else
    {
        parent->m_right_node = node;
    }

    //1.parent为黑，直接插入不用变色
    if(parent->m_color == COLOR::BLACK)
    {
        return true;
    }

    //2.parent为红，叔叔节点为红。parent和叔叔节点变为黑，爷爷节点变红。向上调整(相当于爷爷节点插入一个新节点)
    RB_tree_node<Key,Value> * uncle = nullptr;
    RB_tree_node<Key,Value> * grandpa = parent->m_parent;

    if(grandpa->m_left_node == parent)
    {
        uncle = grandpa->m_right_node;
    }
    else
    {
        uncle = grandpa->m_left_node;
    }

    while(true)
    {
        if(uncle && uncle->m_color == COLOR::RED)
        {
            parent->m_color = COLOR::BLACK;
            uncle->m_color = COLOR::BLACK;
            grandpa->m_color = COLOR::RED;
        }
        else
        {
            //1.parent和node都是左节点，右旋
            if(grandpa->m_left_node == parent && parent->m_left_node == node)
            {
                rotate_R(grandpa);
            }

            //2.parent是左节点，node是右节点，先左旋再右旋
            if(grandpa->m_left_node == parent && parent->m_right_node == node)
            {
                rotate_L(parent);
                rotate_R(grandpa);
            }

            //3.parent和node都是右节点，左旋
            if(grandpa->m_right_node == parent && parent->m_right_node == node)
            {
                rotate_L(grandpa);
            }

            //4.parent是右节点，node是左节点，先右旋再左旋
            if(grandpa->m_right_node == parent && parent->m_left_node == node)
            {
                rotate_R(parent);
                rotate_L(grandpa);
            }
        }

        if(grandpa == m_root_node)
        {
            grandpa->m_color = COLOR::BLACK;
            break;
        }

        node = grandpa;
        parent = grandpa->m_parent;

        if(parent == m_root_node)
        {
            return true;
        }

        grandpa = parent->m_parent;

        if(grandpa->m_left_node == parent)
        {
            uncle = grandpa->m_right_node;
        }
        else
        {
            uncle = grandpa->m_left_node;
        }
    }

}

template<class Key,class Value>
bool RB_tree<Key,Value>::rotate_R(RB_tree_node<Key,Value> *& node)
{
    RB_tree_node<Key,Value> * parent = node->m_parent;
    RB_tree_node<Key,Value> * sub_L = node->m_left_node;
    RB_tree_node<Key,Value> * sub_LR = sub_L->m_right_node;

    sub_L->m_parent = parent;
    sub_L->m_right_node = node;

    if(sub_LR)
    {
        sub_LR->m_parent = node;
    }

    node->m_parent = sub_L;
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

}

template<class Key,class Value>
bool RB_tree<Key,Value>::rotate_L(RB_tree_node<Key,Value> *& node)
{
    RB_tree_node<Key,Value> * parent = node->m_parent;
    RB_tree_node<Key,Value> * sub_R = node->m_right_node;
    RB_tree_node<Key,Value> * sub_RL = sub_R->m_left_node;

    sub_R->m_parent = parent;
    sub_R->m_left_node = node;

    if(sub_RL)
    {
        sub_RL->m_parent = node;
    }

    node->m_parent = sub_R;
    node->m_right_node = sub_RL;

    if(parent == nullptr)
    {
        m_root_node = sub_R;
    }
    else
    {
        if(parent->m_right_node == node)
        {
            parent->m_right_node = sub_R;
        }
        else
        {
            parent->m_left_node = sub_R;
        }
    }

}

template<class Key,class Value>
void RB_tree<Key,Value>::print_node()
{
    print_node_(m_root_node);
    std::cout<<std::endl;
}

template<class Key,class Value>
void RB_tree<Key,Value>::print_node_(RB_tree_node<Key,Value>* node)
{
    if(node == nullptr)
    {
        return;
    }
    print_node_(node->m_left_node);
    std::cout<< node->m_key <<std::endl;
    print_node_(node->m_right_node);
}


#endif