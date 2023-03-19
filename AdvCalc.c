#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct node {
    int value;
    char *data;
    char *operation;
    struct node* left;
    struct node* right;
};

struct node* newNode(int value, char* data, char *operation) { //value is null if it the node does not have a value yet
    struct node* node
        = (struct node*)malloc(sizeof(struct node));
  
    node->value = value;
    node->data = data;
    node->operation = operation;

    node->left = NULL;
    node->right = NULL;
    return (node);
}

int *plus(int *ap, int *bp) {
    int* x;
    int ans = (*ap + *bp);
    x= &ans;
    return x;
}

int *times(int *ap, int *bp) {
    int* x;
    int ans = (*ap * *bp);
    x= &ans;
    return x;
}

int *minus(int *ap, int *bp) {
    int* x;
    int ans = (*ap - *bp);
    x= &ans;
    return x;
}

int *and(int *ap, int *bp) {
    int* x;
    int ans = (*ap & *bp);
    x= &ans;
    return x;
}

int *or(int *ap, int *bp) {
    int* x;
    int ans = (*ap | *bp);
    x= &ans;
    return x;
}

int *xor(int *ap, int *bp) {
    int* x;
    int ans = (*ap ^ *bp);
    x= &ans;
    return x;
}

int *left_shift(int *ap, int *ip) {
    int* x;
    int ans = (*ap << *ip);
    x= &ans;
    return x;
}

int *right_shift(int *ap, int *ip) {
    int* x;
    int ans = (*ap >> *ip);
    x= &ans;
    return x;
}
/*
int search(char* str, char to_find) {
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
*/
void divide(struct node* root) {
    bool isTerminal = true; //update if /*+- are seen
    int open_parentheses = 0;
    char *data = root->data;
    for(int i = 0; i<strlen(data); i++) {
        char curr = data[i];
        int parentheses_begin = 0;  //niye actigimi unuttum -> xor mu(!=0) yoksa duz parantez mi(==0) ayirt etmek icin
        if(curr == '(') {
        isTerminal = false;
            open_parentheses++;
            if(open_parentheses== 0) {parentheses_begin = i;}
        }
        else if(curr == ')') {open_parentheses--;}
        else if(open_parentheses==0) {
            char one[i];
            char two[strlen(data)-i-1];
            int oridx = -1, andidx = -1, plusidx = -1, minusidx = -1, timesidx = -1;
            switch(curr) {
                case '|':
                    isTerminal = false;
                    /*
                    strncpy(one, data, i);
                    strcpy(two, &data[i+1]);
                    root->left = newNode(0, one, NULL);
                    root->right = newNode(0,two,NULL);
                    root->operation = "|";
                    */
                    oridx = i;
                    break;
                case '&':
                    isTerminal = false;
                    andidx = i;
                    break;
                case '+':
                    isTerminal = false;
                    plusidx = i;
                    break;
                case '-':
                    isTerminal = false;
                    minusidx = i;
                    break;
                case '*':
                    isTerminal = false;
                    timesidx = i;
                    break;    
                default:
                    break;
            }
            if(oridx != -1) {
                strncpy(one, data, oridx);
                strcpy(two, &data[oridx+1]);
                root->left = newNode(0, one, NULL);
                root->right = newNode(0,two,NULL);
                root->operation = "|";
            }
            else if(andidx != -1) {
                strncpy(one, data, andidx);
                strcpy(two, &data[andidx+1]);
                root->left = newNode(0, one, NULL);
                root->right = newNode(0,two,NULL);
                root->operation = "&";
            }
            else if(plusidx != -1) {
                strncpy(one, data, plusidx);
                strcpy(two, &data[plusidx+1]);
                root->left = newNode(0, one, NULL);
                root->right = newNode(0,two,NULL);
                root->operation = "+";
            }
            else if(minusidx != -1) {
                strncpy(one, data, minusidx);
                strcpy(two, &data[minusidx+1]);
                root->left = newNode(0, one, NULL);
                root->right = newNode(0,two,NULL);
                root->operation = "-";
            }
            else if(timesidx != -1) {
                strncpy(one, data, timesidx);
                strcpy(two, &data[timesidx+1]);
                root->left = newNode(0, one, NULL);
                root->right = newNode(0,two,NULL);
                root->operation = "*";
            }
        }
    }
    if(open_parentheses != 0){
        printf("error!");
    }
}

int main() {
    int t = 40, qt = 15;
    int x = 2;
    //int ans = *or(and(&t,&t),plus(&t, xor(&t, left_shift(&qt, &x))));
    int a = 5, b = 3 , c = 7, d = 10, e = 2, f = 15, g = 1, h = 12;
//    int *aa, *bb;
//    aa = left_shift(&d,&e);
//    bb = or(right_shift(&f,&g),&h);
//    printf("aa = %d\n bb = %d\n",*aa, *bb);
    int ans = *xor(times(plus(&a,&b),&c),and(left_shift(&d,&e),or(right_shift(&f,&g),&h)));
    printf("ans = %d\n",ans);
//    ans = *xor(&t,&qt);
//    ans = *and(aa,bb);
    ans = *and(left_shift(&d,&e),or(right_shift(&f,&g),&h));
    printf("ans = %d\n",ans);

            

}

