#ifndef CONF_H
#define CONF_H

#define SEPARATOR ":"
#define MESSAGE_END "$$"
#define MAX_MSG_SIZE 256

#define PORT_NUMBER 4244
#define MAX_REQUEST 5
#define DISTTOUR 17444

#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

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
	printf("Start reading\n");
	while(l != NULL) {
		printf("%s \n", toString(l->data));
		l = l->pNext; 
	}
	printf("End list reading\n");
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
        return 0;
    } else {
        //printf("Sending the following message : %s\n", data);
        return 1;
    }
    return 0;
} 

int checkMessage(char data[]) {
	if (strlen(data)<7) {
		return 0;
	}
	if(data[strlen(data)-1]==':') return 0;
	return 1;
}

int strpos(char *hay, char *needle, int offset)
{
	char haystack[strlen(hay)];
	strncpy(haystack, hay+offset, strlen(hay)-offset);
	char *p = strstr(haystack, needle);
	if (p)
		return p - haystack+offset;
	return -1;
}

// parse the receive message to extract a single message for later exploitation
/*T_list splitMessages(T_list list, char data[]) {
	if (strlen(data) > 1) {		
		char* token;
		if (strpos(data, MESSAGE_END, 0)>=7){  // check if the message is full  
			token = strtok(data, MESSAGE_END); // cut in two parts //
			list = addNode(token, list);
			printf("Message ok : %s\n", token);

			// get all remaining messages from token
			while( token != NULL ) {
				if (strpos(token, MESSAGE_END, 0)>=0){token = strtok(NULL, MESSAGE_END);} 
				printf("%s\n", token);	
				printf("%d\n",strpos(token, MESSAGE_END, 0));
				if (strpos(token, MESSAGE_END, 0)>=7){ // 
					if (token != NULL) {
						if (checkMessage(token)) {
							printf("\nList %s\n", token);
							list = addNode(token, list);
						}
					}
				}
			}
		} 
	}
	return list;
}
*/
T_list splitMessages(T_list list, char data[]) {
	if (strlen(data) > 1) {		
		char* token;
		// get first message 
		token = strtok(data, MESSAGE_END); // cut in two parts
		if (checkMessage(token)) {
			//printf("\nList %s\n", token);
			list = addNode(token, list);
		}
		

		// get all remaining messages from token
		while( token != NULL ) {
			token = strtok(NULL, MESSAGE_END);
			//data+=2; //remove $$ from $$1:5:... 
			//token = strtok(data, MESSAGE_END); // extract token from 1:5:1:1$$...
			if (token != NULL) {
				if (checkMessage(token)) {
					//printf("\nList other msg %s\n", token); // quasi certain que ca n'apparait jamais ca ... strtok l.210 renvoit tjrs NULL car strtok(str, delim)
					list = addNode(token, list);
				}
			}
			
		}
	}
	return list;
}


T_list parseMessage(T_list list, int* code, int* id, int* position, int* speed) {
		char *ptr;
		char delim[] = SEPARATOR;
		if ((ptr = strtok(list->data, delim))) {
			*code = atoi(ptr);
		} else {
			*code = -1;
		}
		if ((ptr = strtok(NULL, delim))){
			*id = atoi(ptr);
		} else{
			*id = -1;
		}
		if ((ptr = strtok(NULL, delim))) {
			*position = atoi(ptr);
		} else {
			*position = -1;
		}
		if ((ptr = strtok(NULL, MESSAGE_END))) {
			*speed = atoi(ptr);
		} else {
			*speed = -1;
		}
		return removeFirstNode(list);
}



float timeDiff(struct timeval *start, struct timeval *end) {
    return (end->tv_sec - start->tv_sec)*1e6 + (end->tv_usec - start->tv_usec);
}
// usage : time_diff(&lastUpdate, &currentTime)
// struct timeval currentTime;
// gettimeofday(&currentTime, NULL);

#endif