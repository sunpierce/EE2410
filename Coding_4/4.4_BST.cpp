# include <iostream>
# include <queue>
using namespace std;

template <class K, class E> 
class BST;

template <class K, class E>
ostream& operator<<(ostream& os, const BST<K,E>& b);

template <class T> 
class TreeNode {
    template <class K, class E> 
    friend class BST;
    template <class K, class E> 
    friend ostream& operator<<(ostream& os, const BST<K,E>& b);
    public: 
        TreeNode();
        TreeNode(TreeNode<T> * a, T item, TreeNode<T> * b);
    private:
        T data;
        TreeNode<T> * leftChild, * rightChild;
        int leftSize;
};

template <class T> 
TreeNode<T>::TreeNode()
{
    data.first = data.second = 0;
    leftChild = rightChild = 0;
    leftSize = 0;
}

template <class T> 
TreeNode<T>::TreeNode(TreeNode<T> * a, T item, TreeNode<T> * b)
{
    data = item;
    leftChild = a;
    rightChild = b;
    leftSize = 1;
}

template <class K, class E>
class Dictionary {
    public:
        virtual bool IsEmpty() const = 0;
        virtual pair<K, E> *Get(const K &x) const = 0;
        virtual void Insert(const pair<K, E> &thePar) = 0;
        virtual void Delete(const K &x) = 0;
};

template <class K, class E> 
class BST: public Dictionary<K,E> {
    friend ostream& operator<<<K, E>(ostream& os, const BST<K,E>& b);
    public: 
        BST() { root = 0; }
        bool IsEmpty() const { return root == 0; }
        pair<K,E> * Get(const K& key) const;
        void Insert(const pair<K,E>& x);
        void Delete(const K& x);
        pair<K,E> * RankGet(int r);
        void Split(const K& k, BST<K,E>& l, pair<K,E> *& mid, BST<K,E>& r);
    private: 
        TreeNode< pair<K,E> > * root;
};

template <class K, class E> 
pair<K,E> * BST<K,E>::Get(const K& key) const 
{
    TreeNode< pair<K,E> > * cur = root;
    while (cur) {
        if (key < cur->data.first) cur = cur->leftChild;
        else if (key > cur->data.first) cur = cur->rightChild;
        else return &cur->data;
    }
    return 0;
}

template <class K, class E> 
pair<K,E> * BST<K,E>::RankGet(int r)
{
    TreeNode< pair<K,E> > * cur = root;
    while (cur) {
        if (r < cur->leftSize) cur = cur->leftChild;
        else if (r > cur->leftSize) {
            r -= cur->leftSize;
            cur = cur->rightChild;
        } else return &cur->data;
    }
    return 0;
}

template <class K, class E> 
void BST<K,E>::Insert(const pair<K,E>& thePair)
{
    // insert thePair into the BST
    // search for thePair.first, pp parent of p
    TreeNode< pair<K,E> > * p = root, * pp = 0;
    while (p) {
        pp = p;
        if (thePair.first < p->data.first) {
            p->leftSize++;
            p = p->leftChild;
        }
        else if (thePair.first > p->data.first) p = p->rightChild;
        else {
            // duplicate, update associated element
            p->data.second = thePair.second;
            return;
        }
    }
    // perform insertion
    p = new TreeNode< pair<K,E> >(0,thePair,0);
    if (root) {
        if (thePair.first < pp->data.first) pp->leftChild = p;
        else pp->rightChild = p;
    } else root = p;
}

template <class K, class E>
void BST<K, E>::Delete(const K &x)
{
    int state = 0;
    TreeNode<pair<K, E> > * currentNode = root, * pointer = 0;
    pair<K, E> *exist = 0;
    exist = Get(x);
    if (exist != 0) {
        while (currentNode) {
            if (x < currentNode->data.first) {
                currentNode->leftSize--;
                pointer = currentNode;
                currentNode = currentNode->leftChild;
                state = 0;
            } else if (x > currentNode->data.first) {
                pointer = currentNode;
                currentNode = currentNode->rightChild;
                state = 1;
            } else {
                if ((currentNode->leftChild == 0) && (currentNode->rightChild == 0))
                {
                    if (state == 0) pointer->leftChild = 0;
                    else pointer->rightChild = 0;
                    delete currentNode;
                } else if (currentNode->leftChild != 0 && currentNode->rightChild == 0)
                {
                    if (state == 0) pointer->leftChild = currentNode->leftChild;
                    else pointer->rightChild = currentNode->leftChild;
                    delete currentNode;
                } else if ((currentNode->leftChild == 0) && (currentNode->rightChild != 0))
                {
                    if (state == 0) pointer->leftChild = currentNode->rightChild;
                    else pointer->rightChild = currentNode->rightChild;
                    delete currentNode;
                } else {
                    TreeNode<pair<K, E> > * max = currentNode->leftChild;
                    TreeNode<pair<K, E> > * Pre_Max = 0;
                    while (max->rightChild) {
                        Pre_Max = max;
                        max = max->rightChild;
                    }
                    if (state == 0) pointer->leftChild = max;
                    else pointer->rightChild = max;
                    max->rightChild = currentNode->rightChild;
                    if (Pre_Max){
                        max->leftSize = currentNode->leftSize;
                        max->leftChild = currentNode->leftChild;
                        Pre_Max->rightChild = 0;
                    }
                    delete currentNode;
                }
                break;
            }
        }
    }
}


template <class K, class E> 
ostream& operator<<(ostream& os, const BST<K,E>& b)
{
    queue< TreeNode< pair<K,E> > * > q;
    TreeNode< pair<K,E> > * cur = b.root;
    while (cur) {
        os << "(" << cur->data.first << "," << cur->data.second << ") ";
        if (cur->leftChild) q.push(cur->leftChild);
        if (cur->rightChild) q.push(cur->rightChild);
        if (q.empty()) return os;
        cur = q.front(); q.pop();
    }
    return os;
}

template <class K, class E> 
void BST<K,E>::Split(const K& k, BST<K,E>& small, pair<K,E> *& mid, BST<K,E>& big)
{
    if (!root) {
        small.root = 0;
        big.root = 0;
        return;
    }
    TreeNode<pair<K, E> > * sHead = new TreeNode<pair<K, E> >();
    TreeNode<pair<K, E> > * s = sHead;
    TreeNode<pair<K, E> > * bHead = new TreeNode<pair<K, E> >();
    TreeNode<pair<K, E> > * b = bHead;
    TreeNode<pair<K, E> > * currentNode = root;
    while (currentNode) {
        if (k < currentNode->data.first) {
            b->leftChild = currentNode;
            b = currentNode;
            currentNode = currentNode->leftChild;
        }
        else if (k > currentNode->data.first) {
            s->rightChild = currentNode;
            s = currentNode;
            currentNode = currentNode->rightChild;
        } else {
            s->rightChild = currentNode->leftChild;
            b->leftChild = currentNode->rightChild;
            small.root = sHead->rightChild;
            delete sHead;
            big.root = bHead->leftChild;
            delete bHead;
            mid = new pair<K, E>(currentNode->data.first, currentNode->data.second);
            delete currentNode;
            return;
        }
    }
    s->rightChild = 0;
    b->leftChild = 0;
    small.root = sHead->rightChild;
    delete sHead;
    big.root = bHead->leftChild;
    delete bHead;
    mid = 0;
    return;
}

int main()
{
    BST<int,char> B1, B2, B3;
    pair<int,char> * get;
    int in;
    pair<int, char> n1(50, 'A'), n2(5, 'B'), n3(30, 'C'), n4(40, 'D');
    pair<int, char> n5(80, 'E'), n6(35, 'F'), n7(2, 'G'), n8(20, 'H');
    pair<int, char> n9(15, 'I'), n10(60, 'J'), n11(70, 'K'), n12(8, 'L');
    pair<int, char> n13(10, 'M');
    B1.Insert(n1); B1.Insert(n2); B1.Insert(n3); B1.Insert(n4);
    B1.Insert(n5); B1.Insert(n6); B1.Insert(n7); B1.Insert(n8);
    B1.Insert(n9); B1.Insert(n10); B1.Insert(n11); B1.Insert(n12);
    B1.Insert(n13);
    cout << "Level order traversal of the BST: ";
    cout << B1 << endl;
    if (B1.IsEmpty()) cout << "BST is empty." << endl;
    else cout << "BST is non-empty." << endl;
    cout << "Enter a rank value: ";
    cin >> in;
    get = B1.RankGet(in);
    cout << "The corresponding element is " << get->second << endl;
    cout << "Enter a key value: ";
    cin >> in;
    get = B1.Get(in);
    cout << "The corresponding element is " << get->second << endl;
    cout << "Enter a key value to split: ";
    cin >> in;
    B1.Split(in, B2, get, B3);
    cout << "Level order traversal of small BST: " << B2 << endl;
    cout << "Mid pair: " << "(" << get->first << "," << get->second << ")" << endl;
    cout << "Level order traversal of big BST: " << B3 << endl;
    cout << "Enter a key value to delete in the big BST: ";
    cin >> in;
    B3.Delete(in);
    cout << "Big BST after delete: " << B3 << endl;
    return 0;
}