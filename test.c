#include<iostream>
using namespace std;

int main(){
int ar[5]={1,2,3,100,200};
int* a = ar;
int* b = ar+3;
int c = *a+++*b++;
cout << *a << endl;
int d = *++a+*++b;
cout << c << endl << d << endl;
return 0;
}