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

// implement queue
template <class T>
class Queue: public Bag<T> 
{
    template <class U> 
    friend ostream& operator<<(ostream&, Queue<U>&);
    public:
        Queue(int queueCapacity = 10);
        ~Queue();
        bool IsEmpty() const;
        void Push(const T& item);
        void Pop();
        T& Front() const;
        T& Rear() const;
        int Size() const;
        using Bag<T>::capacity;
        using Bag<T>::array;
    private:
        int front, rear;
};

template <class T>
Queue<T>::Queue(int queueCapacity): Bag<T>(queueCapacity) 
{ front = rear = 0; }

template <class T>
Queue<T>::~Queue() {};

template <class T>
bool Queue<T>::IsEmpty() const 
{ return front == rear; }

template <class T> 
T& Queue<T>::Front() const
{
    if (IsEmpty()) throw "Queue is empty";
    return array[(front+1)%capacity];
}

template <class T>
T& Queue<T>::Rear() const 
{
    if (IsEmpty()) throw "Queue is empty";
    return array[rear];
}

template <class T>
void Queue<T>::Push(const T& item)
{
    if ((rear + 1) % capacity == front) {
        T* newqueue = new T[2 * capacity];
        int start = (front + 1) % capacity;
        if (start < 2)
            copy(array + start, array + capacity + start - 1, newqueue);
        else {
            copy(array + start, array + capacity, newqueue);
            copy(array, array + rear + 1, newqueue + capacity - start);
        }
        front = 2 * capacity - 1;
        rear = capacity - 2;
        delete[] array;
        array = newqueue;
        capacity *= 2;
    }
    rear = (rear + 1) % capacity;
    array[rear] = item;
}

template <class T> 
void Queue<T>::Pop() 
{
    if (IsEmpty()) throw "Queue is empty";
    front = (front + 1) % capacity;
    array[front].~T();
}

template <class T> 
int Queue<T>::Size() const
{ return ((rear - front + capacity) % capacity); }

template <class T> 
ostream& operator<<(ostream& os, Queue<T>& queue)
{
    os << "[Front]  ";
    int i = (queue.front + 1) % queue.capacity;
    while (i != (queue.rear+1)%queue.capacity) {
        os << queue.array[i] << "   ";
        i = (i + 1) % queue.capacity;
    }
    os << "[Rear]" << endl;
    return os;
}

int main()
{
    Queue<int> SI(5);
    Queue<float> SF(5);
    int input;
    float in;
    
    cout << "Enter five integer to push into the queue: ";
    for (int i = 0; i < 5; i++) {
        cin >> input;
        SI.Push(input);
    }
    cout << "Queue: " << SI;
    cout << "The size of the queue is: " << SI.Size() << endl;
    cout << "How many element do you want to pop? ";
    cin >> input;
    for (int i = 0; i < input; i++) SI.Pop();
    cout << "The Queue after pop: " << SI;
    cout << "The size of the queue is: " << SI.Size() << endl;

    cout << "=========================================================" << endl;

    cout << "Enter five float to push into the queue: ";
    for (int i = 0; i < 5; i++) {
        cin >> in;
        SF.Push(in);
    }
    cout << "Queue: " << SF;
    cout << "The size of the queue is: " << SF.Size() << endl;
    cout << "How many element do you want to pop? ";
    cin >> input;
    for (int i = 0; i < input; i++) SF.Pop();
    cout << "The Queue after pop: " << SF;
    cout << "The size of the queue is: " << SF.Size() << endl;

    return 0;
}