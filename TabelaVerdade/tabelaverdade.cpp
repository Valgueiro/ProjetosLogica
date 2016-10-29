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

int found, atomics, tam, accFound;
int variables[5], value[16][100];
vi mkd;
char str[201];//Guarda a completa
char substr[50][201];//Guarda todas as subexpressões

FILE *entrada;
FILE *saida;

//=====================================Funções úteis=========================================
void swap(char str1[], char str2[]){
    char aux[201];
    strcpy(aux, str1);
    strcpy(str1, str2);
    strcpy(str2, aux);
}

void endit(char str[]){
    str[0] = 'e';
    str[1] = 'n';
    str[2] = 'd';
    str[3] = '\0';
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
            fprintf(saida ,"insatisfativel e refutavel\n\n");
        }else if(taut){
            fprintf(saida ,"satisfativel e tautologia\n\n");
        }else{
            fprintf(saida ,"satisfativel e refutavel\n\n");
        }
    }

}

void printLinha(){
    int i;
    for(i=0; i<tam; i++){
        fprintf(saida ,"-");
    }
    fprintf(saida ,"\n");
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
                    fprintf(saida ,"|%s|", substr[i]);
                    first = false;
                }else{
                    fprintf(saida ,"%s|", substr[i]);
                }
            }
            espacos[i]=0;
        }else{
            if(strcmp(substr[i],"end")!=0){
                fprintf(saida ,"%s|",substr[i]);
                if(isAtomic(substr[i][0])) espacos[i]=0;
                else  espacos[i]=strlen(substr[i])-1;
            }
        }

    }

    fprintf(saida, "\n");

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
                 fprintf(saida, "|%d|", value[j][i]);
                 first = false;
            }else{
                if(strcmp(substr[i],"end")!=0){
                    for(int k=0; k<espacos[i]; k++){
                        fprintf(saida, " ");
                    }
                    fprintf(saida, "%d|", value[j][i]);
                }
            }
        }
        fprintf(saida, "\n");
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

//====================================inicialização==========================================
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
    substr[1][0] = 'x';
    substr[1][1] = '\0';
    substr[2][0] = 'y';
    substr[2][1] = '\0';
    substr[3][0] = 'z';
    substr[3][1] = '\0';
    substr[4][0] = 't';
    substr[4][1] = '\0';

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

void orderSubs(){
    int i=0, j=0;
    for(i=5; i<found; i++){
        for(j=i+1; j<found; j++){
            if(strlen(substr[j])<strlen(substr[i])){
                swap(substr[j], substr[i]);
            }else if(strlen(substr[j])==strlen(substr[i])){
                for(int k=0; k<strlen(substr[j]); k++){
                    if(substr[i][k]!=substr[j][k]){
                        if(substr[i][k]>substr[j][k]){
                            swap(substr[j], substr[i]);
                        }
                        break;
                    }
                }
            }

        }
    }

    accFound = found;
    for(i=0; i<found; i++){
        if(!strcmp(substr[i],substr[i+1])){
            endit(substr[i]);
            accFound--;
        }
    }



}

void init(){
    int i, aux, cont=0;

    for(i=0; i<5;i++){
        variables[i] = 0;
    }

    for(i = 0; i<strlen(str); i++){
       aux = isAtomic(str[i]);
       if(aux){
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

    orderSubs();
    //qtd de ---
    tam = atomics;
    for(i=5; i<found; i++){
        if(strcmp(substr[i],"end")!=0){
            tam+=strlen(substr[i]);
        }
    }
    tam+=accFound-4+atomics;
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
    int i, j, qtd, cont;

    entrada = fopen("EntradaTabela.in", "r");

    saida = fopen("saida.txt", "w+");
    fscanf(entrada, "%d\n", &qtd);

    for(cont=1; cont<=qtd;cont++){
        mkd.resize(0);
        found = 0;
        atomics = 0;
        tam = 0;

        fprintf(saida, "Tabela #%d\n", cont);
        fgets(str, 201, entrada);
        init();

        for(i=5; i<found; i++){
            for(j=0; j<pow(2, atomics); j++){
                if(strcmp(substr[i],"end")!=0){
                    value[j][i] = setvalue(substr[i], j, 0, strlen(substr[i]));
                }
            }
        }

        print();
    }

    return 0;
}
