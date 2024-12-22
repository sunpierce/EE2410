#include <iostream>
using namespace std;

template <class T> 
class Chain;
template <class T> 
class LinkedStack;
template <class T> 
class LinkedQueue;

template <class T> 
class ChainNode {
    friend class Chain<T>;
    friend class LinkedStack<T>;
    friend class LinkedQueue<T>;
    template <class U>
    friend ostream& operator<<(ostream&, LinkedQueue<U>&);
    template <class U> 
    friend ostream& operator<<(ostream&, LinkedStack<U>&);
    template <class U> 
    friend ostream& operator<<(ostream&, Chain<U>&);
    public: 
        ChainNode(T e = NULL, ChainNode<T> * next = NULL) { data = e; link = next; }
    private:
        T data;
        ChainNode<T> * link;
};

template <class T> 
class Chain {
    template <class U> 
    friend ostream& operator<<(ostream&, Chain<U>&);  // (b)
    public: 
        Chain() {first = last = nullptr;}
        ~Chain();
        bool IsEmpty() { return first == NULL; }
        int Size();
        void InsertHead(const T& e);
        void DeleteHead();
        const T& Front() { return first->data; }
        const T& Back() { return last->data; }
        void InsertBack(const T& e);
        void DeleteBack();
        T& Get(int index);
        void Set(int index, const T& e);
        int IndexOf(const T& e) const;
        void Delete(int index);
        void Insert(int index, const T& e);
        void InsertBefore(int index, const T& e); // (c)
        void Concatenate(Chain<T>& b);
        void Reverse();
        void Delete(ChainNode<T> * p);
        void DeleteOther(); // (d)
        void divideMid(Chain<T>& sublist); // (e)
        void deconcatenate(Chain<T>& sublist, ChainNode<T> * p); // (f)
        void merge(Chain<T>& b); // (g)
        T Compute(); // (j)
        ChainNode<T> * GetThird(); // additional
        void Insert(ChainNode<T> * p, const T& e);
        class ChainIterator {
            public:
                ChainIterator(ChainNode<T> * startNode = 0) { current = startNode; }
                T& operator*() const { return current->data; }
                T* operator->() const { return &(current->data); }
                ChainIterator& operator++() {
                    current = current->link;
                    return *this;
                }
                ChainIterator& operator++(int) {
                    ChainIterator old = *this;
                    current = current->link;
                    return old;
                }
                bool operator!=(const ChainIterator r) { return current != r.current; }
                bool operator==(const ChainIterator r) { return current == r.current; }
            private:
                ChainNode<T> * current;
        };
        ChainIterator begin() {return ChainIterator(first);}
        ChainIterator end() {return ChainIterator(NULL);}
    protected: // should be accessed by the derived class
        ChainNode<T> * first, * last;
};

template <class T>
Chain<T>::~Chain() 
{
    while (first) {
        ChainNode<T> * next = first->link;
        delete first;
        first = next;
    }
}

template <class T> 
int Chain<T>::Size()
{
    int ct = 0;
    ChainNode<T> * cur = first;
    while (cur != NULL) {
        ct++;
        cur = cur->link;
    }
    return ct;
}

template <class T>
void Chain<T>::InsertHead(const T& e)
{
    if (first) {
        first = new ChainNode<T>(e,first);
    } else {
        first = last = new ChainNode<T>(e);
    }
}

template <class T>
void Chain<T>::DeleteHead()
{
    if (first) {
        if (first == last) {
            delete first;
            //delete last;
            first = last = NULL;
        } else {
            ChainNode<T> * del = first;
            first = first->link;
            delete del;
        }
    }
}

template <class T>
void Chain<T>::InsertBack(const T& e)
{
    if (first) {
        last->link = new ChainNode<T>(e);
        last = last->link;
    } else {
        first = last = new ChainNode<T>(e);
    }
}

template <class T> 
void Chain<T>::DeleteBack()
{
    if (first) {
        if (first == last) {
            delete first;
            //delete last;
            first = last = NULL;
        } else {
            ChainNode<T> * prev = first;
            while (prev->link != last) {
                prev = prev->link;
            }
            prev->link = NULL;
            delete last;
            last = prev;
        }
    }
}

template <class T>
T& Chain<T>::Get(int index)
{
    ChainNode<T> * cur = first;
    if (index >= Size()) throw "Error!";
    for (int i = 1; i <= index; i++) {
        cur = cur->link;
    }
    return cur->data;
}

template <class T> 
void Chain<T>::Set(int index, const T& e)
{
    ChainNode<T> * cur = first;
    if (index >= Size()) throw "Error!";
    for (int i = 1; i <= index; i++) {
        cur = cur->link;
    }
    cur->data = e;
}

template <class T>
int Chain<T>::IndexOf(const T& e) const
{
    ChainNode<T> * cur = first;
    int index = 0;
    while (cur != NULL && cur->data != e) {
        cur = cur->link;
        index++;
    }
    if (cur == NULL) return -1;
    else return index;
}

template <class T>
void Chain<T>::Delete(int index)
{
    int oldsize = Size();
    if (first == 0) throw "Error";
    if (index >= oldsize) throw "Error";
    if (index == 0) {
        DeleteHead();
    } else {
        ChainNode<T> * prev = first;
        for (int i = 1; i < index; i++) prev = prev->link;
        ChainNode<T> * del = prev->link;
        prev->link = del->link;
        delete del;
        if (index == oldsize-1) {
            last = prev;
        }
    }
}

template <class T>
void Chain<T>::Insert(int index, const T& e)
{
    if (index < 0) throw "Error";
    if (index == 0) {
        if (first == last) {
            first = last = new ChainNode<T>(e,NULL);
        } else {
            first = new ChainNode<T>(e,first);
        }
    } else {
        ChainNode<T> * prev = first;
        for (int i = 1; i < index; i++) {
            if (prev == NULL) throw "Bad insert index";
            prev = prev->link;
        }
        prev->link = new ChainNode<T>(e,prev->link);
    }
}

template <class T> 
void Chain<T>::Concatenate(Chain<T>& b)
{
    if (first) {
        last->link = b.first;
        last = b.last;
    } else {
        first = b.first;
        last = b.last;
    }
    b.first = b.last = NULL;
}

template <class T> 
void Chain<T>::Reverse()
{
    ChainNode<T> * cur = first;
    ChainNode<T> * prev = 0;
    ChainNode<T> * r = 0; // prev prev
    while (cur) {
        // advancing
        r = prev;
        prev = cur;
        cur = cur->link;
        // relinking
        prev->link = r;
    }
    last = first;
    first = prev;
}

template <class T>
void Chain<T>::Delete(ChainNode<T> * p)
{
    if (first == 0) return;
    ChainNode<T> * prev = first;
    if (prev == p) {
        DeleteHead();
        return;
    }
    while (prev != NULL && prev->link != p) {
        prev = prev->link;
    }
    if (prev == NULL) return;
    ChainNode<T> * del = prev->link;
    prev->link = del->link;
    if (del == last) last = prev;
    delete del;
}

template <class T>
void Chain<T>::Insert(ChainNode<T> * p, const T& e) // insert at p
{
    ChainNode<T> * cur = first;
    while (cur->link != p) {
        cur = cur->link;
        if (cur == NULL) return;
    }
    cur->link = new ChainNode<T>(e,cur->link);
}

template <class U> 
ostream& operator<<(ostream& os, Chain<U> & C)
{
    if (C.first == NULL) {
        os << "{NULL}" << endl;
        return os;
    }
    ChainNode<U> * temp;
    os << "{";
    for (temp = C.first; temp->link != NULL; temp = temp->link) {
        os << temp->data << ",";
    }
    os << temp->data;
    os << "}" << endl;
    return os;
}

template <class T>
ChainNode<T> * Chain<T>::GetThird()
{
    return (first->link->link);
}

template <class T>
void Chain<T>::InsertBefore(int index, const T& e)
{
    Insert(index-1,e);
}

template <class T> 
void Chain<T>::DeleteOther()
{
    if (first) {
        int idx = 1;
        ChainNode<T> * temp, * prev;
        DeleteHead();
        if (first == 0) return;
        idx++;
        for (temp = first; temp != last; temp = temp->link) {
            if (idx % 2) {
                prev->link = temp->link;
                delete temp;
                temp = prev;
            }
            prev = temp;
            idx++;
        }
        if (idx % 2) DeleteBack();
    }
}

template <class T>
void Chain<T>::divideMid(Chain<T>& sublist)
{
    int tot = Size();
    int mid;
    if (tot%2) mid = tot/2+1;
    else mid = tot/2;
    ChainNode<T> * cur = first;
    for (int i = 0; i < mid-1; i++) cur = cur->link;
    ChainNode<T> * temp = cur->link;
    while (temp) {
        sublist.InsertBack(temp->data);
        temp = temp->link;
    }
    last = cur;
    last->link = NULL;
}

template <class T>
void Chain<T>::deconcatenate(Chain<T>& sublist, ChainNode<T> * p)
{
    ChainNode<T> * prev = first, * temp = p;
    while (prev->link != p) prev = prev->link;
    while (temp) {
        sublist.InsertBack(temp->data);
        temp = temp->link;
    }
    last = prev;
    last->link = NULL;
}

template <class T> 
T Chain<T>::Compute()
{
    ChainNode<T> * p = first, * q = first;
    T result = 0;
    for (int k = 0; k < 5; k++) q = q->link;
    for (; q->link != NULL; p = p->link, q = q->link) {
        result += p->data * q->data;
    }
    result += p->data * q->data;
    return result;
}

template <class T> 
void Chain<T>::merge(Chain<T>& b)
{ 
    // temp and b_temp records the link ChainNode
    ChainNode<T> * cur = first, * b_cur = b.first;
    ChainNode<T> * temp, * b_temp;
    while (cur->link && b_cur->link) {
        temp = cur->link;
        b_temp = b_cur->link;
        cur->link = b_cur;
        cur = temp;
        b_cur->link = temp;
        b_cur = b_temp;
    }
    if (cur->link == NULL) {
        cur->link = b_cur;
    } else {
        b_cur->link = cur->link;
        cur->link = b_cur;
    }
    b.first = b.last = NULL;
}


// implementing stack using chain
template <class T>
class LinkedStack: public Chain<T> {
    template <class U>
    friend ostream& operator<<(ostream&, LinkedStack<U>&);
public:
    LinkedStack() { top = NULL; }
    ~LinkedStack(){}
    bool IsEmpty() const { return (top == NULL); }
    T& Top() const { return top->data; }
    void Push(const T&item);
    void Pop();
protected:
    ChainNode<T> * top;
};

template <class T>
void LinkedStack<T>:: Push(const T &item)
{
    Chain<T>::InsertBack(item);
    top = Chain<T>::last;
}

template <class T>
void LinkedStack<T>:: Pop()
{
    Chain<T>::DeleteBack();
    top = Chain<T>::last;
}

template <class U> 
ostream& operator<<(ostream& os, LinkedStack<U>& S)
{
    ChainNode<U> * cur = S.first;
    os << "[Bottom] ";
    while (cur) {
        os << cur->data << " ";
        cur = cur->link;
    }
    os << "[Top]" << endl;
    return os;
}


// implementing queue using chain
template <class T> 
class LinkedQueue: public Chain<T> 
{
    template <class U> 
    friend ostream& operator<<(ostream&, LinkedQueue<U>&);
    public: 
        LinkedQueue() { front = rear = new ChainNode<T>; }
        ~LinkedQueue() {}
        void Push(const T&);
        void Pop();
        bool IsEmpty() { return (rear == front); }
        T& Front() const { return front->link->data; }
        T& Rear() const { return rear->data; }
    private:
        ChainNode<T> * front, * rear;
};

template <class T> 
void LinkedQueue<T>::Push(const T& item)
{
    Chain<T>::InsertBack(item);
    if (rear == front) { // empty
        rear = Chain<T>::first;
        front->link = rear; // front is always empty
    } else {
        rear = Chain<T>::last;
    }
}

template <class T> 
void LinkedQueue<T>::Pop()
{
    Chain<T>::DeleteHead();
    front->link = Chain<T>::first;
    if (Chain<T>::first == Chain<T>::last) rear = front; // empty linked queue
}

template <class U> 
ostream& operator<<(ostream& os, LinkedQueue<U>& Q)
{
    ChainNode<U> * cur = Q.first;
    os << "[Front] ";
    while (cur) {
        os << cur->data << " ";
        cur = cur->link;
    }
    os << "[Rear]" << endl;
    return os;
}



int main()
{
    Chain<int> C1, C2, C3, C4;
    int input, input2;
    cout << "=============================================== Chain Testing ===============================================" << endl;
    cout << "Enter 25 integers to insert into the chain: ";
    for (int i = 0; i < 25; i++) {
        cin >> input;
        C1.InsertBack(input);
    }
    cout << "Chain: " << C1;
    if (C1.IsEmpty()) cout << "Chain is empty" << endl;
    else cout << "Chain is not empty" << endl;
    cout << "The size of the chain is " << C1.Size() << endl;
    cout << "C1.Compute() = ";
    cout << C1.Compute() << endl;
    cout << "Enter an integer to insert to the head: ";
    cin >> input;
    C1.InsertHead(input);
    cout << "Chain: " << C1;
    cout << "Enter an integer to insert at the back: ";
    cin >> input;
    C1.InsertBack(input);
    cout << "Chain: " << C1;
    C1.DeleteHead();
    cout << "The Chain after deleting head: " << C1;
    cout << "Enter a index to get the corresponding value: ";
    cin >> input;
    cout << "The corresponding value is " << C1.Get(input) << endl;
    cout << "Enter a index and an int to set that index to int: ";
    cin >> input >> input2;
    C1.Set(input, input2);
    cout << "Chain: " << C1;
    cout << "Enter an integer: ";
    cin >> input;
    cout << "The index of that integer is " << C1.IndexOf(input) << endl;
    cout << "Enter a index to delete: ";
    cin >> input;
    C1.Delete(input);
    cout << "Chain: " << C1;
    cout << "Enter a index and an integer to insert at that index: " << endl;
    cin >> input >> input2;
    C1.Insert(input, input2);
    cout << "Chain: " << C1;
    cout << "Enter five integers to insert into a new chain: ";
    for (int i = 0; i < 5; i++) {
        cin >> input;
        C2.InsertBack(input);
    }
    cout << "Chain2: " << C2;
    C1.Concatenate(C2);
    cout << "Concatenating Chain and Chain2: " << C1;
    C1.Reverse();
    cout << "Reversing Chain: " << C1;
    cout << "Enter i, j to insert j immediate before of the ith node: " << endl;
    cin >> input >> input2;
    C1.InsertBefore(input, input2);
    cout << "Chain: " << C1;
    C1.DeleteOther();
    cout <<  "The chain after deleting 1st, 3rd, 5th, ... nodes: " << C1;
    cout << "MyList.dividMid(SubList)" << endl;
    C1.divideMid(C2);
    cout << "MyList (C1): " << C1;
    cout << "SubList (C2): " << C2;
    cout << "ChainNode<int> * p = C1.GetThird() // get the 3rd ChainNode" << endl;
    cout << "C1.deconcatenate(C3,p)" << endl;
    ChainNode<int> * p = C1.GetThird();
    C1.deconcatenate(C3,p);
    cout << "C1: " << C1;
    cout << "C3: " << C3;
    C2.merge(C3);
    cout << "C2.merge(C3): " << C2; // C3 is empty now cannot be accessed


   cout << "=============================================== Stack Testing ===============================================" << endl;
   LinkedStack<char> S;
   char in;
   int num;
   cout << "Enter ten char to push to the stack: ";
   for (int i = 0; i < 10; i++) {
    cin >> in;
    S.Push(in);
   }
   cout << "Stack: " << S;
   cout << "The top element is " << S.Top() << endl;
   cout << "How many char do you want to pop? ";
   cin >> num;
   while (num--) S.Pop();
   cout << "Stack: " << S;
   if (S.IsEmpty()) cout << "The stack is empty" << endl;
   else cout << "The stack is not empty" << endl;


   cout << "=============================================== Queue Testing ===============================================" << endl;
   LinkedQueue<char> Q;
   cout << "Enter ten char to push to the stack: ";
   for (int i = 0; i < 10; i++) {
    cin >> in;
    Q.Push(in);
   }
   cout << "Queue: " << Q;
   cout << "The front element is " << Q.Front() << endl;
   cout << "The rear element is " << Q.Rear() << endl;
   cout << "How many char do you want to pop? ";
   cin >> num;
   while (num--) Q.Pop();
   cout << "Queue: " << Q;
   if (Q.IsEmpty()) cout << "The queue is empty" << endl;
   else cout << "The queue is not empty" << endl;

    return 0;
}