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

        if (root->val <= V) {
            auto r = splitBST(root->right, V);
            root->right = r[0];
            res[0] = root;
            res[1] = r[1];
        } else {
            auto l = splitBST(root->left, V);
            root->left = l[1];
            res[0] = l[0];
            res[1] = root;
        }

        return res;
    }
};