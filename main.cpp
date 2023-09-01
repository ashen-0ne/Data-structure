#include "AVL_tree.h"
#include "RB_tree.h"
#include <map>

int main(int argc,char ** argv)
{
    srand(time(0));
    // AVL_tree<int,std::string> tree;

    // for(int i = 20;i < 10000;i += 10)
    // {
    //     tree.insert(rand()%1000000,std::to_string(rand()%1000000));
    // }

    // for(int i = 20;i < 1000;i += 1)
    // {
    //     tree.insert(i,std::to_string(i));
    // }
    
    // for(int i = 50;i < 500;i += 3)
    // {
    //     tree.erase(i);
    // }

    // tree.print_node();

    // std::map<int,int> m;

    // for(int i = 0;i < 10000;++i)
    // {
    //     m[i] = i;
    // }

    // m.insert(m.begin(),std::make_pair<int,int>(114514,114514));

    // std::cout<<m.begin()->second<<std::endl;

    // for(auto i = m.begin();i != m.end();++i)
    // {
    //     std::cout<<i->second<<std::endl;
    // }

    // std::cout<<m.begin()->second<<std::endl;

    RB_tree<int,int> rb_tree;
    for(int i = 20;i < 10000;i += 10)
    {
        rb_tree.insert(rand()%1000000,rand()%1000000);
    }

    // for(int i = 1;i < 10000;++i)
    // {
    //     rb_tree.insert(i,i);
    // }

    // for(int i = 10000;i > 0;--i)
    // {
    //     rb_tree.insert(i,i);
    // }

    // rb_tree.insert(10,10);

    std::cout<<"m_root_node->m_key:"<<rb_tree.m_root_node->m_key<<std::endl;

    std::map<int,int> m = rb_tree.get_all_route_black_node_count();

    for(auto i = m.begin();i != m.end(); ++i)
    {
        std::cout<<"index:"<<i->first<<" count:"<<i->second<<std::endl;
    }

    return 0;
}