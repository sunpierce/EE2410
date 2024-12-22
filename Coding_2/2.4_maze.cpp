#include <iostream>
#include <stack> // using stack STL
using namespace std;

typedef struct {
    int di, dj;
} Offsets;

enum directions {N, NE, E, SE, S, SW, W, NW};

Offsets Move[8] = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };

typedef struct {
    int x, y, dir;
} Items;

int maze[14][17];
int visited[14][17];

void print_stack(stack<Items>& s) 
{
    stack<Items> temp;
    // reverse the element in the stack
    while (!s.empty()) {
        temp.push(s.top()); s.pop();
    }
    int i = 1;
    while (!temp.empty()) {
        Items tmp = temp.top(); temp.pop();
        cout << "Step " << i << ": ";
        cout << "(" << tmp.x << "," << tmp.y << ",";
        switch (tmp.dir - 1) {
            case N: cout << "N";
            break;
            case NE: cout << "NE";
            break;
            case E: cout << "E";
            break;
            case SE: cout << "SE";
            break;
            case S: cout << "S";
            break;
            case SW: cout << "SW";
            break;
            case W: cout << "W";
            break;
            case NW: cout << "NW";
            break;
        }
        cout << ")" << endl;
        i++;
    }
}

void Solve(const int m, const int p)
{
    stack<Items> s;
    // Starting from (1,1,E)
    visited[1][1] = 1;
    Items temp;
    temp.x = temp.y = 1;
    temp.dir = E;
    s.push(temp);

    while (!s.empty()) {
        temp = s.top(); s.pop();
        int i = temp.x, j = temp.y, d = temp.dir;
        while (d < 8) { // traverse all 8 possible directions
            int g = i + Move[d].di; // the new position
            int h = j + Move[d].dj;
            if ((g == m) && (h == p)) { // reach the end
                print_stack(s); // print out the path
                cout << "The last two positions visited is ";
                cout << "(" << i << "," << j << ") and ";
                cout << "(" << m << "," << p << ")." << endl;
                return; 
            }
            if (!visited[g][h] && !maze[g][h]) { // make sure (g,h) is available
                visited[g][h] = 1;
                temp.x = i; temp.y = j; temp.dir = d+1; // record the prev step
                s.push(temp); // make sure we can return from the death path
                i = g; j = h; d = N; // traverse from N
            } else d++; // otherwise change the direction
        }
    }
    cout << "No path in the maze" << endl;
}

int main()
{
    FILE * MAZE;
    int m, p, i, j;
    MAZE = fopen("maze.txt", "r");
    // check whether the file is read successfully
    if (MAZE == NULL) {
        fprintf(stderr, "Couldn't open %s: %s\n", "maze.txt", strerror(errno));
        exit(1);
    }
    fscanf(MAZE, "%d", &m);
    fscanf(MAZE, "%d", &p);
    for (i = 0; i < (m + 2); i++) {
        for (j = 0; j < (p + 2); j++) {
            if (i == 0 || i == m + 1)
                maze[i][j] = 1;
            else if (j == 0 || j == p + 1) 
                maze[i][j] = 1;
            else fscanf(MAZE, "%d", &maze[i][j]);
        }
    }
    Solve(m,p);

    return 0;
}