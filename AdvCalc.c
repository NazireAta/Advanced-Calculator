#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define INT_BITS 32


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

/*struct stack {
    int top;
    int capacity;
    int* array;
};

struct stack* newStack(int capacity)
{
    struct stack* stack = (struct stack*)malloc(sizeof(struct stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

void push(struct stack* stack, struct node* item)
{
    stack->array[++stack->top] = item;
}

struct node* pop(struct stack* stack)
{
    return stack->array[stack->top--];
}

struct node* peek(struct stack* stack)
{
    return stack->array[stack->top];
}

//stackte push ve pop var peek e gerek yok tree yi dönüp ekle sonra hesaplarken popla teker teker
//stack gereksiz 
void addToStack(struct stack* stack, struct node* root) {
    push(stack, root);
    while (true)
    {
        if(root->left!=NULL){
            struct node* temp =root->left;
            push(stack, temp);
        }
        if(root->right!=NULL) {

        }
         
    }
    
}*/

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

int *right_rotate(int *ap, int *ip) {      //Test it
    int* x;
    int ans = or((*ap >> *ip), (*ap << (INT_BITS - *ip)));
    x= &ans;
    return x;
}

int *left_rotate(int *ap, int *ip) {       //Test it
    int* x;
    int ans = or((*ap << *ip), (*ap >> (INT_BITS - *ip)));
    x= &ans;
    return x;
}

int *not(int ap) { 
    int ans = ~(ap);
    int* x = &ans;
    return x;
}                                       //Test it

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

char* removeWhitespaces(char* input)                                         
{
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i < strlen(input); i++,j++)          
    {
        if (input[i]!=' ')                           
            output[j]=input[i];                     
        else
            j--;                                     
    }
    output[j]=0;
    return output;
}

char* remove_parantheses(char* data) {
//doldur abla
}

void divide(struct node* root) {
    bool isTerminal = true; //update if /*+- are seen
    int open_parentheses = 0;
    char *data = root->data;
    int oridx = -1, andidx = -1, plusidx = -1, minusidx = -1, timesidx = -1;
    char one[strlen(data)];
    char two[strlen(data)];
    int parentheses_begin = -1;   //niye actigimi unuttum -> xor mu(!=0) yoksa duz parantez mi(==0) ayirt etmek icin
    //hala -1 se hiç parantez yok demektir ilk indexte de olabilir
    for(int i = 0; i<strlen(data); i++) {
        char curr = data[i]; 
        if(curr == '(') { //parantez varsa bence is terminal true kalsın çünkü parantez olup operatörler olmayabilir hem fonksiyonları kontrol ederken işimize yarar
            open_parentheses++;
            if(parentheses_begin == -1) {parentheses_begin = i;}  //parantezin ilk başladığı yerin indexi
        }
        else if(curr == ')') {open_parentheses--;}
        else if(open_parentheses==0) {
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
                    if (oridx==-1)
                    {
                        oridx = i;
                        break;
                    }
                    break;
                case '&':
                    isTerminal = false;
                    if (andidx==-1)
                    {
                        andidx = i;
                    }
                    break;
                case '+':
                    isTerminal = false;
                    if (plusidx==-1)
                    {
                        plusidx = i;
                    }
                    break;
                case '-':
                    isTerminal = false;
                    if (minusidx==-1)
                    {
                        minusidx = i;
                    }
                    break;
                case '*':
                    isTerminal = false;
                    if (timesidx==-1)
                    {
                        timesidx = i;
                    }
                    break;    
                default:
                    break;
            }
            
        }
    }
    if(open_parentheses != 0){
        printf("error!");
    }

    else if(oridx != -1) {
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

    else {
        char fnc_name[parentheses_begin];
        strncpy(fnc_name, data, parentheses_begin);
        switch(fnc_name) {
            case "":
                break;
            case "xor":
                root->operation="xor"
                //parantez dengeliyken ilk virgülden ayır
                break;
            //devam
        }
    }
}

//TO DO: remove enclosıng parantheses
int execute(struct node* root) {
    if(root->operation==NULL) {
        root->value=atoi(root->data);
        return root->value;
    }
    else if(root->operation=="+") {
        root->value=execute(root->left) + execute(root->right);
    }
    else if(root->operation=="-"){
        root->value=execute(root->left) - execute(root->right);
    }
    else if(root->operation=="*"){
        root->value=execute(root->left) * execute(root->right);
    }
    else if(root->operation=="&"){
        root->value=and(execute(root->left), execute(root->right));
    }
    else if(root->operation=="|"){
        root->value=or(execute(root->left), execute(root->right));
    }
    else if(root->operation=="xor"){
        root->value=xor(execute(root->left), execute(root->right));
    }
    else if(root->operation=="ls"){
        root->value=left_shift(execute(root->left), execute(root->right));
    }
    else if(root->operation=="rs"){
        root->value=right_shift(execute(root->left), execute(root->right));
    }
    else if(root->operation=="lr"){
        root->value=left_rotate(execute(root->left), execute(root->right));
    }
    else if(root->operation=="rr"){
        root->value=right_rotate(execute(root->left), execute(root->right));
    }
    else if(root->operation=="not"){
        root->value=not(execute(root->left));
    }
    
    return root->value;
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
    //printf("ans = %d\n",ans);
//    ans = *xor(&t,&qt);
//    ans = *and(aa,bb);
    ans = *and(left_shift(&d,&e),or(right_shift(&f,&g),&h));
    //printf("ans = %d\n",ans);
    printf(~12);

            

}

