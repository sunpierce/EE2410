#include <iostream>
using namespace std;

class DblList;
class DblListNode 
{
    friend ostream& operator<<(ostream&, DblList&);
    friend class DblList;
    DblListNode(int d, DblListNode * l, DblListNode * r) { data = d; left = l; right = r; }
    private:
        int data;
        DblListNode * left, * right;
};

class DblList 
{
    friend ostream& operator<<(ostream&, DblList&);
    public:  
        DblList();
        ~DblList();
        void Insert(DblListNode * p, DblListNode * x);
        void Delete(DblListNode * x);
        void PushFront(int x);
        void PushBack(int x);
        void PopFront();
        void PopBack();
        void Concatenate(DblList& l);
    private:  
        DblListNode * head; // points to header node
};

DblList::DblList() 
{
    DblListNode * header = new DblListNode(0,NULL,NULL);
    header->left = header->right = header;
    head = header;
}

DblList::~DblList() 
{
    DblListNode * del = head->right, * tmp;
    while (del != head) {
        tmp = del->right;
        delete del;
        del = tmp;
    }
    delete head;
}

void DblList::Insert(DblListNode * p, DblListNode * x)
{
    p->left = x;
    p->right = x->right;
    x->right->left = p;
    x->right = p;
}

void DblList::Delete(DblListNode * x)
{
    if (x == head) throw "Error";
    x->left->right = x->right;
    x->right->left = x->left;
    delete x;
}

void DblList::PushFront(int x)
{
    DblListNode * p = new DblListNode(x,head,head->right);
    head->right->left = p;
    head->right = p;
}

void DblList::PushBack(int x)
{
    DblListNode * p = new DblListNode(x, head->left, head);
    head->left->right = p;
    head->left = p;
}

void DblList::PopFront()
{
    if (head->right == head) return;
    DblListNode * del = head->right;
    head->right = del->right;
    del->right->left = head;
    delete del;
}

void DblList::PopBack()
{
    if (head->right == head) return;
    DblListNode * del = head->left, * prev = head->left->left;
    prev->right = head;
    head->left = prev;
    delete del;
}

void DblList::Concatenate(DblList& m)
{
    DblListNode * first = m.head->right, * last = m.head->left;
    last->right = head;
    first->left = head->left->right;
    head->left->right = first;
    head->left = last;

    DblListNode * header = new DblListNode(0,NULL,NULL);
    header->left = header->right = header;
    m.head = header;
}

ostream& operator<<(ostream& os, DblList& l)
{
    if (l.head->right == l.head) {
        os << "{NULL}" << endl;
        return os;
    }
    DblListNode * cur = l.head->right;
    os << "{";
    while (cur->right != l.head) {
        os << cur->data << ",";
        cur = cur->right;
    }
    os << cur->data;
    os << "}" << endl;
    return os;
}

int main()
{
    DblList L, L2;
    int input;
    cout << "Enter five integers to push to the front of the list: ";
    for (int i = 0; i < 5; i++) {
        cin >> input;
        L.PushFront(input);
    }
    cout << "L1: " << L;
    cout << "Enter five integers to push to the back of the list: ";
    for (int i = 0; i < 5; i++) {
        cin >> input;
        L.PushBack(input);
    }
    cout << "L1: " << L;
    cout << "How many element do you want to pop from the front of the list? ";
    cin >> input;
    while (input--) L.PopFront();
    cout << "L1: " << L;
    cout << "How many element do you want to pop from the back of the list? ";
    cin >> input;
    while (input--) L.PopBack();
    cout << "L1: " << L;
    cout << "Enter five integers to push to the back of list 2: ";
    for (int i = 0; i < 5; i++) {
        cin >> input;
        L2.PushBack(input);
    }
    cout << "L2: " << L2;
    L.Concatenate(L2);
    cout << "L1 concatenate with L2: " << L;
    cout << "L2: " << L2;

    return 0;
}