#ifndef RB_TREE_H
#define RB_TREE_H

#include <map>

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

    void insert(Key key,Value value);
    int erase(Key key);
    void rotate_R(RB_tree_node<Key,Value> * node);
    void rotate_L(RB_tree_node<Key,Value> * node);
    void print_node();
    void print_node_(RB_tree_node<Key,Value>* node);

    std::map<int,int> get_all_route_black_node_count();
    void recursive_count(const RB_tree_node<Key,Value> * node,int &index,int count,std::map<int,int> & m);

public:
    RB_tree_node<Key,Value> * m_root_node{nullptr};
};

template<class Key,class Value>
void RB_tree<Key,Value>::insert(Key key,Value value)
{
    if(m_root_node == nullptr)
    {
        m_root_node = new RB_tree_node<Key,Value>(key,value);
        m_root_node->m_color = COLOR::BLACK;
        return;
    }

    RB_tree_node<Key,Value> * parent = nullptr;
    RB_tree_node<Key,Value> * node = m_root_node;
    while(node)
    {
        if(node->m_key == key)
        {
            node->m_value = value;
            return;
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

    while(node != m_root_node && node->m_parent->m_color == COLOR::RED)
    {
        RB_tree_node<Key,Value> * xpp = node->m_parent->m_parent;
        //父节点为祖父节点的左节点
        if(node->m_parent == xpp->m_left_node)
        {
            RB_tree_node<Key,Value> * uncle = xpp->m_right_node;
            //情况1，如果存在叔节点，并且叔节点颜色为红色
            if(uncle && uncle->m_color == COLOR::RED)
            {
                //调整父节点和叔节点颜色为黑，祖父节点为红。祖父节点子树路径上黑色节点数量不变
                //当前节点跳至祖父节点，开始下一轮调整
                uncle->m_color = COLOR::BLACK;
                node->m_parent->m_color = COLOR::BLACK;
                xpp->m_color = COLOR::RED;
                node = xpp;
            }
            //1.此节点为插入节点，此时叔节点不存在
            //2.此节点为上一轮变色的祖父节点，此时叔节点为黑色
            //此两种情况使用同样方法处理
            else
            {
                //情况3，如果当前节点为父节点的右节点，对父节点进行一次左旋，调整为情况2
                if(node == node->m_parent->m_right_node)
                {
                    node = node->m_parent;
                    rotate_L(node);
                }
                //情况2，如果当前节点为父节点的左节点，父节点也是祖父节点的左节点，对祖父节点进行一次右旋
                //原祖父节点为黑，现父节点替换祖父节点的位置，变为黑，无需继续向上调整
                rotate_R(xpp);
                node->m_parent->m_color = COLOR::BLACK;
                xpp->m_color = COLOR::RED;
            }
        }
        //父节点为祖父节点的右节点
        else
        {
            RB_tree_node<Key,Value> * uncle = xpp->m_left_node;
            //情况4.如果存在叔节点，并且叔节点颜色为红色
            if(uncle && uncle->m_color == COLOR::RED)
            {
                //调整父节点和叔节点颜色为黑，祖父节点为红。祖父节点子树路径上黑色节点数量不变
                //当前节点跳至祖父节点，开始下一轮调整
                uncle->m_color = COLOR::BLACK;
                node->m_parent->m_color = COLOR::BLACK;
                xpp->m_color = COLOR::RED;
                node = xpp;
            }
            //1.此节点为插入节点，此时叔节点不存在
            //2.此节点为上一轮变色的祖父节点，此时叔节点为黑色
            else
            {
                //情况6，如果当前节点为父节点的左节点，对父节点进行一次右旋，调整为情况5
                if(node == node->m_parent->m_left_node)
                {
                    node = node->m_parent;
                    rotate_R(node);
                }
                //情况5，如果当前节点为父节点的右节点，父节点也是祖父节点的右节点，对祖父节点进行一次左旋
                //原祖父节点为黑，现父节点替换祖父节点的位置，变为黑，无需继续向上调整
                rotate_L(xpp);
                node->m_parent->m_color = COLOR::BLACK;
                xpp->m_color = COLOR::RED;
            }
        }
    }
    m_root_node->m_color = COLOR::BLACK;
}


template<class Key,class Value>
void RB_tree<Key,Value>::rotate_R(RB_tree_node<Key,Value> * node)
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
void RB_tree<Key,Value>::rotate_L(RB_tree_node<Key,Value> * node)
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

template<class Key,class Value>
std::map<int,int> RB_tree<Key,Value>::get_all_route_black_node_count()
{
    std::map<int,int> m;
    int index = 0;
    recursive_count(m_root_node,index,0,m);
    return m;
}

template<class Key,class Value>
void RB_tree<Key,Value>::recursive_count(const RB_tree_node<Key,Value> * node,int& index,int count,std::map<int,int> & m)
{
    if(node == nullptr)
    {
        m[index] = count + 1;
        return;
    }

    if(node->m_color == COLOR::BLACK)
    {
        count++;
    }

    recursive_count(node->m_left_node,index,count,m);
    index++;
    recursive_count(node->m_right_node,index,count,m);
    return;
}

template<class Key,class Value>
int RB_tree<Key,Value>::erase(Key key)
{
    RB_tree_node<Key,Value> * node = m_root_node;
    RB_tree_node<Key,Value> * parent = nullptr;

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

    RB_tree_node<Key,Value> * sub_L = node->m_left_node;
    RB_tree_node<Key,Value> * sub_LP = nullptr;

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
        RB_tree_node<Key,Value> * sub_R = node->m_right_node;
        if(parent == nullptr)
        {
            m_root_node = sub_R;
            m_root_node->m_color = COLOR::BLACK;
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
        RB_tree_node<Key,Value> * sub_R = node->m_right_node;
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

    

    delete node;
    return 1;
}
#endif