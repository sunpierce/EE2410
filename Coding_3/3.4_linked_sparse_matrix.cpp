#include <iostream> 
using namespace std;

struct Triple 
{
    int row, col, value;
};

class Matrix;

class MatrixNode 
{
    friend class Matrix;
    friend istream &operator>>(istream&, Matrix&);
    friend ostream &operator<<(ostream&, Matrix&);
    private:
        MatrixNode *down, *right;
        bool head;
        union{
            MatrixNode *next;
            Triple triple;
        };
        MatrixNode(bool b, Triple *t);
};

MatrixNode::MatrixNode(bool b, Triple * t)
{
    head = b;
    if (b) right = down = this;
    else triple = *t;
}

class Matrix 
{
    friend istream& operator>>(istream&, Matrix&);
    friend ostream& operator<<(ostream&, Matrix&);
    public:
        Matrix(){}; // default constructor
        ~Matrix(); // default destructor
        Matrix(const Matrix &b); // copy constructor
        Matrix * operator+(const Matrix& b) const;
        Matrix * operator*(const Matrix& b) const;
        Matrix * Transpose();
    private:
        MatrixNode * headnode = NULL;
        MatrixNode * av = NULL;
};

Matrix:: ~Matrix()
{
    if (!headnode) 
        return;
    
    MatrixNode *x = headnode->right;
    
    headnode->right = av;
    av = headnode;
    
    while (x != headnode) {
        MatrixNode *y = x->right;
        x->right = av;
        av = y;
        x = x->next;
    }
    headnode = 0;
}

Matrix:: Matrix(const Matrix &b)
{
    Triple s, t;
    MatrixNode *newHead;
    s.row = b.headnode->triple.row;
    s.col = b.headnode->triple.col;
    s.value = b.headnode->triple.value;
    int p = max(s.row, s.col);
    newHead = new MatrixNode(false, &s);
    if (p == 0) {
        newHead->right = newHead;
    }
    else {
        MatrixNode ** head = new MatrixNode* [p];
        for (int i = 0; i < p; i++) {
            head[i] = new MatrixNode(true, 0);
            head[i]->down = head[i]->next = head[i]->right = head[i];
        }
        int currentRow = 0;
        MatrixNode *last = head[0];
        MatrixNode *temp = b.headnode->right;
        MatrixNode *curr_row = b.headnode->right;
        while (curr_row != b.headnode) {
            last = head[currentRow];
            temp = curr_row->right;
            while (temp != curr_row) {
                t.col = temp->triple.col; t.row = temp->triple.row; t.value = temp->triple.value;
                last = last->right = new MatrixNode(false, &t);
                head[t.col]->next = ((head[t.col]->next)->down) = last;
                temp = temp->right;
            }
            last->right = head[currentRow];
            curr_row = curr_row->next;
            currentRow++;
        }
        for (int i = 0; i < p - 1; i++)
        head[i]->next->down = head[i];
        for (int i = 0; i < p - 1; i++)
        head[i]->next = head[i + 1];
        head[p - 1]->next = newHead;
        newHead->right = head[0];
    }
    headnode = newHead;
}

Matrix * Matrix::operator+(const Matrix& b) const
{
    Matrix * c;
    MatrixNode * temp1 = headnode->right, * temp2 = b.headnode->right;
    MatrixNode * curr_row_head, * curr_row_head_b;
    if (headnode->triple.col != b.headnode->triple.col || headnode->triple.row != b.headnode->triple.row) throw "Error!";
    Triple s;
    s.col = headnode->triple.col; 
    s.row = headnode->triple.row; 
    s.value = 0;
    c = new Matrix;
    c->headnode = new MatrixNode(false, &s);
    int p = max(headnode->triple.col, headnode->triple.row);
    MatrixNode ** head = new MatrixNode * [p];
    for (int i = 0; i < p; i++) {
        head[i] = new MatrixNode(true, 0);
        head[i]->next = head[i]->right = head[i];
    }
    int currentRow = 0;
    MatrixNode * last = head[0];
    while (temp1 != headnode && temp2 != b.headnode) {
        last = head[currentRow];
        curr_row_head = temp1; curr_row_head_b = temp2;
        temp1 = temp1->right; temp2 = temp2->right;
        while (temp1 != curr_row_head && temp2 != curr_row_head_b) {
            if (temp1->triple.row == temp2->triple.row) {
                if (temp1->triple.col > temp2->triple.col) {
                    Triple t;
                    t.row = temp1->triple.row; 
                    t.col = temp2->triple.col; 
                    t.value = temp2->triple.value;
                    last = last->right = new MatrixNode(false, &t);
                    head[t.col]->next = ((head[t.col]->next)->down) = last;
                    c->headnode->triple.value++;
                    temp2 = temp2->right;
                }
                else if (temp1->triple.col < temp2->triple.col) {
                    Triple t;
                    t.row = temp1->triple.row; 
                    t.col = temp1->triple.col; 
                    t.value = temp1->triple.value;
                    last = last->right = new MatrixNode(false, &t);
                    head[t.col]->next = ((head[t.col]->next)->down) = last;
                    c->headnode->triple.value++;
                    temp1 = temp1->right;
                }
                else {
                    int sum = temp2->triple.value + temp1->triple.value;
                    if (sum) {
                        Triple t;
                        t.row = temp1->triple.row; 
                        t.col = temp2->triple.col; 
                        t.value = sum;
                        last = last->right = new MatrixNode(false, &t);
                        head[t.col]->next = ((head[t.col]->next)->down) = last;
                        c->headnode->triple.value++;
                    }
                    temp1 = temp1->right;
                    temp2 = temp2->right;
                }
            }
        }
        while (temp1 != curr_row_head) {
            Triple t;
            t.row = temp1->triple.row; 
            t.col = temp1->triple.col; 
            t.value = temp1->triple.value;
            last = last->right = new MatrixNode(false, &t);
            head[t.col]->next = ((head[t.col]->next)->down) = last;
            c->headnode->triple.value++;
            temp1 = temp1->right;
        }
        while (temp2 != curr_row_head_b) {
            Triple t;
            t.row = temp1->triple.row; 
            t.col = temp2->triple.col; 
            t.value = temp2->triple.value;
            last = last->right = new MatrixNode(false, &t);
            head[t.col]->next = ((head[t.col]->next)->down) = last;
            c->headnode->triple.value++;
            temp2 = temp2->right;
        }
        last->right = head[currentRow];
        currentRow++;
        temp1 = temp1->next; 
        temp2 = temp2->next;
    }
    for (int i = 0; i < p - 1; i++) head[i]->next->down = head[i];
    for (int i = 0; i < p - 1; i++) head[i]->next = head[i + 1];
    head[p - 1]->next = c->headnode;
    c->headnode->right = head[0];
    return c;
}

Matrix * Matrix::operator*(const Matrix& b) const
{
    Matrix * c;
    MatrixNode * curr_row_head, * curr_col_head_b;
    if (headnode->triple.col != b.headnode->triple.row) throw "Error!";
    Triple s;
    s.col = b.headnode->triple.col; 
    s.row = headnode->triple.row; 
    s.value = 0;
    c = new Matrix;
    c->headnode = new MatrixNode(false, &s);
    int p = max(b.headnode->triple.col, headnode->triple.row);
    MatrixNode ** head = new MatrixNode* [p];
    for (int i = 0; i < p; i++) {
        head[i] = new MatrixNode(true, 0);
        head[i]->next = head[i]->right = head[i];
    }
    int currentRow = 0;
    MatrixNode * last = head[0];
    MatrixNode * temp1 = headnode->right, *temp2 = b.headnode->right;
    curr_row_head = headnode->right; 
    curr_col_head_b = b.headnode->right;
    while (curr_row_head != headnode) {
        last = head[currentRow];
        curr_col_head_b = b.headnode->right;
        int currentCol = 0;
        while (curr_col_head_b != b.headnode) {
            temp1 = curr_row_head->right;
            temp2 = curr_col_head_b->down;
            int sum = 0;
            while (temp1 != curr_row_head && temp2 != curr_col_head_b) {
                if (temp1->triple.col == temp2->triple.row) {
                    sum += temp1->triple.value * temp2->triple.value;
                    temp1 = temp1->right;
                    temp2 = temp2->down;
                }
                else if (temp1->triple.col < temp2->triple.row)
                    temp1 = temp1->right;
                else
                    temp2 = temp2->down;
            }
            if (sum) {
                Triple t;
                t.row = currentRow; 
                t.col = currentCol; 
                t.value = sum;
                last = last->right = new MatrixNode(false, &t);
                head[t.col]->next = ((head[t.col]->next)->down) = last;
                c->headnode->triple.value++;
            }
            curr_col_head_b = curr_col_head_b->next;
            currentCol++;
        }
        curr_row_head = curr_row_head->next;
        last->right = head[currentRow];
        currentRow++;
    }
    for (int i = 0; i < p - 1; i++) head[i]->next->down = head[i];
    for (int i = 0; i < p - 1; i++) head[i]->next = head[i + 1];
    head[p - 1]->next = c->headnode;
    c->headnode->right = head[0];
    return c;
    
}

Matrix* Matrix:: Transpose()
{
    Matrix * c;
    MatrixNode * temp;
    Triple s;
    s.col = headnode->triple.col; 
    s.row = headnode->triple.row; 
    s.value = headnode->triple.value;
    c = new Matrix;
    c->headnode = new MatrixNode(false, &s);
    int p = max(headnode->triple.col, headnode->triple.row);
    MatrixNode ** head = new MatrixNode * [p];
    for (int i = 0; i < p; i++) {
        head[i] = new MatrixNode(true, 0);
        head[i]->next = head[i]->right = head[i];
    }
    int currentRow = 0;
    MatrixNode * last = head[0];
    MatrixNode * curr_col = headnode->right;
    while (curr_col != headnode) {
        temp = curr_col->down;
        last = head[currentRow];
        while (temp != curr_col) {
            Triple t;
            t.row = temp->triple.col; 
            t.col = temp->triple.row; 
            t.value = temp->triple.value;
            last = last->right = new MatrixNode(false, &t);
            head[t.col]->next = ((head[t.col]->next)->down) = last;
            temp = temp->down;
        }
        last->right = head[currentRow];
        currentRow++;
        curr_col = curr_col->next;
    }
    for (int i = 0; i < p - 1; i++) head[i]->next->down = head[i];
    for (int i = 0; i < p - 1; i++) head[i]->next = head[i + 1];
    head[p - 1]->next = c->headnode;
    c->headnode->right = head[0];
    return c;
}

istream& operator>>(istream& is, Matrix& matrix)
{
    Triple s, t;
    is >> s.row >> s.col >> s.value;
    int p = max(s.row, s.col);
    matrix.headnode = new MatrixNode(false, &s);
    if (p == 0) {
        matrix.headnode->right = matrix.headnode;
        return is;
    }
    MatrixNode ** head = new MatrixNode* [p];
    for (int i = 0; i < p; i++) {
        head[i] = new MatrixNode(true, 0);
        head[i]->down = head[i]->next = head[i]->right = head[i];
    }
    int currentRow = 0;
    MatrixNode *last = head[0];
    for (int i = 0; i < s.value; i++) {
        is >> t.row >> t.col >> t.value;
        if (t.row > currentRow) {
            last->right = head[currentRow];
            currentRow = t.row;
            last = head[currentRow];
        }
        last = last->right = new MatrixNode(false, &t);
        head[t.col]->next = ((head[t.col]->next)->down) = last;
    }
    last->right = head[currentRow];
    for (int i = 0; i < p; i++)
    head[i]->next->down = head[i];
    for (int i = 0; i < p - 1; i++)
    head[i]->next = head[i + 1];
    head[p - 1]->next = matrix.headnode;
    matrix.headnode->right = head[0];
    return is;
}

ostream & operator<<(ostream &os, Matrix &matrix)
{
    MatrixNode * temp = matrix.headnode, * cur_row_head;
    
    temp = matrix.headnode->right;
    while (temp != matrix.headnode) {
        cur_row_head = temp;
        temp = temp->right;
        while (temp != cur_row_head) {
            if (temp->right == cur_row_head) {
                if (temp->right->next == matrix.headnode)
                    os << "(" << temp->triple.row << ", " << temp->triple.col << ", " << temp->triple.value << ")";
                else
                    os << "(" << temp->triple.row << ", " << temp->triple.col << ", " << temp->triple.value << "), ";
            }
            else
            os << "(" << temp->triple.row << ", " << temp->triple.col << ", " << temp->triple.value << "), ";
            temp = temp->right;
        }
        temp = temp->next;
    }
    os << endl;
    
    return os;
}

int main()
{
    Matrix a, b, * c;
    cout << "Enter the first matrix: " << endl;
    cin >> a;
    cout << "M1: " << a;
    cout << "Enter the second matrix: " << endl;
    cin >> b;
    cout << "M2: " << b;
    c = a + b;
    cout << "M1 + M2 = " << *c;
    c = a * b;
    cout << "M1 * M2 = " << *c;
    Matrix d(*c);
    cout << "Copy of M1 * M2 using copy constructor: " << d;

    return 0;
}