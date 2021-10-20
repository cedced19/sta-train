#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "RBC.h"

#define MAXLEN 500

Train *trains=NULL;

void* connection_handler(void *socket_desc){
    int sock = *(int*)socket_desc;
    char message[MAXLEN];
    int read_size;
    T_list list = NULL;
    int code;
    int id;
    int pos;
    int speed;

    do {
            int read = read_size = recv(sock , message , MAXLEN , 0);
            if (read < 0) {
                    perror("Reading stream message");
            } else if (read == 0) {
                // TODO remove train
                perror("Ending connection\n");
                break;
            } else {
                do {
                    list = getOneMessage(list,message);
                    parseMessage(message, &code, &id, &pos, &speed);
                    printf("Responding new message from %i with code %i\n", id, code);
                    switch(code){
                        case 1: // receive position/speed
                            // store data
                            printf("Received initialization from train %i\n", id);
                            trains=storeData(id,pos,speed,trains);
                            sendData(sock, 2, id, pos, speed); //send ack 
                            showTrains(trains);
                            break;
                        case 3 : // send command
                            printf("Received position from train %i\n", id);
                            trains=storeData(id,pos,speed,trains);
                            showTrains(trains);
                            sendData(sock, 6, id, pos, 40); //send ack 
                            break;
                        default:
                            break;
                    }
                    //showTrains(trains);
                    
                    list = removeFirstNode(list);
                    //showList(list);
                    printf("Message done!\n");
                
                } while(list!=NULL);
            }
    } while(1);
    return NULL;
}


static Train * newTrain(int id, int pos, int speed) {
	Train * train; 
	CHECK_IF(train = malloc(sizeof(Train)), NULL, "malloc allocateNode"); 
	train->id = id; 
	train->pos = pos; 
	train->speed = speed; 
	train->nextTrain = NULL;

	return train;
}


Train * addTrain (int id, int pos, int speed, Train *trains) {
	Train * train; 
	train = newTrain(id, pos, speed); 
	train->nextTrain = trains; 
	return train;
}

void showTrains(Train *trains) {
	while(trains!= NULL) {
		printf(" => Train numéro : %d \n",trains->id);
        printf("  --------------------\n");
		printf("\tpos : %d \n",trains->pos);
		printf("\tspeed : %d \n\n",trains->speed);
		trains= trains->nextTrain; 
	}
}

Train * selectTrain (int id_train, Train *trains) {
    Train *trainscp = trains;
    while(trains!=NULL && id_train!=trains->id){
        trains=trains->nextTrain;
    }
    if(trains==NULL) trains=addTrain(id_train,-1,-1,trainscp);
    return trains;
} 

void * storeData(int id_train, int pos, int speed, Train * trains){
    trains=selectTrain(id_train, trains);
    trains->pos=pos;
    trains->speed=speed;
    showTrains(trains);
    return NULL;
}

int calcDistance(int id_train){
    Train *train1 = selectTrain(id_train, trains);
    // chercher le train suivant (position sup, à un tour pres)
    int distmin;
    while(trains!=NULL){
        if(trains->id!=id_train){
            if(abs(train1->pos - trains->pos)<distmin) distmin=abs(train1->pos - trains->pos);
            if(abs(train1->pos - trains->pos + DISTTOUR)<distmin) distmin=abs(train1->pos - trains->pos + DISTTOUR);
            if(abs(train1->pos - trains->pos - DISTTOUR)<distmin) distmin=abs(train1->pos - trains->pos - DISTTOUR);
        }
    }
    return distmin;
}

int main()
{
    pthread_t thread;

    int sock;
    struct sockaddr_in server, client;

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Opening stream socket");
        exit(1);
    }

    /* Name socket */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT_NUMBER);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server))) {
        perror("Binding stream socket");
        exit(1);
    }

    /* Accepting incoming connections */
    listen(sock, MAX_REQUEST);
    puts("Listening");
    int c = sizeof(struct sockaddr_in);
    int new_socket;
    trains=addTrain(0,-1,-1,NULL);

    while ((new_socket = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c)))
    {
        int *new_sock = malloc(1);
        *new_sock = new_socket;

        /* Start a new thread to handle the connection */
        if(pthread_create(&thread, NULL, connection_handler, (void*) new_sock)<0)
        {
            perror("Could not create thread");
            return 1;
        }

        puts("New connection assigned to handler");
    }

}