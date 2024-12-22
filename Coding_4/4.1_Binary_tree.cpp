# include <iostream>
# include <stack>
# include <queue>
using namespace std;

template <class T> class Tree;
template <class T> class InorderIterator;

template <class T> 
class TreeNode {
    friend class Tree<T>;
    friend class InorderIterator<T>;
    private: 
        T data;
        TreeNode<T> * leftChild;
        TreeNode<T> * rightChild;
};

template <class T> 
class Tree {
    friend class InorderIterator<T>;
    public: 
        Tree() { root = NULL; } //
        Tree(Tree<T>& bt1, T& item, Tree<T>& bt2); //
        Tree(const Tree<T>& t) { root = Copy(t.root); } //
        ~Tree() { /*FreeTree(root);*/ } //
        //void FreeTree(TreeNode<T> * p); //
        bool IsEmpty() { return (root == NULL); } //
        Tree<T> LeftSubtree(); //
        Tree<T> RightSubtree(); //
        T RootData() { return root->data; } //
        // requirement
        void Inorder() { Inorder(root); } //
        void Inorder(TreeNode<T> * cur); //
        void Preorder() { Preorder(root); } //
        void Preorder(TreeNode<T> * cur); //
        void Postorder() { Postorder(root); } //
        void Postorder(TreeNode<T> * cur); //
        void Levelorder(); //
        void NonrecInorder(); //
        void NonstackInorder(); //
        bool operator==(const Tree& t);
        TreeNode<T> * Copy(TreeNode<T> * p); //
        bool Equal(const Tree<T>& t) { return Equal(root, t.root); } //
        bool Equal(TreeNode<T>* a, TreeNode<T> * b); //
        void Setup1(); //
        void Setup2(); //
        void printBT(const string& prefix, const TreeNode<T> * node, bool isLeft); //
        void output() { printBT("", root, false); } //
    private:
        TreeNode<T> * root;
};

template <class T> 
class InorderIterator {
    public:
        InorderIterator() { currentNode = Tree<T>::root;}
        InorderIterator(Tree<T> tree): t(tree) { currentNode = t.root; }
        T * Next();
        T& operator*() { return currentNode->data; }
        bool operator!=(const InorderIterator r) { return currentNode != r.currentNode; }
    private:
        Tree<T> t;
        stack<TreeNode<T>*> s;
        TreeNode<T> * currentNode;
};

template <class T> 
T * InorderIterator<T>::Next()
{
    while (currentNode) {
        s.push(currentNode);
        currentNode = currentNode->leftChild;
    }
    if (s.empty()) return 0;
    currentNode = s.top(); s.pop();
    T& temp = currentNode->data;
    currentNode = currentNode->rightChild;
    return &temp;
}

template <class T> 
TreeNode<T> * Tree<T>::Copy(TreeNode<T> * p)
{
    if (p == NULL) return 0;
    TreeNode<T> * np = new TreeNode<T>;
    np->data = p->data;
    np->leftChild = p->rightChild;
    np->rightChild = p->rightChild;
    return np;
}

template <class T> 
Tree<T>::Tree(Tree<T>& bt1, T& item, Tree<T>& bt2)
{
    // left subtree bt1, right subtree bt2
    root = new TreeNode<T>;
    root->data = item;
    root->leftChild = bt1.root;
    root->rightChild = bt2.root;
}

/*template <class T> 
void Tree<T>::FreeTree(TreeNode<T> * p)
{
    if (p) {
        FreeTree(p->leftChild);
        FreeTree(p->rightChild);
        delete p;
    }
}*/

template <class T> 
Tree<T> Tree<T>::LeftSubtree()
{
    Tree<T> left;
    left.root = root->leftChild;
    return left;
}

template <class T> 
Tree<T> Tree<T>::RightSubtree()
{
    Tree<T> right;
    right.root = root->rightChild;
    return right;
}

template <class T> 
void Tree<T>::Inorder(TreeNode<T> * cur)
{
    if (cur) {
        Inorder(cur->leftChild);
        cout << cur->data;
        Inorder(cur->rightChild);
    }
}

template <class T> 
void Tree<T>::Preorder(TreeNode<T> * cur)
{
    if (cur) {
        cout << cur->data;
        Preorder(cur->leftChild);
        Preorder(cur->rightChild);
    }
}

template <class T> 
void Tree<T>::Postorder(TreeNode<T> * cur) 
{
    if (cur) {
        Postorder(cur->leftChild);
        Postorder(cur->rightChild);
        cout << cur->data;
    }
}

template <class T> 
void Tree<T>::Levelorder()
{
    // implemented by queue STL
    queue< TreeNode<T> * > q;
    TreeNode<T> * cur = root;
    while (cur) {
        cout << cur->data;
        if (cur->leftChild) q.push(cur->leftChild);
        if (cur->rightChild) q.push(cur->rightChild);
        if (q.empty()) return;
        cur = q.front(); q.pop();
    }
}

template <class T> 
void Tree<T>::NonrecInorder()
{
    // implemented by stack STL
    stack< TreeNode<T> * > s;
    TreeNode<T> * cur = root;
    while (1) {
        while (cur) {
            s.push(cur);
            cur = cur->leftChild;
        }
        if (s.empty()) return;
        cur = s.top(); s.pop();
        cout << cur->data;
        cur = cur->rightChild;
    }
}

template <class T> 
void Tree<T>::NonstackInorder()
{
    if (!root)
        return;
    TreeNode<T> *top = 0, *lastRight = 0, *p, *q, *r, *r1;
    p = q = root;
    while(1) {
        while(1) {
            if ((!p->leftChild) && (!p->rightChild)) { 
                cout << p->data; break;
            }
            if (!p->leftChild) {
                cout << p->data;
                r = p->rightChild;
                p->rightChild = q;
                q = p; p = r;
            }
            else {
                r = p->leftChild; p->leftChild = q;
                q = p; p = r;
            }
        }
        TreeNode<T> *av = p;
        while (1) {
            if (p == root)
                return;
            if (!q->leftChild) {
                    r = q->rightChild; q->rightChild = p;
                    p = q; q = r;
                }
            else if (!q->rightChild) {
                r = q->leftChild; q->leftChild = p;
                p = q; q = r;
                cout << p->data;
            }
            else {
                if (q == lastRight) {
                    r = top; lastRight = r->leftChild;
                    top = r->rightChild;
                    r->leftChild = r->rightChild = 0;
                    r = q->rightChild; q->rightChild = p;
                    p = q; q = r;
                }
                else {
                    cout << q->data;
                    av->leftChild = lastRight; av->rightChild = top;
                    top = av; lastRight = q;
                    r = q->leftChild; q->leftChild = p;
                    r1 = q->rightChild; q->rightChild = r;
                    p = r1;
                    break;
                }
            } 
        }
    }   
}

template <class T> 
bool Tree<T>::Equal(TreeNode<T> * a, TreeNode<T> * b)
{
    // workhorse
    if ((!a)&&(!b)) return true;
    return ( a && b && (a->data == b->data) && 
                Equal(a->leftChild, b->leftChild) &&
                Equal(a->rightChild, b->rightChild) );
}

template <class T>
void Tree<T>::Setup1()
{
    char ch[9] = {'A','B','-','C','*','D','*','E','+'};
    Tree<char> emptyTree;
    Tree<char> A(emptyTree, ch[0], emptyTree);
    Tree<char> B(emptyTree, ch[1], emptyTree);
    Tree<char> Minus(A, ch[2], B);
    Tree<char> C(emptyTree, ch[3], emptyTree);
    Tree<char> MUL1(Minus, ch[4], C);
    Tree<char> D(emptyTree, ch[5], emptyTree);
    Tree<char> MUL2(MUL1, ch[6], D);
    Tree<char> E(emptyTree, ch[7], emptyTree);
    Tree<char> ADD(MUL2, ch[8], E);
    root = ADD.root;
}

template <class T> 
void Tree<T>::Setup2()
{
    char ch[9] = {'H','J','D','E','B','F','G','C','A'};
    Tree<char> emptyTree;
    Tree<char> H(emptyTree, ch[0], emptyTree);
    Tree<char> J(emptyTree, ch[1], emptyTree);
    Tree<char> D(H, ch[2], J);
    Tree<char> E(emptyTree, ch[3], emptyTree);
    Tree<char> B(D, ch[4], E);
    Tree<char> F(emptyTree, ch[5], emptyTree);
    Tree<char> G(emptyTree, ch[6], emptyTree);
    Tree<char> C(F, ch[7], G);
    Tree<char> A(B, ch[8], C);
    root = A.root;
}

template <class T>
void Tree<T>::printBT(const string& prefix, const TreeNode<T> * node, bool isLeft)
{
    if( node != nullptr )
    {
        cout << prefix;
        cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        cout << node->data << endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->leftChild, true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->rightChild, false);
    }
}

int main()
{
    cout << "For the example binary tree (a):" << endl;
    Tree<char> A;
    A.Setup1();
    A.output();
    if (A.IsEmpty()) cout << "The BT is empty." << endl;
    else cout << "The BT is nonempty." << endl;
    Tree<char> left = A.LeftSubtree(), right = A.RightSubtree();
    cout << "The left subtree of (a): " << endl;
    left.output();
    cout << "The right subtree of (a): " << endl;
    right.output();
    cout << "The root data is: " << A.RootData() << endl;
    cout << "Inorder traversal: ";
    A.Inorder();
    cout << endl;
    cout << "Postorder traversal: ";
    A.Postorder();
    cout << endl;
    cout << "Preorder traversal: ";
    A.Preorder();
    cout << endl;
    cout << "Levelorder traversal: ";
    A.Levelorder();
    cout << endl;
    cout << "Non-recursive inorder traversal: ";
    A.NonrecInorder();
    cout << endl;
    cout << "Inorder traversal without using stack: ";
    A.NonstackInorder();
    cout << endl;

    cout << "For the example binary tree (b):" << endl;
    Tree<char> B;
    B.Setup2();
    B.output();
    if (B.IsEmpty()) cout << "The BT is empty." << endl;
    else cout << "The BT is nonempty." << endl;
    Tree<char> leftb = B.LeftSubtree(), rightb = B.RightSubtree();
    cout << "The left subtree of (b): " << endl;
    leftb.output();
    cout << "The right subtree of (b): " << endl;
    rightb.output();
    cout << "The root data is: " << B.RootData() << endl;
    cout << "Inorder traversal: ";
    B.Inorder();
    cout << endl;
    cout << "Postorder traversal: ";
    B.Postorder();
    cout << endl;
    cout << "Preorder traversal: ";
    B.Preorder();
    cout << endl;
    cout << "Levelorder traversal: ";
    B.Levelorder();
    cout << endl;
    cout << "Non-recursive inorder traversal: ";
    B.NonrecInorder();
    cout << endl;
    cout << "Inorder traversal without using stack: ";
    B.NonstackInorder();
    cout << endl;
    if (A.Equal(B)) cout << "BT(a) and BT(b) are equal." << endl;
    else cout << "BT(a) and BT(b) are not equal." << endl;

    return 0;
}