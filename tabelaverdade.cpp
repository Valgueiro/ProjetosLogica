#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <math.h>
#define dbg printf("aqui\n")
#define dbg1 printf("aqui1\n")

using namespace std;

typedef vector <int> vi;
typedef pair <int, int> ii;
typedef vector<ii> vii;

int found, atomics;
int variables[5], value[16][100];
vi mkd;
char str[201];//Guarda a completa
char substr[50][201];//Guarda todas as subexpressões

//=====================================Funções úteis=========================================
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

int valor(char a, int j){
    int aux = isAtomic(a);
    if(aux){
        return value[aux][j];
    }
    return aux;
}

void print(){
    int i, j;
    //Primeira linha com as subexpressões:
    for(i=1; i<found; i++){
        if(i<=4){
            if(variables[i]){
             printf("%s ", substr[i]);
            }
        }else{
            printf("%s ",substr[i]);
        }

    }
    printf("\n");

    //Valores da tabela;
    for(j=0; j<pow(2, atomics); j++){
        for(i=1; i<found; i++){
            if(i<=4){
              if(!variables[i])
                  continue;
            }
            printf("%d ", value[j][i]);
        }
        printf("\n");
    }
}
//===========================================================================================

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

//====================================Inicialização==========================================
void setvariables(){
    int aux = atomics, cont=0, j;
    for(int i=1; i<5; i++){
        if(variables[i]){
            aux--;
            for(j=0; j<pow(2,atomics);){
                for(cont = 0;cont<pow(2,aux); cont++){
                    value[cont+j][i]=0;
                }
                j+=cont;
                for(cont = 0;cont<pow(2,aux); cont++){
                    value[j+cont][i]=1;
                }
                j+=cont;
            }
        }
    }
}

void pass(int begin, int end){
    int i, j;
    for(i = begin, j=0; i<=end; i++, j++) substr[found][j] = str[i];
    substr[found][j] = '\0';
    found++;
}

int findSub(int where){
    int i;
    for(;str[where]!=')' && mkd[where]!=1 && where<strlen(str); where++);
    for(i=where; i>=0; i--)  if(str[i]=='(' && mkd[i]==0) break;
    if(i==-1) return 0;

    pass(i, where);
    mkd[where] = 1;
    mkd[i] = 1;

    if(where<strlen(str)) findSub(where+1);

}

void init(){
    int i, aux, cont=0;
    for(i = 0; i<strlen(str); i++){
       aux = isAtomic(str[i]);
       if(aux){
          substr[aux][0] = str[i];
          substr[aux][1] = '\0';
          if(!variables[aux]){
             variables[aux] = 1;
             cont++;
          }
       }

       mkd.push_back(0);
    }
    atomics = cont;
    setvariables();

    found = 5;
    findSub(0);
}
//===========================================================================================

int main(){
    scanf(" %s", str);

    init();

    print();

   return 0;
}
