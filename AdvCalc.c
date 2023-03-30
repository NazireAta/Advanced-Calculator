#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

#define INT_BITS 32
#define TABLE_SIZE 128
bool s_et = false;


struct node { //every node of the binary tree must hold
    int value;
    char *data;
    char *operation;
    struct node* left;
    struct node* right;
};

struct node* newNode(int value, char *data, char *operation) { //value is null if it the node does not have a value yet
    struct node* node = (struct node*)malloc(sizeof(struct node));

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
        int c = key[i];
        val = (int)(val +(int)(c)*pow(31,i)) % TABLE_SIZE;
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
        ht->table[index] = create_var(key, value);
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
        else {curr->next = create_var(key, value);}
    }
}

int get(hash_table* ht, char* key) { //get the value of the variable from hash table
    int index = hash(key);
    var* curr = ht->table[index];
    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) { return curr->value; }
        curr = curr->next;
    }
    return 0;
}


int plus(int ap, int bp) {
    int ans = ap + bp;      // adres verme şeyini kaldırınca düzeldi &
    return ans;
}

int times(int ap, int bp) {
    int ans = ap * bp;
    return ans;
}

int minus(int ap, int bp) {
    int ans = ap -  bp;
    return ans;
}

int and(int ap, int bp) {
    int ans =  ap &  bp;
    return ans;
}

int or(int ap, int bp) {
    int ans =  ap |  bp;
    return ans;
}

int xor(int ap, int bp) {
    int ans =  ap ^  bp;
    return ans;
}

int left_shift(int ap, int ip) {
    int ans =  ap <<  ip;
    return ans;
}

int right_shift(int ap, int ip) {
    int ans =  ap >>  ip;
    return ans;
}

int right_rotate(int ap, int ip) {      //Test it
    int ans = or(( ap >>  ip), (  ap << (INT_BITS -  ip)));
    return ans;
}

int left_rotate(int ap, int ip) {       //Test it
    int ans = or(( ap <<  ip),(  ap >> (INT_BITS -  ip)));
    return ans;
}

int not(int ap) {
    int ans = ~(ap);
    return ans;
}                                     //Test it


int search_char(char* str, char to_find) {    // works right
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

char* remove_whitespaces(char* s) {  // works right
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
    if(len<0){return "";}
    strncpy(new, s + begin, len);
    new[len] = '\0';
    return new;
}

char* remove_parentheses(const char* data) {            //works right
    char* output = (char*)malloc(strlen(data) - 2 + 1);
    strncpy(output, data + 1, strlen(data) - 2);
    output[strlen(data) - 2] = '\0';
    return output;
}

char* comments(char* data){
    int loc = search_char(data,'%');
    char* output = (char*)malloc(loc+1);
    strncpy(output, data, loc);
    output[loc] = '\0';
    return output;
}

bool is_variable(char* data) {   //works right
    if(strlen(data) == 0){return true;}
    for(int i = 0; i < strlen(data); i++) {
        if (!isdigit(data[i])) {
            return true;
        }
    }
    return false;
}

bool is_valid_variable(char* data) {    //works right
    if(strlen(data) == 0){return false;}
    for(int i = 0; i < strlen(data); i++) {
        if(!isalpha(data[i])) {
            return false;
        }
    }
    return true;
}

void divide(struct node* root) {
    if (s_et){return;}
    
    bool isTerminal = true;
    int open_parentheses = 0;
    root->data = remove_whitespaces(root->data);
    char *data = root->data;
    int oridx = -1, andidx = -1, plusidx = -1, minusidx = -1, timesidx = -1;
    int parentheses_begin = -1;   //niye actigimi unuttum -> xor mu(!=0) yoksa duz parantez mi(==0) ayirt etmek icin
    for(int i = 0; i < strlen(data); i++) {
        char curr = data[i];
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
                    if (oridx==-1) {oridx = i;}
                    break;
                case '&':
                    isTerminal = false;
                    if (andidx==-1) {andidx = i;}
                    break;
                case '+':
                    isTerminal = false;
                    if (plusidx==-1){plusidx = i;}
                    break;
                case '-':
                    isTerminal = false;
                    if (minusidx==-1){minusidx = i;}
                    break;
                case '*':
                    isTerminal = false;
                    if (timesidx==-1){timesidx = i;}
                    break;
                default:
                    break;
            }

        }
    }
    if(open_parentheses != 0){
        printf("Error unbalanced parentheses!");
        s_et = true;
        return;
    }
    //TO DO
    /*
        makefile
        null ve 0i karistirip +2 = 2 diyor
    */

    else if(oridx != -1) {
        char* one = (char*) malloc(oridx +1);
        strncpy(one, data, oridx);
        char* two = (char*) malloc(strlen(data) - oridx);
        one[oridx] = 0;
        strcpy(two, &data[oridx+1]);
        root->left = newNode(0, one, NULL);
        root->right = newNode(0,two,NULL);
        root->operation = "|";
    }
    else if(andidx != -1) {
        char* one = (char*) malloc(andidx +1);
        strncpy(one, data, andidx);
        char* two = (char*) malloc(strlen(data) - andidx);
        one[andidx] = 0;
        strcpy(two, &data[andidx+1]);
        root->left = newNode(0, one, NULL);
        root->right = newNode(0,two,NULL);
        root->operation = "&";
    }
    else if(plusidx != -1) {
        char* one = (char*) malloc(plusidx +1);
        char* two = (char*) malloc(strlen(data) - plusidx);
        strncpy(one, data, plusidx);
        one[plusidx] = 0;
        strcpy(two, &data[plusidx+1]);
        root->left = newNode(0, one, NULL);
        root->right = newNode(0,two,NULL);
        root->operation = "+";
    }
    else if(minusidx != -1) {
        char* one = (char*) malloc(minusidx +1);
        char* two = (char*) malloc(strlen(data) - minusidx);
        strncpy(one, data, minusidx);
        one[minusidx] = 0;
        strcpy(two, &data[minusidx+1]);
        root->left = newNode(0, one, NULL);
        root->right = newNode(0,two,NULL);
        root->operation = "-";
    }
    else if(timesidx != -1) {
        char* one = (char*) malloc(timesidx +1);
        char* two = (char*) malloc(strlen(data) - timesidx);
        strncpy(one, data, timesidx);
        one[timesidx] = 0;
        strcpy(two, &data[timesidx+1]);
        root->left = newNode(0, one, NULL);
        root->right = newNode(0,two,NULL);
        root->operation = "*";
    }
    else {
        if(parentheses_begin !=-1) {
            char fnc_name[parentheses_begin+1];
            strncpy(fnc_name, data, parentheses_begin);
            fnc_name[parentheses_begin] = 0;
            char* func= remove_whitespaces(fnc_name);
            int comma;
            if(parentheses_begin==0) {
                root->data = remove_parentheses(root->data);
                divide(root);
            }
            else if(strcmp(func, "xor") == 0) {
                comma = search_char(data+parentheses_begin+1, ',');   //works right, dot it also for else ifs
                char* one = (char*) malloc(comma +1);
                char* two = (char*) malloc(strlen(data)-comma-parentheses_begin-2);
                strncpy(one, data+parentheses_begin+1, comma);
                one[comma] = 0;
                strncpy(two, data+comma+parentheses_begin+2,strlen(data)-comma-parentheses_begin-3);
                two[strlen(data)-comma-parentheses_begin-3]=0;
                root->left = newNode(0, one, NULL);
                root->right = newNode(0,two,NULL);
                root->operation = "xor";
            }
            else if(strcmp(func, "ls") == 0){
                comma = search_char(data+parentheses_begin, ',');
                char* one = (char*) malloc(comma +1);
                char* two = (char*) malloc(strlen(data)-comma-parentheses_begin-2);
                strncpy(one, data+parentheses_begin+1, comma);
                one[comma] = 0;
                strncpy(two, data+comma+parentheses_begin+2,strlen(data)-comma-parentheses_begin-3);
                two[strlen(data)-comma-parentheses_begin-3]=0;
                root->left = newNode(0, one, NULL);
                root->right = newNode(0,two,NULL);
                root->operation = "ls";
            }
            else if(strcmp(func, "rs") == 0){
                comma = search_char(data+parentheses_begin, ',');
                char* one = (char*) malloc(comma +1);
                char* two = (char*) malloc(strlen(data)-comma-parentheses_begin-2);
                strncpy(one, data+parentheses_begin+1, comma);
                one[comma] = 0;
                strncpy(two, data+comma+parentheses_begin+2,strlen(data)-comma-parentheses_begin-3);
                two[strlen(data)-comma-parentheses_begin-3]=0;
                root->left = newNode(0, one, NULL);
                root->right = newNode(0,two,NULL);
                root->operation = "rs";
            }
            else if(strcmp(func, "lr") == 0) {
                comma = search_char(data+parentheses_begin, ',');
                char* one = (char*) malloc(comma +1);
                char* two = (char*) malloc(strlen(data)-comma-parentheses_begin-2);
                strncpy(one, data+parentheses_begin+1, comma);
                one[comma] = 0;
                strncpy(two, data+comma+parentheses_begin+2,strlen(data)-comma-parentheses_begin-3);
                two[strlen(data)-comma-parentheses_begin-3]=0;
                root->left = newNode(0, one, NULL);
                root->right = newNode(0,two,NULL);
                root->operation = "lr";
            }
            else if(strcmp(func, "rr") == 0) {
                comma = search_char(data+parentheses_begin, ',');
                char* one = (char*) malloc(comma +1);
                char* two = (char*) malloc(strlen(data)-comma-parentheses_begin-2);
                strncpy(one, data+parentheses_begin+1, comma);
                one[comma] = 0;
                strncpy(two, data+comma+parentheses_begin+2,strlen(data)-comma-parentheses_begin-3);
                two[strlen(data)-comma-parentheses_begin-3]=0;
                root->left = newNode(0, one, NULL);
                root->right = newNode(0,two,NULL);
                root->operation = "rr";
            }
            else if(strcmp(func, "not") == 0) {
                char* one = (char*)malloc(strlen(data)-parentheses_begin-1);
                strncpy(one, data+parentheses_begin+1,strlen(data)-parentheses_begin-2);
                one[strlen(data)-parentheses_begin-2] = 0;
                root->left = newNode(0,(char *) one,NULL);
                root->operation="not";
            }

            else {
                printf("%s","Error wrong fct name!");   // For wrong function names like zortt
                s_et = true;
                return;
            }
        }

    }

    if (root->left!=NULL) {
        divide(root->left);
        if (root->right!=NULL) {
            divide(root->right);
        }
    }

}

int execute(hash_table* ht, struct node* root) {
    
    if(root->operation==NULL) {
        if(!is_variable(root->data)) {
            root->value= atoi(root->data);                         //buralara dikkat int dönüyolar (int *) yapılmalı mı
        }
        else{
            if(is_valid_variable(root->data)) {
                root->value = get(ht, root->data);                 // buraya da
                //hashten değer getir value ya ver
            }
            else {
                printf("Error invalid var name!");
                s_et = true;
                return 0;
            }
        }
        return root->value;
    }
    else if(root->operation=="+") {
        root->value= plus(execute(ht, root->left), execute(ht, root->right));
    }
    else if(root->operation=="-"){
        root->value=minus(execute(ht, root->left), execute(ht, root->right));
    }
    else if(root->operation=="*"){
        root->value=times(execute(ht, root->left), execute(ht, root->right));
    }
    else if(root->operation=="&"){
        root->value=and(execute(ht, root->left), execute(ht, root->right));
    }
    else if(root->operation=="|"){
        root->value=or(execute(ht, root->left), execute(ht, root->right));
    }
    else if(root->operation=="xor"){
        root->value=xor(execute(ht, root->left), execute(ht, root->right));
    }
    else if(root->operation=="ls"){
        root->value=left_shift(execute(ht, root->left), execute(ht, root->right));
    }
    else if(root->operation=="rs"){
        root->value=right_shift(execute(ht, root->left), execute(ht, root->right));
    }
    else if(root->operation=="lr"){
        root->value=left_rotate(execute(ht, root->left), execute(ht, root->right));
    }
    else if(root->operation=="rr"){
        root->value=right_rotate(execute(ht, root->left), execute(ht, root->right));
    }
    else if(root->operation=="not"){
        root->value=not(execute(ht, root->left));
    }
    return root->value;
}

int main() {
    hash_table ht;
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht.table[i] = NULL;
    }
    char data[256];
    //scanf("%[^\n]c", data)
    while(fgets(data, 256, stdin)) {
        //while(true) {
        struct node* root = newNode(0,NULL,NULL);

        int equals = search_char(data, '=');
        if(equals==strlen(data)){
            //atama yok
            root->data= remove_whitespaces(data);
            if(root->data == "") {
                printf("\n");
                continue;
            }
            if(s_et) {continue;}
            divide(root);
            if(s_et) {continue;}
            int ans = execute(&ht, root);
            printf("%d\n", ans);
        }
        else{
            char* variable = (char*) malloc(equals +1);   //variable name is on the right side
            char* two = (char*) malloc(strlen(data)-equals);        // two is the value to be assigned to the variable
            strncpy(variable, data, equals);
            variable[equals] = 0;
            char* var = remove_whitespaces(variable); //name of variable

            if(!is_valid_variable(var)) {
                printf("%s\n", "Error LHS invalid var name!");
                continue; //just keep taking another input
            }
            strcpy(two, data+equals+1);
            two[strlen(data)-equals-1]=0;
            root->data=two;
            if(s_et) {continue;}
            divide(root);
            if(s_et) {continue;}
            int ans = execute(&ht, root);
            if(ans || (ans == 0)){
                insert(&ht, var, root->value);
                printf("%d\n", ans);
            }
        }
    }


}