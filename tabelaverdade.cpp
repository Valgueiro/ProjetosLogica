#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cmath>

using namespace std;

typedef vector <int> vi;
typedef pair <int, int> ii;
typedef vector<ii> vii;

int found;
vi mkd;
char str[201];//Guarda a completa
char substr[50][201];//Guarda todas as subexpressões

int isAtomic(char a){
   if(a == 'x')
      return 1;
   if(a == 'y')
      return 2;
   if(a == 'z')
      return 3;
   if(a == 't')
      return 4;
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
   int i, j, k, aux, cont, atomics, icog[4]={};
   scanf(" %s", str);
   int value[16][100];

   for(i = 0; i<strlen(str); i++){
      aux = isAtomic(str[i]);
      if(aux){
         substr[aux-1][0] = str[i];
         substr[aux-1][1] = '\0';
         if(!icog[aux-1]){
            icog[aux-1] = 1;
            cont++;
         }
      }
      mkd.push_back(0);
   }
   atomics = cont;
   found = 4;

   findSub(mkd, 0);
   
   for(i=0; i<4; i++){
      if(icog[i]){
         for(j=0; j<pow(2, atomics);){
            for(; j<pow(2, cont-1); j++){
               value[j][i] = 0;
            }
            for(; j<pow(2, cont-1); k++){
               value[j][i] = 1;
            }
            j+=pow(2, cont);
         }
         cont--;
      }
   }
   // for(i=0; i<found; i++){
   //    if(i<=3){
   //       if(icog[i]){
   //          printf("%s ", substr[i]);
   //       }
   //    }else{
   //       printf("%s ",substr[i]);   
   //    }
      
   // }

   for(i=0; i<4; i++){
      for(j=0; j<pow(2, atomics); j++){
         printf("%d\n", value[j][i]);
      }
      printf("outro:\n");
   }
   

   return 0;
}
