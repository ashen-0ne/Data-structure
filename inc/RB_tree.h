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
    RB_tree_node<K,V> * m_left{nullptr};
    RB_tree_node<K,V> * m_right{nullptr};
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
    void erase(Key key);
    RB_tree_node<Key,Value> * erase_and_rebalance(RB_tree_node<Key,Value> * node);
    void rotate_R(RB_tree_node<Key,Value> * node);
    void rotate_L(RB_tree_node<Key,Value> * node);
    void print_node();
    void print_node_(RB_tree_node<Key,Value>* node);

    std::map<int,int> get_all_route_black_node_count();
    void recursive_count(const RB_tree_node<Key,Value> * node,int &index,int count,std::map<int,int> & m);
    RB_tree_node<Key,Value> * get_Max_node(const RB_tree_node<Key,Value> * & node);

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
            node = node->m_left;
        }
        else
        {
            parent = node;
            node = node->m_right;
        }
    }

    node = new RB_tree_node<Key,Value>(key,value);
    node->m_parent = parent;
    if(parent->m_key > key)
    {
        parent->m_left = node;
    }
    else
    {
        parent->m_right = node;
    }

    while(node != m_root_node && node->m_parent->m_color == COLOR::RED)
    {
        RB_tree_node<Key,Value> * xpp = node->m_parent->m_parent;
        //父节点为祖父节点的左节点
        if(node->m_parent == xpp->m_left)
        {
            RB_tree_node<Key,Value> * uncle = xpp->m_right;
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
                if(node == node->m_parent->m_right)
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
            RB_tree_node<Key,Value> * uncle = xpp->m_left;
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
                if(node == node->m_parent->m_left)
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
    RB_tree_node<Key,Value> * sub_L = node->m_left;
    RB_tree_node<Key,Value> * sub_LR = sub_L->m_right;

    sub_L->m_parent = parent;
    sub_L->m_right = node;

    if(sub_LR)
    {
        sub_LR->m_parent = node;
    }

    node->m_parent = sub_L;
    node->m_left = sub_LR;

    if(parent == nullptr)
    {
        m_root_node = sub_L;
    }
    else
    {
        if(parent->m_left == node)
        {
            parent->m_left = sub_L;
        }
        else
        {
            parent->m_right = sub_L;
        }
    }

}

template<class Key,class Value>
void RB_tree<Key,Value>::rotate_L(RB_tree_node<Key,Value> * node)
{
    RB_tree_node<Key,Value> * parent = node->m_parent;
    RB_tree_node<Key,Value> * sub_R = node->m_right;
    RB_tree_node<Key,Value> * sub_RL = sub_R->m_left;

    sub_R->m_parent = parent;
    sub_R->m_left = node;

    if(sub_RL)
    {
        sub_RL->m_parent = node;
    }

    node->m_parent = sub_R;
    node->m_right = sub_RL;

    if(parent == nullptr)
    {
        m_root_node = sub_R;
    }
    else
    {
        if(parent->m_right == node)
        {
            parent->m_right = sub_R;
        }
        else
        {
            parent->m_left = sub_R;
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
    print_node_(node->m_left);
    std::cout<< node->m_key <<std::endl;
    print_node_(node->m_right);
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

    recursive_count(node->m_left,index,count,m);
    index++;
    recursive_count(node->m_right,index,count,m);
    return;
}

template<class Key,class Value>
RB_tree_node<Key,Value> * RB_tree<Key,Value>::get_Max_node(const RB_tree_node<Key,Value> * & node)
{
    RB_tree_node<Key,Value> * ret = node;
    while(ret->m_right)
    {
        ret = ret->m_right;
    }
    return ret;
}

template<class Key,class Value>
void RB_tree<Key,Value>::erase(Key key)
{
    RB_tree_node<Key,Value> * node = m_root_node;
    while(node)
    {
        if(node->m_key > key)
        {
            node = node->m_left;
        }
        else if(node->m_key < key)
        {
            node = node->m_right;
        }
        else
        {
            break;
        }
    }
    if(node == nullptr)
    {
        return;
    }
    node = erase_and_rebalance(node);
    delete node;
    return;
}

template<class Key,class Value>
RB_tree_node<Key,Value> * RB_tree<Key,Value>::erase_and_rebalance(RB_tree_node<Key,Value> * node)
{
    RB_tree_node<Key,Value> * y = node;
    RB_tree_node<Key,Value> * x = nullptr;
    RB_tree_node<Key,Value> * x_parent = nullptr;

    //待删除节点没有左子节点，或者左右子节点都没有
    if(node->m_left == nullptr)
    {
        x = node->m_right; //x有可能为空
    }
    else
    {
        //待删除节点只有左子节点
        if(node->m_right == nullptr)
        {
            x = node->m_left;  //x不为空
        }
        //待删除节点左右子节点皆有
        else
        {
            //找到比删除节点大的节点中最小的一个
            y = node->m_right;
            while(y->m_left != nullptr)
            {
                y = y->m_left;
            }
            //x此时为在旋转中可能需要调整父节点的节点
            x = y->m_right;
        }
    }

    //待删除节点有左右子节点
    if(y != node)
    {
        //此时替换节点为y

        //y接管node左节点
        node->m_left->m_parent = y;
        y->m_left = node->m_left;

        //y不为node的右子节点，此时y的右节点需要进行处理
        //1.x为y的右子节点，需替换y成为y父节点的左子节点
        //2.y的右子节点需要变为node的右子节点
        if(y != node->m_right)
        {
            x_parent = y->m_parent;
            if(x != nullptr)
            {
                x->m_parent = y->m_parent;
            }
            y->m_parent->m_left = x;
            y->m_right = node->m_right;
            node->m_right->m_parent = y;
        }
        //y为node右子节点，y的右子节点无需调整
        else
        {
            x_parent = y;
        }

        if(m_root_node == node)
        {
            m_root_node = y;
        }
        else if(node->m_parent->m_left = node)
        {
            node->m_parent->m_left = y;
        }
        else
        {
            node->m_parent->m_right = y;
        }
        y->m_parent = node->m_parent;
        std::swap(node->m_color,y->m_color);
        //y指向待删除节点
        y = node;
    }
    //待删除节点左右子节点不全
    else
    {
        //此时替换节点为x

        x_parent = y->m_parent;
        if(x != nullptr)
        {
            x->m_parent = y->m_parent;
        }
        if(m_root_node == node)
        {
            m_root_node = x;
        }
        else if(node->m_parent->m_left == node)
        {
            node->m_parent->m_left = x;
        }
        else
        {
            node->m_parent->m_right = x;
        }
    }

    //待删除节点有左右子节点时，此颜色为替换节点的颜色
    //为黑意味着替换节点所在的路径上黑节点数量-1，需要调整

    //待删除节点左右节点不全时，此颜色为待删除节点本身颜色。如果为红说明左右节点都为空，无需调整。
    //为黑意味着此子树黑节点数量-1，需要调整
    if(y->m_color == COLOR::BLACK)
    {
        while(x != m_root_node && (x == nullptr || x->m_color == COLOR::BLACK))
        {
            if(x == x_parent->m_left)
            {
                RB_tree_node<Key,Value> * w = x_parent->m_right;
                if(w->m_color == COLOR::RED)
                {
                    w->m_color = COLOR::BLACK;
                    x_parent->m_color = COLOR::RED;
                    rotate_L(x_parent);
                    w = x_parent->m_right;
                }
                if((w->m_left == nullptr || w->m_left->m_color == COLOR::BLACK)
                    &&(w->m_right == nullptr || w->m_right->m_color == COLOR::BLACK))
                {
                    w->m_color = COLOR::RED;
                    x = x_parent;
                    x_parent = x_parent->m_parent;
                }
                else
                {
                    if(w->m_right == nullptr || w->m_right->m_color == COLOR::BLACK)
                    {
                        if(w->m_left)
                        {
                            w->m_left->m_color = COLOR::BLACK;
                        }
                        w->m_color = COLOR::RED;
                        rotate_R(w);
                        w = x_parent->m_right;
                    }
                    w->m_color = x_parent->m_color;
                    x_parent->m_color = COLOR::BLACK;
                    if(w->m_right)
                    {
                        w->m_right->m_color = COLOR::BLACK;
                    }
                    rotate_L(x_parent);
                    break;
                }
            }
            else
            {
                RB_tree_node<Key,Value> * w = x_parent->m_left;
                if(w->m_color == COLOR::RED)
                {
                    w->m_color = COLOR::BLACK;
                    x_parent->m_color = COLOR::RED;
                    rotate_R(x_parent);
                    w = x_parent->m_left;
                }
                if((w->m_right == nullptr || w->m_right->m_color == COLOR::BLACK)
                    && (w->m_left == nullptr || w->m_left->m_color == COLOR::BLACK))
                {
                    w->m_color = COLOR::RED;
                    x = x_parent;
                    x_parent = x_parent->m_parent;
                }
                else
                {
                    if(w->m_left == nullptr || w->m_left->m_color == COLOR::BLACK)
                    {
                        if(w->m_right)
                        {
                            w->m_right->m_color = COLOR::BLACK;
                        }
                        w->m_color = COLOR::RED;
                        rotate_L(w);
                        w = x_parent->m_left;
                    }
                    w->m_color = x_parent->m_color;
                    x_parent->m_color = COLOR::BLACK;
                    if(w->m_left)
                    {
                        w->m_left->m_color = COLOR::BLACK;
                    }
                    rotate_R(x_parent);
                    break;
                }
            }
        }
        if(x != nullptr)
        {
            x->m_color = COLOR::BLACK;
        }
    }
    return y;
}

#endif