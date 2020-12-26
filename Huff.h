#pragma once

template<typename E> // Leaf node subclass
class HuffNode{
private:
    E it ; // Value
    int wgt ; // Weight
    HuffNode<E>* lc = NULL; // Left child
    HuffNode<E>* rc = NULL; // Right child
public:

    


    HuffNode() {
        it = NULL;
        wgt = 0;
    }

    HuffNode(const E& val, int freq) {
        it = val;
        wgt = freq;
    }

    void setWgt(int p) {
        wgt = p;
    }

    void setValue(E it) {
        this->it = it;
    }

    int weight() {
        return wgt;
    }

    E val() {
        return it;
    }

    bool isLeaf() {
        if (lc==NULL&&rc==NULL)
        {
            return true;
        }
        else {
            return false;
        }
       
    }

    HuffNode<E>* left() const {
        return lc;
    }

    void setLeft(HuffNode<E>* b) {
        lc = (HuffNode<E>*) b;
    }

    HuffNode<E>* right() const {
        return rc;
    }

    void setRight(HuffNode<E>* b) {
        rc = (HuffNode<E>*) b;
    }
};



template<typename E>
class HuffTree {
private:
    HuffNode<E>* Root; // Tree root
public:
    HuffTree() { // Leaf constructor
        Root = NULL;
    }

    HuffTree(HuffNode<char>* huff ) { // Leaf constructor
        Root = huff;
    }

    ~HuffTree() = default; // Destructor
    HuffNode<E>* root() {
        return Root;
    } // Get root
    int weight() {
        return Root->weight();
    } // Root weight
    void print() {}
};

class Cmp
{
public:
    bool operator() (HuffNode<char> *a, HuffNode<char> *b)
    {
        return a->weight() > b->weight();    // 从小到大 ，freq 小的 优先级别高 
    }
};
