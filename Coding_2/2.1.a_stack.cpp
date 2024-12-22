#include <iostream>
using namespace std;

template <class T>
class Bag 
{
    //template <class U>
    //friend ostream& operator<<(ostream&, Bag<U> &);
    template <class U>
    friend void ChangeSize1D(U* &a, const int oldSize, const int newSize);
    public:
        Bag (int bagCapacity= 10);
        virtual ~Bag();
        virtual int Size() const;
        virtual bool IsEmpty() const;
        virtual T& Element() const;
        virtual void Push(const T&);
        virtual void Pop();
    protected:
        T * array;
        int capacity;
        int top;
};

template <class U>
void ChangeSize1D(U* &a, const int oldSize, const int newSize) 
{
    if (newSize < 0) throw "New length must be >= 0";
    U * temp = new U[newSize];
    int number = min(oldSize, newSize);
    copy(a, a + number, temp);
    delete[]a;
    a = temp;
}

template <class T>
Bag<T>:: Bag(int bagCapacity) : capacity(bagCapacity) 
{
    if (capacity < 1) throw "Capacity must be > 0";
    array = new T[capacity];
    top = -1;
}

template <class T>
inline Bag<T>::~Bag() { delete [] array; }

template <class T>
inline int Bag<T>:: Size() const { return top + 1; }

template <class T>
inline bool Bag<T>:: IsEmpty() const { return Size() == 0; }

template<class T>
T& Bag<T>:: Element() const 
{
    if (IsEmpty()) throw "Bag is empty";
    return array[0];
}

template <class T>
void Bag<T>:: Push(const T& x) 
{
    if (capacity == top + 1) {
        ChangeSize1D(array, capacity, 2 * capacity);
        capacity *= 2;
    }
    array[++top] = x;
}

template <class T>
void Bag<T>:: Pop() 
{
    if (IsEmpty()) throw "Bag is empty, cannot delete";
    int deletePos = top / 2;
    copy(array + deletePos + 1, array + top + 1, array + deletePos);
    top--;
}

/*template <class U>
ostream& operator<<(ostream &os, Bag<U> &bag) 
{
    int i = 0;
    os << "Bag : (";
    while (i < (bag.Size() - 1)) {
        os << bag.array[i] << ",";
        i++;
    }
    os << bag.array[i] << ")" << endl;
    return os; 
}*/

// implementing stack 
template <class T>
class Stack: public Bag<T>
{
    template <class U>
    friend ostream& operator<<(ostream&, Stack<U> &);
    //template <class U> 
    //friend void ChangeSize1D(U *& a, const int oldSize, const int newSize);
    public:
        // Stack must redefine its constructor and destructor
        Stack(int stackCapacity = 10);
        ~Stack();
        // Stack should redefine Top and Pop
        int Top() const;
        void Pop();
        // The remaining would inherit from Bag
        //bool IsEmpty() const;
        using Bag<T>::IsEmpty;
    //protected:
        //T * array;
        //int capacity;
        //int top;
        using Bag<T>:: array;
        using Bag<T>:: top;
        using Bag<T>:: capacity;
};

template <class T>
Stack<T>::Stack(int stackCapacity): Bag<T>(stackCapacity) {}

template <class T>
Stack<T>::~Stack(){}

template <class T>
int Stack<T>::Top() const
{
    if (IsEmpty()) throw "Stack is empty";
    return array[top];
}

template <class T> 
void Stack<T>::Pop() 
{
    if (IsEmpty()) throw "Stack is empty";
    array[top--].~T();
}

template <class U>
ostream& operator<<(ostream &os, Stack<U>& stack) 
{
    int i = 0;
    os << "[bottom]  ";
    while (i < (stack.Size())) {
        os << stack.array[i] << "   ";
        i++;
    }
    os << "[top]" << endl;
    return os; 
}

int main()
{
    Stack<int> SI(5);
    Stack<float> SF(5);
    int input;
    float in;
    
    cout << "Enter five integer to push into the stack: ";
    for (int i = 0; i < 5; i++) {
        cin >> input;
        SI.Push(input);
    }
    cout << "Stack: " << SI;
    cout << "The size of the stack is: " << SI.Size() << endl;
    cout << "How many element do you want to pop? ";
    cin >> input;
    for (int i = 0; i < input; i++) SI.Pop();
    cout << "The Stack after pop: " << SI;
    cout << "The size of the stack is: " << SI.Size() << endl;

    cout << "=========================================================" << endl;

    cout << "Enter five float to push into the stack: ";
    for (int i = 0; i < 5; i++) {
        cin >> in;
        SF.Push(in);
    }
    cout << "Stack: " << SF;
    cout << "The size of the stack is: " << SF.Size() << endl;
    cout << "How many element do you want to pop? ";
    cin >> input;
    for (int i = 0; i < input; i++) SF.Pop();
    cout << "The Stack after pop: " << SF;
    cout << "The size of the stack is: " << SF.Size() << endl;

    return 0;
}