#include "AVL_tree.h"

int main(int argc,char ** argv)
{
    AVL_tree<int,std::string> tree;

    // for(int i = 20;i < 10000;i += 10)
    // {
    //     tree.insert(i,std::to_string(i));
    // }

    // for(int i = 70000;i >= 69000;i -= 10)
    // {
    //     tree.insert(i,std::to_string(i));
    // }
    
    // tree.insert(20,"20");
    // tree.insert(30,"30");
    // tree.insert(25,"25");

    // tree.insert(30,"30");
    // tree.insert(20,"20");
    // tree.insert(40,"40");
    // tree.insert(35,"35");
    // tree.insert(50,"50");
    // tree.insert(34,"34");
    // tree.insert(36,"36");

    // tree.insert(10,"10");
    tree.insert(20,"20");
    tree.print_node();
    tree.insert(30,"30");
    tree.print_node();
    tree.insert(40,"40");
    tree.print_node();
    tree.insert(50,"50");
    tree.print_node();
    tree.insert(60,"60");

    tree.print_node();

    return 0;
}