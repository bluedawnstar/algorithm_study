#include <vector>
#include <string>
#include <stack>
#include <memory>
#include <string>
#include <algorithm>

using namespace std;

template <typename T, size_t ChunkSizeN>
struct Allocator {
    struct ChunkT {
        int n;
        T values[ChunkSizeN];

        ChunkT() {
            n = 0;
        }
    };
    stack<shared_ptr<ChunkT>> mChunks;

    Allocator() {
        mChunks.push(shared_ptr<ChunkT>(new ChunkT()));
    }

    ~Allocator() {
        // no action
    }

    T* alloc() {
        if (mChunks.top()->n >= ChunkSizeN)
            mChunks.push(shared_ptr<ChunkT>(new ChunkT()));
        return &mChunks.top()->values[mChunks.top()->n++];
    }

    void clear() {
        stack<shared_ptr<ChunkT>>().swap(mChunks);
    }
};

struct SuffixTree {
    //TODO: check character count
    static const size_t MaxCharN = 26;
    //TODO: check conversion from character to value
    int ch2val(char ch) {
        return ch - 'a';
    }

    struct Node {
        struct Node *children[MaxCharN];

        //pointer to other node via suffix link
        struct Node* suffixLink;

        /*(start, end) interval specifies the edge, by which the
        node is connected to its parent node. Each edge will
        connect two nodes, one parent and one child, and
        (start, end) interval of a given edge  will be stored
        in the child node. Lets say there are two nods A and B
        connected by an edge with indices (5, 8) then this
        indices (5, 8) will be stored in node B. */
        int start;
        int* end;

        /*for leaf nodes, it stores the index of suffix for
        the path  from root to leaf*/
        int suffixIndex;

        int edgeLength() const {
            return *end - start + 1;
        }
    };
    Node* root;         //Pointer to root node

                        /*lastNewNode will point to newly created internal node,
                        waiting for it's suffix link to be set, which might get
                        a new suffix link (other than root) in next extension of
                        same phase. lastNewNode will be set to NULL when last
                        newly created internal node (if there is any) got it's
                        suffix link reset to new internal node created in next
                        extension of same phase. */
    Node* lastNewNode;
    Node* activeNode;

    /*activeEdge is represeted as input string character
    index (not the character itself)*/
    int activeEdge;
    int activeLength;

    // remainingSuffixCount tells how many suffixes yet to
    // be added in tree
    int  remainingSuffixCount;
    int  leafEnd;
    int* rootEnd;
    int* splitEnd;

    const char* text;   // text
    int  size;          // Length of input string

    Allocator<Node, 10> mNodeAllocator;
    Allocator<int, 100> mIntAllocator;

    Node* newNode(int start, int* end) {
        Node* node = mNodeAllocator.alloc();
        for (int i = 0; i < MaxCharN; i++)
            node->children[i] = nullptr;

        /*For root node, suffixLink will be set to NULL
        For internal nodes, suffixLink will be set to root
        by default in  current extension and may change in
        next extension*/
        node->suffixLink = root;
        node->start = start;
        node->end = end;

        /*suffixIndex will be set to -1 by default and
        actual suffix index will be set later for leaves
        at the end of all phases*/
        node->suffixIndex = -1;

        return node;
    }

    bool walkDown(Node* currNode) {
        /*activePoint change for walk down (APCFWD) using
        Skip/Count Trick  (Trick 1). If activeLength is greater
        than current edge length, set next  internal node as
        activeNode and adjust activeEdge and activeLength
        accordingly to represent same activePoint*/
        if (activeLength >= currNode->edgeLength()) {
            activeEdge += currNode->edgeLength();
            activeLength -= currNode->edgeLength();
            activeNode = currNode;
            return true;
        }
        return false;
    }

    void extendSuffixTree(int pos) {
        /*Extension Rule 1, this takes care of extending all
        leaves created so far in tree*/
        leafEnd = pos;

        /*Increment remainingSuffixCount indicating that a
        new suffix added to the list of suffixes yet to be
        added in tree*/
        remainingSuffixCount++;

        /*set lastNewNode to NULL while starting a new phase,
        indicating there is no internal node waiting for
        it's suffix link reset in current phase*/
        lastNewNode = nullptr;

        //Add all suffixes (yet to be added) one by one in tree
        while (remainingSuffixCount > 0) {
            if (activeLength == 0)
                activeEdge = pos; //APCFALZ

                                  // There is no outgoing edge starting with
                                  // activeEdge from activeNode
            if (activeNode->children[ch2val(text[activeEdge])] == nullptr) {
                //Extension Rule 2 (A new leaf edge gets created)
                activeNode->children[ch2val(text[activeEdge])] = newNode(pos, &leafEnd);

                /*A new leaf edge is created in above line starting
                from  an existng node (the current activeNode), and
                if there is any internal node waiting for it's suffix
                link get reset, point the suffix link from that last
                internal node to current activeNode. Then set lastNewNode
                to NULL indicating no more node waiting for suffix link
                reset.*/
                if (lastNewNode != nullptr) {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = nullptr;
                }
            }
            // There is an outgoing edge starting with activeEdge from activeNode
            else {
                // Get the next node at the end of edge starting
                // with activeEdge
                Node* next = activeNode->children[ch2val(text[activeEdge])];
                if (walkDown(next)) { //Do walkdown
                                      //Start from next node (the new activeNode)
                    continue;
                }

                /*Extension Rule 3 (current character being processed
                is already on the edge)*/
                if (text[next->start + activeLength] == text[pos]) {
                    //If a newly created node waiting for it's 
                    //suffix link to be set, then set suffix link 
                    //of that waiting node to curent active node
                    if (lastNewNode != nullptr && activeNode != root) {
                        lastNewNode->suffixLink = activeNode;
                        lastNewNode = nullptr;
                    }

                    //APCFER3
                    activeLength++;

                    /*STOP all further processing in this phase
                    and move on to next phase*/
                    break;
                }

                /*We will be here when activePoint is in middle of
                the edge being traversed and current character
                being processed is not on the edge (we fall off
                the tree). In this case, we add a new internal node
                and a new leaf edge going out of that new node. This
                is Extension Rule 2, where a new leaf edge and a new
                internal node get created*/
                splitEnd = mIntAllocator.alloc();
                *splitEnd = next->start + activeLength - 1;

                //New internal node
                Node* split = newNode(next->start, splitEnd);
                activeNode->children[ch2val(text[activeEdge])] = split;

                //New leaf coming out of new internal node
                split->children[ch2val(text[pos])] = newNode(pos, &leafEnd);
                next->start += activeLength;
                split->children[ch2val(text[next->start])] = next;

                /*We got a new internal node here. If there is any
                internal node created in last extensions of same
                phase which is still waiting for it's suffix link
                reset, do it now.*/
                if (lastNewNode != nullptr) {
                    /*suffixLink of lastNewNode points to current newly
                    created internal node*/
                    lastNewNode->suffixLink = split;
                }

                /*Make the current newly created internal node waiting
                for it's suffix link reset (which is pointing to root
                at present). If we come across any other internal node
                (existing or newly created) in next extension of same
                phase, when a new leaf edge gets added (i.e. when
                Extension Rule 2 applies is any of the next extension
                of same phase) at that point, suffixLink of this node
                will point to that internal node.*/
                lastNewNode = split;
            }

            /* One suffix got added in tree, decrement the count of
            suffixes yet to be added.*/
            remainingSuffixCount--;
            if (activeNode == root && activeLength > 0) { //APCFER2C1
                activeLength--;
                activeEdge = pos - remainingSuffixCount + 1;
            } else if (activeNode != root) { //APCFER2C2
                activeNode = activeNode->suffixLink;
            }
        }
    }

    //Print the suffix tree as well along with setting suffix index
    //So tree will be printed in DFS manner
    //Each edge along with it's suffix index will be printed
    void setSuffixIndexByDFS(Node* n, int labelHeight) {
        if (n == nullptr)
            return;

        if (n->start != -1) { //A non-root node
                              //Print the label on edge from parent to current node
                              //print(n->start, *(n->end));
        }
        bool leaf = true;
        for (int i = 0; i < MaxCharN; i++) {
            if (n->children[i] != nullptr) {
                //if (leaf == 1 && n->start != -1)
                //    printf(" [%d]\n", n->suffixIndex);

                //Current node is not a leaf as it has outgoing edges from it.
                leaf = false;
                setSuffixIndexByDFS(n->children[i], labelHeight + edgeLength(n->children[i]));
            }
        }
        if (leaf) {
            n->suffixIndex = size - labelHeight;
            //printf(" [%d]\n", n->suffixIndex);
        }
    }

    /*
    void freeSuffixTreeByPostOrder(Node *n) {
        if (n == nullptr)
            return;

        for (int i = 0; i < MaxCharN; i++) {
            if (n->children[i] != nullptr) {
                freeSuffixTreeByPostOrder(n->children[i]);
            }
        }
        if (n->suffixIndex == -1)
            delete n->end;
        delete n;
    }
    */

    //-------------------------------------------------------------------------

    SuffixTree() {
        root = nullptr;
        lastNewNode = nullptr;
        activeNode = nullptr;

        activeEdge = -1;
        activeLength = 0;

        remainingSuffixCount = 0;
        leafEnd = -1;
        rootEnd = nullptr;
        splitEnd = nullptr;

        text = nullptr;
        size = -1; //Length of input string
    }

    ~SuffixTree() {
        //freeSuffixTreeByPostOrder(root);
        mNodeAllocator.clear();
        mIntAllocator.clear();
    }

    int edgeLength(const Node* n) const {
        return n->edgeLength();
    }

    /*Build the suffix tree and print the edge labels along with
    suffixIndex. suffixIndex for leaf edges will be >= 0 and
    for non-leaf edges will be -1*/
    void build(const char* s, int n) {
        text = s;
        size = n;

        rootEnd = mIntAllocator.alloc();
        *rootEnd = -1;

        /*Root is a special node with start and end indices as -1,
        as it has no parent from where an edge comes to root*/
        root = newNode(-1, rootEnd);

        activeNode = root; //First activeNode will be root
        for (int i = 0; i < size; i++)
            extendSuffixTree(i);

        int labelHeight = 0;
        setSuffixIndexByDFS(root, labelHeight);
    }
};

long long countSubstrings(SuffixTree::Node* node) {
    if (!node)
        return 0;

    long long count = node->edgeLength();
    for (auto p : node->children) {
        if (p)
            count += countSubstrings(p);
    }

    return count;
}

long long countSubstrings(SuffixTree& tree) {
    return countSubstrings(tree.root) - 1;
}


/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

template <typename T>
vector<int> makeSuffixArrayFast(T s, int n);

template <typename T>
long long countSubstringsFast(const vector<int>& suffixArray, T s, int n);

void testSuffixTree() {
    return; //TODO: if you want to test string functions, make this line to a comment.

    cout << "-- makeSuffixTree() -------------" << endl;

    string S("abdadafaaabdfaeef");

    long long cnt1, cnt2;
    {
        vector<int> a2 = makeSuffixArrayFast(S, (int)S.length());
        cnt1 = countSubstringsFast(a2, S, (int)S.length());
    }
    {
        SuffixTree tree;
        tree.build(&S[0], (int)S.length());
        cnt2 = countSubstrings(tree);
    }

    assert(cnt1 == cnt2);

    cout << "OK! : countSubstrings(\"" << S << "\") = " << cnt2 << endl;
}
