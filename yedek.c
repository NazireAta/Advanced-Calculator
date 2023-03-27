#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int search_char(char* str, char to_find) { //çalışıyo
    int open_parentheses = 0;
    for(int i = 0; i<strlen(str); i++) {
        if(str[i] == '(') {
            open_parentheses++;
        }
        else if(str[i] == ')') {open_parentheses--;}
        else if(open_parentheses==0) {
            if(str[i]==to_find) {return i;}
        }
    }
} 

char* comments(char* data){
    int loc = search_char(data,'%');
    char* output = (char*)malloc(loc+1);
    strncpy(output, data, loc);
    output[loc] = '\0';
    return output;
}

int main(){
    char* annen = "adskjdhdhaj%%jdakjda";
    
    printf(annen);
    annen = comments(annen);
    printf("\n%s",annen);
}