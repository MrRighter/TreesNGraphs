#pragma once

#include "../TreesNGraphs/VARIANTS.h"

namespace MIREA_KB3_ALGIS {

    template<std::default_initializable Key, std::default_initializable Value> class Tree_Ivanov0000;
    //Объявляется дерево выше итератора, чтобы итератор знал, что определение дерева будет ниже

    struct Key_Ivanov0000 {};
    struct Value_Ivanov0000 {};

    template <std::default_initializable Key, std::default_initializable Value>
    class Iterator_Ivanov0000 {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        value_type operator*() const { /* Возврат текущего Key*/ }
        /* Остальные переопределённые операторы для forward_iterator*/

    }

    template <std::default_initializable Key, std::default_initializable Value>
    class Tree_Ivanov0000 : public Variant000Tree<Key, Value, Iterator_Ivanov0000<Key, Value>, 50> {
    public:
        virtual bool Insert(Key key, Value value, Key parent = Key{}, int tin = -1) { /* Реализация метода вставки */ }
        /* Остальные определения методов */
    };

    template<std::default_initializable Key, std::default_initializable Value>
    class Graph_Ivanov0000 : public Variant000Graph<Key, Value, 50, Iterator_Ivanov0000<Key, Value>, Tree_Ivanov0000<Key, Value>> {
    public:
        bool addVertex(Key key, Value value) { /* Вставка новой вершины */ }
        /* Остальные определения методов */
    };
}
