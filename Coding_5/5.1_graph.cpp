#include <iostream>
#include <queue>
using namespace std;

class Graph;
class Chain;
class ChainIterator;

class ChainNode {
    friend class Chain;
    friend class Graph;
    friend class ChainIterator;
    ChainNode(int e, ChainNode * next): data(e), link(next) {}
    private:
        int data;
        ChainNode * link;
};

class Chain {
    friend class Graph;
    Chain() { first = 0; }
    class ChainIterator {
        public:
            ChainIterator(ChainNode * p = 0): current(p) {}
            int * operator->() const { return &current->data; }
            int& operator*() const { return current->data; }
            ChainIterator & operator++() { current = current->link; return *this; }
            ChainIterator operator++(int) // postincrement
            {
                ChainIterator old = *this;
                current = current->link;
                return old;
            } 
            bool NotNull() const { return current != 0; }
            bool NextNotNull() const { return (current->link) != 0; }
            bool operator!=(const ChainIterator r) const { return current != r.current; }
            bool operator==(const ChainIterator r) const { return current == r.current; }
        private: 
            ChainNode * current;
    };
    private:
        ChainNode * first;
};

class Graph {
    public:
        Graph(int num);
        void DFS(const int v);
        void DFSDrive(const int v);
        void BFS(int v);
        void Component();
        void DfnLow(const int x); // perform DFS at x and compute dfn and low simultaneously
        void DfnLow(const int u, const int v); // v is the parent of u
        void Setup1();
        void Setup2();
        void Setup3();
    private:
        bool * visited;
        bool * articulate;
        int n, e; // number of vertices and number of edges
        int * dfn, * low;
        int num; // for Dfnlow (record the number of iter. in DFS)
        Chain * adjLists;
};

Graph::Graph(int num) 
{
    n = num;
    e = 0;
    dfn = low = 0; 
    visited = articulate = 0;
    num = 1;
    adjLists = new Chain[n];
}

void Graph::DFSDrive(const int v)
{
    visited = new bool[n];
    fill(visited, visited + n, 0);
    DFS(v);
    cout << endl;
    delete [] visited;
}

void Graph::DFS(const int v)
{
    visited[v] = true;
    cout << v << "  ";
    Chain::ChainIterator iter(adjLists[v].first); // first is already the next element
    if (!iter.NotNull()) return;
    int w = *iter;
    while (1) {
        if (!visited[w]) DFS(w);
        if (iter.NextNotNull()) w = *(++iter);
        else return;
    }
}

void Graph::BFS(int v)
{
    visited = new bool[n];
    fill(visited, visited + n, false);
    visited[v] = true;
    queue<int> q;
    q.push(v);
    while (!q.empty()) {
        v = q.front(); q.pop();
        cout << v << "  ";
        Chain::ChainIterator iter(adjLists[v].first);
        for (; iter.NotNull(); iter++) {
            if (!visited[*iter]) {
                q.push(*iter);
                visited[*iter] = true;
            }
        }
    }
    cout << endl;
    delete [] visited;
}

void Graph::Component()
{
    visited = new bool[n];
    fill(visited, visited + n, false);
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            cout << "{  ";
            DFS(i);
            cout << "} ";
        }
    }
    cout << endl;
    delete [] visited;
}

void Graph::DfnLow(const int x)
{
    num = 1;
    int ct = 0;
    dfn = new int[n];
    low = new int[n];
    articulate = new bool[n];
    fill(articulate, articulate + n, false);
    fill(dfn, dfn + n, 0);
    fill(low, low + n, 0);
    DfnLow(x, -1); // x is root
    for (Chain::ChainIterator i(adjLists[3].first); i.NotNull(); i++) ct++;
    if (ct > 2) articulate[3] = true;
    else articulate[3] = false;
    for (int i = 0; i < n; i++) {
        cout << "Vertex " << i << ": " << "dfn: " << dfn[i] << ", " << "low: " << low[i] << ", " << "articulate: " << ((articulate[i] == 1)? "Yes": "No") << endl;
    }
    delete [] dfn;
    delete [] low;
    delete [] articulate;
}

void Graph::DfnLow(const int u, const int v)
{
    dfn[u] = low[u] = num++;
    for (Chain::ChainIterator i(adjLists[u].first); i.NotNull(); i++) {
        if (dfn[*i] == 0) {
            DfnLow(*i, u);
            low[u] = min(low[u], low[*i]);
            if (low[*i] >= dfn[u] && u != 3) articulate[u] = true;
        } else if (*i != v) { 
            low[u] = min(low[u], dfn[*i]); // back edge
        }
    }
}

void Graph::Setup1()
{
    adjLists[0].first = new ChainNode(1, new ChainNode(2,0));
    adjLists[1].first = new ChainNode(0, new ChainNode(3,0));
    adjLists[2].first = new ChainNode(0, new ChainNode(3,0));
    adjLists[3].first = new ChainNode(1, new ChainNode(2,0));
    adjLists[4].first = new ChainNode(5,0);
    adjLists[5].first = new ChainNode(4, new ChainNode(6,0));
    adjLists[6].first = new ChainNode(5, new ChainNode(7,0));
    adjLists[7].first = new ChainNode(6,0);
}

void Graph::Setup2()
{
    adjLists[0].first = new ChainNode(1, new ChainNode(2,0));
    adjLists[1].first = new ChainNode(0, new ChainNode(3, new ChainNode(4,0)));
    adjLists[2].first = new ChainNode(0, new ChainNode(5, new ChainNode(6,0)));
    adjLists[3].first = new ChainNode(1, new ChainNode(7,0));
    adjLists[4].first = new ChainNode(1, new ChainNode(7,0));
    adjLists[5].first = new ChainNode(2, new ChainNode(7,0));
    adjLists[6].first = new ChainNode(2, new ChainNode(7,0));
    adjLists[7].first = new ChainNode(3, new ChainNode(4, new ChainNode(5, new ChainNode(6,0))));
}

void Graph::Setup3()
{
    adjLists[0].first = new ChainNode(1,0);
    adjLists[1].first = new ChainNode(0, new ChainNode(2, new ChainNode(3,0)));
    adjLists[2].first = new ChainNode(1, new ChainNode(4,0));
    adjLists[3].first = new ChainNode(1, new ChainNode(4, new ChainNode(5,0)));
    adjLists[4].first = new ChainNode(2, new ChainNode(3,0));
    adjLists[5].first = new ChainNode(3, new ChainNode(6, new ChainNode(7,0)));
    adjLists[6].first = new ChainNode(5, new ChainNode(7,0));
    adjLists[7].first = new ChainNode(5, new ChainNode(8, new ChainNode(9,0)));
    adjLists[8].first = new ChainNode(7,0);
    adjLists[9].first = new ChainNode(7,0);
}

int main()
{
    Graph G1(8), G2(8), G3(10);
    G1.Setup1(); G2.Setup2(); G3.Setup3();
    cout << "============================= G1 =============================" << endl;
    cout << "DFS traversal start at 0: ";
    G1.DFSDrive(0);
    cout << "DFS traversal start at 3: ";
    G1.DFSDrive(3);
    cout << "DFS traversal start at 7: ";
    G1.DFSDrive(7);
    cout << "BFS traversal start at 0: ";
    G1.BFS(0);
    cout << "BFS traversal start at 3: ";
    G1.BFS(3);
    cout << "BFS traversal start at 7: ";
    G1.BFS(7);
    cout << "Component of G1: ";
    G1.Component();
    cout << "dfn and low of G1: " << endl;
    G1.DfnLow(3);
    cout << "============================= G2 =============================" << endl;
    cout << "DFS traversal start at 0: ";
    G2.DFSDrive(0);
    cout << "DFS traversal start at 3: ";
    G2.DFSDrive(3);
    cout << "DFS traversal start at 7: ";
    G2.DFSDrive(7);
    cout << "BFS traversal start at 0: ";
    G2.BFS(0);
    cout << "BFS traversal start at 3: ";
    G2.BFS(3);
    cout << "BFS traversal start at 7: ";
    G2.BFS(7);
    cout << "Component of G2: ";
    G2.Component();
    cout << "dfn and low of G2: " << endl;
    G2.DfnLow(3);
    cout << "============================= G3 =============================" << endl;
    cout << "DFS traversal start at 0: ";
    G3.DFSDrive(0);
    cout << "DFS traversal start at 3: ";
    G3.DFSDrive(3);
    cout << "DFS traversal start at 7: ";
    G3.DFSDrive(7);
    cout << "BFS traversal start at 0: ";
    G3.BFS(0);
    cout << "BFS traversal start at 3: ";
    G3.BFS(3);
    cout << "BFS traversal start at 7: ";
    G3.BFS(7);
    cout << "Component of G3: ";
    G3.Component();
    cout << "dfn and low of G3: " << endl;
    G3.DfnLow(3);
    return 0;
}