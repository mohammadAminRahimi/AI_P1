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
}; 

//global variables
int numberOfRwos, numberOfColor, numberOfCards;


int goalTest(struct State* state);
struct State* initialStateInitialization();
void solution(struct State* state);
int  isExplored(struct State);
struct State* childNode(struct State* state, int i, int j);
struct  Node*  newSNode(int number, char color);
int getNumber(char* s);
char getChar(char* s);
void printState(struct State* state);

//queue related functions declaration
struct State* deQueue(struct Queue* q);
void enQueue(struct Queue* q, struct State* key);
struct Queue* createQueue();
struct QNode* newNode(struct State* k);
int isEmpty(struct Queue* q);
int isInQueue(struct Queue* q, struct State* state);

int  main(){
    scanf("%d",&numberOfCards);
    scanf("%d",&numberOfColor);
    scanf("%d",&numberOfRwos);
    getchar();

    
    
    // initial state of the problem
    struct State* initialState = initialStateInitialization();

    if(goalTest(initialState)){
        solution(initialState);
        printf("WE ARE WINNER\n");
        return 0;
    }
    printf("you are not winner\n");
    return 0;
    struct Queue* frontier  = createQueue();
    enQueue(frontier, initialState);
    struct Queue* explored = createQueue();

    while(!isEmpty(frontier)){
        struct State* state = deQueue(frontier);
        enQueue(explored, state);
        for(int i=0 ; i<numberOfRwos ; i++){
            for(int j=0 ; j<numberOfRwos ; j++){
                if(i==j)
                    continue;
                struct State* childState =  childNode(state, i, j);
                if(childState == NULL)
                    continue;
                if(!isInQueue(explored, childState) && !isInQueue(frontier, childState)){
                    if(goalTest(childState)){
                        solution(childState);
                        return 0;
                    }
                    enQueue(frontier, childState);
                }
            }
        }
    }
}


struct State* childNode(struct State* state, int i, int j){
    struct State* childNode = NULL;
    //

    return state;
}



int goalTest(struct State* state){
    int colorCounter = 0;
    for(int i=0 ; i<numberOfRwos ; i++){
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

struct State* initialStateInitialization(){
    struct State* initialState = (struct State*)malloc(sizeof(struct State));
    initialState->rows = (struct Node**)malloc(numberOfRwos*sizeof(struct Node*));
    for(int i=0 ; i<numberOfRwos ; i++)
        initialState->rows[i] = NULL;
    for(int i=0 ; i<numberOfRwos ; i++){
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
            // printf("%s\n",token);
            int number = getNumber(token);
            char color =  getChar(token);
            struct Node* node = newSNode(number, color);
            node->next = initialState->rows[i];
            initialState->rows[i] = node;
            // printf("%d %c\n", initialState->rows[i]->number, initialState->rows[i]->color);
            token = strtok(NULL, " ");
        }
    }



    
    return initialState;
}

void solution(struct State* state){
    return;
}

void printState(struct State* state){
    struct Node* node;
    for(int i=0 ; i<numberOfRwos ; i++){
        node = state->rows[i];
        for(int j=0 ; node!=NULL ; node=node->next){
            printf("%c %d   ", node->color, node->number);
        }
        printf("\n");
    }
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
struct Queue* createQueue() 
{ 
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = q->rear = NULL; 
    return q; 
} 
  
// The function to add a key k to q 
void enQueue(struct Queue* q, struct State* key) 
{ 
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
    return 0;
}




int getNumber(char* s){
    return s[0]-48;
}

char getChar(char* s){
    return s[1];
}