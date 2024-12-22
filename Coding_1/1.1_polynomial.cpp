#include <iostream>
using namespace std;

class Polynomial;

class Term {
    // permit Polynomial, operator<<, operator>> to access the private data members of Term
    friend Polynomial;
    friend ostream& operator<<(ostream& os, Polynomial& p);
    friend istream& operator>>(istream& is, Polynomial& p);
    private:
        float coef;
        int exp;
};

class Polynomial {
    public:
        Polynomial();
        //construct the polynomial p(x) = 0
        //~Polynomial();
        //destructor
        Polynomial Add(Polynomial poly);
        //return the sum of *this and poly
        Polynomial Subt(Polynomial poly);
        //return the difference of *this and poly
        Polynomial Mult(Polynomial poly);
        //return the product of *this and poly
        void NewTerm(const float theCoeff, const int theExp);
        // construct a new term
        float Eval(float f );
        //Evaluate the polynomial *this at f and return the results
        int operator!();
        // if *this is the zero polynomial, return 1; else return 0;
        float Coef(int e);
        // return the coefficient of e in *this
        int LeadExp();
        // return the largest exponent in *this
	    friend ostream& operator<<(ostream& os, Polynomial& p);
        friend istream& operator>>(istream& is, Polynomial& p);
    private:
	    Term * termArray;
	    int	capacity; // size of termArray
	    int	terms;    // number of nonzero terms
};

Polynomial:: Polynomial() 
{
    capacity = 1;
    terms = 0;
    termArray = new Term[capacity];
    termArray[0].coef = 0;    
    termArray[0].exp = 0;
};

/*Polynomial::~Polynomial() {
    delete[] termArray;
}*/

Polynomial Polynomial::Add(Polynomial b)
{
    // this + b = c
    Polynomial c;
    int aPos = 0, bPos = 0;
    float coef;

    while ((aPos < terms) && (bPos < b.terms)) {
        if (termArray[aPos].exp == b.termArray[bPos].exp) {
            coef = termArray[aPos].coef + b.termArray[bPos].coef;
            if (coef) c.NewTerm(coef, termArray[aPos].exp);
            aPos++;
            bPos++;
        } else if (termArray[aPos].exp < b.termArray[bPos].exp) {
            c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        } else {
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }
    for (; aPos < terms; aPos++)
        c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
    for (; bPos < b.terms; bPos++)
        c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
    return c;
}

Polynomial Polynomial::Subt(Polynomial b)
{
    // this - b = c
    Polynomial negB; // negate B
    for (int i = 0; i < b.terms; i++)
        negB.NewTerm(-b.termArray[i].coef, b.termArray[i].exp);
    return this->Add(negB);
}

Polynomial Polynomial::Mult(Polynomial b)
{
    // product = this * b
    Polynomial product, tmp;
    float coef = 0;
    int exp = 0;

    for (int i = 0; i < terms; i++)
        for (int j = 0; j < b.terms; j++)
        {
            exp = termArray[i].exp + b.termArray[j].exp;
            coef = termArray[i].coef * b.termArray[j].coef;
            tmp.NewTerm(coef, exp);
            product = product.Add(tmp);
            // reset tmp
            tmp.terms = 0;
            tmp.termArray[0].coef = 0;
            tmp.termArray[0].exp = 0;
        }

    return product;
}

void Polynomial::NewTerm(const float theCoeff, const int theExp)
{
    if (terms == capacity) {
        capacity *= 2;
        Term * tmp = new Term[capacity];
        copy(termArray, termArray + terms, tmp);
        delete [] termArray;
        termArray = tmp;
    }
    termArray[terms].coef = theCoeff;
    termArray[terms++].exp = theExp;
}

float Polynomial::Eval(float f)
{
    double sum = 0;
    for (int i = 0; i < terms; i++) {
        sum += pow(f,termArray[i].exp) * termArray[i].coef;
    }
    return sum;
}

int Polynomial::operator!()
{
    if (terms) return 0;
    else return 1;
}

float Polynomial::Coef(int exp)
{
    for (int i = 0; i < terms; i++) {
        if (termArray[i].exp == exp) return termArray[i].coef;
    }
    return 0; // if exp not found then the coef must be zero
}

int Polynomial::LeadExp()
{
    int max = termArray[0].exp;
    for (int i = 1; i < terms; i++)
        max = (termArray[i].exp > max)? termArray[i].exp: max;
    return max;
}

istream& operator>>(istream& in, Polynomial& p)
{
    float coef; // input coef
    int exp; // input exp
    while (in >> coef) {
        in >> exp;
        if (coef == 0 && exp == 0) break;
        p.NewTerm(coef, exp);
        if (in.peek() == '\n') { // peek at the next character without extracting it
            in.ignore(); // if it is newline character then ignore it
            break; // break the loop
        }
    }
    return in;
}

ostream& operator<<(ostream& os, Polynomial& p)
{
    if (!p.terms) {
        os << "0" << endl;
        return os;
    }
    int pos = 0;
    while (pos < p.terms) {
        os << "(" << p.termArray[pos].coef << ")";
        if (p.termArray[pos].exp > 0) os << "*x^(" << p.termArray[pos].exp << ")"; // not constant term
        if (pos != p.terms-1) os << " + "; // not the last term
        pos++;
    }
    os << endl;
    return os;
}

int main()
{
    Polynomial a, b, add, sub, mul;
    float f;
    int exp;

    cout << "Enter the first polynomial f1(x) in the form of coef exp coef exp ..." << endl;
    cin >> a;
    cout << "Enter the second polynomial f2(x) in the form of coef exp coef exp ..." << endl;
    cin >> b;

    add = a.Add(b);
    mul = a.Mult(b);
    sub = a.Subt(b);

    cout << "====================================" << endl;
    cout << "f1(x) = " << a;
    cout << "exp of the leading term: " << a.LeadExp() << endl;
    cout << "coef of the leading term: " << a.Coef(a.LeadExp()) << endl;
    if (!a) cout << "f1(x) is zero" << endl;
    else cout << "f1(x) is not zero" << endl;
    cout << "====================================" << endl;
    cout << "f2(x) = " << b;
    cout << "exp of the leading term: " << b.LeadExp() << endl;
    cout << "coef of the leading term: " << b.Coef(b.LeadExp()) << endl;
    if (!b) cout << "f2(x) is zero" << endl;
    else cout << "f2(x) is not zero" << endl;
    cout << "====================================" << endl;
    cout << "f1(x) + f2(x) = " << add;
    cout << "f1(x) * f2(x) = " << mul;
    cout << "f1(x) - f2(x) = " << sub;
    cout << "====================================" << endl;
    cout << "Enter a value v: ";
    cin >> f;
    cout << "f1(v) + f2(v) = " << add.Eval(f) << endl;
    cout << "f1(v) * f2(v) = " << mul.Eval(f) << endl;
    cout << "f1(v) - f2(v) = " << sub.Eval(f) << endl;
    cout << "====================================" << endl;
    cout << "Enter a exp to see its coef of f1(x) * f2(x): ";
    cin >> exp;
    cout << "The corresponding coef is: " << mul.Coef(exp) << endl;

    return 0;
}