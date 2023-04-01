#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define INT_BITS 64
#define TABLE_SIZE 128
bool s_et = false;


struct node { //every node of the binary tree must hold some info about the expression so far
    long long int value;
    char *data;
    char *operation;
    struct node* left;
    struct node* right;
};

struct node* newNode(long long int value, char *data, char *operation) { //constructor for node
    struct node* node = (struct node*)malloc(sizeof(struct node)); // must allocate the node in memory

    node->value = value;
    node->data = data;
    node->operation = operation;

    node->left = NULL;
    node->right = NULL;
    return (node);
}

typedef struct var {//each variable is held in the hash table with a variable name key and value
    char* key;
    long long int value;
    struct var* next; //hash table uses separate chaining
} var;

typedef struct hash_table {
    var* table[TABLE_SIZE]; //initialize table of length 128
} hash_table;

int poww(int a){ //returns 31 to the power of a
    int val = 1;
    for(int i = 0; i < a; i++){
        val = (val * 31) % TABLE_SIZE;  //modulo table size at each step
    }
    return val;
}

int hash(char* key) { //hash function
    int val = 0;
    for(int i = 0; i<strlen(key); i++){
        int c = key[i];
        val = (int)(val +(int)(c)*poww(i)) % TABLE_SIZE;
    }
    return val;
}

var* create_var(char* key, long long int value) {  //contructor for variables
    var* new_var = (var*)malloc(sizeof(var));
    new_var->key = key;
    new_var->value = value;
    new_var->next = NULL;
    return new_var;
}

void insert(hash_table* ht, char* key, long long int value) { // insert a variable into the hash table
    int index = hash(key);
    var* curr = ht->table[index];
    if (curr == NULL) { //if variable is not defined yet
        ht->table[index] = create_var(key, value); // create a variable with the given key and value
    }
    else {  // if the variable is already in use
        while (curr->next != NULL) {  //finds where the key is in the linked list
            if (strcmp(curr->key, key) == 0) {
                curr->value = value;
                return;
            }
            curr = curr->next;
        }
        if (strcmp(curr->key, key) == 0) {curr->value = value;}//adds the key value pair if there is no match
        else {curr->next = create_var(key, value);}//updates the value of the variable if it is found
    }
}

long long int get(hash_table* ht, char* key) { //get the value of the variable from hash table
    int index = hash(key);
    var* curr = ht->table[index];
    while (curr != NULL) { //iterate through the linked list to find the key
        if (strcmp(curr->key, key) == 0) {
            return curr->value;
        }
        curr = curr->next;
    }
    return 0;
}
//the following 11 functions are defined in an usual manner as in mathematics
long long int plus(long long int ap, long long int bp) {
    long long int ans = ap + bp;      // adres verme şeyini kaldırınca düzeldi &
    return ans;
}

long long int times(long long int ap, long long int bp) {
    long long int ans = ap * bp;
    return ans;
}

long long int minus(long long int ap, long long int bp) {
    long long int ans = ap -  bp;
    return ans;
}

long long int and(long long int ap, long long int bp) {
    long long int ans =  ap &  bp;
    return ans;
}

long long int or(long long int ap, long long int bp) {
    long long int ans =  ap |  bp;
    return ans;
}

long long int xor(long long int ap, long long int bp) {
    long long int ans =  ap ^  bp;
    return ans;
}

long long int left_shift(long long int ap, long long int ip) {
    long long int ans =  ap <<  ip;
    return ans;
}

long long int right_shift(long long int ap, long long int ip) {
    long long int ans =  ap >>  ip;
    return ans;
}

long long int right_rotate(long long int ap, long long int ip) {
    long long int ans = or(( ap >>  ip), (  ap << (INT_BITS -  ip)));
    return ans;
}

long long int left_rotate(long long int ap,long long int ip) {
    long long int ans = or(( ap <<  ip),(  ap >> (INT_BITS -  ip)));
    return ans;
}

long long int not(long long int ap) {
    long long int ans = ~(ap);
    return ans;
}

int search_char(char* str, char to_find) {    //searches for char to_find in str. Ignores occurances in parentheses.
    int open_parentheses = 0;
    for(int i = 0; i<strlen(str); i++) {
        if(str[i] == '(') {open_parentheses++;}
        else if(str[i] == ')') {open_parentheses--;}
        else if(open_parentheses==0) {if(str[i]==to_find) {return i;}}
    }
}

char* remove_whitespaces(char* s) {  // works right
    int size;
    int end = strlen(s)-1;
    int begin=0;
    size = strlen(s);

    if (!size) {return s;}

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

char* remove_parentheses(const char* data) {         //removes the enclosing parentheses of data.
    char* output = (char*)malloc(strlen(data) - 2 + 1);
    strncpy(output, data + 1, strlen(data) - 2);
    output[strlen(data) - 2] = '\0';
    return output;
}

char* comments(char* data){ //returns the string before the comma in data.
    int loc = search_char(data,'%');
    char* output = (char*)malloc(loc+1);
    strncpy(output, data, loc);
    output[loc] = '\0';
    return output;
}

bool is_variable(char* data) {   //checks if data consists only of digits, i.e. if data is a number
    if(strlen(data) == 0){return true;}
    for(int i = 0; i < strlen(data); i++) {
        if (!isdigit(data[i])) {return true;}
    }
    return false;
}

bool is_valid_variable(char* data) {     //checks if data consists only of letters, i.e. if data is a variable name
    if(strlen(data) == 0){return false;}
    for(int i = 0; i < strlen(data); i++) {
        if(!isalpha(data[i])) {return false;}
    }
    return true;
}

void divide(struct node* root) { //forms the parse tree
    if (s_et){return;}//if the input is erroneuos, don't divide any further.
    bool isTerminal = true; //will be used to determine when to ignore the operators
    int open_parentheses = 0; // first we strip data of any leading whitespaces
    root->data = remove_whitespaces(root->data);
    char *data = root->data;
    int oridx = -1, andidx = -1, plusidx = -1, minusidx = -1, timesidx = -1;//will be used to determine if any operators occur
    int parentheses_begin = -1;    //will be used to check if data is enclosed by parentheses or a function like xor
    for(int i = 0; i < strlen(data); i++) {
        char curr = data[i];
        if(curr == '(') {
            isTerminal=false;
            open_parentheses++;
            if(parentheses_begin == -1) {parentheses_begin = i;}
        }
        else if(curr == ')') {open_parentheses--;} //ignore any characters enclosed by parentheses
        else if(open_parentheses==0) {
            switch(curr) { //checks if there are any operations to do
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
    if(open_parentheses != 0){ // in the case of unbalanced parentheses, display an error message.
        printf("%s\n","Error!");
        s_et = true; //no need to look any further in erroneous input
        return;
    }

    //keeping operator precedence in mind, check what to do with the input data. Only "or" case will be explained, the other operations are similar.

    else if(oridx != -1) {
        //split data in two: one corresponds to the part after |, two corresponds to the part after.
        char* one = (char*) malloc(oridx +1);
        strncpy(one, data, oridx);
        char* two = (char*) malloc(strlen(data) - oridx);
        one[oridx] = 0;
        strcpy(two, &data[oridx+1]);
        root->left = newNode(0, one, NULL);//initialize left child, set its data equal to one
        root->right = newNode(0,two,NULL);
        root->operation = "|"; //set the current nodes operation to or
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
    else {  //if there are no basic operations, we check for functions
        if(parentheses_begin !=-1) { //if there are parentheses anywhere in data
            char fnc_name[parentheses_begin+1];
            strncpy(fnc_name, data, parentheses_begin);  //holds the string just before the parentheses
            fnc_name[parentheses_begin] = 0;
            char* func= remove_whitespaces(fnc_name);
            int comma;
            if(parentheses_begin==0) { //if the parentheses is at the beginning, then data is enclosed by parentheses, remove and continue
                root->data = remove_parentheses(root->data);
                divide(root);
            }
            //if the string before parentheses is nonempty then there is a function. The process is similar for all functions, only "xor" will be explained.
            else if(strcmp(func, "xor") == 0) {
                comma = search_char(data+parentheses_begin+1, ','); //locate where the comma to separate two inputs of the function is.
                char* one = (char*) malloc(comma +1);
                char* two = (char*) malloc(strlen(data)-comma-parentheses_begin-2);//one corresponds to the first input, two corresponds to the second.
                strncpy(one, data+parentheses_begin+1, comma);
                one[comma] = 0;
                strncpy(two, data+comma+parentheses_begin+2,strlen(data)-comma-parentheses_begin-3);
                two[strlen(data)-comma-parentheses_begin-3]=0;
                root->left = newNode(0, one, NULL); //initialize left child, set its data equal to one
                root->right = newNode(0,two,NULL);
                root->operation = "xor"; //set the current nodes operation to xor
            }
            else if(strcmp(func, "ls") == 0){
                comma = search_char(data+parentheses_begin+1, ',');
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
                comma = search_char(data+parentheses_begin+1, ',');
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
                comma = search_char(data+parentheses_begin+1, ',');
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
                comma = search_char(data+parentheses_begin+1, ',');
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
                printf("%s\n","Error!");  //if the function is not any of the above, then the function name is erroneous
                s_et = true;
                return;
            }
        }

    }
//if there are any more node to divide, recursively call divide.
    if (root->left!=NULL) {
        divide(root->left);
        if (root->right!=NULL) {divide(root->right);}
    }

}

long long int execute(hash_table* ht, struct node* root) {// uses the binary tree structure to find the value of the expression provided.
    if(s_et){return 0;}//no need to continue if the input is erroneous.
    if(root->operation==NULL) {//if data is terminal
        if(!is_variable(root->data)) {//if data has a numerical value, assign it to the value of the node
            char *ptr;
            root->value= strtoll(root->data,&ptr,10);
        }
        else{
            if(is_valid_variable(root->data)) {root->value = get(ht, root->data);}//if data is a valid variable name, get its value from the hash table
            else {
                printf("%s\n","Error!");
                s_et = true;//no need to continue if the input is erroneous
                return 0;
            }
        }
        return root->value;
    }
    //if there are any operations to do with the node, get the function inputs from left and right children and call the correct function
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
    hash_table ht;//initialize an empty hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht.table[i] = NULL;
    }
    char datap[256];
    printf("%s", "> ");
    while(fgets(datap, 256, stdin)) {//continue the program as long as any input is provided
        s_et = false;
        char *data = comments(datap);//ignore any text after %
        struct node* root = newNode(0,NULL,NULL);//initialize the root of the parse tree

        int equals = search_char(data, '=');
        if(equals==strlen(data)){//if input is not an assignment
            root->data= remove_whitespaces(data);
            if(root->data == "") { //if empty input is provided
                printf("%s", "> ");
                continue;
            }
            divide(root);//forms the parse tree
            long long int ans = execute(&ht, root);//calculates the value of the expression provided
            if(!s_et){printf("%lld\n", ans);} //if the input is not erroneous, print it.
        }
        else{// inout is an assignment
            char* variable = (char*) malloc(equals +1);   //variable name is on the right side
            char* two = (char*) malloc(strlen(data)-equals);        // two is the value to be assigned to the variable
            strncpy(variable, data, equals);
            variable[equals] = 0;
            char* var = remove_whitespaces(variable); //name of variable

            if(!is_valid_variable(var)) {
                printf("%s\n", "Error!");
                s_et = true;
                printf("%s", "> ");
                continue; //just keep taking another input
            }
            strcpy(two, data+equals+1);
            two[strlen(data)-equals-1]=0;
            root->data=two;
            divide(root); //forms the parse tree
            if(s_et) {
                printf("%s", "> ");
                continue;
            }
            long long int ans = execute(&ht, root);
            if(!s_et){ //if the input is erroneous, continues the program without printing an answer
                insert(&ht, var, root->value); //updates the hash table
            }
        }
        printf("%s", "> ");
    }


}
