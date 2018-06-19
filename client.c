#include <stdio.h>      
#include <sys/types.h>
#include <sys/socket.h>   
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define dim 5
#define dimCmd 256

int main(int argc, char *argv[]) {
    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    char buffer[256] = "";
    struct sockaddr_in simpleServer;
    
    if (3 != argc) {

        fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
        exit(1);

    }

    /* create a streaming socket      */
    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (simpleSocket == -1) {

        fprintf(stderr, "Could not create a socket!\n");
        exit(1);

    }
    else {
	    fprintf(stderr, "Socket created!\n");
    }

    /* retrieve the port number for connecting */
    simplePort = atoi(argv[2]);

    /* setup the address structure */
    /* use the IP address sent as an argument for the server address  */
    //bzero(&simpleServer, sizeof(simpleServer)); 
    memset(&simpleServer, '\0', sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    //inet_addr(argv[2], &simpleServer.sin_addr.s_addr);
    simpleServer.sin_addr.s_addr=inet_addr(argv[1]);
    simpleServer.sin_port = htons(simplePort);

    //CONTROLLO ID PER CONNESSIONE AL SERVER


    	int simpleId=0;

		printf("Inserire identificativo valido (compreso tra 1 e 20): ");

		//do{
							
			scanf("%d",&simpleId);
			/*if(simpleId < 1 || simpleId > 20){
				printf("Errore, inserisci un numero compreso tra 1 e 20: ");
			}	
			
		}while(simpleId < 1 || simpleId > 20);*/
		




    /*  connect to the address and port with our socket  */
    returnStatus = connect(simpleSocket, (struct sockaddr *)&simpleServer, sizeof(simpleServer));

    if (returnStatus == 0) {
	    fprintf(stderr, "Connessione riuscita!\n");
    }
    else {
        fprintf(stderr, "Connesssione non riuscita!\n");
	    close(simpleSocket);
	    exit(1);
    }

    //Invio ID al Server
		char id[12]="";
		sprintf(id,"%d",simpleId);
		int sendId = 0;
		
		sendId = write (simpleSocket,id,strlen(id));


    //Risposta Identificazione Utente da parte del Server
    returnStatus = read(simpleSocket, buffer, sizeof(buffer));
    
    if ( returnStatus > 0 ) {
        printf("%s",buffer);
    } else {
        fprintf(stderr, "Return Status = %d \n", returnStatus);
    } 
    
    // Interazione con il Server

    char cmd[dimCmd]="";
    int sendCmd=0;
    printf("Inserire comando: ");
    scanf("%s",cmd);
    //fgets(cmd,dimCmd,stdin);
    sendCmd = write(simpleSocket, cmd, strlen(cmd));



        close(simpleSocket);
    
    return 0;
}
