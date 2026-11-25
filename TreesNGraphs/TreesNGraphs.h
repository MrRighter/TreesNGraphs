#pragma once
#ifndef MIREA_KB3_ALGIS_TREESNGRAPHS
#define MIREA_KB3_ALGIS_TREESNGRAPHS

#include <compare>
#include <concepts>
#include <iterator>
#include <memory>
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <set>

namespace MIREA_KB3_ALGIS {

    template <std::size_t N>
    concept ValidSize = (N == 50); // Правильный N = 50, максимальное число элементов

    template <typename T>
    concept Printable = requires(std::ostream & os, const T & value) { // Требование возможности вывода на печать
        { os << value } -> std::same_as<std::ostream&>;
    };

    template<std::default_initializable Key, std::default_initializable Value>
        requires Printable<Key> && Printable<Value>
    struct ITreeNode {
        Key key; //Ключ - тип данных, по которому производится поиск и сравнение
        Value value; // Значение - полезные данные
        int tin, tout; // Глубинные числа, обозначающие шаг начала и конца обработки узла на этапе построения дерева из графа

        ITreeNode() {
            key = Key();
            value = Value();
            tin = -1;
            tout = -1;
        }

        virtual std::strong_ordering operator<=>(const ITreeNode& other) const final { // Генерация операторов сравнения узлов из сравнения по ключу
            return key <=> other.key;
        }
        virtual ~ITreeNode() = default;
    };

    template<std::default_initializable Key, std::default_initializable Value, std::forward_iterator Iterator>
        requires Printable<Key>&& Printable<Value> 
    class ITree {
    public:
        ITree() {};
        virtual ~ITree() = default;

        /* АТД */
        /* Публичный интерфейс класса, можно использовать из main() или из класса графа */
        virtual bool Insert(Key key, Value value, Key parent = Key{}, int tin = -1) = 0; //Вставка элемента ключ-значение
        bool SetTOut(Key key, int tout) {
            ITreeNode<Key,Value> *node = getNode(key);
            if (node == nullptr) return false;
            node->tout = tout;
            return true;
        }
        virtual Value Find(Key key) = 0; // Поиск по ключу
        virtual Key GetParent(Key key) = 0; // Поиск родительского ключа для текущего
        virtual bool IsChild(Key key, Key parent) = 0; // Является ли ключ родителем переданного ключа
        virtual bool IsAncestor(Key key, Key ancestor) = 0; // Является ли ключ предком переданного ключа

        virtual bool IsEmpty() = 0; // Пусто ли дерево
        virtual void Clear() = 0; // Очистить дерево

        virtual void PrintTree() = 0; // Печать дерева

        virtual Iterator begin() noexcept = 0; // Обязательный метод в С++ для возврата итератора начала обхода
        virtual Iterator end() noexcept = 0; // То же для конца обхода

        /* Служебные */
        /* Можно использовать внутри класса и в итераторе,
           в настоящем проекте использовался бы модификатор protected */
        virtual ITreeNode<Key, Value>* getNode(Key key) = 0; //Вернуть узел по ключу
        virtual ITreeNode<Key, Value>* getRoot() = 0; //Вернуть узел корня
        virtual ITreeNode<Key, Value>* getParentNode(Key key) = 0; //Вернуть узел родителя
        virtual ITreeNode<Key, Value>* leftmost_child(Key key) = 0; // Вернуть узел левого сына
        virtual ITreeNode<Key, Value>* right_sibling(Key key) = 0; // Вернуть узел правого брата
    };

    template<typename Key, typename Value>
    struct NodeCursorToParent : public ITreeNode<Key, Value> { // Узел варианта с курсором на родителя
        int parent; // курсор на родителя в массиве, -1 - корень дерева
        NodeCursorToParent() : parent(-1), ITreeNode<Key, Value>() { }
    };

    template<typename Key, typename Value, std::forward_iterator Iterator, std::size_t N>
        requires ValidSize<N>
    class CursorToParentTree : public ITree<Key, Value, Iterator> {
        //Реализация дерева курсором на родителя. array[0] - корень, содержит курсор на -1
    protected:
        NodeCursorToParent<Key, Value>* _array;
        int _root;
    public:
        CursorToParentTree() {
            this->_array = new NodeCursorToParent<Key, Value>[N];
            this->_root = -1;
        }
        ~CursorToParentTree() { delete[] this->_array; }

        ITreeNode<Key, Value>* getRoot() final { return this->_root >= 0 ? &this->_array[this->_root] : nullptr; }
        virtual const ITreeNode<Key, Value>* getArray() final { return _array; }
    };

    template<typename Key, typename Value, std::size_t N>
    struct NodeChildrenListPtr : public ITreeNode<Key, Value> { // узел варианта с указателями на детей
        NodeChildrenListPtr** _children;
        NodeChildrenListPtr<Key, Value, N>() { this->_children = new NodeChildrenListPtr<Key, Value, N> *[N]; std::fill_n(this->_children, N, nullptr);}
        ~NodeChildrenListPtr<Key, Value, N>() {
            for (int i = 0; i < N; i++) { delete this->_children[i]; }
            delete[] this->_children;
        };
    };

    template<typename Key, typename Value, std::forward_iterator Iterator, std::size_t N>
        requires ValidSize<N>
    class ChildrenListPtrTree : public ITree<Key, Value, Iterator> {
        //Реализация дерева списком сыновей, где список - односвязный
    protected:
        NodeChildrenListPtr<Key, Value, N>* _root;
    public:
        ChildrenListPtrTree() { _root = nullptr;}
        ITreeNode<Key, Value>* getRoot() final { return _root; }
        ~ChildrenListPtrTree() { delete _root; }
    };

    template<typename Key, typename Value, std::size_t N>
    struct NodeChildrenListCursor : public ITreeNode<Key, Value> {
        int* _children;
        virtual int* getChildren() final { return _children; }
        NodeChildrenListCursor() { this->_children = new int[N]; std::fill_n(this->_children, N, -1);}
        ~NodeChildrenListCursor() { delete[] this->_children; }
    };

    template<typename Key, typename Value, std::forward_iterator Iterator, std::size_t N>
        requires ValidSize<N>
    class ChildrenListCursorTree : public ITree<Key, Value, Iterator> {
        //Реализация дерева списком сыновей, где в узле список индексов сыновей в общем массиве
    protected:
        NodeChildrenListCursor<Key, Value, N>* _array;
        int _root = -1;
    public:
        ITreeNode<Key, Value>* getRoot() final { return this->_root >= 0 ? &this->_array[this->_root] : nullptr; }
        virtual const NodeChildrenListCursor<Key, Value, N>* getArray() final { return _array; }
        ChildrenListCursorTree() { this->_array = new NodeChildrenListCursor<Key, Value, N>[N]; }
        ~ChildrenListCursorTree() { delete[] this->_array; }
    };

    template<typename Key, typename Value>
    struct NodeLeftSonRightSiblingPtr : public ITreeNode<Key, Value> {
        NodeLeftSonRightSiblingPtr *leftSon, *rightSibling;
        NodeLeftSonRightSiblingPtr() { leftSon = nullptr; rightSibling = nullptr; }
        ~NodeLeftSonRightSiblingPtr() { delete rightSibling; delete leftSon; }
    };

    template<typename Key, typename Value, std::forward_iterator Iterator>
    class LeftSonRightSiblingPtrTree : public ITree<Key, Value, Iterator> {
        //Реализация дерева с указателями на левого сына и список правых братьев
    protected:
        NodeLeftSonRightSiblingPtr<Key, Value>* _root;
    public:
        ITreeNode<Key, Value>* getRoot() final { return _root; }
        LeftSonRightSiblingPtrTree() { _root = nullptr; }
        ~LeftSonRightSiblingPtrTree() { delete _root; }
    };

    template<typename Key, typename Value>
    struct NodeLeftSonRightSiblingArray : public ITreeNode<Key, Value> {
        int leftSon;
        int rightSibling;
        NodeLeftSonRightSiblingArray() {
            leftSon = -1;
            rightSibling = -1;
        }
    };

    template<typename Key, typename Value, std::forward_iterator Iterator, std::size_t N>
        requires ValidSize<N>
    class LeftSonRightSiblingArrayTree : public ITree<Key, Value, Iterator> {
        //Реализация дерева с индексами на левого сына и список правых братьев в общем массиве
    protected:
        int _root;
        NodeLeftSonRightSiblingArray<Key, Value>* _array;
    public:
        ITreeNode<Key, Value>* getRoot() final { return this->_root >= 0 ? &this->_array[this->_root] : nullptr; }
        virtual const NodeLeftSonRightSiblingArray<Key, Value>* getArray() final { return this->_array; }
        LeftSonRightSiblingArrayTree() {
            this->_array = new NodeLeftSonRightSiblingArray<Key, Value>[N]();
            _root = -1;
        }
        ~LeftSonRightSiblingArrayTree() {
            delete[] this->_array;
        }
    };

    enum NodeColor { WHITE, GREY, BLACK };
    enum LinkType { FORWARD, BACK, TREE, INTERNAL_CROSS, EXTERNAL_CROSS, NONE };
    
    template<typename Key, typename Value>
    struct GraphNode {
        Key key;
        Value value;
        NodeColor color;
        GraphNode() { color = NodeColor::WHITE; }
    };
    
    struct GraphLink {
        int cost;
        std::string label;
        LinkType type;
        GraphLink() { cost = -1; type = LinkType::NONE; }
        virtual ~GraphLink() = default;
    };

    template<std::default_initializable Key, std::default_initializable Value, std::forward_iterator TreeIterator>
    class IGraph {
    protected:
        std::vector <ITree<Key, Value, TreeIterator> *> _forest;
        std::vector<GraphNode<Key, Value>> _nodes;

    public:
        virtual ~IGraph() = default;

        virtual bool addVertex(Key key, Value value) = 0; // добавление вершины
        virtual bool addEdge(Key outKey, Key inKey, std::string label = "", int cost = 0) = 0; // добавление дуги

        virtual bool hasVertex(Key key) const = 0; // проверка наличия вершины
        virtual Value Find(Key key) = 0; // поиск значения по ключу
        virtual bool hasEdge(Key outKey, Key inKey) const = 0; // проверка наличия дуги
        virtual LinkType getEdgeType(Key outKey, Key inKey) = 0; // возврат типа дуги
        virtual int getEdgeCost(Key outKey, Key inKey) = 0; // возврат цены дуги
        virtual std::string getEdgeLabel(Key outKey, Key inKey) = 0; // возврат метки дуги
        virtual bool markEdge(Key outKey, Key inKey, LinkType type) = 0; // задать тип дуги

        virtual std::set<Key> getNeighbors(Key key) = 0; //вернуть отсортированный(!!) список соседей узла

        virtual bool IsEmpty() const = 0; // истина, если граф пуст
        virtual void Clear() = 0; // очистить граф

        /* ************ */

        virtual void DFS(Key key, Key parent, ITree<Key, Value, TreeIterator>& tree, int& timer) {}; //обход в глубину
        virtual void BFS(Key key, Key parent, ITree<Key, Value, TreeIterator>& tree, int& timer) {}; //обход в ширину
        virtual void CalcForest(Key key = Key{}) = 0;   //заполнить лес деревьев _forest начиная с вершины key
                                                        //с помощью DFS или BFS по варианту, переопределить нужный, другой оставить пустым {}
                                                        //обходить вершины в порядке возрастания ключа
        virtual void MarkLinks(ITree<Key, Value, TreeIterator>& tree) = 0; //пометить согласно дереву
        virtual void MarkAllLinks() = 0; // гуляя по лесу пометить всё
        virtual void PrintGraph() = 0; // печать узлов и дуг(с типом, меткой и стоимостью)
        ITree<Key, Value, TreeIterator>* getTree(int treeN = 0) { //вернуть дерево из леса по номеру
            if (treeN < _forest.size()) return _forest[treeN]; else return nullptr;
        } 
        const std::vector<GraphNode<Key, Value>> &getNodes() { return _nodes; } //вернуть список узлов
        virtual const GraphLink * getImplementation() = 0; //вернуть реализацию графа
        virtual void ResetColors() final{ // сброс цветов узлов
            for (auto &node : _nodes) { // (!!! Пример использования итератора вектора !!!)
                node.color = NodeColor::WHITE;
            }
        }
        virtual void ResetEdgesType() = 0; // сброс типов дуг
        virtual void ClearForest() final { // очистка леса
            for (auto tree : _forest) delete tree;
            _forest.clear();
        }
    };

    template<typename Key>
    struct AdjMatrixGraphLink : public GraphLink {
        bool link; // значения true, false
        AdjMatrixGraphLink() : GraphLink() { link = false; }
    };

    template<typename Key, typename Value, std::forward_iterator TreeIterator, std::size_t N,
        std::derived_from<ITree<Key, Value, TreeIterator>> Tree>
        requires ValidSize<N>
    class AdjMatrixGraph : public IGraph<Key, Value, TreeIterator> {
    protected:
        AdjMatrixGraphLink<Key>* _adjMatrix; // матрица смежности
    public:
        const GraphLink * getImplementation() final { return _adjMatrix; }
        AdjMatrixGraph() : IGraph<Key, Value, TreeIterator>() {
            this->_adjMatrix = new AdjMatrixGraphLink<Key>[N * N];
        }
        ~AdjMatrixGraph() {
            delete[] this->_adjMatrix;
            for (auto tree : this->_forest) delete tree;
        }
    };

    template<typename Key>
    struct IncMatrixGraphLink : public GraphLink {
        int link; // -1, 0, 1, 2 - для ориентированного графа
        // 2 - петля
        IncMatrixGraphLink() : GraphLink() { link = 0; }
    };

    template<typename Key, typename Value, std::forward_iterator TreeIterator, std::size_t N,
        std::derived_from<ITree<Key, Value, TreeIterator>> Tree>
        requires ValidSize<N>
    class IncMatrixGraph : public IGraph<Key, Value, TreeIterator> {
    protected:
        IncMatrixGraphLink<Key> *_incMatrix; // матрица инцидентности, по горизонтали все возможные варианты дуг (в т.ч. кратные), по вертикали узлы
        std::vector<std::tuple<Key, Key, int>> _index; //Содержит соответствие пары ключей столбцу в матрице, обновляется при добавлении узла
    public:
        const GraphLink * getImplementation() final { return _incMatrix; }
        const std::vector<std::tuple<Key, Key, int>> getIndex() { return _index; }
        IncMatrixGraph() : IGraph<Key, Value, TreeIterator>() {
            this->_incMatrix = new IncMatrixGraphLink<Key>[N * N * N]();
        }
        ~IncMatrixGraph() { 
            delete[] this->_incMatrix;
            for (auto tree : this->_forest) delete tree;
        }
    };

    template<typename Key>
    struct LinkListGraphLink : public GraphLink {
        Key first;
        Key second;
        LinkListGraphLink* next; //односвязный список дуг
        LinkListGraphLink() : GraphLink() { next = nullptr; }
        ~LinkListGraphLink() { delete next; }
    };

    template<typename Key, typename Value, std::forward_iterator TreeIterator, std::size_t N,
        std::derived_from<ITree<Key, Value, TreeIterator>> Tree>
        requires ValidSize<N>
    class LinkListGraph : public IGraph<Key, Value, TreeIterator> {
    protected:
        LinkListGraphLink<Key>* _head; // список дуг
    public:
        const GraphLink * getImplementation() final { return _head; }
        LinkListGraph() : IGraph<Key, Value, TreeIterator>(){
            this->_head = nullptr;
        }
        ~LinkListGraph() {  
            delete this->_head;
            for (auto tree : this->_forest) delete tree;
        }
    };
}

#endif // !MIREA_KB3_ALGIS_TREESNGRAPHS
