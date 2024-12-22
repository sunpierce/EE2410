# include <iostream> 
using namespace std;

template <class T>
class MinHeap;

template <class T>
class MinPQ
{
    public:
        virtual ~MinPQ(){};
        virtual bool IsEmpty() const = 0;
        virtual const T &Top() const = 0;
        virtual void Push(const T &) = 0;
        virtual void Pop() = 0;
};

template <class T> 
class MinHeap: public MinPQ<T>
{
    template <class U>
    friend ostream& operator<<(ostream& os, MinHeap<U>& a);
    public:
        MinHeap(int theCapacity = 10);
        MinHeap(T array[], int N); // bottom-up heap construction
        bool IsEmpty() const { return heapSize == 0; }
        const T& Top() const { return heap[1]; }
        void Push(const T& x);
        void Pop();
        void ChangeSize(T *& oldHeap, const int oldSize, const int newSize);
    private: 
        T * heap;
        int heapSize, capacity;
};

template <class T>
ostream& operator<<(ostream& os, MinHeap<T>& a)
{
    for (int i = 1; i <= a.heapSize; i++) os << a.heap[i] << "  ";
    os << endl;
    return os;
}

template <class T> 
MinHeap<T>::MinHeap(int theCapacity)
{
    capacity = theCapacity;
    heapSize = 0;
    heap = new T[capacity+1];
}

template <class T>
MinHeap<T>:: MinHeap(T array[], int N) {
    heap = new T[2 * N];
    capacity = 2 * N;
    heapSize = N;
    int i = 0, j;
    while (i < N)
    {
        heap[i + 1] = array[i];
        i++;
    }
    for(j = N / 2; j >= 1; j--) {
        int k = j;
        T temp = heap[k];
        bool flag = false;
        while (!flag && 2 * k <= N) {
            int l = 2 * k;
            if (l < N)
                if (heap[l] > heap[l + 1])
                    l++;
            if (temp <= heap[l])
                flag = true;
            else {
                heap[k] = heap[l];
                k = l;
            }
        }
        heap[k] = temp;
    }
}

template <class T> 
void MinHeap<T>::ChangeSize(T *& oldHeap, const int oldSize, const int newSize)
{
    T * temp = new T[newSize];
    int num = min(oldSize, newSize);
    copy(oldHeap+1, oldHeap+num+1, temp+1);
    delete[] oldHeap;
    oldHeap = temp;
}

template <class T> 
void MinHeap<T>::Push(const T& e)
{
    if (heapSize == capacity) {
        ChangeSize(heap, capacity, 2 * capacity);
        capacity *= 2;
    }
    int currentNode = ++heapSize;
    while (currentNode != 1 && heap[currentNode/2] > e) {
        // bubbling up
        heap[currentNode] = heap[currentNode/2]; // move parent down
        currentNode /= 2;
    }
    heap[currentNode] = e;
}

template <class T> 
void MinHeap<T>::Pop() 
{
    if (IsEmpty()) throw "Heap is empty. Cannot delete.";
    heap[1].~T(); // delete max element
    T last = heap[heapSize--]; // remove the last element
    // trickle down to find a position to place the last element
    int cur = 1; // root
    int child = 2;
    while (child <= heapSize) {
        // Set child to larger child of cur
        if (child < heapSize && heap[child] > heap[child+1]) child++;
        if (last <= heap[child]) break;
        heap[cur] = heap[child]; // move child up
        cur = child; child *= 2; // move down a level
    }
    heap[cur] = last;
}

int main()
{
    int in;
    MinHeap<int> MH;
    MH.Push(50); MH.Push(5); MH.Push(30); MH.Push(40);
    MH.Push(80); MH.Push(35); MH.Push(2); MH.Push(20);
    MH.Push(15); MH.Push(60); MH.Push(70); MH.Push(8);
    MH.Push(10);
    cout << "Initializing min heap by using a series of 13 pushes: ";
    cout << MH;
    cout << "How many element do you want to pop from the max heap? ";
    cin >> in;
    while (in--) MH.Pop();
    cout << "Min heap after pop: ";
    cout << MH;
    cout << "Initializing min heap by bottom up heap construction: ";
    int a[13] = {50, 5, 30, 40, 80, 35, 2, 20, 15, 60, 70, 8, 10};
    MinHeap<int> MH2(a,13);
    cout << MH2;
    if (MH2.IsEmpty()) cout << "Max heap is empty" << endl;
    else cout << "Max heap is non-empty" << endl;
    cout << "Enter a number to push to the max heap: ";
    cin >> in;
    MH2.Push(in);
    cout << "Max heap after inserting: " << MH2;
    cout << "The min element of is: " << MH2.Top() << endl;
    cout << "The elements of min heap in ascending order: ";
    while (!MH2.IsEmpty()) {
        cout << MH2.Top() << "  ";
        MH2.Pop();
    }
    cout << endl;
    return 0;
}