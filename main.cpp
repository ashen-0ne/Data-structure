#include "AVL_tree.h"

int main(int argc,char ** argv)
{
    AVL_tree<int,std::string> tree;

    for(int i = 20;i < 70;i += 10)
    {
        tree.insert(i,std::to_string(i));
        tree.print_node();
        std::cout<<std::endl;
    }
    
    // tree.insert(21,"21");
    tree.insert(39,"39");

    tree.print_node();

    return 0;
}