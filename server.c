#include <stdio.h>      
#include <sys/types.h>
#include <sys/socket.h>   
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <ctype.h>

#define buffer 256
#define SIZE 4
#define SENDSIZE 7

const char SEND[SENDSIZE] = "SEND ID"; 
const char READ[SIZE] = "READ";
const char DELE[SIZE] = "DELE";
const char QUIT[SIZE] = "QUIT";



void convertToUpperCase(char *s){
    while(*s != '\0'){
        *s = toupper((unsigned char)*s);
        ++s;
    }
}




int main(int argc, char *argv[]) {
    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    struct sockaddr_in simpleServer;


    if (2 != argc) {

        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);

    }

    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (simpleSocket == -1) {

        fprintf(stderr, "Non è possibile creare la socket!\n");
        exit(1);

    }
    else {
	    fprintf(stderr, "Socket creata!\n");
    }

    /* retrieve the port number for listening */
    simplePort = atoi(argv[1]);

    /* setup the address structure */
    /* use INADDR_ANY to bind to all local addresses  */
    memset(&simpleServer, '\0', sizeof(simpleServer)); 
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
    simpleServer.sin_port = htons(simplePort);

    /*  bind to the address and port with our socket  */
    returnStatus = bind(simpleSocket,(struct sockaddr *)&simpleServer,sizeof(simpleServer));

    if (returnStatus == 0) {
	    fprintf(stderr, "Bind completed!\n");
    }
    else {
        fprintf(stderr, "Could not bind to address!\n");
		close(simpleSocket);
		exit(1);
    }

    /* lets listen on the socket for connections      */
    returnStatus = listen(simpleSocket, 5);

    if (returnStatus == -1) {
        fprintf(stderr, "Cannot listen on socket!\n");
		close(simpleSocket);
        exit(1);
    }else{

    while (1) {
        struct sockaddr_in clientName = { 0 };
	    int simpleChildSocket = 0;
	    int clientNameLength = sizeof(clientName);
	   /* wait here */

        simpleChildSocket = accept(simpleSocket,(struct sockaddr *)&clientName, &clientNameLength);

        if (simpleChildSocket == -1) {

            fprintf(stderr, "Cannot accept connections!\n");
            close(simpleSocket);
            exit(1);
        }

    /* handle the new connection request  */     

    /* ricezione ID dal client */
        int rId;
        char sId[buffer] = "";    
        char messaggio [buffer] = "";

        rId = read(simpleChildSocket,sId,sizeof(sId));

        int userID = atoi(sId);
    /* invio messagio connessione al client */
        if(userID<1 || userID>20){
            strcpy(messaggio,"ER UTENTE NON VALIDO. ARRIVEDERCI.\n");
            write(simpleChildSocket, messaggio, strlen(messaggio));
            close(simpleChildSocket);
        }
        else{
            strcpy(messaggio,"OK BENVENUTO UTENTE ");
            //char messaggio [buffer]="";
            strcat (messaggio,sId);
            strcat (messaggio,"\n");
            write(simpleChildSocket, messaggio, strlen(messaggio));
        }
        //write(simpleChildSocket, messaggio, strlen(messaggio));
        //Ricezione comando dal Client

        int readCmd=0;
        char cmd[buffer]="";
        char msgS[buffer]="";

        readCmd= read(simpleChildSocket, cmd, sizeof(cmd));
        int rcvID;
        //char appo[2]="";
        //memcpy(appo, &cmd[6], 2 );

        convertToUpperCase(cmd);
        

        if(strncmp(cmd ,SEND, SENDSIZE)==0){
            rcvID=atoi(&cmd[8]);
            //printf("%s\n",cmd);
            //printf("L'ID e' : %d\n",rcvID);
            if(rcvID<1 || rcvID >20){
                strcpy(messaggio,"ER UTENTE NON VALIDO. ARRIVEDERCI.\n");
                write(simpleChildSocket, messaggio, strlen(messaggio));
                close(simpleChildSocket);
            }else{
                //GESTIONE ID E MESSAGGIO
            }
        }else{
            strcpy(messaggio,"ER FORMATO NON VALIDO. ARRIVEDERCI.\n");
            write(simpleChildSocket, messaggio, strlen(messaggio));
            close(simpleChildSocket);
        }
            






        close(simpleChildSocket);
    }




    }

    //lettura messaggi dal client
    







    close(simpleSocket);
    
    return 0;
}



