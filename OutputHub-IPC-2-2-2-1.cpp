 #include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <arpa/inet.h>	// For sockets
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>	// For sockets
#include <sys/socket.h>	// For sockets
#include <sys/shm.h> 	// For shared memory

#include "OutHub-IPC-Header-2-2-2-1.h"
#include "pi-Net-OutHub-Anncmnts.c"
#include "OutHub-IPC-Functions.cpp" 

// Shared Name Id's (Variables that Share Name and Initialization, Until Manipulated by Their Indepent Processes //
//== After Modification with in a Separte Process these Variables Share Name, but not New Update Stored Value(s) ==//

#define MSGSZ 84
#define MAX_NUMBER_OF_TOKENS 16

int keepAliveCounter, insideKeepAliveCount;

struct MsgBuff{
	long mtype;
	char mtext[MSGSZ];
};

int processChildStates = 1;	
int processParentStates = 1;
bool initializedOnce = false;

int main(int argc, char **argv)
{

// Shared Name Id's (Variables that Share Name and Initialization, Until Manipulated by Their Indepent Processes //
//== After Modification with in a Separte Process these Variables Share Name, but not New Update Stored Value(s) ==//	
	
//############## End shared memory initialization ################//

pid_t pid = fork();

// If fork() success
 if (pid >= 0)
{		
// Run in one infinite loop
for ( ; ; )
	{
	//============= Start child process ==================//
	if (pid == 0)
		{	
		// Child process will create the socket and bind
		// the socket for reading packet data
		long msgCtrlValue; 		
		MsgBuff sndBuf;	
		static int msqid;
		key_t key;
		//size_t buf_length;
		MsgBuff *ptrSnd = &sndBuf;
		if(initializedOnce == false)
			{	
			printf("\n\nour're Here...\n\nChildProcess State: %d\n\n", processChildStates);
			fflush(stdout);	
			/////////////////////////////////////////////////////////////sndBuf.mtype = Header_Cmpnts;	
			key = ftok("/home/pi/projects/Output/OutputHub-02/keyFile", 'B');	

			if( (msqid = msgget(key, 0666 | IPC_CREAT) ) < 0)
				{perror("msgget"); exit(1);}
			else
				{(void)fprintf(stderr, "msgget: mgget succeeded: msgqid = %d\n", msqid);}
			msgCtrlValue = 0;
			processChildStates = 1;
			initializedOnce = true;
			sleep(1);				
			}		
					
		switch(processChildStates)
			{
			case 1 :
				{
				//////////////////////////////////////////////////////////////////	close(pipefds[0]);
				memset(sockData, 0, sizeof(sockData) );
				// Creating the socket from variable sockfd
				sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
				if (sock == -1) 
					{printf("Error Creating Socket\n\n"); fflush(stdout); }
				else 
					{printf("Socket Created\n\n"); fflush(stdout); }
				// Set and initialize socket members
				memset( (char *) &myaddr, 0, sizeof(myaddr) );
				myaddr.sin_family = AF_INET;
				myaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Broadcast any port that's sending us data
				myaddr.sin_port = htons(LOCAL_PORT); // Receive data at LocalPort 1234
									
				// Bind the socket
				// The syntax for binding a socket is
				// int bind(mySocket, (struct sockaddr *)&myaddr, sizeof(myaddr) )
				if ( bind(sock, (struct sockaddr *)&myaddr, sizeof(struct sockaddr)) == -1) 
					{printf("\nBind Socket Function Unsuccessful\n"); fflush(stdout);}
				else 
					{printf("\nBind Socket Function Verified...\n"); fflush(stdout); }
				addr_len = sizeof(struct sockaddr);	
					
				printf("\n\nListening for UDP Data\n\n");
				fflush(stdout);
				memset(sockData, 0, sizeof(sockData) );
				//memset(sharedPtr, 0, SMSIZE);
				if ( (numbytes = recvfrom(sock, sockData, MAX_UDP_INPUT_STREAM, MSG_OOB,
				(struct sockaddr *)&theiraddr, &addr_len) ) == -1) 
					{printf("\n\nSocket Not Read...\n\n"); fflush(stdout); }
				else
					{
					if(sockData[0] == (char)Header_Cmpnts)
						{		
						memset(&sndBuf, 0, sizeof(MsgBuff) );	

						msgCtrlValue = extractStrTokenCmpnt(ptrSnd->mtext, sockData);
					
						printf("\n\nRChild (1): Raw Socket Data Extracted and Filtered: %s, Ctrl Token: %d\n\n", ptrSnd->mtext, (int) msgCtrlValue); 
					
						ptrSnd->mtype = (long)Header_Cmpnts;
						if(msgsnd(msqid, &sndBuf, (80), 0) == -1) 
							perror("msgsnd");		
						else
							{printf("\n\nFirst Socket Data Transmitted...\n\n"); fflush(stdout); processChildStates = 2; }	
											
						}		
					}					
				sleep(1);
				break;
				}		
			case 2 : //Child_Main_Processes :
				{
				// Listen for  UDP data in an infinite for loop
				printf("\n\nListening for UDP Data\n\n");
				fflush(stdout);
				memset(sockData, 0, sizeof(sockData) );
				if ( (numbytes = recvfrom(sock, sockData, MAX_UDP_INPUT_STREAM, MSG_OOB,
				(struct sockaddr *)&theiraddr, &addr_len) ) == -1) 
					{printf("\n\nSocket Not Read...\n\n"); fflush(stdout); }
				else
					{
					if(sockData[0] >0)
						{		
						memset(&sndBuf, 0, sizeof(sndBuf) );	

						msgCtrlValue = extractStrTokenCmpnt(ptrSnd->mtext, sockData);
					
						printf("\n\nRChild (2): Raw Socket Data Extracted and Filtered: %s, Ctrl Token: %d\n\n", ptrSnd->mtext, (int) msgCtrlValue); 
					
						ptrSnd->mtype = (long)msgCtrlValue;
						if(msgsnd(msqid, &sndBuf, (80), 0) == -1) 
							perror("msgsnd");		
						else
							{printf("\n\nChild (2): Socket Data Transmitted...\n\n"); fflush(stdout); }						
						}		
					}			
				break;
				}		
			default : break;
			}
			//############### End child process ##################//
		}						
	else //============== Start parent process ===============//
		{
		MsgBuff rcvBuf;	
		static int msqid;
		key_t key;
		//size_t buf_length;
		MsgBuff *rcvPtr = &rcvBuf;
		
		int outputState;
		int linePositionVector;	
		int fileIndexPosition;

		if(initializedOnce == false)
			{	
			key = ftok("/home/pi/projects/Output/OutputHub-02/keyFile", 'B');	

			if( (msqid = msgget(key, 0666 | IPC_CREAT) ) < 0)
				{perror("msgget");	exit(1); }
			else
				{(void)fprintf(stderr, "msgget: mgget succeeded: msgqid = %d\n", msqid);}
	
			anncmntType = Header_Cmpnts;
			fileIndexPosition = 1;
			outputState = PostInitPageHander;
			linePositionVector = 0;				
			processParentStates = 1;	
			initializedOnce = true;	
			sleep(1);			
			}				
			memset(&rcvBuf, 0, sizeof(MsgBuff) );			
			int ia;
			char packet[16];
			memset(packet, 0, sizeof(packet) );
			switch(processParentStates)
				{
				case 1 : //Parent_Initialization :
				{
				wav = playSound(BootAnncmnt);
				printf("\n\nBoot Announcement Debugging Output: %d\n\n", BootAnncmnt);
				fflush(stdout);
				Printer = fopen("/dev/usb/lp0", "r+"); //b");	
				if(Printer)
					{
					printf("\n\nPrinter Connection Opened via a File Handle\n\n");
					
					fflush(stdout);
					//fprintf(Printer, "%s\n\r", InitHeadConstStr[0]);
					printBootInitHeader(InitHeadConstStr[0], InitHeadConstStr[1], InitHeadConstStr[2]);
					sleep(1);
					fprintf(Printer, "\n\r  %s\n\r", DefaultHeaderString);				
					fflush(Printer);
					fflush(stdout);					
					}
				else
					printf("\nPrinter Connection Failed to Opened\n");
			
				while(processParentStates == 1)
					{	
					if(msgrcv(msqid, rcvPtr, MSGSZ, (long)Header_Cmpnts, MSG_NOERROR | IPC_NOWAIT) == -1)
						{
						if(keepAliveCounter++ > 10) //20)
							{		
							keepAliveCounter = 0;
							fprintf(Printer, "%c", 0x07);
							fflush(Printer);								
							}	
						sleep(1);							
						perror("rcvmsg");	
						}
					else
						{
							
						updateHeader(stdPrinterHdrBuf, rcvBuf.mtext, DefaultHeaderString);	

						printf("\nParent (1) IPC Message:\n%s\nFirst IPC Message Type: %d", rcvBuf.mtext, (int)rcvBuf.mtype);
						printf("\nContents of Updated Run-Time Header, Contents were Updated in Child Process:\n%s\n\n", stdPrinterHdrBuf);
						fflush(stdout);
						
						fprintf(Printer, "\n\r  %s\n\r\n\r", stdPrinterHdrBuf);				
						fflush(Printer);					
						printf("\n\nFirst IPC Message:\n%s\nFirst IPC Message Type: %d", rcvBuf.mtext, (int)rcvBuf.mtype);
						fflush(stdout);
						wav = playSound(BootAnncmnt);
						linePositionVector = 0;
						////outputState = PostInitPageHander;
						processParentStates = 2;						
						}
					}
				}
				break;
				case 2 : //Parent_Main_Processes :
					{
					for(ia=Help_1; ia<=(MAX_NUMBER_OF_TOKENS + Help_1); ia++)
						{
						if(msgrcv(msqid, &rcvBuf, MSGSZ, (long)ia, MSG_NOERROR | IPC_NOWAIT) == -1)
							{perror("rcvmsg"); fflush(stdout); usleep(100000); }
						else
							{
							printf("\n\nPartent (2) IPC Message: %s, %ld\n", rcvPtr->mtext, rcvPtr->mtype); //rcvBuf.mtext); 
							fflush(stdout); 
							
							
							if(rcvPtr->mtype != (long)BootAnncmnt)
								{
								if(rcvPtr->mtype == (long)Header_Cmpnts)
									updateHeader(stdPrinterHdrBuf, rcvBuf.mtext, DefaultHeaderString);	
								else
									{
									outputFunctions(rcvPtr->mtext, rcvPtr->mtype, &linePositionVector, &outputState, &fileIndexPosition);		
								; //***//	fprintf(stdout, "\n\nOutput State Variable, in Parent(2) Calling Function Scope: %d\n\n", outputState);
								; //***//	fprintf(stdout, "\nLine Position Vector Variable, in Parent(2) Calling Function Scope: %d\n", linePositionVector);
								; //***//	fprintf(stdout, "\nFile Position Vector Variable, in Parent(2) Calling Function Scope: %d\n\n", fileIndexPosition);
								; //***//	fflush(stdout);
									}
								}
							
							}
						}
					printf("\n\n********************************\n\n");
					
					if(rcvPtr->mtype == (long)Header_Cmpnts)
						{
						fprintf(stderr, "\n\nParent (2):\n Message Type IPC : %d\n", (int)rcvPtr->mtype);
						fflush(stdout);
						}	
					
					if(keepAliveCounter++ > 37)
						{
						keepAliveCounter = 0;
						//fprintf(Printer, "%c", 0x06);
						fprintf(Printer, "%c", 0x07);						
						fflush(Printer);
						if(insideKeepAliveCount++ > 10)
							{
							insideKeepAliveCount = 0;
							//fprintf(Printer, "%s\n\r", PrinterMaintenanceString);				
							fflush(Printer);
							keepAliveCounter = 0;								
							}
						}
						fflush(stdout);	
						sleep(3);	
					}
				break;
				default :
			; //***//	printf("\n\nParent Processes Existing in an Undefined State...\n\n");
			; //***//	fflush(stdout);
				break;
				}				

		} // End if() Parent Process
	} // End infinite for loop, Loop Contains (2) Processes				
} // End if Fork() Sucessful		
else
{
// fork failed
return 1;
}

return 0;
} // End of Main (Outer Container, for (2) forked Processes)


