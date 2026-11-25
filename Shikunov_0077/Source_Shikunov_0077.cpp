#include "Header_Shikunov_0077.h"

int main() {

    Key_Shikunov_0077 key1();
    Value_Shikunov_0077 value1();

    Tree_Shikunov_0077<Key_Shikunov_0077, Value_Shikunov_0077> tree();

    /* Проверяем методы дерева */
    tree.Insert(key1, value1);

    tree.PrintTree();

    for (auto node : tree) { //вызывается итератор
        std::cout << node << std::endl;
    }

    Graph_Shikunov_0077<Key_Shikunov_0077, Value_Shikunov_0077> graph();

    /* Проверяем методы графа */
    graph.addVertex(key1, value1);

    graph.CalcForest(key1);

    graph.MarkAllLinks();

    ITree<Key_Shikunov_0077, Value_Shikunov_0077> treePtr = getTree();

    for (auto node : *treePtr) { //проверка полученного дерева
        std::cout << node << std::endl;
    }

    graph.PrintGraph();

}
