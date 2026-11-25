#include "Header_Ivanov0000.h"

int main() {

    Key_Ivanov0000 key1();
    Value_Ivanov0000 value1();

    Tree_Ivanov0000< Key_Ivanov0000, Value_Ivanov0000> tree();
    
    /* Проверяем методы дерева */
    tree.Insert(key1, value1);

    tree.PrintTree();

    for (auto node : tree) { //вызывается итератор
        std::cout << node << std::endl;
    }

    Graph_Ivanov0000<Key_Ivanov0000, Value_Ivanov0000> graph();
    
    /* Проверяем методы графа */
    graph.addVertex(key1, value1);

    graph.CalcForest(key1);

    graph.MarkAllLinks();

    ITree<Key_Ivanov0000, Value_Ivanov0000> treePtr = getTree();

    for (auto node : *treePtr) { //проверка полученного дерева
        std::cout << node << std::endl;
    }

    graph.PrintGraph();

}