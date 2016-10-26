#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <cstring>
#include <vector>
#include <math.h>

using namespace std;

typedef vector <int> vi;
typedef pair <int, int> ii;
typedef vector<ii> vii;

//=====================================Funções úteis=========================================
void swap(char str1[], char str2[]){
    char aux[201];
    strcpy(aux, str1);
    strcpy(str1, str2);
    strcpy(str2, aux);
}

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
    if(strlen(str)<=3){
        fprintf(saida ,"satisfativel e refutavel\n\n");
    }else{
        if(insat){
            fprintf(saida ,"insatistativel e refutavel\n\n");
        }else if(taut){
            fprintf(saida ,"satistativel e tautologia\n\n");
        }else{
            fprintf(saida ,"satisfativel e refutavel\n\n");
        }
    }
}


void print(){

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

int Fnot(int a){
    if(a==0) return 1;
    return 0;
}
//===========================================================================================

//====================================inicialização==========================================
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
    findSub(0);

    return 0;
}
