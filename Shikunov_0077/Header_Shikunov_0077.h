#pragma once

#include "../TreesNGraphs/VARIANTS.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <compare>
#include <algorithm>

namespace MIREA_KB3_ALGIS {

    template<std::default_initializable Key, std::default_initializable Value> class Tree_Shikunov_0077;

    struct Key_Shikunov_0077 {
        int id;
        Key_Shikunov_0077() : id(0) {}
        Key_Shikunov_0077(int i) : id(i) {}
        auto operator<=>(const Key_Shikunov_0077& other) const = default;

        friend std::ostream& operator<<(std::ostream& os, const Key_Shikunov_0077& k) {
            return os << k.id << "_Shikunov";
        }
    };

    struct Value_Shikunov_0077 {
        int data;
        Value_Shikunov_0077() : data(0) {}
        Value_Shikunov_0077(int d) : data(d) {}

        friend std::ostream& operator<<(std::ostream& os, const Value_Shikunov_0077& v) {
            return os << v.data << "_0077";
        }
    };


    template <std::default_initializable Key, std::default_initializable Value>
    class Iterator_Shikunov_0077 {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator_Shikunov_0077() : treePtr(nullptr), currentIndex(-1) {}
        Iterator_Shikunov_0077(Tree_Shikunov_0077<Key, Value>* tree, int index) : treePtr(tree), currentIndex(index) {}

        value_type operator*() const {
            return treePtr->getNodeByIndex(currentIndex)->key;
        }

        pointer operator->() const {
            return &(treePtr->getNodeByIndex(currentIndex)->key);
        }

        Iterator_Shikunov_0077& operator++() {
            if (!treePtr || currentIndex == -1) return *this;

            int rightSib = treePtr->getRightSiblingIndex(currentIndex);
            if (rightSib != -1) {
                currentIndex = rightSib;
                int leftChild = treePtr->getLeftChildIndex(currentIndex);
                while (leftChild != -1) {
                    currentIndex = leftChild;
                    leftChild = treePtr->getLeftChildIndex(currentIndex);
                }
            }
            else {
                currentIndex = treePtr->getParentIndex(currentIndex);
            }
            return *this;
        }

        Iterator_Shikunov_0077 operator++(int) {
            Iterator_Shikunov_0077 temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator_Shikunov_0077& other) const {
            return treePtr == other.treePtr && currentIndex == other.currentIndex;
        }

        bool operator!=(const Iterator_Shikunov_0077& other) const {
            return !(*this == other);
        }

    private:
        Tree_Shikunov_0077<Key, Value>* treePtr;
        int currentIndex;
    };


    template <std::default_initializable Key, std::default_initializable Value>
    class Tree_Shikunov_0077 : public Variant077Tree<Key, Value, Iterator_Shikunov_0077<Key, Value>, 50> {
        friend class Iterator_Shikunov_0077<Key, Value>;
    public:
        virtual bool Insert(Key key, Value value, Key parent = Key{}, int tin = -1) override {
            if (this->_root == -1) {
                this->_array[0].key = key;
                this->_array[0].value = value;
                this->_array[0].tin = tin;
                this->_root = 0;
                _size = 1;
                return true;
            }

            int parentIdx = FindIndex(parent);
            if (parentIdx == -1) return false;

            if (_size >= 50) return false;
            int freeIdx = _size++;

            this->_array[freeIdx].key = key;
            this->_array[freeIdx].value = value;
            this->_array[freeIdx].tin = tin;

            if (this->_array[parentIdx].leftSon == -1) {
                this->_array[parentIdx].leftSon = freeIdx;
            }
            else {
                int sibling = this->_array[parentIdx].leftSon;
                while (this->_array[sibling].rightSibling != -1) {
                    sibling = this->_array[sibling].rightSibling;
                }
                this->_array[sibling].rightSibling = freeIdx;
            }
            return true;
        }
        
        int _size = 0;

        virtual Value Find(Key key) override {
            int idx = FindIndex(key);
            return idx != -1 ? this->_array[idx].value : Value{};
        }

        virtual Key GetParent(Key key) override {
            int idx = FindIndex(key);
            if (idx == -1 || idx == this->_root) return Key{};
            int pIdx = getParentIndex(idx);
            return this->_array[pIdx].key;
        }

        virtual bool IsChild(Key key, Key parent) override {
            int pIdx = FindIndex(parent);
            if (pIdx == -1) return false;
            int child = this->_array[pIdx].leftSon;
            while (child != -1) {
                if (this->_array[child].key == key) return true;
                child = this->_array[child].rightSibling;
            }
            return false;
        }

        virtual bool IsAncestor(Key key, Key ancestor) override {
            int current = FindIndex(key);
            while (current != -1 && current != this->_root) {
                current = getParentIndex(current);
                if (current != -1 && this->_array[current].key == ancestor) return true;
            }
            return false;
        }

        virtual bool IsEmpty() override { return this->_root == -1; }

        virtual void Clear() override {
            for (int i = 0; i < 50; ++i) {
                this->_array[i].key = Key{};
                this->_array[i].value = Value{};
                this->_array[i].leftSon = -1;
                this->_array[i].rightSibling = -1;
                this->_array[i].tin = -1;
                this->_array[i].tout = -1;
            }
            this->_root = -1;
            _size = 0;
        }

        virtual void PrintTree() override {
            if (IsEmpty()) return;
            PrintNode(this->_root, 0);
        }

        virtual Iterator_Shikunov_0077<Key, Value> begin() noexcept override {
            if (IsEmpty()) return end();
            int current = this->_root;
            int leftChild = getLeftChildIndex(current);
            while (leftChild != -1) {
                current = leftChild;
                leftChild = getLeftChildIndex(current);
            }
            return Iterator_Shikunov_0077<Key, Value>(this, current);
        }

        virtual Iterator_Shikunov_0077<Key, Value> end() noexcept override {
            return Iterator_Shikunov_0077<Key, Value>(this, -1);
        }

        virtual ITreeNode<Key, Value>* getNode(Key key) override {
            int idx = FindIndex(key);
            return idx != -1 ? &this->_array[idx] : nullptr;
        }

        virtual ITreeNode<Key, Value>* getParentNode(Key key) override {
            int idx = FindIndex(key);
            if (idx == -1 || idx == this->_root) return nullptr;
            return &this->_array[getParentIndex(idx)];
        }

        virtual ITreeNode<Key, Value>* leftmost_child(Key key) override {
            int idx = FindIndex(key);
            if (idx == -1 || this->_array[idx].leftSon == -1) return nullptr;
            return &this->_array[this->_array[idx].leftSon];
        }

        virtual ITreeNode<Key, Value>* right_sibling(Key key) override {
            int idx = FindIndex(key);
            if (idx == -1 || this->_array[idx].rightSibling == -1) return nullptr;
            return &this->_array[this->_array[idx].rightSibling];
        }

        void PrintRightSiblings(Key key) {
            int idx = FindIndex(key);
            if (idx == -1) { std::cout << "Node not found.\n"; return; }
            int sib = this->_array[idx].rightSibling;
            std::cout << "Siblings to the right of " << key << ":\n";
            bool found = false;
            while (sib != -1) {
                std::cout << "  Key: " << this->_array[sib].key << ", Value: " << this->_array[sib].value << "\n";
                sib = this->_array[sib].rightSibling;
                found = true;
            }
            if (!found) std::cout << "  None.\n";
        }

        int getDepth(Key key) {
            int idx = FindIndex(key);
            if (idx == -1) return -1;
            int depth = 0;
            while (idx != this->_root) {
                idx = getParentIndex(idx);
                depth++;
            }
            return depth;
        }

        ITreeNode<Key, Value>* getNodeByIndex(int idx) { return &this->_array[idx]; }
        int getLeftChildIndex(int idx) { return this->_array[idx].leftSon; }
        int getRightSiblingIndex(int idx) { return this->_array[idx].rightSibling; }

        int getParentIndex(int idx) {
            for (int i = 0; i < _size; ++i) {
                if (this->_array[i].leftSon == idx) return i;
                if (this->_array[i].rightSibling == idx) return getParentIndex(i);
            }
            return -1;
        }

    private:
        int FindIndex(Key key) {
            for (int i = 0; i < _size; ++i) {
                if (this->_array[i].key == key) return i;
            }
            return -1;
        }

        void PrintNode(int idx, int depth) {
            if (idx == -1) return;
            std::string indent(depth * 4, ' ');
            std::cout << indent << "[" << this->_array[idx].key << " : " << this->_array[idx].value
                << "] (tin: " << this->_array[idx].tin << ", tout: " << this->_array[idx].tout << ")\n";

            int child = this->_array[idx].leftSon;
            while (child != -1) {
                PrintNode(child, depth + 1);
                child = this->_array[child].rightSibling;
            }
        }
    };


    template<std::default_initializable Key, std::default_initializable Value>
    class Graph_Shikunov_0077 : public Variant077Graph<Key, Value, 50, Iterator_Shikunov_0077<Key, Value>, Tree_Shikunov_0077<Key, Value>> {
    public:
        virtual bool addVertex(Key key, Value value) override {
            if (hasVertex(key)) return false;
            GraphNode<Key, Value> newNode;
            newNode.key = key;
            newNode.value = value;
            this->_nodes.push_back(newNode);
            return true;
        }

        virtual bool addEdge(Key outKey, Key inKey, std::string label = "", int cost = 0) override {
            int outIdx = FindIndex(outKey);
            int inIdx = FindIndex(inKey);
            if (outIdx == -1 || inIdx == -1) return false;

            int linkIdx = outIdx * 50 + inIdx;
            this->_adjMatrix[linkIdx].link = true;
            this->_adjMatrix[linkIdx].cost = cost;
            this->_adjMatrix[linkIdx].label = label;
            return true;
        }

        virtual bool hasVertex(Key key) const override {
            for (const auto& node : this->_nodes) if (node.key == key) return true;
            return false;
        }

        virtual Value Find(Key key) override {
            for (const auto& node : this->_nodes) if (node.key == key) return node.value;
            return Value{};
        }

        virtual bool hasEdge(Key outKey, Key inKey) const override {
            int outIdx = FindIndexConst(outKey);
            int inIdx = FindIndexConst(inKey);
            if (outIdx == -1 || inIdx == -1) return false;
            return this->_adjMatrix[outIdx * 50 + inIdx].link;
        }

        virtual LinkType getEdgeType(Key outKey, Key inKey) override {
            int outIdx = FindIndex(outKey);
            int inIdx = FindIndex(inKey);
            if (outIdx == -1 || inIdx == -1) return LinkType::NONE;
            return this->_adjMatrix[outIdx * 50 + inIdx].type;
        }

        virtual int getEdgeCost(Key outKey, Key inKey) override {
            int outIdx = FindIndex(outKey);
            int inIdx = FindIndex(inKey);
            if (outIdx == -1 || inIdx == -1) return -1;
            return this->_adjMatrix[outIdx * 50 + inIdx].cost;
        }

        virtual std::string getEdgeLabel(Key outKey, Key inKey) override {
            int outIdx = FindIndex(outKey);
            int inIdx = FindIndex(inKey);
            if (outIdx == -1 || inIdx == -1) return "";
            return this->_adjMatrix[outIdx * 50 + inIdx].label;
        }

        virtual bool markEdge(Key outKey, Key inKey, LinkType type) override {
            int outIdx = FindIndex(outKey);
            int inIdx = FindIndex(inKey);
            if (outIdx == -1 || inIdx == -1 || !this->_adjMatrix[outIdx * 50 + inIdx].link) return false;
            this->_adjMatrix[outIdx * 50 + inIdx].type = type;
            return true;
        }

        virtual std::set<Key> getNeighbors(Key key) override {
            std::set<Key> neighbors;
            int outIdx = FindIndex(key);
            if (outIdx == -1) return neighbors;
            for (int i = 0; i < this->_nodes.size(); ++i) {
                if (this->_adjMatrix[outIdx * 50 + i].link) {
                    neighbors.insert(this->_nodes[i].key);
                }
            }
            return neighbors;
        }

        virtual bool IsEmpty() const override { return this->_nodes.empty(); }

        virtual void Clear() override {
            this->_nodes.clear();
            this->ClearForest();
            for (int i = 0; i < 50 * 50; ++i) {
                this->_adjMatrix[i].link = false;
                this->_adjMatrix[i].type = LinkType::NONE;
            }
        }

        virtual void DFS(Key key, Key parent, ITree<Key, Value, Iterator_Shikunov_0077<Key, Value>>& tree, int& timer) override {
            int idx = FindIndex(key);
            if (idx == -1) return;

            this->_nodes[idx].color = NodeColor::GREY;
            tree.Insert(key, this->_nodes[idx].value, parent, timer++);

            std::set<Key> neighbors = getNeighbors(key);
            for (const auto& nb : neighbors) {
                int nIdx = FindIndex(nb);
                if (this->_nodes[nIdx].color == NodeColor::WHITE) {
                    DFS(nb, key, tree, timer);
                }
            }
            this->_nodes[idx].color = NodeColor::BLACK;
            tree.SetTOut(key, timer++);
        }

        virtual void CalcForest(Key key = Key{}) override {
            this->ClearForest();
            this->ResetColors();
            this->ResetEdgesType();
            int timer = 0;

            std::vector<Key> sortedKeys;
            for (const auto& node : this->_nodes) sortedKeys.push_back(node.key);
            std::sort(sortedKeys.begin(), sortedKeys.end());

            for (const auto& k : sortedKeys) {
                int idx = FindIndex(k);
                if (this->_nodes[idx].color == NodeColor::WHITE) {
                    auto* newTree = new Tree_Shikunov_0077<Key, Value>();
                    DFS(k, Key{}, *newTree, timer);
                    this->_forest.push_back(newTree);
                }
            }
        }

        virtual void MarkLinks(ITree<Key, Value, Iterator_Shikunov_0077<Key, Value>>& tree) override {
            for (int i = 0; i < this->_nodes.size(); ++i) {
                for (int j = 0; j < this->_nodes.size(); ++j) {
                    if (this->_adjMatrix[i * 50 + j].link) {
                        Key u = this->_nodes[i].key;
                        Key v = this->_nodes[j].key;
                        auto* nodeU = tree.getNode(u);
                        auto* nodeV = tree.getNode(v);

                        if (nodeU && nodeV) {
                            if (tree.IsChild(v, u)) {
                                this->_adjMatrix[i * 50 + j].type = LinkType::TREE;
                            }
                            else if (tree.IsAncestor(u, v)) {
                                this->_adjMatrix[i * 50 + j].type = LinkType::BACK;
                            }
                            else if (tree.IsAncestor(v, u)) {
                                this->_adjMatrix[i * 50 + j].type = LinkType::FORWARD;
                            }
                            else {
                                this->_adjMatrix[i * 50 + j].type = LinkType::INTERNAL_CROSS;
                            }
                        }
                        else if (nodeU && !nodeV) {
                            this->_adjMatrix[i * 50 + j].type = LinkType::EXTERNAL_CROSS;
                        }
                        else if (!nodeU && nodeV) {
                            this->_adjMatrix[i * 50 + j].type = LinkType::EXTERNAL_CROSS;
                        }
                    }
                }
            }
        }

        virtual void MarkAllLinks() override {
            for (auto* tree : this->_forest) {
                MarkLinks(*tree);
            }
        }

        virtual void PrintGraph() override {
            std::cout << "Graph (Adjacency Matrix):\n";
            for (const auto& node : this->_nodes) {
                std::cout << "[" << node.key << "] -> ";
                bool hasEdges = false;
                for (const auto& nb : getNeighbors(node.key)) {
                    int i = FindIndex(node.key);
                    int j = FindIndex(nb);
                    std::cout << nb << " (Type: " << LinkTypeToString(this->_adjMatrix[i * 50 + j].type)
                        << ", Cost: " << this->_adjMatrix[i * 50 + j].cost << ") ";
                    hasEdges = true;
                }
                if (!hasEdges) std::cout << "None";
                std::cout << "\n";
            }
        }

        virtual void ResetEdgesType() override {
            for (int i = 0; i < 50 * 50; ++i) {
                this->_adjMatrix[i].type = LinkType::NONE;
            }
        }

        void PrintCrossEdgesDepth() {
            std::cout << "Cross edges depths:\n";
            for (int i = 0; i < this->_nodes.size(); ++i) {
                for (int j = 0; j < this->_nodes.size(); ++j) {
                    if (this->_adjMatrix[i * 50 + j].link) {
                        LinkType t = this->_adjMatrix[i * 50 + j].type;
                        if (t == LinkType::INTERNAL_CROSS || t == LinkType::EXTERNAL_CROSS) {
                            Key u = this->_nodes[i].key;
                            Key v = this->_nodes[j].key;
                            int depthU = -1, depthV = -1;

                            for (auto* tree : this->_forest) {
                                auto* myTree = static_cast<Tree_Shikunov_0077<Key, Value>*>(tree);
                                if (tree->getNode(u)) depthU = myTree->getDepth(u);
                                if (tree->getNode(v)) depthV = myTree->getDepth(v);
                            }
                            std::cout << "Cross Edge (" << u << " -> " << v << "): Depth of " << u << " = " << depthU
                                << ", Depth of " << v << " = " << depthV << "\n";
                        }
                    }
                }
            }
        }

    private:
        int FindIndex(Key key) {
            for (int i = 0; i < this->_nodes.size(); ++i) {
                if (this->_nodes[i].key == key) return i;
            }
            return -1;
        }

        int FindIndexConst(Key key) const {
            for (int i = 0; i < this->_nodes.size(); ++i) {
                if (this->_nodes[i].key == key) return i;
            }
            return -1;
        }

        std::string LinkTypeToString(LinkType type) {
            switch (type) {
            case LinkType::TREE: return "TREE";
            case LinkType::BACK: return "BACK";
            case LinkType::FORWARD: return "FORWARD";
            case LinkType::INTERNAL_CROSS: return "INTERNAL_CROSS";
            case LinkType::EXTERNAL_CROSS: return "EXTERNAL_CROSS";
            default: return "NONE";
            }
        }
    };

}
