#include <iostream>
using namespace std;

class String
{
    friend ostream& operator<<(ostream& out, String& s);
    friend istream& operator>>(istream& in, String& s);
    public:
        String(char *init, int m);
        // constructor using input string init of length m
        bool operator == (String t);//equality test
        bool operator!( ); // empty test, true or false
        int Length( );//get the number of characters of *this
        String Concat(String t);  
        // concatenation with another string t
        String Substr(int i, int j);  // generate a substring i~i+j-1
        int Find(String pat);
        int FastFind(String pat);
        // Return an index i such that pat matches the substring
        // of the object begins at position i.  Return -1 if pat
        // is empty or not a substring of the object
        String Delete(int start, int length); //remove length characters beginning at start
        String CharDelete(char c); //returns the string with all occurrence of c removed.
        int Compare(String y); //compare two strings of letters of alphabet.
    private:
        char * str;
};

String::String(char * init, int m)
{
    str = new char[m+1];
    for (int i = 0; i < m; i++) str[i] = init[i];
    str[m] = '\0';
}

bool String::operator==(String s)
{
    if (Length() != s.Length()) return false;
    int i;
    for (i = 0; i < Length() && str[i] == s.str[i]; i++);
    if (i == Length()) return true;
    else return false;
}

bool String::operator!()
{
    if (Length()) return false;
    else return true;
}

int String::Length()
{
    int i;
    for (i = 0; str[i] != '\0'; i++);
    return i;
}

String String::Concat(String s)
{
    char * result = new char[Length()+s.Length()];
    int i, j;
    for (i = 0, j = 0; i < Length(); i++, j++) result[j] = str[i];
    for (i = 0; i < s.Length(); i++, j++) result[j] = s.str[i];
    String ans(result, Length()+s.Length());
    return ans;
}

String String::Substr(int l, int length)
{
    if (Length() < l+length) throw "Error";
    char * result = new char[length];
    for (int i = 0; i < length; i++) result[i] = str[l+i];
    String ans(result, length);
    return ans;
}

int String::Find(String pat)
{
    for (int start = 0; start <= Length()-pat.Length(); start++) {
        int j;
        for (j = 0; j < pat.Length() && str[start+j] == pat.str[j]; j++);
        if (j == pat.Length()) return start;
    }
    return -1;
}

int String::FastFind(String pat)
{
    int lengthP = pat.Length(), lengthS = Length();
    int * f = new int[lengthP];
    f[0] = -1;
    for (int j = 1; j < lengthP; j++) 
    {
        int i = f[j-1];
        while ((str[j] != str[i+1]) && (i >= 0)) 
            i = f[i];
        if (str[j] == str[i+1]) 
            f[j] = i+1;
        else 
            f[j] = -1;
    }

    int posP = 0, posS = 0;
    while((posP < lengthP) && (posS < lengthS)){
        if (pat.str[posP] == str[posS]) {  
        posP++; 
        posS++;    
        }else{
        if (posP == 0) 
            posS++;
        else 
            posP = f[posP -1] + 1;
        }
    }
    if (posP < lengthP) 
        return -1;
    else // Textbook stops pattern matching once a match is found
        return posS-lengthP;
}

String String::Delete(int start, int length)
{
    char * result = new char[Length()-length];
    int i, j;
    for (i = 0, j = 0; j < start; i++, j++) result[i] = str[j];
    for (j = start+length; j < Length(); i++, j++) result[i] = str[j];
    String ans(result, Length()-length);
    return ans;
}

String String::CharDelete(char c)
{
    int i, j, num = 0;
    for (i = 0; i < Length(); i++)
        if (str[i] == c) num++;
    char * result = new char[Length()-num];
    i = 0; j = 0;
    while (i < Length()) {
        if (str[i] != c) {
            result[j] = str[i];
            i++; j++;
        } else i++;
    }
    String ans(result, Length()-num);
    return ans;
}

int String::Compare(String y)
{
    int i = 0, j = 0;
    while (i < Length() && j < y.Length()) {
        if (str[i] == y.str[j]) {
            i++; j++;
        } else if (str[i] > y.str[j]) return 1;
        else return -1;
    }
    if (i == Length() && j == y.Length()) return 0;
    else if (Length()-i) return 1;
    else return 0;
}

ostream& operator<<(ostream& out, String& s)
{
    out << s.str << endl;
    return out;
}

istream& operator>>(istream& in, String& s)
{
    int length;
    cout << "Enter the length of the string: ";
    in >> length;
    cout << "Enter the input string: ";
    char * string = new char[length+1];
    in >> string;
    String result(string, length);
    s = result;
    delete [] string;
    return in;
}

int main()
{
    int i, j;
    char ch;
    String s1(NULL, 0);
    String s2(NULL, 0);
    String s3(NULL, 0);

    cin >> s1;
    cin >> s2;
    cout << "String 1 is : ";
    cout << s1;
    cout << "String 2 is : ";
    cout << s2;
    cout << "=========================================" << endl;
    if (!s1) cout << "String 1 is null";
    else cout << "String 1 is not null";
    cout << endl;
    // cout << "Is String 2 a NULL String? ";
    if (!s2) cout << "String 2 is null";
    else cout << "String 2 is not null";
    cout << endl;
    // cout << "Is String 1 equal to String 2? ";
    cout << "=========================================" << endl;
    if (s1 == s2) cout << "String 1 equals to string 2";
    else {
        if (s1.Compare(s2) == 1) cout << "String 1 is bigger than String 2";
        else cout << "String 1 is smaller than String 2";
    }
    cout << endl;
    cout << "=========================================" << endl;
    s3 = s1.Concat(s2);
    cout << "String 1 concate with String 2 :" << s3;
    cout << "=========================================" << endl;
    cout << "Enter the value i, j to get i~i+j-1 of String 1: ";
    cin >> i >> j;
    cout << "Result: ";
    s3 = s1.Substr(i,j);
    cout << s3;
    cout << "=========================================" << endl;
    cout << "Suppose the pattern is the above result." << endl;
    cout << "Then the position where the pattern starts in String 2: " << endl;
    cout << "By the naive method, the result is: ";
    cout << s2.Find(s3) << endl;
    cout << "By the KMP method, the result is: ";
    cout << s2.FastFind(s3) << endl;
    cout << "=========================================" << endl;
    cout << "Enter the value i, j to delete i~i+j-1 of String 1: ";
    cin >> i >> j;
    s3 = s1.Delete(i, j);
    cout << "Result: ";
    cout << s3;
    cout << "=========================================" << endl;
    cout << "Choose a letter to delete from String 2: ";
    cin >> ch;
    s3 = s2.CharDelete(ch);
    cout << "Result: ";
    cout << s3;
    return 0;
}