#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

// Insertion Sort
template <class T>
void Insert(const T &e, T *a, int i)
{
    a[0] = e;
    while (e < a[i])
    {
        a[i + 1] = a[i];
        i--;
    }
    a[i + 1] = e;
}

template <class T>
void InsertionSort(T *a, const int n)
{
    for (int j = 2; j <= n; j++) {
        T temp = a[j];
        Insert(temp, a, j - 1);
    }
}

// Quick Sort
template <class T>
void QuickSort(T*a, const int left, const int right) 
{
    if (left < right)
    {
        int i = left;
        int j = right + 1;
        int k = (left + right) / 2;
        int pos;
        T temp1 = min(a[i], a[j - 1]);
        T temp2 = min(a[i], a[k]);
        T temp3 = min(a[k], a[j - 1]);
        T temp4 = max(temp1, temp2);
        T temp5 = max(temp2, temp3);
        T pivot = max(temp4, temp5);
        if (pivot == a[i]) pos = left;
        else if (pivot == a[j - 1]) pos = right;
        else pos = k;
        swap(a[pos], a[left]);
        do {
            do {
                i++;
            } while (a[i] < pivot);
            do {
                j--;
            } while (a[j] > pivot);
            if (i < j) swap(a[i], a[j]);
        } while (i < j);
        swap(a[left], a[j]);
        QuickSort(a, left, j - 1);
        QuickSort(a, j + 1, right);
    }
}

// Iterative Merge Sort
template <class T>
void Merge(T *initList, T *mergedList, const int l, const int m, const int n)
{
    int i1, iResult, i2;
    for (i1 = l, iResult = l, i2 = m + 1; i1 <= m && i2 <= n; iResult++)
        if (initList[i1] <= initList[i2]) {
            mergedList[iResult] = initList[i1];
            i1++;
        }
        else {
            mergedList[iResult] = initList[i2];
            i2++;
        }
        copy(initList + i1, initList + m + 1, mergedList + iResult);
        copy(initList + i2, initList + n + 1, mergedList + iResult);
}

template <class T>
void MergePass(T *initList, T *resultList, const int n, const int s)
{
    int i;
    for (i = 1; i <= n - 2 * s + 1; i += 2 * s)
        Merge(initList, resultList, i, i + s - 1, i + 2 * s - 1);
    if ((i + s - 1) < n) Merge(initList, resultList, i, i + s - 1, n);
    else copy(initList + i, initList + n + 1, resultList + i);
}

template <class T>
void Mergesort(T *a, const int n)
{
    T *tempList = new T[n + 1];
    
    for (int l = 1; l < n; l *= 2) {
        MergePass(a, tempList, n, l);
        l *= 2;
        MergePass(tempList, a, n, l);
    }
    delete [] tempList;
}

// Recursive Merge Sort
template <class T>
int ListMerge(T *a, int *link, const int start1, const int start2)
{
    int iResult = 0;
    int i1, i2;
    for (i1 = start1, i2 = start2; i1 && i2;)
        if (a[i1] <= a[i2]) {
            link[iResult] = i1;
            iResult = i1; i1 = link[i1];
        } else {
            link[iResult] = i2;
            iResult = i2; i2 = link[i2];
        }
    
    if (i1 == 0) link[iResult] = i2;
    else link[iResult] = i1;
    return link[0];
}

template <class T>
int rMergeSort(T *a, int *link, const int left, const int right)
{
    if (left >= right) return left;
    int mid = (left + right) / 2;
    return ListMerge(a, link, rMergeSort(a, link, left, mid), rMergeSort(a, link, mid + 1, right));
}

// Heap Sort
template <class T>
void Adjust(T*a, const int root, const int n) 
{
    T e = a[root];
    int j;
    for (j = 2 * root; j <= n; j *= 2) {
        if (j < n && a[j] < a[j + 1]) j++;
        if (e >= a[j]) break;
        a[j/2] = a[j];
    }
    a[j/2] = e;
}

template <class T>
void HeapSort(T *a, const int n) {
    for (int i = n / 2; i >= 1; i--)
        Adjust(a, i, n);
    for (int i = n - 1; i >= 1; i--) {
        swap(a[1], a[i + 1]);
        Adjust(a, 1, i);
    }
}

// Random Generator
// Function to generate an array of 20 random characters
void generateRandomCharacters(char characters[21]) {
    for (int i = 1; i < 21; ++i) {
        characters[i] = 'a' + rand() % 26; // Generates random lowercase letters
    }
}

// Function to generate an array of 20 random integers
void generateRandomIntegers(int integers[21]) {
    for (int i = 1; i < 21; ++i) {
        integers[i] = rand() % 100; // Generates random integers from 0 to 99
    }
}

// Function to generate an array of 20 random floating point numbers
void generateRandomFloats(float floats[21]) {
    for (int i = 1; i < 21; ++i) {
        floats[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100.0; // Generates random floats from 0 to 100
    }
}

// Function to generate an array of 20 random strings
void generateRandomStrings(std::string strings[21]) {
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 1; i < 21; ++i) {
        std::string randomString;
        for (int j = 0; j < 5; ++j) { // Generates random strings of length 5
            randomString += alphabet[rand() % 26];
        }
        strings[i] = randomString;
    }
}

int main()
{
    // Initialize random seed
    srand(static_cast<unsigned int>(time(0)));

    cout << "============================================================================================ characters ============================================================================================" << endl;
    // Generate and print random characters
    char characters[21];
    for (int i = 0; i < 5; i++) {
        generateRandomCharacters(characters);
        std::cout << "Random Characters: ";
        for (int i = 1; i < 21; ++i) {
            std::cout << characters[i] << ' ';
        }
        std::cout << std::endl;
        if (i == 0) {
            InsertionSort(characters, 20);
            cout << "By Insertion Sort: ";
            for (int i = 1; i < 21; i++) cout << characters[i] << " ";
        } else if (i == 1) {
            QuickSort(characters, 1, 20);
            cout << "By Quick Sort: ";
            for (int i = 1; i < 21; i++) cout << characters[i] << " ";
        } else if (i == 2) {
            Mergesort(characters, 20);
            cout << "By Iterative Merge Sort: ";
            for (int i = 1; i < 21; i++) cout << characters[i] << " ";
        } else if (i == 3) {
            int link[21]; fill(link+1, link + 21, 0);
            cout << "By Recursive Merge Sort: ";
            int j = rMergeSort(characters, link, 1, 20);
            for (int i = 0; i < 20; i++) {
                cout << characters[j] << " ";
                j = link[j];
            }
        } else {
            HeapSort(characters, 20);
            cout << "By Heap Sort: ";
            for (int i = 1; i < 21; i++) cout << characters[i] << " ";
        }
        cout << endl;
    }

    cout << "============================================================================================ integers ============================================================================================" << endl;
    // Generate and print random integers
    int integers[20];
    for (int i = 0; i < 5; i++) {
        generateRandomIntegers(integers);
        std::cout << "Random Integers: ";
        for (int i = 0; i < 20; ++i) {
            std::cout << integers[i] << ' ';
        }
        std::cout << std::endl;
        if (i == 0) {
            InsertionSort(integers, 20);
            cout << "By Insertion Sort: ";
            for (int i = 1; i < 21; i++) cout << integers[i] << " ";
        } else if (i == 1) {
            QuickSort(integers, 1, 20);
            cout << "By Quick Sort: ";
            for (int i = 1; i < 21; i++) cout << integers[i] << " ";
        } else if (i == 2) {
            Mergesort(integers, 20);
            cout << "By Iterative Merge Sort: ";
            for (int i = 1; i < 21; i++) cout << integers[i] << " ";
        } else if (i == 3) {
            int link[21]; fill(link+1, link + 21, 0);
            cout << "By Recursive Merge Sort: ";
            int j = rMergeSort(integers, link, 1, 20);
            for (int i = 0; i < 20; i++) {
                cout << integers[j] << " ";
                j = link[j];
            }
        } else {
            HeapSort(integers, 20);
            cout << "By Heap Sort: ";
            for (int i = 1; i < 21; i++) cout << integers[i] << " ";
        }
        cout << endl;
    }

    cout << "============================================================================================ float ============================================================================================" << endl;
    // Generate and print random floating point numbers
    float floats[20];
    for (int i = 0; i < 5; i++) {
        generateRandomFloats(floats);
        std::cout << "Random Floats: ";
        for (int i = 0; i < 20; ++i) {
            std::cout << floats[i] << ' ';
        }
        std::cout << std::endl;
        if (i == 0) {
            InsertionSort(floats, 20);
            cout << "By Insertion Sort: ";
            for (int i = 1; i < 21; i++) cout << floats[i] << " ";
        } else if (i == 1) {
            QuickSort(floats, 1, 20);
            cout << "By Quick Sort: ";
            for (int i = 1; i < 21; i++) cout << floats[i] << " ";
        } else if (i == 2) {
            Mergesort(floats, 20);
            cout << "By Iterative Merge Sort: ";
            for (int i = 1; i < 21; i++) cout << floats[i] << " ";
        } else if (i == 3) {
            int link[21]; fill(link+1, link + 21, 0);
            cout << "By Recursive Merge Sort: ";
            int j = rMergeSort(floats, link, 1, 20);
            for (int i = 0; i < 20; i++) {
                cout << floats[j] << " ";
                j = link[j];
            }
        } else {
            HeapSort(floats, 20);
            cout << "By Heap Sort: ";
            for (int i = 1; i < 21; i++) cout << floats[i] << " ";
        }
        cout << endl;
    }

    cout << "============================================================================================ strings ============================================================================================" << endl;
    // Generate and print random strings
    std::string strings[20];
    for (int i = 0; i < 5; i++) {
        generateRandomStrings(strings);
        std::cout << "Random Strings: ";
        for (int i = 0; i < 20; ++i) {
            std::cout << strings[i] << ' ';
        }
        std::cout << std::endl;
        if (i == 0) {
            InsertionSort(strings, 20);
            cout << "By Insertion Sort: ";
            for (int i = 1; i < 21; i++) cout << strings[i] << " ";
        } else if (i == 1) {
            QuickSort(strings, 1, 20);
            cout << "By Quick Sort: ";
            for (int i = 1; i < 21; i++) cout << strings[i] << " ";
        } else if (i == 2) {
            Mergesort(strings, 20);
            cout << "By Iterative Merge Sort: ";
            for (int i = 1; i < 21; i++) cout << strings[i] << " ";
        } else if (i == 3) {
            int link[21]; fill(link+1, link + 21, 0);
            cout << "By Recursive Merge Sort: ";
            int j = rMergeSort(strings, link, 1, 20);
            for (int i = 0; i < 20; i++) {
                cout << strings[j] << " ";
                j = link[j];
            }
        } else {
            HeapSort(strings, 20);
            cout << "By Heap Sort: ";
            for (int i = 1; i < 21; i++) cout << strings[i] << " ";
        }
        cout << endl;
    }

    return 0;
}