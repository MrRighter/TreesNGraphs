#pragma once

#include "../TreesNGraphs/VARIANTS.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <compare>

namespace MIREA_KB3_ALGIS {

    template<std::default_initializable Key, std::default_initializable Value> class Tree_Shikunov_0077;
    //Объявляется дерево выше итератора, чтобы итератор знал, что определение дерева будет ниже

    struct Key_Shikunov_0077 {};
    struct Value_Shikunov_0077 {};

    template <std::default_initializable Key, std::default_initializable Value>
    class Iterator_Shikunov_0077 {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        value_type operator*() const { /* Возврат текущего Key*/ }
        /* Остальные переопределённые операторы для forward_iterator*/
    };

    template <std::default_initializable Key, std::default_initializable Value>
    class Tree_Shikunov_0077 : public Variant077Tree<Key, Value, Iterator_Shikunov_0077<Key, Value>, 50> {
    public:
        virtual bool Insert(Key key, Value value, Key parent = Key{}, int tin = -1) { /* Реализация метода вставки */ }
        /* Остальные определения методов */
    };

    template<std::default_initializable Key, std::default_initializable Value>
    class Graph_Shikunov_0077 : public Variant077Graph<Key, Value, 50, Iterator_Shikunov_0077<Key, Value>, Tree_Shikunov_0077<Key, Value>> {
    public:
        bool addVertex(Key key, Value value) { /* Вставка новой вершины */ }
        /* Остальные определения методов */
    };

}
