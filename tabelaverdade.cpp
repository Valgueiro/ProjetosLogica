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
void printClassificacao(){
    int j;
    bool taut=true, insat=true;

    for(j=0; j<pow(2, atomics); j++){
        if(!value[j][found-1]){
            taut = false;
        }else{
            insat = false;
        }
    }

    if(insat){
        printf("insatistativel e refutavel\n");
    }else if(taut){
        printf("satistativel e tautologia\n");
    }else{
        printf("satisfativel e refutavel\n");
    }
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
            if(isAtomic(substr[i][0])) espacos[i]=0;
            else  espacos[i]=strlen(substr[i])-1;
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

    printClassificacao();

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
    tam = atomics;
    for(i=5; i<found; i++){
        tam+=strlen(substr[i]);
    }
    tam+=found-4+atomics;
}
//===========================================================================================

int setvalue(char str[], int linha, int begin, int end){
    int i, aux, aux1, aux2, fim, ini, cont = 0;
    if(begin==end){
        aux = isAtomic(str[end]);
        if(aux){//or
            return value[linha][aux];
        }
    }
    for(i=begin; i<end; i++){
        if(str[i]=='('){
            cont++;
        }else if(str[i] == ')'){
            cont--;
        }else{
            if(cont==1){
                if(str[i] == '-'){
                    return Fnot(setvalue(str, linha, i+1, end-1));
                }else if(str[i] == '.'){
                    aux1 = setvalue(str, linha, begin+1, i-1);
                    aux2 = setvalue(str, linha, i+1, end-1);
                    return Fand(aux1, aux2);
                }else if(str[i] == '+'){
                    aux1 = setvalue(str, linha, begin+1, i-1);
                    aux2 = setvalue(str, linha, i+1, end-1);
                    return For(aux1, aux2);
                }else if(str[i] == '>'){
                    aux1 = setvalue(str, linha, begin+1, i-1);
                    aux2 = setvalue(str, linha, i+1, end-1);
                    return Fimp(aux1, aux2);
                }
            }
        }
    }
    aux = isAtomic(str[begin]);
    if(aux){//or
        return value[linha][aux];
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
