
void printBootInitHeader(char *str_1, char *str_2, char *str_3)
{
	fprintf(Printer, "%s\n\r", str_1);
	fflush(Printer);	
	sleep(1);	
	fprintf(Printer, "%s\n\r", str_2);
	fflush(Printer);	
	sleep(1);	
	fprintf(Printer, "%s\n\r", str_3);
	fflush(Printer);	
	sleep(1);	
}

int isValidAsciiNum(int n)
{
	if( (n>47) && (n<58) )
	{
		return 1;
	}
	else
		return 0;
}

int extractStrTokenCmpnt(char *mesStrPtr, char *unFilteredIn)
{
	int i;
	int strCtrlToken = 0;
	//ii=0;
	strCtrlToken = (int) *(unFilteredIn+0); 
	for(i=1; i<(MAX_OUTPUT_STR_LEN-2); i++)
		{
		if( ( *(unFilteredIn+i) > 31) && ( *(unFilteredIn+i) < 128) )
			{
			*(mesStrPtr++) = *(unFilteredIn+i);
			}
		}
	return strCtrlToken;
}

/* Function to update the header with current date stamp.
 * The date stamp received is of format MMDDYY, I.E. 080116 for
 * August 1st, 2016. This function will parse the date string
 * into MM-DD-YYYY format. Legacy Project Component, Should be 
 * Updated...
 */

void updateHeader(char output[], char *input, char hdrConstStr[])
{
	strcpy(output, hdrConstStr);
	int strPosition = 6; // To keep track of the UDP date string
	if(isValidAsciiNum( *(input + strPosition) ) )
		*(output + FIXED_HDR_CMPNTS_LEN) = *(input + strPosition);
	strPosition = 7;
	if(isValidAsciiNum( *(input + strPosition) ) )	
		*(output+FIXED_HDR_CMPNTS_LEN+1) = *(input + strPosition);	
	
	// Date Stamp
	strPosition = 0; // Month1
	if(isValidAsciiNum( *(input + strPosition) ) )	
		*(output+FIXED_HDR_CMPNTS_LEN+4) = *(input + strPosition);		
	strPosition = 1; // Month2
	if(isValidAsciiNum( *(input + strPosition) ) )	
		*(output+FIXED_HDR_CMPNTS_LEN+5) = *(input + strPosition);	
	hdrConstStr[FIXED_HDR_CMPNTS_LEN+6] = '-';
	strPosition = 2; // Day1
	if(isValidAsciiNum( *(input + strPosition) ) )	
		*(output+FIXED_HDR_CMPNTS_LEN+7) = *(input + strPosition);
	strPosition = 3; // Day2
	if(isValidAsciiNum( *(input + strPosition) ) )	
		*(output+FIXED_HDR_CMPNTS_LEN+8) = *(input + strPosition);
	*(output+FIXED_HDR_CMPNTS_LEN+9) = '-';
	*(output+FIXED_HDR_CMPNTS_LEN+10) = '2';
	*(output+FIXED_HDR_CMPNTS_LEN+11) = '0';	
	strPosition = 4; // Year1
	if(isValidAsciiNum( *(input + strPosition) ) )	
		*(output+FIXED_HDR_CMPNTS_LEN+12) = *(input + strPosition);
	strPosition = 5; // Year2
	if(isValidAsciiNum( *(input + strPosition) ) )	
		*(output+FIXED_HDR_CMPNTS_LEN+13) = *(input + strPosition);
}

int indexFileHander(void)
{
	int logIndxFileVect, lgIndxFileVectShadow;
	char indexFileNumStr[10];
	char indexFileBuffer[32];
	memset(indexFileBuffer, 0, sizeof(indexFileBuffer) );
	memset(indexFileNumStr, 0, sizeof(indexFileNumStr) );	
	indx_fp = fopen("/home/pi/projects/Logs/OuthubLogIndex.txt", "r"); //"r+");//LogIndexFile, "r+"); 		
	if(indx_fp) {printf("\nIndex File Opened For Reading\n"); fflush(stdout);}
	else {printf("\nIndex File Failed to Open\n"); fflush(stdout);}		
				
	fgets(indexFileBuffer, INDEX_FILE_CONTENT_LEN, indx_fp); 
	printf("\nIndex File Contents: %s\n", indexFileBuffer);// Log Files Index Vector:
					
	if(strncmp(indexFileBuffer, "Log Files Index Vector:", 22) == 0) //LogFile Ascii Table Number:", 26) == 0)
		{
		printf("\nIndex File Unique ID String is a Match\n");
		for(int i=23; i<INDEX_FILE_CONTENT_LEN; i++)
			{
			indexFileNumStr[(i-23)] = indexFileBuffer[i];
			}	
		printf("\nIndex File Number Cmpnt is: %s\n", indexFileNumStr);
		
		logIndxFileVect = atoi(indexFileNumStr);
						
		printf("\nATOI Output: %u\n", logIndexNumber);
		fflush(stdout); 
		}
	else
		{
		printf("\nIndex File Unique ID String Does Not Match\n"); 
		fflush(stdout);     
		}		
		
	fclose(indx_fp);	
			
	lgIndxFileVectShadow = logIndxFileVect;
		
	if(++lgIndxFileVectShadow > 999)
		lgIndxFileVectShadow = 1;
	
	printf("\n\nLog Index File Opened, Log File Vector Now Points to Log File Number: %d\n\n", logIndxFileVect);
	fflush(stdout);
	
	memset(indexFileNumStr, 0, sizeof(indexFileNumStr) );
	memset(indexFileBuffer, 0, sizeof(indexFileBuffer) );	
	sprintf(indexFileNumStr, "%d", lgIndxFileVectShadow);	
	strcpy(indexFileBuffer, "Log Files Index Vector:");
	strcat(indexFileBuffer, indexFileNumStr);
	
	; //***// printf("\n\nSent from Inside File Handler: %s   , %d\n\n", indexFileBuffer, lgIndxFileVectShadow);	
			
	indx_fp = fopen("/home/pi/projects/Logs/OuthubLogIndex.txt", "w"); //"r+");//LogIndexFile, "r+"); 		
	if(indx_fp) 
		{
		printf("\nIndex File Opened For Reading\n"); fflush(stdout);
		fprintf(indx_fp, "%s", indexFileBuffer);
		}
	else {printf("\nIndex File Failed to Open\n"); fflush(stdout);}			
		
	fclose(indx_fp);

	return logIndxFileVect;
}

int messageSubType(char * messageInformation)
{
	int trapTypeOutput;
	trapTypeOutput = 1;
								
							//printf("\n\nContents of Error Message Test String:%s\n\n", errorMessageTest);	
							//printf("NoDatastrncmpRtnVal: %d", strncmp(errorMessageTest, NoDataStr, 6) );							
							//printf("MuxstrncmpRtnVal: %d", strncmp(errorMessageTest, MuxStr, 3) );								
							
							if( (strncmp(messageInformation, PortC_Str, 5) == 0) )
								{
								printf("\n\nPort C Trap Exectuted\n\n");									
								return 2; //trapTypeOutput = 2;
								}
							if( (strncmp(messageInformation, PortD_Str, 5) == 0) )
								{
								printf("\n\nPort D Trap Exectuted\n\n");									
								return 2; //trapTypeOutput = 2;
								}	
							if( (strncmp(messageInformation, NoDataStr, 6) == 0) )
								{
								printf("\n\nNo Data Trap Exectuted\n\n");
								return 2; //trapTypeOutput = 2;;
								}									
							if( (strncmp(messageInformation, MuxStr, 3) == 0) )
								{
								printf("\n\nMux Some-Number Cycled, Data Trap Exectuted\n\n");									
								return 2; //trapTypeOutput = 2;
								}	
							if( (strncmp(messageInformation, BusDataError, 15) == 0) )
								{
								printf("\n\nBus Data Error Trap Executed\n\n");
								return 2; //trapTypeOutput = 2;;
								}	
							if( (strncmp(messageInformation, FacilityClosed, 14) == 0) )
								{
								printf("\n\nFacility Closing, Take Appropriate Actions\n\n");
								return 3; //rapTypeOutput = 3;;
								}									
								
							/////////memset(errorMessageTest, 0, sizeof(errorMessageTest) );	
	
	fflush(stdout);
	
	
	
	
	
	return trapTypeOutput;;
}

int writeLogFiles(char *messageTraffic, int fileIndexVect)
{
	char fileNumStr[10];
	char completeFileIdStr[48];
	
	memset(fileNumStr, 0, sizeof(fileNumStr) );
	memset(completeFileIdStr, 0, sizeof(completeFileIdStr) );
	sprintf(fileNumStr, "%d", fileIndexVect);
	strcpy(completeFileIdStr, "/home/pi/projects/Logs/ActivityLogNum");
	strcat(completeFileIdStr, fileNumStr);
	strcat(completeFileIdStr, ".txt");
		
	logFiles = fopen(completeFileIdStr, "a"); //"r+");
	if(logFiles) {printf("\nILog File Opened For Appending\n"); fflush(stdout);}
	else {printf("\nlog File Failed to Open\n"); fflush(stdout);}		
	
	fprintf(logFiles, "%s\n", messageTraffic);
	
	fclose(logFiles);
	
	return 0;
}

int outputFunctions(char *messageTraffic, long type, int *linePosition, int *outMode, int *fileIndexVect)
{
	bool skipAnnouncement, endDaysActivity;	
	int mesSubType;	
	static int anncmntTimer;
	skipAnnouncement = false;
	endDaysActivity = false;

	++anncmntTimer; // Not Much of a Timer, Only Increments Once / Pass Through Parser	
	mesSubType = messageSubType(messageTraffic);
	//if( (mesSubType = (messageSubType(messageTraffic) == 1) ) )

	if(type == Stats_Updated_1)
		{
		skipAnnouncement = true;
		if(anncmntTimer > 5)
			{
			anncmntTimer= 0;
			skipAnnouncement = false;
			}
		}
	switch(mesSubType)
		{
		case 1 :
			{
			printf("\n\nStandard Message Traffice, Process as a Normal Message\n\n"); 
			fflush(stdout);
			}	
		break;
		case 2 :
			{
			printf("\n\nMessage Sub-Type == skip Announcement\n\n");
			fflush(stdout);
			skipAnnouncement = true;
			}			
		break;
		case 3 :
			{
			printf("\n\nMessage Sub-Type == End Days Activity\n\n");
			fflush(stdout);
			endDaysActivity = true;
			}				
		break;
		default : break;
		}
	
	switch(*outMode)
		{
		case PostInitPageHander :
		
			*fileIndexVect = indexFileHander();
			
			wav = playSound( (int)type);
			fprintf(Printer, "  %s\n\r", messageTraffic);				
			fflush(Printer);
			*linePosition = FIRST_PAGE_OFFSET;	
			*outMode = (int)Std_Output_Routine;	
		break;
		case Std_Output_Routine :
			if(skipAnnouncement == false)
				wav = playSound( (int)type);
			fprintf(Printer, "  %s\n\r", messageTraffic);				
			fflush(Printer);
			writeLogFiles(messageTraffic, (*fileIndexVect) );
			if(endDaysActivity == true)
				*outMode = (int)New_Page;
			if( ++(*linePosition) > MAX_LINES_PER_PAGE )
				*outMode = (int)New_Page;
		break;		
		case New_Page :
		writeLogFiles(stdPrinterHdrBuf, (*fileIndexVect) );
		
		*fileIndexVect = indexFileHander();

		fprintf(Printer, "\f");
		fprintf(Printer, "\n\r  %s\n\r\n\r", stdPrinterHdrBuf);				
		fflush(Printer);	
		*linePosition = STD_PAGE_OFFSET; // Position of the Line Number Vector in this Instruction List is ONLY SYMBOLIC
		if(skipAnnouncement == false)
			wav = playSound( (int)type);
		fprintf(Printer, "  %s\n\r", messageTraffic);				
		fflush(Printer);
		*outMode = (int)Std_Output_Routine;			
		break;			
		case Std_Output_Header :
		break;			
		default : {*outMode = PostInitPageHander;}break; // Write Close I/O Streams Here(?)
		}	
	return 0;
}
