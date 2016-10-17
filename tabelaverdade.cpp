#include <bits/stdc++.h>

using namespace std;

typedef vector <int> vi;
typedef pair <int, int> ii;
typedef vector<ii> vii;

int atomic(char a){
   if(a == 'x' || a == 'y' || a == "z" || a == "t")return 1;
   return 0;
}
//==================================Fuções das operações======================================
int and(int a, int b){
   if(a==0 || b==0) return 0;
   return 1;
}

int or(int a, int b){
   if(a==1 || b==1) return 1;
   return 0;
}

int imp(int a, int b){
   if(a==1 && b==0) return 0;
   return 1;
}

int not(int a){
   if(a==0) return 1;
   return 0;
}
//===========================================================================================

int main(){


   return 0;
}
