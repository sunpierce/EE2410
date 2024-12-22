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
        CircularList(); // constructor
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
        Node<T> * header;
};

template <class T> 
CircularList<T>::CircularList()
{
    header = new Node<T>(0,0);
    header->link = header;
}

template <class T> 
int CircularList<T>::count()
{
    Node<T> * temp = header->link;
    int ct = 0;
    while (temp != header) {
        ct++;
        temp = temp->link;
    }
    return ct;
}

template <class T> 
void CircularList<T>::InsertFront(T e)
{
    header->link = new Node<T>(e, header->link);
}

template <class T> 
void CircularList<T>::InsertBack(T e)
{
    if (header->link == header) {
        header->link = new Node<T>(e, header);
    } else {
        Node<T> * last = header->link;
        while (last->link != header) {
            last = last->link;
        }
        last->link = new Node<T>(e,header);
    }
}

template <class T> 
void CircularList<T>::DeleteFirst() 
{
    if (header->link == header) return;
    Node<T> * del = header->link;
    header->link = del->link;
    delete del;
}

template <class T> 
void CircularList<T>::DeleteBack() 
{
    if (header->link == header) return;
    Node<T> * del = header->link, * prev = header;
    while (del->link != header) {
        prev = del;
        del = del->link;
    }
    prev->link = header;
    delete del;
}

template <class T> 
void CircularList<T>::DeleteOther() 
{
    if (header->link != header) {
        int idx = 1;
        Node<T> * temp, * prev;
        DeleteFirst();
        if (header->link == header) return;
        idx++;
        for (temp = header->link; temp->link != header; temp = temp->link) {
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
    Node <T> * prev = header, * temp = p;
    while (prev->link != p) prev = prev->link;
    while (temp != header) {
        sublist.InsertBack(temp->data);
        temp = temp->link;
    }
    prev->link = header;
}

template <class T> 
Node<T> * CircularList<T>::GetFourth()
{
    return (header->link->link->link->link);
}

template <class T> 
void CircularList<T>::Merge(CircularList<T>& b) 
{
    Node<T> * cur = header->link, * b_cur = b.header->link;
    Node<T> * temp, * b_temp;
    while (cur->link != header && b_cur->link != header) {
        temp = cur->link;
        b_temp = b_cur->link;
        cur->link = b_cur;
        cur = temp;
        b_cur->link = temp;
        b_cur = b_temp;
    }
    if (cur->link == header) {
        cur->link = b_cur;
        while (b_cur->link != b.header) b_cur = b_cur->link;
        b_cur->link = header;
    } else {
        b_cur->link = cur->link;
        cur->link = b_cur;
    }
}

template <class T> 
ostream& operator<<(ostream& os, CircularList<T>& L)
{
    if (L.header->link == L.header) {
        os << "{NULL}" << endl;
        return os;
    }
    os << "{";
    Node<T> * temp = L.header->link;
    while (temp->link != L.header) {
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