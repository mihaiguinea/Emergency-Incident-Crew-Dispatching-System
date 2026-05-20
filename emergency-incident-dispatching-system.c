#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// EMERGENCY RESPONSE CREW
// id : unique identifier
// type : crew type designation (A, B, C)
// availability : 1 = available, 0 = unavailable

typedef struct unit{
    int id;
    char type;
    int avalability;
}unit;

// INCIDENT LOG
// id : unique identifier
// priority : severity prioritization tier (low, medium, high)
// description : textual breakdown of the emergency event
// status : operational pipeline phase (queued, intervened, solved)

typedef struct incident{
    int id;
    char priority[7];
    char *description;
    char status[11];
}incident;

// EMERGENCY DISPATCH INTERVENTION
// incident : reference pointer to the targeted emergency incident
// unit : reference pointer to the assigned response crew
typedef struct intervention{
    struct incident *incident;
    struct unit *unit;
}intervention;


///////////////
///////////////...........INCIDENTS.......////
///////////////..Circular Doubly Linked List with Sentinel Node..///



// INCIDENT LIST NODE
// elem : incident payload wrapped within the node
// next : pointer tracking the subsequent node
// prev : pointer tracking the preceding node

typedef struct incidentNode{
    incident elem;
    struct incidentNode* next;
    struct incidentNode* prev; 
} incidentNode;


// INCIDENT LIST CONTAINER
// Contains a single anchor pointer referencing the sentinel node
typedef struct{
    incidentNode* sentinel;
}incidentList;


// ALLOCATE AND INITIALIZE EMPTY INCIDENT LIST

incidentList* createList_incident(){
    incidentList* list = malloc(sizeof(incidentList));
    list->sentinel = malloc(sizeof(incidentNode));

    list->sentinel->next = list->sentinel;
    list->sentinel->prev = list->sentinel;
    return list;
}

// VALIDATE IF INCIDENT LIST IS EMPTY
// 1 = list is empty
// 0 = otherwise
int is_empty_incident(incidentList *list){
    if(list->sentinel->next == list->sentinel){
        return 1;
    }
    return 0;
}

// COUNT AND RETURN DYNAMIC NODES WITHIN THE LIST
int length_incident(incidentList* list){
    int i = 0;
    incidentNode* iter = list->sentinel->next;
    while(iter != list->sentinel){
        i++;
        iter = iter-> next;
    }
    return i;
}

// INSERT NEW INCIDENT AT TARGET INDEX
// Traverses the container pipeline to dynamically bind the node at specified offset
void insertAt_incident(incidentList* list, incident elem, int poz){
    incidentNode* iter = list->sentinel;
    int i = 0;
    // Traverse the list until reaching target index position
    while(i < poz && iter->next != list->sentinel){
        i++;
        iter = iter->next;
    }

    // Allocate memory and copy values to the new node structure
    incidentNode* newnode = malloc(sizeof(incidentNode));
    newnode->elem.id  = elem.id;
    strcpy(newnode->elem.priority, elem.priority);
    strcpy(newnode->elem.status, elem.status);
    newnode->elem.description = malloc(strlen(elem.description) + 1);
    strcpy(newnode->elem.description, elem.description);

    // Link the new node securely within the doubly linked topology
    newnode->next = iter->next;
    newnode->prev = iter;

    iter->next->prev = newnode;
    iter->next = newnode;
}

// DETACH AND ERASE FIRST NODE MATCHING TARGET INCIDENT ID
// Safely deallocates node structure along with its dynamic description field
void delete_incident(incidentList* list, incident elem){
    incidentNode* iter = list->sentinel->next;
    // Traverse list layout until identifier match is encountered to trigger erasure
    while(iter != list->sentinel){
        if(iter->elem.id == elem.id){
            iter->prev->next = iter->next;
            iter->next->prev = iter->prev;
            free(iter->elem.description);
            free(iter);
            return;
        }
        iter = iter->next;
    }
}


// RECURSIVELY DEALLOCATE ENTIRE INCIDENT LIST FROM MEMORY
// Traverses node by node to free dynamic string descriptions before node removal
// Cleans up remaining sentinel attributes, sentinel node, and container frame
void freeList_incident(incidentList* list){
    incidentNode* iter = list->sentinel->next;
    
    while(iter != list->sentinel){
        incidentNode* tmp = iter;
        iter = iter->next;
        free(tmp->elem.description);
        free(tmp);
    }
    free(list->sentinel->elem.description);
    free(list->sentinel);
    free(list);
}


////////////////////
///////////////////............INTERVENTIONS.........///
///////////////////..Circular Doubly Linked List with Sentinel Node - Interventions..////


// INTERVENTION LIST NODE
// elem : active dispatch pairing reference payload
// next : pointer tracking the subsequent node
// prev : pointer tracking the preceding node
typedef struct interventionNode{
    intervention elem;
    struct interventionNode* next;
    struct interventionNode* prev; 
} interventionNode;

// INTERVENTION LIST CONTAINER
// sentinel : anchor reference linking the baseline node wrapper
typedef struct{
    interventionNode* sentinel;
}interventionList;


// ALLOCATE AND INITIALIZE EMPTY INTERVENTION LIST
// Allocates structural boundary memory block for a single sentinel anchor
interventionList* createList_intervention(){
    interventionList* list = malloc(sizeof(interventionList));
    list->sentinel = malloc(sizeof(interventionNode));

    list->sentinel->next = list->sentinel;
    list->sentinel->prev = list->sentinel;
    return list;
}

// VALIDATE IF INTERVENTION CONTAINER TRAFFIC IS EMPTY
// 1 = list is empty
// 0 = otherwise
int is_empty_intervention(interventionList *list){
    if(list->sentinel->next == list->sentinel){
        return 1;
    }
    return 0;
}


// COMPUTE RUNTIME LENGTH OF INTERVENTION LIST
// Iterates across current links to evaluate active counts
int length_intervention(interventionList* list){
    int i = 0;
    interventionNode* iter = list->sentinel->next;
    while(iter != list->sentinel){
        i++;
        iter = iter-> next;
    }
    return i;
}

// INSERT INTERVENTION NODE AT SPECIFIED OFFSET POSITION
void insertAt_intervention(interventionList* list, intervention elem, int poz){
    interventionNode* iter = list->sentinel;
    int i = 0;
    // Traverse layout until target slot offset criteria is satisfied
    while(i < poz && iter->next != list->sentinel){
        i++;
        iter = iter->next;
    }

    // Populate attributes and link node safely into internal pipeline
    interventionNode* newnode = malloc(sizeof(interventionNode));
    newnode->elem  = elem;

    newnode->next = iter->next;
    newnode->prev = iter;

    iter->next->prev = newnode;
    iter->next = newnode;
}

// REMOVE TARGET INTERVENTION ROW FROM ACTIVE REGISTRY
void delete_intervention(interventionList* list, intervention elem){
    interventionNode* iter = list->sentinel->next;

    // Scan framework pipeline to unbind and deallocate matching element
    while(iter != list->sentinel){
        if(iter->elem.incident->id == elem.incident->id){
            iter->prev->next = iter->next;
            iter->next->prev = iter->prev;
            free(iter);
            return;
        }
        iter = iter->next;
    }
}

// TEARDOWN INTERVENTION CONTAINER TOPOLOGY FROM MEMORY
// Loops sequentially to clean active wrappers before clearing structural anchors
void freeList_intervention(interventionList* list){
    interventionNode* iter = list->sentinel->next;
    
    while(iter != list->sentinel){
        interventionNode* tmp = iter;
        iter = iter->next;
        free(tmp);
    }
    free(list->sentinel);
    free(list);
}


///////////////////
//////////////////..INCIDENT QUEUE (For Priority Management)../////
///////////////////


// PRIORITY QUEUE LINK NODE
// elem : reference pointer routing to the corresponding incident tracking profile
// next : tracker mapping out consecutive queue layout slots
typedef struct QueueNode{
    incident* elem;
    struct QueueNode *next;
}QueueNode;


// OPERATIONAL QUEUE PIPELINE STRUCTURE
// front : dynamic pointer locking head of queue execution
// rear : dynamic pointer tracking tail element insertions
// size : tally record tracking active element count
typedef struct Queue{
    QueueNode *front;
    QueueNode *rear;
    long size;
}Queue;

// ALLOCATE AND INITIALIZE SECURE PRIORITY QUEUE ENDPOINT
// Instantiates heap memory block allocation for empty buffer monitoring
Queue* createQueue(){
    Queue* q =(Queue*) malloc(sizeof(Queue));

    q->front = NULL;
    q->rear = NULL;
    q->size = 0;

    return q;
} 

// CONFIRM IF CURRENT QUEUE CONTAINS NO ENTRIES
// 1 = queue is empty
// 0 = otherwise
int isQueueEmpty(Queue *q){
    if(q->front == NULL){
        return 1;
    }
    return 0;
}


// APPEND INCIDENT POINTER ENTRY TO TAIL OF EXECUTION PIPELINE
void enqueue(Queue *q, incident* elem){
    if(q == NULL){
        return;
    }
    QueueNode* nod = (QueueNode*) malloc(sizeof(QueueNode));
    if(nod == NULL){
        return;
    }
    nod->elem = elem;
    nod->next = NULL;

    if(isQueueEmpty(q) == 1){
        q->front = nod;
        q->rear = nod;
        q->size++;
        return;
    }
    q->rear->next = nod;
    q->rear = nod;
    q->size++;

    return;

}

// DEQUEUE AND CONSUME TRACKED ELEMENT FROM THE HEAD OF PIPELINE
void dequeue(Queue* q){
    if(q == NULL || isQueueEmpty(q) == 1){
        return;
    }

    QueueNode* nod = q->front;

    q->front = q->front->next;
    q->size--;

    if(q->front == NULL){
        q->rear = NULL;
    }

    free(nod);

}


// RELEASE COMPLETE STRUCTURAL FOOTPRINT OF TARGET QUEUE MATRIX
void destroyQueue(Queue *q){
    if(q == NULL){
        return;
    }
    while(isQueueEmpty(q)==0){
        dequeue(q);
    }
    free(q);
}


//////////////
/////////////..CREW QUEUE (For Available Field Resources)../////
//////////////

// FIELD UNIT REGISTRY QUEUE NODE
// elem : reference tracking target emergency responder crew profile
// next : link tracing neighboring field asset node references
typedef struct QueueNode_unit{
    unit* elem;
    struct QueueNode_unit *next;
}QueueNode_unit;

// DISPATCH QUEUE BOUNDARY MATRIX
// front : structural head pointer tracking deployment pool queue line
// rear : structural boundary pointer mapping entry registry point
// size : runtime indicator monitor length state value
typedef struct Queue_unit{
    QueueNode_unit *front;
    QueueNode_unit *rear;
    long size;
}Queue_unit;

// ALLOCATE AN INSTANCE CONFIGURATION OF EMPTY UNIT QUEUE MATRIX
// Allocates layout frame container maps
Queue_unit* createQueue_unit(){
    Queue_unit* q =(Queue_unit*) malloc(sizeof(Queue_unit));

    q->front = NULL;
    q->rear = NULL;
    q->size = 0;

    return q;
} 

// VALIDATE STATUS STATE CAPACITY ON TARGET RESOURCE POOL LIST
// 1 = queue empty
// 0 = otherwise
int isQueueEmpty_unit(Queue_unit *q){
    if(q->front == NULL){
        return 1;
    }
    return 0;
}

// POSITION EMERGENCY UNIT REFERENCE TO THE REAR BOUNDARY TAIL
void enqueue_unit(Queue_unit *q, unit* elem){
    if(q == NULL){
        return;
    }
    QueueNode_unit* nod = (QueueNode_unit*) malloc(sizeof(QueueNode_unit));
    if(nod == NULL){
        return;
    }
    nod->elem = elem;
    nod->next = NULL;

    if(isQueueEmpty_unit(q) == 1){
        q->front = nod;
        q->rear = nod;
        q->size++;
        return;
    }
    q->rear->next = nod;
    q->rear = nod;
    q->size++;

    return;

}

// POP ENTRY WRAPPER TRACKER FROM DISPATCH HEAD FRAMEWORK
void dequeue_unit(Queue_unit* q){
    if(q == NULL || isQueueEmpty_unit(q) == 1){
        return;
    }

    QueueNode_unit* nod = q->front;

    q->front = q->front->next;
    q->size--;

    if(q->front == NULL){
        q->rear = NULL;
    }

    free(nod);

}

// CLEAN ENVIRONMENT ALLOCATIONS ON EMERGENCY TEAM MONITOR LIST
void destroyQueue_unit(Queue_unit *q){
    if(q == NULL){
        return;
    }
    while(isQueueEmpty_unit(q)==0){
        dequeue_unit(q);
    }
    free(q);
}



///////////
//////////...INTERVENTION STACK (For Dispatch History tracking)..////
///////////

// CHRONOLOGICAL STACK MANAGEMENT TRANSACTION NODE
// elem : reference pipeline tracking targeted assignment intervention records
// next : reference locator tracking nested items beneath history frame
typedef struct StackNode{
    intervention* elem;
    struct StackNode *next;
} StackNode;


// OPERATIONAL LIFO BACKTRACK STACK MATRIX
// head : pointer target tracking current stack peak boundary frame
// size : tracked count evaluating available rollback depths
typedef struct Stack{
    StackNode* head;
    long size;
} Stack;

// INSTANTIATE SECURE BACKLOG HISTORY MANAGEMENT FRAMEWORK
Stack* createStack(void){
    Stack* s =(Stack*) malloc(sizeof(Stack));

    if(s == NULL){
        return NULL;
    }

    s->head = NULL;
    s->size = 0;

    return s;
}

// ASSESS DEPTH INDEX STATE OF DISPATCH ROLLBACK REGISTRY
// 1 = stack empty
// 0 = otherwise
int isStackEmpty(Stack* stack){
    if(stack->head == NULL){
        return 1;
    }
    return 0;
}

// PUSH LAST DISPATCH TRANSACTION RECORD ONTO STACK TOP TERMINAL
void push(Stack *stack, intervention* elem){

    if(stack == NULL){
        return ;
    }
    
    StackNode* cop = malloc(sizeof(StackNode));
    if(cop == NULL){
        return;
    }
    cop->elem = elem;
    cop->next = stack->head;

    stack->head = cop;
    stack->size++;

}

// RETRIEVE CURRENT HISTORICAL TOP TRANSITION METRIC FRAME
intervention* top(Stack *stack){    
    if(stack == NULL || isStackEmpty(stack) == 1){
        exit(1);
    }
    return stack->head->elem;
} 

// POP THE LATEST CHRONOLOGICAL BOUNDARY ENTRY NODE
// Frees the intermediate stack container link
void pop(Stack *stack){

    if(stack == NULL || isStackEmpty(stack) == 1){
        return;
    }

    StackNode* out = stack->head;
    stack->head = stack->head->next;
    stack->size--;

    free(out);

}

// DESTROY AND DEALLOCATE HISTORY ROLLBACK BACKUP REPOSITORY
void destroyStack(Stack *stack){
    if(stack == NULL){
        return;
    }
    while(isStackEmpty(stack) == 0){
        free(stack->head->elem);
        pop(stack);
    }
    free(stack);
}


////////
//////
////////

int main(){

    FILE* fin = fopen("tema1.in", "r");
    FILE* fout = fopen("tema1.out", "w");
    Queue_unit* queue_avalabile_units = createQueue_unit();
    Stack* Stiva_interventii = createStack();

    // Read total crew capacity headcount from file stream
    int nr_echipaje;
    fscanf(fin,"%d", &nr_echipaje);

    // Allocate structural base array map of team pointer indices
    unit **v = malloc(nr_echipaje * sizeof(unit*));

    // Parse specific attribute pairs (ID and operational crew type)
    // Initialize base readiness value status to 1 across all profiles
    for(int i = 0; i < nr_echipaje; i++){
        v[i] = malloc(sizeof(unit));
        fscanf(fin, "%d %c", &v[i]->id, &v[i]->type);
        v[i]->avalability = 1;

        enqueue_unit(queue_avalabile_units, v[i]);
    }

    // Configure structural master incident catalog repository with sentinel nodes
    // Inject testing incident parameter defaults into sentinel structure frame
    incidentList* Lista_incidente = createList_incident();
    Lista_incidente->sentinel->elem.id = 0;
    strcpy(Lista_incidente->sentinel->elem.priority, "low");
    Lista_incidente->sentinel->elem.description = malloc(strlen("test incident") + 1);
    strcpy(Lista_incidente->sentinel->elem.description , "test incident");
    strcpy(Lista_incidente->sentinel->elem.status ,"solved");

    // Initialize intervention execution layout registry with dummy sentinel block
    interventionList* Lista_interventii = createList_intervention();
    Lista_interventii->sentinel->elem.incident = NULL;
    Lista_interventii->sentinel->elem.unit = NULL;

    // Build the 3 stratified triage priority channels for pipeline stream sorting
    Queue* queue_high = createQueue();
    Queue* queue_medium = createQueue();
    Queue* queue_low = createQueue();

    // Loop systematically through scheduled query execution batch list
    int nr_operatii;
    fscanf(fin, "%d", &nr_operatii);
    char functie[30];
    for(int op = 0; op < nr_operatii; op++){
        fscanf(fin, "%s", functie);


        //...ADD INCIDENT...//
        // Register incoming incident profile at the tail layout
