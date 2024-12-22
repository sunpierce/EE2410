#include <iostream>
#include <stack>
using namespace std;

template <class T> 
class Queue
{
    public:
        template <class U>
        friend ostream& operator<<(ostream& out, Queue<U>& queue);
        template <class U>
        friend bool operator==(Queue<U>& q1, Queue<U>& q2); // (c)
        template <class U>
        friend void reverseQueue(Queue<U>& queue); // (e)
        Queue(int queueCapacity = 10);
        bool IsEmpty() const;
        void Push(const T& item);
        void Pop();
        T& Front() const;
        T& Rear() const;
        int Size() const; // (a)
        int Capacity() const; // (b)
        Queue<T> MergeQueue(Queue<T> queue); // (d)
    protected:
        T * queue;
        int front, rear, capacity;
};

template <class T>
Queue<T>::Queue(int queueCapacity): capacity(queueCapacity)
{
    if (capacity < 1) throw "Queue capacity must be > 0";
    queue = new T[capacity];
    front = rear = 0; // indicate empty queue
}

template <class T>
inline bool Queue<T>::IsEmpty() const
{
    return front == rear;
}

template <class T>
inline T& Queue<T>::Front() const 
{
    if (IsEmpty()) throw "Queue is empty. No front element";
    return queue[(front + 1) % capacity];
}

template <class T>
inline T& Queue<T>::Rear() const 
{
    if (IsEmpty()) throw "Queue is empty. No rear element";
    return queue[rear];
}

template <class T>
void Queue<T>:: Push(const T& item) 
{
    if ((rear + 1) % capacity == front) {
        T* newqueue = new T[2 * capacity];
        int start = (front + 1) % capacity;
        if (start < 2)
            copy(queue + start, queue + capacity + start - 1, newqueue);
        else {
            copy(queue + start, queue + capacity, newqueue);
            copy(queue, queue + rear + 1, newqueue + capacity - start);
        }
        front = 2 * capacity - 1;
        rear = capacity - 2;
        delete[] queue;
        queue = newqueue;
        capacity *= 2;
    }
    rear = (rear + 1) % capacity;
    queue[rear] = item;
}

template <class T>
void Queue<T>:: Pop() 
{
    if (IsEmpty()) throw "Queue is empty, cannot delete";
    front = (front + 1) % capacity;
    queue[front].~T();
}

template <class T> 
inline int Queue<T>::Size() const
{ return (rear - front + capacity) % capacity; }

template <class T>
inline int Queue<T>::Capacity() const
{ return capacity; }

template <class T> 
void reverseQueue(Queue<T>& queue)
{
    // using STL stack to implement
    stack<T> s;
    while (!queue.IsEmpty()) {
        s.push(queue.Front());
        queue.Pop();
    }
    while (!s.empty()) {
        queue.Push(s.top());
        s.pop();
    }
}

template <class T>
bool operator==(Queue<T>& q1, Queue<T>& q2)
{
    if (q1.Size() != q2.Size()) return false;
    for (int i = 1; i <= q1.Size(); i++) {
        if (q1.queue[(q1.front+i)%q1.capacity] != q2.queue[(q2.front+i)%q2.capacity])
            return false;
    }
    return true;
}

template <class T>
Queue<T> Queue<T>::MergeQueue(Queue<T> q2)
{
    Queue<T> result(capacity+q2.capacity);
    int pos = (front + 1) % capacity;
    int pos2 = (q2.front + 1) % q2.capacity;
    while (pos != rear && pos2 != q2.rear) {
        result.Push(queue[pos]);
        result.Push(q2.queue[pos2]);
        pos = (pos + 1) % capacity;
        pos2 = (pos2 + 1) % q2.capacity;
    }
    // remaining term
    if (Size() < q2.Size()) {
        result.Push(queue[pos]);
        while (pos2 != rear) {
            result.Push(q2.queue[pos2]);
            pos2 = (pos2 + 1) % q2.capacity;
        }
        result.Push(q2.queue[pos2]);
    } else if (Size() > q2.Size()) {
        result.Push(queue[pos]);
        pos = (pos + 1) % capacity;
        result.Push(q2.queue[pos2]);
        while (pos != rear) {
            result.Push(queue[pos]);
            pos = (pos + 1) % capacity;
        }
        result.Push(queue[pos]);
    } else {
        result.Push(queue[pos]);
        result.Push(q2.queue[pos2]);
    }
    return result;
}

template <class T>
ostream& operator<<(ostream& os, Queue<T>& queue)
{
    os << "[Front]  ";
    int i = (queue.front + 1) % queue.capacity;
    while (i != (queue.rear+1)%queue.capacity) {
        os << queue.queue[i] << "   ";
        i = (i + 1) % queue.capacity;
    }
    os << "[Rear]" << endl;
    return os;
}

// Start implementing deque
template <class T>
class Deque: public Queue<T> 
{
        using Queue<T>::queue;
        using Queue<T>::capacity;
        using Queue<T>::front;
        using Queue<T>::rear;
        using Queue<T>::IsEmpty;
        using Queue<T>::Push;
        using Queue<T>::Pop;
        template <class U>
        friend ostream& operator<<(ostream&, Deque<U>&);
    public:
        Deque(int dequeCapacity = 10);
        ~Deque();
        void Push_front(const T& item);
        void Push_rear(const T& item);
        void Pop_front();
        void Pop_rear();
};

template <class T> 
Deque<T>::Deque(int dequeCapacity): Queue<T>(dequeCapacity) {}

template <class T>
Deque<T>::~Deque() {}

template <class T> 
void Deque<T>::Push_rear(const T& item)
{ Push(item); }

template <class T>
void Deque<T>::Pop_front()
{ Pop(); }

template <class T> 
void Deque<T>::Push_front(const T& item)
{
    if ((rear + 1) % capacity == front) {
        T* newqueue = new T[2 * capacity];
        int start = (front + 1) % capacity;
        if (start < 2)
            copy(queue + start, queue + capacity + start - 1, newqueue);
        else {
            copy(queue + start, queue + capacity, newqueue);
            copy(queue, queue + rear + 1, newqueue + capacity - start);
        }
        front = 2 * capacity - 1;
        rear = capacity - 2;
        delete[] queue;
        queue = newqueue;
        capacity *= 2;
    }
    queue[front] = item;
    front = (front - 1 + capacity) % capacity;
}

template <class T> 
void Deque<T>::Pop_rear()
{
    if (IsEmpty()) throw "Queue is empty";
    queue[rear].~T();
    rear = (rear - 1 + capacity) % capacity;
}

template <class T>
ostream& operator<<(ostream& os, Deque<T>& queue)
{
    os << "[Front]  ";
    int i = (queue.front + 1) % queue.capacity;
    while (i != (queue.rear+1)%queue.capacity) {
        os << queue.queue[i] << "   ";
        i = (i + 1) % queue.capacity;
    }
    os << "[Rear]" << endl;
    return os;
}

int main()
{
    Deque<int> dq;
    int num;

    cout << "Enter eight integers to push into the deque: " << endl;
    for (int i = 0; i < 8; i++) {
        cin >> num;
        dq.Push_rear(num);
    }
    cout << "Deque: " << dq;
    cout << "How many elements do you want to pop from the front? " << endl;
    cin >> num;
    while (num--) { dq.Pop_front(); }
    cout << "Deque: " << dq;
    cout << "Enter ten integers to push into the front: " << endl;
    for (int i = 0; i < 10; i++) {
        cin >> num;
        dq.Push_front(num);
    }
    cout << "Deque: " << dq;
    cout << "How many elements do you want to pop from the rear? " << endl;
    cin >> num;
    while (num--) { dq.Pop_rear(); }
    cout << "Deque: " << dq;
    cout << "The element at the front is " << dq.Front() << endl;
    cout << "The element at the rear is " << dq.Rear() << endl;

    return 0;
}