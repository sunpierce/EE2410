#include <iostream>
using namespace std;

class SparseMatrix;

class MatrixTerm {
    friend SparseMatrix;
    friend ostream& operator<<(ostream& out, SparseMatrix &m);
    friend istream& operator>>(istream& in, SparseMatrix &m);
    private:
        int row, col, val; // a triple representing a term
};

class SparseMatrix {
    public:
        // constructor
        SparseMatrix(int r, int c, int t);
        SparseMatrix Transpose();
        SparseMatrix FastTranspose();
        SparseMatrix Add(SparseMatrix b);
        SparseMatrix Multiply(SparseMatrix b);
        void StoreSum(const int sum, const int r, const int c);
        void ChangeSize1D(const int newSize);
        friend ostream& operator<<(ostream& out, SparseMatrix& m);
        friend istream& operator>>(istream& in, SparseMatrix& m);
    private:
        int rows, cols, terms, capacity;
        MatrixTerm * termMatrix;
};

SparseMatrix::SparseMatrix(int r, int c, int t)
{
    rows = r;
    cols = c;
    terms = t;
    capacity = 10;
    termMatrix = new MatrixTerm[capacity];
}

SparseMatrix SparseMatrix::Transpose() // O(cols*terms)
{
    SparseMatrix b(cols, rows, terms); // matrix after transpose
    if (terms > 0) {
        int idxB = 0; // index for sparse matrix b
        for (int c = 0; c < cols; c++) // column-by-column transpose
            for (int i = 0; i < terms; i++) 
                if (termMatrix[i].col == c) {
                    b.termMatrix[idxB].row = c;
                    b.termMatrix[idxB].col = termMatrix[i].row;
                    b.termMatrix[idxB].val = termMatrix[i].val;
                    idxB++;
                }
    }
    return b;
}

SparseMatrix SparseMatrix::FastTranspose()
{
    SparseMatrix b(cols, rows, terms);
    if (terms > 0) {
        int * rowSize = new int[cols];
        int * rowStart = new int[cols];
        // calculate the row size of the new matrix
        fill(rowSize, rowSize+cols, 0);
        for (int i = 0; i < terms; i++) rowSize[termMatrix[i].col]++;
        // calculate the starting array index of each row of the new matrix
        rowStart[0] = 0;
        for (int i = 1; i < cols; i++) rowStart[i] = rowStart[i-1] + rowSize[i-1];
        for (int i = 0; i < terms; i++) {
            int j = rowStart[termMatrix[i].col];
            b.termMatrix[j].row = termMatrix[i].col;
            b.termMatrix[j].col = termMatrix[i].row;
            b.termMatrix[j].val = termMatrix[i].val;
            rowStart[termMatrix[i].col]++;
        }
    }
    return b;
}

void SparseMatrix::ChangeSize1D(const int newSize)
{
    // change the array size to newSize
    if (newSize < terms) throw "New size must be >= number of terms";
    MatrixTerm * temp = new MatrixTerm[newSize]; // new array
    copy(termMatrix, termMatrix+terms, temp);
    delete [] termMatrix;
    termMatrix = temp; // make termMatrix point to newly created array
    capacity = newSize;
}

void SparseMatrix::StoreSum(const int sum, const int r, const int c)
{
    if (sum != 0) {
        if (terms == capacity) ChangeSize1D(2*capacity);
        termMatrix[terms].row = r;
        termMatrix[terms].col = c;
        termMatrix[terms++].val = sum;
    }
}

SparseMatrix SparseMatrix::Multiply(SparseMatrix b) 
{
    if (cols != b.rows) throw "Incompatible matrices";
    SparseMatrix bXpose = b.FastTranspose();
    SparseMatrix d(rows, b.cols, 0);
    int currRowIndex = 0, currRowBegin = 0, currRowA = termMatrix[0].row;
    
    if (terms == capacity) ChangeSize1D(terms + 1);
    bXpose.ChangeSize1D(bXpose.terms + 1);
    termMatrix[terms].row = rows;
    bXpose.termMatrix[b.terms].row = b.cols;
    bXpose.termMatrix[b.terms].col = -1;

    int sum = 0;

    while (currRowIndex < terms) {
        int currColB = bXpose.termMatrix[0].row;
        int currColIndex = 0;
        while (currColIndex <= b.terms) {
            if (termMatrix[currRowIndex].row != currRowA) {
                d.StoreSum(sum, currRowA, currColB);
                sum = 0;
                currRowIndex = currRowBegin;
                while (bXpose.termMatrix[currColIndex].row == currColB)
                    currColIndex++;
                currColB = bXpose.termMatrix[currColIndex].row;
            }
            else if (bXpose.termMatrix[currColIndex].row != currColB) {
                d.StoreSum(sum, currRowA, currColB);
                sum = 0;
                currRowIndex = currRowBegin;
                currColB = bXpose.termMatrix[currColIndex].row;
            }
            else
                if (termMatrix[currRowIndex].col < bXpose.termMatrix[currColIndex].col) currRowIndex++;
                else if (termMatrix[currRowIndex].col == bXpose.termMatrix[currColIndex].col) {
                    sum += termMatrix[currRowIndex].val * bXpose.termMatrix[currColIndex].val;
                    currRowIndex++;
                    currColIndex++;
                }
                else currColIndex++;
        }
        while (termMatrix[currRowIndex].row == currRowA) currRowIndex++;
        currRowBegin = currRowIndex;
        currRowA = termMatrix[currRowIndex].row;
    }
    
    return d;
}

SparseMatrix SparseMatrix::Add(SparseMatrix b)
{
    if ((cols != b.cols) || (rows != b.rows)) throw "Incompatible matrices";
    SparseMatrix d(rows, cols, 0);
    int aPos = 0, bPos = 0;
    while ((aPos < terms) && (bPos < b.terms)) {
        if (termMatrix[aPos].row == b.termMatrix[bPos].row) {
            if (termMatrix[aPos].col > b.termMatrix[bPos].col) {
                d.StoreSum(b.termMatrix[bPos].val, b.termMatrix[bPos].row, b.termMatrix[bPos].col);
                bPos++;
            } else if (termMatrix[aPos].col < b.termMatrix[bPos].col) {
                d.StoreSum(termMatrix[aPos].val, termMatrix[aPos].row, termMatrix[aPos].col);
                aPos++;
            } else {
                d.StoreSum(termMatrix[aPos].val+b.termMatrix[bPos].val, termMatrix[aPos].row, termMatrix[aPos].col);
                aPos++; bPos++;
            }
        } else { // different row
            if (termMatrix[aPos].row < b.termMatrix[bPos].row) {
                d.StoreSum(termMatrix[aPos].val, termMatrix[aPos].row, termMatrix[aPos].col);
                aPos++;
            } else {
                d.StoreSum(b.termMatrix[bPos].val, b.termMatrix[bPos].row, b.termMatrix[bPos].col);
                bPos++;
            }
        }
    }
    // output the remaining term of this or b
    for (; aPos < terms; aPos++) d.StoreSum(termMatrix[aPos].val, termMatrix[aPos].row, termMatrix[aPos].col);
    for (; bPos < b.terms; bPos++) d.StoreSum(b.termMatrix[bPos].val, b.termMatrix[bPos].row, b.termMatrix[bPos].col);
    return d;
}

istream& operator>>(istream& in, SparseMatrix& m) 
{
    cout << "Enter the rows in total: ";
    in >> m.rows;
    cout << "Enter the cols in total: ";
    in >> m.cols;

    cout << "Enter the matrix's row col val row col val ..." << endl;
    int r, c, v;
    while (in >> r >> c >> v) {
        m.StoreSum(v, r, c);
        if (in.peek() == '\n') {
            in.ignore();
            break;
        }
    }
    return in;
}

ostream& operator<<(ostream& out, SparseMatrix& m)
{
    out << "{ ";
    int pos;
    for (pos = 0; pos < m.terms; pos++) {
        out << "(";
        out << m.termMatrix[pos].row << ",";
        out << m.termMatrix[pos].col << ",";
        out << m.termMatrix[pos].val;
        out <<  ") ";
    }
    out << "}" << endl;
    return out;
}

int main() {
    SparseMatrix a(0,0,0);
    SparseMatrix b(0,0,0);
    SparseMatrix c(0,0,0);
    
    cout << "For matrix M1: " << endl;
    cin >> a;
    cout << "M1 = " << a;
    cout << "For matrix M2: " << endl;
    cin >> b;
    cout << "M2 = " << b;
    cout << "====================================" << endl;
    c = a.FastTranspose();
    cout << "Fast Transpose of M1 = " << c;
    c = a.Transpose();
    cout << "Transpose of M1 = " << c;
    c = b.FastTranspose();
    cout << "Fast Transpose of M2 = " << c;
    c = b.Transpose();
    cout << "Transpose of M2 = " << c;
    cout << "====================================" << endl;
    c = a.Multiply(b);
    cout << "M1 * M2 = " << c;
    c = a.Add(b);
    cout << "M1 + M2 = " << c;

    return 0;
}