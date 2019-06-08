#include <iostream>
#include <cmath>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

template<class ValueType> class Set {
public:
    class Node {
    public:
        ValueType key;
        int h = 1;
        Node* L = nullptr;
        Node* R = nullptr;
        Node* P = nullptr;

        Node() : key(ValueType()), h(0) {}
        Node(const ValueType& _key)
            : key(_key), h(1) {}
        Node(const ValueType& _key, int _h) : key(_key), h(_h) {}
    };

    Node* root;
    int sz;

    int getHeight(Node *_node) {
        if (_node != nullptr) {
            return _node->h;
        }
        return 0;
    }

    static Node* getMin(Node* _node) {
        if (_node == nullptr) {
            return nullptr;
        }
        Node* cur = _node;
        while (cur->L != nullptr) {
            cur= cur->L;
        }
        return cur;
    }

    static Node* getNextNode(Node *_node) {
        if (_node) {
            if (_node->R != nullptr) {
                return getMin(_node->R);
            }
            Node* par = _node->P;
            while (par != nullptr && _node == par->R) {
                _node = par;
                par = par->P;
            }
            return par;
        }
        return _node;
    }

    static Node* getMax(Node* _node) {
        if (_node == nullptr) {
            return nullptr;
        }
        Node* cur = _node;
        while (cur->R != nullptr) {
            cur = cur->R;
        }
        return cur;
    }

    static Node* getPrevNode(Node *cur, Node *_node) {
        if (_node == nullptr) {
            return getMax(cur);
        }
        if (_node->L != nullptr) {
            return getMax(_node->L);
        }
        Node* par = _node->P;
        while (par != nullptr && _node == par->L) {
            _node = par;
            par = par->P;
        }
        return par;
    }

    Node* right_rotate(Node* _node) {
        if (_node) {
            Node* _node_rev = _node->L;
            _node->L = _node_rev->R;
            _node_rev->R = _node;
            if (_node != nullptr) {
                if (_node->R) {
                    _node->R->P = _node;
                }
                if (_node->L) {
                    _node->L->P = _node;
                }
                _node->h = max(getHeight(_node->L), getHeight(_node->R)) + 1;
            }
            if (_node_rev != nullptr) {
                if (_node_rev->R) {
                    _node_rev->R->P = _node_rev;
                }
                if (_node_rev->L) {
                    _node_rev->L->P = _node_rev;
                }
                _node_rev->h = max(getHeight(_node_rev->L), getHeight(_node_rev->R)) + 1;
            }
            return _node_rev;
        }
        return nullptr;
    }

    Node* left_rotate(Node* _node) {
        if (!_node) return _node;
        Node* _node_rev = _node->R;
        _node->R = _node_rev->L;
        _node_rev->L = _node;
        if (_node != nullptr) {
            if (_node->R) {
                _node->R->P = _node;
            }
            if (_node->L) {
                _node->L->P = _node;
            }
            _node->h = max(getHeight(_node->L), getHeight(_node->R)) + 1;
        }
        if (_node_rev != nullptr) {
            if (_node_rev->R) {
                _node_rev->R->P = _node_rev;
            }
            if (_node_rev->L) {
                _node_rev->L->P = _node_rev;
            }
            _node_rev->h = max(getHeight(_node_rev->L), getHeight(_node_rev->R)) + 1;
        }
        return _node_rev;
    }

    Node* update(Node* _node) {
        if (_node != nullptr) {
            _node->h = max(getHeight(_node->L), getHeight(_node->R)) + 1;
            if (_node->R) {
                _node->R->P = _node;
            }
            if (_node->L) {
                _node->L->P = _node;
            }
            if (getHeight(_node->R) - getHeight(_node->L) == 2) {
                if (getHeight(_node->R->L) - getHeight(_node->R->R) > 0) {
                    _node->R = right_rotate(_node->R);
                }
                return left_rotate(_node);
            }
            if (getHeight(_node->R) - getHeight(_node->L) == -2) {
                if (getHeight(_node->L->L) - getHeight(_node->L->R) < 0) {
                    _node->L = left_rotate(_node->L);
                }
                return right_rotate(_node);
            }
        }
        return _node;
    }

    class iterator {
    private:
        Node *root;
        Node *_node;

    public:
        iterator() : root(nullptr), _node(nullptr) {}
        iterator(Node *_node, Node *root) : root(root), _node(_node) {}

        bool operator==(const iterator& diff) const {
            if (_node == diff._node) {
                return true;
            }
            return false;
        }
        bool operator!=(const iterator& diff) const {
            if (_node == diff._node) {
                return false;
            }
            return true;
        }

        iterator operator--() {
            _node = getPrevNode(root, _node);
            return *this;
        }
        const iterator operator--(int) {
            iterator __node = (*this);
            _node = getPrevNode(root, _node);
            return __node;
        }

        iterator operator++() {
            _node = getNextNode(_node);
            return *this;
        }
        const iterator operator++(int) {
            iterator __node = *this;
            _node = getNextNode(_node);
            return __node;
        }

        ValueType& operator*() {
            return (_node->key);
        }
        ValueType* operator->() {
            return &(_node->key);
        }
    };

    Set() : root(nullptr), sz(0) {}

    template<class InputIter>
    Set(InputIter _begin, InputIter _end) : Set() {
        while(_begin != _end) {
            insert(*_begin);
            ++_begin;
        }
    }

    Set(const initializer_list<ValueType>& lst) : Set() {
        sz = 0;
        for (auto x : lst) {
            insert(x);
        }
    }

    ~Set() {
        while (true) {
            if (root == nullptr) {
                break;
            }
            erase(root->key);
        }
        sz = 0;
        root = nullptr;
    }

    Set(const Set &_set) : sz(0), root(nullptr) {
        auto it = _set.begin();
        while (it != _set.end()) {
            insert(*it);
            ++it;
        }
    }

    Set& operator=(const Set& _node) {
        if (root == _node.root) {
            return *this;
        }
        while (true) {
            if (root == nullptr) {
                break;
            }
            erase(root->key);
        }
        sz = 0;
        root = nullptr;
        for (const auto& i : _node) {
            insert(i);
        }
        return (*this);
    }

    bool empty() const {
        return !sz;
    }

    size_t size() const {
        return sz;
    }

    iterator begin() const {
        if (root == nullptr) {
            return iterator(root, root);
        }
        return iterator(getMin(root), root);
    }

    iterator end() const {
        return iterator(nullptr, root);
    }

    Node* _insert(Node* _node, const ValueType& x) {
        if (_node) {
            if (_node->key < x) {
                _node->R = _insert(_node->R, x);
                return update(_node);
            }
            else if (x < _node->key) {
                _node->L = _insert(_node->L, x);
                return update(_node);
            }
        }
        return new Node(x);
    }

    void insert(const ValueType& x) {
        if (find(x) == end()) {
            root = _insert(root, x);
            root->P = nullptr;
            sz++;
        }
    }

    Node* __erase(Node* _node) {
        if (_node->L) {
            _node->L = __erase(_node->L);
            return update(_node);
        }
        return _node->R;
    }
    Node* _erase(Node* _node, const ValueType& _key) {
        if (!_node) return _node;
        if (_key < _node->key) _node->L = _erase(_node->L, _key);
        else if (_node->key < _key) _node->R = _erase(_node->R, _key);
        else {
            Node* left_son = _node->L;
            Node* right_son = _node->R;
            _node->L = nullptr, _node->R = nullptr;
            delete _node;
            if (right_son) {
                Node* _min = getMin(right_son);
                _min->R = __erase(right_son);
                _min->L = left_son;
                return update(_min);
            }
            return left_son;
        }
        return update(_node);
    }

    void erase(const ValueType& x) {
        if (find(x) != end()) {
            root = _erase(root, x);
            if (root) {
                root->P = nullptr;
            }
            sz--;
        }
    }

    Node* _lower_bound(Node* _node, const ValueType& key) const {
        if (_node) {
            if (_node->key < key) {
                return _lower_bound(_node->R, key);
            }
            else if (key < _node->key) {
                Node* __node = _lower_bound(_node->L, key);
                if (!__node) {
                    return _node;
                }
                return __node;
            }
        }
        return _node;
    }

    iterator find(const ValueType& _key) const {
        Node *_node = _lower_bound(root, _key);
        if (_node == nullptr) {
            return end();
        }
        if (_node->key < _key) {
            return end();
        }
        if (_key < _node->key) {
            return end();
        }
        return iterator(_node, root);
    }
    iterator lower_bound(const ValueType& elem) const {
        return iterator(_lower_bound(root, elem), root);
    }
};

