#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Node {
    char color;
    int number;
    struct  Node* next;
};

struct State{
    struct Node** rows;
    struct State* parent;
    int h;
    int g;
    int f;
};


// A linked list (LL) node to store a queue entry 
struct QNode { 
    struct State* key; 
    struct QNode* next; 
}; 
  
// The queue, front stores the front node of LL and rear stores the 
// last node of LL 
struct Queue { 
    struct QNode *front, *rear; 
    int number;
    char name;
}; 


//global variables
int numberOfRows, numberOfColor, numberOfCards;
int numberOfCreatedState=0, numberOfExploredState=0;

struct QNode* newQNode(struct State* k);
void upadateQueue(struct Queue* frontier, struct State* childState);   
void freeState(struct State* state);
void heuristic(struct State* state);
int goalTest(struct State* state);
struct State* initialStateInitialization();
void solution(struct State* state);
int  isExplored(struct State);
struct State* childStateCreate(struct State* state, int i, int j);
struct  Node*  newSNode(int number, char color);
int getNumber(char* s);
char getChar(char* s);
void printState(struct State* state);
struct State* copyState(struct State* state);
int compareState(struct State* first, struct State* second);
//queue related functions declaration
struct State* deQueue(struct Queue* q);
void enQueue(struct Queue* q, struct State* key);
void pEnqueue(struct Queue* q, struct State* key);
struct Queue* createQueue(char name);
struct QNode* newNode(struct State* k);
int isEmpty(struct Queue* q);
int isInQueue(struct Queue* q, struct State* state);






int main(){
    scanf("%d",&numberOfCards);
    scanf("%d",&numberOfColor);
    scanf("%d",&numberOfRows);
    getchar();

    
    
    // initial state of the problem
    struct State* initialState = initialStateInitialization();
    numberOfCreatedState++;
    if(goalTest(initialState)){
        solution(initialState);
        return 0;
    }
    struct Queue* frontier  = createQueue('f');
    pEnqueue(frontier, initialState);
    struct Queue* explored = createQueue('p');

    while(!isEmpty(frontier)){
        struct State* state = deQueue(frontier);
        enQueue(explored, state);
        numberOfExploredState++;
        for(int i=0 ; i<numberOfRows ; i++){
            for(int j=0 ; j<numberOfRows ; j++){
                if(i==j)
                    continue;
                struct State* childState =  childStateCreate(state, i, j);
                if(childState == NULL)
                    continue;
                if(!isInQueue(explored, childState)){
                    if(!isInQueue(frontier, childState)){
                        numberOfCreatedState++;
                        if(goalTest(childState)){
                            solution(childState);
                            return 0;
                        }
                        pEnqueue(frontier, childState);
                    }else{
                        upadateQueue(frontier, childState); 
                    }
                }
                else{
                    freeState(childState);
                }
            }
        }
    }
}


struct State* initialStateInitialization(){
    struct State* initialState = (struct State*)malloc(sizeof(struct State));
    initialState->rows = (struct Node**)malloc(numberOfRows*sizeof(struct Node*));
    initialState->parent = NULL;
    for(int i=0 ; i<numberOfRows ; i++)
        initialState->rows[i] = NULL;
    for(int i=0 ; i<numberOfRows ; i++){
        char string[40];
        for(int j=0 ; j<40 ; j++){
            string[j] = getchar();
            if(string[j] == '\n'){
                string[j] = '\0';
                break;
            }
        }
        if(strlen(string)==1)
            continue;
        char* token = strtok(string, " ");
        while(token){
            int number = getNumber(token);
            char color =  getChar(token);
            struct Node* node = newSNode(number, color);
            node->next = initialState->rows[i];
            initialState->rows[i] = node;
            token = strtok(NULL, " ");
        }
    }
    initialState->g = 0;
    heuristic(initialState);
    initialState->f = initialState->h + initialState->g;
    return initialState;
}


void pEnqueue(struct Queue* q, struct State* key1){
        q->number +=1;

    struct QNode *key = newQNode(key1); 
  
    // If queue is empty, then new node is front and rear both 
    if (q->rear == NULL) { 
        q->front = q->rear = key; 
        return; 
    } 
    if(q->front->key->f > key->key->f){
        key->next = q->front;
        q->front = key;
        return;
    }
    if(q->front->next==NULL ){
        q->rear->next = key;
        q->rear = key;
        return;
    }
    struct QNode* temp = q->front;
    while(temp->next!=NULL){
        if(temp->next->key->f >= key->key->f){
            if(temp->next->key->f == key->key->f){
                if(temp->next->key->h < key->key->h){
                    temp = temp->next;
                    continue;
                }
            }
            key->next = temp->next;
            temp->next = key;
            return;
        }
        temp = temp->next;
    }
    temp->next = key;
    q->rear = key;
}


void upadateQueue(struct Queue* frontier, struct State* childState){
    struct QNode* temp = frontier->front;
    if(compareState(temp->key, childState)){
        if(childState->f < temp->key->f){
            temp->key->f = childState->f;
            temp->key->g = childState->g;
            temp->key->parent = childState->parent;
        }
        freeState(childState);
        return;
    }
    while(temp->next!=NULL){
        if(compareState(temp->next->key, childState)){
            if(childState->f < temp->next->key->f){
                if(temp->next==frontier->rear){
                    frontier->rear = temp;
                    freeState(temp->next->key);
                    free(temp->next);
                    frontier->rear->next = NULL;
                    return;
                }
                struct QNode* temp2 = temp->next;
                temp->next = temp->next->next;
                freeState(temp2->key);
                free(temp2);
                pEnqueue(frontier, childState);
                return;
            }
            freeState(childState);
            return;
        }
        temp = temp->next;
    }
}


void heuristic(struct State* state){
    int h=0;
    for(int i=0 ; i<numberOfRows ; i++){
        struct Node* node = state->rows[i];
        if(node == NULL){
            continue;
        }
        char color = node->color;
        int expectedNumber = node->number;
        int store = 0;
        while(node!=NULL){
            if(node->number!=expectedNumber || node->color!=color){
                expectedNumber = node->number + 1;
                color = node->color;
                h += store;
                store = 1;
            }else{
                store++;
                expectedNumber++;
            }
            if(node->next==NULL){
                if(expectedNumber-1!=numberOfCards){
                    h += store;
                }
            }
            node = node->next;
        }
    }
    state->h = h;
}


struct State* childStateCreate(struct State* s, int i, int j){
    struct State* state = copyState(s);
    state->parent = s;
    struct Node* iNode = state->rows[i];
    if(iNode==NULL)
        return NULL;
    if(state->rows[j]!=NULL){
        if(iNode->number >= state->rows[j]->number)
            return NULL;
    }
    state->rows[i] = iNode->next;
    iNode->next = state->rows[j];
    state->rows[j] = iNode;
    state->g = s->g + 1;
    heuristic(state);
    state->f = state->g + state->h;
    return state;
}


int goalTest(struct State* state){
    int colorCounter = 0;
    for(int i=0 ; i<numberOfRows ; i++){
        struct Node* node = state->rows[i];
        if(node==NULL)
            continue;
            
        //chech to see if the row is empty
        char color = node->color;
        int number = node->number;
        
        if(number!=1)
            return 0;

        node = node->next;
        for(int j=1 ; j<numberOfCards ; j++){
            if(node==NULL){
                return 0;
            }
            if(node->color!=color || node->number<=number){
                return 0;
            }
            number = node->number;
            node = node->next;
        }
        colorCounter++;
    }
    if(colorCounter==numberOfColor)
        return 1;
    return 0;
}


void solution(struct State* state){
    printf("***********************solution**********************\n");
    while(state->parent != NULL){
        printState(state);
        printf("****************\n");
        state =  state->parent;
    }
    printState(state);
    printf("******\nnumber of created states:%d, number of explored states:%d\n", numberOfCreatedState, numberOfExploredState);
}


void printState(struct State* state){
    struct Node* node;
    for(int i=0 ; i<numberOfRows ; i++){
        node = state->rows[i];
        if(node==NULL){
            printf("#\n");
            continue;
        }
        for(int j=0 ; node!=NULL ; node=node->next){
            printf("%c%d   ", node->color, node->number);
        }
        printf("\n");
    }
    printf("g%d h%d f%d\n", state->g, state->h, state->f);
}


struct  Node*  newSNode(int number, char color){
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->number = number;
    node->color = color;
    node->next = NULL;
    return node;
}




// A utility function to create a new linked list node. 
struct QNode* newQNode(struct State* k) 
{ 
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode)); 
    temp->key = k; 
    temp->next = NULL; 
    return temp; 
} 
  

// A utility function to create an empty queue 
struct Queue* createQueue(char c) 
{ 
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = q->rear = NULL; 
    q->number=0;
    q->name = c;
    return q; 
} 


// The function to add a key k to q 
void enQueue(struct Queue* q, struct State* key) 
{ 

    q->number +=1;
    // Create a new LL node 
    struct QNode* temp = newQNode(key); 
  
    // If queue is empty, then new node is front and rear both 
    if (q->rear == NULL) { 
        q->front = q->rear = temp; 
        return; 
    } 
  
    // Add the new node at the end of queue and change rear 
    q->rear->next = temp; 
    q->rear = temp; 
} 
  

// Function to remove a key from given queue q 
struct State* deQueue(struct Queue* q) 
{ 
    q->number -=1;
    // If queue is empty, return NULL. 
    if (q->front == NULL) 
        return NULL; 
  
    // Store previous front and move front one node ahead 
    struct QNode* temp = q->front; 
  
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) 
        q->rear = NULL; 
    struct State* state = temp->key;
    free(temp); 
    return state;
} 


int isEmpty(struct Queue* q){
    if(q->rear == NULL){
        return 1;
    }
    return 0;
}


int isInQueue(struct Queue* q, struct State* state){
    struct QNode* qNode = q->front;
    int counter=0;
    while(qNode!=NULL){
        qNode = qNode->next;
    }
    counter=0;
    qNode = q->front;
    while(qNode!=NULL){
        counter++;
        if(compareState(state, qNode->key)){
            return 1;
        }
        qNode = qNode->next;
    }
    return  0;
}


int compareState(struct State* first, struct State* second){
    if(first==NULL || second==NULL){
    }
    for(int i=0 ; i<numberOfRows ; i++){
        if(first->rows[i]==NULL || second->rows[i]==NULL){
            if(first->rows[i]==NULL && second->rows[i]==NULL)
                continue;
            return 0;
        }
        struct Node* fNode = first->rows[i];
        struct Node* sNode = second->rows[i];

        while(fNode!=NULL && sNode!=NULL){

            if(fNode->color!=sNode->color || fNode->number!=sNode->number)
                return 0;
            fNode = fNode->next;
            sNode = sNode->next;
        }
        if(sNode!=NULL  || fNode!=NULL)
            return 0;
    }
    return 1;
}


int getNumber(char* s){
    return s[0]-48;
}


char getChar(char* s){
    return s[1];
}


struct State* copyState(struct State* state){
    struct State* newState = (struct State*)malloc(sizeof(struct State));
    newState->rows = (struct Node**)malloc(numberOfRows*sizeof(struct Node*));
    for(int i=0 ; i<numberOfRows ; i++)
        newState->rows[i] = NULL;
    for(int i=0 ; i<numberOfRows ; i++){
        struct Node* node = state->rows[i];
        if(node==NULL)
            continue;

        struct Node* temp = newSNode(node->number, node->color);
        newState->rows[i] = temp;
        node = node->next;
        while(node!=NULL){
            temp->next = newSNode(node->number, node->color);
            temp = temp->next;
            node = node->next;
        }
    }
    return newState;
}


void freeState(struct State* state){
    struct Node* node,*temp;
    for(int i=0 ; i<numberOfRows ; i++){
        node = state->rows[i];
        while(node !=NULL){
            temp = node->next;
            free(node);
            node = temp;
        }
    }
    free(state);
}
