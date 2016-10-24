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

int found, atomics, tam;
int variables[5], value[16][100];
vi mkd;
char str[201];//Guarda a completa
char substr[50][201];//Guarda todas as subexpressões


int setvalue(char str[], int linha, int begin, int end);

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
    return value[j][aux];
}

void printLinha(){
    int i;
    for(i=0; i<tam; i++){
        printf("-");
    }
    printf("\n");
}

void print(){
    bool first=true;
    int i, j, espacos[200];
    //Primeira linha com as subexpressões:
    printLinha();
    for(i=1; i<found; i++){
        if(i<=4){
            if(variables[i]){
                if(first){
                    printf("|%s|", substr[i]);
                    first = false;
                }else{
                    printf("%s|", substr[i]);
                }
            }
            espacos[i]=0;
        }else{
            printf("%s|",substr[i]);
            espacos[i]=strlen(substr[i])-1;
        }

    }
    printf("\n");

    //Valores da tabela;
    for(j=0; j<pow(2, atomics); j++){
        printLinha();
        first = true;
        for(i=1; i<found; i++){
            if(i<=4){
              if(!variables[i])
                  continue;
            }
            if(first){
                 printf("|%d|", value[j][i]);
                 first = false;
            }else{
                for(int k=0; k<espacos[i]; k++){
                    printf(" ");
                }
                printf("%d|", value[j][i]);
            }
        }
        printf("\n");
    }
    printLinha();
}

int final(char str[], int begin){
    int end = begin;
    while(str[end]!=')'){
        end++;
    }
    return end-1;
}

int comeco(char str[], int end){
    int begin = end;
    while(str[begin]!='('){
        begin--;
    }
    return begin-1;
}

int takevalue(int i, char str[], int* aux1, int* aux2, int linha){
    int ini, fim;
    if(isAtomic(str[i-1])){
        *aux1 = valor(str[i-1], linha);
    }else{
        ini = comeco(str, i-1);
        *aux1 = setvalue(str, linha, ini, i-1);
        printf("sub:");
        for(int j=ini; j<i-1;j++){
            printf("%c", str[j]);
        }
        printf(" -> value:%d\n", *aux1);
    }
    if(isAtomic(str[i+1])){
        *aux2 = valor(str[i-1], linha);
    }else{
        fim = final(str, i+1);
        *aux2 = setvalue(str, linha, i+1, fim);
        printf("sub:");
        for(int j=i+1; j<fim;j++){
            printf("%c", str[j]);
        }
        printf(" -> value:%d\n", *aux2);
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

    //qtd de ---
    tam = atomics, i;
    for(i=5; i<found; i++){
        tam+=strlen(substr[i]);
    }
    tam+=found-1;
}
//===========================================================================================

int setvalue(char str[], int linha, int begin, int end){
    int i, aux, aux1, aux2, fim, ini;
    for(i=begin; i<end; i++){
         if(str[i] == '+'){//or
            printf("or:\n");
            takevalue(i, str, &aux1, &aux2, linha);
            return For(aux1, aux2);
        }else if(str[i] == '.'){
            printf("and:\n");
            takevalue(i, str, &aux1, &aux2, linha);
            return Fand(aux1, aux2);
        }else if(str[i] == '>'){
            printf("imp:\n");
            takevalue(i, str, &aux1, &aux2, linha);
            return Fimp(aux1, aux2);
        }else if(str[i] == '-'){//not
            if(isAtomic(str[i+1])){
                printf("not:%c\n", str[i+1]);
                printf("%d->%d\n",valor(str[i+1], linha), Fnot(valor(str[i+1], linha)));
                return Fnot(valor(str[i+1], linha));
            }else{
                fim = final(str, i+1);
                aux = setvalue(str, linha, i+1, fim);
                return Fnot(aux);
            }
        }
    }
}

int main(){
    int i, j;
    scanf(" %s", str);

    init();

    for(i=5; i<found; i++){
        for(j=0; j<pow(2, atomics); j++){
            value[j][i] = setvalue(substr[i], j, 0, strlen(substr[i]));
        }
    }


    print();

   return 0;
}
