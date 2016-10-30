#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <cstring>
#include <vector>

using namespace std;

typedef vector <int> vi;
typedef pair <int, int> ii;
typedef vector<ii> vii;

int found=0;
char str[1001], substr[750][751];
vi mkd;
//=====================================Funções úteis=========================================
void swap(char str1[], char str2[]){
    char aux[201];
    strcpy(aux, str1);
    strcpy(str1, str2);
    strcpy(str2, aux);
}

int isAtomic(char a){
    if(a == 'a')
        return 1;
    if(a == 'b')
        return 2;
    if(a == 'c')
        return 3;
    if(a == 'd')
        return 4;
    return 0;
}

//====================================inicialização==========================================
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

//===========================================================================================


int main(){
    int i;
    scanf(" %s", str);
    for(i=0; i<strlen(str); i++){
        mkd.push_back(0);
    }
    found = 0;
    findSub();

    for(i=0; i<found; i++){
        printf("%s \n", substr[i]);
    }

    return 0;
}
