#ifndef CONF_H
#define CONF_H

#define SEPARATOR ":"
#define MESSAGE_END "$$"
#define MAX_MSG_SIZE 1024

#define PORT_NUMBER 4242
#define MAX_REQUEST 5

#include <stdlib.h>
#include <assert.h>

#ifndef DEBUG
#define CHECK_IF(stat, val, msg)    \
	if ( (stat) == (val) )          \
	{                               \
		perror(msg);                \
		exit(EXIT_FAILURE);       \
    }                       
#else
#define CHECK_IF(stat, val, msg)    \
	if ( (stat) == (val) )          \
	{                               \
    	perror(msg);                \
		exit(EXIT_FAILURE);       \
    }                               \
    else printf("%s ... OK\n", msg)
#endif


typedef char* T_elt;

char * toString(T_elt e) {
	return e; 
}

typedef struct node {
	T_elt data;
	struct node *pNext;
} T_node, * T_list;


T_node * addNode (T_elt e, T_node * node);
void showList(T_list l);
void freeList(T_list l);
T_elt getFirstElt(T_list l);
T_list removeFirstNode(T_list l);

static T_node * newNode(T_elt e) {
	T_node * pNode; 
	CHECK_IF(pNode = malloc(sizeof(T_node)), NULL, "malloc allocateNode"); 
	pNode->data = e; 
	pNode->pNext = NULL;

	return pNode;
}


T_node * addNode (T_elt e, T_node * node) {
	T_node * pNode; 
	pNode = newNode(e); 
	pNode->pNext = node; 
	return pNode;
}


void showList(T_list l) {
	while(l != NULL) {
		printf("%s \n", toString(l->data));
		l = l->pNext; 
	}
}

void freeList(T_list l) {
	assert(l != NULL); 
	T_node * pAux; 
	// Il faut un pointeur auxiliaire : 
	// on ne doit libérer qu'après avoir enregistré quelque part
	// l'adresse de la maille suivante
	while(l != NULL) {
		#ifdef DEBUG
			printf("Libération de %d\n", l->data);
		#endif
		pAux = l->pNext; 
		free(l); 
		l = pAux; 
	}
	printf("NULL");
}

T_elt getFirstElt(T_list l) {
	return l->data; 
}

T_list removeFirstNode(T_list l) {
	assert(l!= NULL);	
	T_node * p =l->pNext; 
	free(l); 
	return p; 
}

T_list tailAddNode(T_elt e, T_list l){
	T_node * pNode; 
	pNode = newNode(e); 
	pNode->pNext = NULL;           //Création d'une nouvelle Node
	if(l==NULL)
		return pNode;
	else{
		T_list lAux=l;
		while(lAux->pNext!=NULL){        //Parcours de la liste
			lAux = lAux->pNext; 
		}
		lAux->pNext=pNode;               //ajout de la node en bout de liste
		return l;
	}
}

char * intToString(int node) {
	static char buffer[11];
	sprintf(buffer, "%d", node); 
	return buffer; 
}

int sendData(int socket, int code, int id, int position, int speed) {
	char data[MAX_MSG_SIZE] = "";
    strcat(data, intToString(code));
	strcat(data, SEPARATOR);
	strcat(data, intToString(id));
    strcat(data, SEPARATOR);
	strcat(data, intToString(position));
    strcat(data, SEPARATOR);
    strcat(data, intToString(speed));
    strcat(data, MESSAGE_END);

    if (send(socket, data, strlen(data),MSG_NOSIGNAL) < 0) {
        perror("Error writing message to socket");
        return -1;
    } else {
        printf("Sending the following message : %s\n", data);
        return 1;
    }
    return -1;
} 


// parse the receive message to extract a single message for later exploitation
T_list getOneMessage(T_list list, char data[]) {
    char* token;
    int i = 0;

    // get first message 
    token = strtok(data, MESSAGE_END); // cut in two parts
	
    list = addNode(token, list);

    // get all remaining messages from token
    while( token != NULL ) {
        token = strtok(NULL, MESSAGE_END);
		if (token != NULL) {
			list = addNode(token, list);
		}
        
    }
	return list;
}

void parseMessage(char data[], int* code, int* id, int* position, int* speed) {
	char *ptr;
	char delim[] = SEPARATOR;

	ptr = strtok(data, delim);
	*code = atoi(ptr);
	ptr = strtok(NULL, delim);
	*id = atoi(ptr);
	ptr = strtok(NULL, delim);
	*position = atoi(ptr);
	ptr = strtok(NULL, MESSAGE_END);
	*speed = atoi(ptr);
}

#endif