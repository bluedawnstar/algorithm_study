/**
* Definition for a binary tree node.
* struct TreeNode {
*     int val;
*     TreeNode *left;
*     TreeNode *right;
*     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
* };
*/
class Solution {
public:
    vector<TreeNode*> splitBST(TreeNode* root, int V) {
        vector<TreeNode*> res{ nullptr, nullptr };
        if (root == nullptr)
            return res;

        TreeNode* parent = nullptr;
        TreeNode* curr = root;
        if (curr->val < V) {
            // detach bigger
            while (curr && V > curr->val) {
                parent = curr;
                curr = curr->right;
            }
            if (curr != nullptr) {
                if (parent)
                    parent->right = curr->left;
                else
                    root = curr->left;
                curr->left = nullptr;
            }
            res[0] = root;
            res[1] = curr;
        } else {
            // detach smaller
            while (curr && V < curr->val) {
                parent = curr;
                curr = curr->left;
            }
            if (curr != nullptr) {
                if (parent)
                    parent->left = curr->right;
                else
                    root = curr->right;
                curr->right = nullptr;
            }
            res[0] = curr;
            res[1] = root;
        }

        return res;
    }
};
