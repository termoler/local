#include <iostream>
#include "flat_map.h"

#include <string>


int main() {
    flat_map<int, std::string> tree;
    for(int i = 0; i < 200; i++){
        tree[i] = std::to_string(20 - i);
    }
    tree.erase(5);
    flat_map<std::string, std::string> m;
    auto adf = m.find("Asdf");
    m["hello"] = "world";
    m["abc"] = "defg";
    m["123"] = "111";
    std::cout<<m["hello"]<<tree[2];
//    flat_map<int, std::string> bereza;
//    bereza = tree;
//    tree.clear();
//    auto it = tree.begin();
//    Iterator<int, std::string> rew(it);
//    bereza[1111] = "134";
//    for(it = bereza.begin(); it != bereza.end(); ++it)
//    {
//        std::cout<<it->first<<" "<<it->second<<std::endl;
//    }
//
//
//    auto g = bereza.begin();
//    for(g = bereza.begin(); g != bereza.end(); ++g)
//    {
//        std::cout<<g->first<<" "<<g->second<<std::endl;
//    }
//    bereza[11112] = "134";
//    std::cout<<"\n";
//    return 0;
}