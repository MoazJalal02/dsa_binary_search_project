#include <iostream>
#include <string>
using namespace std;

// =====================
// Tree Node Definition
// =====================
/* * Othman: 1. UPDATED NODE STRUCTURE
 * Adding fields for component marks, total marks, and grade.
 */
struct TreeNode
{
    string studname;

    TreeNode *left;
    TreeNode *right;

    // New fields for Part B
    double assignmentMark; // Component Mark 1
    double examMark;       // Component Mark 2
    double totalMark;      // Calculated Total
    char grade;            // Calculated Grade
};

// =====================
// TreeType Class
// =====================
class TreeType
{
public:
    TreeType();
    ~TreeType();

    bool IsEmpty() const;
    int NumberOfNodes() const;

    void RetrieveItem(string &item, bool &found);

    // Othman: Updated to accept marks for Part B
    void InsertItem(string item, double assign, double exam);

    void DeleteItem(string item);
    void PrintTree() const;

    // Othman: Public traversal wrappers
    void PrintInOrder() const;
    void PrintPreOrder() const;
    void PrintPostOrder() const;

private:
    TreeNode *root;

    // Helper functions
    void Destroy(TreeNode *&tree);
    int CountNodes(TreeNode *tree) const;

    // Othman:  New Prototype to matche new implementation
    void Insert(TreeNode *&tree, string name, double assign, double exam);

    void Retrieve(TreeNode *tree, string &item, bool &found) const;
    void Delete(TreeNode *&tree, string item);
    void DeleteNode(TreeNode *&tree);
    // SHIMAA : Change the fuction to Return full predecessor node
    void GetPredecessor(TreeNode *tree, TreeNode *&pred);

    // Legacy helper (prints name only)
    void PrintInOrderOnlyName(TreeNode *tree) const;

    // Othman: Part B Helpers
    void DisplayRecord(TreeNode *tree) const;
    void InOrder(TreeNode *tree) const;
    void PreOrder(TreeNode *tree) const;
    void PostOrder(TreeNode *tree) const;
};

// =====================
// Constructor / Destructor
// =====================
TreeType::TreeType()
{
    root = nullptr;
}

TreeType::~TreeType()
{
    Destroy(root);
}

// =====================
// Public Operations
// =====================
bool TreeType::IsEmpty() const
{
    return root == nullptr;
}

int TreeType::NumberOfNodes() const
{
    return CountNodes(root);
}

void TreeType::InsertItem(string item, double assign, double exam)
{
    Insert(root, item, assign, exam);
}

void TreeType::RetrieveItem(string &item, bool &found)
{
    Retrieve(root, item, found);
}

void TreeType::DeleteItem(string item)
{
    Delete(root, item);
}

void TreeType::PrintTree() const
{
    if (root == nullptr)
    {
        cout << "Tree is empty.\n";
        return;
    }
    PrintInOrderOnlyName(root);
    cout << endl;
}

// =====================
// Helper Implementations
// =====================
void TreeType::Destroy(TreeNode *&tree)
{
    if (tree != nullptr)
    {
        Destroy(tree->left);
        Destroy(tree->right);
        delete tree;
        tree = nullptr;
    }
}

int TreeType::CountNodes(TreeNode *tree) const
{
    if (tree == nullptr)
        return 0;
    return 1 + CountNodes(tree->left) + CountNodes(tree->right);
}

/* * Othman: 3. UPDATED INSERT LOGIC FOR PART B
 * Calculations for Total Mark and Grade must happen here.
 */
void TreeType::Insert(TreeNode *&tree, string name, double assign, double exam)
{
    if (tree == nullptr)
    {
        // Create new node with all record details
        tree = new TreeNode;
        tree->studname = name;
        tree->assignmentMark = assign;
        tree->examMark = exam;

        // Requirement: Compute Total Mark
        tree->totalMark = assign + exam;

        // Requirement: Compute Grade based on total
        if (tree->totalMark >= 80)
            tree->grade = 'A';
        else if (tree->totalMark >= 70)
            tree->grade = 'B';
        else if (tree->totalMark >= 60)
            tree->grade = 'C';
        else if (tree->totalMark >= 50)
            tree->grade = 'D';
        else
            tree->grade = 'F';

        tree->left = nullptr;
        tree->right = nullptr;
    }
    // Navigation logic (alphabetical)
    else if (name < tree->studname)
        Insert(tree->left, name, assign, exam);
    else if (name > tree->studname)
        Insert(tree->right, name, assign, exam);
    else
        cout << "Duplicate name not allowed: " << name << endl;
}

void TreeType::Retrieve(TreeNode *tree, string &item, bool &found) const
{
    if (tree == nullptr)
    {
        found = false;
        return;
    }
    if (item < tree->studname)
        Retrieve(tree->left, item, found);
    else if (item > tree->studname)
        Retrieve(tree->right, item, found);
    else
    {
        found = true;
        item = tree->studname;
    }
}

void TreeType::Delete(TreeNode *&tree, string item)
{
    if (tree == nullptr)
        return;
    if (item < tree->studname)
        Delete(tree->left, item);
    else if (item > tree->studname)
        Delete(tree->right, item);
    else
        DeleteNode(tree);
}

void TreeType::DeleteNode(TreeNode *&tree)
{
    TreeNode *temp = tree;
    // Case 1: leaf
    if (tree->left == nullptr && tree->right == nullptr)
    {
        delete tree;
        tree = nullptr;
    }
    // Case 2: one child (right)
    else if (tree->left == nullptr)
    {
        tree = tree->right;
        delete temp;
    }
    // Case 3: one child (left)
    else if (tree->right == nullptr)
    {
        tree = tree->left;
        delete temp;
    }
    // Case 4: two children

    else
    {
        TreeNode *predecessor;
        GetPredecessor(tree->left, predecessor);

        // SHIMAA :// Copy entire student record to keep name, marks, total, and grade consistent after deletion
        tree->studname = predecessor->studname;
        tree->assignmentMark = predecessor->assignmentMark;
        tree->examMark = predecessor->examMark;
        tree->totalMark = predecessor->totalMark;
        tree->grade = predecessor->grade;

        Delete(tree->left, predecessor->studname);
    }
}

// SHIMAA : Returns the full predecessor node so the entire student record can be copied during deletion

void TreeType::GetPredecessor(TreeNode *tree, TreeNode *&pred)
{
    while (tree->right != nullptr)
        tree = tree->right;
    pred = tree;
}

// Othman: Renamed to avoid conflict with the new Part B InOrder traversal
void TreeType::PrintInOrderOnlyName(TreeNode *tree) const
{
    if (tree != nullptr)
    {
        PrintInOrderOnlyName(tree->left);
        cout << tree->studname << " ";
        PrintInOrderOnlyName(tree->right);
    }
}

// =========================================================
// PART B: STUDENT RECORDS & TRAVERSALS "Othman"
// =========================================================

// Othman: Added Helper function to display a single record in the required format
void TreeType::DisplayRecord(TreeNode *tree) const
{
    if (tree == nullptr)
        return;
    cout << "-----------------------------------" << endl;
    cout << "Student Name: " << tree->studname << endl;
    cout << "Assignment:   " << tree->assignmentMark << endl;
    cout << "Exam Mark:    " << tree->examMark << endl;
    cout << "Total Mark:   " << tree->totalMark << endl;
    cout << "Grade:        " << tree->grade << endl;
}

// Othman Requirement: Inorder traversal (Sorted alphabetically by name)
void TreeType::InOrder(TreeNode *tree) const
{
    if (tree != nullptr)
    {
        InOrder(tree->left);
        DisplayRecord(tree); // Visit Root
        InOrder(tree->right);
    }
}

// Othman Requirement: Preorder traversal
void TreeType::PreOrder(TreeNode *tree) const
{
    if (tree != nullptr)
    {
        DisplayRecord(tree); // Visit Root
        PreOrder(tree->left);
        PreOrder(tree->right);
    }
}

// Othman Requirement: Postorder traversal
void TreeType::PostOrder(TreeNode *tree) const
{
    if (tree != nullptr)
    {
        PostOrder(tree->left);
        PostOrder(tree->right);
        DisplayRecord(tree); // Visit Root
    }
}

/* * Othman: Added Print functionalites for the pre, In and postOrder Traverse.
 */
void TreeType::PrintInOrder() const
{
    if (root == nullptr)
        cout << "Tree is empty." << endl;
    else
        InOrder(root);
}

void TreeType::PrintPreOrder() const
{
    if (root == nullptr)
        cout << "Tree is empty." << endl;
    else
        PreOrder(root);
}

void TreeType::PrintPostOrder() const
{
    if (root == nullptr)
        cout << "Tree is empty." << endl;
    else
        PostOrder(root);
}

// =====================
// Main (Testing)
// =====================
int main()
{
    TreeType bst;
    bool found;
    string name;

    // FIXED: Updated calls to include marks (Name, Assignment, Exam)
    bst.InsertItem("Ali", 30, 35);   // Total 65 (C)
    bst.InsertItem("Siti", 40, 45);  // Total 85 (A)
    bst.InsertItem("Muthu", 35, 40); // Total 75 (B)
    bst.InsertItem("Ahmad", 25, 25); // Total 50 (D)
    bst.InsertItem("John", 10, 20);  // Total 30 (F)

    // testing for part B: Othman
    cout << "=== INORDER TRAVERSAL (Sorted Alphabetically) ===" << endl;
    bst.PrintInOrder();

    cout << "\n=== PREORDER TRAVERSAL ===" << endl;
    bst.PrintPreOrder();

    cout << "\n=== POSTORDER TRAVERSAL ===" << endl;
    bst.PrintPostOrder();

    cout << "\n=== Original Simple Print ===" << endl;
    bst.PrintTree();
    // Othman:  end of part B
    cout << "Total nodes: " << bst.NumberOfNodes() << endl;

    name = "Muthu";
    bst.RetrieveItem(name, found);
    cout << "Search Muthu: " << (found ? "FOUND" : "NOT FOUND") << endl;
    // testing for part D : SHIMAA
    cout << "Deleting Ahmad...\n";
    bst.DeleteItem("Ahmad");

    cout << "After deletion: ";

    bst.PrintTree();
    // END OF PART D

    return 0;
}