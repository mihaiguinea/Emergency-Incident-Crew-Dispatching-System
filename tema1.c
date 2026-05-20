/* GUINEA Mihai - 315CD */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//  ECHIPAJ DE INTERVENTIE
//  id : identificator
// type : tipul echipajului(A, B, C)
// avalability : 1 = disponibil, 0 = indisponibil

typedef struct unit{
    int id;
    char type;
    int avalability;
}unit;

// INCIDENT
// id : identificator
// prioprity : grad de prioritate(low, medium, high)
// description : descrierea incidentului
// status : status in privinta rezolvarii(queued, intervened, solved)

typedef struct incident{
    int id;
    char priority[7];
    char *description;
    char status[11];
}incident;

// INTERVENTIE
// incident : pointer la incidentul asociat
// unit : pointer la unitatea asociata
typedef struct intervention{
    struct incident *incident;
    struct unit *unit;
}intervention;


///////////////
///////////////...........INCIDENTE.......////
///////////////..Lista dublu inlantuita circulara cu santinela..///



//  NOD LISTA DE INCIDENTE
//  elem = incidentul pus intr-un nod
//  next = pointer la urmatorul nod
//  prev = pointer la nodul precedent

typedef struct incidentNode{
	incident elem;
	struct incidentNode* next;
	struct incidentNode* prev; 
} incidentNode;


// LISTA DE INCIDENTE
// contine un singur pointer catre santinela
typedef struct{
    incidentNode* sentinel;
}incidentList;


// CREAREA UNEI LISTE GOALE DE INCIDENTE

incidentList* createList_incident(){
    incidentList* list = malloc(sizeof(incidentList));
    list->sentinel = malloc(sizeof(incidentNode));

    list->sentinel->next = list->sentinel;
    list->sentinel->prev = list->sentinel;
    return list;
}

// VERIFICARE DACA LISTA ESTE GOALA
// 1 = lista goala
// 0 = altfel
int is_empty_incident(incidentList *list){
    if(list->sentinel->next == list->sentinel){
        return 1;
    }
    return 0;
}

// RETURNEAZA NUMARUL DE NODURI DIN LISTA
int length_incident(incidentList* list){
    int i = 0;
    incidentNode* iter = list->sentinel->next;
    while(iter != list->sentinel){
        i++;
        iter = iter-> next;
    }
    return i;
}

// INSEREAZA UN INCIDENT LA O POZITIE DATA
// Parcurge toata lista si insereaza noul nod acolo
void insertAt_incident(incidentList* list, incident elem, int poz){
    incidentNode* iter = list->sentinel;
    int i = 0;
    // parcurge lista pana la pozitia poz
    while(i < poz && iter->next != list->sentinel){
        i++;
        iter = iter->next;
    }

    // aloca si da valori pentru noul nod
    incidentNode* newnode = malloc(sizeof(incidentNode));
    newnode->elem.id  = elem.id;
    strcpy(newnode->elem.priority, elem.priority);
    strcpy(newnode->elem.status, elem.status);
    newnode->elem.description = malloc(strlen(elem.description) + 1);
    strcpy(newnode->elem.description, elem.description);

    // leaga noul nod in lista
    newnode->next = iter->next;
    newnode->prev = iter;

    iter->next->prev = newnode;
    iter->next = newnode;
}

// STERGE PRIMUL NOD DIN LISTA CARE ARE UN ANUMIT ID DAT
// Elibereaza nodul, dar si campul description
void delete_incident(incidentList* list, incident elem){
    incidentNode* iter = list->sentinel->next;
    //parcurge lista pana la inatlnirea nodului si il sterge
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


//  ELIBEREAZA TOATA MEMORIA LISTEI DE INCIDENTE
//  Parcurge lista nod cu nod si elibereaza mai intai description si apoi intreg nodul
//  La final se elibereaza description din sentinela, santinela si structura listei
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
///////////////////............INTERVENTII.........///
///////////////////..Lista dublu inlantuita circulara cu santinela - Interventii..////


//  NOD DIN LISTA CU INTERVENTII
//  elem = nod cu interventia
//  next = pointer la nodul urmator
//  prev = pointer la nodul precedent
typedef struct interventionNode{
	intervention elem;
	struct interventionNode* next;
	struct interventionNode* prev; 
} interventionNode;

//  LISTA DE INTERVENTII
//  sentinel = pointer catre santinela
typedef struct{
    interventionNode* sentinel;
}interventionList;


//  CREEAZA O LISTA GOALA DE INTERVENTII
//  Se aloca memorie pentru un singur nod, santinela
interventionList* createList_intervention(){
    interventionList* list = malloc(sizeof(interventionList));
    list->sentinel = malloc(sizeof(interventionNode));

    list->sentinel->next = list->sentinel;
    list->sentinel->prev = list->sentinel;
    return list;
}

//  SE VERIFICA DACA LISTA DE INTERVENTII ESTE GOALA
//  1 = lista goala
// 0 = altfel
int is_empty_intervention(interventionList *list){
    if(list->sentinel->next == list->sentinel){
        return 1;
    }
    return 0;
}


//  RETURNEAZA LUNGIMEA LISTEI DE INTERVENTII
//  Parcurge lista si returneaza lungimea
int length_intervention(interventionList* list){
    int i = 0;
    interventionNode* iter = list->sentinel->next;
    while(iter != list->sentinel){
        i++;
        iter = iter-> next;
    }
    return i;
}

//  INSEREAZA UN NOD LA O POZITIE DATA
void insertAt_intervention(interventionList* list, intervention elem, int poz){
    interventionNode* iter = list->sentinel;
    int i = 0;
    // Parcurge lista pana la intalnirea nodului dorit
    while(i < poz && iter->next != list->sentinel){
        i++;
        iter = iter->next;
    }

    //  Da valori noului nod si il introduce in lista 
    interventionNode* newnode = malloc(sizeof(interventionNode));
    newnode->elem  = elem;

    newnode->next = iter->next;
    newnode->prev = iter;

    iter->next->prev = newnode;
    iter->next = newnode;
}

// STERGE PRIMA INTEREVNTIE DIN LISTA
void delete_intervention(interventionList* list, intervention elem){
    interventionNode* iter = list->sentinel->next;

    //Parcurge lista si sterge nodul
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

//  ELIBEREAZA MEMORIA LISTEI DE INTERVENTII
//  Parcurge lista nod cu nod si elibereaza description si apoi structura nodului
//  Elibereaza apoi santinela si structura listei
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
//////////////////..COADA DE INCIDENTE(pentru cozile de prioritate)../////
///////////////////


//  NOD DIN COADA DE INCIDENTE
//  elem  = pointer catre incidentul din lista de incidente
//  next = pointer catre urmatorul nod din coada
typedef struct QueueNode{
	incident* elem;
	struct QueueNode *next;
}QueueNode;


//  STRUCTURA COZII DE INCIDENTE
//  front = pointer la primul element
//  rear = pointer la ultimul element
//  size = numarul de elemente
typedef struct Queue{
	QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

//  CREEAZA O COADA DE INCIDENTE GOALA
//  Aloca si initializeaza coada
Queue* createQueue(){
	Queue* q =(Queue*) malloc(sizeof(Queue));

	q->front = NULL;
	q->rear = NULL;
	q->size = 0;

	return q;
} 

//  VERIFICA DACA COADA ESTE GOALA
//  1 = coada goala
//  0 = altfel
int isQueueEmpty(Queue *q){
	if(q->front == NULL){
		return 1;
	}
	return 0;
}


//  ADAUGA UN INCIDENT LA FINALUL COZII
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

//  EXTRAGE PRIMUL ELEMENT DIN COADA
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


//  ELIBEREAZA TOATA MEMORIA COZII
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
/////////////..COADA DE ECHIPAJE(pentru coada de disponibilitate)../////
//////////////

//  NOD DIN COADA DE ECHIPAJE
//  elem = pointer catre echipaj
//  next = pointer catre urmatorul element din coada
typedef struct QueueNode_unit{
	unit* elem;
	struct QueueNode_unit *next;
}QueueNode_unit;

//  STRUCTURA COZII
//  front = pointer la primul element
//  rear = pointer la ulrimul element
//  size = lungimea cozii
typedef struct Queue_unit{
	QueueNode_unit *front;
	QueueNode_unit *rear;
	long size;
}Queue_unit;

//  CREEAZA O COADA DE ECHIPAJE GOALA
//  Aloca si initializeaza coada
Queue_unit* createQueue_unit(){
	Queue_unit* q =(Queue_unit*) malloc(sizeof(Queue_unit));

	q->front = NULL;
	q->rear = NULL;
	q->size = 0;

	return q;
} 

//  VERIFICA DACA COADA ESTE GOALA
//  1 = coada goala
//  0 = altfel
int isQueueEmpty_unit(Queue_unit *q){
	if(q->front == NULL){
		return 1;
	}
	return 0;
}

//  ADAUGA UN INCIDENT LA FINALUL COZII DE ECHIPAJE
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

//  STERGE UN ELEMENT DE LA INCEPUTUL COZII DE ECHIPAJE
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

//  ELIBEREAZA MEMORIA COZII
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
//////////...STIVA DE INTERVENTII(pentru istoric)..////
///////////

//  NOD DIN STIVA
//  elem = pointer la interventie
//  next = pointer la urmatorul element din stiva
typedef struct StackNode{
	intervention* elem;
	struct StackNode *next;
} StackNode;


//  STRUCTURA STIVEI
//  head = varful stivei
//  size = numarul de elemente
typedef struct Stack{
	StackNode* head;
	long size;
} Stack;

//  ALOCA SI INITIALIZEAZA O STIVA GOALA
Stack* createStack(void){
	Stack* s =(Stack*) malloc(sizeof(Stack));

	if(s == NULL){
		return NULL;
	}

	s->head = NULL;
	s->size = 0;

	return s;
}

//  VERIFICA DACA STIVA ESTE GOALA
//  1 = stiva goala
//  0 = altfel
int isStackEmpty(Stack* stack){
	if(stack->head == NULL){
		return 1;
	}
	return 0;
}

//  ADAUGA O INTERVENTIE IN VARFUL STIVEI
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

//  RETURNEAZA INTERVENTIA DIN VARFUL STIVEI
intervention* top(Stack *stack){	
	if(stack == NULL || isStackEmpty(stack) == 1){
		exit(1);
	}
	return stack->head->elem;
} 

//  EXTRAGE VARFUL STIVEI 
//  Elibereaza nodul
void pop(Stack *stack){

	if(stack == NULL || isStackEmpty(stack) == 1){
		return;
	}

	StackNode* out = stack->head;
	stack->head = stack->head->next;
	stack->size--;

	free(out);

}

//  ELIBEREAZA MEMORIA INTERGII STIVE
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

    // citeste numarul de echipaje
    int nr_echipaje;
    fscanf(fin,"%d", &nr_echipaje);

    //  aloca vectorul de pointeri la echipaje
    unit **v = malloc(nr_echipaje * sizeof(unit*));

    //  citeste fiecare echipaj(id si type)
    //  intial toate echipajele au avalability = 1
    for(int i = 0; i < nr_echipaje; i++){
        v[i] = malloc(sizeof(unit));
        fscanf(fin, "%d %c", &v[i]->id, &v[i]->type);
        v[i]->avalability = 1;

        enqueue_unit(queue_avalabile_units, v[i]);
    }

    //  initializeaza lista de incidente cu santinela
    //  da santinelei valorile de incident test
    incidentList* Lista_incidente = createList_incident();
    Lista_incidente->sentinel->elem.id = 0;
    strcpy(Lista_incidente->sentinel->elem.priority, "low");
    Lista_incidente->sentinel->elem.description = malloc(strlen("test incident") + 1);
    strcpy(Lista_incidente->sentinel->elem.description , "test incident");
    strcpy(Lista_incidente->sentinel->elem.status ,"solved");

    //  initializeaza lista de interventii cu santinela goala
    interventionList* Lista_interventii = createList_intervention();
    Lista_interventii->sentinel->elem.incident = NULL;
    Lista_interventii->sentinel->elem.unit = NULL;

    //  creeaza cele 3 cozi de prioritate pentru incidente
    Queue* queue_high = createQueue();
    Queue* queue_medium = createQueue();
    Queue* queue_low = createQueue();

    //  citeste si proceseaza operatiile
    int nr_operatii;
    fscanf(fin, "%d", &nr_operatii);
    char functie[30];
    for(int op = 0; op < nr_operatii; op++){
        fscanf(fin, "%s", functie);


        //...ADD INCIDENT...//
        //  Adauga un incident nou la finalul listei de incidente
        //  Adauga incidentul in coada de prioritate corespunzatoare
        if(strcmp(functie, "ADD_INCIDENT") == 0){
            incident *elem = malloc(sizeof(incident));
            elem->description = malloc(400 * sizeof(char));
            fscanf(fin,"%d", &elem->id);
            fscanf(fin,"%s", elem->priority);
            fscanf(fin," %[^\n]", elem->description);
            strcpy(elem->status, "queued");

            // insereaza la finalul listei
            insertAt_incident(Lista_incidente, *elem, length_incident(Lista_incidente));

            // gaseste nodul din lista dupa id
            incidentNode* nod = Lista_incidente->sentinel->next;
            while(nod != Lista_incidente->sentinel){
                if(nod->elem.id == elem->id) break;
                nod = nod->next;
            }

            // adauga in coada corespunzatoare prioritatii
            if(strcmp(nod->elem.priority, "low") == 0){
                enqueue(queue_low, &nod->elem);
            }
            if(strcmp(nod->elem.priority , "medium") == 0){
                enqueue(queue_medium, &nod->elem);
            }
            if(strcmp(nod->elem.priority , "high") == 0){
                enqueue(queue_high, &nod->elem);
            }

            // elibereaza copia temporara
            free(elem->description);
            free(elem);
        }
        

        ///...CHECK UNITS AVALABILITY...///
        //  Afiseaza numarul de echipaje disponibile(lungimea cozii)
        if(strcmp(functie, "CHECK_UNITS_AVAILABILITY") == 0){
            fprintf(fout,"Number of available units: %ld\n", queue_avalabile_units->size);
        }

        ///...DISPATCH...///
        //  Demareaza o interventie
        //  Extrage cel mai prioritar incident si primul echipaj disponibil
        //  Cele doua sunt asociate si salvate in lista de interventii si in stiva de istoric
        if(strcmp(functie, "DISPATCH") == 0){
            if(isQueueEmpty(queue_high) == 0 && isQueueEmpty_unit(queue_avalabile_units) == 0){
                incident* incident = queue_high->front->elem;
                unit* unit = queue_avalabile_units->front->elem;
                dequeue(queue_high);
                dequeue_unit(queue_avalabile_units);

                strcpy(incident->status ,"intervened");
                unit->avalability = 0;

                intervention* interventie = malloc(sizeof(intervention));
                interventie->incident = incident;
                interventie->unit = unit;
                insertAt_intervention(Lista_interventii, *interventie,length_intervention(Lista_interventii) + 1);

                push(Stiva_interventii, interventie);
            }else{
                // alege coada cu cea mai mare prioritate nevida
                if(isQueueEmpty(queue_medium) == 0 && isQueueEmpty_unit(queue_avalabile_units) == 0){
                    incident* incident = queue_medium->front->elem;
                    unit* unit = queue_avalabile_units->front->elem;
                    dequeue(queue_medium);
                    dequeue_unit(queue_avalabile_units);

                    strcpy(incident->status ,"intervened");
                    unit->avalability = 0;

                    intervention* interventie = malloc(sizeof(intervention));
                    interventie->incident = incident;
                    interventie->unit = unit;
                    insertAt_intervention(Lista_interventii, *interventie,length_intervention(Lista_interventii) + 1);

                    push(Stiva_interventii, interventie);
                }else{
                    if(isQueueEmpty(queue_low) == 0 && isQueueEmpty_unit(queue_avalabile_units) == 0){
                        incident* incident = queue_low->front->elem;
                        unit* unit = queue_avalabile_units->front->elem;
                        dequeue(queue_low);
                        dequeue_unit(queue_avalabile_units);

                        strcpy(incident->status ,"intervened");
                        unit->avalability = 0;

                        intervention* interventie = malloc(sizeof(intervention));
                        interventie->incident = incident;
                        interventie->unit = unit;
                        insertAt_intervention(Lista_interventii, *interventie,length_intervention(Lista_interventii) + 1);

                        push(Stiva_interventii, interventie);
                    }else{
                        //  niciun incident sau niciun echipaj disponibil
                        fprintf(fout,"INVALID OPERATION! ERROR 404\n");
                    }
                }
            }
        }

        ///...UNDO LAST DISPATCH...///
        //  Anuleaza ultima interventie nefinalizata din stiva de istoric
        //  Extrage din stiva pana gaseste o interventie cu statusul solved
        //  Readauga incidentul la inceputul cozii sale
        //  Readauga echipajul la finalul cozii de disponibilitate
        //  Sterge interventia din lista de interventii
        if(strcmp(functie, "UNDO_LAST_DISPATCH") == 0){
            intervention* interventie = NULL;

            // parcurge stiva si cauta prima interventie nefinalizata
            while(isStackEmpty(Stiva_interventii) == 0){
                intervention* top_int = top(Stiva_interventii);
                pop(Stiva_interventii);

                // verifica statusul incidentului din lista
                incidentNode* nod = Lista_incidente->sentinel->next;
                while(nod != Lista_incidente->sentinel){
                    if(nod->elem.id == top_int->incident->id){
                        if(strcmp(nod->elem.status, "solved") != 0){
                            interventie = top_int;
                        }
                        break;
                    }
                    nod = nod->next;
                }
                if(interventie != NULL){
                    break;
                }
                // eliberam nodul
                free(top_int);
            }
            

            if(interventie == NULL){
                fprintf(fout,"INVALID OPERATION! ERROR 404\n");
            }else{
                incident* inc = interventie->incident;
                unit* u = interventie->unit;
                // resteaza statusul incidentului
                strcpy(inc->status, "queued");

                incidentNode* nod = Lista_incidente->sentinel->next;
                while(nod != Lista_incidente->sentinel){
                    if(nod->elem.id == inc->id){
                        strcpy(nod->elem.status, "queued");
                        break;
                    }
                    nod = nod->next;
                }
    
                // readauga incidentul la inceputul cozii sale de prioritate
                Queue* q = NULL;
                if(strcmp(inc->priority,"high") == 0 ){
                    q = queue_high;
                }else{
                    if(strcmp(inc->priority, "medium") == 0 ){
                        q = queue_medium;
                    }else{
                        if(strcmp(inc->priority,"low") == 0 ){
                            q = queue_low;
                        }
                    }
                }

                if( q != NULL){
                    QueueNode* new_node = malloc(sizeof(QueueNode));
                    new_node->elem = inc;
                    new_node->next = q->front;
                    q->front = new_node;
                    if(q->rear == NULL){
                        q->rear = new_node;
                    }
                    q->size++;
                }

                // readauga echipajul la finalul cozii de disponibilitate
                u->avalability = 1;
                enqueue_unit(queue_avalabile_units, u);

                // sterge interventia din lista si elibereaza
                delete_intervention(Lista_interventii, *interventie);
                free(interventie);

            }
    
        }


        ///...SOLVED INCIDENT...///
        //  Marcheaza un incident ca rezolvat si elibereaza echipajul
        //  eroare daca id-ul nu exista sau incidentul nu are status intervened
        if(strcmp(functie, "SOLVED_INCIDENT") == 0){
            int id;
            fscanf(fin,"%d", &id);

            // verifica daca incidentul exista in lista
            int exista_inc = 0;
            incidentNode* nod = Lista_incidente->sentinel->next;
            while(nod != Lista_incidente->sentinel){
                if(nod->elem.id == id){
                    exista_inc = 1;
                    break;
                }
                nod = nod->next;
            }
            if(exista_inc == 0){
                fprintf(fout,"INVALID OPERATION! ERROR 404\n");
            }else{
                // cauta o interventie activa pentru acest incident
                int k = 0;
                interventionNode* iter = Lista_interventii->sentinel->next;
                while(iter != Lista_interventii->sentinel){
                    if(iter->elem.incident->id == id){
                        if(strcmp(iter->elem.incident->status, "intervened")!=0){
                            break; // nu este in desfasurare
                        }
                        k = 1;
                        strcpy(iter->elem.incident->status , "solved");
                        iter->elem.unit->avalability = 1;
                        enqueue_unit(queue_avalabile_units,iter->elem.unit);
                    }
                    iter = iter->next;
                }
                if(k == 0){
                    fprintf(fout,"INVALID OPERATION! ERROR 404\n");
                }
            }
        }

        ///...SHOW UNIT...///
        //  Afiseaza detaliile unui echipaj dupa un id dat
        //  Eroare daca id-ul nu exista
        if(strcmp(functie, "SHOW_UNIT") == 0){
            int id;
            fscanf(fin,"%d", &id);

            int k = 0;
            for(int i =0; i < nr_echipaje; i++){
                if(v[i]->id == id){
                    k = 1;
                    fprintf(fout,"Unit %d is type %c and is ", v[i]->id, v[i]->type);
                    if(v[i]->avalability == 0){
                        fprintf(fout,"unavailable\n");
                    }else{
                        fprintf(fout,"available\n");
                    }
                }
            }
            if(k == 0){
                fprintf(fout,"INVALID OPERATION! ERROR 404\n");
            }
        }

        ///...SHOW INCIDENT...///
        //  Afiseaza detaliile unui incident dupa un id dat
        //  Eroare daca id-ul nu exista
        if(strcmp(functie, "SHOW_INCIDENT") == 0){
            int id;
            fscanf(fin,"%d", &id);

            int k = 0;
            incidentNode* iter = Lista_incidente->sentinel->next;
            while(iter != Lista_incidente->sentinel){
                if(iter->elem.id == id){
                    fprintf(fout,"Incident %d has %s priority, the following description: %s and is %s\n", iter->elem.id,iter->elem.priority, iter->elem.description, iter->elem.status);
                    k = 1;
                }
                iter = iter->next;
            }
            if(k == 0){
                fprintf(fout,"INVALID OPERATION! ERROR 404\n");
            }
        }

        ///...SHOW INTERVENTIONS...///
        //  Afiseaza toate interventiile in ordinea din lista
        //  Daca lista e goala se afiseaza mesajul dat
        if(strcmp(functie, "SHOW_INTERVENTIONS") == 0){
            interventionNode* iter = Lista_interventii->sentinel->next;
            int k = 0;
            while(iter != Lista_interventii->sentinel){
                k = 1;
                fprintf(fout,"Incident %d was assigned to unit %d, and has the following status: \"%s\"\n", iter->elem.incident->id, iter->elem.unit->id, iter->elem.incident->status);
                iter = iter->next;
            }
            if(k == 0){
                fprintf(fout,"No intervention has been initiated\n");
            }
        }
   
    }

    /// Elibereaza toata memoria alocata
    destroyStack(Stiva_interventii);
    destroyQueue_unit(queue_avalabile_units);
    destroyQueue(queue_high);
    destroyQueue(queue_low);
    destroyQueue(queue_medium);
    freeList_incident(Lista_incidente);
    freeList_intervention(Lista_interventii);
    for(int i = 0; i < nr_echipaje; i++){
        free(v[i]);
    }
    free(v);
    fclose(fin);
    fclose(fout);
    return 0;
   
}