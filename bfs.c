#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct Card {
    char color;
    int number;
};


struct Node {
    struct Card card;
    struct  Node* next;
};

struct State{
    struct Node* rows;
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
struct State* initialStateInitializatiion();
void solution(struct State* state);
int  isExplored(struct State);
struct State* childNode(struct State* state, int i, int j);

//queue related functions declaration
struct State* deQueue(struct Queue* q);
void enQueue(struct Queue* q, struct State* key);
struct Queue* createQueue();
struct QNode* newNode(struct State* k);
int isEmpty(struct Queue* q);
int isInQueue(struct Queue* q, struct State* state);

int  main(){
    scanf("%d %d %d",&numberOfCards, &numberOfColor, &numberOfRwos);
    
    
    // initial state of the problem
    struct State* initialState = initialStateInitializatiion();

    if(goalTest(initialState)){
        solution(initialState);
        return 0;
    }

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
    return 0;
}

struct State* initialStateInitializatiion(){
    struct State* initialState = ;
    return initialState;
}

void solution(struct State* state){
    return;
}







// A utility function to create a new linked list node. 
struct QNode* newNode(struct State* k) 
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
    struct QNode* temp = newNode(key); 
  
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