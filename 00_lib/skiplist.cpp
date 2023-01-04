//
// Created by kuzdavoj@fel.cvut.cz on 1/3/2023.
//


// A skip list is a data structure that allows for efficient search,
// insertion, and deletion of elements. It is a type of self-organizing
// list that uses a probabilistic data structure to maintain a sorted list of elements.
//
// In a skip list, each element is associated with a level in the list.
// The level of an element is chosen randomly with a probability that
// decreases as the level increases. This means that the higher the level
// of an element, the less likely it is to be chosen.
//
// The elements in the skip list are arranged in levels, with each level
// containing a subset of the elements in the list. The levels are linked
// together by pointers, which allow for efficient navigation through the list.
// To search for an element in a skip list, the search starts at the highest level
// and follows the pointers until it reaches the correct level to find the element.
// This allows the search to skip over large numbers of elements, which can greatly improve the performance of the search.
//
// Skip lists are similar to balanced tree data structures, such as red-black trees
// , but they are simpler to implement and can be more efficient in some cases.



#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    Node *right, *down;
    int val;
    Node(Node *right, Node *down, int val): right(right), down(down), val(val) {}
};

///https://leetcode.com/problems/design-skiplist/solutions/400028/c-skiplist-2-pointer-for-each-node-64ms/

class Skiplist {
    Node* head;

public:
//    Skiplist() {}

    Skiplist() { head = new Node(NULL, NULL, 0); }

    bool search(int num) {
        Node *p = head;
        while(p) {
            while(p->right and p->right->val < num) p = p->right;
            if(!p->right or p->right->val > num) p = p->down;
            else return true;
        }
        return false;
    }

    void add(int num) {
        insertPoints.clear();
        Node *p = head;
        while(p) {
            while(p->right and p->right->val < num) p = p->right;
            insertPoints.push_back(p);
            p = p->down;
        }

        Node* downNode = NULL;
        bool insertUp = true;
        while(insertUp and insertPoints.size()) {
            Node *ins = insertPoints.back();
            insertPoints.pop_back();

            ins->right = new Node(ins->right, downNode, num);
            downNode = ins->right;

            insertUp = (rand() & 1) == 0;
        }
        if(insertUp) {
            head = new Node(new Node(NULL, downNode, num), head, 0);
        }
    }
    vector<Node*> insertPoints;

    bool erase(int num) {
        Node *p = head;
        bool seen = false;
        while(p) {
            while(p->right and p->right->val < num) p = p->right;
            if(!p->right or p->right->val > num) p = p->down;
            else {
                seen = true;
                p->right = p->right->right;
                p = p->down;
            }
        }
        return seen;
    }
};
int main() {
    Skiplist sl;
    sl.add(1);
}
