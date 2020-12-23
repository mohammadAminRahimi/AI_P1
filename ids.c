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


struct  Node*  newSNode(int number, char color);
int getNumber(char* s);
char getChar(char* s);
struct State* copyState(struct State* state);
struct State* initialStateInitialization();
int goalTest(struct State* state);
int recursiveDLS(struct State* state, int limit);
void printState(struct State* state);
struct State* childStateCreate(struct State* s, int i, int j);
void solution(struct State* state);
void freeState(struct State* state);

//global variables
int numberOfRows, numberOfColor, numberOfCards;
int numberOfCreatedState=0; 





int main(){
    int limit;
    printf("please enter number of cardes, number of colors and number of rows respectively\n");
    scanf("%d",&numberOfCards);
    scanf("%d",&numberOfColor);
    scanf("%d",&numberOfRows);
    printf("please enter number of begining limit\n");
    scanf("%d", &limit);
    getchar();

    struct State* initialState = initialStateInitialization();
    numberOfCreatedState++;
    // *********************** 10 time
    for(int i=limit; i<limit+10 ; i++){
        int result = recursiveDLS(initialState, i);
        if(result==1)
            break;
    }
    freeState(initialState);
    return 0;
}


int recursiveDLS(struct State* state, int limit){
    if(goalTest(state)){
        solution(state);
        return 1;
    }
    if(limit==0){
        return 0;
    }
    for(int i=0 ; i<numberOfRows ; i++){
        for(int j=0 ; j<numberOfRows ; j++){
            if(i==j)
                continue;
            struct State* childState =  childStateCreate(state, i, j);
            if(childState == NULL)
                continue;
            numberOfCreatedState++;
            int result = recursiveDLS(childState, limit-1);
            if(result==1){
                freeState(childState);
                return 1;
            }
            freeState(childState);
        }
    }
    return 0;
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


void solution(struct State* state){
    printf("***********************solution**********************\n");
    while(state->parent != NULL){
        printState(state);
        printf("****************\n");
        state =  state->parent;
    }
    printState(state);
    printf("********\n number of created states:%d", numberOfCreatedState);
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

    return state;
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


struct  Node*  newSNode(int number, char color){
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->number = number;
    node->color = color;
    node->next = NULL;
    return node;
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