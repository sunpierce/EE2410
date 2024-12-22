# include <iostream>
using namespace std;

template <class T> 
class ThreadedBT;

template <class T> 
class InorderIterator;

template <class T> 
class ThreadedNode
{
    friend class ThreadedBT<T>;
    friend void SetUp(ThreadedBT<char>&);
    public: 
        ThreadedNode(T e, ThreadedNode<T> * left = 0, ThreadedNode<T> * right = 0, bool leftT = 0, bool rightT = 0) 
        {
            data = e;
            leftChild = left;
            rightChild = right;
            leftThread = leftT;
            rightThread = rightT;
        }
        void SetThread(ThreadedNode<T> * left, ThreadedNode<T> * right) 
        {
            leftChild = left;
            rightChild = right;
            if (left) leftThread = 1;
            if (right) rightThread = 1;
        }
    private: 
        T data;
        ThreadedNode<T> * leftChild;
        ThreadedNode<T> * rightChild;
        bool leftThread;
        bool rightThread;
};

template <class T> 
class ThreadedBT
{
    friend void SetUp(ThreadedBT<char>&);
    public: 
        ThreadedBT() { head = 0; size = 0; }
        void Inorder();
        void Postorder();
        void Preorder();
        void Postorder(ThreadedNode<T> * cur);
        void Preorder(ThreadedNode<T> * cur);
        void InsertRight(ThreadedNode<T> * root, ThreadedNode<T> * right);
        void InsertLeft(ThreadedNode<T> * root, ThreadedNode<T> * left);
        ThreadedNode<T> * GetNode(T);
        // nested class
        class InorderIterator 
        {
            friend class ThreadedBT<T>;
            public: 
                InorderIterator(ThreadedNode<T> * p = 0) { current = p; }
                T * Next()
                {
                    ThreadedNode<T> * temp = current->rightChild;
                    if (current->rightThread) {
                        current = temp;
                    } else {
                        while (!temp->leftThread) temp = temp->leftChild;
                        current = temp;
                    }
                    if (current) return &current->data;
                    else return 0;
                }
            private: 
                ThreadedNode<T> * current;
        };
        InorderIterator Begin() { return InorderIterator(head); }
    private: 
        ThreadedNode<T> * head;
        int size;
};

template <class T> 
void ThreadedBT<T>::Inorder()
{
    InorderIterator it = Begin();
    T * tmp;
    for (int i = 0; i < size; i++) {
        tmp = it.Next();
        if (tmp && it.current != head) cout << *tmp;
    }
    cout << endl;
}

template <class T>
void ThreadedBT<T>::Preorder()
{
    Preorder(head->leftChild);
    cout << endl;
}

template <class T> 
void ThreadedBT<T>::Preorder(ThreadedNode<T> * cur)
{
    cout << cur->data;
    if (!cur->leftThread) Preorder(cur->leftChild);
    if (!cur->rightThread) Preorder(cur->rightChild);
}

template <class T> 
void ThreadedBT<T>::Postorder()
{
    Postorder(head->leftChild);
    cout << endl;
}

template <class T> 
void ThreadedBT<T>::Postorder(ThreadedNode<T> * cur)
{
        if (!cur->leftThread) Postorder(cur->leftChild);
        if (!cur->rightThread) Postorder(cur->rightChild);
        cout << cur->data;
}

template <class T> 
void ThreadedBT<T>::InsertRight(ThreadedNode<T> * root, ThreadedNode<T> * right)
{
    right->rightChild = root->rightChild;
    right->rightThread = root->rightThread;

    right->leftChild = root;
    right->leftThread = true;

    root->rightChild = right;
    root->rightThread = false;

    if (! right->rightThread) {
        ThreadedNode<T> * temp = next(right);
        temp->leftChild = right;
    }
    size++;
}

template <class T> 
void ThreadedBT<T>::InsertLeft(ThreadedNode<T> * root, ThreadedNode<T> * left)
{
    left->leftChild = root->leftChild;
    left->leftThread = root->leftThread;

    left->rightChild = root;
    left->rightThread = true;

    root->leftChild = left;
    root->leftThread = false;

    if (!left->leftThread) {
        ThreadedNode<T> * temp = left->leftChild;
        while (temp->rightChild != root) temp = temp->rightChild;
        temp->rightChild = left;
    }
    size++;
}

template <class T>
ThreadedNode<T> * ThreadedBT<T>::GetNode(T t)
{
    InorderIterator it = Begin();
    int ct = 0;
    while ( (*(it.Next()) != t ) && (++ct <= size) );
    if(it.current->data == t) return it.current;
    return 0;
}

void SetUp(ThreadedBT<char>& T)
{
    ThreadedNode<char> * J = new ThreadedNode<char>('J');
    ThreadedNode<char> * H = new ThreadedNode<char>('H');
    ThreadedNode<char> * G = new ThreadedNode<char>('G');
    ThreadedNode<char> * F = new ThreadedNode<char>('F');
    ThreadedNode<char> * E = new ThreadedNode<char>('E');
    ThreadedNode<char> * D = new ThreadedNode<char>('D',H,J);
    ThreadedNode<char> * C = new ThreadedNode<char>('C',F,G);
    ThreadedNode<char> * B = new ThreadedNode<char>('B',D,E);
    ThreadedNode<char> * A = new ThreadedNode<char>('A',B,C);
    T.head = new ThreadedNode<char>('-',A);
    T.head->rightChild = T.head;
    E->SetThread(B,A);
    F->SetThread(A,C);
    G->SetThread(C,T.head);
    H->SetThread(T.head,D);
    J->SetThread(D,B);
    T.size = 9;
}

int main()
{
    char in;
    ThreadedBT<char> T;
    ThreadedNode<char> * R, * temp, * L;
    SetUp(T);
    cout << "(a) Forward iterator by sequencing through the nodes in inorder: ";
    T.Inorder();
    cout << "(b) Traverse a threaded binary tree in postorder: ";
    T.Postorder();
    cout << "(c) Traverse a threaded binary tree in preorder: ";
    T.Preorder();
    cout << "(d) Suppose we want to insert a new node r as the right child of node s in a threaded binary tree." << endl;
    cout << "Enter the data of the node s: ";
    cin >> in;
    temp = T.GetNode(in);
    cout << "Enter the data of the node r: ";
    cin >> in;
    R = new ThreadedNode<char>(in);
    T.InsertRight(temp,R);
    cout << "The inorder traversal of the new tree: ";
    T.Inorder();
    cout << "(e) Suppose we want to insert a new node l as the left child of node s in a threaded binary tree." << endl;
    cout << "Enter the data of the node s: ";
    cin >> in;
    temp = T.GetNode(in);
    cout << "Enter the data of the node l: ";
    cin >> in;
    L = new ThreadedNode<char>(in);
    T.InsertLeft(temp,L);
    cout << "The inorder traversal of the new tree: ";
    T.Inorder();

    return 0;
}