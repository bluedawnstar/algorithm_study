/**
* Definition for singly-linked list.
* struct ListNode {
*     int val;
*     ListNode *next;
*     ListNode(int x) : val(x), next(NULL) {}
* };
*/

class Solution {
public:
    typedef pair<int, ListNode*> PT;

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        ListNode *head, *tail, dummy(0);
        head = tail = &dummy;

        priority_queue<PT, vector<PT>, greater<PT>> Q;
        for (int i = 0; i < (int)lists.size(); i++) {
            if (lists[i])
                Q.emplace(lists[i]->val, lists[i]);
        }

        while (!Q.empty()) {
            auto p = Q.top();
            Q.pop();

            tail->next = p.second;
            tail = tail->next;

            if (p.second->next)
                Q.emplace(p.second->next->val, p.second->next);
        }

        return head->next;
    }
};