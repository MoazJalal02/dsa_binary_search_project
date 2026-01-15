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
    double totalMark;      // Calculated Total
    char grade;            // Calculated Grade
    
    // Add missing assessment fields
    double quiz;
    double assignment;
    double test1;
    double labTest;
    double project;
    double exercise;
    double finalExam;
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

    // MOAZ - Part A: Insert node
    void InsertItem(string studname, double quiz, double assign, double test1, double lab, double proj, double exc, double exam);

    
    void PrintTree() const;
    
    // Othman: Public traversal wrappers
    void PrintInOrder() const;
    void PrintPreOrder() const;
    void PrintPostOrder() const;
    
    // MOAZ - Part C : Search Student by name
    void SearchStudent(string name) const;
    
    // SHIMAA - Part D: Delete node
    void DeleteItem(string studname);

    // Mathaba - Part E : Statistics
    void PrintStatistics() const;


private:
    TreeNode *root;

    // Helper functions
    void Destroy(TreeNode *&tree);
    int CountNodes(TreeNode *tree) const;

    // MOAZ - Part A:  Insert node
    void Insert(TreeNode *&tree, string name, double quiz, double assign, double test1, double lab, double proj, double exc, double exam);

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

    // MOAZ - Part C: helper function for searching
    void Search(TreeNode* tree, string name) const; 

    // Mathaba: Part E helper struct to hold stats
    struct Stats
    {
        int count;
        double sum;

        // highest and lowest
        double maxTotal;
        string maxName;
        double minTotal;
        string minName;

        // grade distribution
        int countA, countB, countC, countD, countF;

        // top 3 trackers
        double top1, top2, top3;
        string name1, name2, name3;

        Stats()
        {
            count = 0;
            sum = 0.0;

            maxTotal = -1e18;
            maxName = "";

            minTotal = 1e18;
            minName = "";

            countA = countB = countC = countD = countF = 0;

            top1 = top2 = top3 = -1e18;
            name1 = name2 = name3 = "";
        }
    };

    void CollectStats(TreeNode *tree, Stats &s) const;
    void UpdateTop3(Stats &s, const string &name, double total) const;

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

void TreeType:: InsertItem(string studentName, double quiz, double assign, double test1, double lab, double proj, double exc, double exam)
{
    Insert(root, studentName, quiz, assign, test1, lab, proj, exc, exam);
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

/* MOAZ - Part A: */
void TreeType::Insert(TreeNode *&tree, string name, double quiz, double assign, double test1, double lab, double proj, double exc, double exam)
{
    if (tree == nullptr)
    {
        tree = new TreeNode;
        tree->studname = name;
        
        // Save all marks
        tree->quiz = quiz;
        tree->assignment = assign;
        tree->test1 = test1;
        tree->labTest = lab;
        tree->project = proj;
        tree->exercise = exc;
        tree->finalExam = exam;

        // Calculate Total correctly (Sum of all components)
        tree->totalMark = quiz + assign + test1 + lab + proj + exc + exam;

        // Grade Logic (Same as before)
        if (tree->totalMark >= 80) tree->grade = 'A';
        else if (tree->totalMark >= 65) tree->grade = 'B';
        else if (tree->totalMark >= 50) tree->grade = 'C';
        else if (tree->totalMark >= 40) tree->grade = 'D';
        else tree->grade = 'F';

        tree->left = nullptr;
        tree->right = nullptr;
    }
    // Navigation logic (alphabetical)
    else if (name <= tree->studname)
        Insert(tree->left, name, quiz, assign, test1, lab, proj, exc, exam);
    else 
        Insert(tree->right, name, quiz, assign, test1, lab, proj, exc, exam);
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
        tree->assignment = predecessor->assignment;
        tree->finalExam = predecessor->finalExam;
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
    cout << "Assignment:   " << tree->assignment << endl;
    cout << "Exam Mark:    " << tree->finalExam << endl;
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

// =====================================
// MOAZ - PART C: SEARCH STUDENT BY NAME
// =====================================

void TreeType::SearchStudent(string name) const {
    Search(root, name);
}

void TreeType::Search(TreeNode* tree, string name) const {
    if (tree == nullptr) {
        cout << "Student '" << name << "' was not found in the records." << endl;
        return;
    }

    if (name == tree->studname) {
        cout << "\n=== RECORD FOUND ===" << endl;
        DisplayRecord(tree); 
        cout << "====================" << endl;
    }
    else if (name < tree->studname) {
        Search(tree->left, name);
    }
    else {
        Search(tree->right, name);
    }
}

// =====================
// MATHABA: PART E STATISTICS
// =====================
// Requirement check (Part E):
// Must compute stats using tree traversal
// Need: class average, highest+lowest, grade distribution, and top 3 by total mark
// I'm using a simple traversal to visit every student once and update the stats as I go
// This keeps it clean and avoids needing extra storage

void TreeType::UpdateTop3(Stats &s, const string &name, double total) const
{
    // Keep track of the top 3 totals by shifting values when we find a higher score
    if (total > s.top1)
    {
        s.top3 = s.top2; s.name3 = s.name2;
        s.top2 = s.top1; s.name2 = s.name1;
        s.top1 = total;  s.name1 = name;
    }
    else if (total > s.top2)
    {
        s.top3 = s.top2; s.name3 = s.name2;
        s.top2 = total;  s.name2 = name;
    }
    else if (total > s.top3)
    {
        s.top3 = total;  s.name3 = name;
    }
}

void TreeType::CollectStats(TreeNode *tree, Stats &s) const
{
    if (tree == nullptr) return;

    // The order doesn't matter for statistics, but inorder keeps the traversal consistent
    CollectStats(tree->left, s);

    // Update all running statistics for the current student
    s.count++;
    s.sum += tree->totalMark;

    // Check if this student has the highest total so far
    if (tree->totalMark > s.maxTotal)
    {
        s.maxTotal = tree->totalMark;
        s.maxName = tree->studname;
    }

    // Check if this student has the lowest total so far
    if (tree->totalMark < s.minTotal)
    {
        s.minTotal = tree->totalMark;
        s.minName = tree->studname;
    }

    // Count how many students fall into each grade
    if (tree->grade == 'A') s.countA++;
    else if (tree->grade == 'B') s.countB++;
    else if (tree->grade == 'C') s.countC++;
    else if (tree->grade == 'D') s.countD++;
    else s.countF++;

    // Update the top 3 list
    UpdateTop3(s, tree->studname, tree->totalMark);

    CollectStats(tree->right, s);
}

void TreeType::PrintStatistics() const
{
    // Print message for user if tree is empty
    if (root == nullptr)
    {
        cout << "Tree is empty. No statistics available.\n";
        return;
    }

    // This struct stores everything we want to compute while traversing
    Stats s;
    CollectStats(root, s);

    // Average is just sum divided by number of students
    double average = (s.count == 0) ? 0.0 : (s.sum / s.count);

    cout << "\n========== CLASS STATISTICS ==========\n";
    cout << "Total Students: " << s.count << endl;
    cout << "Class Average Total Mark: " << average << endl;

    cout << "\nHighest Total Mark:\n";
    cout << "Name: " << s.maxName << " | Total: " << s.maxTotal << endl;

    cout << "\nLowest Total Mark:\n";
    cout << "Name: " << s.minName << " | Total: " << s.minTotal << endl;

    cout << "\nGrade Distribution:\n";
    cout << "A: " << s.countA << endl;
    cout << "B: " << s.countB << endl;
    cout << "C: " << s.countC << endl;
    cout << "D: " << s.countD << endl;
    cout << "F: " << s.countF << endl;

    cout << "\nTop 3 Students By Total Mark:\n";
    if (s.name1 != "") cout << "1) " << s.name1 << " | " << s.top1 << endl;
    if (s.name2 != "") cout << "2) " << s.name2 << " | " << s.top2 << endl;
    if (s.name3 != "") cout << "3) " << s.name3 << " | " << s.top3 << endl;

    cout << "======================================\n";
}


// =====================
// Main (Testing)
// =====================
int main()
{
    TreeType bst;
    bool found;
    string name;

    // Node insertion for testing
    bst.InsertItem("Ali", 6.5, 6.5, 13.0, 10.0, 6.5, 3.0, 19.5); // Total ~65 (B)
    bst.InsertItem("Siti", 8.5, 8.5, 17.0, 13.0, 8.5, 4.0, 25.5); // Total ~85 (A)
    bst.InsertItem("Muthu", 7.5, 7.5, 15.0, 11.0, 7.5, 3.5, 23.0); // Total ~75 (B)
    bst.InsertItem("Ahmad", 5.0, 5.0, 10.0, 7.5, 5.0, 2.5, 15.0); // Total ~50 (C)
    bst.InsertItem("John", 3.0, 3.0, 6.0, 4.5, 3.0, 1.5, 9.0); // Total ~30 (F)
    
    // Part E START
    // Mathaba: Part E: statistics after insertion
    // Print stats after inserting students so we can see the summary clearly
    cout << "\n=== CLASS STATISTICS (BEFORE DELETION) ===" << endl;
    bst.PrintStatistics();
    // Part E end

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

    // MOAZ - PART C: 
    name = "Muthu";
    bst.SearchStudent(name);
    // testing for part D : SHIMAA
    cout << "Deleting Ahmad...\n";
    bst.DeleteItem("Ahmad");

    cout << "After deletion: ";

    bst.PrintTree();
    
    // END OF PART D

    // Mathaba: Part E START
    cout << "\n=== CLASS STATISTICS (AFTER DELETION) ===" << endl;
    bst.PrintStatistics();
    // Mathaba Part E END

    return 0;
}