#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define h_addr h_addr_list[0] /* for backward compatibility */

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    // The code first connects to port 6000 (23000 for emulator)
    // of localizer
    // and requests data by sending the target's MAC
    // address. The localizer responds by giving the port 
    // which the target has to listen to get the data.
    // The target then listens to this port indefinitely.

    // Command line argument - IP address of the localizer 
    // (ee107server.stanford.edu)

    int portno = 23000; // 23000 for emulator, 6000 for actual
    // Don't use port 6000 unless you have reserved it.
    char hostname[] = "ee107server.stanford.edu"; 
    // char hostname[] = "localhost"; // For debugging
                    // hostname or IP address of the localizer
    
    // No need to touch this part under normal operations //
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    // No need to touch this part under normal operations //
    
    /** Send the MAC address for which the data is needed **/
    char macAddress[]="C03FD560BEEF"; // 12 characters

    n = write(sockfd,macAddress,sizeof(macAddress));
    if (n < 0) 
        error("ERROR writing to socket");
    int portToReadData=0;
    n = read(sockfd,&portToReadData,sizeof(int));
    if(portToReadData!=-1)
        printf("Localizer asked to listen at Port %d.\n",portToReadData);
    else {
        printf("Localizer says the MAC address are already on the list.\n");
        printf("Exiting...\n");
        return(0);
    }
    close(sockfd);



    /** Listen at the port that the localizer said **/
    // No need to touch this part under normal operations //
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    int newsockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM and SOCK_DGRAM
    if (sockfd < 0) 
      error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = portToReadData;
    // allow socket/address reuse 
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))<0)
      error("setsockopt(SO_REUSEADDR) failed");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
      error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    // No need to touch this part under normal operations //
    
    const int lengthOfData=12;
    float newData[lengthOfData]; // Array to hold new data from the localizer
    while(1) {
        printf("Listening at port %d...\n", portToReadData);
        newsockfd = accept(sockfd, 
                     (struct sockaddr *) &cli_addr, 
                     &clilen);

        if (newsockfd < 0) 
            error("ERROR on accept");
        float newData[16];
        n = read(newsockfd,newData,sizeof(float)*lengthOfData);
        if (n < 0)  error("ERROR reading from socket");
        printf("Data received from port %d: \n", portToReadData);

        /* newData array has the output from the 
        localization algorithm. */
        printf("Timestamp: %f s, %f ms\n", newData[0], newData[1]);
        printf("Position (%f, %f, %f): \n", newData[2],newData[3],newData[4]);
        printf("Accelerometer (%f, %f, %f): \n", newData[5],newData[6],newData[7]);
        printf("Orientation quat (%f, %f, %f, %f): \n", newData[8],newData[9],newData[10],newData[11]);

        /***************
        ****************
        Do whatever you want with newData!
        ****************
        ****************/

        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
