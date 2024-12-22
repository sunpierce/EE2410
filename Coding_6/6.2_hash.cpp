#include <iostream>
#include <stdio.h>
using namespace std;

const int SIZE = 26; // table size

class LinearProbingTable {
    public:  
        LinearProbingTable();
        int HashFunction(const char * key) { return (key[0] - 'a');}
        void Insert(const char * key);
        bool Get(const char * key); // return true if found otherwise return false
        void Display();
    private:
        char * ht[SIZE];
};

LinearProbingTable::LinearProbingTable()
{
    for (int i = 0; i < SIZE; i++) ht[i] = 0;
}

bool LinearProbingTable::Get(const char * key)
{
    int i = HashFunction(key), j = i;
    while (ht[j]) {
        if (!strcmp(ht[j], key)) return true;
        j = (j + 1) % SIZE;
        if (j == i) return false;
    }
    return false;
}

void LinearProbingTable::Insert(const char * key)
{
    int i = HashFunction(key), j = i;
    while (ht[j]) {
        j = (j + 1) % SIZE;
        if (j == i) {
            cout << "The table is full." << endl;
            return;
        }
    }
    ht[j] = new char[strlen(key)+1];
    strcpy(ht[j], key);
    cout << "Successful insertion for key [" << key << "]" << endl;
}

void LinearProbingTable::Display()
{
    printf("%5s%10s\n", "i", "ht[i]");
    for (int i = 0; i < SIZE; i++) {
        printf("%5d%10s\n", i, (ht[i] == 0)? "-": ht[i]);
    }
}

class ChainingTable;

class ChainNode {
    friend class ChainingTable;
    public:
        ChainNode(const char * k, ChainNode * n)
        {
            key = new char[strlen(k)+1];
            strcpy(key, k);
            next = n;
        }
    private:
        char * key;
        ChainNode * next;
};

class ChainingTable {
    public:  
        ChainingTable();
        int HashFunction(const char * key) { return (key[0] - 'a');}
        void Insert(const char * key);
        bool Get(const char * key); // return true if found otherwise return false
        void Display();
    private:
        ChainNode * ht[SIZE];
};

ChainingTable::ChainingTable()
{
    for (int i = 0; i < SIZE; i++) ht[i] = NULL;
}

void ChainingTable::Insert(const char * key)
{
    int i = HashFunction(key);
    ChainNode * node = new ChainNode(key,0);
    if (ht[i] == 0) ht[i] = node;
    else {
        ChainNode * cur = ht[i];
        while (cur->next != NULL) cur = cur->next;
        cur->next = node;
    }
    cout << "Successful insertion for key [" << key << "]" << endl;
}

bool ChainingTable::Get(const char * key)
{
    int i = HashFunction(key);
    ChainNode * cur = ht[i];
    while (cur) {
        if (!strcmp(cur->key, key)) return true;
        cur = cur->next;
    }
    return false;
}

void ChainingTable::Display()
{
    for (int i = 0; i < SIZE; i++) {
        printf("%5d  ", i);
        if (ht[i] == 0) cout << "NULL" << endl;
        else {
            ChainNode * cur = ht[i];
            while (cur) {
                cout << cur->key << " --> ";
                cur = cur->next;
            }
            cout << "NULL" << endl;
        }
    }
}

int main()
{
    cout << "============== Linear probing hash table ==============" << endl;
    LinearProbingTable ht1;
    const char * keys[] = {"aafe", "add", "akls", "anhlk", "akd", "edl", "g", "klja", "qoi", "zlk"};
    for (int i = 0; i < 10; i++) ht1.Insert(keys[i]);
    cout << "Hash Table: " << endl;
    ht1.Display();
    const char * search[] = {"anhlk", "akd", "aafe", "qo", "b"};
    for (int i = 0; i < 5; i++) {
        if (ht1.Get(search[i])) cout << "Key [" << search[i] << "] found" << endl;
        else cout << "Key [" << search[i] << "] not found" << endl;
    }
    cout << "============== Chaining hash table ==============" << endl;
    ChainingTable ht2;
    for (int i = 0; i < 10; i++) ht2.Insert(keys[i]);
    cout << "Hash Table: " << endl;
    ht2.Display();
    for (int i = 0; i < 5; i++) {
        if (ht2.Get(search[i])) cout << "Key [" << search[i] << "] found" << endl;
        else cout << "Key [" << search[i] << "] not found" << endl;
    }
    return 0;
}