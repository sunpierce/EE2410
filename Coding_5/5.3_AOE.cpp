#include <iostream> 
#include <stack>
#include <stdio.h>
using namespace std;

class Graph;
class Chain;
class Iterator;

class ChainNode {
    friend class Chain;
    friend class Graph;
    friend class Iterator;
    ChainNode(int e, int a, ChainNode * nxt): event(e), activity(a), link(nxt) {}
    private:
        int event, activity;
        ChainNode * link;
};

class Chain {
    friend class Graph;
    Chain() { first = 0; }
    class ChainIterator {
        public:
            ChainIterator(ChainNode * p = 0): current(p) {}
            int * operator->() const { return &current->event; }
            int& operator*() const { return current->activity; }
            int GetE() const { return current->event; }
            int GetA() const { return current->activity; }
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
        void TopologicalOrder();
        void Calculate();
        void Setup1();
        void Setup2();
    private:  
        int n, edge;
        int * count; // calculate the in-degree of every vertex
        Chain * adjLists, * invLists;
        int * ee, * le;
        int * e, * l;
        int * recordPrev, * recordSucc, * len;
};

Graph::Graph(int num)
{
    n = num;
    edge = 0;
    adjLists = new Chain[n];
    invLists = new Chain[n];
    count = new int[n];
    ee = le = e = l = 0;
    recordPrev = recordSucc = len = 0;
}

void Graph::TopologicalOrder()
{
    stack<int> s;
    for (int i = 0; i < n; i++) if (count[i] == 0) s.push(i);
    for (int i = 0; i < n; i++) {
        int j = s.top(); s.pop();
        cout << j << "  ";
        Chain::ChainIterator iter(adjLists[j].first);
        while (iter.NotNull()) {
            count[iter.GetE()]--;
            if (count[iter.GetE()] == 0) s.push(iter.GetE());
            iter++;
        }
    }
    cout << endl;
}

void Graph::Calculate()
{
    // record forward order in order[]
    int * order = new int[n];
    stack<int> s;
    for (int i = 0; i < n; i++) if (count[i] == 0) s.push(i);
    for (int i = 0; i < n; i++) {
        int j = s.top(); s.pop();
        order[i] = j;
        Chain::ChainIterator iter(adjLists[j].first);
        while (iter.NotNull()) {
            count[iter.GetE()]--;
            if (count[iter.GetE()] == 0) s.push(iter.GetE());
            iter++;
        }
    }
    // forward stage -> calculate ee[]
    ee = new int[n];   // early event time
    e = new int[edge+1]; // early activity time
    ee[0] = 0;
    for (int j = 1, i = order[j]; j < n; i = order[++j]) {
        Chain::ChainIterator it(invLists[i].first);
        for (; it.NotNull(); it++) {
            ee[i] = max(ee[i], ee[it.GetE()] + it.GetA());
        }
    }
    for (int i = 1; i <= edge; i++) e[i] = ee[recordPrev[i]];
    // backward stage -> calculate le[]
    le = new int[n]; // latest event time
    l = new int[edge+1];
    fill(le, le + n, 1e9);
    le[n-1] = ee[n-1];
    for (int j = n-2, i = order[j]; j >= 0; i = order[--j]) {
        Chain::ChainIterator it(adjLists[i].first);
        for (; it.NotNull(); it++) {
            le[i] = min(le[i], le[it.GetE()] - it.GetA());
        }
    }
    for (int i = 1; i <= edge; i++) l[i] = le[recordSucc[i]] - len[i];
    cout << "Earliest times of all events: " << endl;
    cout << " event i " << "  ee[i]  " << endl;
    for (int i = 0; i < n; i++) {
        printf("%9d", i);
        printf("%9d\n", ee[i]);
    }
    cout << "Latest times of all events: " << endl;
    cout << " event i " << "  le[i]  " << endl;
    for (int i = 0; i < n; i++) {
        printf("%9d", i);
        printf("%9d\n", le[i]);
    }
    cout << " Activity " << "  e(a_i)  " << "  l(a_i)  " << "   slack  " << " critical " << endl;
    for (int i = 1; i <= edge; i++) {
        printf("%6d", i);
        printf("%10d", e[i]);
        printf("%10d", l[i]);
        printf("%10d", l[i] - e[i]);
        if (l[i] - e[i] == 0) cout << "        YES   ";
        else cout << "        NO    ";
        cout << endl;
    }
    delete [] ee;
    delete [] e;
    delete [] le;
    delete [] l;
}

void Graph::Setup1()
{
    edge = 11;
    recordPrev = new int[edge+1];
    recordSucc = new int[edge+1];
    len = new int[edge+1];
    count[0] = 0;
    count[1] = count[2] = count[3] = count[5] = count[6] = 1;
    count[4] = count[7] = count[8] = 2;
    adjLists[0].first = new ChainNode(1, 6, new ChainNode(2, 4, new ChainNode(3, 5, 0)));
    adjLists[1].first = new ChainNode(4, 1, 0);
    adjLists[2].first = new ChainNode(4, 1, 0);
    adjLists[3].first = new ChainNode(5, 2, 0);
    adjLists[4].first = new ChainNode(6, 9, new ChainNode(7, 7, 0));
    adjLists[5].first = new ChainNode(7, 4, 0);
    adjLists[6].first = new ChainNode(8, 2, 0);
    adjLists[7].first = new ChainNode(8, 4, 0);
    adjLists[8].first = 0;
    invLists[0].first = 0;
    invLists[1].first = new ChainNode(0, 6, 0);
    invLists[2].first = new ChainNode(0, 4, 0);
    invLists[3].first = new ChainNode(0, 5, 0);
    invLists[4].first = new ChainNode(1, 1, new ChainNode(2, 1, 0));
    invLists[5].first = new ChainNode(3, 2, 0);
    invLists[6].first = new ChainNode(4, 9, 0);
    invLists[7].first = new ChainNode(4, 7, new ChainNode(5, 4, 0));
    invLists[8].first = new ChainNode(6, 2, new ChainNode(7, 4, 0));
    recordPrev[1] = 0; recordPrev[2] = 0; recordPrev[3] = 0;
    recordPrev[4] = 1; recordPrev[5] = 2; recordPrev[6] = 3;
    recordPrev[7] = 4; recordPrev[8] = 4; recordPrev[9] = 5;
    recordPrev[10] = 6; recordPrev[11] = 7;
    recordSucc[1] = 1; recordSucc[2] = 2; recordSucc[3] = 3;
    recordSucc[4] = 4; recordSucc[5] = 4; recordSucc[6] = 5;
    recordSucc[7] = 6; recordSucc[8] = 7; recordSucc[9] = 7;
    recordSucc[10] = 8; recordSucc[11] = 8;
    len[1] = 6; len[2] = 4; len[3] = 5; len[4] = 1; len[5] = 1;
    len[6] = 2; len[7] = 9; len[8] = 7; len[9] = 4; len[10] = 2;
    len[11] = 4;
}

void Graph::Setup2()
{
    edge = 14;
    recordPrev = new int[edge+1];
    recordSucc = new int[edge+1];
    len = new int[edge+1];
    count[0] = 0;
    count[1] = count[2] = count[5] = count[7] = 1;
    count[3] = count[4] = count[6] = count[8] = count[9] = 2;
    adjLists[0].first = new ChainNode(1, 5, new ChainNode(2, 6, 0));
    adjLists[1].first = new ChainNode(3, 3, 0);
    adjLists[2].first = new ChainNode(3, 6, new ChainNode(4, 3, 0));
    adjLists[3].first = new ChainNode(4, 3, new ChainNode(6, 4, new ChainNode(5, 4, 0)));
    adjLists[4].first = new ChainNode(6, 1, new ChainNode(7, 4, 0));
    adjLists[5].first = new ChainNode(9, 4, 0);
    adjLists[6].first = new ChainNode(8, 5, 0);
    adjLists[7].first = new ChainNode(8, 2, 0);
    adjLists[8].first = new ChainNode(9, 2, 0);
    adjLists[9].first = 0;
    invLists[0].first = 0;
    invLists[1].first = new ChainNode(0, 5, 0);
    invLists[2].first = new ChainNode(0, 6, 0);
    invLists[3].first = new ChainNode(1, 3, new ChainNode(2, 6, 0));
    invLists[4].first = new ChainNode(2, 3, new ChainNode(3, 3, 0));
    invLists[5].first = new ChainNode(3, 4, 0);
    invLists[6].first = new ChainNode(3, 4, new ChainNode(4, 1, 0));
    invLists[7].first = new ChainNode(4, 4, 0);
    invLists[8].first = new ChainNode(6, 5, new ChainNode(7, 2, 0));
    invLists[9].first = new ChainNode(5, 4, new ChainNode(8, 2, 0));
    recordPrev[1] = 0; recordPrev[2] = 0; recordPrev[3] = 1;
    recordPrev[4] = 2; recordPrev[5] = 2; recordPrev[6] = 3; 
    recordPrev[7] = 3; recordPrev[8] = 3; recordPrev[9] = 4;
    recordPrev[10] = 4; recordPrev[11] = 6; recordPrev[12] = 7;
    recordPrev[13] = 5; recordPrev[14] = 8;
    recordSucc[1] = 1; recordSucc[2] = 2; recordSucc[3] = 3;
    recordSucc[4] = 3; recordSucc[5] = 4; recordSucc[6] = 4;
    recordSucc[7] = 5; recordSucc[8] = 6; recordSucc[9] = 6;
    recordSucc[10] = 7; recordSucc[11] = 8; recordSucc[12] = 8;
    recordSucc[13] = 9; recordSucc[14] = 9;
    len[1] = 5; len[2] = 6; len[3] = 3; len[4] = 6; len[5] = 3;
    len[6] = 3; len[7] = 4; len[8] = 4; len[9] = 1; len[10] = 4;
    len[11] = 5; len[12] = 2; len[13] = 4; len[14] = 2;
}

int main()
{
    Graph G1(9), G2(10);
    G1.Setup1();
    G2.Setup2();
    cout << "============================= Figure 6.39 =============================" << endl;
    cout << "Topological order: ";
    G1.TopologicalOrder();
    G1.Setup1();
    G1.Calculate();
    cout << "critical path: " << endl;
    cout << "a1-a4-a8-a11" << endl;
    cout << "a1-a4-a7-a10" << endl;
    cout << "By reducing a1 from 6 to 1, we can decrease the project lenght by 5" << endl;
    cout << "============================= Figure 6.44 =============================" << endl;
    cout << "Topological order: ";
    G2.TopologicalOrder();
    G2.Setup2();
    G2.Calculate();
    cout << "critical path: " << endl;
    cout << "a2-a4-a6-a9-a11-a14" << endl;
    cout << "a2-a4-a6-a10-a12-a14" << endl;
    cout << "a2-a4-a8-a11-a14" << endl;
    cout << "By reducing a2 from 6 to 1, we can decrease the project lenght by 5" << endl;
    return 0;
}