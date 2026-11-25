#pragma once

#ifndef MIREA_KB3_ALGIS_TREESNGRAPHS_VARIANT_02
#define MIREA_KB3_ALGIS_TREESNGRAPHS_VARIANT_02

#include "TreesNGraphs.h"
namespace MIREA_KB3_ALGIS {

    //Вариант 077
    //Дерево	                                Граф	            Итератор	    Обход графа
    //Левый сын, правый брат (массив, индексы)  Матрица смежности	Симметричный	В глубину

    template<std::default_initializable Key, std::default_initializable Value,
        std::forward_iterator Iterator, std::size_t N>
    class Variant077Tree : public LeftSonRightSiblingArrayTree<Key, Value, Iterator, N> {};

    template<std::default_initializable Key, std::default_initializable Value, std::size_t N,
        std::forward_iterator Iterator,
        std::derived_from<Variant077Tree<Key, Value, Iterator, N>> Tree>
        requires ValidSize<N>
    class Variant077Graph : public AdjMatrixGraph<Key, Value, Iterator, N, Tree> {};

}

#endif
