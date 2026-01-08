#include <iostream>
#include <string>
using namespace std;

// =====================
// Tree Node Definition
// =====================
struct TreeNode {
    string studname;
    TreeNode* left;
    TreeNode* right;
};

// =====================
// TreeType Class
// =====================
class TreeType {
public:
    TreeType();
    ~TreeType();

    bool IsEmpty() const;
    int NumberOfNodes() const;

    void RetrieveItem(string& item, bool& found);
    void InsertItem(string item);
    void DeleteItem(string item);
    void PrintTree() const;

private:
    TreeNode* root;

    // Helper functions
    void Destroy(TreeNode*& tree);
    int CountNodes(TreeNode* tree) const;

    void Insert(TreeNode*& tree, string item);
    void Retrieve(TreeNode* tree, string& item, bool& found) const;
    void Delete(TreeNode*& tree, string item);

    void DeleteNode(TreeNode*& tree);
    void GetPredecessor(TreeNode* tree, string& data);

    void PrintInOrder(TreeNode* tree) const;
};

// =====================
// Constructor / Destructor
// =====================
TreeType::TreeType() {
    root = nullptr;
}

TreeType::~TreeType() {
    Destroy(root);
}

// =====================
// Public Operations
// =====================
bool TreeType::IsEmpty() const {
    return root == nullptr;
}

int TreeType::NumberOfNodes() const {
    return CountNodes(root);
}

void TreeType::InsertItem(string item) {
    Insert(root, item);
}

void TreeType::RetrieveItem(string& item, bool& found) {
    Retrieve(root, item, found);
}

void TreeType::DeleteItem(string item) {
    Delete(root, item);
}

void TreeType::PrintTree() const {
    if (root == nullptr) {
        cout << "Tree is empty.\n";
        return;
    }
    PrintInOrder(root);
    cout << endl;
}

// =====================
// Helper Implementations
// =====================
void TreeType::Destroy(TreeNode*& tree) {
    if (tree != nullptr) {
        Destroy(tree->left);
        Destroy(tree->right);
        delete tree;
        tree = nullptr;
    }
}

int TreeType::CountNodes(TreeNode* tree) const {
    if (tree == nullptr)
        return 0;
    return 1 + CountNodes(tree->left) + CountNodes(tree->right);
}

void TreeType::Insert(TreeNode*& tree, string item) {
    if (tree == nullptr) {
        tree = new TreeNode{item, nullptr, nullptr};
        return;
    }

    if (item < tree->studname)
        Insert(tree->left, item);
    else if (item > tree->studname)
        Insert(tree->right, item);
    else
        cout << "Duplicate name not allowed: " << item << endl;
}

void TreeType::Retrieve(TreeNode* tree, string& item, bool& found) const {
    if (tree == nullptr) {
        found = false;
        return;
    }

    if (item < tree->studname)
        Retrieve(tree->left, item, found);
    else if (item > tree->studname)
        Retrieve(tree->right, item, found);
    else {
        found = true;
        item = tree->studname;
    }
}

void TreeType::Delete(TreeNode*& tree, string item) {
    if (tree == nullptr)
        return;

    if (item < tree->studname)
        Delete(tree->left, item);
    else if (item > tree->studname)
        Delete(tree->right, item);
    else
        DeleteNode(tree);
}

void TreeType::DeleteNode(TreeNode*& tree) {
    TreeNode* temp = tree;

    // Case 1: leaf
    if (tree->left == nullptr && tree->right == nullptr) {
        delete tree;
        tree = nullptr;
    }
    // Case 2: one child (right)
    else if (tree->left == nullptr) {
        tree = tree->right;
        delete temp;
    }
    // Case 3: one child (left)
    else if (tree->right == nullptr) {
        tree = tree->left;
        delete temp;
    }
    // Case 4: two children
    else {
        string predecessor;
        GetPredecessor(tree->left, predecessor);
        tree->studname = predecessor;
        Delete(tree->left, predecessor);
    }
}

void TreeType::GetPredecessor(TreeNode* tree, string& data) {
    while (tree->right != nullptr)
        tree = tree->right;
    data = tree->studname;
}

void TreeType::PrintInOrder(TreeNode* tree) const {
    if (tree != nullptr) {
        PrintInOrder(tree->left);
        cout << tree->studname << " ";
        PrintInOrder(tree->right);
    }
}

// =====================
// Main (Testing)
// =====================
int main() {
    TreeType bst;
    bool found;
    string name;

    bst.InsertItem("Ali");
    bst.InsertItem("Siti");
    bst.InsertItem("Muthu");
    bst.InsertItem("Ahmad");
    bst.InsertItem("John");

    cout << "Inorder Traversal (sorted): ";
    bst.PrintTree();

    cout << "Total nodes: " << bst.NumberOfNodes() << endl;

    name = "Muthu";
    bst.RetrieveItem(name, found);
    cout << "Search Muthu: " << (found ? "FOUND" : "NOT FOUND") << endl;

    cout << "Deleting Ahmad...\n";
    bst.DeleteItem("Ahmad");

    cout << "After deletion: ";
    bst.PrintTree();

    return 0;
}