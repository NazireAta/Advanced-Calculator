#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define INT_BITS 32
#define TABLE_SIZE 128

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

typedef struct var {
    char* key;
    int value;
    struct var* next;
} var;

typedef struct hash_table {
    var* table[TABLE_SIZE]; //initialize table of length 128
} hash_table;

int hash(char* key) { //hash function
    int val = 0;
    for(int i = 0; i<strlen(key); i++){
        char c = key[i];
        val = (val + (int)(c)*(int)pow(31,i)) %TABLE_SIZE;
    }
    return val;
}

var* create_var(char* key, int value) { //contructor
    var* new_var = (var*)malloc(sizeof(var));
    new_var->key = key;
    new_var->value = value;
    new_var->next = NULL;
    return new_var;
}


void insert(hash_table* ht, char* key, int value) { // insert a variable into the hash table
    int index = hash(key);
    var* curr = ht->table[index];
    if (curr == NULL) { //variable is not defined yet
        ht->table[index] = create_node(key, value);
    } 
    else {
        while (curr->next != NULL) {
            if (strcmp(curr->key, key) == 0) {
                curr->value = value;
                return;
            }
            curr = curr->next;
        }
        if (strcmp(curr->key, key) == 0) {curr->value = value;} 
        else {curr->next = create_node(key, value);}
    }
}

int get(hash_table* ht, char* key) { //get the value of the variable from hash table
    int index = hash(key);
    var* curr = ht->table[index];
    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) {return curr->value;}
        curr = curr->next;
    }
    return 0; //variables that are not defined yet are 0
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

char* remove_whitespaces(char* s) {  //çalışıyopull
    /*char* new_str = (char*)malloc(strlen(str) + 1); // allocate memory for the new string
char* remove_whitespaces(char* s) {  //çalışıyo
    /*char* new_str = (char*)malloc(strlen(str) + 1); // allocate memory for the new string
    if (!new_str) {
        return NULL; // error, unable to allocate memory
    }
    int j = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isspace(str[i])) { // check if the current character is not a whitespace
            new_str[j++] = str[i];
        }
    }
    new_str[j] = '\0'; // add null terminator to the new string
    return new_str;*/

    int size;
    int end = strlen(s)-1;
    int begin=0;
    size = strlen(s);


    if (!size) {
        return s;
    }

    while (end >=0  && isspace(s[end]))
        end--;

    while (begin<size && isspace(s[begin]))
        begin++;

    char* new = (char*)malloc(strlen(s));
    int len = end - begin + 1;

    strncpy(new, s + begin, len);
    new[len] = '\0';
    return new;
}


char* remove_parentheses(const char* data) {
    int count = 0;
    for (int i = 0; i < strlen(data); i++) {
        if (data[i] == '(') {count++;}
    }
    char* output = (char*)malloc(strlen(data) - 2 * count + 1);
    int j = 0;
    for (int i = 0; i < strlen(data); i++) {
        if (data[i] == '(' || data[i] == ')') {continue;}
        output[j++] = data[i];
    }
    output[j] = '\0';
    return output;
}

bool is_variable(char* data) { //returns  if the given string only consists of alphabetical chars
    for(int i = 0; i < strlen(data); i++) {
        if (!isdigit(data[i])) {
            return true;
        }
    }
    return false;
}

bool is_valid_variable(char* data) {
    for(int i = 0; i < strlen(data); i++) {
        if(!isalpha(data[i])) {
            return false;
        }
    }
    return true;
}

void divide(struct node* root) {
    bool isTerminal = true; 
    int open_parentheses = 0;
    char *data = root->data;
    data = remove_whitespaces(data);
    data = remove_whitespaces(data);
    int oridx = -1, andidx = -1, plusidx = -1, minusidx = -1, timesidx = -1;
    int len = strlen(data);
    char one[len];
    char two[len];
    int len = strlen(data);
    char one[len];
    char two[len];
    int parentheses_begin = -1;   //niye actigimi unuttum -> xor mu(!=0) yoksa duz parantez mi(==0) ayirt etmek icin
    //hala -1 se hiç parantez yok demektir ilk indexte de olabilir
    for(int i = 0; i<strlen(data); i++) {
        char curr = data[i]; 
        if(curr == '(') { 
        if(curr == '(') { 
            isTerminal=false;
            open_parentheses++;
            if(parentheses_begin == -1) {parentheses_begin = i;}  //parantezin ilk başladığı yerin indexi
        }
        else if(curr == ')') {open_parentheses--;}
        else if(open_parentheses==0) {
            switch(curr) {
                case '|':
                    isTerminal = false;
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
        char fnc_name[3];
        //strncpy(fnc_name, data, parentheses_begin); //works right
        strncpy(fnc_name, data, 3);  //başında zaten boşluk yok ilk 3 karakteri alsın "ls " ya da "ls(" diye kontrol edelim
        char fnc_name[3];
        //strncpy(fnc_name, data, parentheses_begin); //works right
        strncpy(fnc_name, data, 3);  //başında zaten boşluk yok ilk 3 karakteri alsın "ls " ya da "ls(" diye kontrol edelim
        int comma;
        if(strcmp(fnc_name, "xor") == 0) {  
            /*comma = search_char(data+parentheses_begin, ',');   
        if(strcmp(fnc_name, "xor") == 0) {  
            /*comma = search_char(data+parentheses_begin, ',');   
            char one[comma +1];                                 
            char two[strlen(data)-comma-4]; 
            strncpy(one, data+4, comma);                     
            one[comma] = 0;                     
            strncpy(two, data+comma+5,strlen(data)-comma-6);    
            two[strlen(data)-comma-6]=0;*/

            comma = search_char(data+parentheses_begin+1, ',');   //works right, dot it also for else ifs
            char one[comma +1];
            char two[strlen(data)-comma-parentheses_begin-2];
            strncpy(one, data+parentheses_begin+1, comma);
            one[comma] = 0;    
            strncpy(two, data+comma+parentheses_begin+2,strlen(data)-comma-parentheses_begin-3);
            two[strlen(data)-comma-parentheses_begin-2]=0;
            char* new_one = remove_whitespaces(one);
            char* new_two= remove_whitespaces(two);
            root->left = newNode(0, new_one, NULL);
            root->right = newNode(0,new_two,NULL);
            root->operation = "xor";
        }
        else if(strcmp(fnc_name, "ls ") || strcmp(fnc_name, "ls(")){
            comma = search_char(data+parentheses_begin, ',');
            char one[comma +1];
            char two[strlen(data)-comma-3];
            strncpy(one, data+3, comma);
            one[comma] = 0;
            strncpy(two, data+comma+4,strlen(data)-comma-5);
            two[strlen(data)-comma-5]=0;
            root->left = newNode(0, one, NULL);
            root->right = newNode(0,two,NULL);
            root->operation = "ls";
        }
        else if(strcmp(fnc_name, "rs ") || strcmp(fnc_name, "rs(")){
            comma = search_char(data+parentheses_begin, ',');
            char one[comma +1];
            char two[strlen(data)-comma-3];
            strncpy(one, data+3, comma);
            one[comma] = 0;
            strncpy(two, data+comma+4,strlen(data)-comma-5);
            two[strlen(data)-comma-5]=0;
            root->left = newNode(0, one, NULL);
            root->right = newNode(0,two,NULL);
            root->operation = "rs";
        }
        else if(strcmp(fnc_name, "lr ") || strcmp(fnc_name, "lr(")) {
            comma = search_char(data+parentheses_begin, ',');
            char one[comma +1];
            char two[strlen(data)-comma-3];
            strncpy(one, data+3, comma);
            one[comma] = 0;
            strncpy(two, data+comma+4,strlen(data)-comma-5);
            //two[strlen(data)-comma-3]=0;  //
            two[strlen(data)-comma-5]=0;   //böyle doğru veriyo tekrar kontrol ederiz
            root->left = newNode(0, one, NULL);
            root->right = newNode(0,two,NULL);
            root->operation = "lr";
        }
        else if(strcmp(fnc_name, "rr ") || strcmp(fnc_name, "rr(")){
            comma = search_char(data+parentheses_begin, ',');
            char one[comma +1];
            char two[strlen(data)-comma-3];
            strncpy(one, data+3, comma);
            one[comma] = 0;
            strncpy(two, data+comma+4,strlen(data)-comma-5);
            two[strlen(data)-comma-5]=0;
            root->left = newNode(0, one, NULL);
            root->right = newNode(0,two,NULL);
            root->operation = "rr";
        }
        else if(strcmp(fnc_name, "not")) {
            char one[strlen(data)-5];
            strncpy(one, data+4,strlen(data)-4);
            one[strlen(data)-5] = 0;
            root->left=one;
            root->operation="not";
        }
        else if(parentheses_begin==0) { //bu hariç hepsinde root left rightlardan devam bunda aynı roottan devam ediyoruz
            char one[strlen(data)-2];
            strncpy(one, data+1,strlen(data)-2);
            root->data=one;
        }
    }

    if (!isTerminal) {
        if (root->left!=NULL) {
            divide(root->left);
            if (root->right!=NULL) {
            divide(root->right);
            }
        }
        else {
            divide(root);
        }        
    }
    
}

int execute(hash_table* ht, struct node* root) {
    if(root->operation==NULL) {            //burda isdecimal gibi bi şeyle variable mı sayı mı bak
        if(!is_variable) {
            root->value=atoi(root->data);
        }
        else{
            if(is_valid_variable(root->data)) {
                root->value =get(ht,root->data);
                //hashten değer getir value ya ver
            }
            else {
                printf("Error!")
                return;
            }
        }
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
    hash_table ht;
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht.table[i] = NULL;
    }

    char* data = "";
    while(scanf("%s", data)) {           //Burdan emin değilim
        struct node* root;

        int equals = search_char(data, '=');
        if(equals==strlen(data)){
            //atama yok
            data = remove_whitespaces(data); 
            data = remove_whitespaces(data); 
            root->data=data;
        }
        else{
            //atama var
            //variable isalphabetic mi diye bak
            int equals = search_char(data, '=');   //first find where the comma is, returns length of the first input of xor
            char variable[equals +1];
            char two[strlen(data)-equals];        // two işlemler          
            strncpy(variable, data, equals);                    
            variable[equals] = 0;   
            char* var = remove_whitespaces(variable);

            if(!is_valid_variable(var)) { 
                printf("%s", "ERROR");
                continue;
            }                                 
            strncpy(two, data+equals+1,strlen(data)-equals);
            two[strlen(data)-equals]=0;
            root->data=two;
            insert(&ht, var, root->value);
        }

        divide(root);
        int ans = execute(root);

        if (equals==strlen(data)) {
            printf("%d", ans);
        }
        else {
            // hashtable da char one = ans
        }
        
    }
    
    
    
    int t = 40, qt = 15;
    int x = 2;
    //int ans = *or(and(&t,&t),plus(&t, xor(&t, left_shift(&qt, &x))));
    int a = 5, b = 3 , c = 7, d = 10, e = 2, f = 15, g = 1, h = 12;
    
    
/*     char *data = "lr(mersdajsklda,insadlkdkl)";
    int comma = search_char(data+3,',');
    char one[comma +1];
    char two[strlen(data)-comma-3];
    printf("%d and value %c \n", comma, data[comma]);
    strncpy(one, data+3, comma);
    one[comma] = 0;
    strncpy(two, data+comma+4,strlen(data)-comma-5);
    two[strlen(data)-comma-3]=0;
    printf("%s ve de %s \n",one,two); 
 */


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
    //printf(~12);

    char *data2 = "((  (((H      ello) ))))";
    char *data22;
    printf("%s",remove_whitespaces(data2));

}
