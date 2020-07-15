//---------------- pi / Network OutputHub Shared / Global Definitions + Variables ---------------//
//**************************** Global Defined Program Components *****************************//
// Shared Across Functions Contained in Both main and Project-Specific (Prj-Exclusive) Libraries // 

//#define MAX_BUFFER 256
#define SMSIZE 256	// Shared memory size
#define BUFSIZE 1024
#define LOCAL_PORT 1234
#define SERVER_PORT 1235

#define MAX_UDP_INPUT_STREAM 512
#define MAX_BUFFERED_SHARED_MEMORY 20
#define FIXED_HDR_CMPNTS_LEN 50 // Used to Establish the Base for Dynamic Vector to Perform Str (Data-Word) Substitution(s)
#define MAX_DATA_IMPORTED 256
#define MAX_DATA_LOG_ACT_LEN 128
#define MAX_OUTPUT_STR_LEN 80 //84 	// Do Not Send More Than (80) "VISABLE / PRINTABLE"
#define INDEX_FILE_CONTENT_LEN 32

// Printer Driving and Activity Defines + Logging Maximum Values and Index Limits 
#define MAX_LINES_PER_PAGE 60
#define FIRST_PAGE_OFFSET 8
#define STD_PAGE_OFFSET 4
#define MAX_BUFFERED_PAGE_LINES (MAX_LINES_PER_PAGE + (10) )

int wav;
int anncmntType;
//int anncmntCounter;
int logIndexNumber;

enum Message_Data_Types {Help_1=65, Entry, Exit, Opened, Closed, Alarm_1, 
Catch, Heat_Sensor, Stats_Updated_1, Header_Cmpnts, Blank_1, Denied_Access, Alarm_2, blank, BootAnncmnt, Help_2}; 

enum Output_Handers {PostInitPageHander=1, Std_Output_Routine, New_Page, Std_Output_Header};

int previousMessageDataType, messageDataCounter;

char IndexFileNameStr[] = {"/var/log/OuthubLog/OuthubLogIndex.txt"}; // Fixed Index File Location
char IndexFileConstStr[] = {"Last Closed Log-File Number:"}; // Reference String

char DefaultHeaderString[] = {"        DERRELS mini STORAGE INC, FACILITY NUMBER 99, 01-01-2099  "};

char PrinterMaintenanceString[] = {"     -------- PRINTER MAINTENANCE TRANSMISSION --------        "};

// ---- (Errata) Initialized String Below Contains Const(Var-Name) Reference, But is Modified by The Program ---- //
char InitHeadConstStr[][MAX_OUTPUT_STR_LEN] = {"        BOOTING & INITIALIZING OUTPUT-HUB(IPC), VERSION: 2.2.2.1",
" WHEN SYSTEM CTRL'R DATA IS RCV'D, HEADER FACILITY_NUMBER & DATE WILL REFRESH",
"IF THE HEADER FIELDS DO NOT UPDATE APPROPRIATELY, VERIFY NETWORK FUNCTIONALITY"};

char NoDataStr[] = {"No Data"};
char MuxStr[] = {"Mux:"};
char PortC_Str[] = {"Port C"};
char PortD_Str[] = {"Port D"};
char BusDataError[] = {"Invalid Data Set"};
char FacilityClosed[] = {"Facility closed"};

//char fileNameDateCmpnt[10];

//------------------------ Socket Variables --------------------------//
int sock, numbytes;
struct sockaddr_in myaddr, theiraddr;
socklen_t addr_len;
char sockData[BUFSIZ];
//-------------------------Arrays, Structs and Ptr Variables ---------------------------//
FILE *Printer;
FILE *indx_fp;
FILE *logFiles;

char indexFileContentStr[INDEX_FILE_CONTENT_LEN+2];	

char bufferedSharedMemory[MAX_BUFFERED_SHARED_MEMORY];
char scratchRamBuffer[128];
char sharedMemoryRamBuffer[MAX_BUFFERED_SHARED_MEMORY];
char transferBuffer[MAX_OUTPUT_STR_LEN];
//char importedData[MAX_DATA_IMPORTED];
char stdPrinterHdrBuf[MAX_OUTPUT_STR_LEN];

char loggedData[INDEX_FILE_CONTENT_LEN][MAX_OUTPUT_STR_LEN];



