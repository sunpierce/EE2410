#include <iostream>
using namespace std;

template <class T> 
class CircularList;

template <class T> 
class Node {
    public:
        friend class CircularList<T>;
        template <class U>
        friend ostream& operator<<(ostream& os, CircularList<U>& L);
        Node(T e, Node<T> * r) { data = e; link = r; }
    private:
        T data;
        Node<T> * link;
};

template <class T> 
class CircularList {
    template <class U> 
    friend ostream& operator<<(ostream&  os, CircularList<U>& L);
    public: 
        CircularList() { first = NULL; } // constructor
        ~CircularList();
        int count(); // (a)
        void InsertFront(T e); // (b)
        void InsertBack(T e);  // (c)
        void DeleteFirst(); // (d)
        void DeleteBack(); // (e)
        void DeleteOther(); // (f)
        void Deconcatenate(CircularList<T>& sublist, Node<T> * p); // (g)
        Node<T> * GetFourth(); // for (g) 
        void Merge(CircularList<T>& b); // (h)
    private: 
        Node<T> * first;
};

template <class T>
CircularList<T>::~CircularList()
{
    if (first) {
        Node<T> * del = first->link;
        while (del != first) {
            Node<T> * temp = del->link;
            delete del;
            del = temp;
        }
        delete first;
        first = NULL;
    }
}

template <class T> 
int CircularList<T>::count()
{
    if (first == NULL) return 0;
    Node<T> * temp = first->link;
    int ct = 1;
    while (temp != first) {
        ct++;
        temp = temp->link;
    }
    return ct;
}

template <class T> 
void CircularList<T>::InsertFront(T e)
{
    if (first == NULL) {
        first = new Node<T>(e,NULL);
        first->link = first;
    } else {
        Node<T> * last = first;
        while (last->link != first) {
            last = last->link;
        }
        last->link = first = new Node<T>(e, first);
    }
}

template <class T> 
void CircularList<T>::InsertBack(T e)
{
    if (first == NULL) {
        first = new Node<T>(e,NULL);
        first->link = first;
    } else {
        Node<T> * last = first;
        while (last->link != first) {
            last = last->link;
        }
        last = last->link = new Node<T>(e,first);
    }
}

template <class T> 
void CircularList<T>::DeleteFirst() 
{
    if (first == NULL) return;
    if (first->link == first) {
        delete first;
        first = NULL;
        return;
    }
    Node<T> * del = first, * last = first;
    while (last->link != first) {
        last = last->link;
    }
    first = last->link = del->link;
    delete del;
}

template <class T> 
void CircularList<T>::DeleteBack() 
{
    if (first == NULL) return;
    if (first->link == first) {
        delete first;
        first = NULL;
        return;
    }
    Node<T> * del = first, * prev = first;
    while (del->link != first) {
        prev = del;
        del = del->link;
    }
    prev->link = first;
    delete del;
}

template <class T> 
void CircularList<T>::DeleteOther() 
{
    if (first) {
        int idx = 1;
        Node<T> * temp, * prev;
        DeleteFirst();
        if (first == 0) return;
        idx++;
        for (temp = first; temp->link != first; temp = temp->link) {
            if (idx % 2) {
                prev->link = temp->link;
                delete temp;
                temp = prev;
            }
            prev =temp;
            idx++;
        }
        if (idx % 2) DeleteBack();
    }
}

template <class T> 
void CircularList<T>::Deconcatenate(CircularList<T>& sublist, Node<T> * p)
{
    Node <T> * prev = first, * temp = p;
    while (prev->link != p) prev = prev->link;
    while (temp != first) {
        sublist.InsertBack(temp->data);
        temp = temp->link;
    }
    prev->link = first;
}

template <class T> 
Node<T> * CircularList<T>::GetFourth()
{
    return (first->link->link->link);
}

template <class T> 
void CircularList<T>::Merge(CircularList<T>& b) 
{
    Node<T> * cur = first, * b_cur = b.first;
    Node<T> * temp, * b_temp;
    while (cur->link != first && b_cur->link != first) {
        temp = cur->link;
        b_temp = b_cur->link;
        cur->link = b_cur;
        cur = temp;
        b_cur->link = temp;
        b_cur = b_temp;
    }
    if (cur->link == first) {
        cur->link = b_cur;
        while (b_cur->link != b.first) b_cur = b_cur->link;
        b_cur->link = first;
    } else {
        b_cur->link = cur->link;
        cur->link = b_cur;
    }
    b.first = NULL;
}

template <class T> 
ostream& operator<<(ostream& os, CircularList<T>& L)
{
    if (L.first == NULL) {
        os << "{NULL}" << endl;
        return os;
    }
    os << "{";
    Node<T> * temp = L.first;
    while (temp->link != L.first) {
        os << temp->data << ",";
        temp = temp->link;
    }
    os << temp->data << "}" << endl;
    return os;
}

int main()
{
    CircularList<int> L1, L2;
    Node<int> * p;
    int num;

    cout << "Enter ten integers to insert at the front: ";
    for (int i = 0; i < 10; i++) {
        cin >> num;
        L1.InsertFront(num);
    }
    cout << "L1: " << L1;
    cout << "Enter ten integers to insert at the back: ";
    for (int i = 0; i < 10; i++) {
        cin >> num;
        L1.InsertBack(num);
    }
    cout << "L1: " << L1;
    cout << "The size of L1 is " << L1.count() << endl;
    cout << "How many elements do you want to delete from the front? ";
    cin >> num;
    while (num--) L1.DeleteFirst();
    cout << "L1: " << L1;
    cout << "How many elements do you want to delete from the back? ";
    cin >> num;
    while (num--) L1.DeleteBack();
    cout << "L1: " << L1;
    L1.DeleteOther();
    cout <<"L1.DeleteOther(): " << L1;
    p = L1.GetFourth();
    L1.Deconcatenate(L2, p);
    cout << "p = L1.GetFourth()" << endl;
    cout << "Performing L1.Deconcatenate(L2,p)" << endl;
    cout << "L1: " << L1;
    cout << "L2: " << L2;
    L1.Merge(L2);
    cout << "L1 merge L2: " << L1;

    return 0;
}