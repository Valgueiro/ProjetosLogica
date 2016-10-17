#include <bits/stdc++.h>

using namespace std;

typedef vector <int> vi;
typedef pair <int, int> ii;
typedef vector<ii> vii;

int found;
vi mkd;
char str[201];//Guarda a completa
char substr[50][201];//Guarda todas as subexpressões

int atomic(char a){
   if(a == 'x' || a == 'y' || a == 'z' || a == 't') return 1;
   return 0;
}

void pass(int begin, int end){
   int i, j;
   for(i = begin, j=0; i<=end; i++, j++) substr[found][j] = str[i];
   substr[found][j] = '\0';
   found++;
}
//==================================Fuções das operações======================================
int Fand(int a, int b){
   if(a==0 || b==0) return 0;
   return 1;
}

int For(int a, int b){
   if(a==1 || b==1) return 1;
   return 0;
}

int Fimp(int a, int b){
   if(a==1 && b==0) return 0;
   return 1;
}

int Fnot(int a){
   if(a==0) return 1;
   return 0;
}
//===========================================================================================

int findSub(vi mkd, int where){
   int i;
   for(;str[where]!=')' && mkd[where]!=1 && where<strlen(str); where++);
   for(i=where; i>=0; i--)  if(str[i]=='(' && mkd[i]==0) break;
   if(i==-1) return 0;

   pass(i, where);
   mkd[where] = 1;
   mkd[i] = 1;

   if(where<strlen(str)) findSub(mkd, where+1);

}

int main(){
   int i;
   scanf(" %s", str);

   for(i = 0; i<strlen(str); i++){
      mkd.push_back(0);
   }
   found = 0;
   findSub(mkd, 0);
   for(i=0; i<found; i++){
      printf("%s\n",substr[i]);
   }

   return 0;
}
