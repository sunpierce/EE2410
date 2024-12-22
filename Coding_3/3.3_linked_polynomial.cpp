#include <iostream>
using namespace std;

template <class T> 
class List;

class Polynomial;

struct Term
{
    int coef, exp;
    Term Set(int c, int e) { coef = c; exp = e; return *this; }
};

template <class T> 
class Node 
{
    friend istream& operator>>(istream& is, Polynomial& p);
    friend ostream& operator<<(ostream& os, Polynomial& p);
    friend class List<T>;
    friend class Polynomial;
    template <class U>
    friend ostream& operator<<(ostream& os, List<U>& L);
    private:
        T data;
        Node<T> * link;
};

template <class T> 
class List
{
    friend istream& operator>>(istream& is, Polynomial& p);
    friend ostream& operator<<(ostream& os, Polynomial& p);
    friend class Polynomial;
    template <class U>
    friend ostream& operator<<(ostream& os, List<U>& L);
    public: 
        List();
        ~List();
        int count();
        void InsertFront(T e);
        void InsertBack(T e);
        void DeleteFirst();
        void DeleteBack();
        Node<T> * GetNode();
        void RetNode(Node<T> *& x);
    private:
        Node<T> * header, * av;
};

template <class T> 
List<T>::List()
{
    header = new Node<T>;
    header->link = header;
    av = NULL;
}

template <class T> 
List<T>::~List()
{
    if (header->link != header) {
        Node<Term> * first = header->link;
        header->link = av;
        av = first;
        header = NULL;
    }
}

template <class T> 
int List<T>::count()
{
    Node<T> * temp = header->link;
    int ct = 0;
    while (temp != header) {
        ct++;
        temp = temp->link;
    }
    return ct;
}

template <class T> 
void List<T>::InsertFront(T e)
{
    Node<T> * node = GetNode();
    node->data = e;
    node->link = header->link;
    header->link = node;
}

template <class T> 
void List<T>::InsertBack(T e)
{
    Node<T> * temp = header->link;
    while (temp->link != header) temp = temp->link;
    Node<T> * node = GetNode();
    node->data = e;
    node->link = header;
    temp->link = node;
}

template <class T> 
void List<T>::DeleteFirst()
{
    if (header->link == header) return;
    Node<T> * temp = header->link;
    header->link = temp->link;
    RetNode(temp);
}

template <class T> 
void List<T>::DeleteBack()
{
    if (header->link == header) return;
    Node<T> * temp = header->link, * prev = temp;
    if (temp->link == header) {
        DeleteFirst();
        return;
    }
    while (temp->link != header) {
        prev = temp;
        temp = temp->link;
    }
    RetNode(temp);
    prev->link = header;
}

template <class T> 
Node<T> * List<T>::GetNode()
{
    Node<T> * node;
    if (av) {
        node = av;
        av = av->link;
    } else node = new Node<T>;
    return node;
}

template <class T>
void List<T>::RetNode(Node<T> *& node)
{
    node->link = av;
    av = node;
    node = 0;
}

template <class U> 
ostream& operator<<(ostream& os, List<U>& L)
{
    Node<U> * temp;
    int i = 0;
    os << "{";
    for (temp = L.header->link; temp->link != L.header; temp = temp->link, i++) {
        os << "(" << i << "," << temp->data << "),";
    }
    os << "(" << i << "," << temp->data << ")}" << endl;
    return os; 
}

class Polynomial
{
    public:
        friend istream& operator>>(istream& is, Polynomial& x);
        friend ostream& operator<<(ostream& os, Polynomial& a);
        Polynomial(){};
        Polynomial(const Polynomial &a);
        ~Polynomial();
        const Polynomial& operator=(const Polynomial& a);
        Polynomial operator+(const Polynomial& b) const;
        Polynomial operator-(const Polynomial& b) const;
        Polynomial operator*(const Polynomial& b) const;
        void Set(Term);
        double Evaluate(float x) const;
    private:
        List<Term> poly;
};

Polynomial::Polynomial(const Polynomial& a)
{
    Term data;
    Polynomial * p = new Polynomial;
    Node<Term> *temp = a.poly.header->link;
    
    while (temp != a.poly.header) {
        p->poly.InsertBack(data.Set(temp->data.coef, temp->data.exp));
        temp = temp->link;
    }
    this->poly = p->poly;
}

Polynomial:: ~Polynomial()
{
    while (poly.header->link != poly.header) 
        poly.DeleteFirst();
}

const Polynomial& Polynomial:: operator=(const Polynomial &a)
{
    Term data;
    Polynomial * p = new Polynomial;
    Node<Term> * temp = a.poly.header->link;
    while (temp != a.poly.header) {
        p->poly.InsertBack(data.Set(temp->data.coef, temp->data.exp));
        temp = temp->link;
    }
    this->poly = p->poly;
    return *this;
}

Polynomial Polynomial:: operator+(const Polynomial &b) const
{
    Term data;
    Node<Term> *temp1 = poly.header->link;
    Node<Term> *temp2 = b.poly.header->link;
    Polynomial c;
    while (temp1 != poly.header && temp2 != b.poly.header) {
        if (temp1->data.exp == temp2->data.exp) {
            int sum = temp1->data.coef + temp2->data.coef;
            if (sum) c.poly.InsertBack(data.Set(sum, temp1->data.exp));
            temp1 = temp1->link; temp2 = temp2->link;
        }
        else if (temp1->data.exp < temp2->data.exp) {
            c.poly.InsertBack(data.Set(temp2->data.coef, temp2->data.exp));
            temp2 = temp2->link;
        }
        else {
            c.poly.InsertBack(data.Set(temp1->data.coef, temp1->data.exp));
            temp1 = temp1->link;
        }
    }
    while (temp1 != poly.header) {
        c.poly.InsertBack(data.Set(temp1->data.coef, temp1->data.exp));
        temp1 = temp1->link;
    }
    while (temp2 != b.poly.header) {
        c.poly.InsertBack(data.Set(temp2->data.coef, temp2->data.exp));
        temp2 = temp2->link;
    }
    return c;
}

Polynomial Polynomial:: operator-(const Polynomial &b) const
{
    Term data;
    Node<Term> *temp1 = poly.header->link;
    Node<Term> *temp2 = b.poly.header->link;
    Polynomial c;
    while (temp1 != poly.header && temp2 != b.poly.header) {
        if (temp1->data.exp == temp2->data.exp) {
            int sum = temp1->data.coef - temp2->data.coef;
            if (sum) c.poly.InsertBack(data.Set(sum, temp1->data.exp));
            temp1 = temp1->link; temp2 = temp2->link;
        }
        else if (temp1->data.exp < temp2->data.exp) {
            c.poly.InsertBack(data.Set(-1 * temp2->data.coef, temp2->data.exp));
            temp2 = temp2->link;
        }
        else {
            c.poly.InsertBack(data.Set(temp1->data.coef, temp1->data.exp));
            temp1 = temp1->link;
        }
    }
    while (temp1 != poly.header) {
        c.poly.InsertBack(data.Set(-1 * temp1->data.coef, temp1->data.exp));
        temp1 = temp1->link;
    }
    while (temp2 != b.poly.header) {
        c.poly.InsertBack(data.Set(temp2->data.coef, temp2->data.exp));
        temp2 = temp2->link;
    }
    return c;
}

Polynomial Polynomial:: operator*(const Polynomial &b) const
{
    Term data;
    Node<Term> *temp1, *temp2;
    Polynomial c;
    bool First = 1;
    for (temp1 = poly.header->link; temp1 != poly.header; temp1 = temp1->link) {
        for (temp2 = b.poly.header->link; temp2 != b.poly.header; temp2 = temp2->link) {
            Polynomial *newPoly = new Polynomial;
            newPoly->poly.InsertBack(data.Set(temp1->data.coef * temp2->data.coef, temp1->data.exp + temp2->data.exp));
            if (First) {
                c = *newPoly;
                First = 0;
            }
            else
                c = *newPoly + c;
            delete newPoly;
        }
    }
    return c;
}

double Polynomial:: Evaluate(float x) const
{
    Node<Term> * temp = poly.header->link;
    double result = 0;
    while (temp != poly.header) {
        result += temp->data.coef * pow(x, temp->data.exp);
        temp = temp->link;
    }
    return result;
}

void Polynomial:: Set(Term term)
{
    poly.InsertBack(term);
}

ostream &operator<<(ostream &os, Polynomial &a)
{
    Node<Term> * temp;
    int num;
    num = a.poly.count();
    temp = a.poly.header->link;
    if (num == 0) {
        os << "0" << endl;
        return os;
    }
    if (num == 1) {
        if (temp->data.coef != 1) {
            if (temp->data.coef > 0) os << temp->data.coef << " ";
            else os << temp->data.coef;
        }
        else if (temp->data.exp == 0) os << temp->data.coef;
        if (temp->data.exp > 1) os << "x^" << temp->data.exp ;
        else if (temp->data.exp == 1) os << "x";
    } else {
        if (temp->data.coef != 1) os << temp->data.coef << " ";
        if (temp->data.exp > 1) os << "x^" << temp->data.exp << " ";
        else if (temp->data.exp == 1) os << "x ";
        num--;
        for (temp = temp->link; num != 1; num--, temp = temp->link) {
            if (temp->data.coef != 1) {
                if (temp->data.coef > 0) os << "+" << temp->data.coef << " ";
                else os << temp->data.coef << " ";
            }
            else os << "+ ";
            if (temp->data.exp != 1) os << "x^" << temp->data.exp << " ";
            else if (temp->data.exp == 1) os << "x ";
        }
        if (temp->data.coef != 1) {
            if (temp->data.coef > 0 && temp->data.exp != 0) os << "+" << temp->data.coef << " ";
            else if (temp->data.coef < 0 && temp->data.exp != 0) os << temp->data.coef << " ";
            else if (temp->data.coef > 0) os << "+" << temp->data.coef;
            else os << temp->data.coef;
        }
        else if (temp->data.exp != 0) os << "+ ";
        else os << "+ 1";
        if (temp->data.exp != 0 && temp->data.exp != 1) os << "x^" << temp->data.exp;
        else if (temp->data.exp == 1) os << "x";
    }
    os << endl;    
    return os;
}

istream &operator>>(istream &is, Polynomial &x)
{
    Term data;
    char temp;
    int exp, coef;
    Polynomial c;
    while ((temp = getchar()) != '}') {
        getchar();
        is >> coef >> temp >> exp;
        getchar();
        c.poly.InsertBack(data.Set(coef, exp));
    }
    getchar();
    x = c;
    return is;
}

int main()
{
    Polynomial a, b, c, d;
    double num;

    cout << "Enter the first polynomial in the form of {(coef,exp),(coef,exp),...)}: ";
    cin >> a;
    cout << "P1(x) = " << a;
    cout << "Enter the second polynomial in the form of {(coef,exp),(coef,exp),...)}: ";
    cin >> b; 
    cout << "P2(x) = " << b;
    c = a + b;
    cout << "P1(x) + P2(x) = " << c;
    c = a - b;
    cout << "P1(x) - P2(x) = " << c;
    c = a * b;
    cout << "P1(x) * P2(x) = " << c;
    cout << "Enter a value v: ";
    cin >> num;
    cout << "P1(v) * P2(v) = " << c.Evaluate(num) << endl;

    return 0;
}