#include <iostream>
#include <stdio.h>
using namespace std;

class Graph {
    public:  
        Graph(int num);
        void printPath(int u);
        void Dijkstra(const int n, const int v);
        void BellmanFord(const int n, const int v);
        void Floyd(const int n);
        void Setup1();
        void Setup2();
        void Setup3();
        void Setup4();
        void Setup5();
        void Setup6();
    private:  
        int n; // number of vertices
        int * dist,  * p;
        bool * s;
        int ** adjMatrix;
};

Graph::Graph(int num)
{
    n = num;
    dist = p = 0;
    s = 0;
    adjMatrix = new int *[n];
    for (int i = 0; i < n; i++) adjMatrix[i] = new int[n];

}

void Graph::printPath(int u)
{
    if (p[u] == 1e9) cout << u << " ";
    else {
        printPath(p[u]);
        cout << u << " ";
    }
}

void Graph::Dijkstra(const int n, const int v)
{
    dist = new int[n];
    s = new bool[n];
    p = new int[n]; // record the prev
    int * order = new int[n-1]; // record the order of vertex selected
    for (int i = 0; i < n; i++) {
        s[i] = false;
        dist[i] = adjMatrix[v][i];
    }
    for (int i = 0; i < n; i++) {
        if (i != v) p[i] = v;
        else p[i] = 1e9;
    }
    s[v] = true;
    dist[v] = 0;
    printf("%10s", "iteration"); printf("%10s", "vertex");
    for (int i = 0; i < n; i++) printf("%8d", i); 
    cout << endl;
    printf("%10s", "Initial"); printf("%10s", "----");
    for (int i = 0; i < n; i++) {
        if (dist[i] == 1e9) printf("%8s", "Inf.");
        else printf("%8d", dist[i]);
    }
    cout << endl;
    // n-1 iterations
    for (int i = 0; i < n-1; i++) {
        // choose u that is not in s and has smallest dist[u]
        int u;
        int tmp_min = 1e9;
        for (int i = 0; i < n; i++) {
            if (!s[i] && (dist[i] < tmp_min)) {
                tmp_min = dist[i];
                u = i;
            }
        }
        // should deal with the case where some vertex is unreachable
        if (u == order[i-1]) {
            order[i] = -1;
            break;
        }
        printf("%10d", i+1);
        printf("%10d", u);
        order[i] = u;
        s[u] = true; // u becomes a member of s
        for (int w = 0; w < n; w++) {
            if (!s[w] && dist[u] + adjMatrix[u][w] < dist[w]) {
                dist[w] = dist[u] + adjMatrix[u][w];
                p[w] = u;
            }
        }
        for (int i = 0; i < n; i++) {
            if (dist[i] == 1e9) printf("%8s", "Inf.");
            else printf("%8d", dist[i]);
        }
        cout << endl;
    }
    for (int i = 0; i < n-1; i++) {
        if (order[i] == -1) break;
        cout << "Path(" << i << ")  ";
        printPath(order[i]);
        cout << "-> Length: " << dist[order[i]] << endl;
    }
    delete [] dist;
    delete [] s;
    delete [] p;
    delete [] order;
}

void Graph::BellmanFord(const int n, const int v)
{
    dist = new int[n];
    int * old_dist = new int[n]; // record dist of previous step
    // dist^1[] initialization
    for (int i = 0; i < n; i++) {
        dist[i] = adjMatrix[v][i];
        old_dist[i] = adjMatrix[v][i];
    }
    printf("%s\n", "          dist^k[]");
    printf("%3s", "k");
    for (int i = 0; i < n; i++) printf("%8d", i);
    cout << endl;
    printf("%3d", 1);
    for (int i = 0; i < n; i++) {
        if (dist[i] == 1e9) printf("%8s", "Inf.");
        else printf("%8d", dist[i]);
    }
    cout << endl;
    // dist^2 ~ dist^{n-1}
    for (int k = 2; k <= n-1; k++) {
        for (int u = 0; u < n; u++) {
            if (u != v) {
                for (int i = 0; i < n; i++) {
                    if (dist[u] > old_dist[i] + adjMatrix[i][u])
                        dist[u] = old_dist[i] + adjMatrix[i][u];
                }
            }
        }
        printf("%3d", k);
        for (int i = 0; i < n; i++) {
            if (dist[i] == 1e9) printf("%8s", "Inf.");
            else printf("%8d", dist[i]);
            old_dist[i] = dist[i];
        }
        cout << endl;
    }
    delete [] dist;
    delete [] old_dist;
}

void Graph::Floyd(const int n)
{
    int A[n][n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) A[i][j] = adjMatrix[i][j];
    cout << "A^{-1} : " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (A[i][j] == 1e9) printf("%3s", "X");
            else printf("%3d", A[i][j]);
        }
        cout << endl;
    }
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (A[i][j] > (A[i][k] + A[k][j]) && A[i][k] != 1e9 && A[k][j] != 1e9)
                    A[i][j] = A[i][k] + A[k][j];
        cout << endl;
        cout << "A^{"<< k <<"} : " << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (A[i][j] == 1e9) printf("%3s", "X");
                else printf("%3d", A[i][j]);
            }
            cout << endl;
        }
    }
    
}

void Graph::Setup1()
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j) adjMatrix[i][j] = 0;
            else adjMatrix[i][j] = 1e9;
        }
    adjMatrix[1][0] = 300; adjMatrix[2][0] = 1000; adjMatrix[2][1] = 800;
    adjMatrix[3][2] = 1200; adjMatrix[4][3] = 1500; adjMatrix[4][5] = 250;
    adjMatrix[5][3] = 1000; adjMatrix[5][6] = 900; adjMatrix[5][7] = 1400;
    adjMatrix[6][7] = 1000; adjMatrix[7][0] = 1700;
}

void Graph::Setup2()
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j) adjMatrix[i][j] = 0;
            else adjMatrix[i][j] = 1e9;
        }
    adjMatrix[0][1] = 50; adjMatrix[0][2] = 45; adjMatrix[0][3] = 10;
    adjMatrix[1][2] = 10; adjMatrix[1][3] = 15; adjMatrix[2][4] = 30;
    adjMatrix[3][0] = 20; adjMatrix[3][4] = 15; adjMatrix[4][1] = 20;
    adjMatrix[4][2] = 35; adjMatrix[5][4] = 3;
}

void Graph::Setup3()
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j) adjMatrix[i][j] = 0;
            else adjMatrix[i][j] = 1e9;
        }
    adjMatrix[0][1] = 20; adjMatrix[0][2] = 15; adjMatrix[1][0] = 2;
    adjMatrix[1][4] = 10; adjMatrix[1][5] = 30; adjMatrix[2][3] = 4;
    adjMatrix[2][5] = 10; adjMatrix[4][3] = 15; adjMatrix[5][3] = 4;
    adjMatrix[5][4] = 10;
}

void Graph::Setup4()
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j) adjMatrix[i][j] = 0;
            else adjMatrix[i][j] = 1e9;
        }
    adjMatrix[0][1] = 6;
    adjMatrix[0][2] = 5;
    adjMatrix[0][3] = 5;
    adjMatrix[1][4] = -1;
    adjMatrix[2][1] = -2;
    adjMatrix[2][4] = 1;
    adjMatrix[3][2] = -2;
    adjMatrix[3][5] = -1;
    adjMatrix[4][6] = 3;
    adjMatrix[5][6] = 3;
}

void Graph::Setup5()
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j) adjMatrix[i][j] = 0;
            else adjMatrix[i][j] = 1e9;
        }
    adjMatrix[0][1] = 7;
    adjMatrix[0][2] = 5;
    adjMatrix[1][2] = -5;
}

void Graph::Setup6()
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j) adjMatrix[i][j] = 0;
            else adjMatrix[i][j] = 1e9;
        }
    adjMatrix[0][1] = 4;
    adjMatrix[0][2] = 11;
    adjMatrix[1][0] = 6;
    adjMatrix[1][2] = 2;
    adjMatrix[2][0] = 3;
}

int main()
{
    cout << "Dijkstra's algorithm for G1: " << endl;
    Graph G1(8);
    G1.Setup1();
    G1.Dijkstra(8,4);
    cout << endl;
    cout << "Dijkstra's algorithm for G1': " << endl;
    Graph G2(6);
    G2.Setup2();
    G2.Dijkstra(6,0);
    cout << endl;
    cout << "Dijkstra's algorithm for G1'': " << endl;
    Graph G3(6);
    G3.Setup3();
    G3.Dijkstra(6,0);
    cout << "=============================================================================" << endl;
    cout << "Bellman-Ford Algorithm for G2: " << endl;
    Graph G4(7);
    G4.Setup4();
    G4.BellmanFord(7,0);
    cout << endl;
    cout << "Bellman-Ford Algorithm for G2': " << endl;
    Graph G5(3);
    G5.Setup5();
    G5.BellmanFord(3,0);
    cout << "=============================================================================" << endl;
    cout << "Floyd's Algorithm for G3: " << endl;
    Graph G6(3);
    G6.Setup6();
    G6.Floyd(3);
    cout << endl;
    cout << "Floyd's Algorithm for G2: " << endl;
    Graph G7(7);
    G7.Setup4();
    G7.Floyd(7);
    return 0;
}