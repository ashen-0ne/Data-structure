#include "AVL_tree.h"
#include "RB_tree.h"
#include <map>

int main(int argc,char ** argv)
{
    srand(time(0));
    AVL_tree<int,std::string> tree;

    // for(int i = 20;i < 10000;i += 10)
    // {
    //     tree.insert(rand()%1000000,std::to_string(rand()%1000000));
    // }

    // for(int i = 20;i < 10000;i += 10)
    // {
    //     tree.erase(rand()%1000000);
    // }

    // for(int i = 20;i < 10000;i += 10)
    // {
    //     tree.insert(rand()%1000000,std::to_string(rand()%1000000));
    // }

    tree.print_node();

    // RB_tree<int,int> rb_tree;
    // for(int i = 20;i < 10000;i += 10)
    // {
    //     rb_tree.insert(rand()%1000000,rand()%1000000);
    // }

    // for(int i = 300; i < 600; i+= 10)
    // {
    //     rb_tree.insert(i,i);
    // }

    // for(int i = 300,i < 600;i += 10)
    // {
    //     rb_tree.erase(i);
    // }

    // rb_tree.print_node();
    // for(int i = 50000;i >= 10000;i -= 10)
    // {
    //     tree.insert(i,std::to_string(i));
    //     // tree.level_print();
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
    // tree.insert(20,"20");
    // tree.print_node();
    // tree.insert(30,"30");
    // tree.print_node();
    // tree.insert(40,"40");
    // tree.print_node();
    // tree.insert(50,"50");
    // tree.print_node();
    // tree.insert(60,"60");

    return 0;
}