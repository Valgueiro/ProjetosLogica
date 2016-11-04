#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <cstring>
#include <vector>

using namespace std;

//================================Variaveis Globais================================//
bool horn, fnc;
int found=0;
char str[1001], substr[750][751];
vector<int> mkd;

FILE* in = fopen("Expressoes.in", "r");
FILE* out = fopen("Expressoes.out", "w");
//=================================================================================//

//==================================Funções úteis==================================//
void swap(char str1[], char str2[]){
    char aux[201];
    strcpy(aux, str1);
    strcpy(str1, str2);
    strcpy(str2, aux);
}

int isAtomic(char a){
    if(a == 'a') return 1;
    if(a == 'b') return 2;
    if(a == 'c') return 3;
    if(a == 'd') return 4;
    return 0;
}
//=================================================================================//

//===========================Encontrar as SubExpressões============================//
void pass(int begin, int end){
    int i, j;
    for(i = begin, j=0; i<=end; i++, j++){
        substr[found][j] = str[i];
        mkd[i] = 1;
    }
    substr[found][j] = '\0';
    found++;
}

int findSub(){
    int i=0, j=0;
    while(i<strlen(str)){
        for(; i<strlen(str); i++){
            if(str[i]=='.'){
                if(!mkd[i]){
                    break;
                }
            }
        }
        mkd[i] = 1;

        for(j=0; j<strlen(str); j++){
            if(str[j]=='('){
                if(!mkd[j]){
                    break;
                }
            }
        }
        mkd[j] = 1;
        pass(j, i-1);
    }
}
//=================================================================================//

//==============================Analisar(FNC e Horn)===============================//
void analyze(){
    int i, j;
    for(i=0; i<found; i++){
        for(j=1; j <strlen(substr[i])-1; j++){
            if(isAtomic(substr[i][j]) == 0 && (substr[i][j] != '+' && substr[i][j] != '-')){
                fnc = false;
                break;
            }
        }
        if(!fnc) break;
    }

    int cont;
    if(fnc){
        for(i=0; i<found; i++){
            cont = 0;
            for(j=1; j <strlen(substr[i]); j++){
                if(isAtomic(substr[i][j])){
                    if(substr[i][j-1]!='-') cont++;
                }
            }
            if(cont>1){
                horn = false;
                break;
            }
        }
    }
}
//=================================================================================//

//===============================Encontrando SAT===================================//
void clear(char str[], int begin){
    int i, qtd=2;

    if((begin+2) == strlen(str) && (str[begin-1]=='(' || str[begin-2]=='(')){
        str[0] = '\0';
    }else{
        if(str[begin-1]=='+'){
            begin--;
        }else if(str[begin-1]=='-'){
            qtd++;
            begin--;
            if(str[begin-1] == '+'){
                begin--;
            }
        }

        for(i=begin; i+qtd<=strlen(str); i++){
            str[i] = str[i+qtd];
        }
        str[i] = '\0';
    }
}

bool upd(char str[], char op, int key, bool neg){
    int tam = strlen(str), k;
    bool res = false;

    for(k=0; k<tam; k++){
        if(str[k]==op && k!=key){
            if(str[k-1] == '-'){
                if(!neg){
                    res = true;
                }
            }else{
                if(neg){
                    res = true;
                }
            }
            clear(str, k);
            tam = strlen(str);
            k=0;
        }
    }
    return res;
}

void minimize(){
    int i, j, k, f, aux;
    bool fri, neg;

    for(i=0; i<found; i++){
        for(j=0; j<strlen(substr[i]); j++){
            f = isAtomic(substr[i][j]);
            if(f){
                fri = false;
                neg = false;

                if(substr[i][j-1]=='-'){
                    neg = true;
                }

                fri = upd(substr[i], substr[i][j], j, neg);

                if(fri){
                    substr[i][0] = '\0';
                }
            }
        }
    }
}

bool contradicao(){
    bool first = true;
    int aux;
    int ready = 0, l, find=0, i, m;
    
    for(i=0; i<found; i++){
        if(strlen(substr[i])==3){
            if(first){
                first = false;
                aux = isAtomic(substr[i][1]);
                l=i;
            }else if(aux == isAtomic(substr[i][1])){
                find++;
                m=i;
                break;
            }
        }else if(strlen(substr[i])==4){
            if(first){
                first = false;
                aux = isAtomic(substr[i][2]);
                l=i;
            }else if(aux == isAtomic(substr[i][2])){
                find++;
                m=i;
                break;
            }
        }
    }

    if(first) return false;

    if(find==1){
        if(substr[l][1]=='-'){
            if(substr[m][1] == substr[l][2]){
                return true;
            }
      
            
        }else{
            if(substr[m][2] == substr[l][1]){
                return true;
            }              
        }
    }
    
    return false;
}

int sat(){
    char aux[4];
    int i, j, f;
    bool find = false;
    
    if(contradicao()){
        return 0;
    }

    for(i=0; i<found; i++){
        if(strlen(substr[i])==3){
            find = true;
            aux[0] = substr[i][1];
            aux[1] = '\0';
            substr[i][0]='\0';
            f = isAtomic(substr[i][1]);
            break;
        }
    }

    if(!find){
        return 1;
    }

    for(i=0; i<found; i++){
        for(j=0; j<strlen(substr[i]); j++){
            if(isAtomic(substr[i][j]) == f){
                if(substr[i][j-1]=='-'){
                    clear(substr[i], j);
                    j=0;
                }
            }
        }
     }

    return sat();
}
//=================================================================================//

//============================Inicialização e Print================================//
void init(){
    fscanf(in, " %s", str);

    mkd.clear();
    for(int i=0; i<strlen(str); i++){
        mkd.push_back(0);
    }

    horn = true;
    fnc = true;

    found = 0;
    findSub();

    analyze();
}

void resultado(int caso){  
    fprintf(out, "caso #%d: ", caso);
    
    if(!horn){
        fprintf(out, "nem todas as clausulas sao de horn\n");
    }else if(!fnc){
        fprintf(out, "nao esta na FNC\n");
    }else{
        minimize();
        bool res = sat();
        if(res){
            fprintf(out, "satisfativel\n");
        }else{
            fprintf(out, "insatisfativel\n");
        }
    }
}
//=================================================================================//

//=====================================Main========================================//
int main(){
    int i, res, caso, aux;
    
    fscanf(in, "%d", &aux);
    
    for(caso = 1; caso<=aux; caso++){
        init();
      
        resultado(caso);
    }
    
    return 0;
}
//=================================================================================//