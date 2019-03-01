#pragma once

struct CycleDetector {
    struct Node {
        //T       value;
        Node*   next;
    };

    // returns (the starting node, index of the starting node)
    // if there is no cycle, then returns (nullptr, -1)
    static pair<Node*,int> findCycle(Node* head) {
        Node *p1 = head;
        Node *p2 = head;

        while (p2 && p2->next) {
            p1 = p1->next;
            p2 = p2->next->next;

            if (p1 == p2) {
                p1 = head;
                int index = 0;
                while (p1 != p2) {
                    p1 = p1->next;
                    p2 = p2->next;
                    index++;
                }
                return make_pair(p1, index);
            }
        }

        return make_pair(nullptr, -1);
    }
};
