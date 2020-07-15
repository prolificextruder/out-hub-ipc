int playSound(int wavSelection)
	{	
	int status = 0;
	char command[64];	
	memset(command, 0, sizeof(command) );
	
	; //***// printf("\n\nMissing Announcement: %d\n\n", wavSelection);
	switch(wavSelection)
		{
		case Help_1:
		sprintf(command, "aplay %s", "/var/lib/OutputHub/help1.wav");
		status = system(command);
		break;
		
		case Entry:
		;
		break;
		
		case Exit:
		;
		break;
		
		case Opened:
		;
		break;
		
		case Closed:
		;
		break;
		
		case Alarm_1:
		sprintf(command, "aplay %s",  "/var/lib/OutputHub/alarmhrn.wav");
		status = system(command);		
		break;
		
		case Catch:
		sprintf(command, "aplay %s", "/var/lib/OutputHub/catch1.wav");
		status = system(command);
		break;
		
		case Heat_Sensor:
		sprintf(command, "aplay %s", "/var/lib/OutputHub/heat3.wav");
		status = system(command);
		break;
		
		case Stats_Updated_1:
		sprintf(command, "aplay %s", "/var/lib/OutputHub/timestatusupdate.wav");
		status = system(command);
		//closeFileFlag = true;
		break;	
		
		case Header_Cmpnts:
		;
		break;	
		
		case Blank_1:
		;
		break;
		
		case Denied_Access:
		sprintf(command, "aplay %s",  "/var/lib/OutputHub/denied2.wav");
		status = system(command);
		break;
		
		case blank :
		sprintf(command, "aplay %s",  "/var/lib/OutputHub/alarmhrn.wav");
		status = system(command);
		break;
		
		case Alarm_2:
		sprintf(command, "aplay %s",  "/var/lib/OutputHub/alarmhrn.wav");
		status = system(command);
		break;			
	
		case BootAnncmnt:
		sprintf(command, "aplay %s",  "/var/lib/OutputHub/bootannouncement.wav");
		status = system(command);			
		break;	
		
		default:
		//sprintf(command, "aplay %s", "/var/lib/OutputHub/help1.wav");
		//status = system(command);
		break;
		}
	printf("\n\nSystem Call Return Value: %d\n\n", status);
	return 1;
	}

