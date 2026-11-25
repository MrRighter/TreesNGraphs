#include "Header_Shikunov_0077.h"
#include <iostream>

using namespace MIREA_KB3_ALGIS;

int main() {
    std::setlocale(LC_ALL, "Russian");

    std::cout << "=== ТЕСТИРОВАНИЕ ДЕРЕВА (Ручное заполнение) ===\n";

    Key_Shikunov_0077 key1(1);
    Value_Shikunov_0077 value1(10);

    Tree_Shikunov_0077<Key_Shikunov_0077, Value_Shikunov_0077> tree;

    tree.Insert(Key_Shikunov_0077(1), Value_Shikunov_0077(100));
    tree.Insert(Key_Shikunov_0077(2), Value_Shikunov_0077(200), Key_Shikunov_0077(1));
    tree.Insert(Key_Shikunov_0077(3), Value_Shikunov_0077(300), Key_Shikunov_0077(1));
    tree.Insert(Key_Shikunov_0077(4), Value_Shikunov_0077(400), Key_Shikunov_0077(2));
    tree.Insert(Key_Shikunov_0077(5), Value_Shikunov_0077(500), Key_Shikunov_0077(2));

    std::cout << "Вывод дерева (PrintTree):\n";
    tree.PrintTree();

    std::cout << "\nИтератор (Симметричный обход):\n";
    for (auto node : tree) {
        std::cout << node << " ";
    }
    std::cout << "\n";

    std::cout << "\nПрикладная задача (узлы справа):\n";
    Key_Shikunov_0077 searchKey;
    std::cout << "Введите ID ключа для поиска правых братьев (например, 2): ";
    std::cin >> searchKey.id;
    tree.PrintRightSiblings(searchKey);


    std::cout << "\n\n=== ТЕСТИРОВАНИЕ ГРАФА (16 вершин, 3 компоненты связности) ===\n";
    Graph_Shikunov_0077<Key_Shikunov_0077, Value_Shikunov_0077> graph;

    for (int i = 1; i <= 16; ++i) {
        graph.addVertex(Key_Shikunov_0077(i), Value_Shikunov_0077(i * 10));
    }

    // Компонента связности 1 (содержит циклы)
    graph.addEdge(Key_Shikunov_0077(1), Key_Shikunov_0077(2), "e1", 5);
    graph.addEdge(Key_Shikunov_0077(1), Key_Shikunov_0077(3), "e2", 3);
    graph.addEdge(Key_Shikunov_0077(2), Key_Shikunov_0077(4), "e3", 2);
    graph.addEdge(Key_Shikunov_0077(2), Key_Shikunov_0077(5), "e4", 4);
    graph.addEdge(Key_Shikunov_0077(3), Key_Shikunov_0077(6), "e5", 6);
    graph.addEdge(Key_Shikunov_0077(4), Key_Shikunov_0077(7), "e6", 1);
    graph.addEdge(Key_Shikunov_0077(5), Key_Shikunov_0077(8), "e7", 7);
    graph.addEdge(Key_Shikunov_0077(6), Key_Shikunov_0077(9), "e8", 8);
    graph.addEdge(Key_Shikunov_0077(7), Key_Shikunov_0077(10), "e9", 9);
    graph.addEdge(Key_Shikunov_0077(8), Key_Shikunov_0077(11), "e10", 2);
    graph.addEdge(Key_Shikunov_0077(9), Key_Shikunov_0077(12), "e11", 3);

    // Обратные и поперечные дуги для создания циклов
    graph.addEdge(Key_Shikunov_0077(12), Key_Shikunov_0077(1), "back1", 1);
    graph.addEdge(Key_Shikunov_0077(3), Key_Shikunov_0077(5), "cross1", 4); // Поперечная дуга
    graph.addEdge(Key_Shikunov_0077(6), Key_Shikunov_0077(10), "cross2", 5); // Поперечная дуга

    // Компонента связности 2 (простой цикл)
    graph.addEdge(Key_Shikunov_0077(13), Key_Shikunov_0077(14), "e12", 1);
    graph.addEdge(Key_Shikunov_0077(14), Key_Shikunov_0077(15), "e13", 1);
    graph.addEdge(Key_Shikunov_0077(15), Key_Shikunov_0077(13), "e14", 1);

    // Компонента связности 3 (изолированная вершина)
    // Вершина 16 не имеет дуг, как и требуется в задании ("один из которых состоит из 1 узла")

    graph.CalcForest();
    graph.MarkAllLinks();

    std::cout << "\nИсходный граф и типы его дуг:\n";
    graph.PrintGraph();

    std::cout << "\nЛеса деревьев (Результат обхода DFS):\n";
    for (int i = 0; i < graph.getNodes().size(); ++i) {
        auto* t = graph.getTree(i);
        if (t) {
            std::cout << "Дерево " << i + 1 << ":\n";
            t->PrintTree();
        }
    }

    std::cout << "\nПрикладная задача (глубина поперечных дуг):\n";
    graph.PrintCrossEdgesDepth();

    std::cout << "\n=== Проверка полученного дерева из леса (treePtr) ===\n";
    ITree<Key_Shikunov_0077, Value_Shikunov_0077, Iterator_Shikunov_0077<Key_Shikunov_0077, Value_Shikunov_0077>>* treePtr = graph.getTree(0);

    if (treePtr) {
        for (auto node : *treePtr) {
            std::cout << node << std::endl;
        }
    }

    graph.PrintGraph();

    return 0;

}
